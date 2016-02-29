#include "noise_class.h"

float fxmeansuse[12][5][30] = {0};
float fxrangeuse[12][5][30] = {0};

float TRIGGERWINDOWWIDTH_MTD = 25.0E-06;
//float TRIGGERWINDOWWIDTH_TOF = 1.650E-06;
//float TRIGGERWINDOWWIDTH_TOF = 5.0E-06;
float TRIGGERWINDOWWIDTH_TOF = 2.0E-06; // as of run 17056042 changed TOF readout window from 5us to 2us

Bool_t reject;
int itrayinloop;
int imodule;
Double_t fline(Double_t *x, Double_t *par) {
	itrayinloop		= x[0];
	imodule			= itrayinloop%32;
	if (reject) {
		if (imodule<8 || imodule>18){
			TF1::RejectPoint();
			return 0;
		}
	}
	return par[0] + par[1]*x[0];
}
Double_t sinx(Double_t *x, Double_t *par) {
	return par[0]*TMath::Sin(x[0]/57.29578);
}

Float_t TrayToPhi(int itray){
	float phi;
	float dphi	= (6./180.)*TMath::ACos(-1.);
	if (itray<=60){
		if (itray>=1&&itray<=13){		phi = (13-itray)*dphi;        } else 
								{		phi = (13+(60-itray))*dphi;   }
	} else if (itray>60){
		if (itray>=103&&itray<=120){	phi	= (itray-103)*dphi;       } else 
								   {	phi	= (17 + (itray-60))*dphi; }
	}
	return phi;
}

