#include "caleval_class.h"
#include <fstream>
#include <stdio.h>      /* printf */
#include <stdlib.h>     /* qsort */
#include <iomanip>

Int_t map_vpd_trg2tof_west[16] = 
	{  0,  1,  2,  3,
	   6,  7,  8, 15,
	  10,  9, 12,  4,
	  11, 16, 17, 18 };
Int_t map_vpd_trg2tof_east[16] = 
	{  0,  1, 16,  3,
	   6,  7,  8,  9,
	  10, 12, 11,  4,
	  15,  2, 17, 18 };
	  
const int NDEAD	=   1;
int DEAD[NDEAD]	= {10};		// idetew (this is West11 numbering from 1) Run-14
	  
Int_t map_vpd_tof2trg_west[19];
Int_t map_vpd_tof2trg_east[19];

bool DoFitSliceY(TH2D*);
void FitGaussian(TH1D*,TF1*,int);
void Norm2D(TH1D*, TH2D*, TH2D*);

bool	exclude;
double	exclude_lower,exclude_upper;
Double_t mypoly(Double_t *x, Double_t *par){
	if (exclude && (x[0]>exclude_lower&&x[0]<exclude_upper)){
		TF1::RejectPoint();
		return 0;
	}
	return par[0] + par[1]*x[0] + par[2]*x[0]*x[0];
}
Double_t mygaus(Double_t *x, Double_t *par){
	if (exclude && (x[0]>exclude_lower&&x[0]<exclude_upper)){
		TF1::RejectPoint();
		return 0;
	}
	return par[0]*exp(-0.5*TMath::Power((x[0]-par[1])/par[2],2));
}

int floatcomp(const void* elem1, const void* elem2){
    if(*(const float*)elem1 < *(const float*)elem2)
        return -1;
    return *(const float*)elem1 > *(const float*)elem2;
}

