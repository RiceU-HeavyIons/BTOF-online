#include <iostream>
#include <iomanip>
#include "TError.h"
using namespace std;

void thub() {

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

char type[20];

//sprintf(type,"13036036_tof");
//sprintf(type,"13037090_tof");
//sprintf(type,"13039132_tof");
//sprintf(type,"13039_tof");
//sprintf(type,"13039_tof_tofmult4");
//sprintf(type,"13039136_tof");
//sprintf(type,"13041010_tof");
//sprintf(type,"13042023_tof");

//sprintf(type,"13078014_tof");
sprintf(type,"13080022_tof");

//sprintf(type,"12153002_tof");
//sprintf(type,"12147026_tof");
//sprintf(type,"12144052_tof");
//sprintf(type,"12140040_tof");

//sprintf(type,"12138_tof");		// noise data test!
//sprintf(type,"12133046_tof");		// noise data test!

//sprintf(type,"12133018_tof");
//sprintf(type,"test_tof");

//sprintf(type,"12130061_tof");
//sprintf(type,"12130062_tof");
//sprintf(type,"12130063_tof");

//sprintf(type,"12127004_tof");
//sprintf(type,"12129010_tof");
//sprintf(type,"12129011_tof");
//sprintf(type,"12129012_tof");
//sprintf(type,"12129013_tof");

//sprintf(type,"12127004_tof");
//sprintf(type,"12128039_tof");
//sprintf(type,"12129003_tof");

//sprintf(type,"12118044");
//sprintf(type,"12118035");
//sprintf(type,"test");
//sprintf(type,"12116080");
//sprintf(type,"12116080_tof200fast");
//sprintf(type,"12116080_tof200fast_allevts");
//sprintf(type,"12116080_mb");
//sprintf(type,"12116080_vpdmontac");
//sprintf(type,"12116080_bbcsmallmonnarrow");
//sprintf(type,"12116080_bbclargemonnarrow");

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

// 
// 	//---- run11 auau19
// 	// mb1-fast					1			340011	
// 	// mb2-slow					2			340012	
// 	// ht-11						8			340500	
// 	// HLT-tracks					10			340005	
// 	// zdc-mon						40			7		
// 	// zdc-mon-tac					80			8		
// 	// vpd-mon						100			340060	
// 	// vpd-mon-tac					200			340061	
// 	// bbc-small-mon				400			340052	
// 	// bbc-small-mon-narrow		800			340053	
// 	// bbc-small-mon-wide			1000		340054	
// 	// bbc-large-mon				2000		340065	
// 	// bbc-large-mon-narrow		4000		340066	
// 	// bbc-large-mon-wide			8000		340067	
// 	// zerobias					20000		9300	
// 	// HLT-good					40000		340004	
// 	// bbc-small-tof-mon-wide		100000		340080	
// 	// bbc-small-tof-mon-narrow	200000		340081	
// 	// mtd							400000		340710	
// 	// tof-200-fast				800000		340300	
// 	// tof-250-slow				1000000		340301	
// 	const char* triggernames[32] = {
// 				"mb1-fast",					//1			340011	
// 				"mb2-slow",					//2			340012	
// 				" ",						//4
// 				"ht-11",					//8			340500	
// 				"HLT-tracks",				//10		340005	
// 				" ",						//20
// 				"zdc-mon",					//40		7		
// 				"zdc-mon-tac",				//80		8		
// 				"vpd-mon",					//100		340060	
// 				"vpd-mon-tac",				//200		340061	
// 				"bbc-small-mon",			//400		340052	
// 				"bbc-small-mon-narrow",		//800		340053	
// 				"bbc-small-mon-wide",		//1000		340054	
// 				"bbc-large-mon",			//2000		340065	
// 				"bbc-large-mon-narrow",		//4000		340066	
// 				"bbc-large-mon-wide",		//8000		340067	
// 				" ",						//10000
// 				"zerobias",					//20000		9300	
// 				"HLT-good",					//40000		340004	
// 				" ",						//80000
// 				"bbc-small-tof-mon-wide",	//100000	340080	
// 				"bbc-small-tof-mon-narrow",	//200000	340081	
// 				"mtd",						//400000	340710	
// 				"tof-200-fast",				//800000	340300	
// 				"tof-250-slow",				//1000000	340301	
// 				" "," "," "," "," "," "," "	
// 	};
//
//---- run11 auau200
// hlt-heavy-fragment	hlt			1000	6		1.0	687
// hlt-diElectron		hlt			2000	8		1.0	1345
// hlt-random			hlt			80000	19		1.0	200
// L2gamma				gamma		100000	20		1.0	536
// zerobias				zerobias	10		9300	10000000.0	363
// vpd-zdc-mb			physics		20		350001	123.5	4174
// vpd-zdc-novtx-mon	physics		400		350002	308.7	3909
// vpd-zdc-mb-protected	physics		400000	350003	2.4	169414
// NPE_25_nozdc			ht			40		350501	1.0	2227
// NPE_15				physics		10000	350502	1.0	12881
// NPE_18				ht			20000	350503	1.0	7423
// NPE_25				ht			40000	350504	1.0	1222
// NPE_11				physics		800		350505	4.9	17575
// 	const char* triggernames[32] = {
// 		"UPC_main",				// 1
// 		"ZDC_monitor",			// 2
// 		"CosmicBeam1",			// 4
// 		" ",					// 8
// 		"zerobias",				// 10
// 		"vpd-zdc-mb",			// 20
// 		"NPE_25_nozdc",			// 40
// 		"CosmicBeam2",			// 80
// 		"CosmicBeam3",			// 100
// 		" ",					// 200
// 		"vpd-zdc-novtx-mon",	// 400
// 		"NPE_11",				// 800
// 		"hlt-heavy-fragment",	// 1000
// 		"hlt-diElectron",		// 2000
// 		"mtd",					// 4000
// 		"UPC_topo",				// 8000
// 		"NPE_15",				// 10000
// 		"NPE_18",				// 20000
// 		"NPE_25",				// 40000
// 		"hlt-random",			// 80000
// 		"L2gamma",				// 100000
// 		"future-guardian",		// 200000
// 		"vpd-zdc-mb-protected",	// 400000
// 		"zdc-ht-mon",			// 800000
// 		"NPE_15_ftp",			// 1000000
// 		"NPE_18_ftp",			// 2000000
// 		"NPE_25_ftp",			// 4000000
// 		"NPE_15_nobsmd",		// 8000000
// 		"NPE_11_ftp",			// 10000000
// 		"ftpc_mb_protected",	// 20000000
// 		" "," "};	
	
	
//---- run12 pp200
// 	const char* triggernames[32] = {
// 		" ",				// 1
// 		"bbc",				// 2
// 		" ",				// 4
// 		" ",				// 8
// 		"BHT2 ",			// 10
// 		"BHT2*BBCMB ",		// 20
// 		"EHT0",				// 40
// 		"FGT_test",			// 80
// 		"JP0",				// 100
// 		"JP1",				// 200
// 		"BBCMB ",			// 400
// 		"VPDMB ",			// 800
// 		"VPDMB-nobsmd",		// 1000
// 		"ZDCMB-trgonly ",	// 2000
// 		"MTD*VPD ",			// 4000
// 		"BHT0*VPDMB ",		// 8000
// 		"BHT0*BBCMB*TOF0 ",	// 10000
// 		"BHT1*VPDMB ",		// 20000
// 		"AJP ",				// 40000
// 		" ",				// 80000
// 		"MTDcosmic*!YB*TOF1 ",		// 100000
// 		"MTD*BHT3 ",		// 200000
// 		"TOFMult4 ",		// 400000
// 		"JP2*L2JetHigh",	// 800000
// 		"BHT3 ",			// 1000000
// 		"BHT3*L2BW ",		// 2000000
// 		"BBCMB*TOF0 ",		// 4000000
// 		"TOFMult3*VPD ",	// 8000000
// 		"ZEROBIAS ",		// 10000000
// 		"laser",			// 20000000
// 		"HT0*DAQ10k",		// 40000000
// 		" ",				// 80000000
// 		};		

//---- run12 pp500
	const char* triggernames[32] = {
		"VPDMB",			// 1
		"VPDMB-nobsmd",		// 2
		"BBCMB",			// 4
		"ZDCMB-trgonly",	// 8
		"BHT0*BBCMB*TOF0",	// 10
		"BHT0*VPD",			// 20
		" ",				// 40
		"BHT2",				// 80
		"BHT2*BJP1",		// 100
		"BHT3",				// 200
		"EHT0-nofgt",		// 400
		"EHT0-fgt",			// 800
		"EHT1",				// 1000
		"JP0",				// 2000
		"JP1",				// 4000
		"JP2",				// 8000
		"AJP",				// 10000
		"FMS-base",			// 20000
		"FMSSmBS0",			// 40000
		"FMSLgBS0",			// 80000
		"FMSJP1",		// 100000
		"FMSHT0",		// 200000
		"FMSJP0*JP0",	// 400000
		"",			// 800000
		"",			// 1000000
		"",			// 2000000
		"",			// 4000000
		"",			// 8000000
		"",			// 10000000
		"ZEROBIAS",		// 20000000
		"",			// 40000000
		" ",		// 80000000
		};		
	
	
	TLatex *ttrgnames[32];
	for (int it=0;it<32;it++){
		unsigned int ival = pow(2,it);
		cout<<setbase(10)<<it<<" "
			<<setprecision(12)<<ival<<" "
	    	<<setbase(16)<<ival<<" "<<setbase(10)
	    	<<triggernames[it]<<endl;
		sprintf(buf,triggernames[it]);
		ttrgnames[it] = new TLatex(it,0.6,buf);
		ttrgnames[it]->SetTextAngle(90);
		ttrgnames[it]->SetTextSize(0.03);
		ttrgnames[it]->SetTextAlign(12);
	}

TLatex *thistext = new TLatex();
	thistext->SetTextSize(0.08);
	thistext->SetNDC();

TH1::AddDirectory(kFALSE);

sprintf(buf,"/star/institutions/rice/llope/dd/daqdoer_%s.root",type);
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
	TF1 *g1	= new TF1("g1","gaus",140,180);
		g1->SetLineWidth(1);
		g1->SetLineColor(2);
	TF1 *g2	= new TF1("g2","gaus",2910,2950);
		g2->SetLineWidth(1);
		g2->SetLineColor(2);
	htrgdtime_traycorrected->Fit("g1","R");
	htrgdtime_traycorrected->Fit("g2","+R");
	double gpars1[3],gpars2[3];
	g1->GetParameters(gpars1);
	g2->GetParameters(gpars2);
	float crossingperiod	= (gpars2[1]-gpars1[1])/26.;
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
	TH1D *htrgdtime[4];
	TH1D *htrgdtimekeep[4];
//	TH1D *htrgdtime_special[4];
//	TH1D *htrgdtimekeep_special[4];
	TH1D *htrgdtime0_bytrig[32];
	TH1D *htrgdtime0keep_bytrig[32];
	TH1D *htrgdtime_traycorrected_bytrig[32];
	for (int i=0;i<4;i++){
		sprintf(buf,"htrgdtime%d",i);
		htrgdtime[i]	= (TH1D*)f->Get(buf);
		sprintf(buf,"htrgdtimekeep%d",i);
		htrgdtimekeep[i]	= (TH1D*)f->Get(buf);
//		sprintf(buf,"htrgdtime_special%d",i);
//		htrgdtime_special[i]	= (TH1D*)f->Get(buf);
//		sprintf(buf,"htrgdtimekeep_special%d",i);
//		htrgdtimekeep_special[i]	= (TH1D*)f->Get(buf);
	}
	TH1D *hnhits_prec_bytrig[32];
	TH1D *hnhits_tric_bytrig[32];
	TH1D *hnhits_posc_bytrig[32];
	for (int i=0;i<32;i++){
		sprintf(buf,"htrgdtime0_bytrig%d",i);
		htrgdtime0_bytrig[i]	= (TH1D*)f->Get(buf);
		sprintf(buf,"htrgdtime0keep_bytrig%d",i);
		htrgdtime0keep_bytrig[i]	= (TH1D*)f->Get(buf);
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

	TH1D *htrgdtime_mtd = (TH1D*)f->Get("htrgdtime_mtd");
	TH1D *htrgdtime_mtd_tray[4];
	for (int i=0;i<4;i++){
		sprintf(buf,"htrgdtime_mtd_tray%d",i);
		htrgdtime_mtd_tray[i]	= (TH1D*)f->Get(buf);
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
	
	//------------------------------------------------------------------
	
	gStyle->SetPadGridX(kFALSE);
	gStyle->SetPadGridY(kFALSE);
	gStyle->SetPadLeftMargin(0.16);
	gStyle->SetPadRightMargin(0.02);

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
		for (int it=0;it<32;it++){
			ttrgnames[it]->Draw("same");
		}
	ccan[ican]->cd(); ccan[ican]->Update();
	ccan[ican]->Print(psfileopen);
	
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
	}
	ccan[ican]->cd(); ccan[ican]->Update();
	ccan[ican]->Print(psfile);

	thistext->SetTextSize(0.06);
 	++ican; sprintf(buf,"ccan%d",ican);
 	ccan[ican] = new TCanvas(buf,buf,0,0,750.,750.*(8.5/11.));
 	ccan[ican]->SetFillColor(10);
 	ccan[ican]->cd(); ccan[ican]->Divide(8,4,0.0001,0.0001);
	for (int i=0;i<32;i++){
		ccan[ican]->cd(1+i);
		gPad->SetLogy(1);
		float thisscale = htrgbits->GetBinContent(i+1);
		if (thisscale>0){
			htrgdtime0_bytrig[i]->Scale(1./thisscale);
			htrgdtime0keep_bytrig[i]->Scale(1./thisscale);
		}
		htrgdtime0_bytrig[i]->GetYaxis()->SetLabelSize(0.08);
		htrgdtime0_bytrig[i]->GetXaxis()->SetRangeUser(2400.,3300.);
		htrgdtime0_bytrig[i]->Draw();
		htrgdtime0keep_bytrig[i]->Draw("same");
		sprintf(buf,"%s",triggernames[i]);
		thistext->DrawLatex(0.18,0.84,buf);
	}
	ccan[ican]->cd(); ccan[ican]->Update();
	ccan[ican]->Print(psfileopen);
	thistext->SetTextSize(0.08);

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
 		for (int i=0;i<48;i++){
 			lcrossings[i]->Draw("same");
 		}
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
		if (ic==27){ hntofhits_crossing[ic]->SetFillColor(3); }
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
 	++ican; sprintf(buf,"ccan%d",ican);
 	ccan[ican] = new TCanvas(buf,buf,0,0,750.,750.*(8.5/11.));
 	ccan[ican]->SetFillColor(10);
 	ccan[ican]->cd(); ccan[ican]->Divide(1,1,0.0001,0.0001);
		ccan[ican]->cd(1);
		gPad->SetLogy(1);
		htrgdtime_rawall->Draw();
	ccan[ican]->cd(); ccan[ican]->Update();
	ccan[ican]->Print(psfile);
	//
 	++ican; sprintf(buf,"ccan%d",ican);
 	ccan[ican] = new TCanvas(buf,buf,0,0,750.,750.*(8.5/11.));
 	ccan[ican]->SetFillColor(10);
 	ccan[ican]->cd(); ccan[ican]->Divide(2,2,0.0001,0.0001);
	for (int i=0;i<4;i++){
		ccan[ican]->cd(1+i);
		gPad->SetLogy(1);
		gPad->SetGridx(1);
		htrgdtime[i]->GetXaxis()->SetRangeUser(-250.5,5249.5);
		htrgdtime[i]->Draw();
		htrgdtimekeep[i]->Draw("same");
	}
	ccan[ican]->cd(); ccan[ican]->Update();
	ccan[ican]->Print(psfile);
	
 	++ican; sprintf(buf,"ccan%d",ican);
 	ccan[ican] = new TCanvas(buf,buf,0,0,750.,750.*(8.5/11.));
 	ccan[ican]->SetFillColor(10);
 	ccan[ican]->cd(); ccan[ican]->Divide(8,4,0.0001,0.0001);
	for (int i=0;i<32;i++){
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
	ccan[ican]->Print(psfileopen);

//  	++ican; sprintf(buf,"ccan%d",ican);
//  	ccan[ican] = new TCanvas(buf,buf,0,0,750.,750.*(8.5/11.));
//  	ccan[ican]->SetFillColor(10);
//  	ccan[ican]->cd(); ccan[ican]->Divide(1,1,0.0001,0.0001);
// 	ccan[ican]->cd(1);
// 		gPad->SetLogz(1);
// 		htrgdtime_traycorrected_tray->Draw("colz");
// 	ccan[ican]->cd(); ccan[ican]->Update();
// 	ccan[ican]->Print(psfile);

 	++ican; sprintf(buf,"ccan%d",ican);
 	ccan[ican] = new TCanvas(buf,buf,0,0,750.,750.*(8.5/11.));
 	ccan[ican]->SetFillColor(10);
 	ccan[ican]->cd(); ccan[ican]->Divide(8,4,0.0001,0.0001);
	for (int i=0;i<32;i++){
		ccan[ican]->cd(1+i);
		gPad->SetLogy(1);
		gPad->SetGridx(0);
		htrgdtime_traycorrected_bytrig[i]->Draw();
		htrgdtime_traycorrected_bytrig[i]->Draw("same");
		sprintf(buf,"%s",triggernames[i]);
		thistext->DrawLatex(0.18,0.84,buf);
	}
	ccan[ican]->cd(); ccan[ican]->Update();
	ccan[ican]->Print(psfileopen);

	//---- mtd.....
	//
 	++ican; sprintf(buf,"ccan%d",ican);
 	ccan[ican] = new TCanvas(buf,buf,0,0,750.,750.*(8.5/11.));
 	ccan[ican]->SetFillColor(10);
 	ccan[ican]->cd(); ccan[ican]->Divide(1,1,0.0001,0.0001);
	ccan[ican]->cd(1);
		gPad->SetLogy(1);
		htrgdtime_mtd->Draw();
		htrgdtimekeep_mtd->SetFillStyle(1001);
		htrgdtimekeep_mtd->SetFillColor(3);
		htrgdtimekeep_mtd->Draw("same");
	ccan[ican]->cd(); ccan[ican]->Update();
	ccan[ican]->Print(psfile);
	//
 	++ican; sprintf(buf,"ccan%d",ican);
 	ccan[ican] = new TCanvas(buf,buf,0,0,750.,750.*(8.5/11.));
 	ccan[ican]->SetFillColor(10);
 	ccan[ican]->cd(); ccan[ican]->Divide(2,2,0.0001,0.0001);
	for (int i=0;i<4;i++){
		ccan[ican]->cd(1+i);
		gPad->SetLogy(1);
		//htrgdtime_mtd_tray[i]->GetXaxis()->SetRangeUser(2400.,3300.);
		htrgdtime_mtd_tray[i]->Draw();
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
		htrgdtime_tray->GetYaxis()->SetRangeUser(2750.,3100.);
		htrgdtime_tray->Draw("col");
	ccan[ican]->cd(); ccan[ican]->Update();
	ccan[ican]->Print(psfile);
	
	TH1D *htrgdtime_bytray[120];
	TF1	*thisgaus[120];
	double gpars[3];
	TLine *linel[120];
	TLine *lineu[120];
	float amax = 4500.;
	float amax_max = 0;
	float peaks[120];
	for (int ix=1;ix<=120;ix++){
//	for (int ix=1;ix<=30;ix++){
		sprintf(buf,"htrgtime_bytray%d",ix-1);
		htrgdtime_bytray[ix-1] = (TH1D*)htrgdtime_tray->ProjectionY(buf,ix,ix,"");
		htrgdtime_bytray[ix-1]->SetName(buf);
		sprintf(buf,"dtrgtime, tray=%d",ix);
		htrgdtime_bytray[ix-1]->SetTitle(buf);
		int jpad	= (ix-1)%20;
		if ((ix-1)%20==0){
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
			htrgdtime_bytray[ix-1]->GetXaxis()->SetRangeUser(2750.,3100.);
			htrgdtime_bytray[ix-1]->SetMaximum(1.05*ypeak);
			htrgdtime_bytray[ix-1]->Draw();
			linel[ix-1]->Draw();
			lineu[ix-1]->Draw();
			htrgdtime_bytray[ix-1]->Draw("same");
			thisgaus[ix-1]->Draw("same");
		if (ix%20==0){
			ccan[ican]->cd(); ccan[ican]->Update();
			ccan[ican]->Print(psfile);
		}
	}
	cout<<" max Y = "<<amax_max<<endl;
	
	for (int i=0;i<120;i++){
		sprintf(buf,"%5.1f",peaks[i]);
		cout<<buf;
		if (i!=119)cout<<",";
		if ((i+1)%10==0)cout<<endl;
	}	cout<<endl;

	for (int i=0;i<48;i++){
		sprintf(buf,"%5.1f",crossings[i]);
		cout<<buf;
		if (i!=47)cout<<",";
		if ((i+1)%10==0||i==47)cout<<endl;
	}	cout<<endl;
//	sprintf(buf,"TF1 *crossingline = new TF1(\"crossingline\",\"%6.3f+x*%7.3f\");",crossings[0],crossingperiod);
//	cout<<buf<<endl;
	cout<<"float crossingorigin = "<<crossings[0]<<";"<<endl;
	cout<<"float crossingperiod = "<<crossingperiod<<";"<<endl;
	cout<<endl;

 	++ican; sprintf(buf,"ccan%d",ican);
 	ccan[ican] = new TCanvas(buf,buf,0,0,750.,750.*(8.5/11.));
 	ccan[ican]->SetFillColor(10);
 	ccan[ican]->cd(); ccan[ican]->Divide(1,1,0.0001,0.0001);
	ccan[ican]->cd(1);
		gPad->SetLogz(1);
		htrgdtime_trayoff->GetYaxis()->SetRangeUser(2750.,3100.);
		htrgdtime_trayoff->Draw("col");
	ccan[ican]->cd(); ccan[ican]->Update();
	ccan[ican]->Print(psfile);

 	++ican; sprintf(buf,"ccan%d",ican);
 	ccan[ican] = new TCanvas(buf,buf,0,0,750.,750.*(8.5/11.));
 	ccan[ican]->SetFillColor(10);
 	ccan[ican]->cd(); ccan[ican]->Divide(1,1,0.0001,0.0001);
	ccan[ican]->cd(1);
		gPad->SetLogz(1);
		hnearly_nintime->Draw("col");
	ccan[ican]->cd(); ccan[ican]->Update();
	ccan[ican]->Print(psfile);

 	++ican; sprintf(buf,"ccan%d",ican);
 	ccan[ican] = new TCanvas(buf,buf,0,0,750.,750.*(8.5/11.));
 	ccan[ican]->SetFillColor(10);
 	ccan[ican]->cd(); ccan[ican]->Divide(1,1,0.0001,0.0001);
	ccan[ican]->cd(1);
		gPad->SetLogy(1);
		hp_bbc->SetMinimum(0.5);
		hp_bbc->Draw();
		hp_bbc_tofintime->SetLineColor(3);
		hp_bbc_tofintime->Draw("same");
		hp_bbc_tofearly->SetLineColor(2);
		hp_bbc_tofearly->Draw("same");
	ccan[ican]->cd(); ccan[ican]->Update();
	ccan[ican]->Print(psfile);
	//
 	++ican; sprintf(buf,"ccan%d",ican);
 	ccan[ican] = new TCanvas(buf,buf,0,0,750.,750.*(8.5/11.));
 	ccan[ican]->SetFillColor(10);
 	ccan[ican]->cd(); ccan[ican]->Divide(1,1,0.0001,0.0001);
	ccan[ican]->cd(1);
		gPad->SetLogy(1);
		hnh_bbcl_tofearly->SetMinimum(0.5);
		hnh_bbcl_tofearly->SetLineColor(2);
		hnh_bbcl_tofearly->Draw();
		hnh_bbcl->Draw("same");
		hnh_bbcl_tofintime->SetLineColor(3);
		hnh_bbcl_tofintime->Draw("same");
	ccan[ican]->cd(); ccan[ican]->Update();
	ccan[ican]->Print(psfile);

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
// 	ccan[ican]->Print(psfileopen);
	

	
	ccan[ican]->Print(psfileclose);
	sprintf(buf,"ps2pdf %s %s",psfile,pdffile);
	gSystem->Exec(buf);
	sprintf(buf,"cp -v %s /star/u/llope/WWW/files/",pdffile);
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