void noise::Loop() {


	char	buf[120];
	char	tbuf[120];
	TCanvas *ccan[100];
	int		ican		= -1;
	int		nhtray[300];
	int		nhtray_totL[300];
	int		nhtray_totH[300];
	int		thisyear,thisday,thisrunonday;
	int		nhitevt_tot			=  0;
	int		nhitevt_totL		=  0;
	int		nhitevt_totH		=  0;
	int		nhittotal			=  0;
	int		nhitLEtotal			=  0;
	int		nhitTEtotal			=  0;
	int		nhittotal_tot		=  0;
	int		nhittotal_totL		=  0;
	int		nhittotal_totH		=  0;
	int		nhitevt_tot_vpd		=  0;
	int		nhitevt_totL_vpd	=  0;
	int		nhitevt_totH_vpd	=  0;
	int		nhittotal_vpd		=  0;
	int		nhitLEtotal_vpd		=  0;
	int		nhitTEtotal_vpd		=  0;
	int		nhittotal_tot_vpd	=  0;
	int		nhittotal_totL_vpd	=  0;
	int		nhittotal_totH_vpd	=  0;
	int		nhitevt_tot_mtdBL[30]	=  {0};
	int		nhitevt_totL_mtdBL[30]	=  {0};
	int		nhitevt_totH_mtdBL[30]	=  {0};
 	int		nhittotal_mtd		=  0;
// 	int		nhitLEtotal_mtd		=  0;
// 	int		nhitTEtotal_mtd		=  0;
	int		nhittotal_tot_mtdBL[30]		=  {0};
	int		nhittotal_totL_mtdBL[30]	=  {0};
	int		nhittotal_totH_mtdBL[30]	=  {0};

	//---- index is loop ID ((tray+1)%5), value is tray ID
	Int_t LoopID;
	Int_t TrayIDinLoop;
	Int_t TrayID_LoopIn[24]  = { 5,10,15,16,21,26,31,36, 45, 50, 55, 60,
	                            61,66,71,76,81,86,91,96,101,106,111,116};
	Int_t TrayID_LoopOut[24] = { 1, 6,11,20,25,30,35,40, 41, 46, 51, 56,
	                            65,70,75,80,85,90,95,100,105,110,115,120};
	Int_t ModuleIDinLoop;
	Int_t GlobalModule;
	Int_t GlModToIDinLoop[3840];
	Float_t	RadToDeg	= 180./TMath::ACos(-1.);

						 // 0, 1, 2, 3, 4, 5, 6, 7
	int ModToEtaPosn5[8] = {1, 2, 3, 9, 5, 4, 9, 9};	// index is tdig [0,1,2,4,5], value is eta position [1,5] 
	int ModToEtaPosn3[8] = {2, 3, 9, 9, 4, 9, 9, 9};	// index is tdig [0,1,2,4,5], value is eta position [1,5] 
	//
	int ActiveRun13[30] = {8,9,10,11,12,13,14,0,0,15,0,0,0,0,0,0,0,0,0,0,0,1,0,0,2,3,4,5,6,7};
	int ActiveLabels13[15]= {22,25,26,27,28,29,30,1,2,3,4,5,6,7,10};
	//
	// 9,23 are impossible
	int ActiveRun14[30] = {1,2,3,4,5,6,7,8,0,10,11,12,13,14,15,16,17,18,19,20,21,22,0,24,25,26,27,28,29,30};

	float	off_tray[120];
	float	off_glmod[3840];
	TFile *fsl	= new TFile("./sl.root","READ");
		fsl->cd();
		TH1D *hdtstart_tray		= (TH1D*)gDirectory->Get("hdtstart_tray");
		for (int ib=1;ib<=120;ib++){
			off_tray[ib-1]		= hdtstart_tray->GetBinContent(ib);
		}
		TH1D *hdtstart2pk_mod	= (TH1D*)gDirectory->Get("hdtstart2pk_mod");
		for (int ib=1;ib<=3840;ib++){
			off_glmod[ib-1]		= hdtstart2pk_mod->GetBinContent(ib);
		}
	fsl->Close();

	sprintf(buf,"./fitzhit_14042.root");
	cout<<"..... noise::loop  Opening "<<buf<<endl;
	TFile *fin = new TFile(buf,"read");
		TH3D *xmeansuse	= (TH3D*)fin->Get("xmeansuse");
		TH3D *xrangeuse	= (TH3D*)fin->Get("xrangeuse");
	for (int ib=0;ib<30;ib++){
		for (int ip=0;ip<5;ip++){
			for (int is=0;is<12;is++){
				fxmeansuse[is][ip][ib]	= xmeansuse->GetBinContent(ib+1,ip+1,is+1);
				fxrangeuse[is][ip][ib]	= xrangeuse->GetBinContent(ib+1,ip+1,is+1);
			}
		}
	}	
	
	sprintf(buf,"/star/institutions/rice/geurts/noise/noise_%d",kRunUse);
	TString namebase	= TString(buf);
	TString nameextr	= TString(".root");
	TString nameroot	= namebase+nameextr;
	TString nameextp	= TString(".ps");
	TString nameposc	= namebase+nameextp;
	cout<<"..... noise::loop  Opening "<<nameroot.Data()<<endl;
	TFile *fout = new TFile(nameroot.Data(),"RECREATE");
	//
	TH1D *hinfo				= new TH1D("hinfo","information",100,-0.5,99.5);
	//
	TH1D *hrate_tray		= new TH1D("hrate_tray"    ,"rate/cell by tray ID",120,-0.5,119.5);
	TH1D *hrate_glmodule	= new TH1D("hrate_glmodule","rate/cell by global module ID",3840,-0.5,3839.5);
	TH1D *hrate_trmodule	= new TH1D("hrate_trmodule","rate/cell by tray module ID",32,-0.5,31.5);
	TH1D *hrate_trcell		= new TH1D("hrate_trcell"  ,"rate/cell by tray cell ID",6,-0.5,5.5);

	TH1D *htray_npevtray25 = new TH1D("htray_npevtray25","rate/cell by tray ID, nHits/tray/ev>25",120,-0.5,119.5);
	TH1D *htray_npevtray50 = new TH1D("htray_npevtray50","rate/cell by tray ID, nHits/tray/ev>50",120,-0.5,119.5);
	TH1D *htray_npevtray100 = new TH1D("htray_npevtray100","rate/cell by tray ID, nHits/tray/ev>100",120,-0.5,119.5);
	TH1D *htray_npevtray190 = new TH1D("htray_npevtray190","rate/cell by tray ID, nHits/tray/ev>190",120,-0.5,119.5);

	TH1D *hnpev 		= new TH1D("hnpev"    ,"nHits/ev, ToT range"     ,250,0.,1000.);
	TH1D *hnpevtray		= new TH1D("hnpevtray","nHits/tray/ev, ToT range",193,0., 193.);
	TH1D *hnpev_totL 	= new TH1D("hnpev_totL"    ,"nHits/ev, low ToT"     ,250,0.,1000.);
	TH1D *hnpevtray_totL= new TH1D("hnpevtray_totL","nHits/tray/ev, low ToT",193,0., 193.);
	TH1D *hnpev_totH 	= new TH1D("hnpev_totH"    ,"nHits/ev, high ToT"     ,250,0.,1000.);
	TH1D *hnpevtray_totH= new TH1D("hnpevtray_totH","nHits/tray/ev, high ToT",193,0.,193.);
	
	TH1D *htrel			= new TH1D("htrel","t-t_{ear}, nHits/tray/ev>25",500,0.,500.);
	TH1D *htrelzoom		= new TH1D("htrelzoom","t-t_{ear}, nHits/tray/ev>25",200,0.1,50.1);
	TH2D *htrel_module		= new TH2D("htrel_module","t-t_{ear} vs module, nHits/tray/ev>25",32,-0.5,31.5,250,0.,500.);
	TH2D *htrelzoom_module	= new TH2D("htrelzoom_module","t-t_{ear} vs module, nHits/tray/ev>25",32,-0.5,31.5,100,0.1,50.1);
	TH2D *htrel_relmodule		= new TH2D("htrel_relmodule","t-t_{ear} vs rel module, nHits/tray/ev>25",63,-31.5,31.5,250,0.,500.);
	TH2D *htrelzoom_relmodule	= new TH2D("htrelzoom_relmodule","t-t_{ear} vs rel module, nHits/tray/ev>25",63,-31.5,31.5,100,0.1,50.1);

	TH2D *htrelzoom_relmod_imear[32];
	for (int i=0;i<32;i++){
		sprintf(buf,"htrelzoom_relmod_imear%d",i);
		htrelzoom_relmod_imear[i] = new TH2D(buf,"t-t_{ear} vs rel module, nHits/tray/ev>25, by iMear",63,-31.5,31.5,100,0.1,50.1);
	}

	TH1D *htotwide	= new TH1D("htotwide","ToT",400,0.0,1000.0);
	TH1D *htot		= new TH1D("htot","ToT TOF",160,0.0,40.0);
	TH2D *htot_nh	= new TH2D("htot_nh","ToT vs nHits/tray/ev",80,0.0,350.0,80,0.0,40.0);
	TH1D *htot_vpd	= new TH1D("htot_vpd","ToT VPD",160,0.0,40.0);
	TH1D *htot_mtd[30];
	for (int i=0;i<30;i++){
		sprintf(buf,"htot_mtd%d",i);
		sprintf(tbuf,"ToT BL%d",i+1);
		htot_mtd[i]	= new TH1D(buf,tbuf,160,0.0,60.0);
	}
	TH1D *htot_nhlow= new TH1D("htot_nh_low","ToT, nHits/tray/ev<10",80,0.0,40.0);
	TH1D *htot_nhhig= new TH1D("htot_nh_hig","ToT, nHits/tray/ev>10",80,0.0,40.0);
	TH1D *hrate_tray_totL = new TH1D("hrate_tray_totL","rate/cell by tray ID, low ToT",120,-0.5,119.5);
	TH1D *hrate_tray_totH = new TH1D("hrate_tray_totH","rate/cell by tray ID, high ToT",120,-0.5,119.5);
	TH1D *hrate_glmodule_totL	= new TH1D("hrate_glmodule_totL","rate/cell by global module ID, low ToT",3840,-0.5,3839.5);
	TH1D *hrate_glmodule_totH	= new TH1D("hrate_glmodule_totH","rate/cell by global module ID, high ToT",3840,-0.5,3839.5);
	TH1D *hrate_trmodule_totL	= new TH1D("hrate_trmodule_totL","rate/cell by tray module ID, low ToT",32,-0.5,31.5);
	TH1D *hrate_trmodule_totH	= new TH1D("hrate_trmodule_totH","rate/cell by tray module ID, high ToT",32,-0.5,31.5);
	TH1D *hrate_trcell_totL		= new TH1D("hrate_trcell_totL"  ,"rate/cell by tray cell ID, low ToT",6,-0.5,5.5);
	TH1D *hrate_trcell_totH		= new TH1D("hrate_trcell_totH"  ,"rate/cell by tray cell ID, high ToT",6,-0.5,5.5);

	TH1D *hrate_trmodule_loopid[5];
	TH1D *hrate_trmodule_loopid_totH[5];
	TH1D *hrate_trmodule_loopid_totL[5];
	for (int id=0;id<5;id++){
		sprintf(buf,"hrate_trmodule_loopid%d",id);
		sprintf(tbuf,"rate/cell by tray module ID, TrayIDinLoop=%d",id);
		hrate_trmodule_loopid[id] = new TH1D(buf,tbuf,32,-0.5,31.5);
		sprintf(buf,"hrate_trmodule_loopid_totH%d",id);
		sprintf(tbuf,"rate/cell by tray module ID, High-ToT, TrayIDinLoop=%d",id);
		hrate_trmodule_loopid_totH[id] = new TH1D(buf,tbuf,32,-0.5,31.5);
		sprintf(buf,"hrate_trmodule_loopid_totL%d",id);
		sprintf(tbuf,"rate/cell by tray module ID, Low-ToT, TrayIDinLoop=%d",id);
		hrate_trmodule_loopid_totL[id] = new TH1D(buf,tbuf,32,-0.5,31.5);
	}
	TH1D *hrate_loopmodule = new TH1D("hrate_loopmodule","rate/cell by loop module ID",160,-0.5,159.5);
	TH1D *hrate_loopmodule_totH = new TH1D("hrate_loopmodule_totH","rate/cell by loop module ID, High-ToT",160,-0.5,159.5);
	TH1D *hrate_loopmodule_totL = new TH1D("hrate_loopmodule_totL","rate/cell by loop module ID, Low-ToT",160,-0.5,159.5);

	TH2D *hrate_trmodcell_loopid[5];
	TH2D *hrate_trmodcell_loopid_totH[5];
	TH2D *hrate_trmodcell_loopid_totL[5];
	for (int id=0;id<5;id++){
		sprintf(buf,"hrate_trmodcell_loopid%d",id);
		sprintf(tbuf,"rate/cell by tray module ID, TrayIDinLoop=%d",id);
		hrate_trmodcell_loopid[id] = new TH2D(buf,tbuf,6,-0.5,5.5,32,-0.5,31.5);
		sprintf(buf,"hrate_trmodcell_loopid_totH%d",id);
		sprintf(tbuf,"rate/cell by tray module ID, High-ToT, TrayIDinLoop=%d",id);
		hrate_trmodcell_loopid_totH[id] = new TH2D(buf,tbuf,6,-0.5,5.5,32,-0.5,31.5);
		sprintf(buf,"hrate_trmodcell_loopid_totL%d",id);
		sprintf(tbuf,"rate/cell by tray module ID, Low-ToT, TrayIDinLoop=%d",id);
		hrate_trmodcell_loopid_totL[id] = new TH2D(buf,tbuf,6,-0.5,5.5,32,-0.5,31.5);
	}

	TH2D *hvpdmap	= new TH2D("hvpdmap" ,"vpd map",24,-0.5,23.5,24,-0.5,23.5);
	TH1D *hvpdcell	= new TH1D("hvpdcell","vpd cell",40,-1.5,38.5);
	TH2D *htotx_vpd	= new TH2D("htotx_vpd","htotx_vpd",50,-1.0,1.0,40,5.0,45.0);
	TH1D *hp_vpd_tof	= (TH1D*)finput->Get("hp_vpd_tof");
	if (hp_vpd_tof){ cout<<"..... noise::loop  Found VPD TOF Hit pattern..... "<<endl; }

	//TH1D *hmtdcell		= new TH1D("hmtdcell","mtd cell",192,-0.5,191.5);
	//TH1D *hmtdcell_totL	= new TH1D("hmtdcell_totL","mtd cell_totL",192,-0.5,191.5);
	//TH1D *hmtdcell_totH	= new TH1D("hmtdcell_totH","mtd cell_totH",192,-0.5,191.5);
	//TH2D *hmtdmap	= new TH2D("hmtdmap" ,"mtd map",24,-0.5,23.5,24,-0.5,23.5);
	//TH2D *htotx_mtd	= new TH2D("htotx_mtd","htotx_mtd",50,-1.0,1.0,40,5.0,45.0);
	TH2D *hcellmod	= new TH2D("hcellmod","hcellmod",4,-0.5,3.5,192,-0.5,191.5);

	TH1D *hmtdcell_bl[30];
	for (int i=0;i<30;i++){
		sprintf(buf,"hmtdcell_bl%d",i);
		sprintf(tbuf,"Rate BL%d",i+1);
		hmtdcell_bl[i] = new TH1D(buf,tbuf,192,-0.5,191.5);
	}
	sprintf(tbuf,"Rate (Hz) vs (BL,strip-posn), Run=%d",kRunUse);
	TH2D *hmtdstripsbl	= new TH2D("hmtdstripsbl",tbuf,30,-0.5,29.5,120,-0.5,119.5);
	//
	TH1D *hmtdcell[5][30];
	TH1D *hmtdstriprat[5][30];
	TH2D *hmtdhitz_strip[5][30];
	TH2D *hmtdhitz_strip_zoff[5][30];
	TH2D *hmtdstripcorr[5][30];
	TH1D *hmtdnsignalsevent[5][30];
	for (int ibl=0;ibl<30;ibl++){
		for (int ie=0;ie<5;ie++){
			//
			sprintf(buf,"hmtdcell%d%d",ie,ibl);
			hmtdcell[ie][ibl]=new TH1D(buf,buf,24,-0.5,23.5);
			sprintf(buf,"hmtdstriprat%d%d",ie,ibl);
			hmtdstriprat[ie][ibl]=new TH1D(buf,buf,12,-0.5,11.5);
			sprintf(buf,"hmtdhitz_strip%d%d",ie,ibl);
			sprintf(tbuf,"hmtdhitz_strip pos=%d BL%d",ie+1,ibl+1);
			hmtdhitz_strip[ie][ibl]=new TH2D(buf,tbuf,12,-0.5,11.5,560,-7.,7.);
			sprintf(buf,"hmtdhitz_strip_zoff%d%d",ie,ibl);
			sprintf(tbuf,"hmtdhitz_strip_zoff pos=%d BL%d",ie+1,ibl+1);
			hmtdhitz_strip_zoff[ie][ibl]=new TH2D(buf,tbuf,12,-0.5,11.5,560,-7.,7.);
			//
			sprintf(buf,"hmtdstripcorr%d%d",ie,ibl);
			hmtdstripcorr[ie][ibl]=new TH2D(buf,buf,12,-0.5,11.5,12,-0.5,11.5);
			sprintf(buf,"hmtdnsignalsevent%d%d",ie,ibl);
			hmtdnsignalsevent[ie][ibl]=new TH1D(buf,buf,24,-0.5,23.5);
				hmtdnsignalsevent[ie][ibl]->SetMinimum(0.5);
				hmtdnsignalsevent[ie][ibl]->SetFillStyle(1001);
				hmtdnsignalsevent[ie][ibl]->SetFillColor(7);
			//
		}
	}
	TH2D *hmtdhitz_tota_strip1  = new TH2D("hmtdhitz_tota_strip1" ,"hmtdhitz_tota_strip1" ,50,5.,55.,275,-5.5,5.5);			
	TH2D *hmtdhitz_tota_strip6  = new TH2D("hmtdhitz_tota_strip6" ,"hmtdhitz_tota_strip6" ,50,5.,55.,275,-5.5,5.5);			
	TH2D *hmtdhitz_tota_strip12 = new TH2D("hmtdhitz_tota_strip12","hmtdhitz_tota_strip12",50,5.,55.,275,-5.5,5.5);			
	TH2D *hmtdhitz_totb_strip1  = new TH2D("hmtdhitz_totb_strip1" ,"hmtdhitz_totb_strip1" ,50,5.,55.,275,-5.5,5.5);			
	TH2D *hmtdhitz_totb_strip6  = new TH2D("hmtdhitz_totb_strip6" ,"hmtdhitz_totb_strip6" ,50,5.,55.,275,-5.5,5.5);			
	TH2D *hmtdhitz_totb_strip12 = new TH2D("hmtdhitz_totb_strip12","hmtdhitz_totb_strip12",50,5.,55.,275,-5.5,5.5);			
	TH2D *hmtdhitz_totm_strip1  = new TH2D("hmtdhitz_totm_strip1" ,"hmtdhitz_totm_strip1" ,50,5.,55.,275,-5.5,5.5);			
	TH2D *hmtdhitz_totm_strip6  = new TH2D("hmtdhitz_totm_strip6" ,"hmtdhitz_totm_strip6" ,50,5.,55.,275,-5.5,5.5);			
	TH2D *hmtdhitz_totm_strip12 = new TH2D("hmtdhitz_totm_strip12","hmtdhitz_totm_strip12",50,5.,55.,275,-5.5,5.5);			
	TH2D *hmtdhitz_totm_strip1_norm  = new TH2D("hmtdhitz_totm_strip1_norm" ,"hmtdhitz_totm_strip1_norm" ,50,5.,55.,275,-5.5,5.5);			
	TH2D *hmtdhitz_totm_strip6_norm  = new TH2D("hmtdhitz_totm_strip6_norm" ,"hmtdhitz_totm_strip6_norm" ,50,5.,55.,275,-5.5,5.5);			
	TH2D *hmtdhitz_totm_strip12_norm = new TH2D("hmtdhitz_totm_strip12_norm","hmtdhitz_totm_strip12_norm",50,5.,55.,275,-5.5,5.5);			
	//
	TH1D *htota_strip1  = new TH1D("htota_strip1" ,"htota_strip1" ,100,5.,55.);	htota_strip1->SetLineColor(3); 		
	TH1D *htota_strip6  = new TH1D("htota_strip6" ,"htota_strip6" ,100,5.,55.);	htota_strip6->SetLineColor(3);
	TH1D *htota_strip12 = new TH1D("htota_strip12","htota_strip12",100,5.,55.);	htota_strip12->SetLineColor(3);
	TH1D *htotb_strip1  = new TH1D("htotb_strip1" ,"htotb_strip1" ,100,5.,55.);	htotb_strip1->SetLineColor(4);
	TH1D *htotb_strip6  = new TH1D("htotb_strip6" ,"htotb_strip6" ,100,5.,55.);	htotb_strip6->SetLineColor(4);
	TH1D *htotb_strip12 = new TH1D("htotb_strip12","htotb_strip12",100,5.,55.);	htotb_strip12->SetLineColor(4);
	TH1D *htotm_strip1  = new TH1D("htotm_strip1" ,"htotm_strip1" ,100,5.,55.);	htotm_strip1->SetLineColor(1);
	TH1D *htotm_strip6  = new TH1D("htotm_strip6" ,"htotm_strip6" ,100,5.,55.);	htotm_strip6->SetLineColor(1);
	TH1D *htotm_strip12 = new TH1D("htotm_strip12","htotm_strip12",100,5.,55.);	htotm_strip12->SetLineColor(1);
	TH2D *htotm_strip	= new TH2D("htotm_strip"  ,"htotm_strip"  ,12,-0.5,11.5,100,5.,55.);

	TH2D *hmtdonesidedhit	= new TH2D("hmtdonesidedhit","hmtdonesidedhit",30,0.5,30.5,120,0.5,120.5);
	
	int   nch_vpd_e;
	int   ich_vpd_e[100];
	float tot_vpd_e[100];
	int   nch_vpd_w;
	int   ich_vpd_w[100];
	float tot_vpd_w[100];
	int   nch_mtd_e;
	int   ich_mtd_e[100];
	float tot_mtd_e[100];
	int   nch_mtd_w;
	int   ich_mtd_w[100];
	float tot_mtd_w[100];
	int kmtdtray,kmtdmod,kvpdcell,kmtdglcell,kmtdstrip,kmtdstrlr,kmtdmodch,ketaposn;
	float fmtdx;

	float mtdstrips[2][12][5][30];
	float mtdstrtot[2][12][5][30];
	
	//---- start event loop....
	if (fChaint == 0) return;
	Long64_t nentriestree = fChaint->GetEntries();
	Long64_t nentries;
	if (nToDo>0){ nentries = nToDo; } else { nentries = nentriestree; }
	cout<<"..... noise::loop  NentriesTree = "<<nentriestree<<endl;
	cout<<"..... noise::loop  nToDo        = "<<nToDo       <<endl;
	cout<<"..... noise::loop  nentries     = "<<nentries    <<endl;
	if (kRunUse<999999){
		thisyear		= kRunUse/1000;
		thisday			= kRunUse - thisyear*1000;
		thisrunonday	= 0;
	} else {
		thisyear		= kRunUse/1000000;
		thisday			=(kRunUse - thisyear*1000000)/1000;
		thisrunonday	= kRunUse - thisyear*1000000 - thisday*1000;
	}
	
	Long64_t nb = 0;
	for (Long64_t jentry=0; jentry<nentries;jentry++) {
		Long64_t ientry = LoadTree(jentry);
		if (ientry < 0) break;
		nb 		= fChaint->GetEntry(jentry);
		nb 		= fChainm->GetEntry(jentry);
		if (jentry%100000==0){ cout<< "Processing "<<jentry<<"\t "<<thisyear<<" "<<thisday<<" "<<thisrunonday<<endl; }

		//cout<<nTofHits<<" "<<nTofHitsLE<<" "<<nTofHitsTE<<" "
		//	<<nMtdHits<<" "<<nMtdHitsLE<<" "<<nMtdHitsTE<<endl;
		//continue;
		
		//---- start loop over hits in this event....
		//
		for (int it=0;it<300;it++){ nhtray[it] = nhtray_totL[it] = nhtray_totH[it] = 0.; }
		nhitevt_tot = nhitevt_totL = nhitevt_totH = 0;
		for (int i=0;i<30;i++){
			nhitevt_tot_mtdBL[i]	= 0;
			nhitevt_totL_mtdBL[i]	= 0;
			nhitevt_totH_mtdBL[i]	= 0;
		}
		nhitevt_tot_vpd = nhitevt_totL_vpd = nhitevt_totH_vpd = 0;
		nch_vpd_e = nch_vpd_w = 0;
		nch_mtd_e = nch_mtd_w = 0;
		int nhmtray[30]		= {0};
		for (int ib=0;ib<30;ib++){
			for (int ip=0;ip<5;ip++){
				for (int is=0;is<12;is++){
					for (int ilr=0;ilr<2;ilr++){
						mtdstrips[ilr][is][ip][ib] = -1;
						mtdstrtot[ilr][is][ip][ib] = -1;
					}
				}
			}
		}
		//
		//---- start loop over tof hits................
		for (int ih=0;ih<nTofHits;ih++){
			//
			float thistot	= tte[ih] - tle[ih];
			//
			LoopID	= tray[ih]/5;
			if (TrayID_LoopIn[LoopID] < TrayID_LoopOut[LoopID]){
				TrayIDinLoop	= tray[ih] - (TrayID_LoopIn[LoopID]-1);
			} else {
				TrayIDinLoop	= (TrayID_LoopIn[LoopID]-1) - tray[ih];
			}
			ModuleIDinLoop		= 32*TrayIDinLoop + module[ih];
			GlobalModule		= tray[ih]*32 + module[ih];
			GlModToIDinLoop[GlobalModule]	= ModuleIDinLoop;
			//
			if (tray[ih]<=120){ 
				nhittotal			+= 1;
			} else if (tray[ih]==121 || tray[ih]==122) {
				nhittotal_vpd		+= 1;
				kvpdcell			= cell[ih];
				hvpdcell->Fill(kvpdcell);
			}
			//
			hrate_tray->Fill(tray[ih]);
//if (tray[ih]==0){
//	cout<<ih<<" "<<nTofHits<<" "<<thistot<<" "<<module[ih]<<" "<<cell[ih]<<" "
//		<<" "<<TrayIDinLoop<<" "<<ModuleIDinLoop<<endl;
//}
			hrate_glmodule->Fill(GlobalModule);
			if (tray[ih]<120){
				hrate_trmodule->Fill(module[ih]);
				hrate_trmodule_loopid[TrayIDinLoop]->Fill(module[ih]);
				hrate_trmodcell_loopid[TrayIDinLoop]->Fill(cell[ih],module[ih]);
				hrate_trcell->Fill(cell[ih]);
				hrate_loopmodule->Fill(ModuleIDinLoop);
			}
			//
			if (tray[ih]<120){
				htotwide->Fill(thistot);
				htot->Fill(thistot);
			} else if (tray[ih]==121 || tray[ih]==122) {		//CORRECT IDS?!?!?!?!?!
				htot_vpd->Fill(thistot);
			}
			//
			if (tray[ih]<120 && thistot>11.0 && thistot< 30.){
				++nhitevt_tot; 				
				++nhtray[tray[ih]];
			}
			if (tray[ih]<120 && thistot>11.0 && thistot<=18.){ 
				++nhitevt_totL; 
				++nhtray_totL[tray[ih]]; 
				hrate_tray_totL->Fill(tray[ih]);
				hrate_glmodule_totL->Fill(GlobalModule);
				hrate_trmodule_totL->Fill(module[ih]);
				//cout<<ih<<" "<<tray[ih]<<" "<<thistot<<" "<<module[ih]<<" "<<TrayIDinLoop<<endl;
				hrate_trmodule_loopid_totL[TrayIDinLoop]->Fill(module[ih]);
				hrate_trmodcell_loopid_totL[TrayIDinLoop]->Fill(cell[ih],module[ih]);
				hrate_loopmodule_totL->Fill(ModuleIDinLoop);
				hrate_trcell_totL->Fill(cell[ih]);
			} else if (tray[ih]<120 && thistot>18. && thistot<30.){ 
				++nhitevt_totH; 
				++nhtray_totH[tray[ih]]; 
				hrate_tray_totH->Fill(tray[ih]);
				hrate_glmodule_totH->Fill(GlobalModule);
				hrate_trmodule_totH->Fill(module[ih]);
				hrate_trmodule_loopid_totH[TrayIDinLoop]->Fill(module[ih]);
				hrate_trmodcell_loopid_totH[TrayIDinLoop]->Fill(cell[ih],module[ih]);
				hrate_loopmodule_totH->Fill(ModuleIDinLoop);
				hrate_trcell_totH->Fill(cell[ih]);
			} 
			//
			if ((tray[ih]==121||tray[ih]==122) && thistot>10.0 && thistot< 40.){
				++nhitevt_tot_vpd; 			
				++nhtray[tray[ih]];
			}
		}	
		//---- end loop over tof hits....
	
		//---- start loop over mtd hits................
		int mtdnhitsmodule[5][30]	= {0};
		for (int ih=0;ih<nMtdHits;ih++){
			//
			kmtdtray		= mtray[ih];					// [1,30]
			kmtdmod			= mmodule[ih];					// 0,1,2,4,5
			kmtdstrip		= mstrip[ih];
			kmtdmodch		= mmodch[ih] - 1;
			kmtdglcell		= kmtdmod*24 + kmtdmodch;
			kmtdstrlr		= mstrlr[ih];
			if (kmtdtray>=12&&kmtdtray<=20){
				ketaposn	= ModToEtaPosn3[kmtdmod];		// 2-4
			} else {
				ketaposn	= ModToEtaPosn5[kmtdmod];		// 1-5
			}
			//
			//int kx		= ActiveRun13[kmtdtray-1] - 1;
			int kx			= ActiveRun14[kmtdtray-1] - 1;
			int ky			= (ketaposn-1)*24 + kmtdstrlr*12 + kmtdstrip - 1;
			if(kx>=0){ hmtdstripsbl->Fill(kx,ky,1); }
			//
			if (mtle[ih]>0){
				mtdnhitsmodule[ketaposn-1][kmtdtray-1]++;
			}
			//
//			cout<<kmtdmod<<" "<<ketaposn<<endl;
			float thistot	= mtte[ih] - mtle[ih];
			if (thistot<5.0) continue; 
			htot_mtd[kmtdtray-1]->Fill(thistot);
			++nhmtray[kmtdtray-1];
			//
			//if (mstrlr[ih] && mmodule[ih]<=2){ kmtdcell += 12; } else
			//if (mstrlr[ih] && mmodule[ih]==3){ kmtdcell += 18; }
			hcellmod->Fill(kmtdmod,kmtdglcell);
			if (kmtdmod<0||kmtdmod>5||kmtdglcell<0||kmtdglcell>=192){
				cout<<" huh ... "<<mtray[ih]<<" "<<kmtdmod<<" "<<kmtdglcell<<" "<<kmtdstrip<<endl;
			}
			nhittotal_mtd	+= 1;
			//hmtdcell->Fill(kmtdglcell);
			hmtdcell[ketaposn-1][kmtdtray-1]->Fill(kmtdmodch);
			hmtdcell_bl[kmtdtray-1]->Fill(kmtdglcell);
			if (thistot> 12.0 && thistot< 50.){
				++nhitevt_tot_mtdBL[kmtdtray-1];
			}
			if (thistot> 12.0 && thistot<=17.){ 
				++nhitevt_totL_mtdBL[kmtdtray-1];
				//hmtdcell_totL->Fill(kmtdglcell);
			} else if (thistot>17. && thistot<50.){ 
				++nhitevt_totH_mtdBL[kmtdtray-1];
				//hmtdcell_totH->Fill(kmtdglcell);
			} 
			//if (kmtdmod==-1){
			//  cout<<"MTD error.... "<<tray[ih]<<" "<<module[ih]<<" "<<cell[ih]<<" "<<module[ih]*6+cell[ih]<<endl;
			//}
// 			if (kmtdmod<=3){
// 				ich_mtd_w[nch_mtd_w]	= kmtdcell;
// 				tot_mtd_w[nch_mtd_w]	= thistot;
// 				nch_mtd_w++;
// 			} else {
// 				ich_mtd_e[nch_mtd_e]	= kmtdcell;
// 				tot_mtd_e[nch_mtd_e]	= thistot;
// 				nch_mtd_e++;
// 			}
			//
			//---- collect Z-positions of hits.....
			//
			mtdstrips[kmtdstrlr][kmtdstrip-1][ketaposn-1][kmtdtray-1] = mtle[ih];
			mtdstrtot[kmtdstrlr][kmtdstrip-1][ketaposn-1][kmtdtray-1] = mtot[ih];
			//cout<<kmtdstrlr<<" "<<kmtdstrip<<" "<<ketaposn<<" "<<kmtdtray<<" "<<mtle[ih]<<endl;
			//
		}	
		//---- end loop over mtd hits....
		
		//---- MTD Nsignals/event & stripcorr
		for (int ibl=0;ibl<30;ibl++){
			for (int ie=0;ie<5;ie++){
				//
				for (int il=0;il<12;il++){
					for (int ir=0;ir<12;ir++){
						float tl	= mtdstrips[0][il][ie][ibl];
						float tr	= mtdstrips[1][ir][ie][ibl];
						if (tl>0&&tr>0){
							hmtdstripcorr[ie][ibl]->Fill(il,ir,1.0);
//if (ibl==6&&ie==4){
//	cout<<il<<" "<<ir<<"\t"<<tl<<" "<<tr<<endl;
//}
						}
					}
				}
				//
				if (mtdnhitsmodule[ie][ibl]>0){
					hmtdnsignalsevent[ie][ibl]->Fill(mtdnhitsmodule[ie][ibl]);
				}
				//
			}
		}

		//---- form MTD hit Z-positions from mtdstrips bank...
		for (int ib=0;ib<30;ib++){
			for (int ip=0;ip<5;ip++){
				for (int is=0;is<12;is++){
					float sidea	= mtdstrips[0][is][ip][ib];
					float sideb	= mtdstrips[1][is][ip][ib];
					float tota	= mtdstrtot[0][is][ip][ib];
					float totb	= mtdstrtot[1][is][ip][ib];
					float totm	= TMath::Sqrt(mtdstrtot[0][is][ip][ib]*mtdstrtot[1][is][ip][ib]);
					float tave	=            (mtdstrips[0][is][ip][ib]+mtdstrips[1][is][ip][ib])/2.;
					if (sidea>0.&&sideb>0.){
						//
						float Z			= (sidea-sideb); 
						float xmean		= fxmeansuse[is][ip][ib];
						float xrange	= fxrangeuse[is][ip][ib];
						float zcal		= (Z-xmean)*(5.0/xrange);
						hmtdhitz_strip[ip][ib]->Fill(is,Z);
						hmtdhitz_strip_zoff[ip][ib]->Fill(is,zcal);
						//
						htotm_strip->Fill(is,totm); 
						if (is== 0){ 
							hmtdhitz_tota_strip1->Fill(tota,zcal); 
							hmtdhitz_totb_strip1->Fill(totb,zcal); 
							hmtdhitz_totm_strip1->Fill(totm,zcal); 
							hmtdhitz_totm_strip1_norm->Fill(totm,zcal); 
							htota_strip1->Fill(tota); 
							htotb_strip1->Fill(totb); 
							htotm_strip1->Fill(totm); 
						}
						if (is== 5){ 
							hmtdhitz_tota_strip6->Fill(tota,zcal); 
							hmtdhitz_totb_strip6->Fill(totb,zcal); 
							hmtdhitz_totm_strip6->Fill(totm,zcal); 
							hmtdhitz_totm_strip6_norm->Fill(totm,zcal); 
							htota_strip6->Fill(tota); 
							htotb_strip6->Fill(totb); 
							htotm_strip6->Fill(totm); 
						}
						if (is==11){ 
							hmtdhitz_tota_strip12->Fill(tota,zcal); 
							hmtdhitz_totb_strip12->Fill(totb,zcal); 
							hmtdhitz_totm_strip12->Fill(totm,zcal); 
							hmtdhitz_totm_strip12_norm->Fill(totm,zcal); 
							htota_strip12->Fill(tota); 
							htotb_strip12->Fill(totb); 
							htotm_strip12->Fill(totm); 
						}
						//
					} else if ((sidea>0&&sideb<=0)||(sidea<=0&&sideb>0)){
						int k12;
						if (sidea>0&&sideb<=0) k12= 0;
						if (sidea<=0&&sideb>0) k12=12;
						hmtdonesidedhit->Fill(ib+1,is+k12+(ip*24)+1);
						//if (ib+1==10&&ip+1==1){
						//	cout<<"One sided hit ... bl="<<ib+1<<" pos="<<ip+1<<" str="<<is<<"\t"
						//		<<sidea<<" "<<sideb<<"\t"<<(sidea-sideb)<<endl;
						//}
					}
				}
			}
		}

		//---- event processing...
		hnpev->Fill(nhitevt_tot);
		hnpev_totL->Fill(nhitevt_totL);
		hnpev_totH->Fill(nhitevt_totH);
		nhittotal_tot		+= nhitevt_tot;
		nhittotal_totL		+= nhitevt_totL;
		nhittotal_totH		+= nhitevt_totH;
		nhittotal_tot_vpd	+= nhitevt_tot_vpd;
		nhittotal_totL_vpd	+= nhitevt_totL_vpd;
		nhittotal_totH_vpd	+= nhitevt_totH_vpd;
		for (int i=0;i<30;i++){
			nhittotal_tot_mtdBL[i]	+= nhitevt_tot_mtdBL[i];
			nhittotal_totL_mtdBL[i]	+= nhitevt_totL_mtdBL[i];
			nhittotal_totH_mtdBL[i]	+= nhitevt_totH_mtdBL[i];
		}
		//
		for (int it=0;it<120;it++){ 
			//
			hnpevtray->Fill(nhtray[it]);
			hnpevtray_totL->Fill(nhtray_totL[it]);
			hnpevtray_totH->Fill(nhtray_totH[it]);
			//
			if(it<=120 && nhtray[it]>25){
				float	tear 	= 9999999.;
				int		ihear	= -1;
				int		imear	= -1;
				//---- find time of earliest hit in this event in this tray
				for (int ih=0;ih<nTofHits;ih++){
					if (tray[ih]=it){
						int		kglmod	= tray[ih]*32 + module[ih];
						float	tabs	= tle[ih]-off_tray[it]-off_glmod[kglmod];
						if (tabs<tear){
							tear	= tabs;
							ihear	= ih;
							imear	= module[ih];
						}
					}
				}
				//---- now plot times in this tray relative to earliest hit
				if (ihear>-1){
					for (int ih=0;ih<nTofHits;ih++){
						if (tray[ih]=it){
							int		kglmod	= tray[ih]*32 + module[ih];
							float	tabs	= tle[ih]-off_tray[it]-off_glmod[kglmod];
							htrel->Fill(tabs-tear);
							htrelzoom->Fill(tabs-tear);
							htrel_module->Fill(module[ih],tabs-tear);
							htrelzoom_module->Fill(module[ih],tabs-tear);
							htrel_relmodule->Fill(module[ih]-imear,tabs-tear);
							htrelzoom_relmodule->Fill(module[ih]-imear,tabs-tear);
							htrelzoom_relmod_imear[imear]->Fill(module[ih]-imear,tabs-tear);
							//if (tle[ih]-tear<0.1){
							//cout<<it<<" "<<nhtray[it]<<"\t "<<module[ih]<<" "<<cell[ih]<<"\t "
							//	<<tle[ih]<<" "<<tte[ih]-tle[ih]<<endl;
							//}
						}
					}
				}
				//
				htray_npevtray25->Fill(it);
			}
			//
			if(it<=120 && nhtray[it]>50){
				htray_npevtray50->Fill(it);
			}
			if(it<=120 && nhtray[it]>100){
				htray_npevtray100->Fill(it);
			}
			if(it<=120 && nhtray[it]>188){
				htray_npevtray190->Fill(it);
			}
			//
			for (int ih=0;ih<nTofHits;ih++){
				float thistot	= tte[ih] - tle[ih];
				if (tray[ih]<=120){
					htot_nh->Fill(nhtray[it],thistot);
					if (nhtray[it]<=10){ htot_nhlow->Fill(thistot); } else 
					if (nhtray[it]> 10){ htot_nhhig->Fill(thistot); } 
				}
			}
			//
		}
		
// 		for (int ie=0;ie<nch_mtd_e;ie++){
// 			for (int iw=0;iw<nch_mtd_w;iw++){
// 				//cout<<ie<<" "<<iw<<" "<<ich_mtd_w[iw]<<" "<<ich_mtd_e[ie]<<endl;
// 				hmtdmap->Fill(ich_mtd_w[iw],ich_mtd_e[ie]-24,1.0);
// 				if (ich_mtd_e[ie]-24 == ich_mtd_w[iw]){
// 					fmtdx	= (tot_mtd_e[ie]-tot_mtd_w[iw])/(tot_mtd_e[ie]+tot_mtd_w[iw]);
// 					htotx_mtd->Fill(fmtdx,tot_mtd_e[ie]);
// 				}
// 			}
// 		}

	}
	//---- end event loop....

	//---- end of loop processing...
	//
	float timetotal		= ((float)nentries)*TRIGGERWINDOWWIDTH_TOF;
	hinfo->Fill(0.0,timetotal);
	float rate_hits		= ((float)nhittotal)/timetotal;
	float rate_hitsLE	= ((float)nhitLEtotal)/timetotal;
	float rate_hitsTE	= ((float)nhitTEtotal)/timetotal;
	float rate_hits_tot = ((float)nhittotal_tot )/timetotal;
	float rate_hits_totL= ((float)nhittotal_totL)/timetotal;
	float rate_hits_totH= ((float)nhittotal_totH)/timetotal;
	//
	timetotal		= ((float)nentries)*TRIGGERWINDOWWIDTH_MTD;
	hinfo->Fill(1.0,timetotal);
	float rate_hits_mtd		= ((float)nhittotal_mtd)/timetotal;
//	float rate_hitsLE_mtd	= ((float)nhitLEtotal_mtd)/timetotal;
//	float rate_hitsTE_mtd	= ((float)nhitTEtotal_mtd)/timetotal;
	float rate_hits_tot_mtdBL[30];
	float rate_hits_totL_mtdBL[30];
	float rate_hits_totH_mtdBL[30];
	for (int i=0;i<30;i++){
		rate_hits_tot_mtdBL[i]	= ((float)nhittotal_tot_mtdBL[i])/timetotal;
		rate_hits_totL_mtdBL[i]	= ((float)nhittotal_totL_mtdBL[i])/timetotal;
		rate_hits_totH_mtdBL[i]	= ((float)nhittotal_totH_mtdBL[i])/timetotal;
	}
	//
	hinfo->Fill( 2.0,rate_hitsLE);
	hinfo->Fill( 3.0,rate_hitsTE);
	hinfo->Fill( 4.0,rate_hits);
	hinfo->Fill( 5.0,rate_hits_totL);
	hinfo->Fill( 6.0,rate_hits_totH);
	hinfo->Fill( 7.0,rate_hits_tot);
	hinfo->Fill( 8.0,rate_hits_mtd);
	hinfo->Fill( 9.0,nentries);
	for (int i=0;i<30;i++){
		hinfo->Fill(30.0+i,rate_hits_tot_mtdBL[i]);
	}
	//
	//
	timetotal		= ((float)nentries)*TRIGGERWINDOWWIDTH_TOF;
	hrate_tray->Scale(1./timetotal/192.);
	htray_npevtray25->Scale(1./timetotal/192.);
	htray_npevtray50->Scale(1./timetotal/192.);
	htray_npevtray100->Scale(1./timetotal/192.);
	htray_npevtray190->Scale(1./timetotal/192.);
	hrate_glmodule->Scale(1./timetotal/6.);
	hrate_trmodule->Scale(1./timetotal/6./120.);
	hrate_trcell->Scale(1./timetotal/120./32.);
	hrate_tray_totL->Scale(1./timetotal/192.);
	hrate_tray_totH->Scale(1./timetotal/192.);
	hrate_tray_totL->SetLineColor(4);
	hrate_tray_totH->SetLineColor(2);
	hrate_glmodule_totL->Scale(1./timetotal/6.);
	hrate_glmodule_totH->Scale(1./timetotal/6.);
	hrate_glmodule_totL->SetLineColor(4);
	hrate_glmodule_totH->SetLineColor(2);
	hrate_trmodule_totL->Scale(1./timetotal/6./120.);
	hrate_trmodule_totH->Scale(1./timetotal/6./120.);
	hrate_trmodule_totL->SetLineColor(4);
	hrate_trmodule_totH->SetLineColor(2);
	for (int id=0;id<5;id++){
		hrate_trmodule_loopid[id]->Scale(1./timetotal/6./24.);
		hrate_trmodule_loopid_totH[id]->Scale(1./timetotal/6./24.);
		hrate_trmodule_loopid_totL[id]->Scale(1./timetotal/6./24.);
		hrate_trmodule_loopid_totH[id]->SetLineColor(2);
		hrate_trmodule_loopid_totL[id]->SetLineColor(4);
		hrate_trmodcell_loopid[id]->Scale(1./timetotal/24.);
		hrate_trmodcell_loopid_totH[id]->Scale(1./timetotal/24.);
		hrate_trmodcell_loopid_totL[id]->Scale(1./timetotal/24.);
	}
	hrate_loopmodule->Scale(1./timetotal/6./24.);
	hrate_loopmodule_totH->Scale(1./timetotal/6./24.);
	hrate_loopmodule_totL->Scale(1./timetotal/6./24.);
	hrate_loopmodule_totH->SetLineColor(2);
	hrate_loopmodule_totL->SetLineColor(4);
	hrate_trcell_totL->Scale(1./timetotal/120./32.);
	hrate_trcell_totH->Scale(1./timetotal/120./32.);
	hrate_trcell_totL->SetLineColor(4);
	hrate_trcell_totH->SetLineColor(2);
	//hmtdcell_totL->SetLineColor(4);
	//hmtdcell_totH->SetLineColor(2);
	//
	timetotal		= ((float)nentries)*TRIGGERWINDOWWIDTH_MTD;
	hmtdstripsbl->Scale(1./timetotal);
	for (int ibl=0;ibl<30;ibl++){
		for (int ie=0;ie<5;ie++){
			hmtdcell[ie][ibl]->Scale(1./timetotal);
		}
		hmtdcell_bl[ibl]->Scale(1./timetotal);
	}
	//
	cout<<endl;
	cout<<" kRunUse               = "<<kRunUse    <<endl;
	cout<<" Nentries              = "<<nentries   <<endl;
	cout<<" TotalTime TOF (sec)   = "<<((float)nentries)*TRIGGERWINDOWWIDTH_TOF<<endl;
	cout<<" TotalTime MTD (sec)   = "<<((float)nentries)*TRIGGERWINDOWWIDTH_MTD<<endl;
	//cout<<" TOF rate LE  (Hz/ch)  = "<<rate_hitsLE/23040.<<"\t N = "<<nhitLEtotal<<endl;
	//cout<<" TOF rate TE  (Hz/ch)  = "<<rate_hitsTE/23040.<<"\t N = "<<nhitTEtotal<<endl;
	cout<<" TOF   rate      (Hz/ch) = "<<rate_hits/23040.   <<"\t N = "<<nhittotal    <<endl;
	cout<<" TOF   rate TOT  (Hz/ch) = "<<rate_hits_tot/23040. <<"\t N = "<<nhittotal_tot <<endl;
	cout<<" TOF   rate TOTL (Hz/ch) = "<<rate_hits_totL/23040.<<"\t N = "<<nhittotal_totL<<endl;
	cout<<" TOF   rate TOTH (Hz/ch) = "<<rate_hits_totH/23040.<<"\t N = "<<nhittotal_totH<<endl;
	//cout<<" MTD rate LE  (Hz/ch)  = "<<rate_hitsLE_mtd/36.<<"\t N = "<<nhitLEtotal_mtd    <<endl;
	//cout<<" MTD rate TE  (Hz/ch)  = "<<rate_hitsTE_mtd/36.<<"\t N = "<<nhitTEtotal_mtd    <<endl;
	//	cout<<" MTD   rate      (Hz/ch) = "<<rate_hits_mtd/108.  <<"\t N = "<<nhittotal_mtd<<endl;
	cout<<" MTD   rate      (Hz/ch) = "<<rate_hits_mtd/1439.  <<"\t N = "<<nhittotal_mtd<<endl;
// 	cout<<" MTD9  rate TOT  (Hz/ch) = "<<rate_hits_tot_mtd9/36. <<"\t N = "<<nhittotal_tot_mtd9<<endl;
// 	cout<<" MTD9  rate TOTL (Hz/ch) = "<<rate_hits_totL_mtd9/36.<<"\t N = "<<nhittotal_totL_mtd9<<endl;
// 	cout<<" MTD9  rate TOTH (Hz/ch) = "<<rate_hits_totH_mtd9/36.<<"\t N = "<<nhittotal_totH_mtd9<<endl;
// 	cout<<" MTD11 rate TOT  (Hz/ch) = "<<rate_hits_tot_mtd11/72. <<"\t N = "<<nhittotal_tot_mtd11<<endl;
// 	cout<<" MTD11 rate TOTL (Hz/ch) = "<<rate_hits_totL_mtd11/72.<<"\t N = "<<nhittotal_totL_mtd11<<endl;
// 	cout<<" MTD11 rate TOTH (Hz/ch) = "<<rate_hits_totH_mtd11/72.<<"\t N = "<<nhittotal_totH_mtd11<<endl;
//	cout<<" MTD26 rate TOT (Hz/ch) = "<<rate_hits_tot_mtd26/24. <<"\t N = "<<nhittotal_tot_mtd26<<endl;
//	cout<<" MTD27 rate TOT (Hz/ch) = "<<rate_hits_tot_mtd27/24. <<"\t N = "<<nhittotal_tot_mtd27<<endl;
//	cout<<" MTD28 rate TOT (Hz/ch) = "<<rate_hits_tot_mtd28/24. <<"\t N = "<<nhittotal_tot_mtd28<<endl;

	//---- form striprat plots
	for (int ieta=0;ieta<5;ieta++){
		for (int ibl=0;ibl<30;ibl++){
			if (hmtdcell[ieta][ibl]->GetEntries()){
				for (int ichan=0;ichan<12;ichan++){
					double rl	= hmtdcell[ieta][ibl]->GetBinContent(ichan+1);
					double rr	= hmtdcell[ieta][ibl]->GetBinContent(ichan+1+12);
					double rat	= 0;
					if (rl>0 && rl>=rr){
						rat		= rr/rl;
					} else if (rr>0 && rr>rl){
						rat		= rl/rr;
					}
					//if (ieta==0&&ibl==0){
					//	cout<<ibl<<" "<<ieta<<" "<<ichan<<" \t "<<rl<<" "<<rr<<" "<<rat<<endl;
					//}
					hmtdstriprat[ieta][ibl]->Fill(ichan,rat);
				}
			}
		}
	}
	


	//---- make rate by tray vs IDinLoop graph....
	TGraph *trayratebyloopid[24];
	TGraph *trayratebyloopid_totH[24];
	TGraph *trayratebyloopid_totL[24];
	float	highestrate = 0;
	for (int il=0;il<24;il++){
		sprintf(buf,"trayratebyloopid%d",il);
			trayratebyloopid[il] = new TGraph();
			trayratebyloopid[il]->SetMarkerStyle(20);
			trayratebyloopid[il]->SetName(buf);
		sprintf(buf,"trayratebyloopid_totH%d",il);
			trayratebyloopid_totH[il] = new TGraph();
			trayratebyloopid_totH[il]->SetMarkerStyle(20);
			trayratebyloopid_totH[il]->SetMarkerColor(2);
			trayratebyloopid_totH[il]->SetLineColor(2);
			trayratebyloopid_totH[il]->SetName(buf);
		sprintf(buf,"trayratebyloopid_totL%d",il);
			trayratebyloopid_totL[il] = new TGraph();
			trayratebyloopid_totL[il]->SetMarkerStyle(20);
			trayratebyloopid_totL[il]->SetMarkerColor(4);
			trayratebyloopid_totL[il]->SetLineColor(4);
			trayratebyloopid_totL[il]->SetName(buf);
		for (int itil=0;itil<5;itil++){
			int	  ib	= il*5 + itil + 1;
			int   it	= hrate_tray->GetBinCenter(ib);
			float rate	= hrate_tray->GetBinContent(ib);
			float rateH	= hrate_tray_totH->GetBinContent(ib);
			float rateL	= hrate_tray_totL->GetBinContent(ib);
			//cout<<il<<" "<<itil<<" "<<ib<<" "<<it<<" "<<rate<<" "<<highestrate<<endl;
			if (rate > highestrate){
				highestrate	= rate;
			}
			//
			LoopID	= il;
			if (TrayID_LoopIn[LoopID] < TrayID_LoopOut[LoopID]){
				TrayIDinLoop	= it - (TrayID_LoopIn[LoopID]-1);
			} else {
				TrayIDinLoop	= (TrayID_LoopIn[LoopID]-1) - it;
			}
			//
			if (rate>=1.0){
				int kkk	= trayratebyloopid[il]->GetN();
				trayratebyloopid[il]->SetPoint(kkk,TrayIDinLoop,rate);
				kkk		= trayratebyloopid_totH[il]->GetN();
				trayratebyloopid_totH[il]->SetPoint(kkk,TrayIDinLoop,rateH);
				kkk		= trayratebyloopid_totL[il]->GetN();
				trayratebyloopid_totL[il]->SetPoint(kkk,TrayIDinLoop,rateL);
			}
		}
	}
	//
	float	highestrateloopmod = 0;
	for (int ib=1;ib<=160;ib++){
		float rate	= hrate_loopmodule->GetBinContent(ib);
		if (rate > highestrateloopmod){
			highestrateloopmod	= rate;
		}
	}
	//
	cout<<" Highest TrayRateByLoopID      = "<<highestrate<<endl;
	cout<<" Highest TrayRateByLoopMopdule = "<<highestrateloopmod<<endl;
	//

	//-------------------------------------------------------------------------
	//---- make plots....
	//
	gROOT->SetStyle("Plain"); 
	gStyle->SetOptStat(0);
	gStyle->SetOptFit(1111);
	gStyle->SetPadBottomMargin(0.07);
	gStyle->SetPadTopMargin(0.05);
	gStyle->SetPadLeftMargin(0.09);
	gStyle->SetPadRightMargin(0.02);
	gStyle->SetOptFit(0);
	gStyle->SetPalette(1);
	gStyle->SetTitleFontSize(0.05);
	gStyle->SetLabelSize(0.06,"x");
	gStyle->SetLabelSize(0.06,"y");
	//
 	++ican; sprintf(buf,"ccan%d",ican);
 	ccan[ican] = new TCanvas(buf,buf,0,0,800.,800.*(7.5/10.));
 	ccan[ican]->SetFillColor(10);
 	ccan[ican]->cd(); ccan[ican]->Divide(1,4,0.0001,0.0001);
		ccan[ican]->cd(1);
			gPad->SetLogy(1);
			hrate_tray->SetMinimum(0.1);	
			hrate_tray->Draw();
			hrate_tray_totH->Draw("same");
			hrate_tray_totL->Draw("same");
		ccan[ican]->cd(2);
			hrate_glmodule->SetMinimum(0);	
			hrate_glmodule->Draw();			
			hrate_glmodule_totH->Draw("same");
			hrate_glmodule_totL->Draw("same");
		ccan[ican]->cd(3);
			hrate_trmodule->SetMinimum(0);	
			hrate_trmodule->Draw();	
			hrate_trmodule_totH->Draw("same");
			hrate_trmodule_totL->Draw("same");
		ccan[ican]->cd(4);
			hrate_trcell->SetMinimum(0);	
			hrate_trcell->Draw();			
			hrate_trcell_totH->Draw("same");
			hrate_trcell_totL->Draw("same");
	ccan[ican]->cd(); ccan[ican]->Update();
	ccan[ican]->Print("noise.ps(");



// 	++ican; sprintf(buf,"ccan%d",ican);
// 	ccan[ican] = new TCanvas(buf,buf,0,0,800.,800.*(7.5/10.));
// 	ccan[ican]->SetFillColor(10);
// 	ccan[ican]->cd(); ccan[ican]->Divide(1,1,0.0001,0.0001);
//			ccan[ican]->cd(1);
//			hmtdonesidedhit->Draw("colz");
//	ccan[ican]->cd(); ccan[ican]->Update();
//	ccan[ican]->Print("noise.ps");
	//
//  	++ican; sprintf(buf,"ccan%d",ican);
//  	ccan[ican] = new TCanvas(buf,buf,0,0,800.,800.*(7.5/10.));
//  	ccan[ican]->SetFillColor(10);
//  	ccan[ican]->cd(); ccan[ican]->Divide(5,6,0.0001,0.0001);
//  	TH1D *htemp[30];
//  	int kkk	= 0;
// 	for (int ibl=1;ibl<=30;ibl++){
// 		sprintf(buf,"htemp%d",ibl-1);
// 		htemp[ibl-1]	= (TH1D*)hmtdonesidedhit->ProjectionY(buf,ibl,ibl);
// 		if (ActiveRun14[ibl-1]>0){
// 			++kkk;
// 			ccan[ican]->cd(kkk);
// 			htemp[ibl-1]->Draw();
// 		}
// 	}
// 	ccan[ican]->cd(); ccan[ican]->Update();
// 	ccan[ican]->Print("noise.ps");
	
	
	
	TH1F *frame;
	//
//  	++ican; sprintf(buf,"ccan%d",ican);
//  	ccan[ican] = new TCanvas(buf,buf,0,0,800.,800.*(7.5/10.));
//  	ccan[ican]->SetFillColor(10);
//  	ccan[ican]->cd(); ccan[ican]->Divide(3,1,0.0001,0.0001);
// 		ccan[ican]->cd(1);
// 			frame = gPad->DrawFrame(-0.5,0.0,4.5,1.05*highestrate);
// 			for (int il=0;il<24;il++){
// 				trayratebyloopid[il]->Draw("PL");
// 			}
// 		ccan[ican]->cd(2);
// 			frame = gPad->DrawFrame(-0.5,0.0,4.5,1.05*highestrate);
// 			for (int il=0;il<24;il++){
// 				trayratebyloopid_totL[il]->Draw("PL");
// 			}
// 		ccan[ican]->cd(3);
// 			frame = gPad->DrawFrame(-0.5,0.0,4.5,1.05*highestrate);
// 			for (int il=0;il<24;il++){
// 				trayratebyloopid_totH[il]->Draw("PL");
// 			}
// 	ccan[ican]->cd(); ccan[ican]->Update();
// 	ccan[ican]->Print("noise.ps");
	//
//  	++ican; sprintf(buf,"ccan%d",ican);
//  	ccan[ican] = new TCanvas(buf,buf,0,0,800.,800.*(7.5/10.));
//  	ccan[ican]->SetFillColor(10);
//  	ccan[ican]->cd(); ccan[ican]->Divide(3,2,0.0001,0.0001);
// 			for (int id=0;id<5;id++){
// 				ccan[ican]->cd(id+1);
// 				//frame = gPad->DrawFrame(-0.5,0.0,31.5,30.0);
// 					hrate_trmodule_loopid[id]->SetMinimum( 0);
// 					hrate_trmodule_loopid[id]->SetMaximum(1.05*highestrateloopmod);
// 					hrate_trmodule_loopid[id]->Draw();
// 					hrate_trmodule_loopid_totH[id]->Draw("same");
// 					hrate_trmodule_loopid_totL[id]->Draw("same");
// 			}
// 	ccan[ican]->cd(); ccan[ican]->Update();
// 	ccan[ican]->Print("noise.ps");
	//
	TLine *traybreaks[5];
	TLine *traybreaksdiff[5];
	for (int i=0;i<5;i++){
		float x			= ((float)(i*32)) - 0.5;
		traybreaks[i]	= new TLine(x,0.,x,1.05*highestrateloopmod);
			traybreaks[i]->SetLineColor(17);
		traybreaksdiff[i]	= new TLine(x,-15,x,25.);
			traybreaksdiff[i]->SetLineColor(17);
	}
	//
	//---- page 4..!!!!!!!!...................................
	reject = kTRUE;
	Double_t flinpars[2];
	Double_t flinparsH[2];
	TF1 *fl = new TF1("fl",fline,-0.5,159.5,2);
	hrate_loopmodule->Fit(fl,"0Q");
		fl->GetParameters(flinpars);
		TF1 *flall = new TF1("flall","pol1",-0.5,159.5);
			flall->SetParameters(flinpars);	
	TF1 *flH = new TF1("flH",fline,-0.5,159.5,2);
	hrate_loopmodule_totH->Fit(flH,"0Q");
		flH->GetParameters(flinparsH);
		TF1 *flallH = new TF1("flallH","pol1",-0.5,159.5);
			flallH->SetParameters(flinparsH);	
			flallH->SetLineColor(2);
	//
	hinfo->Fill(20.0,flinpars[0]);
	hinfo->Fill(21.0,flinpars[1]);
	cout<<" Loop Intercept = "<<flinpars[0]<<endl;
	cout<<" Loop Slope     = "<<flinpars[1]<<endl;
	//
 	++ican; sprintf(buf,"ccan%d",ican);
 	ccan[ican] = new TCanvas(buf,buf,0,0,800.,800.*(7.5/10.));
 	ccan[ican]->SetFillColor(10);
 	ccan[ican]->cd(); ccan[ican]->Divide(1,1,0.0001,0.0001);
			ccan[ican]->cd(1);
			//frame = gPad->DrawFrame(-0.5,0.0,31.5,30.0);
				hrate_loopmodule->SetMinimum( 0);
				hrate_loopmodule->SetMaximum(1.05*highestrateloopmod);
				hrate_loopmodule->Draw();
				hrate_loopmodule_totH->Draw("same");
				hrate_loopmodule_totL->Draw("same");
				flall->Draw("same");
				flallH->Draw("same");
				for (int i=0;i<5;i++){
					traybreaks[i]->Draw();
				}
				hrate_loopmodule->Draw("same");
				hrate_loopmodule_totH->Draw("same");
				hrate_loopmodule_totL->Draw("same");
	ccan[ican]->cd(); ccan[ican]->Update();
	ccan[ican]->Print("noise.ps");
	//
	TH1D *temp	= new TH1D("temp","difference between noise rate and mid-tray fit",160,-0.5,159.5);
		temp->SetLineColor(1);
	TH1D *tempH	= new TH1D("tempH","difference between noise rate and mid-tray fit",160,-0.5,159.5);
		tempH->SetLineColor(2);
	for (int ib=1;ib<=160;ib++){
		int   itrayinloop	= hrate_loopmodule->GetBinCenter(ib);
		float thisnoise		= hrate_loopmodule->GetBinContent(ib);
		float thisfitval	= flall->Eval(itrayinloop);
		float diff			= thisnoise - thisfitval;
		temp->SetBinContent(ib,diff);
		      itrayinloop	= hrate_loopmodule_totH->GetBinCenter(ib);
		      thisnoise		= hrate_loopmodule_totH->GetBinContent(ib);
		      thisfitval	= flallH->Eval(itrayinloop);
		      diff			= thisnoise - thisfitval;
		tempH->SetBinContent(ib,diff);
	}
	//
	TLine *lzero = new TLine(-0.5,0,159.5,0);
 	++ican; sprintf(buf,"ccan%d",ican);
 	ccan[ican] = new TCanvas(buf,buf,0,0,800.,800.*(7.5/10.));
 	ccan[ican]->SetFillColor(10);
 	ccan[ican]->cd(); ccan[ican]->Divide(1,1,0.0001,0.0001);
			ccan[ican]->cd(1);
				temp->SetMinimum(-15.);
				temp->SetMaximum( 25.);
				temp->Draw();
				tempH->Draw("same");
				lzero->Draw();
				for (int i=0;i<5;i++){
					traybreaksdiff[i]->Draw();
				}
				temp->Draw("same");
	ccan[ican]->cd(); ccan[ican]->Update();
	ccan[ican]->Print("noise.ps");
	//
	gStyle->SetPadLeftMargin(0.04);
	gStyle->SetPadRightMargin(0.10);
 	++ican; sprintf(buf,"ccan%d",ican);
 	ccan[ican] = new TCanvas(buf,buf,0,0,800.,800.*(7.5/10.));
 	ccan[ican]->SetFillColor(10);
 	ccan[ican]->cd(); ccan[ican]->Divide(5,1,0.0001,0.0001);
		for (int id=0;id<5;id++){
			ccan[ican]->cd(id+1);
			//frame = gPad->DrawFrame(-0.5,0.0,31.5,30.0);
				hrate_trmodcell_loopid[id]->SetMinimum( 0);
				hrate_trmodcell_loopid[id]->SetMaximum(1.05*highestrateloopmod);
				if (id!=4){
					hrate_trmodcell_loopid[id]->Draw("col");
				} else {
					hrate_trmodcell_loopid[id]->Draw("colz");
				}
				//hrate_trmodcell_loopid_totH[id]->Draw("same");
				//hrate_trmodcell_loopid_totL[id]->Draw("same");
		}
	ccan[ican]->cd(); ccan[ican]->Update();
	ccan[ican]->Print("noise.ps");
	gStyle->SetPadLeftMargin(0.09);
	gStyle->SetPadRightMargin(0.10);
		
	//---- loop-correct phi noise....................................
	TH1D *hrate_nphi	= new TH1D("hrate_nphi","NModules by #Phi",60,-3.,357.);
	TH1D *hrate_phi		= new TH1D("hrate_phi","rate/cell by #Phi, IDinLoop-Corrected",60,-3.,357.);
	TH1D *hrate_phi_norm= new TH1D("hrate_phi_norm","rate/cell by #Phi, IDinLoop-Corrected",60,-3.,357.);
	TH1D *hratee_nphi	= new TH1D("hratee_nphi","NModules by #Phi_{East}",60,-3.,357.);
	TH1D *hratee_phi	= new TH1D("hratee_phi","rate/cell by #Phi_{East}, IDinLoop-Corrected",60,-3.,357.);
	TH1D *hratee_phi_norm= new TH1D("hratee_phi_norm","rate/cell by #Phi_{East}, IDinLoop-Corrected",60,-3.,357.);
	TH1D *hratew_nphi	= new TH1D("hratew_nphi","NModules by #Phi_{West}",60,-3.,357.);
	TH1D *hratew_phi	= new TH1D("hratew_phi","rate/cell by #Phi_{West}, IDinLoop-Corrected",60,-3.,357.);
	TH1D *hratew_phi_norm= new TH1D("hratew_phi_norm","rate/cell by #Phi_{West}, IDinLoop-Corrected",60,-3.,357.);
	hrate_phi->Sumw2();
	hratee_phi->Sumw2();
	hratew_phi->Sumw2();
	hrate_phi_norm->Sumw2();
	hratee_phi_norm->Sumw2();
	hratew_phi_norm->Sumw2();
	int fifthtraymask[120] = {0};
	float aminval,rate,valfit;
	int		istyle	= 1;
	for (int i=0;i<3840;i++){
		//
		int 	idinloop	= GlModToIDinLoop[i];
		int		itrid		= i/32;
		float	phi			= RadToDeg*TrayToPhi(itrid+1);
		//
		//---- use total noise and loopID fit...
		if (istyle==1){
			rate		= hrate_glmodule->GetBinContent(i+1);
			//valfit	= flall->Eval(idinloop);							// use fit
			valfit		= hrate_loopmodule->GetBinContent(idinloop+1);	// use value
			aminval		= 1.0;
		//---- use low-ToT noise and loopID values...
		} else if (istyle==2){
			rate		= hrate_glmodule_totL->GetBinContent(i+1);
			valfit		= hrate_loopmodule_totL->GetBinContent(idinloop+1);
			aminval		= 0.1;
		}
		//
		float	thisrate	= rate - valfit;
		if (idinloop==4){ fifthtraymask[itrid]	= 1; }
		if (rate>=aminval){
			hrate_nphi->Fill(phi,1.);
			hrate_phi->Fill(phi,thisrate);
			hrate_phi_norm->Fill(phi,thisrate);
			if (itrid<60){
				hratew_nphi->Fill(phi,1.);
				hratew_phi->Fill(phi,thisrate);
				hratew_phi_norm->Fill(phi,thisrate);
			} else {
				hratee_nphi->Fill(phi,1.);
				hratee_phi->Fill(phi,thisrate);
				hratee_phi_norm->Fill(phi,thisrate);
			}
		}
		//if (i<160){
		//	cout<<i<<" "<<rate<<" "<<idinloop<<" "<<valfit<<" "<<itrid<<" "<<phi<<" "<<thisrate<<endl;
		//}
	}
	hrate_phi_norm->Divide(hrate_nphi);
	hratee_phi_norm->Divide(hratee_nphi);
	hratew_phi_norm->Divide(hratew_nphi);
	TF1 *flzero	= new TF1("flzero","pol0",-3,357);
		flzero->SetParameter(0,0.0);
		flzero->SetLineWidth(1);
		flzero->SetLineColor(3);
	TF1 *sintest	= new TF1("sintest",sinx,-3,357,1);
		sintest->SetLineWidth(1);
		sintest->SetLineColor(4);
	TF1 *sinteste	= new TF1("sinteste",sinx,-3,357,1);
		sinteste->SetLineWidth(1);
		sinteste->SetLineColor(4);
	TF1 *sintestw	= new TF1("sintestw",sinx,-3,357,1);
		sintestw->SetLineWidth(1);
		sintestw->SetLineColor(4);
	hrate_phi_norm->Fit("sintest","0N");
	hratee_phi_norm->Fit("sinteste","0N");
	hratew_phi_norm->Fit("sintestw","0N");
	//	
 	++ican; sprintf(buf,"ccan%d",ican);
 	ccan[ican] = new TCanvas(buf,buf,0,0,800.,800.*(7.5/10.));
 	ccan[ican]->SetFillColor(10);
 	ccan[ican]->cd(); ccan[ican]->Divide(1,3,0.0001,0.0001);
	ccan[ican]->cd(1);
		if (istyle==1){ hrate_glmodule->Draw(); } else
		if (istyle==2){ hrate_glmodule_totL->Draw(); } 
	ccan[ican]->cd(2);
		hrate_nphi->Draw();
	ccan[ican]->cd(3);
		hrate_phi_norm->SetMarkerStyle(20);
		hrate_phi_norm->Draw();
		flzero->Draw("same");
		sintest->Draw("same");
		hrate_phi_norm->Draw("same");
	ccan[ican]->cd(); ccan[ican]->Update();
	ccan[ican]->Print("noise.ps");
	//	
//  	++ican; sprintf(buf,"ccan%d",ican);
//  	ccan[ican] = new TCanvas(buf,buf,0,0,800.,800.*(7.5/10.));
//  	ccan[ican]->SetFillColor(10);
//  	ccan[ican]->cd(); ccan[ican]->Divide(1,3,0.0001,0.0001);
// 	ccan[ican]->cd(1);
//		if (istyle==1){ hrate_glmodule->Draw(); } else
//		if (istyle==2){ hrate_glmodule_totL->Draw(); } 
// 	ccan[ican]->cd(2);
// 		hratew_nphi->Draw();
// 	ccan[ican]->cd(3);
// 		hratew_phi_norm->SetMarkerStyle(20);
// 		hratew_phi_norm->Draw();
// 		flzero->Draw("same");
// 		sintestw->Draw("same");
// 		hratew_phi_norm->Draw("same");
// 	ccan[ican]->cd(); ccan[ican]->Update();
// 	ccan[ican]->Print("noise.ps");
// 	//	
//  	++ican; sprintf(buf,"ccan%d",ican);
//  	ccan[ican] = new TCanvas(buf,buf,0,0,800.,800.*(7.5/10.));
//  	ccan[ican]->SetFillColor(10);
//  	ccan[ican]->cd(); ccan[ican]->Divide(1,3,0.0001,0.0001);
// 	ccan[ican]->cd(1);
//		if (istyle==1){ hrate_glmodule->Draw(); } else
//		if (istyle==2){ hrate_glmodule_totL->Draw(); } 
// 	ccan[ican]->cd(2);
// 		hratee_nphi->Draw();
// 	ccan[ican]->cd(3);
// 		hratee_phi_norm->SetMarkerStyle(20);
// 		hratee_phi_norm->Draw();
// 		flzero->Draw("same");
// 		sinteste->Draw("same");
// 		hratee_phi_norm->Draw("same");
// 	ccan[ican]->cd(); ccan[ican]->Update();
// 	ccan[ican]->Print("noise.ps");
	
 	++ican; sprintf(buf,"ccan%d",ican);
 	ccan[ican] = new TCanvas(buf,buf,0,0,800.,800.*(7.5/10.));
 	ccan[ican]->SetFillColor(10);
 	ccan[ican]->cd(); ccan[ican]->Divide(1,4,0.0001,0.0001);
			ccan[ican]->cd(1);
				htray_npevtray25->Draw();
			ccan[ican]->cd(2);
				htray_npevtray50->Draw();
			ccan[ican]->cd(3);
				htray_npevtray100->Draw();
			ccan[ican]->cd(4);
				htray_npevtray190->Draw();
	ccan[ican]->cd(); ccan[ican]->Update();
	ccan[ican]->Print("noise.ps");

	//----- here's the old plots...................................
 	++ican; sprintf(buf,"ccan%d",ican);
 	ccan[ican] = new TCanvas(buf,buf,0,0,800.,800.*(7.5/10.));
 	ccan[ican]->SetFillColor(10);
 	ccan[ican]->cd(); ccan[ican]->Divide(1,2,0.0001,0.0001);
		ccan[ican]->cd(1);
			gPad->SetLogy(1);
			hnpev->Draw();			
			hnpev_totL->SetLineColor(4);			
			hnpev_totH->SetLineColor(2);			
			hnpev_totL->Draw("same");			
			hnpev_totH->Draw("same");			
		ccan[ican]->cd(2);
			gPad->SetLogy(1);
			hnpevtray->Draw();			
			hnpevtray_totL->SetLineColor(4);			
			hnpevtray_totH->SetLineColor(2);			
			hnpevtray_totL->Draw("same");			
			hnpevtray_totH->Draw("same");			
	ccan[ican]->cd(); ccan[ican]->Update();
	ccan[ican]->Print("noise.ps");
	//
 	++ican; sprintf(buf,"ccan%d",ican);
 	ccan[ican] = new TCanvas(buf,buf,0,0,800.,800.*(7.5/10.));
 	ccan[ican]->SetFillColor(10);
 	ccan[ican]->cd(); ccan[ican]->Divide(3,2,0.0001,0.0001);
		ccan[ican]->cd(1);
			gPad->SetLogy(1);
			htrel->Draw();			
		ccan[ican]->cd(2);
			gPad->SetLogz(1);
			htrel_module->Draw("colz");			
		ccan[ican]->cd(3);
			gPad->SetLogz(1);
			htrel_relmodule->Draw("colz");			
		ccan[ican]->cd(4);
			gPad->SetLogy(1);
			htrelzoom->Draw();			
		ccan[ican]->cd(5);
			gPad->SetLogy(0);
			htrelzoom_module->Draw("colz");			
		ccan[ican]->cd(6);
			gPad->SetLogy(0);
			htrelzoom_relmodule->Draw("colz");			
	ccan[ican]->cd(); ccan[ican]->Update();
	ccan[ican]->Print("noise.ps");

	TLine *lpos	= new TLine(  0.0, 0.0,32.0,18.0); lpos->SetLineColor(7);
	TLine *lneg	= new TLine(-32.0,18.0, 0.0, 0.0); lneg->SetLineColor(7);
 	++ican; sprintf(buf,"ccan%d",ican);
 	ccan[ican] = new TCanvas(buf,buf,0,0,1120,800);
 	ccan[ican]->SetFillColor(10);
 	ccan[ican]->cd(); ccan[ican]->Divide(8,4,0.0001,0.0001);
	for (int i=0;i<32;i++){
		ccan[ican]->cd(1+i);
		htrelzoom_relmod_imear[i]->Draw("colz");
		lpos->Draw(); 
		lneg->Draw();
	}
	ccan[ican]->cd(); ccan[ican]->Update();
	ccan[ican]->Print("noise.ps");

 	++ican; sprintf(buf,"ccan%d",ican);
 	ccan[ican] = new TCanvas(buf,buf,0,0,800.,800.*(7.5/10.));
 	ccan[ican]->SetFillColor(10);
 	ccan[ican]->cd(); ccan[ican]->Divide(2,2,0.0001,0.0001);
		ccan[ican]->cd(1);
			gPad->SetLogy(1);
			htotwide->Draw();			
		ccan[ican]->cd(2);
			gPad->SetLogy(0);
			htot->Draw();
		ccan[ican]->cd(3);
			gPad->SetLogz(1);
			htot_nh->Draw("colz");			
		ccan[ican]->cd(4);
			htot_nhlow->SetLineColor(4);
			htot_nhlow->Scale(1./htot_nhlow->Integral());	
			htot_nhlow->Draw();	
			htot_nhhig->SetLineColor(2);
			htot_nhhig->Scale(1./htot_nhhig->Integral());	
			htot_nhhig->Draw("same");
	ccan[ican]->cd(); ccan[ican]->Update();
	ccan[ican]->Print("noise.ps");

//  	++ican; sprintf(buf,"ccan%d",ican);
//  	ccan[ican] = new TCanvas(buf,buf,0,0,800.,800.*(7.5/10.));
//  	ccan[ican]->SetFillColor(10);
//  	ccan[ican]->cd(); ccan[ican]->Divide(2,2,0.0001,0.0001);
// 		ccan[ican]->cd(1);
// 			gPad->SetLogy(1);
// 			htot_vpd->Draw();			
// 		ccan[ican]->cd(2);
// 			htotx_vpd->Draw("colz");			
// 		ccan[ican]->cd(3);
// 			//hvpdmap->Draw("colz");			
// 			hvpdcell->Draw();			
// 		ccan[ican]->cd(4);
// 			hp_vpd_tof->Draw();
// 	ccan[ican]->cd(); ccan[ican]->Update();
// 	ccan[ican]->Print("noise.ps");

//  	++ican; sprintf(buf,"ccan%d",ican);
//  	ccan[ican] = new TCanvas(buf,buf,0,0,800.,800.*(7.5/10.));
//  	ccan[ican]->SetFillColor(10);
//  	ccan[ican]->cd(); ccan[ican]->Divide(1,1,0.0001,0.0001);
// 		ccan[ican]->cd(1);
// 			gPad->SetLogy(1);
// 			//hmtdcell->SetFillColor(5);
// 			//hmtdcell->SetFillStyle(1001);
// 			hmtdcell->SetMinimum(0.5);			
// 			hmtdcell->Draw();			
// 			hmtdcell_totH->Draw("same");			
// 			hmtdcell_totL->Draw("same");			
// 	ccan[ican]->cd(); ccan[ican]->Update();
// 	ccan[ican]->Print("noise.ps");


	gStyle->SetPadTopMargin(0.064);
 	++ican; sprintf(buf,"ccan%d",ican);
 	ccan[ican] = new TCanvas(buf,buf,0,0,800.,800.*(7.5/10.));
 	ccan[ican]->SetFillColor(10);
 	ccan[ican]->cd(); ccan[ican]->Divide(1,1,0.0001,0.0001);
		ccan[ican]->cd(1);
		gPad->SetLogz(1);
		hmtdstripsbl->Draw("colz");
		for (int i=0;i<30;i++){
			//sprintf(buf,"%d",ActiveRun14[i]);
			sprintf(buf,"%d",i+1);
			hmtdstripsbl->GetXaxis()->SetBinLabel(i+1,buf);
		}	
		gPad->Update();
	ccan[ican]->cd(); ccan[ican]->Update();
	ccan[ican]->Print("noise.ps");
	gStyle->SetPadTopMargin(0.05);

//FUCK
	gStyle->SetTitleFontSize(0.1);
 	++ican; sprintf(buf,"ccan%d",ican);
 	ccan[ican] = new TCanvas(buf,buf,0,0,800.,800.*(7.5/10.));
 	ccan[ican]->SetFillColor(10);
 	ccan[ican]->cd(); ccan[ican]->Divide(5,6,0.0001,0.0001);
	for (int i=0;i<30;i++){
		ccan[ican]->cd(1+i);
		if (hmtdcell_bl[i]->GetEntries()>10){
			hmtdcell_bl[i]->SetFillColor(5);
			hmtdcell_bl[i]->SetFillStyle(1001);
			hmtdcell_bl[i]->SetMinimum(0.5);			
			hmtdcell_bl[i]->Draw();			
			//gPad->SetLogy(1);
		}
	}
	ccan[ican]->cd(); ccan[ican]->Update();
	ccan[ican]->Print("noise.ps");
	gStyle->SetTitleFontSize(0.02);

	TLatex *mtdplotlabels[5][30];
	for (int ibl=0;ibl<30;ibl++){
		if (ActiveRun14[ibl]>0){
			for (int i=0;i<5;i++){
				sprintf(buf,"BL%d-%d",ibl+1,i+1);
				mtdplotlabels[i][ibl] = new TLatex(0.15,0.17,buf);
				mtdplotlabels[i][ibl]->SetTextSize(0.5);
				mtdplotlabels[i][ibl]->SetNDC();				
			}
		}
	}

	int kactive = 0;
		
	kactive = 0;
	gStyle->SetPadLeftMargin(0.12);
 	++ican; sprintf(buf,"ccan%d",ican);
 	ccan[ican] = new TCanvas(buf,buf,0,0,800.,800.*(7.5/10.));
 	ccan[ican]->SetFillColor(10);
 	ccan[ican]->cd(); ccan[ican]->Divide(5,30,0.0001,0.0001);
	for (int ibl=0;ibl<30;ibl++){
		if (ActiveRun14[ibl]>0){
			for (int i=0;i<5;i++){
				ccan[ican]->cd(5*kactive+1+i);
				gPad->SetLogy(1);
				hmtdcell[i][ibl]->SetMinimum(  0.5);
				//hmtdcell[i][ibl]->SetMaximum(600.);
				hmtdcell[i][ibl]->SetFillColor(5);
				hmtdcell[i][ibl]->SetFillStyle(1001);
				hmtdcell[i][ibl]->Draw();
				hmtdcell[i][ibl]->GetYaxis()->SetLabelSize(0.08);
				mtdplotlabels[i][ibl]->Draw("same");
				gPad->Update();
			}
			++kactive;
		}
	}
	ccan[ican]->cd(); ccan[ican]->Update();
	ccan[ican]->Print("noise.ps");
	gStyle->SetPadLeftMargin(0.09);	
	//
	kactive = 0;
	gStyle->SetPadLeftMargin(0.12);
 	++ican; sprintf(buf,"ccan%d",ican);
 	ccan[ican] = new TCanvas(buf,buf,0,0,800.,800.*(7.5/10.));
 	ccan[ican]->SetFillColor(10);
 	ccan[ican]->cd(); ccan[ican]->Divide(5,30,0.0001,0.0001);
	for (int ibl=0;ibl<30;ibl++){
		if (ActiveRun14[ibl]>0){
			for (int i=0;i<5;i++){
				ccan[ican]->cd(5*kactive+1+i);
				gPad->SetLogy(1);
				hmtdnsignalsevent[i][ibl]->Draw();
				mtdplotlabels[i][ibl]->SetX(0.55);
				mtdplotlabels[i][ibl]->SetY(0.64);
				mtdplotlabels[i][ibl]->Draw("same");
				gPad->Update();
			}
			++kactive;
		}
	}
	ccan[ican]->cd(); ccan[ican]->Update();
	ccan[ican]->Print("noise.ps");
	gStyle->SetPadLeftMargin(0.09);	
	//
	kactive = 0;
	gStyle->SetPadLeftMargin(0.12);
 	++ican; sprintf(buf,"ccan%d",ican);
 	ccan[ican] = new TCanvas(buf,buf,0,0,800.,800.*(7.5/10.));
 	ccan[ican]->SetFillColor(10);
 	ccan[ican]->cd(); ccan[ican]->Divide(5,30,0.0001,0.0001);
	for (int ibl=0;ibl<30;ibl++){
		if (ActiveRun14[ibl]>0){
			for (int i=0;i<5;i++){
				ccan[ican]->cd(5*kactive+1+i);
				gPad->SetLogz(1);
				hmtdstripcorr[i][ibl]->SetMinimum(0.5);
				hmtdstripcorr[i][ibl]->Draw("col");
				mtdplotlabels[i][ibl]->SetX(0.15);
				mtdplotlabels[i][ibl]->SetY(0.64);
				mtdplotlabels[i][ibl]->Draw("same");
				gPad->Update();
			}
			++kactive;
		}
	}
	ccan[ican]->cd(); ccan[ican]->Update();
	ccan[ican]->Print("noise.ps");
	gStyle->SetPadLeftMargin(0.09);	

	gStyle->SetTitleFontSize(0.1);
 	++ican; sprintf(buf,"ccan%d",ican);
 	ccan[ican] = new TCanvas(buf,buf,0,0,800.,800.*(7.5/10.));
 	ccan[ican]->SetFillColor(10);
 	ccan[ican]->cd(); ccan[ican]->Divide(5,6,0.0001,0.0001);
	for (int ibl=0;ibl<30;ibl++){
		ccan[ican]->cd(ibl+1);
			htot_mtd[ibl]->SetTitleSize(0.2,"t");
			htot_mtd[ibl]->SetFillColor(5);
			htot_mtd[ibl]->SetFillStyle(1001);
			htot_mtd[ibl]->Draw();
	}
	ccan[ican]->cd(); ccan[ican]->Update();
	ccan[ican]->Print("noise.ps");
	gStyle->SetTitleFontSize(0.02);

//  	++ican; sprintf(buf,"ccan%d",ican);
//  	ccan[ican] = new TCanvas(buf,buf,0,0,800.,800.*(7.5/10.));
//  	ccan[ican]->SetFillColor(10);
//  	ccan[ican]->cd(); ccan[ican]->Divide(2,2,0.0001,0.0001);
// 		ccan[ican]->cd(1);
// 			htot_mtd26->Draw();
// 		ccan[ican]->cd(2);
// 			htot_mtd27->Draw();
// 		ccan[ican]->cd(3);
// 			htot_mtd28->Draw();
// 	ccan[ican]->cd(); ccan[ican]->Update();
// 	ccan[ican]->Print("noise.ps");

// 	cout<<"........................................."<<endl;
// 	if (hnhitstof_time){
// 		cout<<"hnhitstof_time max bin .... "<<hnhitstof_time->GetMaximumBin()<<endl;
// 	}
// 	cout<<"........................................."<<endl;
// 	if (hnhitstof_time && hnhitsmtd26_time){
// 		if (hnhitstof_time->GetMaximumBin()>1){
// 			++ican; sprintf(buf,"ccan%d",ican);
// 			ccan[ican] = new TCanvas(buf,buf,0,0,800.,800.*(7.5/10.));
// 			ccan[ican]->SetFillColor(10);
// 			ccan[ican]->cd(); ccan[ican]->Divide(2,2,0.0001,0.0001);
// 				ccan[ican]->cd(1);
// 					hnhitstof_time->SetLineColor(3);
// 					hnhitstof_time->Draw();
// 				ccan[ican]->cd(2);
// 					hnhitsmtd26_time->SetLineColor(6);
// 					hnhitsmtd26_time->Draw();
// 				ccan[ican]->cd(3);
// 					hnhitsmtd27_time->SetLineColor(4);
// 					hnhitsmtd27_time->Draw();
// 				ccan[ican]->cd(4);
// 					hnhitsmtd28_time->SetLineColor(7);
// 					hnhitsmtd28_time->Draw();
// 			ccan[ican]->cd(); ccan[ican]->Update();
// 			ccan[ican]->Print("noise.ps");
// 			//
// 			++ican; sprintf(buf,"ccan%d",ican);
// 			ccan[ican] = new TCanvas(buf,buf,0,0,800.,800.*(7.5/10.));
// 			ccan[ican]->SetFillColor(10);
// 			ccan[ican]->cd(); ccan[ican]->Divide(1,1,0.0001,0.0001);
// 				ccan[ican]->cd(1);
// 					hnhitsmtd28_time->SetLineColor(7);
// 					hnhitsmtd28_time->SetMinimum(0.5);
// 					hnhitsmtd28_time->Draw();
// 					hnhitstof_time->SetLineColor(3);
// 					hnhitstof_time->Draw("same");
// 					hnhitsmtd26_time->SetLineColor(6);
// 					hnhitsmtd26_time->Draw("same");
// 					hnhitsmtd27_time->SetLineColor(4);
// 					hnhitsmtd27_time->Draw("same");
// 			ccan[ican]->cd(); ccan[ican]->Update();
// 			ccan[ican]->Print("noise.ps");
// 			//	
// 		}
// 	}

	kactive	= 0;
	gStyle->SetPadLeftMargin(0.12);
 	++ican; sprintf(buf,"ccan%d",ican);
 	ccan[ican] = new TCanvas(buf,buf,0,0,800.,800.*(7.5/10.));
 	ccan[ican]->SetFillColor(10);
 	ccan[ican]->cd(); ccan[ican]->Divide(5,30,0.0001,0.0001);
	for (int ibl=0;ibl<30;ibl++){
		if (ActiveRun14[ibl]>0){
			for (int i=0;i<5;i++){
				ccan[ican]->cd(5*kactive+1+i);
				gPad->SetLogz(1);
				hmtdhitz_strip[i][ibl]->Draw("col");
				hmtdhitz_strip[i][ibl]->GetYaxis()->SetLabelSize(0.08);
				mtdplotlabels[i][ibl]->Draw("same");
				gPad->Update();
			}
			++kactive;
		}
	}
	ccan[ican]->cd(); ccan[ican]->Update();
	ccan[ican]->Print("noise.ps");
	gStyle->SetPadLeftMargin(0.09);	

	//
	//---- find TDIG offsets in hitZ plots...
	//
	TH1D *hZstrip[12];
	TH1D *hZstrips[12][5][30];
	for (int is=0;is<12;is++){
		//
//		sprintf(buf,"hZstrip%d",is);
//		hZstrip[is]	= (TH1D*)hmtdhitz_strip[2][27]->ProjectionY(buf,is+1,is+1);
//		sprintf(buf,"strip=%d",is);
//		hZstrip[is]->SetTitle(buf);
		//
		sprintf(buf,"hZstrip%d",is);
		hZstrip[is]	= new TH1D(buf,buf,560,-7,7);
 		for (int ib=0;ib<30;ib++){
 			for (int ip=0;ip<5;ip++){
				sprintf(buf,"hZstrips%d_%d_%d",is,ip,ib);
				hZstrips[is][ip][ib]	= new TH1D(buf,buf,560,-7,7);
 				for (int iby=1;iby<=560;iby++){
 					float zh = hmtdhitz_strip[ip][ib]->GetYaxis()->GetBinCenter(iby);
 					float nh = hmtdhitz_strip[ip][ib]->GetBinContent(is+1,iby);
 					hZstrip[is]->Fill(zh,nh);
 					hZstrips[is][ip][ib]->Fill(zh,nh);
 				}
 			}
 		}
	}
// 	float meanzcent = 0;
// 	float zoffsets[12] = {0};
// 	float meanzmean = 0;
// 	float zmeans[12] = {0};
//  	++ican; sprintf(buf,"ccan%d",ican);
//  	ccan[ican] = new TCanvas(buf,buf,0,0,800.,800.*(7.5/10.));
//  	ccan[ican]->SetFillColor(10);
//  	ccan[ican]->cd(); ccan[ican]->Divide(4,3,0.0001,0.0001);
// 	for (int i=0;i<12;i++){
// 		ccan[ican]->cd(1+i);
// 		int nb	= hZstrip[i]->GetXaxis()->GetNbins();
// 		double aintegral = hZstrip[i]->Integral(1,nb);
// 		if (aintegral>0.){
// 			hZstrip[i]->Scale(1./aintegral);
// 			float amean = hZstrip[i]->GetMean();
// 			float zmin=-99;
// 			float zmax=-99;
// 			for (int ib=1;ib<=nb;ib++){
// 				float nz = hZstrip[i]->GetBinContent(ib);
// 				float  z = hZstrip[i]->GetBinCenter(ib);
// 				if (zmin<-98. && nz>0.01){ zmin = z; }
// 			}
// 			for (int ib=nb;ib>=1;ib--){
// 				float nz = hZstrip[i]->GetBinContent(ib);
// 				float  z = hZstrip[i]->GetBinCenter(ib);
// 				if (zmax<-98. && nz>0.01){ zmax = z; }
// 			}
// 			float zcent	 = (zmax - zmin)/2.;
// 			meanzcent	+= zcent/12;
// 			float zoff   = zcent - 4.90625;
// 			zoffsets[i]	 = zoff;
// 			meanzmean	+= amean/12;
// 			zmeans[i]	 = amean;
// 			cout<<"strip="<<i<<" "<<nb<<"\t"<<aintegral<<"\t"<<zmin<<" "<<zmax<<"\t"<<zcent<<" "<<zoff<<"\t"<<amean<<endl;
// 			//
// 			gPad->SetLogy(1);
// 			hZstrip[i]->Draw();
// 			//
// 		}
// 	} 
// 	ccan[ican]->cd(); ccan[ican]->Update();
// 	ccan[ican]->Print("noise.ps");
// 	gStyle->SetPadLeftMargin(0.09);	
// 	cout<<"........................................."<<endl;
// 	cout<<" meanzcent = "<<meanzcent<<endl;
// 	cout<<" float zoffsets[12] = {";
// 	for (int i=0;i<12;i++){
// 		if (i<11){ cout<<zoffsets[i]<<",";  }
// 		    else { cout<<zoffsets[i]<<"};"; }
// 	}
// 	cout<<endl;	
// 	cout<<"........................................."<<endl;
// 	cout<<" meanzmean = "<<meanzmean<<endl;
// 	cout<<"float zmeansuse[12] = {";
// 	for (int i=0;i<12;i++){
// 		if (i<11){ cout<<zmeans[i]<<",";  }
// 		    else { cout<<zmeans[i]<<"};"; }
// 	}
// 	cout<<endl;	
// 	cout<<"........................................."<<endl;

// 	for (int ibl=0;ibl<30;ibl++){
// 		if (hmtdhitz_strip_zoff[0][ibl]->GetEntries()>100){
// 			gStyle->SetPadLeftMargin(0.12);
// 			++ican; sprintf(buf,"ccan%d",ican);
// 			ccan[ican] = new TCanvas(buf,buf,0,0,800.,800.*(7.5/10.));
// 			ccan[ican]->SetFillColor(10);
// 			ccan[ican]->cd(); ccan[ican]->Divide(3,2,0.0001,0.0001);
// 				for (int i=0;i<5;i++){
// 					ccan[ican]->cd(1+i);
// 					gPad->SetLogz(1);
// 					hmtdhitz_strip_zoff[i][ibl]->Draw("col");
// 					hmtdhitz_strip_zoff[i][ibl]->GetYaxis()->SetLabelSize(0.08);
// 					gPad->Update();
// 				}
// 			ccan[ican]->cd(); ccan[ican]->Update();
// 			ccan[ican]->Print("noise.ps");
// 			gStyle->SetPadLeftMargin(0.09);	
// 		}
// 	}

	gStyle->SetPadLeftMargin(0.12);
 	++ican; sprintf(buf,"ccan%d",ican);
 	ccan[ican] = new TCanvas(buf,buf,0,0,800.,800.*(7.5/10.));
 	ccan[ican]->SetFillColor(10);
 	ccan[ican]->cd(); ccan[ican]->Divide(3,2,0.0001,0.0001);
		ccan[ican]->cd(1);
			gPad->SetLogz(1);
			hmtdhitz_tota_strip1->Draw("col");
		ccan[ican]->cd(2);
			gPad->SetLogz(1);
			hmtdhitz_tota_strip6->Draw("col");
		ccan[ican]->cd(3);
			gPad->SetLogz(1);
			hmtdhitz_tota_strip12->Draw("col");
		ccan[ican]->cd(4);
			gPad->SetLogz(1);
			hmtdhitz_totb_strip1->Draw("col");
		ccan[ican]->cd(5);
			gPad->SetLogz(1);
			hmtdhitz_totb_strip6->Draw("col");
		ccan[ican]->cd(6);
			gPad->SetLogz(1);
			hmtdhitz_totb_strip12->Draw("col");
	ccan[ican]->cd(); ccan[ican]->Update();
	ccan[ican]->Print("noise.ps");
	gStyle->SetPadLeftMargin(0.09);	
	//
	gStyle->SetPadLeftMargin(0.12);
 	++ican; sprintf(buf,"ccan%d",ican);
 	ccan[ican] = new TCanvas(buf,buf,0,0,800.,800.*(7.5/10.));
 	ccan[ican]->SetFillColor(10);
 	ccan[ican]->cd(); ccan[ican]->Divide(3,2,0.0001,0.0001);
		ccan[ican]->cd(1);
			gPad->SetLogz(1);
			htotm_strip1->Draw();
			htota_strip1->Draw("same");
			htotb_strip1->Draw("same");
		ccan[ican]->cd(2);
			gPad->SetLogz(1);
			htotm_strip6->Draw();
			htota_strip6->Draw("same");
			htotb_strip6->Draw("same");
		ccan[ican]->cd(3);
			gPad->SetLogz(1);
			htotm_strip12->Draw();
			htota_strip12->Draw("same");
			htotb_strip12->Draw("same");
		ccan[ican]->cd(4);
			gPad->SetLogz(1);
			hmtdhitz_totm_strip1->Draw("col");
		ccan[ican]->cd(5);
			gPad->SetLogz(1);
			hmtdhitz_totm_strip6->Draw("col");
		ccan[ican]->cd(6);
			gPad->SetLogz(1);
			hmtdhitz_totm_strip12->Draw("col");
	ccan[ican]->cd(); ccan[ican]->Update();
	ccan[ican]->Print("noise.ps");
	gStyle->SetPadLeftMargin(0.09);	
	//
	int nbx	= hmtdhitz_totm_strip1->GetNbinsX();
	int nby	= hmtdhitz_totm_strip1->GetNbinsY();
	for (int ibx=1;ibx<=nbx;ibx++){
		TH1D *htemp1	= (TH1D*)hmtdhitz_totm_strip1->ProjectionY("htemp1",ibx,ibx);
		TH1D *htemp6	= (TH1D*)hmtdhitz_totm_strip6->ProjectionY("htemp6",ibx,ibx);
		TH1D *htemp12	= (TH1D*)hmtdhitz_totm_strip12->ProjectionY("htemp12",ibx,ibx);
		float aint1		= htemp1->Integral();
		float aint6		= htemp6->Integral();
		float aint12	= htemp12->Integral();
		//cout<<aint1<<" "<<aint6<<" "<<aint12<<endl;
		float az		= 0;
		for (int iby=1;iby<=nby;iby++){
			if (aint1){ 
				az = hmtdhitz_totm_strip1->GetBinContent(ibx,iby);
				hmtdhitz_totm_strip1_norm->SetBinContent(ibx,iby,az/aint1); 
				//if (ibx==17) cout<<ibx<<" "<<iby<<" "<<az<<" "<<aint1<<" "<<az/aint1<<endl;
			} else {
				hmtdhitz_totm_strip1_norm->SetBinContent(ibx,iby,0); 
			}
			if (aint6){ 
				az = hmtdhitz_totm_strip6->GetBinContent(ibx,iby);
				hmtdhitz_totm_strip6_norm->SetBinContent(ibx,iby,az/aint6); 
			} else {
				hmtdhitz_totm_strip6_norm->SetBinContent(ibx,iby,0); 
			}
			if (aint12){ 
				az = hmtdhitz_totm_strip12->GetBinContent(ibx,iby);
				hmtdhitz_totm_strip12_norm->SetBinContent(ibx,iby,az/aint12); 
			} else {
				hmtdhitz_totm_strip12_norm->SetBinContent(ibx,iby,0); 
			}
		}	
	}
	gStyle->SetPadLeftMargin(0.12);
 	++ican; sprintf(buf,"ccan%d",ican);
 	ccan[ican] = new TCanvas(buf,buf,0,0,800.,800.*(7.5/10.));
 	ccan[ican]->SetFillColor(10);
 	ccan[ican]->cd(); ccan[ican]->Divide(3,2,0.0001,0.0001);
		ccan[ican]->cd(1);
			gPad->SetLogz(1);
			htotm_strip1->Draw();
			htota_strip1->Draw("same");
			htotb_strip1->Draw("same");
		ccan[ican]->cd(2);
			gPad->SetLogz(1);
			htotm_strip6->Draw();
			htota_strip6->Draw("same");
			htotb_strip6->Draw("same");
		ccan[ican]->cd(3);
			gPad->SetLogz(1);
			htotm_strip12->Draw();
			htota_strip12->Draw("same");
			htotb_strip12->Draw("same");
		ccan[ican]->cd(4);
			gPad->SetLogz(1);
			hmtdhitz_totm_strip1_norm->Draw("col");
		ccan[ican]->cd(5);
			gPad->SetLogz(1);
			hmtdhitz_totm_strip6_norm->Draw("col");
		ccan[ican]->cd(6);
			gPad->SetLogz(1);
			hmtdhitz_totm_strip12_norm->Draw("col");
	ccan[ican]->cd(); ccan[ican]->Update();
	ccan[ican]->Print("noise.ps");
	gStyle->SetPadLeftMargin(0.09);	
	//
	gStyle->SetPadLeftMargin(0.12);
 	++ican; sprintf(buf,"ccan%d",ican);
 	ccan[ican] = new TCanvas(buf,buf,0,0,800.,800.*(7.5/10.));
 	ccan[ican]->SetFillColor(10);
 	ccan[ican]->cd(); ccan[ican]->Divide(1,1,0.0001,0.0001);
		ccan[ican]->cd(1);
			gPad->SetLogz(1);
			htotm_strip->Draw("colz");
	ccan[ican]->cd(); ccan[ican]->Update();
	ccan[ican]->Print("noise.ps");
	gStyle->SetPadLeftMargin(0.09);	


	float meantot_tof	= htot->GetMean();
	float meantot_mtd[30];
	hinfo->Fill(22.0,meantot_tof);
	for (int i=0;i<30;i++){
		meantot_mtd[i]	= htot_mtd[i]->GetMean();
		hinfo->Fill(60.0+i,meantot_mtd[i]);
	}
	//
	cout<<" <ToT> TOF   = "<<meantot_tof<<endl;
	cout<<" <ToT> MTD01 = "<<meantot_mtd[0]<<endl;
	cout<<" <ToT> MTD06 = "<<meantot_mtd[5]<<endl;
	cout<<" <ToT> MTD10 = "<<meantot_mtd[9]<<endl;
	cout<<" <ToT> MTD22 = "<<meantot_mtd[21]<<endl;
	cout<<" <ToT> MTD25 = "<<meantot_mtd[24]<<endl;
	cout<<" <ToT> MTD28 = "<<meantot_mtd[27]<<endl;

//  	++ican; sprintf(buf,"ccan%d",ican);
//  	ccan[ican] = new TCanvas(buf,buf,0,0,800.,800.*(7.5/10.));
//  	ccan[ican]->SetFillColor(10);
//  	ccan[ican]->cd(); ccan[ican]->Divide(1,1,0.0001,0.0001);
// 		ccan[ican]->cd(1);
// 			gPad->SetLogz(1);
// 			hcellmod->Draw("col");			
// 	ccan[ican]->cd(); ccan[ican]->Update();
// 	ccan[ican]->Print("noise.ps");


	kactive = 0;
	gStyle->SetPadLeftMargin(0.12);
 	++ican; sprintf(buf,"ccan%d",ican);
 	ccan[ican] = new TCanvas(buf,buf,0,0,800.,800.*(7.5/10.));
 	ccan[ican]->SetFillColor(10);
 	ccan[ican]->cd(); ccan[ican]->Divide(5,30,0.0001,0.0001);
	for (int ibl=0;ibl<30;ibl++){
		if (ActiveRun14[ibl]>0){
			for (int i=0;i<5;i++){
				ccan[ican]->cd(5*kactive+1+i);
				//gPad->SetLogy(1);
				hmtdstriprat[i][ibl]->SetMinimum(0.0);
				hmtdstriprat[i][ibl]->SetMaximum(1.1);
				hmtdstriprat[i][ibl]->SetFillColor(3);
				hmtdstriprat[i][ibl]->SetFillStyle(1001);
				hmtdstriprat[i][ibl]->Draw();
				hmtdstriprat[i][ibl]->GetYaxis()->SetLabelSize(0.08);
				mtdplotlabels[i][ibl]->Draw("same");
				gPad->Update();
			}
			++kactive;
		}
	}
	ccan[ican]->cd(); ccan[ican]->Update();
	ccan[ican]->Print("noise.ps");
	gStyle->SetPadLeftMargin(0.09);	


	//----- end of all processing......
	ccan[ican]->Print("noise.ps]");
	fout->Write();
	sprintf(buf,"/bin/mv %s.ps %s.ps","noise",namebase.Data());
	cout<<"..... noise::loop Executing ... "<<buf<<endl;
	gSystem->Exec(buf);
	sprintf(buf,"/usr/bin/ps2pdf %s.ps %s.pdf",namebase.Data(),namebase.Data());
	cout<<"..... noise::loop Executing ... "<<buf<<endl;
	gSystem->Exec(buf);
	sprintf(buf,"/bin/cp %s.pdf /star/u/geurts/WWW/files/",namebase.Data());
	cout<<"..... noise::loop Executing ... "<<buf<<endl;
	gSystem->Exec(buf);
	cout<<"..... noise::loop Done...."<<endl;	

}
//---- end noise::loop