//=======================================================================================
//
void caleval::Loop(Int_t ntodo){
//
//	choice=0 ToT tLE
//	choice=1 ADC TAC
//	choice=2 ADC tLE
//	choice=3 ToT TAC
//
//	if (iChoice!=0&&iChoice!=1){ cout<<"iChoice error"<<endl; exit(0); }
	if (iChoice==0 || iChoice==2){ tsign	=  1.0; } else 	// time from tLE
	if (iChoice==1 || iChoice==3){ tsign	= -1.0; }		// time from TAC
	tfactor	= tsign*29.9792458/2.;

	bool	IsDAQ	= false;
	bool	IsFO	= false;
#ifdef muapp
	IsFO	= true;
	cout <<".... Loop Starting MUAPP......... "<<(int)IsFO<<" "<<(int)IsDAQ<<endl;
#else
	IsDAQ	= true;
	cout <<".... Loop Starting DAQDOER....... "<<(int)IsFO<<" "<<(int)IsDAQ<<endl;
#endif

	char		buf[100],tbuf[100];
	TCanvas 	*ccan[1000];
	TLatex		*text[1000];
	TH1F		*frame[100];
	Int_t		ican=-1,itext=-1,iframe=-1;
	for (int i=0;i<1000;i++){
		text[i]	= new TLatex();
		text[i]->SetNDC();
		text[i]->SetTextSize(0.06);
	}

	float ZVertexMax	= 50.;
	float ZVertexMaxDT	= 20.;
	cout<<".... iRhicRun        = "<<iRhicRun<<endl;
	cout<<".... iTrgCrate       = "<<iTrgCrate<<endl;	
	cout<<".... ZVertexMax      = "<<ZVertexMax<<endl;
	cout<<".... Nprimarycut     = "<<Nprimarycut<<endl;
	cout<<".... ADC Lower Limit = "<<ADCminimum<<endl;
	cout<<".... iAlgorithm      = "<<iAlgorithm<<endl; 
	cout<<".... NDETSIDE        = "<<NDETSIDE<<endl; 
	cout<<".... iChoice         = "<<iChoice<<endl; 
	cout<<".... TACtoNS         = "<<TACtoNS<<endl; 
	cout<<".... tfactor         = "<<tfactor<<endl; 
	
	//---- set file names...
	//
	if (iRhicRun==14 && iEnergy==203){ sprintf(runstring,"run14auhe200"); } else
	if (iRhicRun==14 && iEnergy== 15){ sprintf(runstring,"run14auau015"); } else
	if (iRhicRun==14 && iEnergy==200){ sprintf(runstring,"run14auau200"); } else
	if (iRhicRun==13 && iEnergy==500){ sprintf(runstring,"run13pp500");   } else
	if (iRhicRun==10 && iEnergy==200){ sprintf(runstring,"run10auau200"); } else
	if (iRhicRun==10 && iEnergy== 62){ sprintf(runstring,"run10auau062"); } else
	if (iRhicRun==10 && iEnergy== 39){ sprintf(runstring,"run10auau039"); } else
	if (iRhicRun==10 && iEnergy==  7){ sprintf(runstring,"run10auau007"); } 	
	TString base	= TString(Form("caleval_%s_ic%d",runstring,iChoice));
	TString calbase	= TString(Form("../cal/root/collapse_cal_%s_ic%d_ia%d",runstring,iChoice,iAlgorithm));
	if (iChoice!=0&&iTrgCrate==0){						// uses BBQ
		base 		+= TString("_bbq");
		calbase 	+= TString("_bbq");
	} else if (iChoice!=0&&iTrgCrate==1){				// uses MXQ
		base 		+= TString("_mxq");
		calbase 	+= TString("_mxq");
	}
	cout<<"..... utils      runstring = "<<runstring<<endl;
	cout<<"..... utils           base = "<<base<<endl;
	//
	cout<<".... filename base   = "<<base<<endl; 
	TString rootfile,psfile,psfileO,psfileC,pdffile,calfile;
		rootfile	= TString("./root/") + base + TString(".root");
		psfile		= TString("./ps/")   + base + TString(".ps");
		psfileO		= TString("./ps/")   + base + TString(".ps(");
		psfileC		= TString("./ps/")   + base + TString(".ps]");
		pdffile		= TString("./ps/")   + base + TString(".pdf");
		calfile		= calbase + TString(".txt");
	cout<<"....        rootfile = "<<rootfile<<endl; 
	cout<<"....        psfile   = "<<psfile<<endl; 
	cout<<"....        psfileO  = "<<psfileO<<endl; 
	cout<<"....        psfileC  = "<<psfileC<<endl; 
	cout<<"....        pdffile  = "<<pdffile<<endl; 
	cout<<"....        calfile  = "<<calfile<<endl; 
	
	//---- read calibration data.... 
	cout<<".... Loop   Read Cal Data.... iChoice="<<iChoice<<" iAlgorithm="<<iAlgorithm<<endl;
	cout<<".... Loop        calfile = "<<calfile.Data()<<endl;
	ifstream textin;	textin.open(calfile.Data(), ifstream::in);
	int 	ind,npts;
	float	aval;
	for (int is=0;is<2;is++){
		for (int idet=0;idet<NDETSIDE;idet++){
			int idetew	= idet + is*NDETSIDE;
			textin>>ind;
			textin>>npts;
			cout<<npts<<endl;
			for (int ib=0;ib<npts;ib++){
				textin>>aval;
				binxlow[ib][idetew]	= aval;
			}
			for (int ib=0;ib<npts;ib++){
				textin>>aval;
				bincorr[ib][idetew]	= aval;
				if (is==0&&idet==0&&ib<3) cout<<is<<" "<<idet<<" "<<idetew<<"\t"<<binxlow[ib][idetew]<<" "<<bincorr[ib][idetew]<<endl;
				if (is==1&&idet==0&&ib<3) cout<<is<<" "<<idet<<" "<<idetew<<"\t"<<binxlow[ib][idetew]<<" "<<bincorr[ib][idetew]<<endl;
			}
		}
	}
	textin.close();
	for (int is=0;is<2;is++){
		for (int idet=0;idet<NDETSIDE;idet++){
			int idetew		= idet + is*NDETSIDE;
			fcorr[idetew]	= new TGraph(); fcorr[idetew]->SetLineColor(1);  fcorr[idetew]->SetLineWidth(2);  
			for (int ib=0;ib<npts-1;ib++){
				float aval	= (binxlow[ib][idetew] + binxlow[ib+1][idetew])/2.0;
				float corr	=  bincorr[ib][idetew];
				if (corr!=0){
					int k	= fcorr[idetew]->GetN();
					fcorr[idetew]->SetPoint(k,aval,corr);
				}
			}
			cout<<is<<" "<<idet<<" "<<idetew<<"\t"<<fcorr[idetew]->Eval(20)<<endl;
			//
			fcorrA[idetew]	= new TGraph();	fcorrA[idetew]->SetLineColor(2); fcorrA[idetew]->SetLineWidth(2); 
			fcorrB[idetew]	= new TGraph();	fcorrB[idetew]->SetLineColor(6); fcorrB[idetew]->SetLineWidth(2); 
			fcorrC[idetew]	= new TGraph();	fcorrC[idetew]->SetLineColor(4); fcorrC[idetew]->SetLineWidth(2); 
			//
		}
	}
	//
	//---- pre-tock data needs a large E/W offset... (adds to east) only used for raw_ch0ch0
	CrudeEWoffset = 0;
	if ((iChoice==0||iChoice==2) && iRhicRun==10 && iEnergy==  7){ CrudeEWoffset = -163.0; }
	if ((iChoice==0||iChoice==2) && iRhicRun==10 && iEnergy== 39){ CrudeEWoffset = -163.0; }
	if ((iChoice==0||iChoice==2) && iRhicRun==10 && iEnergy== 62){ CrudeEWoffset = -163.0; }
	if ((iChoice==0||iChoice==2) && iRhicRun==10 && iEnergy==200){ CrudeEWoffset = -163.0; }
	//
	if ((iChoice==0||iChoice==2) && iRhicRun==14 && iEnergy==200){ CrudeEWoffset = -163.0; }
	if ((iChoice==0||iChoice==2) && iRhicRun==14 && iEnergy== 15){ CrudeEWoffset =   -3.0; }
//	if ((iChoice==0||iChoice==2) && iRhicRun==13 && iEnergy==500){ CrudeEWoffset =   -3.0; }
	//
//	if ((iChoice==1||iChoice==3) && iRhicRun==10 && iEnergy==  7){ CrudeEWoffset =   -2.9; }
//	if ((iChoice==1||iChoice==3) && iRhicRun==10 && iEnergy== 39){ CrudeEWoffset =   -2.9; }
//	if ((iChoice==1||iChoice==3) && iRhicRun==10 && iEnergy== 62){ CrudeEWoffset =   -3.2; }
//	if ((iChoice==1||iChoice==3) && iRhicRun==10 && iEnergy==200){ CrudeEWoffset =   -0.9; }
//	if ((iChoice==1||iChoice==3) && iRhicRun==13 && iEnergy==500){ CrudeEWoffset =    0.5; }
	//
	cout<<".... Loop   Read Cal Data complete..."<<endl;
		
	//---- book...
	//
	fout = new TFile(rootfile.Data(),"RECREATE");
	fout->cd();
	//
	TH1D *hcoco_vpd_w		= new TH1D("hcoco_vpd_w","hcoco_vpd_w",256,-25.5,230.5);
	TH1D *hcoco_vpd_e		= new TH1D("hcoco_vpd_e","hcoco_vpd_e",256,-25.5,230.5);
	TH1D *hnev_daqbits		= new TH1D("hnev_daqbits","hnev_daqbits",64,-0.5,63.5);
	TH1D *hnev_daqbits_keep	= new TH1D("hnev_daqbits_keep","hnev_daqbits_keep",64,-0.5,63.5);
	TH1D *hrefmult			= new TH1D("hrefmult","hrefmult",130,0,650);
	TH1D *hrefmult_peri		= new TH1D("hrefmult_peri","hrefmult_peri",130,0,650);
	TH1D *hrefmult_cent		= new TH1D("hrefmult_cent","hrefmult_cent",130,0,650);
		hrefmult->SetLineWidth(2);
		hrefmult->SetLineColor(1);
		hrefmult_peri->SetFillColor(4);
		hrefmult_cent->SetFillColor(2);
	//
	for (int is=0;is<2;is++){
		for (int idet=0;idet<NDETSIDE;idet++){
			int idetew			= idet + is*NDETSIDE;
			if (iChoice==0||iChoice==3){ A_nbin= 128; A_min= 0  ; A_max=  64. ; } else
			if (iChoice==1||iChoice==2){ A_nbin=4096; A_min=-0.5; A_max=4095.5; }
			sprintf(buf,"hA%d",idetew);
			hA[idetew]	= new TH1D(buf,buf,A_nbin,A_min,A_max);
			hA[idetew]->SetFillColor(3);
			sprintf(buf,"hA_even%d",idetew);
			hA_even[idetew]	= new TH1D(buf,buf,A_nbin,A_min,A_max);
			hA_even[idetew]->SetFillColor(5);
			sprintf(buf,"hA_odd%d",idetew);
			hA_odd[idetew]	= new TH1D(buf,buf,A_nbin,A_min,A_max);
			hA_odd[idetew]->SetFillColor(7);
		}
	}	
	cout<<"A_nbin = "<<A_nbin<<endl;
	cout<<"A_min  = "<<A_min<<endl;
	cout<<"A_max  = "<<A_max<<endl;
	//
	TH2D *hzz_rawch0ch0				= new TH2D("hzz_rawch0ch0","hzz_rawch0ch0",50,-50,50,400,-200,200);
	TH1D *hdzz_rawch0ch0			= new TH1D("hdzz_rawch0ch0","hdzz_rawch0ch0",        400,-50,50);
	//TH2D *hdzz_rawch0ch0_nprimary	= new TH2D("hdzz_rawch0ch0_nprimary","hdzz_rawch0ch0_nprimary",200,0.0,400,400,-200,200);
	//TH2D *hdzz_rawch0ch0_nglobal	= new TH2D("hdzz_rawch0ch0_nglobal","hdzz_rawch0ch0_nglobal",200,0.0,4000,400,-200,200);
	//TH2D *hdzz_rawch0ch0_nvert		= new TH2D("hdzz_rawch0ch0_nvert","hdzz_rawch0ch0_nvert",40,-0.5,39.5,400,-200,200);
	TH1D *hzz_rawch0ch0_1;
	TH1D *hzz_rawch0ch0_2;
	TF1  *fzz_rawch0ch0_1	= new TF1("fzz_rawch0ch0_1","pol1",-15,15);
			fzz_rawch0ch0_1->SetLineColor(2);
			fzz_rawch0ch0_1->SetLineWidth(1);
	TF1 *fgau_rawch0ch0		= new TF1("fgau_rawch0ch0","gaus",-100,100);	fgau_rawch0ch0->SetLineColor(2);
	TH2D *hzz_rawch0ch0_peri				= new TH2D("hzz_rawch0ch0_peri","hzz_rawch0ch0_peri",50,-50,50,400,-200,200);
	TH1D *hdzz_rawch0ch0_peri			= new TH1D("hdzz_rawch0ch0_peri","hdzz_rawch0ch0_peri",        400,-50,50);
	//TH2D *hdzz_rawch0ch0_peri_nprimary	= new TH2D("hdzz_rawch0ch0_peri_nprimary","hdzz_rawch0ch0_peri_nprimary",200,0.0,400,400,-200,200);
	//TH2D *hdzz_rawch0ch0_peri_nglobal	= new TH2D("hdzz_rawch0ch0_peri_nglobal","hdzz_rawch0ch0_peri_nglobal",200,0.0,4000,400,-200,200);
	//TH2D *hdzz_rawch0ch0_peri_nvert		= new TH2D("hdzz_rawch0ch0_peri_nvert","hdzz_rawch0ch0_peri_nvert",40,-0.5,39.5,400,-200,200);
	TH1D *hzz_rawch0ch0_peri_1;
	TH1D *hzz_rawch0ch0_peri_2;
	TF1  *fzz_rawch0ch0_peri_1	= new TF1("fzz_rawch0ch0_peri_1","pol1",-15,15);
			fzz_rawch0ch0_peri_1->SetLineColor(2);
			fzz_rawch0ch0_peri_1->SetLineWidth(1);
	TF1 *fgau_rawch0ch0_peri		= new TF1("fgau_rawch0ch0_peri","gaus",-100,100);	fgau_rawch0ch0_peri->SetLineColor(2);
	TH2D *hzz_rawch0ch0_cent				= new TH2D("hzz_rawch0ch0_cent","hzz_rawch0ch0_cent",50,-50,50,400,-200,200);
	TH1D *hdzz_rawch0ch0_cent			= new TH1D("hdzz_rawch0ch0_cent","hdzz_rawch0ch0_cent",        400,-50,50);
	//TH2D *hdzz_rawch0ch0_cent_nprimary	= new TH2D("hdzz_rawch0ch0_cent_nprimary","hdzz_rawch0ch0_cent_nprimary",200,0.0,400,400,-200,200);
	//TH2D *hdzz_rawch0ch0_cent_nglobal	= new TH2D("hdzz_rawch0ch0_cent_nglobal","hdzz_rawch0ch0_cent_nglobal",200,0.0,4000,400,-200,200);
	//TH2D *hdzz_rawch0ch0_cent_nvert		= new TH2D("hdzz_rawch0ch0_cent_nvert","hdzz_rawch0ch0_cent_nvert",40,-0.5,39.5,400,-200,200);
	TH1D *hzz_rawch0ch0_cent_1;
	TH1D *hzz_rawch0ch0_cent_2;
	TF1  *fzz_rawch0ch0_cent_1	= new TF1("fzz_rawch0ch0_cent_1","pol1",-15,15);
			fzz_rawch0ch0_cent_1->SetLineColor(2);
			fzz_rawch0ch0_cent_1->SetLineWidth(1);
	TF1 *fgau_rawch0ch0_cent		= new TF1("fgau_rawch0ch0_cent","gaus",-100,100);	fgau_rawch0ch0_cent->SetLineColor(2);

 	TH2D *hzz_raw11			= new TH2D("hzz_raw11","hzz_raw11",50,-50,50,400,-200,200);
 	TH1D *hdzz_raw11		= new TH1D("hdzz_raw11","hdzz_raw11",        400,-50,50);
 	TH1D *hzz_raw11_1;
 	TH1D *hzz_raw11_2;
 	TF1  *fzz_raw11_1		= new TF1("fzz_raw11_1","pol1",-15,15);
 			fzz_raw11_1->SetLineColor(2);
 			fzz_raw11_1->SetLineWidth(1);
	TF1 *fgau_raw11			= new TF1("fgau_raw11","gaus",-100,100);	fgau_raw11->SetLineColor(2);
 	TH2D *hzz_raw11_peri			= new TH2D("hzz_raw11_peri","hzz_raw11_peri",50,-50,50,400,-200,200);
 	TH1D *hdzz_raw11_peri		= new TH1D("hdzz_raw11_peri","hdzz_raw11_peri",        400,-50,50);
 	TH1D *hzz_raw11_peri_1;
 	TH1D *hzz_raw11_peri_2;
 	TF1  *fzz_raw11_peri_1		= new TF1("fzz_raw11_peri_1","pol1",-15,15);
 			fzz_raw11_peri_1->SetLineColor(2);
 			fzz_raw11_peri_1->SetLineWidth(1);
	TF1 *fgau_raw11_peri			= new TF1("fgau_raw11_peri","gaus",-100,100);	fgau_raw11_peri->SetLineColor(2);
 	TH2D *hzz_raw11_cent			= new TH2D("hzz_raw11_cent","hzz_raw11_cent",50,-50,50,400,-200,200);
 	TH1D *hdzz_raw11_cent		= new TH1D("hdzz_raw11_cent","hdzz_raw11_cent",        400,-50,50);
 	TH1D *hzz_raw11_cent_1;
 	TH1D *hzz_raw11_cent_2;
 	TF1  *fzz_raw11_cent_1		= new TF1("fzz_raw11_cent_1","pol1",-15,15);
 			fzz_raw11_cent_1->SetLineColor(2);
 			fzz_raw11_cent_1->SetLineWidth(1);
	TF1 *fgau_raw11_cent			= new TF1("fgau_raw11_cent","gaus",-100,100);	fgau_raw11_cent->SetLineColor(2);

	TH2D *hzz_cal11			= new TH2D("hzz_cal11","hzz_cal11",50,-50,50,400,-200,200);
	TH1D *hdzz_cal11		= new TH1D("hdzz_cal11","hdzz_cal11",        400,-50,50);
	TH1D *hzz_cal11_1;
	TH1D *hzz_cal11_2;
	TF1  *fzz_cal11_1		= new TF1("fzz_cal11_1","pol1",-15,15);
			fzz_cal11_1->SetLineColor(2);
			fzz_cal11_1->SetLineWidth(1);
	TF1 *fgau_cal11			= new TF1("fgau_cal11","gaus",-100,100);	fgau_cal11->SetLineColor(2);
	TH2D *hzz_cal11_peri			= new TH2D("hzz_cal11_peri","hzz_cal11_peri",50,-50,50,400,-200,200);
	TH1D *hdzz_cal11_peri		= new TH1D("hdzz_cal11_peri","hdzz_cal11_peri",        400,-50,50);
	TH1D *hzz_cal11_peri_1;
	TH1D *hzz_cal11_peri_2;
	TF1  *fzz_cal11_peri_1		= new TF1("fzz_cal11_peri_1","pol1",-15,15);
			fzz_cal11_peri_1->SetLineColor(2);
			fzz_cal11_peri_1->SetLineWidth(1);
	TF1 *fgau_cal11_peri			= new TF1("fgau_cal11_peri","gaus",-100,100);	fgau_cal11_peri->SetLineColor(2);
	TH2D *hzz_cal11_cent			= new TH2D("hzz_cal11_cent","hzz_cal11_cent",50,-50,50,400,-200,200);
	TH1D *hdzz_cal11_cent		= new TH1D("hdzz_cal11_cent","hdzz_cal11_cent",        400,-50,50);
	TH1D *hzz_cal11_cent_1;
	TH1D *hzz_cal11_cent_2;
	TF1  *fzz_cal11_cent_1		= new TF1("fzz_cal11_cent_1","pol1",-15,15);
			fzz_cal11_cent_1->SetLineColor(2);
			fzz_cal11_cent_1->SetLineWidth(1);
	TF1 *fgau_cal11_cent			= new TF1("fgau_cal11_cent","gaus",-100,100);	fgau_cal11_cent->SetLineColor(2);

	TH2D *hzz_calavg		= new TH2D("hzz_calavg","hzz_calavg",50,-50,50,400,-200,200);
	TH1D *hdzz_calavg		= new TH1D("hdzz_calavg","hdzz_calavg",        400,-50,50);
	TH1D *hzz_calavg_1;
	TH1D *hzz_calavg_2;
	TF1  *fzz_calavg_1		= new TF1("fzz_calavg_1","pol1",-15,15);
			fzz_calavg_1->SetLineColor(2);
			fzz_calavg_1->SetLineWidth(1);
	TF1 *fgau_calavg		= new TF1("fgau_calavg","gaus",-100,100);	fgau_calavg->SetLineColor(2);
	TH2D *hzz_calavg_peri		= new TH2D("hzz_calavg_peri","hzz_calavg_peri",50,-50,50,400,-200,200);
	TH1D *hdzz_calavg_peri		= new TH1D("hdzz_calavg_peri","hdzz_calavg_peri",        400,-50,50);
	TH1D *hzz_calavg_peri_1;
	TH1D *hzz_calavg_peri_2;
	TF1  *fzz_calavg_peri_1		= new TF1("fzz_calavg_peri_1","pol1",-15,15);
			fzz_calavg_peri_1->SetLineColor(2);
			fzz_calavg_peri_1->SetLineWidth(1);
	TF1 *fgau_calavg_peri		= new TF1("fgau_calavg_peri","gaus",-100,100);	fgau_calavg_peri->SetLineColor(2);
	TH2D *hzz_calavg_cent		= new TH2D("hzz_calavg_cent","hzz_calavg_cent",50,-50,50,400,-200,200);
	TH1D *hdzz_calavg_cent		= new TH1D("hdzz_calavg_cent","hdzz_calavg_cent",        400,-50,50);
	TH1D *hzz_calavg_cent_1;
	TH1D *hzz_calavg_cent_2;
	TF1  *fzz_calavg_cent_1		= new TF1("fzz_calavg_cent_1","pol1",-15,15);
			fzz_calavg_cent_1->SetLineColor(2);
			fzz_calavg_cent_1->SetLineWidth(1);
	TF1 *fgau_calavg_cent		= new TF1("fgau_calavg_cent","gaus",-100,100);	fgau_calavg_cent->SetLineColor(2);

	TH2D *hzz_cal11A		= new TH2D("hzz_cal11A","hzz_cal11A",50,-50,50,400,-200,200);
	TH1D *hdzz_cal11A		= new TH1D("hdzz_cal11A","hdzz_cal11A",        400,-50,50);
	TH1D *hzz_cal11A_1;
	TH1D *hzz_cal11A_2;
	TF1  *fzz_cal11A_1		= new TF1("fzz_cal11A_1","pol1",-15,15);
			fzz_cal11A_1->SetLineColor(2);
			fzz_cal11A_1->SetLineWidth(1);
	TF1 *fgau_cal11A		= new TF1("fgau_cal11A","gaus",-100,100);	fgau_cal11A->SetLineColor(2);
	TH2D *hzz_cal11A_peri		= new TH2D("hzz_cal11A_peri","hzz_cal11A_peri",50,-50,50,400,-200,200);
	TH1D *hdzz_cal11A_peri		= new TH1D("hdzz_cal11A_peri","hdzz_cal11A_peri",        400,-50,50);
	TH1D *hzz_cal11A_peri_1;
	TH1D *hzz_cal11A_peri_2;
	TF1  *fzz_cal11A_peri_1		= new TF1("fzz_cal11A_peri_1","pol1",-15,15);
			fzz_cal11A_peri_1->SetLineColor(2);
			fzz_cal11A_peri_1->SetLineWidth(1);
	TF1 *fgau_cal11A_peri		= new TF1("fgau_cal11A_peri","gaus",-100,100);	fgau_cal11A_peri->SetLineColor(2);
	TH2D *hzz_cal11A_cent		= new TH2D("hzz_cal11A_cent","hzz_cal11A_cent",50,-50,50,400,-200,200);
	TH1D *hdzz_cal11A_cent		= new TH1D("hdzz_cal11A_cent","hdzz_cal11A_cent",        400,-50,50);
	TH1D *hzz_cal11A_cent_1;
	TH1D *hzz_cal11A_cent_2;
	TF1  *fzz_cal11A_cent_1		= new TF1("fzz_cal11A_cent_1","pol1",-15,15);
			fzz_cal11A_cent_1->SetLineColor(2);
			fzz_cal11A_cent_1->SetLineWidth(1);
	TF1 *fgau_cal11A_cent		= new TF1("fgau_cal11A_cent","gaus",-100,100);	fgau_cal11A_cent->SetLineColor(2);

	TH2D *hdzzrm_cal11A		= new TH2D("hdzzrm_cal11A","hdzzrm_cal11A",50,0,600,100,-25,25);
	TH2D *hdzzevt_cal11A	= new TH2D("hdzzevt_cal11A","hdzzevt_cal11A",55,0,550000,100,-25,25);
	TH2D *hdzztime_cal11A	= new TH2D("hdzztime_cal11A","hdzztime_cal11A",60,0,1200,100,-25,25);

	TH2D *hzz_cal11B		= new TH2D("hzz_cal11B","hzz_cal11B",50,-50,50,400,-200,200);
	TH1D *hdzz_cal11B		= new TH1D("hdzz_cal11B","hdzz_cal11B",        400,-50,50);
	TH1D *hzz_cal11B_1;
	TH1D *hzz_cal11B_2;
	TF1  *fzz_cal11B_1		= new TF1("fzz_cal11B_1","pol1",-15,15);
			fzz_cal11B_1->SetLineColor(2);
			fzz_cal11B_1->SetLineWidth(1);
	TF1 *fgau_cal11B		= new TF1("fgau_cal11B","gaus",-100,100);	fgau_cal11B->SetLineColor(2);
	TH2D *hzz_cal11B_peri		= new TH2D("hzz_cal11B_peri","hzz_cal11B_peri",50,-50,50,400,-200,200);
	TH1D *hdzz_cal11B_peri		= new TH1D("hdzz_cal11B_peri","hdzz_cal11B_peri",        400,-50,50);
	TH1D *hzz_cal11B_peri_1;
	TH1D *hzz_cal11B_peri_2;
	TF1  *fzz_cal11B_peri_1		= new TF1("fzz_cal11B_peri_1","pol1",-15,15);
			fzz_cal11B_peri_1->SetLineColor(2);
			fzz_cal11B_peri_1->SetLineWidth(1);
	TF1 *fgau_cal11B_peri		= new TF1("fgau_cal11B_peri","gaus",-100,100);	fgau_cal11B_peri->SetLineColor(2);
	TH2D *hzz_cal11B_cent		= new TH2D("hzz_cal11B_cent","hzz_cal11B_cent",50,-50,50,400,-200,200);
	TH1D *hdzz_cal11B_cent		= new TH1D("hdzz_cal11B_cent","hdzz_cal11B_cent",        400,-50,50);
	TH1D *hzz_cal11B_cent_1;
	TH1D *hzz_cal11B_cent_2;
	TF1  *fzz_cal11B_cent_1		= new TF1("fzz_cal11B_cent_1","pol1",-15,15);
			fzz_cal11B_cent_1->SetLineColor(2);
			fzz_cal11B_cent_1->SetLineWidth(1);
	TF1 *fgau_cal11B_cent		= new TF1("fgau_cal11B_cent","gaus",-100,100);	fgau_cal11B_cent->SetLineColor(2);

	TH2D *hzz_cal11C		= new TH2D("hzz_cal11C","hzz_cal11C",50,-50,50,400,-200,200);
	TH1D *hdzz_cal11C		= new TH1D("hdzz_cal11C","hdzz_cal11C",        400,-50,50);
	TH1D *hzz_cal11C_1;
	TH1D *hzz_cal11C_2;
	TF1  *fzz_cal11C_1		= new TF1("fzz_cal11C_1","pol1",-15,15);
			fzz_cal11C_1->SetLineColor(2);
			fzz_cal11C_1->SetLineWidth(1);
	TF1 *fgau_cal11C		= new TF1("fgau_cal11C","gaus",-100,100);	fgau_cal11C->SetLineColor(2);
	TH2D *hzz_cal11C_peri		= new TH2D("hzz_cal11C_peri","hzz_cal11C_peri",50,-50,50,400,-200,200);
	TH1D *hdzz_cal11C_peri		= new TH1D("hdzz_cal11C_peri","hdzz_cal11C_peri",        400,-50,50);
	TH1D *hzz_cal11C_peri_1;
	TH1D *hzz_cal11C_peri_2;
	TF1  *fzz_cal11C_peri_1		= new TF1("fzz_cal11C_peri_1","pol1",-15,15);
			fzz_cal11C_peri_1->SetLineColor(2);
			fzz_cal11C_peri_1->SetLineWidth(1);
	TF1 *fgau_cal11C_peri		= new TF1("fgau_cal11C_peri","gaus",-100,100);	fgau_cal11C_peri->SetLineColor(2);
	TH2D *hzz_cal11C_cent		= new TH2D("hzz_cal11C_cent","hzz_cal11C_cent",50,-50,50,400,-200,200);
	TH1D *hdzz_cal11C_cent		= new TH1D("hdzz_cal11C_cent","hdzz_cal11C_cent",        400,-50,50);
	TH1D *hzz_cal11C_cent_1;
	TH1D *hzz_cal11C_cent_2;
	TF1  *fzz_cal11C_cent_1		= new TF1("fzz_cal11C_cent_1","pol1",-15,15);
			fzz_cal11C_cent_1->SetLineColor(2);
			fzz_cal11C_cent_1->SetLineWidth(1);
	TF1 *fgau_cal11C_cent		= new TF1("fgau_cal11C_cent","gaus",-100,100);	fgau_cal11C_cent->SetLineColor(2);

	TH2D *hzz_cal11Aa		= new TH2D("hzz_cal11Aa","hzz_cal11Aa",50,-50,50,400,-200,200);
	TH1D *hdzz_cal11Aa		= new TH1D("hdzz_cal11Aa","hdzz_cal11Aa",        400,-50,50);
	TH1D *hzz_cal11Aa_1;
	TH1D *hzz_cal11Aa_2;
	TF1  *fzz_cal11Aa_1		= new TF1("fzz_cal11Aa_1","pol1",-15,15);
			fzz_cal11Aa_1->SetLineColor(2);
			fzz_cal11Aa_1->SetLineWidth(1);
	TF1 *fgau_cal11Aa		= new TF1("fgau_cal11Aa","gaus",-100,100);	fgau_cal11Aa->SetLineColor(2);
	TH2D *hzz_cal11Aa_peri		= new TH2D("hzz_cal11Aa_peri","hzz_cal11Aa_peri",50,-50,50,400,-200,200);
	TH1D *hdzz_cal11Aa_peri		= new TH1D("hdzz_cal11Aa_peri","hdzz_cal11Aa_peri",        400,-50,50);
	TH1D *hzz_cal11Aa_peri_1;
	TH1D *hzz_cal11Aa_peri_2;
	TF1  *fzz_cal11Aa_peri_1		= new TF1("fzz_cal11Aa_peri_1","pol1",-15,15);
			fzz_cal11Aa_peri_1->SetLineColor(2);
			fzz_cal11Aa_peri_1->SetLineWidth(1);
	TF1 *fgau_cal11Aa_peri		= new TF1("fgau_cal11Aa_peri","gaus",-100,100);	fgau_cal11Aa_peri->SetLineColor(2);
	TH2D *hzz_cal11Aa_cent		= new TH2D("hzz_cal11Aa_cent","hzz_cal11Aa_cent",50,-50,50,400,-200,200);
	TH1D *hdzz_cal11Aa_cent		= new TH1D("hdzz_cal11Aa_cent","hdzz_cal11Aa_cent",        400,-50,50);
	TH1D *hzz_cal11Aa_cent_1;
	TH1D *hzz_cal11Aa_cent_2;
	TF1  *fzz_cal11Aa_cent_1		= new TF1("fzz_cal11Aa_cent_1","pol1",-15,15);
			fzz_cal11Aa_cent_1->SetLineColor(2);
			fzz_cal11Aa_cent_1->SetLineWidth(1);
	TF1 *fgau_cal11Aa_cent		= new TF1("fgau_cal11Aa_cent","gaus",-100,100);	fgau_cal11Aa_cent->SetLineColor(2);

	TH2D *hzz_cal11Ba		= new TH2D("hzz_cal11Ba","hzz_cal11Ba",50,-50,50,400,-200,200);
	TH1D *hdzz_cal11Ba		= new TH1D("hdzz_cal11Ba","hdzz_cal11Ba",        400,-50,50);
	TH1D *hzz_cal11Ba_1;
	TH1D *hzz_cal11Ba_2;
	TF1  *fzz_cal11Ba_1		= new TF1("fzz_cal11Ba_1","pol1",-15,15);
			fzz_cal11Ba_1->SetLineColor(2);
			fzz_cal11Ba_1->SetLineWidth(1);
	TF1 *fgau_cal11Ba		= new TF1("fgau_cal11Ba","gaus",-100,100);	fgau_cal11Ba->SetLineColor(2);
	TH2D *hzz_cal11Ba_peri		= new TH2D("hzz_cal11Ba_peri","hzz_cal11Ba_peri",50,-50,50,400,-200,200);
	TH1D *hdzz_cal11Ba_peri		= new TH1D("hdzz_cal11Ba_peri","hdzz_cal11Ba_peri",        400,-50,50);
	TH1D *hzz_cal11Ba_peri_1;
	TH1D *hzz_cal11Ba_peri_2;
	TF1  *fzz_cal11Ba_peri_1		= new TF1("fzz_cal11Ba_peri_1","pol1",-15,15);
			fzz_cal11Ba_peri_1->SetLineColor(2);
			fzz_cal11Ba_peri_1->SetLineWidth(1);
	TF1 *fgau_cal11Ba_peri		= new TF1("fgau_cal11Ba_peri","gaus",-100,100);	fgau_cal11Ba_peri->SetLineColor(2);
	TH2D *hzz_cal11Ba_cent		= new TH2D("hzz_cal11Ba_cent","hzz_cal11Ba_cent",50,-50,50,400,-200,200);
	TH1D *hdzz_cal11Ba_cent		= new TH1D("hdzz_cal11Ba_cent","hdzz_cal11Ba_cent",        400,-50,50);
	TH1D *hzz_cal11Ba_cent_1;
	TH1D *hzz_cal11Ba_cent_2;
	TF1  *fzz_cal11Ba_cent_1		= new TF1("fzz_cal11Ba_cent_1","pol1",-15,15);
			fzz_cal11Ba_cent_1->SetLineColor(2);
			fzz_cal11Ba_cent_1->SetLineWidth(1);
	TF1 *fgau_cal11Ba_cent		= new TF1("fgau_cal11Ba_cent","gaus",-100,100);	fgau_cal11Ba_cent->SetLineColor(2);

	TH2D *hzz_cal11Ca		= new TH2D("hzz_cal11Ca","hzz_cal11Ca",50,-50,50,400,-200,200);
	TH1D *hdzz_cal11Ca		= new TH1D("hdzz_cal11Ca","hdzz_cal11Ca",        400,-50,50);
	TH1D *hzz_cal11Ca_1;
	TH1D *hzz_cal11Ca_2;
	TF1  *fzz_cal11Ca_1		= new TF1("fzz_cal11Ca_1","pol1",-15,15);
			fzz_cal11Ca_1->SetLineColor(2);
			fzz_cal11Ca_1->SetLineWidth(1);
	TF1 *fgau_cal11Ca		= new TF1("fgau_cal11Ca","gaus",-100,100);	fgau_cal11Ca->SetLineColor(2);
	TH2D *hzz_cal11Ca_peri		= new TH2D("hzz_cal11Ca_peri","hzz_cal11Ca_peri",50,-50,50,400,-200,200);
	TH1D *hdzz_cal11Ca_peri		= new TH1D("hdzz_cal11Ca_peri","hdzz_cal11Ca_peri",        400,-50,50);
	TH1D *hzz_cal11Ca_peri_1;
	TH1D *hzz_cal11Ca_peri_2;
	TF1  *fzz_cal11Ca_peri_1		= new TF1("fzz_cal11Ca_peri_1","pol1",-15,15);
			fzz_cal11Ca_peri_1->SetLineColor(2);
			fzz_cal11Ca_peri_1->SetLineWidth(1);
	TF1 *fgau_cal11Ca_peri		= new TF1("fgau_cal11Ca_peri","gaus",-100,100);	fgau_cal11Ca_peri->SetLineColor(2);
	TH2D *hzz_cal11Ca_cent		= new TH2D("hzz_cal11Ca_cent","hzz_cal11Ca_cent",50,-50,50,400,-200,200);
	TH1D *hdzz_cal11Ca_cent		= new TH1D("hdzz_cal11Ca_cent","hdzz_cal11Ca_cent",        400,-50,50);
	TH1D *hzz_cal11Ca_cent_1;
	TH1D *hzz_cal11Ca_cent_2;
	TF1  *fzz_cal11Ca_cent_1		= new TF1("fzz_cal11Ca_cent_1","pol1",-15,15);
			fzz_cal11Ca_cent_1->SetLineColor(2);
			fzz_cal11Ca_cent_1->SetLineWidth(1);
	TF1 *fgau_cal11Ca_cent		= new TF1("fgau_cal11Ca_cent","gaus",-100,100);	fgau_cal11Ca_cent->SetLineColor(2);

	TH2D *hidetearliest_raw	= new TH2D("hidetearliest_raw","hidetearliest_raw",19,-0.5,18.5,19,-0.5,18.5);
	TH2D *hidetearliest_cal	= new TH2D("hidetearliest_cal","hidetearliest_cal",19,-0.5,18.5,19,-0.5,18.5);

	TH2D *hslew[19][2];
	TH2D *hcorr[19][2];
	TH2D *hzz_ch[19][2];
	for (int is=0;is<2;is++){
		for (int idet=0;idet<19;idet++){
			sprintf(buf,"hslew%d%d",idet,is);
			//hslew[idet][is]	= new TH2D(buf,buf,128,A_min,A_max,250,-250,250);
			hslew[idet][is]	= new TH2D(buf,buf,128,A_min,A_max,250,-5,5);
			sprintf(buf,"hcorr%d%d",idet,is);
			//hcorr[idet][is]	= new TH2D(buf,buf,128,A_min,A_max,250,-250,250);
			hcorr[idet][is]	= new TH2D(buf,buf,128,A_min,A_max,250,-5,5);
			//
			sprintf(buf,"hzz_ch%d%d",idet,is);
			hzz_ch[idet][is]	= new TH2D(buf,buf,50,-50,50,50,-50,50);
			//
		}
	}

	TH2D *hnerm		= new TH2D("hnerm","N_{east} vs RM",130,0,650,20,-0.5,19.5);
	TH2D *hnwrm		= new TH2D("hnwrm","N_{west} vs RM",130,0,650,20,-0.5,19.5);
	TH2D *hnermnorm	= new TH2D("hnermnorm","N_{east} vs RM, RM-normalized",130,0,650,20,-0.5,19.5);
	TH2D *hnwrmnorm	= new TH2D("hnwrmnorm","N_{west} vs RM, RM-normalized",130,0,650,20,-0.5,19.5);

	TH1D *hresults_mean	= new TH1D("hresults_mean","#mu results"   ,20,0.5,20.5);
	TH1D *hresults_sigm	= new TH1D("hresults_sigm","#sigma results",20,0.5,20.5);
			hresults_mean->SetMarkerStyle(20);
			hresults_sigm->SetMarkerStyle(20);
			hresults_mean->SetLineColor(1);
			hresults_sigm->SetLineColor(1);
			hresults_mean->SetMarkerColor(1);
			hresults_sigm->SetMarkerColor(1);
	TH1D *hresults_mean_peri	= new TH1D("hresults_mean_peri","#mu results Peripheral"   ,20,0.5,20.5);
	TH1D *hresults_sigm_peri	= new TH1D("hresults_sigm_peri","#sigma results Peripheral",20,0.5,20.5);
			hresults_mean_peri->SetMarkerStyle(20);
			hresults_sigm_peri->SetMarkerStyle(20);
			hresults_mean_peri->SetLineColor(4);
			hresults_sigm_peri->SetLineColor(4);
			hresults_mean_peri->SetMarkerColor(4);
			hresults_sigm_peri->SetMarkerColor(4);
	TH1D *hresults_mean_cent	= new TH1D("hresults_mean_cent","#mu results Central"   ,20,0.5,20.5);
	TH1D *hresults_sigm_cent	= new TH1D("hresults_sigm_cent","#sigma results Central",20,0.5,20.5);
			hresults_mean_cent->SetMarkerStyle(20);
			hresults_sigm_cent->SetMarkerStyle(20);
			hresults_mean_cent->SetLineColor(2);
			hresults_sigm_cent->SetLineColor(2);
			hresults_mean_cent->SetMarkerColor(2);
			hresults_sigm_cent->SetMarkerColor(2);

	//------------------------------------
	//------------------------------------
	//
	if (fChain == 0) return;
	Long64_t nb = 0;
	Long64_t nentries;
	if (ntodo==0){
		nentries	= nentriestree;
	} else {
		nentries	= ntodo;
	}
	cout<<".... Nev in Chain = "<<nentriestree<<endl;
	cout<<".... Nev to do    = "<<nentries    <<endl;
	//
	//---- loop 1 to get the refmult and pulse size distributions... 
	//
	float refmultmax;
	for (Long64_t jentry=0; jentry<nentriestree;jentry++) {
		Long64_t ientry = LoadTree(jentry);
		if (ientry < 0) break;
		nb = fChain->GetEntry(jentry);
		if (jentry%100000==0){cout	<<"...Processing "<<jentry<<endl;}
		//
		if (nprimary<Nprimarycut){ continue; }
		if (TMath::Abs(vtx[2])>ZVertexMax){ continue; }
		if (TMath::Sqrt(vtx[0]*vtx[0]+vtx[1]*vtx[1])>1.5){ continue; }
		//
		hrefmult->Fill(grefmult);
		if (grefmult>refmultmax){ refmultmax = grefmult; }
		//if (iRhicRun==13&&iEnergy==200 && grefmult>40){ continue; }
		//
		int nlew[2]	= {0};
		for (int is=0;is<2;is++){
			idetearliest_raw[is]	= -1;
			idetearliest_cal[is]	= -1;
			for (int idet=0;idet<NDETSIDE;idet++){
				lit[idet][is]		= false;
				int idetew			= idet + is*NDETSIDE;
				     if ((iChoice==0||iChoice==2) && T[idetew]> 0){ lit[idet][is]	= true;	}
				else if ((iChoice==1||iChoice==3) && T[idetew]>50){ lit[idet][is]	= true; }
				if (lit[idet][is]){ 
					++nlew[is];
					hA[idetew]->Fill(A[idetew]);
				} else { T[idetew] = Toff[idetew] = 0; }
			}
		}
		hnerm->Fill(grefmult,nlew[1]);
		hnwrm->Fill(grefmult,nlew[0]);
		//	
	}
	cout<<"...loop 1 done... (adc limits and refmult)"<<endl;
	//
	//---- find the peripheral and central refmult limits...
	int	rm_peripheral_min	= -1;
	int	rm_peripheral_max	= -1;
	int	rm_central_min		= -1;
	int	rm_central_max		= -1;
	//
// 	int	RMcuts_12[NCent]	= { 1, 8, 9,10,11,12,13,14, 15, 16, 17, 18, 20, 22, 25, 29,100};	// dau run8  MADE UP
// 	int	RMcuts_24[NCent]	= {10,15,21,30,41,55,73,94,119,149,184,223,269,321,379,446,680};	// RefMultCorr 200.0 run11
// 	int	RMcuts_16[NCent]	= {10,15,21,30,41,55,73,94,119,149,184,223,269,321,379,446,640};	// RefMultCorr 200.0 run10
// 	int	RMcuts_17[NCent]	= { 7,11,16,22,30,41,54,69, 88,109,135,165,199,239,285,339,530};	// RefMultCorr  62.4
// 	int	RMcuts_18[NCent]	= { 7,10,15,21,28,38,50,64, 81,102,125,153,185,222,265,316,500};	// RefMultCorr  39.0
// 	int	RMcuts_25[NCent]	= { 6, 9,13,19,26,35,45,58, 74, 92,114,139,168,201,241,288,460};	// RefMultCorr  27.0
// 	int	RMcuts_23[NCent]	= { 6, 8,12,17,23,30,40,52, 66, 83,102,125,152,183,220,263,430};	// RefMultCorr  19.6
// 	int	RMcuts_20[NCent]	= { 5, 7,10,14,19,26,34,44, 56, 70, 86,105,127,153,184,221,346};	// RefMultCorr  11.5
// 	int	RMcuts_19[NCent]	= { 4, 6, 8,12,16,22,28,37, 46, 58, 72, 87,106,128,154,185,290};	// RefMultCorr   7.7
	//
	       if (iRhicRun==14&&iEnergy== 15){
		rm_peripheral_min	=  20;
		rm_peripheral_max	=  50;
		rm_central_min		= 200;
		rm_central_max		= 350;
	} else if (iRhicRun==14&&iEnergy==200){	
		rm_peripheral_min	=  10;
		rm_peripheral_max	=  41;
		rm_central_min		= 269;
		rm_central_max		= 640;
	} else if (iRhicRun==10&&iEnergy==200){
		rm_peripheral_min	=  10;
		rm_peripheral_max	=  41;
		rm_central_min		= 269;
		rm_central_max		= 640;
	} else if (iRhicRun==10&&iEnergy== 62){
		rm_peripheral_min	=   7;
		rm_peripheral_max	=  30;
		rm_central_min		= 199;
		rm_central_max		= 530;
	} else if (iRhicRun==10&&iEnergy== 39){
		rm_peripheral_min	=   7;
		rm_peripheral_max	=  28;
		rm_central_min		= 185;
		rm_central_max		= 500;
	} else if (iRhicRun==10&&iEnergy==  7){
		rm_peripheral_min	=   4;
		rm_peripheral_max	=  16;
		rm_central_min		= 106;
		rm_central_max		= 290;
	}
	//
//	float arsum		= 0.0;
//	float armnent	= hrefmult->Integral();
	int   nbxrm		= hrefmult->GetNbinsX();
// 	float centlowmax	= 0.2;
// 	float centuppmin	= 0.8;
// 	if (iRhicRun==10&&iEnergy==7){ centlowmax = 0.4; }
// 	for (int ibx=1;ibx<=nbxrm;ibx++){
// 		float ax	 = hrefmult->GetBinCenter(ibx);
// 		float an	 = hrefmult->GetBinContent(ibx);
// 		      arsum	+= an/armnent;
// 		if (rm_peripheral_max==-1 && arsum>centlowmax){ rm_peripheral_max	= ax; }
// 		if (rm_central_min   ==-1 && arsum>centuppmin){ rm_central_min 	= ax; }
// 	}
// 	cout<<".... refmult n entries = "<<armnent<<endl;
// 	cout<<"....    peri percent   = "<<centlowmax<<endl;
// 	cout<<"....    cent percent   = "<<centuppmin<<endl;
// 	cout<<"....    peripheral max = "<<rm_peripheral_max<<endl;
// 	cout<<"....    central    min = "<<rm_central_min   <<endl;
//
	for (int ibx=1;ibx<=nbxrm;ibx++){
		float ax	 = hrefmult->GetBinCenter(ibx);
		float an	 = hrefmult->GetBinContent(ibx);
		if (ax> rm_peripheral_min && ax<=rm_peripheral_max){ hrefmult_peri->Fill(ax,an); }
		if (ax> rm_central_min    && ax<=rm_central_max   ){ hrefmult_cent->Fill(ax,an); }
	}
	//
	//
	//---- form curves for QT input
// 	TGraph *fcorr_TAC[38];
// 	TGraph *fcorrA_TAC[38];
// 	TGraph *fcorrB_TAC[38];
// 	TGraph *fcorrC_TAC[38];
	for (int is=0;is<2;is++){
		for (int idet=0;idet<NDETSIDE;idet++){
			int idetew		= idet + is*NDETSIDE;
			FormCorrA(idetew);
			FormCorrB(idetew);
			FormCorrC(idetew);
			//if (idetew==8){ fcorrA[idetew]->Print(); }
			//
// 			fcorr_TAC[idetew]	= new TGraph();	fcorr_TAC[idetew]->SetLineColor(1);  fcorr_TAC[idetew]->SetLineWidth(2); 
// 			fcorrA_TAC[idetew]	= new TGraph();	fcorrA_TAC[idetew]->SetLineColor(2); fcorrA_TAC[idetew]->SetLineWidth(2); 
// 			fcorrB_TAC[idetew]	= new TGraph();	fcorrB_TAC[idetew]->SetLineColor(6); fcorrB_TAC[idetew]->SetLineWidth(2); 
// 			fcorrC_TAC[idetew]	= new TGraph();	fcorrC_TAC[idetew]->SetLineColor(4); fcorrC_TAC[idetew]->SetLineWidth(2); 			
// 			for (int ia=0;ia<A_nbin;ia++){
// 				float abw	= (A_max-A_min)/A_nbin;
// 				float adc	= A_min + ia*abw + abw/2.;
// 				fcorr_TAC[idetew]->SetPoint( ia,adc,fcorr[idetew]->Eval(adc) /TACtoNS);
// 				fcorrA_TAC[idetew]->SetPoint(ia,adc,fcorrA[idetew]->Eval(adc)/TACtoNS);
// 				fcorrB_TAC[idetew]->SetPoint(ia,adc,fcorrB[idetew]->Eval(adc)/TACtoNS);
// 				fcorrC_TAC[idetew]->SetPoint(ia,adc,fcorrC[idetew]->Eval(adc)/TACtoNS);
// 			}
			//
		}
	}
	invertmap	= true;
	if (iChoice==1){
		WriteCorr(1);				// STEPS....................
	}
	//
	//---- define ZZ cuts...
	TF1 *fzzcuth	= new TF1("fzzcuth","pol1",-50,50);
		fzzcuth->SetParameter(0,20);
		fzzcuth->SetParameter(1,1.0);
	TF1 *fzzcutl	= new TF1("fzzcuth","pol1",-50,50);
		fzzcutl->SetParameter(0,-20);
		fzzcutl->SetParameter(1,1.0);
	//
	//---- loop 2 for analysis...
	//
	bool central,peripheral;
	for (Long64_t jentry=0; jentry<nentries;jentry++) {
		Long64_t ientry = LoadTree(jentry);
		if (ientry < 0) break;
		nb = fChain->GetEntry(jentry);
		if (jentry%10000==0){cout	<<"...Processing "<<jentry<<endl;}
		//
		if (nprimary<Nprimarycut){ continue; }
		if (TMath::Abs(vtx[2])>ZVertexMax){ continue; }
		//
		peripheral = central = false;
		if (grefmult> rm_peripheral_min && grefmult<=rm_peripheral_max){ peripheral	= true; }
		if (grefmult> rm_central_min    && grefmult<=rm_central_max   ){ central	= true; }		
		//
 		//bool	goodtrigger	= false;
 		//int		trgmask[64]	= {0};
 		//for (int i=63;i>=0;i--){
 		//	if (daqbits & (1<<i)){
 		//		hnev_daqbits->Fill(i,1.0);
 		//		trgmask[i]	= 1;
 		//		if (i==0) goodtrigger	= true;
 		//	}
 		//	goodtrigger	= true;		//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 		//} 
 		//if (!goodtrigger){ continue; }
		//
		//---- coarse counter for vpd... (needed for FO data)....
		//for (int is=0;is<2;is++){
		//	for (int idet=0;idet<19;idet++){
		//		int idetew 	= idet + is*19;
		//		if (vpd_cco[idetew]!=0){
		//			if (is==0){ hcoco_vpd_w->Fill(vpd_cco[idetew],1.0); } else
		//			if (is==1){ hcoco_vpd_e->Fill(vpd_cco[idetew],1.0); }
		//		}
		//	}
		//}
		//
		for (int is=0;is<2;is++){
			idetearliest_raw[is]	= -1;
			idetearliest_cal[is]	= -1;
			for (int idet=0;idet<NDETSIDE;idet++){
				lit[idet][is]		= false;
				int idetew			= idet + is*NDETSIDE;
				     if ((iChoice==0||iChoice==2) && T[idetew]> 0){ lit[idet][is]	= true;	}
				else if ((iChoice==1||iChoice==3) && T[idetew]>50){ lit[idet][is]	= true; }
				if (lit[idet][is]){ 
							   T[idetew] 	*= TACtoNS;
							   Toff[idetew]	 = T[idetew];
					if (is==1) Toff[idetew]	-= CrudeEWoffset;
				} else { T[idetew] = Toff[idetew] = -9999999; }
			}
		}
		//
		float ztpc	= vtx[2];
		//
 		float zvpd_rawch0ch0	= Get_rawch0ch0();
 		if (zvpd_rawch0ch0>-990){
 			hzz_rawch0ch0->Fill(ztpc,zvpd_rawch0ch0);
			if (TMath::Abs(vtx[2])<ZVertexMaxDT){ 
	 			hdzz_rawch0ch0->Fill(zvpd_rawch0ch0-ztpc);
	 		}
	 		if (peripheral){
				hzz_rawch0ch0_peri->Fill(ztpc,zvpd_rawch0ch0);
				if (TMath::Abs(vtx[2])<ZVertexMaxDT){ 
					hdzz_rawch0ch0_peri->Fill(zvpd_rawch0ch0-ztpc);
				}
			} 
			if (central){
				hzz_rawch0ch0_cent->Fill(ztpc,zvpd_rawch0ch0);
				if (TMath::Abs(vtx[2])<ZVertexMaxDT){ 
					hdzz_rawch0ch0_cent->Fill(zvpd_rawch0ch0-ztpc);
				}
			}
 			//hdzz_rawch0ch0_nprimary->Fill(nprimary,zvpd_rawch0ch0-ztpc);
 			//hdzz_rawch0ch0_nglobal->Fill(nglobal,zvpd_rawch0ch0-ztpc);
 			//hdzz_rawch0ch0_nvert->Fill(nvert,zvpd_rawch0ch0-ztpc);
 		}
 		//	
  		float zvpd_raw11	= Get_raw11();
  		if (zvpd_raw11>-990){
  			hzz_raw11->Fill(ztpc,zvpd_raw11);
			if (TMath::Abs(vtx[2])<ZVertexMaxDT){ 
	  			hdzz_raw11->Fill(zvpd_raw11-ztpc);
  			}
  			if (peripheral){
				hzz_raw11_peri->Fill(ztpc,zvpd_raw11);
				if (TMath::Abs(vtx[2])<ZVertexMaxDT){ 
					hdzz_raw11_peri->Fill(zvpd_raw11-ztpc);
				}
			} 
			if (central){
				hzz_raw11_cent->Fill(ztpc,zvpd_raw11);
				if (TMath::Abs(vtx[2])<ZVertexMaxDT){ 
					hdzz_raw11_cent->Fill(zvpd_raw11-ztpc);
				}
			}
  		}
		//		
		float zvpd_cal11	= Get_cal11();
		if (zvpd_cal11>-990){
			hzz_cal11->Fill(ztpc,zvpd_cal11);
			if (TMath::Abs(vtx[2])<ZVertexMaxDT){ 
				hdzz_cal11->Fill(zvpd_cal11-ztpc);
			}
  			if (peripheral){
				hzz_cal11_peri->Fill(ztpc,zvpd_cal11);
				if (TMath::Abs(vtx[2])<ZVertexMaxDT){ 
					hdzz_cal11_peri->Fill(zvpd_cal11-ztpc);
				}
			} 
			if (central){
				hzz_cal11_cent->Fill(ztpc,zvpd_cal11);
				if (TMath::Abs(vtx[2])<ZVertexMaxDT){ 
					hdzz_cal11_cent->Fill(zvpd_cal11-ztpc);
				}
			}
		}
		//
		float zvpd_calavg	= Get_calavg();
		if (zvpd_calavg>-990){
			hzz_calavg->Fill(ztpc,zvpd_calavg);
			if (TMath::Abs(vtx[2])<ZVertexMaxDT){ 
				hdzz_calavg->Fill(zvpd_calavg-ztpc);
			}
  			if (peripheral){
				hzz_calavg_peri->Fill(ztpc,zvpd_calavg);
				if (TMath::Abs(vtx[2])<ZVertexMaxDT){ 
					hdzz_calavg_peri->Fill(zvpd_calavg-ztpc);
				}
			} 
			if (central){
				hzz_calavg_cent->Fill(ztpc,zvpd_calavg);
				if (TMath::Abs(vtx[2])<ZVertexMaxDT){ 
					hdzz_calavg_cent->Fill(zvpd_calavg-ztpc);
				}
			}
		}
		//		
		float zvpd_cal11A	= Get_cal11X(1);
		if (zvpd_cal11A>-990){
			hzz_cal11A->Fill(ztpc,zvpd_cal11A);
			if (TMath::Abs(vtx[2])<ZVertexMaxDT){ 
				hdzz_cal11A->Fill(zvpd_cal11A-ztpc);
			}
			if (std::fabs(ztpc)<25){
				hdzzrm_cal11A->Fill(grefmult,zvpd_cal11A-ztpc);
				hdzzevt_cal11A->Fill(jentry,zvpd_cal11A-ztpc);
				hdzztime_cal11A->Fill(itime-itimemin,zvpd_cal11A-ztpc);
			}
  			if (peripheral){
				hzz_cal11A_peri->Fill(ztpc,zvpd_cal11A);
				if (TMath::Abs(vtx[2])<ZVertexMaxDT){ 
					hdzz_cal11A_peri->Fill(zvpd_cal11A-ztpc);
				}
			} 
			if (central){
				hzz_cal11A_cent->Fill(ztpc,zvpd_cal11A);
				if (TMath::Abs(vtx[2])<ZVertexMaxDT){ 
					hdzz_cal11A_cent->Fill(zvpd_cal11A-ztpc);
				}
			}
		}
		//		
		float zvpd_cal11B	= Get_cal11X(2);
		if (zvpd_cal11B>-990){
			hzz_cal11B->Fill(ztpc,zvpd_cal11B);
			if (TMath::Abs(vtx[2])<ZVertexMaxDT){ 
				hdzz_cal11B->Fill(zvpd_cal11B-ztpc);
			}
  			if (peripheral){
				hzz_cal11B_peri->Fill(ztpc,zvpd_cal11B);
				if (TMath::Abs(vtx[2])<ZVertexMaxDT){ 
					hdzz_cal11B_peri->Fill(zvpd_cal11B-ztpc);
				}
			} 
			if (central){
				hzz_cal11B_cent->Fill(ztpc,zvpd_cal11B);
				if (TMath::Abs(vtx[2])<ZVertexMaxDT){ 
					hdzz_cal11B_cent->Fill(zvpd_cal11B-ztpc);
				}
			}
		}
		//		
		float zvpd_cal11C	= Get_cal11X(3);
		if (zvpd_cal11C>-990){
			hzz_cal11C->Fill(ztpc,zvpd_cal11C);
			if (TMath::Abs(vtx[2])<ZVertexMaxDT){ 
				hdzz_cal11C->Fill(zvpd_cal11C-ztpc);
			}
  			if (peripheral){
				hzz_cal11C_peri->Fill(ztpc,zvpd_cal11C);
				if (TMath::Abs(vtx[2])<ZVertexMaxDT){ 
					hdzz_cal11C_peri->Fill(zvpd_cal11C-ztpc);
				}
			} 
			if (central){
				hzz_cal11C_cent->Fill(ztpc,zvpd_cal11C);
				if (TMath::Abs(vtx[2])<ZVertexMaxDT){ 
					hdzz_cal11C_cent->Fill(zvpd_cal11C-ztpc);
				}
			}
		}
		//		
		float zvpd_cal11Aa	= Get_cal11Xa(1);
		if (zvpd_cal11Aa>-990){
			hzz_cal11Aa->Fill(ztpc,zvpd_cal11Aa);
			if (TMath::Abs(vtx[2])<ZVertexMaxDT){ 
				hdzz_cal11Aa->Fill(zvpd_cal11Aa-ztpc);
			}
  			if (peripheral){
				hzz_cal11Aa_peri->Fill(ztpc,zvpd_cal11Aa);
				if (TMath::Abs(vtx[2])<ZVertexMaxDT){ 
					hdzz_cal11Aa_peri->Fill(zvpd_cal11Aa-ztpc);
				}
			} 
			if (central){
				hzz_cal11Aa_cent->Fill(ztpc,zvpd_cal11Aa);
				if (TMath::Abs(vtx[2])<ZVertexMaxDT){ 
					hdzz_cal11Aa_cent->Fill(zvpd_cal11Aa-ztpc);
				}
			}
		}
		//		
		float zvpd_cal11Ba	= Get_cal11Xa(2);
		if (zvpd_cal11Ba>-990){
			hzz_cal11Ba->Fill(ztpc,zvpd_cal11Ba);
			if (TMath::Abs(vtx[2])<ZVertexMaxDT){ 
				hdzz_cal11Ba->Fill(zvpd_cal11Ba-ztpc);
			}
  			if (peripheral){
				hzz_cal11Ba_peri->Fill(ztpc,zvpd_cal11Ba);
				if (TMath::Abs(vtx[2])<ZVertexMaxDT){ 
					hdzz_cal11Ba_peri->Fill(zvpd_cal11Ba-ztpc);
				}
			} 
			if (central){
				hzz_cal11Ba_cent->Fill(ztpc,zvpd_cal11Ba);
				if (TMath::Abs(vtx[2])<ZVertexMaxDT){ 
					hdzz_cal11Ba_cent->Fill(zvpd_cal11Ba-ztpc);
				}
			}
		}
		//		
		float zvpd_cal11Ca	= Get_cal11Xa(3);
		if (zvpd_cal11Ca>-990){
			hzz_cal11Ca->Fill(ztpc,zvpd_cal11Ca);
			if (TMath::Abs(vtx[2])<ZVertexMaxDT){ 
				hdzz_cal11Ca->Fill(zvpd_cal11Ca-ztpc);
			}
  			if (peripheral){
				hzz_cal11Ca_peri->Fill(ztpc,zvpd_cal11Ca);
				if (TMath::Abs(vtx[2])<ZVertexMaxDT){ 
					hdzz_cal11Ca_peri->Fill(zvpd_cal11Ca-ztpc);
				}
			} 
			if (central){
				hzz_cal11Ca_cent->Fill(ztpc,zvpd_cal11Ca);
				if (TMath::Abs(vtx[2])<ZVertexMaxDT){ 
					hdzz_cal11Ca_cent->Fill(zvpd_cal11Ca-ztpc);
				}
			}
		}
		//
		//
		float dtcheck;
		for (int is=0;is<2;is++){
			for (int idet=0;idet<19;idet++){
				if (lit[idet][is]){
					int idetew	= is*NDETSIDE + idet;
					float time	= T[idetew];
					float tcal	= Calibrate(idetew,A[idetew],T[idetew],0);					// FULL RESN.....
//					float tcal	= Calibrate(idetew,A[idetew],T[idetew],1);					// STEPS.....
					int   ntavg	=  0;
					float tavg=-1, tcalavg=-1;
					for (int kdet=0;kdet<19;kdet++){
						int kdetew	= is*NDETSIDE + kdet;
						if (lit[kdet][is] && kdet!=idet){
							tavg	+= T[kdetew];
							tcalavg	+= Calibrate(kdetew,A[kdetew],T[kdetew],0);				// FULL RESN...
//							tcalavg	+= Calibrate(kdetew,A[kdetew],T[kdetew],1);				// STEPS...
							++ntavg;
						}
					}
					if (ntavg>=2){
						dtcheck	= time - (tavg/ntavg);
						hslew[idet][is]->Fill(A[idetew],dtcheck);
						dtcheck	= tcal - (tcalavg/ntavg);
						hcorr[idet][is]->Fill(A[idetew],dtcheck);
					}
				}
			}
		}
// 		if (lit[idet][is])&&lit[1][0]&&lit[2][0]){
// 			tval	= Toff[0] - (Toff[1]+Toff[2])/2.;
// 			hslew0->Fill(A[0],tval);
// 			//if (A[0]<100) cout<<A[0]<<" "<<Toff[0]<<" "<<Calibrate(0,A[0],Toff[0],0)<<endl;
// 			tval	= Calibrate(0,A[0],Toff[0],0) - (Calibrate(1,A[1],Toff[1],0)+Calibrate(2,A[2],Toff[2],0))/2.;
// 			hcorr0->Fill(A[0],tval);
// 		}
		//
		//
		//cout<<nprimary<<" "<<nvert<<" "<<nglobal<<endl;
		//
		if (idetearliest_raw[0]*idetearliest_raw[1]>=0){
			hidetearliest_raw->Fill(idetearliest_raw[0],idetearliest_raw[1]);
		}
		if (idetearliest_cal[0]*idetearliest_cal[1]>=0){
			hidetearliest_cal->Fill(idetearliest_cal[0],idetearliest_cal[1]);
		}
		//
		//---- check calibration factors...
		for (int is=0;is<2;is++){
			int ks	= 1-is;
			for (int idet=0;idet<19;idet++){
				if (lit[idet][is]){
					int   idetew	= is*NDETSIDE + idet;
					//float thistime	= T[idetew];
					float thistime	= Calibrate(idetew,A[idetew],T[idetew],0);
					float thisoppT	= 0;
					float thisoppN	= 0;					
					for (int kdet=0;kdet<19;kdet++){
						if (lit[kdet][ks]){
							int kdetew	= ks*NDETSIDE + kdet;
							thisoppN++;
							//thisoppT	+= T[kdetew];
							thisoppT	+= Calibrate(kdetew,A[kdetew],T[kdetew],0);
						}
					}
					if (thisoppN){
						thisoppT	/= thisoppN;
					} 
					float thisdt	= thistime - thisoppT;
					float thiszvtx;
					if (iChoice==1||iChoice==3){
						if (is==0){ thiszvtx	=  thisdt*29.9792458/2.; } else
						if (is==1){ thiszvtx	= -thisdt*29.9792458/2.; } 
					} else {
						if (is==0){ thiszvtx	= -thisdt*29.9792458/2.; } else
						if (is==1){ thiszvtx	=  thisdt*29.9792458/2.; } 
					}
					float cuth	= fzzcuth->Eval(vtx[2]);
					float cutl	= fzzcutl->Eval(vtx[2]);
					if (thiszvtx>=cutl && thiszvtx<=cuth){
						hzz_ch[idet][is]->Fill(vtx[2],thiszvtx);
					}
				}
			}
		}		
		//
		//
	}	//----- end of event loop....
	//
	//------------------------------------
	//------------------------------------

	TH1D*	hm[NDETSIDE][2];
	TF1*	fm[NDETSIDE][2];
	TH1D*	hzslope		= new TH1D("hzslope","hzslope",38,-0.5,37.5);
			hzslope->SetMarkerStyle(20);
	TH1D*	hzslope1D	= new TH1D("hzslope1D","hzslope1D",93,0.845,1.155);
	cout<<"\tfloat TACtoNS_ch[38] = {";
	for (int is=0;is<2;is++){
		for (int idet=0;idet<19;idet++){
			int   idetew	= is*NDETSIDE + idet;
			float zslope	= 0.0;
			float zslopee	= 0.0;
			float azl		= -20;
			float azh		=  20;
			if (iRhicRun==13&&iEnergy==500){
				azl		= -10;
				azh		=  10;
			}
			sprintf(buf,"fm%d%d",idet,is);
			fm[idet][is] = new TF1(buf,"pol1",azl,azh);
				fm[idet][is]->SetLineColor(2);
				fm[idet][is]->SetLineWidth(2);
			//
			if (hzz_ch[idet][is]->GetEntries()>10){
				//
				hzz_ch[idet][is]->FitSlicesY();
				sprintf(buf,"hzz_ch%d%d_1",idet,is);
				hm[idet][is]	= (TH1D*)gDirectory->Get(buf);
				sprintf(buf,"hm%d%d",idet,is);
				hm[idet][is]->SetName(buf);
					hm[idet][is]->SetMarkerStyle(20);
					hm[idet][is]->SetMarkerSize(0.5);
				//
				sprintf(buf,"fm%d%d",idet,is);
				hm[idet][is]->Fit(buf,"Q0RN");
				fm[idet][is]->SetRange(-50,50);	
				zslope	= fm[idet][is]->GetParameter(1);
				zslopee	= fm[idet][is]->GetParError(1);
				//
			} else {
				sprintf(buf,"hm%d%d",idet,is);
				hm[idet][is]	= new TH1D(buf,buf,50,-50,50);
			}
			//cout<<is<<" "<<idet<<" \t "<<zslope<<" "<<zslopee<<endl;
			hzslope->SetBinContent(idetew+1,zslope);
			hzslope->SetBinError(idetew+1,zslopee);
			hzslope1D->Fill(zslope);
			if (idetew%6==0){ 
				cout<<endl; 
				cout<<"\t\t\t"; 
			}
			if (zslope !=0){ cout<<TACtoNS/zslope; } else 
			               { cout<<TACtoNS; } 
			if (idetew <37){ cout<<","; } else { cout<<"};"; }
			if (idetew==37){ cout<<endl; }
		}	
	}			cout<<endl;


	//---- analysis...
	//
	//
	Norm2D(hrefmult,hnerm,hnermnorm);
	Norm2D(hrefmult,hnwrm,hnwrmnorm);
	//
	bool good;
	//
	good = DoFitSliceY(hzz_rawch0ch0);
	//hzz_rawch0ch0->FitSlicesY();
	hzz_rawch0ch0_1	= (TH1D*)gDirectory->Get("hzz_rawch0ch0_1");
	hzz_rawch0ch0_2	= (TH1D*)gDirectory->Get("hzz_rawch0ch0_2");
	if (good){ 
		hzz_rawch0ch0_1->Fit("fzz_rawch0ch0_1","QN0R"); 
		FitGaussian(hdzz_rawch0ch0,fgau_rawch0ch0,0);
	}
	good = DoFitSliceY(hzz_rawch0ch0_peri);
	//hzz_rawch0ch0_peri->FitSlicesY();
	hzz_rawch0ch0_peri_1	= (TH1D*)gDirectory->Get("hzz_rawch0ch0_peri_1");
	hzz_rawch0ch0_peri_2	= (TH1D*)gDirectory->Get("hzz_rawch0ch0_peri_2");
	if (good){ 
		hzz_rawch0ch0_peri_1->Fit("fzz_rawch0ch0_peri_1","QN0R"); 
		FitGaussian(hdzz_rawch0ch0_peri,fgau_rawch0ch0_peri,0);
	}
	good = DoFitSliceY(hzz_rawch0ch0_cent);
	//hzz_rawch0ch0_cent->FitSlicesY();
	hzz_rawch0ch0_cent_1	= (TH1D*)gDirectory->Get("hzz_rawch0ch0_cent_1");
	hzz_rawch0ch0_cent_2	= (TH1D*)gDirectory->Get("hzz_rawch0ch0_cent_2");
	if (good){ 
		hzz_rawch0ch0_cent_1->Fit("fzz_rawch0ch0_cent_1","QN0R"); 
		FitGaussian(hdzz_rawch0ch0_cent,fgau_rawch0ch0_cent,0);
	}
	//
	good = DoFitSliceY(hzz_raw11);
	//hzz_raw11->FitSlicesY();
	hzz_raw11_1	= (TH1D*)gDirectory->Get("hzz_raw11_1");
	hzz_raw11_2	= (TH1D*)gDirectory->Get("hzz_raw11_2");
	if (good){ 
		hzz_raw11_1->Fit("fzz_raw11_1","QN0R"); 
		FitGaussian(hdzz_raw11,fgau_raw11,0);
	}
	good = DoFitSliceY(hzz_raw11_peri);
	//hzz_raw11_peri->FitSlicesY();
	hzz_raw11_peri_1	= (TH1D*)gDirectory->Get("hzz_raw11_peri_1");
	hzz_raw11_peri_2	= (TH1D*)gDirectory->Get("hzz_raw11_peri_2");
	if (good){ 
		hzz_raw11_peri_1->Fit("fzz_raw11_peri_1","QN0R"); 
		FitGaussian(hdzz_raw11_peri,fgau_raw11_peri,0);
	}
	good = DoFitSliceY(hzz_raw11_cent);
	//hzz_raw11_cent->FitSlicesY();
	hzz_raw11_cent_1	= (TH1D*)gDirectory->Get("hzz_raw11_cent_1");
	hzz_raw11_cent_2	= (TH1D*)gDirectory->Get("hzz_raw11_cent_2");
	if (good){ 
		hzz_raw11_cent_1->Fit("fzz_raw11_cent_1","QN0R"); 
		FitGaussian(hdzz_raw11_cent,fgau_raw11_cent,0);
	}
	//
	good = DoFitSliceY(hzz_cal11);
	//hzz_cal11->FitSlicesY();
	hzz_cal11_1	= (TH1D*)gDirectory->Get("hzz_cal11_1");
	hzz_cal11_2	= (TH1D*)gDirectory->Get("hzz_cal11_2");
	if (good){ 
		hzz_cal11_1->Fit("fzz_cal11_1","QN0R"); 
		FitGaussian(hdzz_cal11,fgau_cal11,0);
	}
	good = DoFitSliceY(hzz_cal11_peri);
	//hzz_cal11_peri->FitSlicesY();
	hzz_cal11_peri_1	= (TH1D*)gDirectory->Get("hzz_cal11_peri_1");
	hzz_cal11_peri_2	= (TH1D*)gDirectory->Get("hzz_cal11_peri_2");
	if (good){ 
		hzz_cal11_peri_1->Fit("fzz_cal11_peri_1","QN0R"); 
		FitGaussian(hdzz_cal11_peri,fgau_cal11_peri,0);
	}
	good = DoFitSliceY(hzz_cal11_cent);
	//hzz_cal11_cent->FitSlicesY();
	hzz_cal11_cent_1	= (TH1D*)gDirectory->Get("hzz_cal11_cent_1");
	hzz_cal11_cent_2	= (TH1D*)gDirectory->Get("hzz_cal11_cent_2");
	if (good){ 
		hzz_cal11_cent_1->Fit("fzz_cal11_cent_1","QN0R"); 
		FitGaussian(hdzz_cal11_cent,fgau_cal11_cent,0);
	}
	//
	good = DoFitSliceY(hzz_calavg);
	//hzz_calavg->FitSlicesY();
	hzz_calavg_1	= (TH1D*)gDirectory->Get("hzz_calavg_1");
	hzz_calavg_2	= (TH1D*)gDirectory->Get("hzz_calavg_2");
	if (good){ 
		hzz_calavg_1->Fit("fzz_calavg_1","QN0R"); 
		FitGaussian(hdzz_calavg,fgau_calavg,0);
	}
	good = DoFitSliceY(hzz_calavg_peri);
	//hzz_calavg_peri->FitSlicesY();
	hzz_calavg_peri_1	= (TH1D*)gDirectory->Get("hzz_calavg_peri_1");
	hzz_calavg_peri_2	= (TH1D*)gDirectory->Get("hzz_calavg_peri_2");
	if (good){ 
		hzz_calavg_peri_1->Fit("fzz_calavg_peri_1","QN0R"); 
		FitGaussian(hdzz_calavg_peri,fgau_calavg_peri,0);
	}
	good = DoFitSliceY(hzz_calavg_cent);
	//hzz_calavg_cent->FitSlicesY();
	hzz_calavg_cent_1	= (TH1D*)gDirectory->Get("hzz_calavg_cent_1");
	hzz_calavg_cent_2	= (TH1D*)gDirectory->Get("hzz_calavg_cent_2");
	if (good){ 
		hzz_calavg_cent_1->Fit("fzz_calavg_cent_1","QN0R"); 
		FitGaussian(hdzz_calavg_cent,fgau_calavg_cent,0);
	}
	//
	good = DoFitSliceY(hzz_cal11A);
	//hzz_cal11A->FitSlicesY();
	hzz_cal11A_1	= (TH1D*)gDirectory->Get("hzz_cal11A_1");
	hzz_cal11A_2	= (TH1D*)gDirectory->Get("hzz_cal11A_2");
	if (good){ 
		hzz_cal11A_1->Fit("fzz_cal11A_1","QN0R"); 
		FitGaussian(hdzz_cal11A,fgau_cal11A,0);
	}
	good = DoFitSliceY(hzz_cal11A_peri);
	//hzz_cal11A_peri->FitSlicesY();
	hzz_cal11A_peri_1	= (TH1D*)gDirectory->Get("hzz_cal11A_peri_1");
	hzz_cal11A_peri_2	= (TH1D*)gDirectory->Get("hzz_cal11A_peri_2");
	if (good){ 
		hzz_cal11A_peri_1->Fit("fzz_cal11A_peri_1","QN0R"); 
		FitGaussian(hdzz_cal11A_peri,fgau_cal11A_peri,0);
	}
	good = DoFitSliceY(hzz_cal11A_cent);
	//hzz_cal11A_cent->FitSlicesY();
	hzz_cal11A_cent_1	= (TH1D*)gDirectory->Get("hzz_cal11A_cent_1");
	hzz_cal11A_cent_2	= (TH1D*)gDirectory->Get("hzz_cal11A_cent_2");
	if (good){ 
		hzz_cal11A_cent_1->Fit("fzz_cal11A_cent_1","QN0R"); 
		FitGaussian(hdzz_cal11A_cent,fgau_cal11A_cent,0);
	}
	//
	good = DoFitSliceY(hzz_cal11B);
	//hzz_cal11B->FitSlicesY();
	hzz_cal11B_1	= (TH1D*)gDirectory->Get("hzz_cal11B_1");
	hzz_cal11B_2	= (TH1D*)gDirectory->Get("hzz_cal11B_2");
	if (good){ 
		hzz_cal11B_1->Fit("fzz_cal11B_1","QN0R"); 
		FitGaussian(hdzz_cal11B,fgau_cal11B,0);
	}
	good = DoFitSliceY(hzz_cal11B_peri);
	//hzz_cal11B_peri->FitSlicesY();
	hzz_cal11B_peri_1	= (TH1D*)gDirectory->Get("hzz_cal11B_peri_1");
	hzz_cal11B_peri_2	= (TH1D*)gDirectory->Get("hzz_cal11B_peri_2");
	if (good){ 
		hzz_cal11B_peri_1->Fit("fzz_cal11B_peri_1","QN0R"); 
		FitGaussian(hdzz_cal11B_peri,fgau_cal11B_peri,0);
	}
	good = DoFitSliceY(hzz_cal11B_cent);
	//hzz_cal11B_cent->FitSlicesY();
	hzz_cal11B_cent_1	= (TH1D*)gDirectory->Get("hzz_cal11B_cent_1");
	hzz_cal11B_cent_2	= (TH1D*)gDirectory->Get("hzz_cal11B_cent_2");
	if (good){ 
		hzz_cal11B_cent_1->Fit("fzz_cal11B_cent_1","QN0R"); 
		FitGaussian(hdzz_cal11B_cent,fgau_cal11B_cent,0);
	}
	//
	good = DoFitSliceY(hzz_cal11C);
	//hzz_cal11C->FitSlicesY();
	hzz_cal11C_1	= (TH1D*)gDirectory->Get("hzz_cal11C_1");
	hzz_cal11C_2	= (TH1D*)gDirectory->Get("hzz_cal11C_2");
	if (good){ 
		hzz_cal11C_1->Fit("fzz_cal11C_1","QN0R"); 
		FitGaussian(hdzz_cal11C,fgau_cal11C,0);
	}
	good = DoFitSliceY(hzz_cal11C_peri);
	//hzz_cal11C_peri->FitSlicesY();
	hzz_cal11C_peri_1	= (TH1D*)gDirectory->Get("hzz_cal11C_peri_1");
	hzz_cal11C_peri_2	= (TH1D*)gDirectory->Get("hzz_cal11C_peri_2");
	if (good){ 
		hzz_cal11C_peri_1->Fit("fzz_cal11C_peri_1","QN0R"); 
		FitGaussian(hdzz_cal11C_peri,fgau_cal11C_peri,0);
	}
	good = DoFitSliceY(hzz_cal11C_cent);
	//hzz_cal11C_cent->FitSlicesY();
	hzz_cal11C_cent_1	= (TH1D*)gDirectory->Get("hzz_cal11C_cent_1");
	hzz_cal11C_cent_2	= (TH1D*)gDirectory->Get("hzz_cal11C_cent_2");
	if (good){ 
		hzz_cal11C_cent_1->Fit("fzz_cal11C_cent_1","QN0R"); 
		FitGaussian(hdzz_cal11C_cent,fgau_cal11C_cent,0);
	}
	//
	good = DoFitSliceY(hzz_cal11Aa);
	//hzz_cal11Aa->FitSlicesY();
	hzz_cal11Aa_1	= (TH1D*)gDirectory->Get("hzz_cal11Aa_1");
	hzz_cal11Aa_2	= (TH1D*)gDirectory->Get("hzz_cal11Aa_2");
	if (good){ 
		hzz_cal11Aa_1->Fit("fzz_cal11Aa_1","QN0R"); 
		FitGaussian(hdzz_cal11Aa,fgau_cal11Aa,0);
	}
	good = DoFitSliceY(hzz_cal11Aa_peri);
	//hzz_cal11Aa_peri->FitSlicesY();
	hzz_cal11Aa_peri_1	= (TH1D*)gDirectory->Get("hzz_cal11Aa_peri_1");
	hzz_cal11Aa_peri_2	= (TH1D*)gDirectory->Get("hzz_cal11Aa_peri_2");
	if (good){ 
		hzz_cal11Aa_peri_1->Fit("fzz_cal11Aa_peri_1","QN0R"); 
		FitGaussian(hdzz_cal11Aa_peri,fgau_cal11Aa_peri,0);
	}
	good = DoFitSliceY(hzz_cal11Aa_cent);
	//hzz_cal11Aa_cent->FitSlicesY();
	hzz_cal11Aa_cent_1	= (TH1D*)gDirectory->Get("hzz_cal11Aa_cent_1");
	hzz_cal11Aa_cent_2	= (TH1D*)gDirectory->Get("hzz_cal11Aa_cent_2");
	if (good){ 
		hzz_cal11Aa_cent_1->Fit("fzz_cal11Aa_cent_1","QN0R"); 
		FitGaussian(hdzz_cal11Aa_cent,fgau_cal11Aa_cent,0);
	}
	//
	good = DoFitSliceY(hzz_cal11Ba);
	//hzz_cal11Ba->FitSlicesY();
	hzz_cal11Ba_1	= (TH1D*)gDirectory->Get("hzz_cal11Ba_1");
	hzz_cal11Ba_2	= (TH1D*)gDirectory->Get("hzz_cal11Ba_2");
	if (good){ 
		hzz_cal11Ba_1->Fit("fzz_cal11Ba_1","QN0R"); 
		FitGaussian(hdzz_cal11Ba,fgau_cal11Ba,0);
	}
	good = DoFitSliceY(hzz_cal11Ba_peri);
	//hzz_cal11Ba_peri->FitSlicesY();
	hzz_cal11Ba_peri_1	= (TH1D*)gDirectory->Get("hzz_cal11Ba_peri_1");
	hzz_cal11Ba_peri_2	= (TH1D*)gDirectory->Get("hzz_cal11Ba_peri_2");
	if (good){ 
		hzz_cal11Ba_peri_1->Fit("fzz_cal11Ba_peri_1","QN0R"); 
		FitGaussian(hdzz_cal11Ba_peri,fgau_cal11Ba_peri,0);
	}
	good = DoFitSliceY(hzz_cal11Ba_cent);
	//hzz_cal11Ba_cent->FitSlicesY();
	hzz_cal11Ba_cent_1	= (TH1D*)gDirectory->Get("hzz_cal11Ba_cent_1");
	hzz_cal11Ba_cent_2	= (TH1D*)gDirectory->Get("hzz_cal11Ba_cent_2");
	if (good){ 
		hzz_cal11Ba_cent_1->Fit("fzz_cal11Ba_cent_1","QN0R"); 
		FitGaussian(hdzz_cal11Ba_cent,fgau_cal11Ba_cent,0);
	}
	//
	good = DoFitSliceY(hzz_cal11Ca);
	//hzz_cal11Ca->FitSlicesY();
	hzz_cal11Ca_1	= (TH1D*)gDirectory->Get("hzz_cal11Ca_1");
	hzz_cal11Ca_2	= (TH1D*)gDirectory->Get("hzz_cal11Ca_2");
	if (good){ 
		hzz_cal11Ca_1->Fit("fzz_cal11Ca_1","QN0R"); 
		FitGaussian(hdzz_cal11Ca,fgau_cal11Ca,0);
	}
	good = DoFitSliceY(hzz_cal11Ca_peri);
	//hzz_cal11Ca_peri->FitSlicesY();
	hzz_cal11Ca_peri_1	= (TH1D*)gDirectory->Get("hzz_cal11Ca_peri_1");
	hzz_cal11Ca_peri_2	= (TH1D*)gDirectory->Get("hzz_cal11Ca_peri_2");
	if (good){ 
		hzz_cal11Ca_peri_1->Fit("fzz_cal11Ca_peri_1","QN0R"); 
		FitGaussian(hdzz_cal11Ca_peri,fgau_cal11Ca_peri,0);
	}
	good = DoFitSliceY(hzz_cal11Ca_cent);
	//hzz_cal11Ca_cent->FitSlicesY();
	hzz_cal11Ca_cent_1	= (TH1D*)gDirectory->Get("hzz_cal11Ca_cent_1");
	hzz_cal11Ca_cent_2	= (TH1D*)gDirectory->Get("hzz_cal11Ca_cent_2");
	if (good){ 
		hzz_cal11Ca_cent_1->Fit("fzz_cal11Ca_cent_1","QN0R"); 
		FitGaussian(hdzz_cal11Ca_cent,fgau_cal11Ca_cent,0);
	}
	//


	hresults_mean->SetBinContent(1,          fgau_rawch0ch0->GetParameter(1)  );
	hresults_mean->SetBinError(1,            fgau_rawch0ch0->GetParError(1)   );
	hresults_sigm->SetBinContent(1,std::fabs(fgau_rawch0ch0->GetParameter(2)) );
	hresults_sigm->SetBinError(1,            fgau_rawch0ch0->GetParError(2)   );
	hresults_mean->SetBinContent(2,          fgau_raw11->GetParameter(1)  );
	hresults_mean->SetBinError(2,            fgau_raw11->GetParError(1)   );
	hresults_sigm->SetBinContent(2,std::fabs(fgau_raw11->GetParameter(2)) );
	hresults_sigm->SetBinError(2,            fgau_raw11->GetParError(2)   );
	hresults_mean->SetBinContent(3,          fgau_cal11->GetParameter(1)  );
	hresults_mean->SetBinError(3,            fgau_cal11->GetParError(1)   );
	hresults_sigm->SetBinContent(3,std::fabs(fgau_cal11->GetParameter(2)) );
	hresults_sigm->SetBinError(3,            fgau_cal11->GetParError(2)   );
	hresults_mean->SetBinContent(4,          fgau_calavg->GetParameter(1)  );
	hresults_mean->SetBinError(4,            fgau_calavg->GetParError(1)   );
	hresults_sigm->SetBinContent(4,std::fabs(fgau_calavg->GetParameter(2)) );
	hresults_sigm->SetBinError(4,            fgau_calavg->GetParError(2)   );
	hresults_mean->SetBinContent(5,          fgau_cal11A->GetParameter(1)  );
	hresults_mean->SetBinError(5,            fgau_cal11A->GetParError(1)   );
	hresults_sigm->SetBinContent(5,std::fabs(fgau_cal11A->GetParameter(2)) );
	hresults_sigm->SetBinError(5,            fgau_cal11A->GetParError(2)   );
	hresults_mean->SetBinContent(6,          fgau_cal11B->GetParameter(1)  );
	hresults_mean->SetBinError(6,            fgau_cal11B->GetParError(1)   );
	hresults_sigm->SetBinContent(6,std::fabs(fgau_cal11B->GetParameter(2)) );
	hresults_sigm->SetBinError(6,            fgau_cal11B->GetParError(2)   );
	hresults_mean->SetBinContent(7,          fgau_cal11C->GetParameter(1)  );
	hresults_mean->SetBinError(7,            fgau_cal11C->GetParError(1)   );
	hresults_sigm->SetBinContent(7,std::fabs(fgau_cal11C->GetParameter(2)) );
	hresults_sigm->SetBinError(7,            fgau_cal11C->GetParError(2)   );
	hresults_mean->SetBinContent(8,          fgau_cal11Aa->GetParameter(1)  );
	hresults_mean->SetBinError(8,            fgau_cal11Aa->GetParError(1)   );
	hresults_sigm->SetBinContent(8,std::fabs(fgau_cal11Aa->GetParameter(2)) );
	hresults_sigm->SetBinError(8,            fgau_cal11Aa->GetParError(2)   );
	hresults_mean->SetBinContent(9,          fgau_cal11Ba->GetParameter(1)  );
	hresults_mean->SetBinError(9,            fgau_cal11Ba->GetParError(1)   );
	hresults_sigm->SetBinContent(9,std::fabs(fgau_cal11Ba->GetParameter(2)) );
	hresults_sigm->SetBinError(9,            fgau_cal11Ba->GetParError(2)   );
	hresults_mean->SetBinContent(10,          fgau_cal11Ca->GetParameter(1)  );
	hresults_mean->SetBinError(10,            fgau_cal11Ca->GetParError(1)   );
	hresults_sigm->SetBinContent(10,std::fabs(fgau_cal11Ca->GetParameter(2)) );
	hresults_sigm->SetBinError(10,            fgau_cal11Ca->GetParError(2)   );

	hresults_mean_peri->SetBinContent(1,          fgau_rawch0ch0_peri->GetParameter(1)  );
	hresults_mean_peri->SetBinError(1,            fgau_rawch0ch0_peri->GetParError(1)   );
	hresults_sigm_peri->SetBinContent(1,std::fabs(fgau_rawch0ch0_peri->GetParameter(2)) );
	hresults_sigm_peri->SetBinError(1,            fgau_rawch0ch0_peri->GetParError(2)   );
	hresults_mean_peri->SetBinContent(2,          fgau_raw11_peri->GetParameter(1)  );
	hresults_mean_peri->SetBinError(2,            fgau_raw11_peri->GetParError(1)   );
	hresults_sigm_peri->SetBinContent(2,std::fabs(fgau_raw11_peri->GetParameter(2)) );
	hresults_sigm_peri->SetBinError(2,            fgau_raw11_peri->GetParError(2)   );
	hresults_mean_peri->SetBinContent(3,          fgau_cal11_peri->GetParameter(1)  );
	hresults_mean_peri->SetBinError(3,            fgau_cal11_peri->GetParError(1)   );
	hresults_sigm_peri->SetBinContent(3,std::fabs(fgau_cal11_peri->GetParameter(2)) );
	hresults_sigm_peri->SetBinError(3,            fgau_cal11_peri->GetParError(2)   );
	hresults_mean_peri->SetBinContent(4,          fgau_calavg_peri->GetParameter(1)  );
	hresults_mean_peri->SetBinError(4,            fgau_calavg_peri->GetParError(1)   );
	hresults_sigm_peri->SetBinContent(4,std::fabs(fgau_calavg_peri->GetParameter(2)) );
	hresults_sigm_peri->SetBinError(4,            fgau_calavg_peri->GetParError(2)   );
	hresults_mean_peri->SetBinContent(5,          fgau_cal11A_peri->GetParameter(1)  );
	hresults_mean_peri->SetBinError(5,            fgau_cal11A_peri->GetParError(1)   );
	hresults_sigm_peri->SetBinContent(5,std::fabs(fgau_cal11A_peri->GetParameter(2)) );
	hresults_sigm_peri->SetBinError(5,            fgau_cal11A_peri->GetParError(2)   );
	hresults_mean_peri->SetBinContent(6,          fgau_cal11B_peri->GetParameter(1)  );
	hresults_mean_peri->SetBinError(6,            fgau_cal11B_peri->GetParError(1)   );
	hresults_sigm_peri->SetBinContent(6,std::fabs(fgau_cal11B_peri->GetParameter(2)) );
	hresults_sigm_peri->SetBinError(6,            fgau_cal11B_peri->GetParError(2)   );
	hresults_mean_peri->SetBinContent(7,          fgau_cal11C_peri->GetParameter(1)  );
	hresults_mean_peri->SetBinError(7,            fgau_cal11C_peri->GetParError(1)   );
	hresults_sigm_peri->SetBinContent(7,std::fabs(fgau_cal11C_peri->GetParameter(2)) );
	hresults_sigm_peri->SetBinError(7,            fgau_cal11C_peri->GetParError(2)   );
	hresults_mean_peri->SetBinContent(8,          fgau_cal11Aa_peri->GetParameter(1)  );
	hresults_mean_peri->SetBinError(8,            fgau_cal11Aa_peri->GetParError(1)   );
	hresults_sigm_peri->SetBinContent(8,std::fabs(fgau_cal11Aa_peri->GetParameter(2)) );
	hresults_sigm_peri->SetBinError(8,            fgau_cal11Aa_peri->GetParError(2)   );
	hresults_mean_peri->SetBinContent(9,          fgau_cal11Ba_peri->GetParameter(1)  );
	hresults_mean_peri->SetBinError(9,            fgau_cal11Ba_peri->GetParError(1)   );
	hresults_sigm_peri->SetBinContent(9,std::fabs(fgau_cal11Ba_peri->GetParameter(2)) );
	hresults_sigm_peri->SetBinError(9,            fgau_cal11Ba_peri->GetParError(2)   );
	hresults_mean_peri->SetBinContent(10,          fgau_cal11Ca_peri->GetParameter(1)  );
	hresults_mean_peri->SetBinError(10,            fgau_cal11Ca_peri->GetParError(1)   );
	hresults_sigm_peri->SetBinContent(10,std::fabs(fgau_cal11Ca_peri->GetParameter(2)) );
	hresults_sigm_peri->SetBinError(10,            fgau_cal11Ca_peri->GetParError(2)   );

	hresults_mean_cent->SetBinContent(1,          fgau_rawch0ch0_cent->GetParameter(1)  );
	hresults_mean_cent->SetBinError(1,            fgau_rawch0ch0_cent->GetParError(1)   );
	hresults_sigm_cent->SetBinContent(1,std::fabs(fgau_rawch0ch0_cent->GetParameter(2)) );
	hresults_sigm_cent->SetBinError(1,            fgau_rawch0ch0_cent->GetParError(2)   );
	hresults_mean_cent->SetBinContent(2,          fgau_raw11_cent->GetParameter(1)  );
	hresults_mean_cent->SetBinError(2,            fgau_raw11_cent->GetParError(1)   );
	hresults_sigm_cent->SetBinContent(2,std::fabs(fgau_raw11_cent->GetParameter(2)) );
	hresults_sigm_cent->SetBinError(2,            fgau_raw11_cent->GetParError(2)   );
	hresults_mean_cent->SetBinContent(3,          fgau_cal11_cent->GetParameter(1)  );
	hresults_mean_cent->SetBinError(3,            fgau_cal11_cent->GetParError(1)   );
	hresults_sigm_cent->SetBinContent(3,std::fabs(fgau_cal11_cent->GetParameter(2)) );
	hresults_sigm_cent->SetBinError(3,            fgau_cal11_cent->GetParError(2)   );
	hresults_mean_cent->SetBinContent(4,          fgau_calavg_cent->GetParameter(1)  );
	hresults_mean_cent->SetBinError(4,            fgau_calavg_cent->GetParError(1)   );
	hresults_sigm_cent->SetBinContent(4,std::fabs(fgau_calavg_cent->GetParameter(2)) );
	hresults_sigm_cent->SetBinError(4,            fgau_calavg_cent->GetParError(2)   );
	hresults_mean_cent->SetBinContent(5,          fgau_cal11A_cent->GetParameter(1)  );
	hresults_mean_cent->SetBinError(5,            fgau_cal11A_cent->GetParError(1)   );
	hresults_sigm_cent->SetBinContent(5,std::fabs(fgau_cal11A_cent->GetParameter(2)) );
	hresults_sigm_cent->SetBinError(5,            fgau_cal11A_cent->GetParError(2)   );
	hresults_mean_cent->SetBinContent(6,          fgau_cal11B_cent->GetParameter(1)  );
	hresults_mean_cent->SetBinError(6,            fgau_cal11B_cent->GetParError(1)   );
	hresults_sigm_cent->SetBinContent(6,std::fabs(fgau_cal11B_cent->GetParameter(2)) );
	hresults_sigm_cent->SetBinError(6,            fgau_cal11B_cent->GetParError(2)   );
	hresults_mean_cent->SetBinContent(7,          fgau_cal11C_cent->GetParameter(1)  );
	hresults_mean_cent->SetBinError(7,            fgau_cal11C_cent->GetParError(1)   );
	hresults_sigm_cent->SetBinContent(7,std::fabs(fgau_cal11C_cent->GetParameter(2)) );
	hresults_sigm_cent->SetBinError(7,            fgau_cal11C_cent->GetParError(2)   );
	hresults_mean_cent->SetBinContent(8,          fgau_cal11Aa_cent->GetParameter(1)  );
	hresults_mean_cent->SetBinError(8,            fgau_cal11Aa_cent->GetParError(1)   );
	hresults_sigm_cent->SetBinContent(8,std::fabs(fgau_cal11Aa_cent->GetParameter(2)) );
	hresults_sigm_cent->SetBinError(8,            fgau_cal11Aa_cent->GetParError(2)   );
	hresults_mean_cent->SetBinContent(9,          fgau_cal11Ba_cent->GetParameter(1)  );
	hresults_mean_cent->SetBinError(9,            fgau_cal11Ba_cent->GetParError(1)   );
	hresults_sigm_cent->SetBinContent(9,std::fabs(fgau_cal11Ba_cent->GetParameter(2)) );
	hresults_sigm_cent->SetBinError(9,            fgau_cal11Ba_cent->GetParError(2)   );
	hresults_mean_cent->SetBinContent(10,          fgau_cal11Ca_cent->GetParameter(1)  );
	hresults_mean_cent->SetBinError(10,            fgau_cal11Ca_cent->GetParError(1)   );
	hresults_sigm_cent->SetBinContent(10,std::fabs(fgau_cal11Ca_cent->GetParameter(2)) );
	hresults_sigm_cent->SetBinError(10,            fgau_cal11Ca_cent->GetParError(2)   );

//	hresults_sigm_peri->Print("range");
//	hresults_sigm_cent->Print("range");

	//----------------------------------------
	//
	//---- painting...
	//
	gROOT->SetStyle("Plain"); 
	gStyle->SetPadBottomMargin(0.07);
	gStyle->SetPadTopMargin(0.05);
	gStyle->SetPadLeftMargin(0.11);
	gStyle->SetPadRightMargin(0.02);
	gStyle->SetOptStat(0);
	gStyle->SetOptFit(0);
	gStyle->SetPalette(1);
	gStyle->SetErrorX(0);

  	++ican; sprintf(buf,"ccan%d",ican);
  	ccan[ican] = new TCanvas(buf,buf,0,0,800.,800.*(7.5/10.));
  	ccan[ican]->SetFillColor(10);
  	ccan[ican]->cd(); ccan[ican]->Divide(3,2,0.0001,0.0001);
  		ccan[ican]->cd(1);
  			gPad->SetLogy(1);
  			hrefmult->Draw();
  			hrefmult->GetXaxis()->SetRangeUser(-0.5,1.05*refmultmax);
  			hrefmult_peri->Draw("same");
  			hrefmult_cent->Draw("same");
  		ccan[ican]->cd(2);
//   			hnerm->Draw("col");
//   			TProfile *hnermproj	= (TProfile*)hnerm->ProfileX("hnermproj");
//   					hnermproj->SetLineColor(5);
//   					hnermproj->SetLineWidth(2);
//   					hnermproj->Draw("HIST same");
   			hnermnorm->Draw("col");
   			TProfile *hnermnormproj	= (TProfile*)hnermnorm->ProfileX("hnermnormproj");
   					hnermnormproj->SetLineColor(5);
   					hnermnormproj->SetLineWidth(2);
   					hnermnormproj->Draw("HIST same");
  		ccan[ican]->cd(5);
//   			hnwrm->Draw("col");
//   			TProfile *hnwrmproj	= (TProfile*)hnwrm->ProfileX("hnwrmproj");
//   					hnwrmproj->SetLineColor(5);
//   					hnwrmproj->SetLineWidth(2);
//   					hnwrmproj->Draw("HIST same");
   			hnwrmnorm->Draw("col");
   			TProfile *hnwrmnormproj	= (TProfile*)hnwrmnorm->ProfileX("hnwrmnormproj");
   					hnwrmnormproj->SetLineColor(5);
   					hnwrmnormproj->SetLineWidth(2);
   					hnwrmnormproj->Draw("HIST same");
  		ccan[ican]->cd(3);
  			hidetearliest_raw->Draw("colz");
  		ccan[ican]->cd(4);
  			gPad->SetLogy(1);
  			hnev_daqbits->Draw();
  		ccan[ican]->cd(6);
  			hidetearliest_cal->Draw("colz");
  	ccan[ican]->cd(); ccan[ican]->Update();
  	ccan[ican]->Print(psfileO.Data());
  
  	//---- Zvtx correlations by detector...
	for (int is=0;is<2;is++){
		++ican; sprintf(buf,"ccan%d",ican);
		ccan[ican] = new TCanvas(buf,buf,0,0,800.,800.*(7.5/10.));
		ccan[ican]->SetFillColor(10);
		ccan[ican]->cd(); ccan[ican]->Divide(5,4,0.0001,0.0001);
		for (int idet=0;idet<NDETSIDE;idet++){
			int idetew	= is*NDETSIDE + idet;
			ccan[ican]->cd(1+idet);
				hzz_ch[idet][is]->Draw("col");
				hm[idet][is]->Draw("same");
				fm[idet][is]->Draw("same");
		}
		ccan[ican]->cd(); ccan[ican]->Update();
		ccan[ican]->Print(psfile.Data());
	}
	gStyle->SetOptStat(2211);
	++ican; sprintf(buf,"ccan%d",ican);
	ccan[ican] = new TCanvas(buf,buf,0,0,800.,800.*(7.5/10.));
	ccan[ican]->SetFillColor(10);
	ccan[ican]->cd(); ccan[ican]->Divide(1,2,0.0001,0.0001);
	ccan[ican]->cd(1);
		hzslope->SetMinimum(0.85);
		hzslope->SetMaximum(1.15);
		hzslope->SetStats(0);
		hzslope->Draw();
	ccan[ican]->cd(2);
		hzslope1D->Draw();
	ccan[ican]->cd(); ccan[ican]->Update();
	ccan[ican]->Print(psfile.Data());
	gStyle->SetOptStat(0);


	hdzzrm_cal11A->FitSlicesY();
	hdzzevt_cal11A->FitSlicesY();
	hdzztime_cal11A->FitSlicesY();
	TH1D *hdzzrm_cal11A_mean = (TH1D*)gDirectory->Get("hdzzrm_cal11A_1");
		hdzzrm_cal11A_mean->SetName("hdzzrm_cal11A_mean");
	TH1D *hdzzrm_cal11A_sigm = (TH1D*)gDirectory->Get("hdzzrm_cal11A_2");
		hdzzrm_cal11A_sigm->SetName("hdzzrm_cal11A_sigm");
	TH1D *hdzzevt_cal11A_mean = (TH1D*)gDirectory->Get("hdzzevt_cal11A_1");
		hdzzevt_cal11A_mean->SetName("hdzzevt_cal11A_mean");
	TH1D *hdzzevt_cal11A_sigm = (TH1D*)gDirectory->Get("hdzzevt_cal11A_2");
		hdzzevt_cal11A_sigm->SetName("hdzzevt_cal11A_sigm");
	TH1D *hdzztime_cal11A_mean = (TH1D*)gDirectory->Get("hdzztime_cal11A_1");
		hdzztime_cal11A_mean->SetName("hdzztime_cal11A_mean");
	TH1D *hdzztime_cal11A_sigm = (TH1D*)gDirectory->Get("hdzztime_cal11A_2");
		hdzztime_cal11A_sigm->SetName("hdzztime_cal11A_sigm");

	//---- 11A dzz vs refmult
	++ican; sprintf(buf,"ccan%d",ican);
	ccan[ican] = new TCanvas(buf,buf,0,0,800.,800.*(7.5/10.));
	ccan[ican]->SetFillColor(10);
	ccan[ican]->cd(); ccan[ican]->Divide(3,3,0.0001,0.0001);
	ccan[ican]->cd(1);
		hdzzrm_cal11A->SetTitle("[Zvtx(TPC)-Zvtx(VPD)] vs. refmult");
		hdzzrm_cal11A->Draw("col");
	ccan[ican]->cd(2);
		hdzzevt_cal11A->SetTitle("[Zvtx(TPC)-Zvtx(VPD)] vs. entry");
		hdzzevt_cal11A->Draw("col");
	ccan[ican]->cd(3);
		hdzztime_cal11A->SetTitle("[Zvtx(TPC)-Zvtx(VPD)] vs. time");
		hdzztime_cal11A->Draw("col");
	ccan[ican]->cd(4);
		hdzzrm_cal11A_mean->SetMarkerStyle(20);
		hdzzrm_cal11A_mean->SetMarkerSize(0.7);
		hdzzrm_cal11A_mean->SetMinimum(-5);
		hdzzrm_cal11A_mean->SetMaximum( 5);
		hdzzrm_cal11A_mean->SetTitle("#mu[Zvtx(TPC)-Zvtx(VPD)] vs. refmult");
		hdzzrm_cal11A_mean->Draw();
	ccan[ican]->cd(5);
		hdzzevt_cal11A_mean->SetMarkerStyle(20);
		hdzzevt_cal11A_mean->SetMarkerSize(0.7);
		hdzzevt_cal11A_mean->SetMinimum(-5);
		hdzzevt_cal11A_mean->SetMaximum( 5);
		hdzzevt_cal11A_mean->SetTitle("#mu[Zvtx(TPC)-Zvtx(VPD)] vs. entry");
		hdzzevt_cal11A_mean->Draw();
	ccan[ican]->cd(6);
		hdzztime_cal11A_mean->SetMarkerStyle(20);
		hdzztime_cal11A_mean->SetMarkerSize(0.7);
		hdzztime_cal11A_mean->SetMinimum(-5);
		hdzztime_cal11A_mean->SetMaximum( 5);
		hdzztime_cal11A_mean->SetTitle("#mu[Zvtx(TPC)-Zvtx(VPD)] vs. time");
		hdzztime_cal11A_mean->Draw();
	ccan[ican]->cd(7);
		hdzzrm_cal11A_sigm->SetMarkerStyle(20);
		hdzzrm_cal11A_sigm->SetMarkerSize(0.7);
		hdzzrm_cal11A_sigm->SetMinimum( 0);
		hdzzrm_cal11A_sigm->SetMaximum(10);
		hdzzrm_cal11A_sigm->SetTitle("#sigma[Zvtx(TPC)-Zvtx(VPD)] vs. refmult");
		hdzzrm_cal11A_sigm->Draw();
	ccan[ican]->cd(8);
		hdzzevt_cal11A_sigm->SetMarkerStyle(20);
		hdzzevt_cal11A_sigm->SetMarkerSize(0.7);
		hdzzevt_cal11A_sigm->SetMinimum( 0);
		hdzzevt_cal11A_sigm->SetMaximum(10);
		hdzzevt_cal11A_sigm->SetTitle("#sigma[Zvtx(TPC)-Zvtx(VPD)] vs. entry");
		hdzzevt_cal11A_sigm->Draw();
	ccan[ican]->cd(9);
		hdzztime_cal11A_sigm->SetMarkerStyle(20);
		hdzztime_cal11A_sigm->SetMarkerSize(0.7);
		hdzztime_cal11A_sigm->SetMinimum( 0);
		hdzztime_cal11A_sigm->SetMaximum(10);
		hdzztime_cal11A_sigm->SetTitle("#sigma[Zvtx(TPC)-Zvtx(VPD)] vs. time");
		hdzztime_cal11A_sigm->Draw();
	ccan[ican]->cd(); ccan[ican]->Update();
	ccan[ican]->Print(psfile.Data());
	gStyle->SetOptStat(0);

  	
  	//---- A distributions...
  	//
	++ican; sprintf(buf,"ccan%d",ican);
	ccan[ican] = new TCanvas(buf,buf,0,0,800.,800.*(7.5/10.));
	ccan[ican]->SetFillColor(10);
	ccan[ican]->cd(); ccan[ican]->Divide(5,4,0.0001,0.0001);
	for (int idet=0;idet<NDETSIDE;idet++){
		ccan[ican]->cd(1+idet);
		int idetew	= idet;
		gPad->SetLogy(1);
		hA[idetew]->Draw();
		hA_even[idetew]->Draw("same");
		hA_odd[idetew]->Draw("same");
	}
	ccan[ican]->cd(); ccan[ican]->Update();
	ccan[ican]->Print(psfile.Data());
	//
	++ican; sprintf(buf,"ccan%d",ican);
	ccan[ican] = new TCanvas(buf,buf,0,0,800.,800.*(7.5/10.));
	ccan[ican]->SetFillColor(10);
	ccan[ican]->cd(); ccan[ican]->Divide(5,4,0.0001,0.0001);
	for (int idet=0;idet<NDETSIDE;idet++){
		ccan[ican]->cd(1+idet);
		int idetew	= idet + 19;
		gPad->SetLogy(1);
		hA[idetew]->Draw();
		hA_even[idetew]->Draw("same");
		hA_odd[idetew]->Draw("same");
	}
	ccan[ican]->cd(); ccan[ican]->Update();
	ccan[ican]->Print(psfile.Data());
 
  	//---- plot correction functions from cal code and downsampled versions...
  	//
	++ican; sprintf(buf,"ccan%d",ican);
	ccan[ican] = new TCanvas(buf,buf,0,0,800.,800.*(7.5/10.));
	ccan[ican]->SetFillColor(10);
	ccan[ican]->cd(); ccan[ican]->Divide(5,4,0.0001,0.0001);
	for (int idet=0;idet<NDETSIDE;idet++){
		ccan[ican]->cd(1+idet);
		int idetew	= idet;
		if (iChoice==1||iChoice==3){
			fcorr[idetew]->SetMinimum(-5);
			fcorr[idetew]->SetMaximum( 5);
		}
		fcorr[idetew]->Draw("AL");
		if (fcorrA[idetew]->GetN()) fcorrA[idetew]->Draw("L");
		if (fcorrB[idetew]->GetN()) fcorrB[idetew]->Draw("L");
		if (fcorrC[idetew]->GetN()) fcorrC[idetew]->Draw("L");
	}
	ccan[ican]->cd(); ccan[ican]->Update();
	ccan[ican]->Print(psfile.Data());
	//
	++ican; sprintf(buf,"ccan%d",ican);
	ccan[ican] = new TCanvas(buf,buf,0,0,800.,800.*(7.5/10.));
	ccan[ican]->SetFillColor(10);
	ccan[ican]->cd(); ccan[ican]->Divide(5,4,0.0001,0.0001);
	for (int idet=0;idet<NDETSIDE;idet++){
		ccan[ican]->cd(1+idet);
		int idetew	= idet + 19;
		if (iChoice==1||iChoice==3){
			fcorr[idetew]->SetMinimum(-5);
			fcorr[idetew]->SetMaximum( 5);
		}
		fcorr[idetew]->Draw("AL");
		if (fcorrA[idetew]->GetN()) fcorrA[idetew]->Draw("L");
		if (fcorrB[idetew]->GetN()) fcorrB[idetew]->Draw("L");
		if (fcorrC[idetew]->GetN()) fcorrC[idetew]->Draw("L");
	}
	ccan[ican]->cd(); ccan[ican]->Update();
	ccan[ican]->Print(psfile.Data());
	//
	++ican; sprintf(buf,"ccan%d",ican);
	ccan[ican] = new TCanvas(buf,buf,0,0,800.,300.);
	ccan[ican]->SetFillColor(10);
	ccan[ican]->cd(); ccan[ican]->Divide(3,1,0.0001,0.0001);
	int kdet	= 0;
	int idetew	= kdet;
	ccan[ican]->cd(1);
		gPad->SetLogx(1);
		if (iChoice==1||iChoice==3){ ++iframe; frame[iframe] = (TH1F*)gPad->DrawFrame(5.,-2.5,3990,1.5); } else
		                           { ++iframe; frame[iframe] = (TH1F*)gPad->DrawFrame(5.,-2.5,  64,1.5); }
		frame[iframe]->GetXaxis()->SetLabelSize(0.06);
		frame[iframe]->GetYaxis()->SetLabelSize(0.06);
		frame[iframe]->GetXaxis()->SetNdivisions(4);
		frame[iframe]->GetXaxis()->SetLabelOffset(-0.002);
		fcorr[idetew]->Draw("L");
		if (fcorrA[idetew]->GetN()) fcorrA[idetew]->Draw("L");
	ccan[ican]->cd(2);
		gPad->SetLogx(1);
		if (iChoice==1||iChoice==3){ ++iframe; frame[iframe] = (TH1F*)gPad->DrawFrame(5.,-2.5,3990,1.5); } else
		                           { ++iframe; frame[iframe] = (TH1F*)gPad->DrawFrame(5.,-2.5,  64,1.5); }
		frame[iframe]->GetXaxis()->SetLabelSize(0.06);
		frame[iframe]->GetYaxis()->SetLabelSize(0.06);
		frame[iframe]->GetXaxis()->SetNdivisions(4);
		frame[iframe]->GetXaxis()->SetLabelOffset(-0.002);
		fcorr[idetew]->Draw("L");
		if (fcorrB[idetew]->GetN()) fcorrB[idetew]->Draw("L");
	ccan[ican]->cd(3);
		gPad->SetLogx(1);
		if (iChoice==1||iChoice==3){ ++iframe; frame[iframe] = (TH1F*)gPad->DrawFrame(5.,-2.5,3990,1.5); } else
		                           { ++iframe; frame[iframe] = (TH1F*)gPad->DrawFrame(5.,-2.5,  64,1.5); }
		frame[iframe]->GetXaxis()->SetLabelSize(0.06);
		frame[iframe]->GetYaxis()->SetLabelSize(0.06);
		frame[iframe]->GetXaxis()->SetNdivisions(4);
		frame[iframe]->GetXaxis()->SetLabelOffset(-0.002);
		fcorr[idetew]->Draw("L");
		if (fcorrC[idetew]->GetN()) fcorrC[idetew]->Draw("L");
	ccan[ican]->cd(); ccan[ican]->Update();
	ccan[ican]->Print(psfile.Data());

// 
// 	//---- plot slewed data and calibration curves....
// 	//
// //	int ys	= 0;
// 	int ks	= 0;
//   	++ican; sprintf(buf,"ccan%d",ican);
//   	ccan[ican] = new TCanvas(buf,buf,0,0,800.,800.*(7.5/10.));
//   	ccan[ican]->SetFillColor(10);
//   	ccan[ican]->cd(); ccan[ican]->Divide(5,4,0.0001,0.0001);
// 	for (int idet=0;idet<NDETSIDE;idet++){
// 		int idetew	= ks*NDETSIDE + idet;
//   		ccan[ican]->cd(1+idet);
// 			hslew[idet][ks]->Draw("col");
// 			//fcorr_TAC[idetew]->Draw("L");
// 			//fcorrA_TAC[idetew]->Draw("L");
// 			fcorr[idetew]->Draw("L");
// 			fcorrA[idetew]->Draw("L");
// 	}
//   	ccan[ican]->cd(); ccan[ican]->Update();
//   	ccan[ican]->Print(psfileO.Data());
// 	//
// 	++ican; sprintf(buf,"ccan%d",ican);
//   	ccan[ican] = new TCanvas(buf,buf,0,0,800.,800.*(7.5/10.));
//   	ccan[ican]->SetFillColor(10);
//   	ccan[ican]->cd(); ccan[ican]->Divide(5,4,0.0001,0.0001);
// 	ks	= 1;
// 	for (int idet=0;idet<NDETSIDE;idet++){
// 		int idetew	= ks*NDETSIDE + idet;
//   		ccan[ican]->cd(1+idet);
// 			hslew[idet][ks]->Draw("col");
// 			//fcorr_TAC[idetew]->Draw("L");
// 			//fcorrA_TAC[idetew]->Draw("L");
// 			fcorr[idetew]->Draw("L");
// 			fcorrA[idetew]->Draw("L");
// 	}
//   	ccan[ican]->cd(); ccan[ican]->Update();
//   	ccan[ican]->Print(psfileO.Data());
	//
	//---- plot corrected data....
	//
	int ks	= 0;
  	++ican; sprintf(buf,"ccan%d",ican);
  	ccan[ican] = new TCanvas(buf,buf,0,0,800.,800.*(7.5/10.));
  	ccan[ican]->SetFillColor(10);
  	ccan[ican]->cd(); ccan[ican]->Divide(5,4,0.0001,0.0001);
	for (int idet=0;idet<NDETSIDE;idet++){
  		ccan[ican]->cd(1+idet);
			hcorr[idet][ks]->Draw("col");
	}
  	ccan[ican]->cd(); ccan[ican]->Update();
  	ccan[ican]->Print(psfileO.Data());
	//
	++ican; sprintf(buf,"ccan%d",ican);
  	ccan[ican] = new TCanvas(buf,buf,0,0,800.,800.*(7.5/10.));
  	ccan[ican]->SetFillColor(10);
  	ccan[ican]->cd(); ccan[ican]->Divide(5,4,0.0001,0.0001);
	ks	= 1;
	for (int idet=0;idet<NDETSIDE;idet++){
  		ccan[ican]->cd(1+idet);
			hcorr[idet][ks]->Draw("col");
	}
  	ccan[ican]->cd(); ccan[ican]->Update();
  	ccan[ican]->Print(psfileO.Data());

  	++ican; sprintf(buf,"ccan%d",ican);
  	ccan[ican] = new TCanvas(buf,buf,0,0,800.,800.*(7.5/10.));
  	ccan[ican]->SetFillColor(10);
  	ccan[ican]->cd(); ccan[ican]->Divide(2,2,0.0001,0.0001);
  		ccan[ican]->cd(1);
  			hzz_rawch0ch0->Draw("colz");
  			fzz_rawch0ch0_1->SetRange(-50,50);
  			fzz_rawch0ch0_1->Draw("same");
  		ccan[ican]->cd(2);
  			hdzz_rawch0ch0->Draw();
			fgau_rawch0ch0->Draw("same");
			++itext; text[itext]->DrawLatex(0.16,0.8,Form("#mu = %4.1f#pm%5.3f cm",fgau_rawch0ch0->GetParameter(1),fgau_rawch0ch0->GetParError(1)));
			++itext; text[itext]->DrawLatex(0.16,0.7,Form("#sigma = %4.1f#pm%5.3f cm",std::fabs(fgau_rawch0ch0->GetParameter(2)),fgau_rawch0ch0->GetParError(2)));
  		ccan[ican]->cd(3);
 			hzz_rawch0ch0_1->SetMarkerStyle(20);
  			hzz_rawch0ch0_1->Draw();
  			fzz_rawch0ch0_1->Draw("same");
  		ccan[ican]->cd(4);
 			hzz_rawch0ch0_2->SetMinimum(0);
 			hzz_rawch0ch0_2->SetMarkerStyle(20);
  			hzz_rawch0ch0_2->Draw();
  		ccan[ican]->cd(5);
  	ccan[ican]->cd(); ccan[ican]->Update();
  	ccan[ican]->Print(psfile.Data());
	//
  	++ican; sprintf(buf,"ccan%d",ican);
  	ccan[ican] = new TCanvas(buf,buf,0,0,800.,800.*(7.5/10.));
  	ccan[ican]->SetFillColor(10);
  	ccan[ican]->cd(); ccan[ican]->Divide(2,2,0.0001,0.0001);
  		ccan[ican]->cd(1);
  			hzz_rawch0ch0_peri->Draw("colz");
  			fzz_rawch0ch0_peri_1->SetRange(-50,50);
  			fzz_rawch0ch0_peri_1->Draw("same");
  		ccan[ican]->cd(2);
  			hdzz_rawch0ch0_peri->Draw();
			fgau_rawch0ch0_peri->Draw("same");
			++itext; text[itext]->DrawLatex(0.16,0.8,Form("#mu = %4.1f#pm%5.3f cm",fgau_rawch0ch0_peri->GetParameter(1),fgau_rawch0ch0_peri->GetParError(1)));
			++itext; text[itext]->DrawLatex(0.16,0.7,Form("#sigma = %4.1f#pm%5.3f cm",std::fabs(fgau_rawch0ch0_peri->GetParameter(2)),fgau_rawch0ch0_peri->GetParError(2)));
  		ccan[ican]->cd(3);
 			hzz_rawch0ch0_peri_1->SetMarkerStyle(20);
  			hzz_rawch0ch0_peri_1->Draw();
  			fzz_rawch0ch0_peri_1->Draw("same");
  		ccan[ican]->cd(4);
 			hzz_rawch0ch0_peri_2->SetMinimum(0);
 			hzz_rawch0ch0_peri_2->SetMarkerStyle(20);
  			hzz_rawch0ch0_peri_2->Draw();
  		ccan[ican]->cd(5);
  	ccan[ican]->cd(); ccan[ican]->Update();
  	ccan[ican]->Print(psfile.Data());
	//
  	++ican; sprintf(buf,"ccan%d",ican);
  	ccan[ican] = new TCanvas(buf,buf,0,0,800.,800.*(7.5/10.));
  	ccan[ican]->SetFillColor(10);
  	ccan[ican]->cd(); ccan[ican]->Divide(2,2,0.0001,0.0001);
  		ccan[ican]->cd(1);
  			hzz_rawch0ch0_cent->Draw("colz");
  			fzz_rawch0ch0_cent_1->SetRange(-50,50);
  			fzz_rawch0ch0_cent_1->Draw("same");
  		ccan[ican]->cd(2);
  			hdzz_rawch0ch0_cent->Draw();
			fgau_rawch0ch0_cent->Draw("same");
			++itext; text[itext]->DrawLatex(0.16,0.8,Form("#mu = %4.1f#pm%5.3f cm",fgau_rawch0ch0_cent->GetParameter(1),fgau_rawch0ch0_cent->GetParError(1)));
			++itext; text[itext]->DrawLatex(0.16,0.7,Form("#sigma = %4.1f#pm%5.3f cm",std::fabs(fgau_rawch0ch0_cent->GetParameter(2)),fgau_rawch0ch0_cent->GetParError(2)));
  		ccan[ican]->cd(3);
 			hzz_rawch0ch0_cent_1->SetMarkerStyle(20);
  			hzz_rawch0ch0_cent_1->Draw();
  			fzz_rawch0ch0_cent_1->Draw("same");
  		ccan[ican]->cd(4);
 			hzz_rawch0ch0_cent_2->SetMinimum(0);
 			hzz_rawch0ch0_cent_2->SetMarkerStyle(20);
  			hzz_rawch0ch0_cent_2->Draw();
  		ccan[ican]->cd(5);
  	ccan[ican]->cd(); ccan[ican]->Update();
  	ccan[ican]->Print(psfile.Data());
	 
 	++ican; sprintf(buf,"ccan%d",ican);
 	ccan[ican] = new TCanvas(buf,buf,0,0,800.,800.*(7.5/10.));
 	ccan[ican]->SetFillColor(10);
 	ccan[ican]->cd(); ccan[ican]->Divide(2,2,0.0001,0.0001);
 		ccan[ican]->cd(1);
 			hzz_raw11->Draw("colz");
  			fzz_raw11_1->SetRange(-50,50);
 			fzz_raw11_1->Draw("same");
		ccan[ican]->cd(2);
			hdzz_raw11->Draw();
			fgau_raw11->Draw("same");
			++itext; text[itext]->DrawLatex(0.16,0.8,Form("#mu = %4.1f#pm%5.3f cm",fgau_raw11->GetParameter(1),fgau_raw11->GetParError(1)));
			++itext; text[itext]->DrawLatex(0.16,0.7,Form("#sigma = %4.1f#pm%5.3f cm",std::fabs(fgau_raw11->GetParameter(2)),fgau_raw11->GetParError(2)));
		ccan[ican]->cd(3);
 			hzz_raw11_1->SetMarkerStyle(20);
 			hzz_raw11_1->Draw();
			fzz_raw11_1->Draw("same");
		ccan[ican]->cd(4);
 			hzz_raw11_2->SetMinimum(0);
 			hzz_raw11_2->SetMarkerStyle(20);
 			hzz_raw11_2->Draw();
 		ccan[ican]->cd(5);
 	ccan[ican]->cd(); ccan[ican]->Update();
 	ccan[ican]->Print(psfile.Data());

 	++ican; sprintf(buf,"ccan%d",ican);
 	ccan[ican] = new TCanvas(buf,buf,0,0,800.,800.*(7.5/10.));
 	ccan[ican]->SetFillColor(10);
 	ccan[ican]->cd(); ccan[ican]->Divide(2,2,0.0001,0.0001);
 		ccan[ican]->cd(1);
 			hzz_cal11->Draw("colz");
  			fzz_cal11_1->SetRange(-50,50);
 			fzz_cal11_1->Draw("same");
		ccan[ican]->cd(2);
			hdzz_cal11->Draw();
			fgau_cal11->Draw("same");
			++itext; text[itext]->DrawLatex(0.16,0.8,Form("#mu = %4.1f#pm%5.3f cm",fgau_cal11->GetParameter(1),fgau_cal11->GetParError(1)));
			++itext; text[itext]->DrawLatex(0.16,0.7,Form("#sigma = %4.1f#pm%5.3f cm",std::fabs(fgau_cal11->GetParameter(2)),fgau_cal11->GetParError(2)));
		ccan[ican]->cd(3);
 			hzz_cal11_1->SetMarkerStyle(20);
 			hzz_cal11_1->Draw();
			fzz_cal11_1->Draw("same");
		ccan[ican]->cd(4);
 			hzz_cal11_2->SetMinimum(0);
 			hzz_cal11_2->SetMarkerStyle(20);
 			hzz_cal11_2->Draw();
 		ccan[ican]->cd(5);
 	ccan[ican]->cd(); ccan[ican]->Update();
 	ccan[ican]->Print(psfile.Data());

 	++ican; sprintf(buf,"ccan%d",ican);
 	ccan[ican] = new TCanvas(buf,buf,0,0,800.,800.*(7.5/10.));
 	ccan[ican]->SetFillColor(10);
 	ccan[ican]->cd(); ccan[ican]->Divide(2,2,0.0001,0.0001);
 		ccan[ican]->cd(1);
 			hzz_calavg->Draw("colz");
  			fzz_calavg_1->SetRange(-50,50);
 			fzz_calavg_1->Draw("same");
		ccan[ican]->cd(2);
			hdzz_calavg->Draw();
			fgau_calavg->Draw("same");
			++itext; text[itext]->DrawLatex(0.16,0.8,Form("#mu = %4.1f#pm%5.3f cm",fgau_calavg->GetParameter(1),fgau_calavg->GetParError(1)));
			++itext; text[itext]->DrawLatex(0.16,0.7,Form("#sigma = %4.1f#pm%5.3f cm",std::fabs(fgau_calavg->GetParameter(2)),fgau_calavg->GetParError(2)));
		ccan[ican]->cd(3);
 			hzz_calavg_1->SetMarkerStyle(20);
 			hzz_calavg_1->Draw();
			fzz_calavg_1->Draw("same");
		ccan[ican]->cd(4);
 			hzz_calavg_2->SetMinimum(0);
 			hzz_calavg_2->SetMarkerStyle(20);
 			hzz_calavg_2->Draw();
 		ccan[ican]->cd(5);
 	ccan[ican]->cd(); ccan[ican]->Update();
 	ccan[ican]->Print(psfile.Data());

 	++ican; sprintf(buf,"ccan%d",ican);
 	ccan[ican] = new TCanvas(buf,buf,0,0,800.,800.*(7.5/10.));
 	ccan[ican]->SetFillColor(10);
 	ccan[ican]->cd(); ccan[ican]->Divide(2,2,0.0001,0.0001);
 		ccan[ican]->cd(1);
 			hzz_cal11A->Draw("colz");
  			fzz_cal11A_1->SetRange(-50,50);
 			fzz_cal11A_1->Draw("same");
		ccan[ican]->cd(2);
			hdzz_cal11A->Draw();
			fgau_cal11A->Draw("same");
			++itext; text[itext]->DrawLatex(0.16,0.8,Form("#mu = %4.1f#pm%5.3f cm",fgau_cal11A->GetParameter(1),fgau_cal11A->GetParError(1)));
			++itext; text[itext]->DrawLatex(0.16,0.7,Form("#sigma = %4.1f#pm%5.3f cm",std::fabs(fgau_cal11A->GetParameter(2)),fgau_cal11A->GetParError(2)));
		ccan[ican]->cd(3);
 			hzz_cal11A_1->SetMarkerStyle(20);
 			hzz_cal11A_1->Draw();
			fzz_cal11A_1->Draw("same");
		ccan[ican]->cd(4);
 			hzz_cal11A_2->SetMinimum(0);
 			hzz_cal11A_2->SetMarkerStyle(20);
 			hzz_cal11A_2->Draw();
 		ccan[ican]->cd(5);
 	ccan[ican]->cd(); ccan[ican]->Update();
 	ccan[ican]->Print(psfile.Data());
	//
 	++ican; sprintf(buf,"ccan%d",ican);
 	ccan[ican] = new TCanvas(buf,buf,0,0,800.,800.*(7.5/10.));
 	ccan[ican]->SetFillColor(10);
 	ccan[ican]->cd(); ccan[ican]->Divide(2,2,0.0001,0.0001);
 		ccan[ican]->cd(1);
 			hzz_cal11A_peri->Draw("colz");
  			fzz_cal11A_peri_1->SetRange(-50,50);
 			fzz_cal11A_peri_1->Draw("same");
		ccan[ican]->cd(2);
			hdzz_cal11A_peri->Draw();
			fgau_cal11A_peri->Draw("same");
			++itext; text[itext]->DrawLatex(0.16,0.8,Form("#mu = %4.1f#pm%5.3f cm",fgau_cal11A_peri->GetParameter(1),fgau_cal11A_peri->GetParError(1)));
			++itext; text[itext]->DrawLatex(0.16,0.7,Form("#sigma = %4.1f#pm%5.3f cm",std::fabs(fgau_cal11A_peri->GetParameter(2)),fgau_cal11A_peri->GetParError(2)));
		ccan[ican]->cd(3);
 			hzz_cal11A_peri_1->SetMarkerStyle(20);
 			hzz_cal11A_peri_1->Draw();
			fzz_cal11A_peri_1->Draw("same");
		ccan[ican]->cd(4);
 			hzz_cal11A_peri_2->SetMinimum(0);
 			hzz_cal11A_peri_2->SetMarkerStyle(20);
 			hzz_cal11A_peri_2->Draw();
 		ccan[ican]->cd(5);
 	ccan[ican]->cd(); ccan[ican]->Update();
 	ccan[ican]->Print(psfile.Data());
	//
 	++ican; sprintf(buf,"ccan%d",ican);
 	ccan[ican] = new TCanvas(buf,buf,0,0,800.,800.*(7.5/10.));
 	ccan[ican]->SetFillColor(10);
 	ccan[ican]->cd(); ccan[ican]->Divide(2,2,0.0001,0.0001);
 		ccan[ican]->cd(1);
 			hzz_cal11A_cent->Draw("colz");
  			fzz_cal11A_cent_1->SetRange(-50,50);
 			fzz_cal11A_cent_1->Draw("same");
		ccan[ican]->cd(2);
			hdzz_cal11A_cent->Draw();
			fgau_cal11A_cent->Draw("same");
			++itext; text[itext]->DrawLatex(0.16,0.8,Form("#mu = %4.1f#pm%5.3f cm",fgau_cal11A_cent->GetParameter(1),fgau_cal11A_cent->GetParError(1)));
			++itext; text[itext]->DrawLatex(0.16,0.7,Form("#sigma = %4.1f#pm%5.3f cm",std::fabs(fgau_cal11A_cent->GetParameter(2)),fgau_cal11A_cent->GetParError(2)));
		ccan[ican]->cd(3);
 			hzz_cal11A_cent_1->SetMarkerStyle(20);
 			hzz_cal11A_cent_1->Draw();
			fzz_cal11A_cent_1->Draw("same");
		ccan[ican]->cd(4);
 			hzz_cal11A_cent_2->SetMinimum(0);
 			hzz_cal11A_cent_2->SetMarkerStyle(20);
 			hzz_cal11A_cent_2->Draw();
 		ccan[ican]->cd(5);
 	ccan[ican]->cd(); ccan[ican]->Update();
 	ccan[ican]->Print(psfile.Data());

 	++ican; sprintf(buf,"ccan%d",ican);
 	ccan[ican] = new TCanvas(buf,buf,0,0,800.,800.*(7.5/10.));
 	ccan[ican]->SetFillColor(10);
 	ccan[ican]->cd(); ccan[ican]->Divide(2,2,0.0001,0.0001);
 		ccan[ican]->cd(1);
 			hzz_cal11B->Draw("colz");
  			fzz_cal11B_1->SetRange(-50,50);
 			fzz_cal11B_1->Draw("same");
		ccan[ican]->cd(2);
			hdzz_cal11B->Draw();
			fgau_cal11B->Draw("same");
			++itext; text[itext]->DrawLatex(0.16,0.8,Form("#mu = %4.1f#pm%5.3f cm",fgau_cal11B->GetParameter(1),fgau_cal11B->GetParError(1)));
			++itext; text[itext]->DrawLatex(0.16,0.7,Form("#sigma = %4.1f#pm%5.3f cm",std::fabs(fgau_cal11B->GetParameter(2)),fgau_cal11B->GetParError(2)));
		ccan[ican]->cd(3);
 			hzz_cal11B_1->SetMarkerStyle(20);
 			hzz_cal11B_1->Draw();
			fzz_cal11B_1->Draw("same");
		ccan[ican]->cd(4);
 			hzz_cal11B_2->SetMinimum(0);
 			hzz_cal11B_2->SetMarkerStyle(20);
 			hzz_cal11B_2->Draw();
 		ccan[ican]->cd(5);
 	ccan[ican]->cd(); ccan[ican]->Update();
 	ccan[ican]->Print(psfile.Data());

 	++ican; sprintf(buf,"ccan%d",ican);
 	ccan[ican] = new TCanvas(buf,buf,0,0,800.,800.*(7.5/10.));
 	ccan[ican]->SetFillColor(10);
 	ccan[ican]->cd(); ccan[ican]->Divide(2,2,0.0001,0.0001);
 		ccan[ican]->cd(1);
 			hzz_cal11C->Draw("colz");
  			fzz_cal11C_1->SetRange(-50,50);
 			fzz_cal11C_1->Draw("same");
		ccan[ican]->cd(2);
			hdzz_cal11C->Draw();
			fgau_cal11C->Draw("same");
			++itext; text[itext]->DrawLatex(0.16,0.8,Form("#mu = %4.1f#pm%5.3f cm",fgau_cal11C->GetParameter(1),fgau_cal11C->GetParError(1)));
			++itext; text[itext]->DrawLatex(0.16,0.7,Form("#sigma = %4.1f#pm%5.3f cm",std::fabs(fgau_cal11C->GetParameter(2)),fgau_cal11C->GetParError(2)));
		ccan[ican]->cd(3);
 			hzz_cal11C_1->SetMarkerStyle(20);
 			hzz_cal11C_1->Draw();
			fzz_cal11C_1->Draw("same");
		ccan[ican]->cd(4);
 			hzz_cal11C_2->SetMinimum(0);
 			hzz_cal11C_2->SetMarkerStyle(20);
 			hzz_cal11C_2->Draw();
 		ccan[ican]->cd(5);
 	ccan[ican]->cd(); ccan[ican]->Update();
 	ccan[ican]->Print(psfile.Data());


 	++ican; sprintf(buf,"ccan%d",ican);
 	ccan[ican] = new TCanvas(buf,buf,0,0,800.,800.*(7.5/10.));
 	ccan[ican]->SetFillColor(10);
 	ccan[ican]->cd(); ccan[ican]->Divide(2,2,0.0001,0.0001);
 		ccan[ican]->cd(1);
 			hzz_cal11Aa->Draw("colz");
  			fzz_cal11Aa_1->SetRange(-50,50);
 			fzz_cal11Aa_1->Draw("same");
		ccan[ican]->cd(2);
			hdzz_cal11Aa->Draw();
			fgau_cal11Aa->Draw("same");
			++itext; text[itext]->DrawLatex(0.16,0.8,Form("#mu = %4.1f#pm%5.3f cm",fgau_cal11Aa->GetParameter(1),fgau_cal11Aa->GetParError(1)));
			++itext; text[itext]->DrawLatex(0.16,0.7,Form("#sigma = %4.1f#pm%5.3f cm",std::fabs(fgau_cal11Aa->GetParameter(2)),fgau_cal11Aa->GetParError(2)));
		ccan[ican]->cd(3);
 			hzz_cal11Aa_1->SetMarkerStyle(20);
 			hzz_cal11Aa_1->Draw();
			fzz_cal11Aa_1->Draw("same");
		ccan[ican]->cd(4);
 			hzz_cal11Aa_2->SetMinimum(0);
 			hzz_cal11Aa_2->SetMarkerStyle(20);
 			hzz_cal11Aa_2->Draw();
 		ccan[ican]->cd(5);
 	ccan[ican]->cd(); ccan[ican]->Update();
 	ccan[ican]->Print(psfile.Data());
	//
 	++ican; sprintf(buf,"ccan%d",ican);
 	ccan[ican] = new TCanvas(buf,buf,0,0,800.,800.*(7.5/10.));
 	ccan[ican]->SetFillColor(10);
 	ccan[ican]->cd(); ccan[ican]->Divide(2,2,0.0001,0.0001);
 		ccan[ican]->cd(1);
 			hzz_cal11Ba->Draw("colz");
  			fzz_cal11Ba_1->SetRange(-50,50);
 			fzz_cal11Ba_1->Draw("same");
		ccan[ican]->cd(2);
			hdzz_cal11Ba->Draw();
			fgau_cal11Ba->Draw("same");
			++itext; text[itext]->DrawLatex(0.16,0.8,Form("#mu = %4.1f#pm%5.3f cm",fgau_cal11Ba->GetParameter(1),fgau_cal11Ba->GetParError(1)));
			++itext; text[itext]->DrawLatex(0.16,0.7,Form("#sigma = %4.1f#pm%5.3f cm",std::fabs(fgau_cal11Ba->GetParameter(2)),fgau_cal11Ba->GetParError(2)));
		ccan[ican]->cd(3);
 			hzz_cal11Ba_1->SetMarkerStyle(20);
 			hzz_cal11Ba_1->Draw();
			fzz_cal11Ba_1->Draw("same");
		ccan[ican]->cd(4);
 			hzz_cal11Ba_2->SetMinimum(0);
 			hzz_cal11Ba_2->SetMarkerStyle(20);
 			hzz_cal11Ba_2->Draw();
 		ccan[ican]->cd(5);
 	ccan[ican]->cd(); ccan[ican]->Update();
 	ccan[ican]->Print(psfile.Data());
	//
 	++ican; sprintf(buf,"ccan%d",ican);
 	ccan[ican] = new TCanvas(buf,buf,0,0,800.,800.*(7.5/10.));
 	ccan[ican]->SetFillColor(10);
 	ccan[ican]->cd(); ccan[ican]->Divide(2,2,0.0001,0.0001);
 		ccan[ican]->cd(1);
 			hzz_cal11Ca->Draw("colz");
  			fzz_cal11Ca_1->SetRange(-50,50);
 			fzz_cal11Ca_1->Draw("same");
		ccan[ican]->cd(2);
			hdzz_cal11Ca->Draw();
			fgau_cal11Ca->Draw("same");
			++itext; text[itext]->DrawLatex(0.16,0.8,Form("#mu = %4.1f#pm%5.3f cm",fgau_cal11Ca->GetParameter(1),fgau_cal11Ca->GetParError(1)));
			++itext; text[itext]->DrawLatex(0.16,0.7,Form("#sigma = %4.1f#pm%5.3f cm",std::fabs(fgau_cal11Ca->GetParameter(2)),fgau_cal11Ca->GetParError(2)));
		ccan[ican]->cd(3);
 			hzz_cal11Ca_1->SetMarkerStyle(20);
 			hzz_cal11Ca_1->Draw();
			fzz_cal11Ca_1->Draw("same");
		ccan[ican]->cd(4);
 			hzz_cal11Ca_2->SetMinimum(0);
 			hzz_cal11Ca_2->SetMarkerStyle(20);
 			hzz_cal11Ca_2->Draw();
 		ccan[ican]->cd(5);
 	ccan[ican]->cd(); ccan[ican]->Update();
 	ccan[ican]->Print(psfile.Data());



 	++ican; sprintf(buf,"ccan%d",ican);
 	ccan[ican] = new TCanvas(buf,buf,0,0,800.,800.*(7.5/10.));
 	ccan[ican]->SetFillColor(10);
 	ccan[ican]->cd(); ccan[ican]->Divide(5,2,0.0001,0.0001);
		ccan[ican]->cd(1);
			hdzz_rawch0ch0->Draw();
			fgau_rawch0ch0->Draw("same");
			++itext; text[itext]->DrawLatex(0.16,0.8,Form("#mu = %4.1f#pm%5.3f cm",fgau_rawch0ch0->GetParameter(1),fgau_rawch0ch0->GetParError(1)));
			++itext; text[itext]->DrawLatex(0.16,0.7,Form("#sigma = %4.1f#pm%5.3f cm",std::fabs(fgau_rawch0ch0->GetParameter(2)),fgau_rawch0ch0->GetParError(2)));
		ccan[ican]->cd(2);
			hdzz_raw11->Draw();
			fgau_raw11->Draw("same");
			++itext; text[itext]->DrawLatex(0.16,0.8,Form("#mu = %4.1f#pm%5.3f cm",fgau_raw11->GetParameter(1),fgau_raw11->GetParError(1)));
			++itext; text[itext]->DrawLatex(0.16,0.7,Form("#sigma = %4.1f#pm%5.3f cm",std::fabs(fgau_raw11->GetParameter(2)),fgau_raw11->GetParError(2)));
		ccan[ican]->cd(3);
			hdzz_cal11->Draw();
			fgau_cal11->Draw("same");
			++itext; text[itext]->DrawLatex(0.16,0.8,Form("#mu = %4.1f#pm%5.3f cm",fgau_cal11->GetParameter(1),fgau_cal11->GetParError(1)));
			++itext; text[itext]->DrawLatex(0.16,0.7,Form("#sigma = %4.1f#pm%5.3f cm",std::fabs(fgau_cal11->GetParameter(2)),fgau_cal11->GetParError(2)));
		ccan[ican]->cd(4);
			hdzz_calavg->Draw();
			fgau_calavg->Draw("same");
			++itext; text[itext]->DrawLatex(0.16,0.8,Form("#mu = %4.1f#pm%5.3f cm",fgau_calavg->GetParameter(1),fgau_calavg->GetParError(1)));
			++itext; text[itext]->DrawLatex(0.16,0.7,Form("#sigma = %4.1f#pm%5.3f cm",std::fabs(fgau_calavg->GetParameter(2)),fgau_calavg->GetParError(2)));
		ccan[ican]->cd(5);
			hdzz_cal11A->Draw();
			fgau_cal11A->Draw("same");
			++itext; text[itext]->DrawLatex(0.16,0.8,Form("#mu = %4.1f#pm%5.3f cm",fgau_cal11A->GetParameter(1),fgau_cal11A->GetParError(1)));
			++itext; text[itext]->DrawLatex(0.16,0.7,Form("#sigma = %4.1f#pm%5.3f cm",std::fabs(fgau_cal11A->GetParameter(2)),fgau_cal11A->GetParError(2)));
		ccan[ican]->cd(6);
			hdzz_cal11B->Draw();
			fgau_cal11B->Draw("same");
			++itext; text[itext]->DrawLatex(0.16,0.8,Form("#mu = %4.1f#pm%5.3f cm",fgau_cal11B->GetParameter(1),fgau_cal11B->GetParError(1)));
			++itext; text[itext]->DrawLatex(0.16,0.7,Form("#sigma = %4.1f#pm%5.3f cm",std::fabs(fgau_cal11B->GetParameter(2)),fgau_cal11B->GetParError(2)));
		ccan[ican]->cd(7);
			hdzz_cal11C->Draw();
			fgau_cal11C->Draw("same");
			++itext; text[itext]->DrawLatex(0.16,0.8,Form("#mu = %4.1f#pm%5.3f cm",fgau_cal11C->GetParameter(1),fgau_cal11C->GetParError(1)));
			++itext; text[itext]->DrawLatex(0.16,0.7,Form("#sigma = %4.1f#pm%5.3f cm",std::fabs(fgau_cal11C->GetParameter(2)),fgau_cal11C->GetParError(2)));
		ccan[ican]->cd(8);
			hdzz_cal11Aa->Draw();
			fgau_cal11Aa->Draw("same");
			++itext; text[itext]->DrawLatex(0.16,0.8,Form("#mu = %4.1f#pm%5.3f cm",fgau_cal11Aa->GetParameter(1),fgau_cal11Aa->GetParError(1)));
			++itext; text[itext]->DrawLatex(0.16,0.7,Form("#sigma = %4.1f#pm%5.3f cm",std::fabs(fgau_cal11Aa->GetParameter(2)),fgau_cal11Aa->GetParError(2)));
		ccan[ican]->cd(9);
			hdzz_cal11Ba->Draw();
			fgau_cal11Ba->Draw("same");
			++itext; text[itext]->DrawLatex(0.16,0.8,Form("#mu = %4.1f#pm%5.3f cm",fgau_cal11Ba->GetParameter(1),fgau_cal11Ba->GetParError(1)));
			++itext; text[itext]->DrawLatex(0.16,0.7,Form("#sigma = %4.1f#pm%5.3f cm",std::fabs(fgau_cal11Ba->GetParameter(2)),fgau_cal11Ba->GetParError(2)));
		ccan[ican]->cd(10);
			hdzz_cal11Ca->Draw();
			fgau_cal11Ca->Draw("same");
			++itext; text[itext]->DrawLatex(0.16,0.8,Form("#mu = %4.1f#pm%5.3f cm",fgau_cal11Ca->GetParameter(1),fgau_cal11Ca->GetParError(1)));
			++itext; text[itext]->DrawLatex(0.16,0.7,Form("#sigma = %4.1f#pm%5.3f cm",std::fabs(fgau_cal11Ca->GetParameter(2)),fgau_cal11Ca->GetParError(2)));
 	ccan[ican]->cd(); ccan[ican]->Update();
 	ccan[ican]->Print(psfile.Data());

 	++ican; sprintf(buf,"ccan%d",ican);
 	ccan[ican] = new TCanvas(buf,buf,0,0,800.,800.*(7.5/10.));
 	ccan[ican]->SetFillColor(10);
 	ccan[ican]->cd(); ccan[ican]->Divide(1,2,0.0001,0.0001);
		ccan[ican]->cd(1);
			hresults_mean->SetMinimum(-20);
			hresults_mean->SetMaximum( 20);
			hresults_mean->Draw();
			hresults_mean_peri->Draw("same");
			hresults_mean_cent->Draw("same");
		ccan[ican]->cd(2);
			hresults_sigm->SetMinimum(  0);
			hresults_sigm->SetMaximum( 30);
			hresults_sigm->Draw();
			hresults_sigm_peri->Draw("same");
			hresults_sigm_cent->Draw("same");
 	ccan[ican]->cd(); ccan[ican]->Update();
 	ccan[ican]->Print(psfile.Data());

	//=============================================================================	
	//
 	cout<<"..... Loop  Writing plots to root file...."<<endl;	
 	fout->Write();
 	fout->Close();
 	cout<<"..... Loop  Closing ps file...."<<endl;
	ccan[ican]->Print(psfileC.Data());
 	sprintf(buf,"/usr/bin/ps2pdf %s %s",psfile.Data(),pdffile.Data());
 	cout<<"..... Loop  Executing ... "<<buf<<endl;
 	gSystem->Exec(buf);
 	sprintf(buf,"/bin/cp %s %s",pdffile.Data(),WebDirectory.Data());
 	cout<<"..... Loop  Executing ... "<<buf<<endl;
 	gSystem->Exec(buf);
	cout<<"..... Loop  Done...."<<endl;	
	
}

