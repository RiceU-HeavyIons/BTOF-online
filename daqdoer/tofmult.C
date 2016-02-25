#include <iostream>
#include <iomanip>
#include "TError.h"
#include "TMath.h"
using namespace std;

Int_t KeepTray(int itray);

void tofmult(int krun) {

	gErrorIgnoreLevel	= 2000;
	char 		buf[100];
	char 		tbuf[100];
	int			ican=-1;
	int			ipad;
	TCanvas*	ccan[100];
	char 		type[20];

	sprintf(buf,"%d",krun);
	sprintf(type,"%s_tof",buf);
	cout<<buf<<" "<<type<<endl;

// 	sprintf(type,"13039132_tof");
// 	sprintf(type,"13039_tof");
// 	sprintf(type,"13039_tof_tofmult4");
// 	sprintf(type,"13042023_tof");
 	
// 	sprintf(type,"12130061_tof");
// 	sprintf(type,"12130062_tof");
// 	sprintf(type,"12130063_tof");

//	sprintf(type,"12127004_tof");
//	sprintf(type,"12129010_tof");
// 	sprintf(type,"12129011_tof");
// 	sprintf(type,"12129012_tof");
// 	sprintf(type,"12129013_tof");

	//sprintf(type,"12127004_tof");
	//sprintf(type,"12128039_tof");
	//sprintf(type,"12129003_tof");
	
	char psfileopen[100];
	char psfile[100];
	char pdffile[100];
	char psfileclose[100];
	char rootfile[100];
	sprintf(psfileopen,"tofmult_%s.ps(",type);
	sprintf(psfile,"tofmult_%s.ps",type);
	sprintf(pdffile,"tofmult_%s.pdf",type);
	sprintf(psfileclose,"tofmult_%s.ps]",type);
	sprintf(rootfile,"tofmult_%s.root",type);
	cout<<type<<endl;
	cout<<psfileopen<<endl;
	cout<<psfile<<endl;
	cout<<psfileclose<<endl;
	cout<<pdffile<<endl;
		
	TLatex *thistext = new TLatex();
		thistext->SetTextSize(0.1);
		thistext->SetNDC();
	
	TH1::AddDirectory(kFALSE);
	
	sprintf(buf,"/star/institutions/rice/llope/dd/daqdoer_%s.root",type);
	TFile *f = new TFile(buf,"READ");
	cout<<buf<<endl;

	f->cd();
	TH1D *htrgdtime[4];
	TH1D *htrgdtimekeep[4];
	for (int i=0;i<4;i++){
		sprintf(buf,"htrgdtime%d",i);
		htrgdtime[i]	= (TH1D*)f->Get(buf);
		sprintf(buf,"htrgdtimekeep%d",i);
		htrgdtimekeep[i]	= (TH1D*)f->Get(buf);
	}
	TH2D *hmultcorr_tray[120];
	TH1D *hmultcorr_tray_diff[120];
	TProfile *hmultcorr_tray_pfx[120];
	double tavg[120];
	double trms[120];
	double tskew[120];
	TH1D *hmultcorrtray_avg  	= new TH1D("hmultcorrtray_avg" ,"hmultcorrtray_avg" ,120,-0.5,119.5);
	TH1D *hmultcorrtray_rms  	= new TH1D("hmultcorrtray_rms" ,"hmultcorrtray_rms" ,120,-0.5,119.5);
	TH1D *hmultcorrtray_skew 	= new TH1D("hmultcorrtray_skew","hmultcorrtray_skew",120,-0.5,119.5);
	TH1D *hncorrtray     		= new TH1D("hncorrtray","hncorrtray",120,-0.5,119.5);
	TH1D *hncorrtraydiag 		= new TH1D("hncorrtraydiag","hncorrtraydiag",120,-0.5,119.5);
	TH1D *hdiagfrac 	 		= new TH1D("hdiagfrac","hdiagfrac",120,-0.5,119.5);
	TH1D *hmultcorrtray_avg_1D  = new TH1D("hmultcorrtray_avg_1D" ,"hmultcorrtray_avg_1D" ,100,-1.0,1.0);
	TH1D *hmultcorrtray_rms_1D  = new TH1D("hmultcorrtray_rms_1D" ,"hmultcorrtray_rms_1D" ,200, 0.0,2.0);
	TH1D *hmultcorrtray_skew_1D = new TH1D("hmultcorrtray_skew_1D","hmultcorrtray_skew_1D",200,-5.0,5.0);
	TH1D *hdiagfrac_1D 			= new TH1D("hdiagfrac_1D"     ,"hdiagfrac_1D"     ,100, 0.5,1.0);
	//
	TH1D *htraytotmult_tof			= (TH1D*)f->Get("htraytotmult_tof");
	TH1D *htraytotmult_trg			= (TH1D*)f->Get("htraytotmult_trg");
	TH2D *htraytotmult_trgvstof		= (TH2D*)f->Get("htraytotmult_trgvstof");
	TH1D *htraytotmult_tofminustrg	= (TH1D*)f->Get("htraytotmult_tofminustrg");
	//
	for (int i=0;i<120;i++){
		//
		sprintf(buf,"hninocorr_tray%d",i);
		hmultcorr_tray[i]		= (TH2D*)f->Get(buf);
		sprintf(buf,"hmultcorr_tray%d",i);
		hmultcorr_tray[i]->SetName(buf);
		hmultcorr_tray[i]->SetTitle("");
		//
		sprintf(buf,"hmultcorr_tray_pfx%d",i);
		hmultcorr_tray_pfx[i]		= (TProfile*)hmultcorr_tray[i]->ProfileX(buf);
		hmultcorr_tray_pfx[i]->SetMarkerStyle(24);
		hmultcorr_tray_pfx[i]->SetMarkerSize(0.6);
		hmultcorr_tray_pfx[i]->SetMarkerColor(5);
		//
		sprintf(buf,"hninocorrtight_tray_diffnoax%d",i);
		//sprintf(buf,"hninocorr_tray_diffnoax%d",i);
		//sprintf(buf,"hninocorr_tray_diff%d",i);
		//sprintf(buf,"hmultcorrtight_tray_diff%d",i);
		//sprintf(buf,"hmultcorr_tray_diff%d",i);
		hmultcorr_tray_diff[i]	= (TH1D*)f->Get(buf);
		sprintf(buf,"hmultcorr_tray_diff%d",i);
		hmultcorr_tray_diff[i]->SetName(buf);
		hmultcorr_tray_diff[i]->SetTitle("");
		//
		if (KeepTray(i+1)){
			tavg[i]		= hmultcorr_tray_diff[i]->GetMean();
			trms[i]		= hmultcorr_tray_diff[i]->GetRMS();
			tskew[i]	= hmultcorr_tray_diff[i]->GetSkewness(1);
			hmultcorrtray_avg->SetBinContent(i+1,tavg[i]);
			hmultcorrtray_rms->SetBinContent(i+1,trms[i]);
			hmultcorrtray_avg_1D->Fill(tavg[i]);
			hmultcorrtray_rms_1D->Fill(trms[i]);
			if (!TMath::IsNaN(tskew[i])){
				hmultcorrtray_skew->SetBinContent(i+1,tskew[i]);
				hmultcorrtray_skew_1D->Fill(tskew[i]);
			}
			cout<<i<<" "<<tavg[i]<<" "<<trms[i]<<" "<<tskew[i]<<endl;
			int nbx	= hmultcorr_tray[i]->GetNbinsX();
			int nby	= hmultcorr_tray[i]->GetNbinsY();
			for (int ix=1;ix<=nbx;ix++){
			for (int iy=1;iy<=nby;iy++){
				if ((ix-1)>0&&(iy-1)>0){
				//} else {
					int nc	= hmultcorr_tray[i]->GetBinContent(ix,iy);
					hncorrtray->Fill(i,nc);
					if (ix==iy){
						hncorrtraydiag->Fill(i,nc);
						hdiagfrac->Fill(i,nc);
					}
				}
			}	}	// end x,y bin loops....
		}
		//
	}
	
	hdiagfrac->Divide(hncorrtray);
	int nbx	= hdiagfrac->GetNbinsX();
	for (int ix=1;ix<=nbx;ix++){
		if (KeepTray(ix)){
			double val	= hdiagfrac->GetBinContent(ix);
			hdiagfrac_1D->Fill(val);
		}
	}
	
	//-------------------------------------------------------
	gStyle->SetPalette(1);
	gStyle->SetOptStat(0);
	gStyle->SetPadGridX(kFALSE);
	gStyle->SetPadGridY(kFALSE);
	gStyle->SetPadTopMargin(0.03);
	gStyle->SetPadRightMargin(0.03);
	
	float nctotal,ncintime;
	
 	++ican; sprintf(buf,"ccan%d",ican);
 	ccan[ican] = new TCanvas(buf,buf,0,0,750.,750.*(8.5/11.));
 	ccan[ican]->SetFillColor(10);
 	ccan[ican]->cd(); ccan[ican]->Divide(2,2,0.0001,0.0001);
	for (int i=0;i<4;i++){
		ccan[ican]->cd(1+i);
		gPad->SetLogy(1);
		htrgdtime[i]->GetXaxis()->SetRangeUser(2400.,3300.);
		htrgdtime[i]->Draw();
		htrgdtimekeep[i]->Draw("same");
		int nbx	= htrgdtime[i]->GetNbinsX();
		nctotal	= ncintime = 0.;
		for (int ib=1;ib<=nbx;ib++){
			float xval		= htrgdtime[i]->GetBinCenter(ib);
			if (xval>=2400.&&xval<3300.){
				nctotal		+= 	htrgdtime[i]->GetBinContent(ib);
				ncintime	+= 	htrgdtimekeep[i]->GetBinContent(ib);
			}
		}
		cout<<i<<" "<<nctotal<<" "<<ncintime<<" "<<ncintime/nctotal<<endl;
	}
	ccan[ican]->cd(); ccan[ican]->Update();
	ccan[ican]->Print(psfileopen);

	gStyle->SetOptStat(111111011);
 	++ican; sprintf(buf,"ccan%d",ican);
 	ccan[ican] = new TCanvas(buf,buf,0,0,750.,750.*(8.5/11.));
 	ccan[ican]->SetFillColor(10);
 	ccan[ican]->cd(); ccan[ican]->Divide(2,2,0.0001,0.0001);
	ccan[ican]->cd(1);
		gPad->SetLogy(1);
		htraytotmult_tof->Draw();
	ccan[ican]->cd(2);
		gPad->SetLogy(1);
		htraytotmult_trg->Draw();
	ccan[ican]->cd(3);
		gPad->SetLogz(1);
		htraytotmult_trgvstof->SetStats(0);
		htraytotmult_trgvstof->Draw("col");
	ccan[ican]->cd(4);
		htraytotmult_tofminustrg->Draw();
	ccan[ican]->cd(); ccan[ican]->Update();
	ccan[ican]->Print(psfile);
	gStyle->SetOptStat(0);

//  	++ican; sprintf(buf,"ccan%d",ican);
//  	ccan[ican] = new TCanvas(buf,buf,0,0,750.,750.*(8.5/11.));
//  	ccan[ican]->SetFillColor(10);
//  	ccan[ican]->cd(); ccan[ican]->Divide(1,3,0.0001,0.0001);
// 	ccan[ican]->cd(1);
// 		hmultcorrtray_avg->Draw("HIST");
// 	ccan[ican]->cd(2);
// 		hmultcorrtray_rms->Draw("HIST");
// 	ccan[ican]->cd(3);
// 		hmultcorrtray_skew->Draw("HIST");
// 	ccan[ican]->cd(); ccan[ican]->Update();
// 	ccan[ican]->Print(psfile);
// 
//  	++ican; sprintf(buf,"ccan%d",ican);
//  	ccan[ican] = new TCanvas(buf,buf,0,0,750.,750.*(8.5/11.));
//  	ccan[ican]->SetFillColor(10);
//  	ccan[ican]->cd(); ccan[ican]->Divide(1,2,0.0001,0.0001);
// 	ccan[ican]->cd(1);
// 		hncorrtray->SetMinimum(0);
// 		hncorrtray->Draw();
// 		hncorrtraydiag->SetLineColor(3);
// 		hncorrtraydiag->Draw("same");
// 	ccan[ican]->cd(2);
// 		hdiagfrac->Draw();
// 	ccan[ican]->cd(); ccan[ican]->Update();
// 	ccan[ican]->Print(psfile);

	gStyle->SetOptStat(1111);
	gStyle->SetOptFit(1111);
 	++ican; sprintf(buf,"ccan%d",ican);
 	ccan[ican] = new TCanvas(buf,buf,0,0,750.,750.*(8.5/11.));
 	ccan[ican]->SetFillColor(10);
 	ccan[ican]->cd(); ccan[ican]->Divide(2,2,0.0001,0.0001);
	ccan[ican]->cd(1);
		hmultcorrtray_avg_1D->Draw();
	ccan[ican]->cd(2);
		hmultcorrtray_rms_1D->Draw();
	ccan[ican]->cd(3);
		hmultcorrtray_skew_1D->Draw();
	ccan[ican]->cd(4);
		hdiagfrac_1D->Draw();
		//hdiagfrac_1D->Fit("gaus");
	ccan[ican]->cd(); ccan[ican]->Update();
	ccan[ican]->Print(psfile);
	gStyle->SetOptStat(0);
	gStyle->SetOptFit(0);

	for (int i=0;i<120;i++){
		if ((i%30)==0){
			//cout<<"new canvas "<<i<<" "<<i%30<<" "<<ican<<endl;
		 	++ican; sprintf(buf,"ccan%d",ican);
		 	ccan[ican] = new TCanvas(buf,buf,0,0,750.,750.*(8.5/11.));
			ccan[ican]->SetFillColor(10);
 			ccan[ican]->cd(); ccan[ican]->Divide(6,5,0.0001,0.0001);
 		}
 		ipad = i%30;
 		//cout<<"ipad = "<<ipad<<endl;
		ccan[ican]->cd(1+ipad);
		hmultcorr_tray[i]->GetXaxis()->SetRangeUser(-0.5,24.49);
		hmultcorr_tray[i]->GetYaxis()->SetRangeUser(-0.5,24.49);
		hmultcorr_tray[i]->Draw("col");
		hmultcorr_tray_pfx[i]->Draw("same");
		sprintf(buf,"Tray=%d",i+1);
		thistext->DrawLatex(0.16,0.85,buf);
		if (((i+1)%30)==0){
			//cout<<"end canvas "<<i<<" "<<(i+1)%30<<endl;
			ccan[ican]->cd(); ccan[ican]->Update();
			ccan[ican]->Print(psfile);
		}
	}

// 	for (int i=0;i<120;i++){
// 		if ((i%30)==0){
// 			//cout<<"new canvas "<<i<<" "<<i%30<<" "<<ican<<endl;
// 		 	++ican; sprintf(buf,"ccan%d",ican);
// 		 	ccan[ican] = new TCanvas(buf,buf,0,0,750.,750.*(8.5/11.));
// 			ccan[ican]->SetFillColor(10);
//  			ccan[ican]->cd(); ccan[ican]->Divide(6,5,0.0001,0.0001);
//  		}
//  		ipad = i%30;
//  		//cout<<"ipad = "<<ipad<<endl;
// 		ccan[ican]->cd(1+ipad);
// 		hmultcorr_tray_diff[i]->GetXaxis()->SetRangeUser(-24.5,24.49);
// 		hmultcorr_tray_diff[i]->Draw();
// 		sprintf(buf,"Tray=%d",i+1);
// 		thistext->DrawLatex(0.16,0.85,buf);
// 		if (((i+1)%30)==0){
// 			//cout<<"end canvas "<<i<<" "<<(i+1)%30<<endl;
// 			ccan[ican]->cd(); ccan[ican]->Update();
// 			ccan[ican]->Print(psfile);
// 		}
// 	}

	
	ccan[ican]->Print(psfileclose);
	TFile *fout = new TFile(rootfile,"RECREATE");
		hmultcorrtray_avg->Write();
		hmultcorrtray_rms->Write();
		hmultcorrtray_skew->Write();
		hncorrtray->Write();
		hncorrtraydiag->Write();
		hdiagfrac->Write();
		for (int i=0;i<120;i++){
			hmultcorr_tray_pfx[i]->Write();
		}
		hmultcorrtray_avg_1D->Write();
		hmultcorrtray_rms_1D->Write();
		hmultcorrtray_skew_1D->Write();
		hdiagfrac_1D->Write();
		htraytotmult_tof->Write();
		htraytotmult_trg->Write();
		htraytotmult_trgvstof->Write();
		htraytotmult_tofminustrg->Write();
	fout->Close();
	sprintf(buf,"ps2pdf %s %s",psfile,pdffile);
	gSystem->Exec(buf);
	sprintf(buf,"cp -v %s /star/u/llope/WWW/files/",pdffile);
	gSystem->Exec(buf);

}

Int_t KeepTray(int it){
		if (it== 95){ return 0; }
		if (it==102){ return 0; }
		//if (it<61){ return 1; }
		//if (it>72){ return 1; }
		//return 0;
		return 1;
}