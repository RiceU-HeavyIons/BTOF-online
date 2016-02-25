#include <iostream>
#include <iomanip>
#include "TError.h"
using namespace std;

void thub14() {

char type[20];

//sprintf(type,"15048021_tof");
//sprintf(type,"15049031_tof");
//sprintf(type,"15073033_tof");

//sprintf(type,"15086055_tof");
//sprintf(type,"15157038_tof");
//sprintf(type,"15169062_tof");
//sprintf(type,"15169_tof");
//sprintf(type,"15171007_tof");
sprintf(type,"16046014_tof");

gErrorIgnoreLevel = 4000;
char 		buf[100];
char 		tbuf[100];
int			ican = -1;
TCanvas*	ccan[100];

gStyle->SetPalette(1);
gStyle->SetOptStat(0);
gStyle->SetPadTopMargin(0.07);
gStyle->SetPadRightMargin(0.03);
gStyle->SetPadLeftMargin(0.08);
gStyle->SetPadBottomMargin(0.08);

char psfileopen[100];
char psfile[100];
char pdffile[100];
char psfileclose[100];
sprintf(psfileopen,"thub_%s.ps(",type);
sprintf(psfile,"thub_%s.ps",type);
sprintf(pdffile,"thub_%s.pdf",type);
sprintf(psfileclose,"thub_%s.ps]",type);
cout<<type<<endl;
cout<<psfileopen<<endl;
cout<<psfile<<endl;
cout<<psfileclose<<endl;
cout<<pdffile<<endl;

	//run-14 auau 15
// 	const char* triggernames[64] = {
// 		"",						// 1	1
// 		"bbc_mon_tof",						// 2	2
// 		"hlt-good-vpd",						// 4	3
// 		"",						// 8	4
// 		"hlt-bes-mon",			// 10	5
// 		"tofmult4",			// 20	6
// 		"",			// 40	7
// 		"",					// 80	8
// 		"",						// 100	9	
// 		"",				// 200	10	
// 		"",				// 400	11	
// 		"",						// 800	12	
// 		"",						// 1000	13	
// 		"",						// 2000	14	
// 		"",		// 4000	15	
// 		"",				// 8000	16	
// 		"BBC_mb",					// 10000	17
// 		"VPD_mb",			// 20000	18
// 		"ZDC-mb",			// 40000	19
// 		"",					// 80000	20
// 		"",			// 100000	21
// 		"Zero-bias",			// 200000	22
// 		"",			// 400000	23
// 		"",			// 800000	24
// 		"",			// 1000000	25
// 		"",			// 2000000	26
// 		"",			// 4000000	27
// 		"",			// 8000000	28
// 		"",					// 10000000	29
// 		"",			// 20000000	
// 		"",					// 40000000	
// 		"",			// 80000000	
// 		"",				// 100000000	
// 		"",			// 200000000	
// 		"",			// 400000000	
// 		"",			// 800000000	
// 		"",				// 1000000000	
// 		"",				// 2000000000	
// 		"",			// 4000000000	
// 		"",		// 8000000000	
// 		"",					// 10000000000	
// 		"",					// 20000000000	
// 		"",			// 40000000000	
// 		"",			// 80000000000	
// 		"",			// 100000000000	
// 		"",			// 200000000000	
// 		"",			// 400000000000	
// 		"",			// 800000000000	
// 		"",			// 1000000000000	
// 		"",			// 2000000000000	
// 		"",			// 4000000000000	
// 		"",			// 8000000000000	
// 		"",			// 10000000000000	
// 		"",			// 20000000000000	
// 		"",			// 40000000000000	
// 		"",			// 80000000000000	
// 		"",			// 100000000000000	
// 		"",			// 200000000000000	
// 		"",			// 400000000000000	
// 		"",			// 800000000000000	
// 		"",			// 1000000000000000	
// 		"",			// 2000000000000000	
// 		"",			// 4000000000000000	
// 		""			// 8000000000000000	
// 		};	

	//run-14 auau 200
// 	const char* triggernames[64] = {
// 		"singlemuon",						// 1	1
// 		"dimuon",						// 2	2
// 		"e-mu",						// 4	3
// 		"vpf-hft-hlt",						// 8	4
// 		"",			// 10	5
// 		"",			// 20	6
// 		"",			// 40	7
// 		"VPDMB-5-p-nobsmd",					// 80	8
// 		"VPDMB-5-p-nobsmd-hltheavyfrag",						// 100	9	
// 		"VPDMB-5-p-nobsmd-hltDiElectron",				// 200	10	
// 		"VPDMB-5",				// 400	11	
// 		"VPDMB-5-ssd",						// 800	12	
// 		"VPDMB-30",						// 1000	13	
// 		"MB-mon",						// 2000	14	
// 		"Zdc-mon",		// 4000	15	
// 		"VPD-ZDC-novtx-mon",				// 8000	16	
// 		"",					// 10000	17
// 		"",			// 20000	18
// 		"Central-5",			// 40000	19
// 		"Central-mon",					// 80000	20
// 		"Central-5-hltheavyfrag",			// 100000	21
// 		"Zero-bias",			// 200000	22
// 		"BHT2*VPDMB-30",			// 400000	23
// 		"BHT3",			// 800000	24
// 		"BHT1*VPDMB-30",			// 1000000	25
// 		"BHT3-L2Gamma",			// 2000000	26
// 		"",			// 4000000	27
// 		"EHT",			// 8000000	28
// 		"UPC-main",					// 10000000	29
// 		"UPC-topo",			// 20000000	30
// 		"UPC-highG",					// 40000000	31
// 		"",			// 80000000	32
// 		"UPC-jpsi-B",				// 100000000 33
// 		"",			// 200000000 34
// 		"Zero-bias",			// 400000000 35
// 		"",			// 800000000 36
// 		"VPDMB-5-nobsmd",				// 1000000000 37
// 		"VPDMB-5-p-nobsmd-hlt",				// 2000000000 38
// 		"ZDC_fast_prepost",			// 4000000000 39
// 		"dimuon-30-hft",		// 8000000000 40
// 		"",					// 10000000000 41
// 		"",					// 20000000000 42
// 		"",			// 40000000000 43
// 		"",			// 80000000000 44
// 		"",			// 100000000000	45
// 		"",			// 200000000000	46
// 		"",			// 400000000000	47
// 		"",			// 800000000000	48
// 		"",			// 1000000000000 49
// 		"",			// 2000000000000 50
// 		"",			// 4000000000000 51
// 		"",			// 8000000000000 52
// 		"",			// 10000000000000 53
// 		"",			// 20000000000000 54
// 		"",			// 40000000000000 55
// 		"",			// 80000000000000 56
// 		"",			// 100000000000000 57
// 		"",			// 200000000000000 58
// 		"",			// 400000000000000 59
// 		"",			// 800000000000000 60
// 		"",			// 1000000000000000	61
// 		"",			// 2000000000000000	62
// 		"future-guardian",			// 4000000000000000	63
// 		""			// 8000000000000000	64
// 		};	

//---- run14 3he+au 200
	const char* triggernames[64] = {
		"singlemuon",						// 1	1
		"dimuon",						// 2	2
		"e-mu",						// 4	3
		"mtd-cosmic1",						// 8	4
		"mtd-cosmic2",			// 10	5
		"mtd-cosmic3",			// 20	6
		"ZDCE",			// 40	7
		"VPD",					// 80	8
		"BBC",						// 100	9	
		"",				// 200	10	
		"Central",				// 400	11	
		"BHT3-BBC-L2Gamma",						// 800	12	
		"BHT2*BBC",						// 1000	13	
		"BHT3*BBC",						// 2000	14	
		"BHT1*BBC",		// 4000	15	
		"",				// 8000	16	
		"ZDC_coinc",					// 10000	17
		"VPD-ZDC-mon",			// 20000	18
		"BBC-ZDC-mon",			// 40000	19
		"ZDCE*TofMult4",					// 80000	20
		"",			// 100000	21
		"fhc_ht",			// 200000	22
		"fhc_ht_bht0",			// 400000	23
		"fhc_ht_eht0",			// 800000	24
		"",			// 1000000	25
		"",			// 2000000	26
		"",			// 4000000	27
		"",			// 8000000	28
		"",					// 10000000	29
		"",			// 20000000	
		"",					// 40000000	
		"",			// 80000000	
		"",				// 100000000	
		"",			// 200000000	
		"",			// 400000000	
		"",			// 800000000	
		"",				// 1000000000	
		"",				// 2000000000	
		"",			// 4000000000	
		"",		// 8000000000	
		"",					// 10000000000	
		"",					// 20000000000	
		"",			// 40000000000	
		"",			// 80000000000	
		"",			// 100000000000	
		"",			// 200000000000	
		"",			// 400000000000	
		"",			// 800000000000	
		"",			// 1000000000000	
		"",			// 2000000000000	
		"",			// 4000000000000	
		"",			// 8000000000000	
		"",			// 10000000000000	
		"",			// 20000000000000	
		"",			// 40000000000000	
		"",			// 80000000000000	
		"",			// 100000000000000	
		"",			// 200000000000000	
		"",			// 400000000000000	
		"Zero-bias",			// 800000000000000	
		"",			// 1000000000000000	
		"",			// 2000000000000000	
		"",			// 4000000000000000	
		""			// 8000000000000000	
		};	
	
	TLatex *ttrgnames[64];
	for (int it=0;it<64;it++){
		//unsigned int ival = pow(2,it);
//		cout<<setbase(10)<<it<<" "
//			//<<setprecision(12)<<ival<<" "
//	    	//<<setbase(16)<<ival<<" "<<setbase(10)
//	    	<<triggernames[it]<<endl;
		sprintf(buf,triggernames[it]);
		ttrgnames[it] = new TLatex(it,0.6,buf);
		ttrgnames[it]->SetTextAngle(90);
		ttrgnames[it]->SetTextSize(0.02);
		ttrgnames[it]->SetTextAlign(12);
	}
	TLatex *tdummy = new TLatex(0.01,0.01,Form("%s",type));
		tdummy->SetNDC();
		tdummy->SetTextAngle(0);
		tdummy->SetTextSize(0.03);
		tdummy->SetTextAlign(12);

TLatex *thistext = new TLatex();
	thistext->SetTextSize(0.08);
	thistext->SetNDC();

TH1::AddDirectory(kFALSE);

sprintf(buf,"/star/institutions/rice/geurts/dd/daqdoer_%s.root",type);
TFile *f = new TFile(buf,"READ");
cout<<buf<<endl;

	f->cd();
	TH1D *htrgdtime_traycorrected;
	sprintf(buf,"htrgdtime_traycorrected");
	htrgdtime_traycorrected	= (TH1D*)f->Get(buf);
	TH2D *htrgdtime_traycorrected_tray;
	sprintf(buf,"htrgdtime_traycorrected_tray");
	htrgdtime_traycorrected_tray	= (TH2D*)f->Get(buf);

	TH1D *hcrossing_traycorrected;
	sprintf(buf,"hcrossing_traycorrected");
	hcrossing_traycorrected	= (TH1D*)f->Get(buf);
	TH1D *hcrossingkeep_traycorrected;
	sprintf(buf,"hcrossingkeep_traycorrected");
	hcrossingkeep_traycorrected	= (TH1D*)f->Get(buf);
		hcrossingkeep_traycorrected->SetFillStyle(1001);
		hcrossingkeep_traycorrected->SetFillColor(7);

	cout<<"Doing crossing time interval fits...."<<endl;
	TF1 *g1	= new TF1("g1","gaus",-2894,-2854);		//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//	TF1 *g1	= new TF1("g1","gaus",-2820,-2780);		//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		g1->SetLineWidth(2);
		g1->SetLineColor(2);
//	TF1 *g2	= new TF1("g2","gaus",1900,1960);		//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	TF1 *g2	= new TF1("g2","gaus",1995,2045);		//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		g2->SetLineWidth(2);
		g2->SetLineColor(2);
	htrgdtime_traycorrected->Fit("g1","R");
	htrgdtime_traycorrected->Fit("g2","+R");
	double gpars1[3],gpars2[3];
	g1->GetParameters(gpars1);
	g2->GetParameters(gpars2);
	float crossingperiod	= (gpars2[1]-gpars1[1])/46.;
	cout<<"Crossing Period = "<<crossingperiod<<endl; 
	float crossings[48];
	TLine *lcrossings[48];
	for (int i=0;i<48;i++){
		crossings[i]	= gpars1[1]+ ((float)i-1)*crossingperiod;
		lcrossings[i]	= new TLine(crossings[i],1,crossings[i],1.e5);
		lcrossings[i]->SetLineColor(7);
	}
	TH1D *hntofhits_crossing[48];
	for (int i=0;i<48;i++){
		sprintf(buf,"hntofhits_crossing%d",i);
		hntofhits_crossing[i]	= (TH1D*)f->Get(buf);
	}

	TH1D *htrgdtime_rawall = (TH1D*)f->Get("htrgdtime_rawall");
	TH1D *htrgdtime[6];
	TH1D *htrgdtimekeep[6];
	TH1D *htrgdtimekeep3[6];
	TH1D *htrgdtime0_bytrig[64];
	TH1D *htrgdtime0keep_bytrig[64];
	TH1D *htrgdtime_bytrig[6][64];
	TH1D *htrgdtimekeep_bytrig[6][64];
	TH1D *htrgdtime_traycorrected_bytrig[64];
	for (int i=0;i<6;i++){
		sprintf(buf,"htrgdtime%d",i);
		htrgdtime[i]	= (TH1D*)f->Get(buf);
		sprintf(buf,"htrgdtimekeep%d",i);
		htrgdtimekeep[i]	= (TH1D*)f->Get(buf);
		sprintf(buf,"htrgdtimekeep3%d",i);
		htrgdtimekeep3[i]	= (TH1D*)f->Get(buf);
	}
	TH1D *hnhits_prec_bytrig[64];
	TH1D *hnhits_tric_bytrig[64];
	TH1D *hnhits_posc_bytrig[64];
	for (int i=0;i<64;i++){
		sprintf(buf,"htrgdtime0_bytrig%d",i);
		htrgdtime0_bytrig[i]	= (TH1D*)f->Get(buf);
		sprintf(buf,"htrgdtime0keep_bytrig%d",i);
		htrgdtime0keep_bytrig[i]	= (TH1D*)f->Get(buf);
		for (int ithub=0;ithub<6;ithub++){
			sprintf(buf,"htrgdtime_bytrig%d%d",ithub,i);
			htrgdtime_bytrig[ithub][i]	= (TH1D*)f->Get(buf);
			sprintf(buf,"htrgdtimekeep_bytrig%d%d",ithub,i);
			htrgdtimekeep_bytrig[ithub][i]	= (TH1D*)f->Get(buf);
		}
		sprintf(buf,"htrgdtime_traycorrected_bytrig%d",i);
		htrgdtime_traycorrected_bytrig[i]	= (TH1D*)f->Get(buf);
		sprintf(buf,"hnhits_prec_bytrig%d",i);
		hnhits_prec_bytrig[i]	= (TH1D*)f->Get(buf);
		sprintf(buf,"hnhits_tric_bytrig%d",i);
		hnhits_tric_bytrig[i]	= (TH1D*)f->Get(buf);
		sprintf(buf,"hnhits_posc_bytrig%d",i);
		hnhits_posc_bytrig[i]	= (TH1D*)f->Get(buf);
	}
	TH2D *htrgdtime_trayoff = (TH2D*)f->Get("htrgdtime_trayoff");
	TH2D *hnearly_nintime = (TH2D*)f->Get("hnearly_nintime");
	TH1D *hp_bbc = (TH1D*)f->Get("hp_bbc");
	TH1D *hp_bbc_tofintime = (TH1D*)f->Get("hp_bbc_tofintime");
	TH1D *hp_bbc_tofearly = (TH1D*)f->Get("hp_bbc_tofearly");
	TH1D *hnh_bbcl = (TH1D*)f->Get("hnh_bbcl");
	TH1D *hnh_bbcl_tofintime = (TH1D*)f->Get("hnh_bbcl_tofintime");
	TH1D *hnh_bbcl_tofearly = (TH1D*)f->Get("hnh_bbcl_tofearly");
	TH1D *htrgbits = (TH1D*)f->Get("htrgbits");
	TH1D *htrgbits_tofintime = (TH1D*)f->Get("htrgbits_tofintime");
	TH1D *htrgbits_tofearly = (TH1D*)f->Get("htrgbits_tofearly");

	TH1D *htrgdtime_mtd[2];
	TH1D *htrgdtimekeep_mtd[2];
	for (int i=0;i<2;i++){
		sprintf(buf,"htrgdtime_mtd%d",i);
		htrgdtime_mtd[i] = (TH1D*)f->Get(buf);
		sprintf(buf,"htrgdtimekeep_mtd%d",i);
		htrgdtimekeep_mtd[i] = (TH1D*)f->Get(buf);
	}
	TH1D *htrgdtime_mtd_tray[30];
	TH1D *htrgdtimekeep_mtd_tray[30];
	for (int i=0;i<30;i++){
		sprintf(buf,"htrgdtime_mtd_tray%d",i);
		htrgdtime_mtd_tray[i]	= (TH1D*)f->Get(buf);
		sprintf(buf,"htrgdtimekeep_mtd_tray%d",i);
		htrgdtimekeep_mtd_tray[i]	= (TH1D*)f->Get(buf);
	}
	
	TH1D *hnhitstof_xingall	= (TH1D*)f->Get("hnhitstof_xingall");
	TH1D *hnhitstof_xingtrg	= (TH1D*)f->Get("hnhitstof_xingtrg");
	TH1D *hnhitstof_xingm2	= (TH1D*)f->Get("hnhitstof_xingm2");
	TH1D *hnhitstof_xingp2	= (TH1D*)f->Get("hnhitstof_xingp2");
	TH1D *hnhitstof_xingm7	= (TH1D*)f->Get("hnhitstof_xingm7");
	TH1D *hnhitstof_xingp7	= (TH1D*)f->Get("hnhitstof_xingp7");
	TH1D *hnhitstof_xingtrg_vpd	= (TH1D*)f->Get("hnhitstof_xingtrg_vpd");
	TH1D *hnhitstof_xingm2_vpd	= (TH1D*)f->Get("hnhitstof_xingm2_vpd");
	TH1D *hnhitstof_xingp2_vpd	= (TH1D*)f->Get("hnhitstof_xingp2_vpd");
	TH1D *hnhitstof_xingm7_vpd	= (TH1D*)f->Get("hnhitstof_xingm7_vpd");
	TH1D *hnhitstof_xingp7_vpd	= (TH1D*)f->Get("hnhitstof_xingp7_vpd");

	TH1D *hbunchidoffset_thubhalf[6][2];
	for (int ithub=0;ithub<6;ithub++){
		for (int ih=0;ih<2;ih++){
			sprintf(buf,"hbunchidoffset_thubhalf%d%d",ithub,ih);
			hbunchidoffset_thubhalf[ithub][ih] = (TH1D*)f->Get(buf);
		}
	}
	
	//------------------------------------------------------------------
	
	gStyle->SetPadGridX(kFALSE);
	gStyle->SetPadGridY(kFALSE);
	gStyle->SetPadLeftMargin(0.09);
	gStyle->SetPadRightMargin(0.02);
	gStyle->SetPalette(1);

 	++ican; sprintf(buf,"ccan%d",ican);
 	ccan[ican] = new TCanvas(buf,buf,0,0,750.,750.*(8.5/11.));
 	ccan[ican]->SetFillColor(10);
 	ccan[ican]->cd(); ccan[ican]->Divide(1,1,0.0001,0.0001);
	ccan[ican]->cd(1);
		gPad->SetLogy(1);
		htrgbits->SetMinimum(0.5);
		htrgbits->Draw();
		htrgbits_tofintime->SetLineColor(3);
		htrgbits_tofintime->Draw("same");
		htrgbits_tofearly->SetLineColor(2);
		htrgbits_tofearly->Draw("same");
		for (int it=0;it<64;it++){
			ttrgnames[it]->Draw("same");
		}
		tdummy->Draw("same");
	ccan[ican]->cd(); ccan[ican]->Update();
	ccan[ican]->Print(psfileopen);
	
	//
	float anhitall[6]={0};
	float anhitkeep1[6]={0};
	float anhitrat1[6]={0};
	float anhitkeep3[6]={0};
	float anhitrat3[6]={0};
	TLatex *anhittext1[6];
	TLatex *anhittext3[6];
	//
 	++ican; sprintf(buf,"ccan%d",ican);
 	ccan[ican] = new TCanvas(buf,buf,0,0,750.,750.*(8.5/11.));
 	ccan[ican]->SetFillColor(10);
 	ccan[ican]->cd(); ccan[ican]->Divide(3,2,0.0001,0.0001);
	for (int i=0;i<6;i++){
		ccan[ican]->cd(1+i);
		gPad->SetLogy(1);
		//
		anhitall[i]		= htrgdtime[i]->Integral();
		anhitkeep1[i]	= htrgdtimekeep[i]->Integral();
		anhitkeep3[i]	= htrgdtimekeep3[i]->Integral();
		if (anhitall[i]>0){ 
			anhitrat1[i] = anhitkeep1[i]/anhitall[i]; 
			anhitrat3[i] = anhitkeep3[i]/anhitall[i]; 
		}
		sprintf(buf,"Prob(kept,#pm1)=%4.1f%%",100.*anhitrat3[i]);
		anhittext3[i]	= new TLatex(0.14,0.87,buf);
			anhittext3[i]->SetNDC();
			anhittext3[i]->SetTextSize(0.05);			
			anhittext3[i]->SetTextColor(1);			
		sprintf(buf,"Prob(kept,0)=%4.1f%%",100.*anhitrat1[i]);
		anhittext1[i]	= new TLatex(0.14,0.81,buf);
			anhittext1[i]->SetNDC();
			anhittext1[i]->SetTextSize(0.05);			
			anhittext1[i]->SetTextColor(1);			
		//
		if (i< 4){ 
			htrgdtime[i]->SetTitle(Form("trgdtime, TRAY thub=%d",i)); 
			htrgdtime[i]->GetXaxis()->SetRangeUser(2400.,3200.);
		} else if (i==4){ 
			htrgdtime[i]->SetTitle(Form("trgdtime, VPD 121")); 
			htrgdtime[i]->GetXaxis()->SetRangeUser(-10.,510.);
		} else if (i==5){ 
			htrgdtime[i]->SetTitle(Form("trgdtime, VPD 122")); 
			htrgdtime[i]->GetXaxis()->SetRangeUser(-10.,510.);
		} 
		htrgdtime[i]->Draw();
		htrgdtimekeep3[i]->Draw("same");
		htrgdtimekeep[i]->Draw("same");
		htrgdtime[i]->Draw("same");
		anhittext3[i]->Draw("same");			
		anhittext1[i]->Draw("same");			
	}
	ccan[ican]->cd(); ccan[ican]->Update();
	ccan[ican]->Print(psfile);

	//---- trig dtime by trigger ID for THUB 0
	//
	gStyle->SetPadLeftMargin(0.16);
	thistext->SetTextSize(0.06);
 	++ican; sprintf(buf,"ccan%d",ican);
 	ccan[ican] = new TCanvas(buf,buf,0,0,750.,750.*(8.5/11.));
 	ccan[ican]->SetFillColor(10);
 	ccan[ican]->cd(); ccan[ican]->Divide(8,4,0.0001,0.0001);
	for (int i=0;i<32;i++){
		ccan[ican]->cd(1+i);
		float thisscale = htrgbits->GetBinContent(i+1);
		if (thisscale>0){
			htrgdtime0_bytrig[i]->Scale(1./thisscale);
			htrgdtime0keep_bytrig[i]->Scale(1./thisscale);
			gPad->SetLogy(1);
		} else {
			gPad->SetLogy(0);
		}
		htrgdtime0_bytrig[i]->GetYaxis()->SetLabelSize(0.08);
		htrgdtime0_bytrig[i]->GetXaxis()->SetRangeUser(2400.,3300.);		//!!!!!!!
		htrgdtime0_bytrig[i]->Draw();
		htrgdtime0keep_bytrig[i]->Draw("same");
		sprintf(buf,"%s",triggernames[i]);
		thistext->SetTextSize(0.09);
		thistext->SetTextColor(2);
		thistext->DrawLatex(0.20,0.84,buf);
		gPad->Update();
	}
	ccan[ican]->cd(); ccan[ican]->Update();
	ccan[ican]->Print(psfile);
 	++ican; sprintf(buf,"ccan%d",ican);
 	ccan[ican] = new TCanvas(buf,buf,0,0,750.,750.*(8.5/11.));
 	ccan[ican]->SetFillColor(10);
 	ccan[ican]->cd(); ccan[ican]->Divide(8,4,0.0001,0.0001);
	for (int i=32;i<64;i++){
		ccan[ican]->cd(1+i-32);
		float thisscale = htrgbits->GetBinContent(i+1);
		if (thisscale>0){
			htrgdtime0_bytrig[i]->Scale(1./thisscale);
			htrgdtime0keep_bytrig[i]->Scale(1./thisscale);
			gPad->SetLogy(1);
		} else {
			gPad->SetLogy(0);
		}
		htrgdtime0_bytrig[i]->GetYaxis()->SetLabelSize(0.08);
		htrgdtime0_bytrig[i]->GetXaxis()->SetRangeUser(2400.,3300.);		//!!!!!!!
		htrgdtime0_bytrig[i]->Draw();
		htrgdtime0keep_bytrig[i]->Draw("same");
		sprintf(buf,"%s",triggernames[i]);
		thistext->DrawLatex(0.20,0.84,buf);
		thistext->SetTextSize(0.09);
		thistext->SetTextColor(2);
		gPad->Update();
	}
	ccan[ican]->cd(); ccan[ican]->Update();
	ccan[ican]->Print(psfile);
	thistext->SetTextSize(0.08);

	//---- trig dtime by trigger ID for VPD west
	//
	gStyle->SetPadLeftMargin(0.16);
	thistext->SetTextSize(0.06);
 	++ican; sprintf(buf,"ccan%d",ican);
 	ccan[ican] = new TCanvas(buf,buf,0,0,750.,750.*(8.5/11.));
 	ccan[ican]->SetFillColor(10);
 	ccan[ican]->cd(); ccan[ican]->Divide(8,4,0.0001,0.0001);
	for (int i=0;i<32;i++){
		ccan[ican]->cd(1+i);
		float thisscale = htrgbits->GetBinContent(i+1);
		if (thisscale>0){
			htrgdtime_bytrig[4][i]->Scale(1./thisscale);
			htrgdtimekeep_bytrig[4][i]->Scale(1./thisscale);
			gPad->SetLogy(1);
		} else {
			gPad->SetLogy(0);
		}
		htrgdtime_bytrig[4][i]->GetYaxis()->SetLabelSize(0.08);
		htrgdtime_bytrig[4][i]->GetXaxis()->SetRangeUser(0,500);		//!!!!!!!
		htrgdtime_bytrig[4][i]->Draw();
		htrgdtimekeep_bytrig[4][i]->Draw("same");
		sprintf(buf,"%s",triggernames[i]);
		thistext->SetTextSize(0.09);
		thistext->SetTextColor(2);
		thistext->DrawLatex(0.20,0.84,buf);
		gPad->Update();
	}
	ccan[ican]->cd(); ccan[ican]->Update();
	ccan[ican]->Print(psfile);
 	++ican; sprintf(buf,"ccan%d",ican);
 	ccan[ican] = new TCanvas(buf,buf,0,0,750.,750.*(8.5/11.));
 	ccan[ican]->SetFillColor(10);
 	ccan[ican]->cd(); ccan[ican]->Divide(8,4,0.0001,0.0001);
	for (int i=32;i<64;i++){
		ccan[ican]->cd(1+i-32);
		float thisscale = htrgbits->GetBinContent(i+1);
		if (thisscale>0){
			htrgdtime_bytrig[4][i]->Scale(1./thisscale);
			htrgdtimekeep_bytrig[4][i]->Scale(1./thisscale);
			gPad->SetLogy(1);
		} else {
			gPad->SetLogy(0);
		}
		htrgdtime_bytrig[4][i]->GetYaxis()->SetLabelSize(0.08);
		htrgdtime_bytrig[4][i]->GetXaxis()->SetRangeUser(0.,500.);		//!!!!!!!
		htrgdtime_bytrig[4][i]->Draw();
		htrgdtimekeep_bytrig[4][i]->Draw("same");
		sprintf(buf,"%s",triggernames[i]);
		thistext->DrawLatex(0.20,0.84,buf);
		thistext->SetTextSize(0.09);
		thistext->SetTextColor(2);
		gPad->Update();
	}
	ccan[ican]->cd(); ccan[ican]->Update();
	ccan[ican]->Print(psfile);
	thistext->SetTextSize(0.08);



	int kk=0;
	TLatex *tbo[2][6][2];
 	++ican; sprintf(buf,"ccan%d",ican);
 	ccan[ican] = new TCanvas(buf,buf,0,0,750.,750.*(8.5/11.));
 	ccan[ican]->SetFillColor(10);
 	ccan[ican]->cd(); ccan[ican]->Divide(6,2,0.0001,0.0001);
	for (int ithub=0;ithub<6;ithub++){
		for (int ih=0;ih<2;ih++){
			int nbx	= hbunchidoffset_thubhalf[ithub][ih]->GetXaxis()->GetNbins();
			for (int ibo=0;ibo<2;ibo++){
				tbo[ibo][ithub][ih]	= new TLatex();
					tbo[ibo][ithub][ih]->SetNDC();
					tbo[ibo][ithub][ih]->SetTextSize(0.1);
					tbo[ibo][ithub][ih]->SetTextColor(2);
			}
			int ibmax1	= hbunchidoffset_thubhalf[ithub][ih]->GetMaximumBin();
			int ibmax2	= 0;
			int ymax	= 0;
			for (int ib=1;ib<=nbx;ib++){
				int y	= hbunchidoffset_thubhalf[ithub][ih]->GetBinContent(ib);
				if (ib!=ibmax1 && y>ymax){
					ymax	= y;
					ibmax2	= ib;
				}
			}
			//
			kk	= ih*6 + ithub;
			ccan[ican]->cd(kk+1);
			hbunchidoffset_thubhalf[ithub][ih]->Draw();
			//
			cout<<ithub<<" "<<ih<<"\t"<<ibmax1<<" "<<ibmax2<<endl;
			float xmax1		= hbunchidoffset_thubhalf[ithub][ih]->GetBinCenter(ibmax1);
			float xmax2		= 0.;
			if (ibmax2>0){
				xmax2		= hbunchidoffset_thubhalf[ithub][ih]->GetBinCenter(ibmax2);
				tbo[0][ithub][ih]->DrawLatex(0.22,0.80,Form("%3.0f",TMath::Min(xmax1,xmax2)));
				tbo[1][ithub][ih]->DrawLatex(0.22,0.72,Form("%3.0f",TMath::Max(xmax1,xmax2)));
			} else {
				tbo[0][ithub][ih]->DrawLatex(0.22,0.80,Form("%3.0f",xmax1));
			}
			//
			hbunchidoffset_thubhalf[ithub][ih]->GetXaxis()->SetRangeUser(TMath::Min(xmax1,xmax2)-5,TMath::Max(xmax1,xmax2)+5);
			gPad->Update();
			//
		}
	}
	ccan[ican]->cd(); ccan[ican]->Update();
	ccan[ican]->Print(psfile);

	gStyle->SetPadLeftMargin(0.09);
	gStyle->SetOptFit(0);
 	++ican; sprintf(buf,"ccan%d",ican);
 	ccan[ican] = new TCanvas(buf,buf,0,0,750.,750.*(8.5/11.));
 	ccan[ican]->SetFillColor(10);
 	ccan[ican]->cd(); ccan[ican]->Divide(1,1,0.0001,0.0001);
	ccan[ican]->cd(1);
		gPad->SetLogy(1);
		htrgdtime_traycorrected->SetFillStyle(1001);
		htrgdtime_traycorrected->SetFillColor(7);
		htrgdtime_traycorrected->Draw();
		g1->Draw("same");
		g2->Draw("same");
		gPad->Update();
		for (int i=0;i<48;i++){
			cout<<"...crossings[i]="<<i<<" "<<crossings[i]<<endl;
			DrawVerticalLine(crossings[i],4);
			//lcrossings[i]->Draw("same");
		}
		//htrgdtime_traycorrected->Draw("same");
	ccan[ican]->cd(); ccan[ican]->Update();
	ccan[ican]->Print(psfile);

 	++ican; sprintf(buf,"ccan%d",ican);
 	ccan[ican] = new TCanvas(buf,buf,0,0,750.,750.*(8.5/11.));
 	ccan[ican]->SetFillColor(10);
 	ccan[ican]->cd(); ccan[ican]->Divide(1,1,0.0001,0.0001);
	ccan[ican]->cd(1);
		gPad->SetLogy(1);
		gPad->SetGridx(1);
		hcrossing_traycorrected->Draw();
		hcrossingkeep_traycorrected->Draw("same");
		sprintf(buf,"Crossing period = %6.2f ns",crossingperiod);
		thistext->SetTextSize(0.04);
		thistext->DrawLatex(0.18,0.84,buf);
	ccan[ican]->cd(); ccan[ican]->Update();
	ccan[ican]->Print(psfile);

	//gStyle->SetTitleFontSize(0.5);
	gStyle->SetTitleW(0.8);
	gStyle->SetTitleH(0.1);
 	++ican; sprintf(buf,"ccan%d",ican);
 	ccan[ican] = new TCanvas(buf,buf,0,0,750.,750.*(8.5/11.));
 	ccan[ican]->SetFillColor(10);
 	ccan[ican]->cd(); ccan[ican]->Divide(8,6,0.0001,0.0001);
	for (int ic=0;ic<48;ic++){
		ccan[ican]->cd(1+ic);
		gPad->SetLogy(1);
		gPad->SetGridx(1);
		hntofhits_crossing[ic]->SetTitleSize(0.9,"t");
		hntofhits_crossing[ic]->SetFillStyle(1001);
		hntofhits_crossing[ic]->SetFillColor(7);
		if (ic==28){ hntofhits_crossing[ic]->SetFillColor(3); }
		hntofhits_crossing[ic]->Draw();
	}
	ccan[ican]->cd(); ccan[ican]->Update();
	ccan[ican]->Print(psfile);
	gStyle->SetTitleFontSize(0);
	gStyle->SetTitleW(0);
	gStyle->SetTitleH(0);

	gStyle->SetOptStat(1111);
 	++ican; sprintf(buf,"ccan%d",ican);
 	ccan[ican] = new TCanvas(buf,buf,0,0,750.,750.*(8.5/11.));
 	ccan[ican]->SetFillColor(10);
 	ccan[ican]->cd(); ccan[ican]->Divide(3,2,0.0001,0.0001);
		ccan[ican]->cd(1);
			gPad->SetLogy(1);
			hnhitstof_xingall->Draw();
		ccan[ican]->cd(2);
			gPad->SetLogy(1);
			hnhitstof_xingtrg->Draw();
		ccan[ican]->cd(3);
			gPad->SetLogy(1);
			hnhitstof_xingm2->Draw();
		ccan[ican]->cd(4);
			gPad->SetLogy(1);
			hnhitstof_xingp2->Draw();	
		ccan[ican]->cd(5);
			gPad->SetLogy(1);
			hnhitstof_xingm7->Draw();	
		ccan[ican]->cd(6);
			gPad->SetLogy(1);
			hnhitstof_xingp7->Draw();	
	ccan[ican]->cd(); ccan[ican]->Update();
	ccan[ican]->Print(psfile);
	gStyle->SetOptStat(0);
	//
	gStyle->SetOptStat(1111);
 	++ican; sprintf(buf,"ccan%d",ican);
 	ccan[ican] = new TCanvas(buf,buf,0,0,750.,750.*(8.5/11.));
 	ccan[ican]->SetFillColor(10);
 	ccan[ican]->cd(); ccan[ican]->Divide(3,2,0.0001,0.0001);
		ccan[ican]->cd(1);
			gPad->SetLogy(1);
			hnhitstof_xingall->Draw();
		ccan[ican]->cd(2);
			gPad->SetLogy(1);
			hnhitstof_xingtrg_vpd->Draw();
		ccan[ican]->cd(3);
			gPad->SetLogy(1);
			hnhitstof_xingm2_vpd->Draw();
		ccan[ican]->cd(4);
			gPad->SetLogy(1);
			hnhitstof_xingp2_vpd->Draw();	
		ccan[ican]->cd(5);
			gPad->SetLogy(1);
			hnhitstof_xingm7_vpd->Draw();	
		ccan[ican]->cd(6);
			gPad->SetLogy(1);
			hnhitstof_xingp7_vpd->Draw();	
	ccan[ican]->cd(); ccan[ican]->Update();
	ccan[ican]->Print(psfile);
	gStyle->SetOptStat(0);	
	//
//  	++ican; sprintf(buf,"ccan%d",ican);
//  	ccan[ican] = new TCanvas(buf,buf,0,0,750.,750.*(8.5/11.));
//  	ccan[ican]->SetFillColor(10);
//  	ccan[ican]->cd(); ccan[ican]->Divide(1,1,0.0001,0.0001);
// 		ccan[ican]->cd(1);
// 		gPad->SetLogy(1);
// 		htrgdtime_rawall->Draw();
// 	ccan[ican]->cd(); ccan[ican]->Update();
// 	ccan[ican]->Print(psfile);
	//
//  	++ican; sprintf(buf,"ccan%d",ican);
//  	ccan[ican] = new TCanvas(buf,buf,0,0,750.,750.*(8.5/11.));
//  	ccan[ican]->SetFillColor(10);
//  	ccan[ican]->cd(); ccan[ican]->Divide(3,2,0.0001,0.0001);
// 	for (int i=0;i<6;i++){
// 		ccan[ican]->cd(1+i);
// 		gPad->SetLogy(1);
// 		gPad->SetGridx(1);
// 		htrgdtime[i]->GetXaxis()->SetRangeUser(-100.0,5100.0);
// 		htrgdtime[i]->Draw();
// 		htrgdtimekeep[i]->Draw("same");
// 	}
// 	ccan[ican]->cd(); ccan[ican]->Update();
// 	ccan[ican]->Print(psfile);
	
	gStyle->SetPadLeftMargin(0.16);
 	++ican; sprintf(buf,"ccan%d",ican);
 	ccan[ican] = new TCanvas(buf,buf,0,0,750.,750.*(8.5/11.));
 	ccan[ican]->SetFillColor(10);
 	ccan[ican]->cd(); ccan[ican]->Divide(8,4,0.0001,0.0001);
	for (int i=0;i<48;i++){
		ccan[ican]->cd(1+i);
		gPad->SetLogy(1);
		float thisscale = htrgbits->GetBinContent(i+1);
		if (thisscale>0){
			hnhits_prec_bytrig[i]->Scale(1./thisscale);
			hnhits_tric_bytrig[i]->Scale(1./thisscale);
			hnhits_posc_bytrig[i]->Scale(1./thisscale);
		}
		hnhits_prec_bytrig[i]->SetLineColor(4);
		hnhits_prec_bytrig[i]->Draw();
		hnhits_prec_bytrig[i]->GetYaxis()->SetLabelSize(0.08);
		hnhits_tric_bytrig[i]->SetLineColor(3);
		hnhits_tric_bytrig[i]->Draw("same");
		hnhits_posc_bytrig[i]->SetLineColor(6);
		hnhits_posc_bytrig[i]->Draw("same");
		sprintf(buf,"%s",triggernames[i]);
		thistext->SetTextSize(0.08);
		thistext->DrawLatex(0.18,0.84,buf);
		thistext->SetTextSize(0.08);
	}
	ccan[ican]->cd(); ccan[ican]->Update();
	ccan[ican]->Print(psfile);

//  	++ican; sprintf(buf,"ccan%d",ican);
//  	ccan[ican] = new TCanvas(buf,buf,0,0,750.,750.*(8.5/11.));
//  	ccan[ican]->SetFillColor(10);
//  	ccan[ican]->cd(); ccan[ican]->Divide(1,1,0.0001,0.0001);
// 	ccan[ican]->cd(1);
// 		gPad->SetLogz(1);
// 		htrgdtime_traycorrected_tray->Draw("colz");
// 	ccan[ican]->cd(); ccan[ican]->Update();
// 	ccan[ican]->Print(psfile);

//  	++ican; sprintf(buf,"ccan%d",ican);
//  	ccan[ican] = new TCanvas(buf,buf,0,0,750.,750.*(8.5/11.));
//  	ccan[ican]->SetFillColor(10);
//  	ccan[ican]->cd(); ccan[ican]->Divide(8,4,0.0001,0.0001);
// 	for (int i=0;i<48;i++){
// 		ccan[ican]->cd(1+i);
// 		gPad->SetLogy(1);
// 		gPad->SetGridx(0);
// 		htrgdtime_traycorrected_bytrig[i]->Draw();
// 		htrgdtime_traycorrected_bytrig[i]->Draw("same");
// 		sprintf(buf,"%s",triggernames[i]);
// 		thistext->DrawLatex(0.18,0.84,buf);
// 	}
// 	ccan[ican]->cd(); ccan[ican]->Update();
// 	ccan[ican]->Print(psfile);

	gStyle->SetPadLeftMargin(0.09);

	//---- mtd.....
	//
 	++ican; sprintf(buf,"ccan%d",ican);
 	ccan[ican] = new TCanvas(buf,buf,0,0,750.,750.*(8.5/11.));
 	ccan[ican]->SetFillColor(10);
 	ccan[ican]->cd(); ccan[ican]->Divide(1,2,0.0001,0.0001);
		ccan[ican]->cd(1);
			gPad->SetLogy(1);
			//htrgdtime_mtd[0]->GetXaxis()->SetRangeUser(2200.,3400.);
			htrgdtime_mtd[0]->Draw();
			htrgdtimekeep_mtd[0]->SetFillStyle(1001);
			htrgdtimekeep_mtd[0]->SetFillColor(3);
			htrgdtimekeep_mtd[0]->Draw("same");
		ccan[ican]->cd(2);
			gPad->SetLogy(1);
			//htrgdtime_mtd[1]->GetXaxis()->SetRangeUser(2200.,3400.);
			htrgdtime_mtd[1]->Draw();
			htrgdtimekeep_mtd[1]->SetFillStyle(1001);
			htrgdtimekeep_mtd[1]->SetFillColor(3);
			htrgdtimekeep_mtd[1]->Draw("same");
	ccan[ican]->cd(); ccan[ican]->Update();
	ccan[ican]->Print(psfile);
	//
 	++ican; sprintf(buf,"ccan%d",ican);
 	ccan[ican] = new TCanvas(buf,buf,0,0,750.,750.*(8.5/11.));
 	ccan[ican]->SetFillColor(10);
 	ccan[ican]->cd(); ccan[ican]->Divide(1,2,0.0001,0.0001);
		ccan[ican]->cd(1);
			gPad->SetLogy(1);
			htrgdtime_mtd[0]->GetXaxis()->SetRangeUser(2200.,3400.);
			htrgdtime_mtd[0]->Draw();
			htrgdtimekeep_mtd[0]->SetFillStyle(1001);
			htrgdtimekeep_mtd[0]->SetFillColor(3);
			htrgdtimekeep_mtd[0]->Draw("same");
		ccan[ican]->cd(2);
			gPad->SetLogy(1);
			htrgdtime_mtd[1]->GetXaxis()->SetRangeUser(2200.,3400.);
			htrgdtime_mtd[1]->Draw();
			htrgdtimekeep_mtd[1]->SetFillStyle(1001);
			htrgdtimekeep_mtd[1]->SetFillColor(3);
			htrgdtimekeep_mtd[1]->Draw("same");
	ccan[ican]->cd(); ccan[ican]->Update();
	ccan[ican]->Print(psfile);

	//
//  	++ican; sprintf(buf,"ccan%d",ican);
//  	ccan[ican] = new TCanvas(buf,buf,0,0,750.,750.*(8.5/11.));
//  	ccan[ican]->SetFillColor(10);
//  	ccan[ican]->cd(); ccan[ican]->Divide(6,5,0.0001,0.0001);
// 	for (int i=0;i<30;i++){
// 		ccan[ican]->cd(1+i);
// 		gPad->SetLogy(1);
// 		htrgdtime_mtd_tray[i]->GetXaxis()->SetRangeUser(2200.,3400.);
// 		htrgdtime_mtd_tray[i]->Draw();
// 		htrgdtimekeep_mtd_tray[i]->SetFillStyle(1001);
// 		htrgdtimekeep_mtd_tray[i]->SetFillColor(3);
// 		htrgdtimekeep_mtd_tray[i]->Draw("same");
// 	}
// 	ccan[ican]->cd(); ccan[ican]->Update();
// 	ccan[ican]->Print(psfile);
	//
	TF1 *ggausmtd[30];
 	++ican; sprintf(buf,"ccan%d",ican);
 	ccan[ican] = new TCanvas(buf,buf,0,0,750.,750.*(8.5/11.));
 	ccan[ican]->SetFillColor(10);
 	ccan[ican]->cd(); ccan[ican]->Divide(6,5,0.0001,0.0001);
	for (int i=0;i<30;i++){
		ccan[ican]->cd(1+i);
		gPad->SetLogy(0);
		//
		sprintf(buf,"ggausmtd%d",i);
		float al	= htrgdtimekeep_mtd_tray[i]->GetMean()-20;
		float au	= htrgdtimekeep_mtd_tray[i]->GetMean()+20;
		ggausmtd[i]	= new TF1(buf,"gaus",al,au);
		if (htrgdtimekeep_mtd_tray[i]->GetEntries()>100){
			htrgdtimekeep_mtd_tray[i]->Fit(buf,"QRN0");
		}
		//
		htrgdtime_mtd_tray[i]->GetXaxis()->SetRangeUser(ggausmtd[i]->GetParameter(1)-100,
														ggausmtd[i]->GetParameter(1)+100);
		htrgdtime_mtd_tray[i]->Draw();
		htrgdtimekeep_mtd_tray[i]->SetFillStyle(1001);
		htrgdtimekeep_mtd_tray[i]->SetFillColor(3);
		htrgdtimekeep_mtd_tray[i]->Draw("same");
		ggausmtd[i]->SetLineColor(2);
		ggausmtd[i]->SetLineWidth(2);
		ggausmtd[i]->Draw("same");
		//      
		if (htrgdtimekeep_mtd_tray[i]->GetEntries()>100){
			sprintf(buf,"#mu=%6.1f       #sigma=%4.1f",
					ggausmtd[i]->GetParameter(1),ggausmtd[i]->GetParameter(2));	
			thistext->SetTextSize(0.09);
			thistext->DrawLatex(0.18,0.84,buf);
			cout<<i+1<<" "<<ggausmtd[i]->GetParameter(1)<<" "
				<<ggausmtd[i]->GetParameter(2)<<endl;	
		}
	}
	ccan[ican]->cd(); ccan[ican]->Update();
	ccan[ican]->Print(psfile);

	//----------------------------------------------------------------
	//
 	++ican; sprintf(buf,"ccan%d",ican);
 	ccan[ican] = new TCanvas(buf,buf,0,0,750.,750.*(8.5/11.));
 	ccan[ican]->SetFillColor(10);
 	ccan[ican]->cd(); ccan[ican]->Divide(1,1,0.0001,0.0001);
	ccan[ican]->cd(1);
		gPad->SetLogz(1);
		htrgdtime_tray->GetYaxis()->SetRangeUser(2600,3200.);
		htrgdtime_tray->Draw("col");
	ccan[ican]->cd(); ccan[ican]->Update();
	ccan[ican]->Print(psfile);

	gStyle->SetPadLeftMargin(0.12);
	
	TH1D *htrgdtime_bytray[122];
	TF1	*thisgaus[122];
	double gpars[3];
	TLine *linel[122];
	TLine *lineu[122];
	float amax = 4500.;
	float amax_max = 0;
	float peaks[122];
	for (int ix=1;ix<=122;ix++){
		sprintf(buf,"htrgtime_bytray%d",ix-1);
		htrgdtime_bytray[ix-1] = (TH1D*)htrgdtime_tray->ProjectionY(buf,ix,ix,"");
		htrgdtime_bytray[ix-1]->SetName(buf);
		sprintf(buf,"dtrgtime, tray=%d",ix);
		htrgdtime_bytray[ix-1]->SetTitle(buf);
		int jpad	= (ix-1)%20;
		if ((ix-1)%20==0){
			//cout<<"new canvas... "<<ix<<" "<<jpad<<endl;
			++ican; sprintf(buf,"ccan%d",ican);
			//cout<<"new canvas "<<ican<<endl; 
			ccan[ican] = new TCanvas(buf,buf,0,0,750.,750.*(8.5/11.));
			ccan[ican]->SetFillColor(10);
			ccan[ican]->cd(); ccan[ican]->Divide(5,4,0.0001,0.0001);
		}
		//
		//---- get info from max peak....
		int ibmax	= htrgdtime_bytray[ix-1]->GetMaximumBin();
		float xpeak	= htrgdtime_bytray[ix-1]->GetBinCenter(ibmax);
		float ypeak	= htrgdtime_bytray[ix-1]->GetBinContent(ibmax);
		//
		//---- get info from fitslicesY....
		//
		//htrgdtime_bytray[ix-1]->
		sprintf(buf,"thisgaus%d",ix-1);
		thisgaus[ix-1]	= new TF1(buf,"gaus",xpeak-30,xpeak+15);
			thisgaus[ix-1]->SetLineColor(3);
			thisgaus[ix-1]->SetLineWidth(1);
		htrgdtime_bytray[ix-1]->Fit(buf,"QNR");
		thisgaus[ix-1]->GetParameters(gpars);			
		//
		peaks[ix-1]	= xpeak;
		peaks[ix-1]	= gpars[1];
		//
		if (ypeak>amax_max){ amax_max = ypeak; }
		float aliml	= xpeak-40.;
		float alimu	= xpeak+40.;
		//cout<<jpad<<" "<<"tray = "<<ix<<"\t peaktrigtime = "<<xpeak
		//	<<"\t windowlimits = "<<aliml<<" "<<alimu<<endl;
		linel[ix-1]	= new TLine(aliml,0.5,aliml,1.05*ypeak);
			linel[ix-1]->SetLineWidth(2.0); linel[ix-1]->SetLineColor(2);
		lineu[ix-1]	= new TLine(alimu,0.5,alimu,1.05*ypeak);
			lineu[ix-1]->SetLineWidth(2.0); lineu[ix-1]->SetLineColor(2);
		//
		ccan[ican]->cd(jpad+1);
			gPad->SetLogy(0);
			if (ix<=120){
				htrgdtime_bytray[ix-1]->GetXaxis()->SetRangeUser(2400.,3300.);
			} else {
				htrgdtime_bytray[ix-1]->GetXaxis()->SetRangeUser(0.,500.);
			}
			htrgdtime_bytray[ix-1]->SetMaximum(1.05*ypeak);
			htrgdtime_bytray[ix-1]->Draw();
			linel[ix-1]->Draw();
			lineu[ix-1]->Draw();
			htrgdtime_bytray[ix-1]->Draw("same");
			thisgaus[ix-1]->Draw("same");
		if (ix%20==0 || ix==122){
			//cout<<"end canvas... "<<ix<<" "<<jpad<<endl;
			ccan[ican]->cd(); ccan[ican]->Update();
			ccan[ican]->Print(psfile);
		}
	}
	cout<<" max Y = "<<amax_max<<endl;
	
	for (int i=0;i<122;i++){
		sprintf(buf,"%5.1f",peaks[i]);
		cout<<buf;
		if (i!=121)cout<<",";
		if ((i+1)%10==0)cout<<endl;
	}	cout<<endl;
	cout<<endl;

//	for (int i=0;i<48;i++){					//!!!!!
//		sprintf(buf,"%5.1f",crossings[i]);
//		cout<<buf;
//		if (i!=47)cout<<",";
//		if ((i+1)%10==0||i==47)cout<<endl;
//	}	cout<<endl;
//	sprintf(buf,"TF1 *crossingline = new TF1(\"crossingline\",\"%6.3f+x*%7.3f\");",crossings[0],crossingperiod);
//	cout<<buf<<endl;
	cout<<"float crossingorigin = "<<crossings[0]<<";"<<endl;
	cout<<"float crossingperiod = "<<crossingperiod<<";"<<endl;
	cout<<endl;

	gStyle->SetPadLeftMargin(0.09);

 	++ican; sprintf(buf,"ccan%d",ican);
 	ccan[ican] = new TCanvas(buf,buf,0,0,750.,750.*(8.5/11.));
 	ccan[ican]->SetFillColor(10);
 	ccan[ican]->cd(); ccan[ican]->Divide(1,1,0.0001,0.0001);
	ccan[ican]->cd(1);
		gPad->SetLogz(1);
		htrgdtime_trayoff->GetYaxis()->SetRangeUser(-250,250);
		htrgdtime_trayoff->Draw("col");
	ccan[ican]->cd(); ccan[ican]->Update();
	ccan[ican]->Print(psfile);

//  	++ican; sprintf(buf,"ccan%d",ican);
//  	ccan[ican] = new TCanvas(buf,buf,0,0,750.,750.*(8.5/11.));
//  	ccan[ican]->SetFillColor(10);
//  	ccan[ican]->cd(); ccan[ican]->Divide(1,1,0.0001,0.0001);
// 	ccan[ican]->cd(1);
// 		gPad->SetLogz(1);
// 		hnearly_nintime->Draw("col");
// 	ccan[ican]->cd(); ccan[ican]->Update();
// 	ccan[ican]->Print(psfile);
// 
//  	++ican; sprintf(buf,"ccan%d",ican);
//  	ccan[ican] = new TCanvas(buf,buf,0,0,750.,750.*(8.5/11.));
//  	ccan[ican]->SetFillColor(10);
//  	ccan[ican]->cd(); ccan[ican]->Divide(1,1,0.0001,0.0001);
// 	ccan[ican]->cd(1);
// 		gPad->SetLogy(1);
// 		hp_bbc->SetMinimum(0.5);
// 		hp_bbc->Draw();
// 		hp_bbc_tofintime->SetLineColor(3);
// 		hp_bbc_tofintime->Draw("same");
// 		hp_bbc_tofearly->SetLineColor(2);
// 		hp_bbc_tofearly->Draw("same");
// 	ccan[ican]->cd(); ccan[ican]->Update();
// 	ccan[ican]->Print(psfile);
// 	//
//  	++ican; sprintf(buf,"ccan%d",ican);
//  	ccan[ican] = new TCanvas(buf,buf,0,0,750.,750.*(8.5/11.));
//  	ccan[ican]->SetFillColor(10);
//  	ccan[ican]->cd(); ccan[ican]->Divide(1,1,0.0001,0.0001);
// 	ccan[ican]->cd(1);
// 		gPad->SetLogy(1);
// 		hnh_bbcl_tofearly->SetMinimum(0.5);
// 		hnh_bbcl_tofearly->SetLineColor(2);
// 		hnh_bbcl_tofearly->Draw();
// 		hnh_bbcl->Draw("same");
// 		hnh_bbcl_tofintime->SetLineColor(3);
// 		hnh_bbcl_tofintime->Draw("same");
// 	ccan[ican]->cd(); ccan[ican]->Update();
// 	ccan[ican]->Print(psfile);

//  	++ican; sprintf(buf,"ccan%d",ican);
//  	ccan[ican] = new TCanvas(buf,buf,0,0,750.,750.*(8.5/11.));
//  	ccan[ican]->SetFillColor(10);
//  	ccan[ican]->cd(); ccan[ican]->Divide(2,2,0.0001,0.0001);
// 	for (int i=0;i<4;i++){
// 		ccan[ican]->cd(1+i);
// 		gPad->SetLogy(1);
// 		htrgdtime[i]->GetXaxis()->SetRangeUser(2400.,3300.);
// 		htrgdtime[i]->Draw();
// 		htrgdtimekeep[i]->Draw("same");
// 		htrgdtime_special[i]->Draw("same");
// 		//htrgdtimekeep_special[i]->Draw("same");
// 	}
// 	ccan[ican]->cd(); ccan[ican]->Update();
// 	ccan[ican]->Print(psfile);
	

	
	ccan[ican]->Print(psfileclose);
	sprintf(buf,"ps2pdf %s %s",psfile,pdffile);
	cout<<buf<<endl;
	gSystem->Exec(buf);
	sprintf(buf,"cp -v %s /star/u/geurts/WWW/files/",pdffile);
	cout<<buf<<endl;
	gSystem->Exec(buf);

}

//----------------------------------------------------------------------------
void DrawVerticalLine(Double_t x, int icol){
   TLine l;
   l.SetLineColor(icol);
   //l.SetLineColor(18);
   Double_t lm = gPad->GetLeftMargin();
   Double_t rm = 1.-gPad->GetRightMargin();
   Double_t tm = 1.-gPad->GetTopMargin();
   Double_t bm = gPad->GetBottomMargin();
   Double_t xndc = (rm-lm)*((x-gPad->GetUxmin())/(gPad->GetUxmax()-gPad->GetUxmin()))+lm;
	//cout<<icol<<" "<<xndc<<" "<<bm<<" "<<tm<<"\t"
	//	<<gPad->GetUxmin()<<" "<<gPad->GetUxmax()
	//	<<endl;
   l.DrawLineNDC(xndc,bm,xndc,tm);
}