//-------------------------------------------------------------------------------------
//
void caleval::FormCorrA(const int idetew){
	double x,y;
	//
	//---- find 8 equal-area ADC slices....
	float atot	= hA[idetew]->GetEntries();
	//cout<<idetew<<" "<<atot<<endl;
	if (atot<300){ 								//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		cout<<"Not enough entries!!  ...idetew="<<idetew<<"  n="<<atot<<endl;
		for (int ibin=1;ibin<=hA[idetew]->GetNbinsX();ibin++){
			float xh		= hA[idetew]->GetBinCenter(ibin);
			int kabin		= ibin/512;
			int k			= fcorrA[idetew]->GetN();
			//if (idetew==8) cout<<"this ... "<<xh<<" "<<kabin<<" "<<k<<" "<<corr2[kabin]<<endl;
			fcorrA[idetew]->SetPoint(k,xh,0.0);
		}
		return;
	}
	//
	float arsum		= 0;
	float check		= 0.125;
	int kabin		=  0;
	int iabin[4096]	= {0};
	for (int ibin=1;ibin<=hA[idetew]->GetNbinsX();ibin++){
		float an	 = hA[idetew]->GetBinContent(ibin);
		arsum		+= an/atot;
		if (arsum>check){
			kabin		+=   1  ;
			check		+= 0.125;					
		}
		if (kabin>7){ kabin = 7; check=1.0; }
		iabin[ibin-1]	= kabin;
		//if (idetew==10) cout<<idetew<<" \t "<<ibin<<" "<<an<<" \t "<<arsum<<" \t "<<check<<" "<<kabin<<endl;
	}
	//	
	int nnbin[8]	= {0};
	float anum[8]={0},aden[8]={0};
	for (int ibin=1;ibin<=hA[idetew]->GetNbinsX();ibin++){
		int inbin		 = iabin[ibin-1];
		nnbin[inbin]	+= 1;
		float xh		 = hA[idetew]->GetBinCenter(ibin);
		float nh		 = hA[idetew]->GetBinContent(ibin);
		float corr		 = fcorr[idetew]->Eval(xh);
		//if (idetew==8) cout<<"this ... "<<ibin<<" "<<xh<<" "<<corr<<endl;
		anum[inbin]		+= nh*corr;
		aden[inbin]		+= nh;
		//
		if (inbin%2==0){ hA_even[idetew]->Fill(xh,nh); } else
		if (inbin%2==1){ hA_odd[idetew]->Fill(xh,nh); }
		//
	}
	float corr2[8]		= {0};
	for (int inbin=0;inbin<8;inbin++){
		corr2[inbin]	= anum[inbin]/aden[inbin];
		//if (idetew==8) cout<<"this ... "<<inbin<<" "<<nnbin[inbin]<<" \t "<<anum[inbin]<<" "<<aden[inbin]<<" "<<corr2[inbin]<<endl;
	}
	for (int ibin=1;ibin<=hA[idetew]->GetNbinsX();ibin++){
		float xh		= hA[idetew]->GetBinCenter(ibin);
		int kabin		= iabin[ibin-1];
		int k			= fcorrA[idetew]->GetN();
		//if (idetew==8) cout<<"this ... "<<xh<<" "<<kabin<<" "<<k<<" "<<corr2[kabin]<<endl;
		fcorrA[idetew]->SetPoint(k,xh,corr2[kabin]);
	}
	//
}
//
void caleval::FormCorrB(const int idetew){
	double x,y;
	//
	//---- find 8 equal-area ADC slices....
	float atot		= hA[idetew]->GetEntries();
	int   nabins	= hA[idetew]->GetNbinsX();
	if (atot<5) return; 
	//
	float arsum				= 0;
	float check				= 0.125;
	int kabin				=  0;
	float arsum_bin[4096]	= {0};
	for (int ibin=1;ibin<=nabins;ibin++){
		float ax	 = hA[idetew]->GetBinCenter(ibin);
		float an	 = hA[idetew]->GetBinContent(ibin);
		arsum		+= an/atot;
		arsum_bin[ibin-1]	= arsum;
	}
	int iabin[4096]			= {0};
	int kabinstart			=  0;
	int kabinstop			=  0;
	int kabinlower[8]		= {0};
	int kabinupper[8]		= {0};
	for (int ibin=1;ibin<=nabins;ibin++){
		float ax	 = hA[idetew]->GetBinCenter(ibin);
		float an	 = hA[idetew]->GetBinContent(ibin);
		if (kabinstart==0 && an>0) kabinstart = ibin; 
		if (ibin>nabins/2 && an>=1) kabinstop = ibin; 
		if (arsum_bin[ibin-1]>check){
			kabin		+=   1  ;
			check		+= 0.125;
			if (kabin>7){ kabin = 7; check=1.0; }
			if (kabinstart>0 && kabinlower[kabin]==0 ){
				kabinlower[kabin]	= ibin;
			}
		}
		iabin[ibin-1]	= kabin;
		//if (idetew==0){
		//	cout<<idetew<<" \t "<<ibin<<" "<<an<<" \t "<<arsum_bin[ibin]<<" \t "
		//		<<check<<" "<<kabin<<" \t "
		//		<<kabinstart<<" \t "
		//		<<fcorr[idetew]->Eval(ax)
		//		<<endl;
		//}
	}
	//
	//if (idetew==0){
	//cout<<".... "<<idetew<<" "<<atot<<" "<<nabins<<" \t "
	//	<<kabinstart<<" "<<kabinstop<<" \t "
	//	<<hA[idetew]->GetBinCenter(kabinstart)<<" "<<hA[idetew]->GetBinCenter(kabinstop)
	//	<<endl;
	//}
	kabinlower[0]	= kabinstart;
	kabinupper[7]	= kabinstop;
	float slope[8]	= {0};
	float yinte[8]	= {0};
	for (int i=0;i<8;i++){
		if (i<7)kabinupper[i]	= kabinlower[i+1]-1;
		float x0	= hA[idetew]->GetBinCenter(kabinlower[i]);
		float x1	= hA[idetew]->GetBinCenter(kabinupper[i]);
		float y0	= fcorr[idetew]->Eval(x0);
		float y1	= fcorr[idetew]->Eval(x1);
		//
		slope[i]	= (y1-y0)/(x1-x0);
		yinte[i]	= y0 - slope[i]*x0;
		//
		//if (idetew==0){
		//cout<<".... "<<i<<" "<<kabinlower[i]<<" "<<kabinupper[i]<<" \t "
		//	<<slope[i]<<" "<<yinte[i]
		//	<<endl;
		//}
	}
 	for (int ibin=1;ibin<=hA[idetew]->GetNbinsX();ibin++){
 		int kabin	= iabin[ibin-1];
 		float sl	= slope[kabin];
 		float yi	= yinte[kabin];
 		float xh	= hA[idetew]->GetBinCenter(ibin);
 		float yc	= sl*xh + yi;
 		int k		= fcorrB[idetew]->GetN();
 		fcorrB[idetew]->SetPoint(k,xh,yc);
	}
	//
}
//
void caleval::FormCorrC(const int idetew){
	//
	if (idetew>0) return;
	//
	if (iChoice==0||iChoice==3){ A_nbin= 128; A_min= 0  ; A_max=  64. ; } else
	if (iChoice==1||iChoice==2){ A_nbin= 128; A_min=-0.5; A_max=4095.5; }
	TH1D *slewnum	= new TH1D("slewnum","slewnum",A_nbin,A_min,A_max);
	TH1D *slewsum	= new TH1D("slewsum","slewsum",A_nbin,A_min,A_max);
	//
	for (int idet=0;idet<38;idet++){
		float atot		= hA[idet]->GetEntries();
		int   nabins	= hA[idet]->GetNbinsX();
		if (atot<5) continue; 
		for (int ibin=1;ibin<=nabins;ibin++){
			float ax	= hA[idet]->GetBinCenter(ibin);
			float an	= hA[idet]->GetBinContent(ibin);
			float corr	= fcorr[idet]->Eval(ax);
			slewnum->Fill(ax,1.0);
			slewsum->Fill(ax,corr);
		}
	}
	//
	slewsum->Divide(slewnum);
	//
	float yoffnum[38]	= {0};
	float yoffsum[38]	= {0};
	for (int idet=0;idet<38;idet++){
		float atot		= hA[idet]->GetEntries();
		int   nabins	= hA[idet]->GetNbinsX();
		if (atot<5) continue; 
		for (int ibin=1;ibin<=nabins;ibin++){
			float ax	= hA[idet]->GetBinCenter(ibin);
			float an	= hA[idet]->GetBinContent(ibin);
			float corr	= fcorr[idet]->Eval(ax);
			int   ibc	= slewsum->FindBin(ax);
			float corra	= slewsum->GetBinContent(ibc);
			yoffnum[idet]	+= an;
			yoffsum[idet]	+= an*(corr-corra);
		}
		if (yoffnum[idet]>0){
			yoffsum[idet]	/= yoffnum[idet];
		} else {
			yoffsum[idet]	 = 0;
		}
	}
	//
	for (int idet=0;idet<38;idet++){
		float atot		= hA[idet]->GetEntries();
		int   nabins	= hA[idet]->GetNbinsX();
		if (atot<5) continue; 
		for (int ibin=1;ibin<=nabins;ibin++){
	 		int np		= fcorrC[idet]->GetN();
			float ax	= hA[idet]->GetBinCenter(ibin);
			int   ibc	= slewsum->FindBin(ax);
			float corra	= slewsum->GetBinContent(ibc);
	 		fcorrC[idet]->SetPoint(np,ax,corra+yoffsum[idet]);
		}	
		//
	}
	//
	delete slewsum;
	delete slewnum;
	//	
}

//-------------------------------------------------------------------------------------
//
float caleval::Calibrate(int idetew, float Aval, float Tval, int isteer){
	if (idetew<0||idetew>=38) return 0;
	float shift;
	if (isteer==0){ shift	= fcorr[idetew]->Eval(Aval);  } else
	if (isteer==1){ shift	= fcorrA[idetew]->Eval(Aval); } else
	if (isteer==2){ shift	= fcorrB[idetew]->Eval(Aval); } else
	if (isteer==3){ shift	= fcorrC[idetew]->Eval(Aval); } else
	              { cout<<"caleval::Calibrate ... unknown isteer "<<isteer<<endl; exit(0); }
	//if (idetew==0||idetew==19){
	//	cout<<idetew<<" "<<Aval<<" "<<Tval<<" \t "<<isteer<<" "<<shift<<" "<<Tval-shift<<endl;
	//}
	return Tval-shift;
}

//-------------------------------------------------------------------------------------
//
bool DoFitSliceY(TH2D *h){
	//
	bool ret	= false;
	//
	const int npar=4;
	int nbx		= h->GetNbinsX();
	float xl	= h->GetXaxis()->GetXmin();
	float xu	= h->GetXaxis()->GetXmax();
	//
	//---- create results hists....
	TString name 		= TString(Form("%s",h->GetName()));
	TH1D **hlist 		= new TH1D*[npar];
	for (int ipar=0;ipar<npar;ipar++){
		TString name2	= name + TString(Form("_%d",ipar));
		delete gDirectory->FindObject(name2);
		hlist[ipar]		= new TH1D(name2.Data(),name2.Data(),nbx,xl,xu);
	}
	//
	//
	bool 	goodfit;
	int 	nslicesfit	= 0;
	for (int ibx=1;ibx<=nbx;ibx++){
		//
		TH1D* hp	= (TH1D*)h->ProjectionY("_temp",ibx,ibx,"E");
		if (hp==0) continue;
		if (hp->GetEntries()<10){ delete hp; continue; }
		float x		= xl + ((float)ibx/(float)nbx)*(xu-xl);
		//
		TF1 *thisg	= new TF1("thisg","gaus",-150,150);
		goodfit	= true;
		FitGaussian(hp,thisg,0);
		for (int ipar=0;ipar<3;ipar++){
			if (thisg->GetParameter(1)>-100.
			 && thisg->GetParameter(1)< 100.
			 //&& thisg->GetParameter(2)<   0.95*maxsig 
			 && thisg->GetParameter(2)>   1.00){
				hlist[ipar]->SetBinContent(ibx,thisg->GetParameter(ipar));
				hlist[ipar]->SetBinError(ibx,thisg->GetParError(ipar));
				++nslicesfit;
			} else {
				goodfit	= false;
			}
		}
		float ndf	= thisg->GetNDF();
		float chi2	= thisg->GetChisquare();
		float redchi2		= -1;
		if (ndf>2) redchi2	= chi2/ndf;
// 		cout<<ibx<<" "<<hp->GetEntries()<<" \t "
// 			<<(int)goodfit<<" \t "
// 			<<thisg->GetParameter(0)<<" "
// 			<<"(x="<<x<<") "
// 			<<thisg->GetParameter(1)<<" "
// 			<<thisg->GetParameter(2)<<" \t "
// 			<<redchi2<<" "
// 			<<endl;
		//
		delete hp;
		delete thisg;
	}
	//
	delete [] hlist;
	if (nslicesfit>5) ret = true;
	return ret;
}

//-------------------------------------------------------------------------------------
//
void FitGaussian(TH1D *h, TF1 *fgau, int debug){
	//
	int 	nb		= h->GetNbinsX();
	int 	ibpeak	= h->GetMaximumBin();
	float	xbw		= h->GetBinWidth(ibpeak);
	float	xpeak	= h->GetBinCenter(ibpeak);
	float	npeak	= h->GetBinContent(ibpeak);
	float 	xstop	= xpeak + 10*xbw;
	bool	stopped	= false;
	for (int ib=ibpeak;ib<=nb;ib++){
		float n		= h->GetBinContent(ib);
		if (n/npeak<0.4){
			xstop	= h->GetBinCenter(ib);
			if (h->GetBinContent(ib+2)<=n){
				stopped	= true;
				break;
			}
		}
	}
	if (!stopped){ xstop = xpeak + 3.*xbw; }
	float dx	= (xstop - xpeak) + xbw/2.;
	fgau->SetParameter(0,100000);
	fgau->SetParameter(1,xpeak);
	fgau->SetParameter(2,5);
	fgau->SetRange(xpeak-dx,xpeak+dx);
	if (debug){
		h->Fit(fgau,"N0R");
	} else {
		h->Fit(fgau,"QN0R");
	}
	fgau->SetRange(-100,100);
	//
	if (debug){
		cout<<nb<<" "<<ibpeak<<" "<<xpeak<<" "<<xstop<<" "<<dx<<" \t "
			<<xpeak-dx<<" "<<xpeak+dx<<" \t "
			<<fgau->GetParameter(0)<<" "
			<<fgau->GetParameter(1)<<" "
			<<fgau->GetParameter(2)<<" "
			<<endl;
	}
	//
}

//-------------------------------------------------------------------------------------
//
void Norm2D(TH1D* hrm, TH2D* hi, TH2D *ho){
	int nbx	= hi->GetNbinsX();
	int nby	= hi->GetNbinsY();
	for (int ibx=1;ibx<=nbx;ibx++){
		float nev	= hrm->GetBinContent(ibx);
		if (nev>5){
			for (int iby=1;iby<=nby;iby++){
				float val	= hi->GetBinContent(ibx,iby);
				float valn	= val/nev;
				ho->SetBinContent(ibx,iby,valn);
			}
		}
	}

}

//-------------------------------------------------------------------------------------
//
float caleval::Get_rawch0ch0(){
	float dt;
	float zvpd;
	bool docalc	= false;
	if (lit[0][0] && lit[0][1]){
		dt		= Toff[0+19] - Toff[0+0];
		zvpd	= dt*tfactor;
		docalc	= true;
		//
		//cout<<" "<<A[0]<<" "<<Toff[0]<<" \t "
		//	<<" "<<A[19]<<" "<<Toff[19]<<" \t "
		//	<<dt<<" "<<zvpd<<endl;
	}
	if (docalc){ return zvpd; } else { return -999; }	
}

float caleval::Get_raw11(){
	float aearliest[2]	= {0};
	float tearliest[2]	= {0};
	bool  foundside[2];
	for (int is=0;is<2;is++){
		tearliest[is]	= 9999999*tsign;
		foundside[is]	= false;
	}			
	for (int is=0;is<2;is++){
		for (int idet=0;idet<NDETSIDE;idet++){
			int idetew	= idet + is*NDETSIDE;
			if (lit[idet][is]){
				if (tsign*Toff[idetew]<tsign*tearliest[is]){
					tearliest[is]			= Toff[idetew];
					aearliest[is]			= A[idetew];
					idetearliest_raw[is]	= idetew;
					foundside[is]			= true;	
				}
			}
			//cout<<is<<" "<<idet<<" "<<idetew<<" "<<(int)lit[idet][is]<<" "
			//	<<Toff[idetew]<<" "<<tearliest[is]<<endl;
		}
	}
	//
	float dt;
	float zvpd;
	bool docalc	= false;
	if ( foundside[0]&&foundside[1] ){	
		dt		= tearliest[1]	- tearliest[0];
		zvpd	= dt*tfactor;
		docalc	= true;
		//cout<<idetearliest_raw[0]<<" "<<aearliest[0]<<" "<<tearliest[0]<<" \t "
		//	<<idetearliest_raw[1]<<" "<<aearliest[1]<<" "<<tearliest[1]<<" \t "
		//	<<dt<<" "<<zvpd<<endl;
	}
	//
	//
	if (docalc){ return zvpd; } else { return -999; }
}

//-------------------------------------------------------------------------------------
//
float caleval::Get_cal11(){
	float aearliest[2]	= {0};
	float tearliest[2]	= {0};
	bool  foundside[2];
	for (int is=0;is<2;is++){
		tearliest[is]	= 9999999*tsign;
		foundside[is]	= false;
	}			
	for (int is=0;is<2;is++){
		for (int idet=0;idet<NDETSIDE;idet++){
			//
			if (lit[idet][is]){
				int idetew	= idet + is*19;
				float Tcal	= Calibrate(idetew,A[idetew],T[idetew],0);
				//
				//cout<<is<<" "<<idet<<" "<<idetew<<" "<<A[idetew]<<" "<<T[idetew]<<" "<<Tcal<<endl;
				if (tsign*Tcal<tsign*tearliest[is]){
					tearliest[is]			= Tcal;
					aearliest[is]			= A[idetew];
					idetearliest_cal[is]	= idetew;
					foundside[is]			= true;						
				}
			}
		}
	}
	//
	float dt;
	float zvpd;
	bool docalc	= false;
	if ( foundside[0]&&foundside[1] ){
		dt		= tearliest[1]	- tearliest[0];
		zvpd	= dt*tfactor;
		docalc	= true;
	}
	//
	//cout<<idetearliest_cal[0]<<" "<<aearliest[0]<<" "<<T[idetearliest_cal[0]]<<" "<<tearliest[0]<<" \t "
	//	<<idetearliest_cal[1]<<" "<<aearliest[1]<<" "<<T[idetearliest_cal[1]]<<" "<<tearliest[1]<<" \t "
	//	<<dt<<" "<<zvpd<<endl;
	//
	if (docalc){ return zvpd; } else { return -999; }
}

//-------------------------------------------------------------------------------------
//
float caleval::Get_calavg(){
	bool  foundside[2];
	for (int is=0;is<2;is++){
		foundside[is]	= false;
	}			
	int 	nlit[2]	= {0};
	float 	tavg[2]	= {0};
	for (int is=0;is<2;is++){
		for (int idet=0;idet<NDETSIDE;idet++){
			//
			if (lit[idet][is]){
				int idetew	= idet + is*19;
				float Tcal	= Calibrate(idetew,A[idetew],T[idetew],0);
				//
				//cout<<is<<" "<<idet<<" "<<idetew<<" "<<A[idetew]<<" "<<T[idetew]<<" "<<Tcal<<endl;
				foundside[is]	= true;
				nlit[is]++;
				tavg[is]+=Tcal;
			}
		}
	}
	//
	float dt;
	float zvpd;
	bool docalc	= false;
	if ( foundside[0]&&foundside[1] ){
		tavg[0]	/= nlit[0];
		tavg[1]	/= nlit[1];
		dt		= tavg[1]	- tavg[0];
		zvpd	= dt*tfactor;
		docalc	= true;
	}
	//
	//cout<<idetearliest_cal[0]<<" "<<aearliest[0]<<" "<<T[idetearliest_cal[0]]<<" "<<tearliest[0]<<" \t "
	//	<<idetearliest_cal[1]<<" "<<aearliest[1]<<" "<<T[idetearliest_cal[1]]<<" "<<tearliest[1]<<" \t "
	//	<<dt<<" "<<zvpd<<endl;
	//
	if (docalc){ return zvpd; } else { return -999; }
}

//-------------------------------------------------------------------------------------
//
float caleval::Get_cal11X(int isteer){
	float aearliest[2]	= {0};
	float tearliest[2]	= {0};
	bool  foundside[2];
	for (int is=0;is<2;is++){
		tearliest[is]	= 9999999*tsign;
		foundside[is]	= false;
	}			
	for (int is=0;is<2;is++){
		for (int idet=0;idet<NDETSIDE;idet++){
			//
			if (lit[idet][is]){
				int idetew	= idet + is*19;
				float Tcal	= Calibrate(idetew,A[idetew],T[idetew],isteer);
				//
				//cout<<is<<" "<<idet<<" "<<idetew<<" "<<A[idetew]<<" "<<T[idetew]<<" "<<Tcal<<endl;
				if (tsign*Tcal<tsign*tearliest[is]){
					tearliest[is]			= Tcal;
					aearliest[is]			= A[idetew];
					idetearliest_cal[is]	= idetew;
					foundside[is]			= true;						
				}
			}
		}
	}
	//
	float dt;
	float zvpd;
	bool docalc	= false;
	if ( foundside[0]&&foundside[1] ){	
		dt		= tearliest[1]	- tearliest[0];
		zvpd	= dt*tfactor;
		docalc	= true;
	}
	//
	//cout<<idetearliest_cal[0]<<" "<<aearliest[0]<<" "<<T[idetearliest_cal[0]]<<" "<<tearliest[0]<<" \t "
	//	<<idetearliest_cal[1]<<" "<<aearliest[1]<<" "<<T[idetearliest_cal[1]]<<" "<<tearliest[1]<<" \t "
	//	<<dt<<" "<<zvpd<<endl;
	//
	if (docalc){ return zvpd; } else { return -999; }
}

//-------------------------------------------------------------------------------------
//
float caleval::Get_cal11Xa(int isteer){
	int ideb	= 0;
	if (ideb)cout<<isteer<<" --------------------------------------"<<endl;
	float tcalE[19]	= {0};
	float tcalW[19]	= {0};
	int nlit[2]		= {0};
	for (int is=0;is<2;is++){
		for (int idet=0;idet<NDETSIDE;idet++){
			if (lit[idet][is]){
				int idetew	= idet + is*19;
				if (ideb) cout<<"lit "<<is<<" "<<idet<<" "<<T[idetew];
				if (is==0){ 
					++nlit[0]; 
					tcalW[idet]	= Calibrate(idetew,A[idetew],T[idetew],isteer); 
					if (ideb)cout<<" \t "<<tcalW[idet];
				} else if (is==1){
					++nlit[1]; 
					tcalE[idet]	= Calibrate(idetew,A[idetew],T[idetew],isteer); 
					if (ideb)cout<<" \t "<<tcalE[idet];
				} 
				if (ideb) cout<<endl;
			} else {
				if (is==0){ tcalW[idet]	= 9999999; } else
				if (is==1){ tcalE[idet]	= 9999999; } 
			}
		}
	}
	//for (int idet=0;idet<NDETSIDE;idet++){
	//	cout<<"E and W "<<idet<<" "<<tcalW[idet]<<" "<<tcalE[idet]<<endl;
	//}
    qsort(tcalW, 19 , sizeof(float), floatcomp);
    qsort(tcalE, 19 , sizeof(float), floatcomp);
    if (ideb){
		for (int idet=0;idet<NDETSIDE;idet++){
			if (tcalW[idet]<9999990||tcalE[idet]<9999990){
				cout<<"sorted  "<<idet<<" "<<tcalW[idet]<<" "<<tcalE[idet]<<" \t "
					<<nlit[0]<<" "<<nlit[1]
					<<endl;
			}
		}
	}
	//
	if (nlit[0]*nlit[1]==0){ return -999; }
	//
	int nuse[2]	= {0};
	for (int is=0;is<2;is++){
		if (nlit[is]<=2){ nuse[is]	= 1; } else 
		if (nlit[is]<=3){ nuse[is]	= 2; } else 
		if (nlit[is]<=6){ nuse[is]	= 3; } else 
		                { nuse[is]	= 4; }
	}
	float tearliest[2]	= {0};
	for (int is=0;is<2;is++){
		if (iChoice==0||iChoice==2){
			for (int iu=0;iu<nuse[is];iu++){
				if (ideb)cout<<"using... "<<is<<" ";
				if (is==0){ tearliest[is] += tcalW[iu]; if(ideb)cout<<tcalW[iu]; } else
				if (is==1){ tearliest[is] += tcalE[iu]; if(ideb)cout<<tcalE[iu]; } 
				if(ideb)cout<<endl;
			}
		} else if (iChoice==1||iChoice==3){
			for (int iu=nlit[is]-nuse[is];iu<nlit[is];iu++){
				if (ideb) cout<<"using... "<<is<<" ";
				if (is==0){ tearliest[is] += tcalW[iu]; if(ideb)cout<<tcalW[iu];} else
				if (is==1){ tearliest[is] += tcalE[iu]; if(ideb)cout<<tcalE[iu];} 
				if(ideb)cout<<endl;
			}
		}
		tearliest[is]	/= (float)nuse[is];
	}
	float dt	= tearliest[1]	- tearliest[0];
	float zvpd	= dt*tfactor;
	return zvpd;

}

//-------------------------------------------------------------------------------------
//
//	isteer = 0   .... full resn
//	isteer = 1   .... steps
//	isteer = 2   .... lines
//	isteer = 3   .... curveavg
//
void caleval::WriteCorr(const int isteer){	
	//
	char		buf[200];
	const char*	cardname[2]	= {"#VP001 East","#VP002 West"};		// east then west
	const char*	slotname[2]	= {"0x16","0x18"};						// east then west
	int 		channum[16]	= {4,5,6,7,12,13,14,15,20,21,22,23,28,29,30,31};
	int			numbin		= 8;
	//	
	cout<<"WriteCorr ... "<<runstring<<" choice="<<iChoice<<"  iTrgCrate = "<<iTrgCrate<<" TACtoNS="<<TACtoNS<<endl;
	if (iTrgCrate==0){ sprintf(buf,"qtcorr_bbq_%s.txt",runstring); } else 
	if (iTrgCrate==1){ sprintf(buf,"qtcorr_mxq_%s.txt",runstring); } 
	cout<<"WriteCorr ... file = "<<buf<<endl;
	//
	//---- get new results...
	double xp,yp;
	float	xmin[10][16][2]	= {0};
	float	xmax[10][16][2]	= {0};
	float	yval[10][16][2]	= {0};
	//
	//---- write out inverse map to check
	for (int is=0;is<2;is++){						// is=0 is West, is=1 is East
		for (int idet=0;idet<NDETSIDE;idet++){		// loop over TOF channel numbers
			cout<<"MAPCHECK....  is="<<is<<"  idet="<<idet<<"   itrg="<<mapTOFtoTRG(is,idet)<<endl;
		}
	}
	//
	//
	for (int is=0;is<2;is++){			// is=0 is West, is=1 is East
		//
		for (int kdet=0;kdet<16;kdet++){				// kdet is the TRG channel number
			//
			int idetew	= -1;
			for (int idet=0;idet<NDETSIDE;idet++){		// loop over TOF channel numbers
				if (kdet==mapTOFtoTRG(is,idet)){		// find TOF channel matching TRG channel
					idetew	= is*NDETSIDE + idet;
				}
			}
			if (idetew<0){
				cout<<"mapping error... is="<<is<<" kdet="<<kdet<<endl;
			}
			//
			bool DeadChannel	= false;
			for (int idead=0;idead<NDEAD;idead++){
				if (idetew==DEAD[idead]){
					DeadChannel	= true;
					cout<<"Channel idetew="<<idetew<<" is Dead!"<<endl;
				}
			}
			//---- get values and limits for dead channels....
			if (DeadChannel){
				for (int k=0;k<8;k++){
					xmax[k][kdet][is]	= 4095; 
					yval[k][kdet][is]	=    0; 
				}
				continue;
			}
			//
			//
			TGraph *fcorr_use;
			if (isteer==0){ fcorr_use	= (TGraph*)fcorr[idetew]->Clone("fcorr_use");  } else
			if (isteer==1){ fcorr_use	= (TGraph*)fcorrA[idetew]->Clone("fcorr_use"); } else
			if (isteer==2){ fcorr_use	= (TGraph*)fcorrB[idetew]->Clone("fcorr_use"); } else
			if (isteer==3){ fcorr_use	= (TGraph*)fcorrC[idetew]->Clone("fcorr_use"); } 
			//
			//---- get edges and values...
			int np	= fcorr_use->GetN();
			//cout<<is<<" "<<idetew<<" np="<<np<<endl;
			int	k	= 0;
			float xpprev=0,ypprev=0;
			for (int ip=0;ip<np;ip++){
				fcorr_use->GetPoint(ip,xp,yp);
				if (ip==0){
//cout<<"WriteCorr "<<is<<" "<<kdet<<" \t "<<ip<<" \t "<<k<<" "<<kdet<<" "<<is<<" "<<xp<<" "<<yp<<endl;
					xmin[k][kdet][is]	= xp;
					yval[k][kdet][is]	= yp;
					xpprev	= xp;
					ypprev	= yp;
					if(idetew==0)cout<<k<<" "<<kdet<<" "<<is<<" "<<xp<<" "<<yp<<endl;
				}
				if (ypprev!=yp){
					++k;
//cout<<"WriteCorr "<<is<<" "<<kdet<<" \t "<<ip<<" \t "<<k<<" "<<kdet<<" "<<is<<" "<<xp<<" "<<yp<<endl;
					xmin[k][kdet][is]	= xp;
					yval[k][kdet][is]	= yp;
					if(idetew==0)cout<<k<<" "<<kdet<<" "<<is<<" "<<xp<<" "<<yp<<endl;
				}
				xpprev	= xp;
				ypprev	= yp;
			}
			//
			//---- now convert adc lower limit values to upper limit values
			//---- and convert the y values in ns to TACbins
			//---- and flip the sign of the TAC corrections 
			//---- and make sure all values are in the range -255,255. 
			for (int k=0;k<8;k++){
				if (k< 7){ xmax[k][kdet][is]	= xmin[k+1][kdet][is]-1; } else 
				if (k==7){ xmax[k][kdet][is]	= 4095; }
				//
				float ysub			= int((yval[k][kdet][is]/TACtoNS)+0.5);
				if (ysub<-255) ysub	= -255;				
				if (ysub> 255) ysub	=  255;				
				yval[k][kdet][is]	= -ysub;
				//
			}
			//
			//---- convert values...
			//if (useit && is==0 && idet==0){
			//	for (int i=0;i<8;i++){
			//		cout<<i<<" "<<xmin[i]<<" "<<int((yval[i]/TACtoNS)+0.5)<<endl;
			//	}
			//}
			//
			delete fcorr_use;
			//
		}	//---- end idet loop
		//
	}	//---- end iside loop...
	//
	//---- write header text...
	ifstream textin;	textin.open("./example_slew_corr.txt", ifstream::in);
	ofstream textout;	textout.open(buf);
	string 		str;
	char*		cstr;
	for (int il=0;il<15;il++){
		getline(textin,str);
		cstr = &str[0];
		textout<<cstr<<endl;
	}
	textin.close();
	//
	//---- write data values...
	for (int is=0;is<2;is++){					// write east then west...
		//
		int ks	= 1-is;
		//
		textout<<endl;
		textout<<cardname[is]<<endl;
		//
		for (int itrg=0;itrg<16;itrg++){	
			//
			//---- write ADC LIMIT line
			textout<<slotname[is]<<"  "<<std::setw(3)<<channum[itrg]<<"\t"<<std::setw(0)<<numbin<<" 0\t";
			for (int i=0;i<8;i++){
				textout<<std::setw(6)<<xmax[i][itrg][ks]<<std::setw(0)<<" ";
			}	textout<<endl;
			//
			//---- write CORRECTION VALUE line
			textout<<slotname[is]<<"  "<<std::setw(3)<<channum[itrg]<<"\t"<<std::setw(0)<<numbin<<" 1\t";
			for (int i=0;i<8;i++){
				textout<<std::setw(6)<<int(yval[i][itrg][ks])<<std::setw(0)<<" ";
			}	textout<<endl;
			//
		}
	}
	//
	textout.close();
	//
}

int caleval::mapTRGtoTOF(const int is, const int itrg){
	int itof	= -1;
	if (itrg<0||itrg>15){
		cout<<"mapTRGtoTOF Error ... "<<is<<" "<<itrg<<endl;
		exit(0);
	}
	if (is==0){				// WEST
		itof	= map_vpd_trg2tof_west[itrg];			
	} else if (is==1){		// EAST
		itof	= map_vpd_trg2tof_east[itrg];			
	}
	return itof;
}
int caleval::mapTOFtoTRG(const int is, const int itof){
	if (invertmap){
		cout<<"Inverting TRGtoTOF map -> TOFtoTRG..."<<endl;
		invertmap	= false;
		for (int i=0;i<19;i++){
			map_vpd_tof2trg_west[i]	= -1;
			map_vpd_tof2trg_east[i]	= -1;
		}
		for (int is=0;is<2;is++){
			for (int itrg=0;itrg<16;itrg++){
				int i;
				if (is==0){
					i	= map_vpd_trg2tof_west[itrg];
					map_vpd_tof2trg_west[i]	= itrg;
				} else if (is==1){
					i	= map_vpd_trg2tof_east[itrg];
					map_vpd_tof2trg_east[i]	= itrg;
				}
				//cout<<is<<" "<<i<<" "<<itrg<<endl;
			}
		}
		cout<<"Done inverting TRGtoTOF map -> TOFtoTRG..."<<endl;
	}
	//
	int itrg	= -1;
	if (itof<0||itof>18){
		cout<<"mapTOFtoTRG Error ... "<<is<<" "<<itof<<endl;
		exit(0);
	}
	if (is==0){				// WEST
		itrg	= map_vpd_tof2trg_west[itof];			
	} else if (is==1){		// EAST
		itrg	= map_vpd_tof2trg_east[itof];			
	}
	return itrg;
	//
}