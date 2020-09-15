const float tofarea =  20.;
const float mtd9area = 218.;
const float mtd11area = 331.;
const float ndaysmax = 1000;

const float TRAYCUTOFF = 4.;

int GetDayNum(int im, int id){
	int idaynum	= 0;
	if (im==1){ idaynum = id; } else				// jan
	if (im==2){ idaynum = id+31; } else				// feb
	if (im==3){ idaynum = id+31+28; } else			// mar
	if (im==4){ idaynum = id+31+28+31; } else		// apr
	if (im==5){ idaynum = id+31+28+31+30; } 		// may
	//cout<<im<<" "<<id<<" "<<idaynum<<endl;
	return idaynum;
}


void summary(){

TH1::AddDirectory(kFALSE);

	gErrorIgnoreLevel	= 2000;

	char		buf[100];
	char		tbuf[500];
	TCanvas*	ccan[500];
	int			ican	= -1;

	int       activeBL[30]	= {1,2,3,4,5,6,7,0,9,10,11,12,13,14,15,16,17,18,19,20,21,22,0,24,25,26,27,28,29,30};
	const int activeN		= 28;
 	const Int_t NRGBs 	= 5;
 	      Int_t NCont 	= activeN;
 	Double_t stops[NRGBs] = { 0.00, 0.10, 0.61, 0.99, 1.00 };
 	Double_t red[NRGBs]   = { 0.00, 0.00, 0.87, 1.00, 0.51 };
 	Double_t green[NRGBs] = { 0.00, 0.81, 1.00, 0.20, 0.00 };
 	Double_t blue[NRGBs]  = { 0.51, 1.00, 0.12, 0.00, 0.00 };
 	int icolstart = TColor::CreateGradientColorTable(NRGBs, stops, red, green, blue, NCont);
 	cout<<"special color index start = "<<icolstart<<endl;
	int kcolors_bl[30];
	int j	= 0;
	for (int i=0;i<30;i++){
		kcolors_bl[i]	= icolstart + j;
		if (activeBL[i]){ ++j; }
	}

	ifstream input;
	string line;
  int ndays = 0;
	int runnum[ndaysmax], year[ndaysmax], day[ndaysmax];
	input.open("noiseruns.txt");

  while (getline(input, line)){
		runnum[ndays] = atoi (line.c_str());
		year[ndays] = runnum[ndays]/1000;
		day[ndays] = runnum[ndays] - year[ndays]*1000;
	  ndays++;
	}
	input.close();

	float gday[ndaysmax];
	for (int n = 0; n<=ndays; n++){
	gday[n] = (year[n]-11)*365 + day[n] + (year[n]-9)/4;	// (year[n]-9)/4 = number of leap days (works until year 2100!)
	}

	float		lastday	= day[ndays-1];
	if (lastday>1000){ lastday	/= 10.; }
	float 	timetotalTOF[ndaysmax];
	float 	timetotalMTD[ndaysmax];
	TH1D*	hinfo[ndaysmax];
	TH1D*	hrate_tray[ndaysmax];
	TH1D*	htot[ndaysmax];
	TH1D*	htot_mtd[30][ndaysmax];
	TGraph *gglobalnoise	= new TGraph();
    	gglobalnoise->SetMarkerStyle(20);
	    gglobalnoise->SetMarkerSize(0.2);
			gglobalnoise->SetMarkerColor(1);
			gglobalnoise->SetLineColor(1);
			gglobalnoise->SetName("gglobalnoise");
	TGraph *gglobalnoiseL	= new TGraph();
			gglobalnoiseL->SetMarkerStyle(20);
			gglobalnoiseL->SetMarkerSize(0.2);
			gglobalnoiseL->SetMarkerColor(4);
			gglobalnoiseL->SetLineColor(4);
			gglobalnoiseL->SetName("gglobalnoiseL");
	TGraph *gglobalnoiseH	= new TGraph();
			gglobalnoiseH->SetMarkerStyle(20);
			gglobalnoiseH->SetMarkerSize(0.2);
			gglobalnoiseH->SetMarkerColor(2);
			gglobalnoiseH->SetLineColor(2);
			gglobalnoiseH->SetName("gglobalnoiseH");

	TGraph *gloopfitpara	= new TGraph();
			gloopfitpara->SetMarkerStyle(24);
			gloopfitpara->SetMarkerColor(1);
			gloopfitpara->SetLineColor(1);
			gloopfitpara->SetName("gloopfitpara");
	TGraph *gloopfitparb	= new TGraph();
			gloopfitparb->SetMarkerStyle(20);
			gloopfitparb->SetMarkerColor(1);
			gloopfitparb->SetLineColor(1);
			gloopfitparb->SetName("gloopfitparb");

	TGraph *gglobalnoisecm2	= new TGraph();
			gglobalnoisecm2->SetMarkerStyle(20);
			gglobalnoisecm2->SetMarkerColor(3);
			gglobalnoisecm2->SetLineColor(3);
			gglobalnoisecm2->SetName("gglobalnoisecm2");
	TGraph *gmeantot		= new TGraph();
			gmeantot->SetMarkerStyle(20);
			gmeantot->SetMarkerColor(3);
			gmeantot->SetLineColor(1);
			gmeantot->SetName("gmeantot");
	TGraph *gglobalnoisecm2_mtd[30];
	TGraph *gmeantot_mtd[30];
	for (int i=0;i<30;i++){
		gglobalnoisecm2_mtd[i]	= new TGraph();
		gglobalnoisecm2_mtd[i]->SetMarkerStyle(20);
		gglobalnoisecm2_mtd[i]->SetMarkerColor(kcolors_bl[i]);
		gglobalnoisecm2_mtd[i]->SetLineColor(kcolors_bl[i]);
		gglobalnoisecm2_mtd[i]->SetName(Form("gglobalnoisecm2_mtd%d",i));
		gmeantot_mtd[i]	= new TGraph();
		gmeantot_mtd[i]->SetMarkerStyle(20);
		gmeantot_mtd[i]->SetMarkerColor(kcolors_bl[i]);
		gmeantot_mtd[i]->SetLineColor(kcolors_bl[i]);
		gmeantot_mtd[i]->SetName(Form("gmeantot_mtd%d",i));
	}

	TGraph *gMCW	= new TGraph();
			gMCW->SetName("gMCW");
	TGraph *gMCW_day	= new TGraph();
			gMCW_day->SetName("gMCW_day");



	TLatex *thistext	= new TLatex();
		thistext->SetTextSize(0.175);
		thistext->SetNDC();
	for (int iday=0;iday<ndays;iday++){
		int	thisntot_mtd[30]	= {0};
		int thisday				= day[iday];
		if (day[iday]<1000  ){
			sprintf(buf,"/gpfs01/star/subsysg/TOF/BTOF-online/noise/noise_%d.root",runnum[iday]);}
		cout<<"i="<<iday<<"\t y="<<year[iday]<<"\t d="<<day[iday]<<" kFileID="<<runnum[iday]<<"\t"<<buf<<endl;
		TFile *fin = new TFile(buf,"READ");
			fin->cd();
			hrate_tray[iday]	= (TH1D*)gDirectory->Get("hrate_tray");
				sprintf(buf,"hrate_tray%d",iday);
				hrate_tray[iday]->SetName(buf);
			hinfo[iday]	= (TH1D*)gDirectory->Get("hinfo");
				sprintf(buf,"hinfo%d",iday);
				hinfo[iday]->SetName(buf);
			htot[iday]	= (TH1D*)gDirectory->Get("htot");
				sprintf(buf,"htot%d",iday);
				sprintf(buf,"ToT TOF, Day=%d",thisday);
				if (htot[iday]){
					htot[iday]->SetName(buf);
					htot[iday]->SetTitle(buf);
				} else {
					htot[iday]	= new TH1D(buf,tbuf,160,0.0,40.0);
				}
			for (int i=0;i<30;i++){
				if (year[iday]< 14){ sprintf(buf,"htot_mtd%d",i+1); } else
				if (year[iday]>=14){ sprintf(buf,"htot_mtd%d",i); }
				htot_mtd[i][iday]	= (TH1D*)gDirectory->Get(buf);
				sprintf(buf,"htot_mtd%d%d",i,iday);
				sprintf(tbuf,"ToT MTD BL%d, Day=%d",i+1,thisday);
				if (htot_mtd[i][iday]){
					htot_mtd[i][iday]->SetName(buf);
					htot_mtd[i][iday]->SetTitle(tbuf);
					thisntot_mtd[i]		= htot_mtd[i][iday]->GetEntries();
				} else {
					htot_mtd[i][iday]	= new TH1D(buf,tbuf,160,0.0,40.0);
					thisntot_mtd[i]		= 0;
				}
			}
		fin->Close();
		//
		float ntrays = 0.0;
		for (int ib=1;ib<=120;ib++){
			if (hrate_tray[iday]->GetBinContent(ib) > TRAYCUTOFF){ ++ntrays; }
		}
		timetotalTOF[iday]	= hinfo[iday]->GetBinContent(1);
		timetotalMTD[iday]	= hinfo[iday]->GetBinContent(2);
		float gnle	= hinfo[iday]->GetBinContent(3)/ntrays/192.;
		float gnte	= hinfo[iday]->GetBinContent(4)/ntrays/192.;
		float gn	= hinfo[iday]->GetBinContent(5)/ntrays/192.;
		float gnL	= hinfo[iday]->GetBinContent(6)/ntrays/192.;
		float gnH	= hinfo[iday]->GetBinContent(7)/ntrays/192.;
		float gnT	= hinfo[iday]->GetBinContent(8)/ntrays/192.;
		float gnmtd[30]	= {0};
		if (year[iday]==13){
			for (int i=0;i<3;i++){
				float 	af	= 120;
				if (i==0) af = 72;
				gnmtd[i+25]	= hinfo[iday]->GetBinContent(16+i)/af;
			}
		} else if (year[iday]>=14) {
			for (int i=0;i<30;i++){
				gnmtd[i]	= hinfo[iday]->GetBinContent(31+i)/120;
			}
		}
		float loopslo=0.;
		float loopint=0.;
		float ameantot_tof=0.;
		float ameantot_mtd[30]={0.};
		if (hinfo[iday]->GetNbinsX()>25){
			loopint			= hinfo[iday]->GetBinContent(21);
			loopslo			= hinfo[iday]->GetBinContent(22);
			ameantot_tof	= hinfo[iday]->GetBinContent(23);
			if (year[iday]==13){
				for (int i=0;i<3;i++){
					ameantot_mtd[i+25]	= hinfo[iday]->GetBinContent(24+i);
				}
			} else if (year[iday]>=14){
				for (int i=0;i<30;i++){
					ameantot_mtd[i]	= hinfo[iday]->GetBinContent(61+i);
				}
			}
		}
		//cout<<"i="<<iday<<"\t y="<<year[iday]<<"\t d="<<day[iday]<<" "<<thisday<<"\t D="<<gday[iday]
		//	<<"\t ntray="<<ntrays<<"\t rate: "<<gnT<<" "<<gnmtd[25]<<" "<<gnmtd[0]<<" "<<gnmtd[5]
		//	<<"\t loop a,b :"<<loopint<<" "<<loopslo
		//	<<"\t <ToT> :"<<ameantot_tof<<" "<<ameantot_mtd[25]<<" "<<ameantot_mtd[0]<<" "<<ameantot_mtd[5]
		//	<<endl;
		gglobalnoise->SetPoint(iday,gday[iday],gn);
		gglobalnoiseL->SetPoint(iday,gday[iday],gnL);
		gglobalnoiseH->SetPoint(iday,gday[iday],gnH);
		gloopfitpara->SetPoint(iday,gday[iday],loopint);
		gloopfitparb->SetPoint(iday,gday[iday],loopslo);
		gglobalnoisecm2->SetPoint(iday,gday[iday],gn/tofarea);
		gmeantot->SetPoint(iday,gday[iday],ameantot_tof);
		for (int i=0;i<30;i++){
			gglobalnoisecm2_mtd[i]->SetPoint(iday,gday[iday],gnmtd[i]/mtd11area);
			if (thisntot_mtd[i]>50){
				gmeantot_mtd[i]->SetPoint(iday,gday[iday],ameantot_mtd[i]);
			}
		}
	}

	gStyle->SetOptStat(0);
	gStyle->SetPadRightMargin(0.02);
	gStyle->SetPadTopMargin(0.02);
	gStyle->SetPadBottomMargin(0.09);
	gStyle->SetPadLeftMargin(0.09);
	gStyle->SetTitleOffset(0.90,"X");
	gStyle->SetTitleOffset(0.90,"Y");

	const int nmodes=17;
	TBox *modes[nmodes];
	modes[0]	= new TBox(-21,0,113,70);
	modes[1]	= new TBox(113,0,147,70);
	modes[2]	= new TBox(147,0,210,70);
	modes[3]	= new TBox(210,0,264,70);
	modes[4]	= new TBox(264,0,315,70);
	modes[5]	= new TBox(315,0,553,70);
	//run-12
	modes[6]	= new TBox(553,0,701,70);	// n2
	modes[7]	= new TBox(701,0,727,70);	// freon prechristmas
	modes[8]	= new TBox(727,0,756,70);	// off, holidays
	modes[9]	= new TBox(756,0,920,70);	// freon, run-12
	modes[10]	= new TBox(920,0,1122,70);	// n2
	//run-13
	modes[11]	= new TBox(1162,0,1262,70); 	// freon
	//pre-run-14 shutdown
	modes[12]	= new TBox(1263,0,1489,70); 	// n2
	//run-14
	modes[13]	= new TBox(1490,0,1499,70); 			// freon purge
	modes[14]	= new TBox(1500,0,1651,70); 	// recirc
        // run-15
	modes[15]	= new TBox(1790,0,2004,70); 	// recirc
        // run-16
	modes[16]	= new TBox(2207,0,gday[ndays-1]+6,70); 	// recirc
	//
	modes[0]->SetFillColor(41);		// recirc w/out aux
	modes[1]->SetFillColor(5);		// recirc w/ aux
	modes[2]->SetFillColor(30);		// purge
	modes[3]->SetFillColor(0);		//
	modes[4]->SetFillColor(18);		// N2
	modes[5]->SetFillColor(30);		// purge
	modes[6]->SetFillColor(18);		// N2
	modes[7]->SetFillColor(30);		// purge
	modes[8]->SetFillColor(0);		// off
	modes[9]->SetFillColor(30);		// purge
	modes[10]->SetFillColor(18);	// n2
	modes[11]->SetFillColor(30);	// purge
	modes[12]->SetFillColor(18);	// n2		// run13-14 shutdown
	modes[13]->SetFillColor(30);	// purge
	modes[14]->SetFillColor(41);	// recirc
	modes[15]->SetFillColor(0);	//
	modes[16]->SetFillColor(30);	// purge(?)
	TH1* hdum1 = new TH1F("hdum1","hdum1",1,0,1); hdum1->SetFillStyle(1001); hdum1->SetFillColor(41);
	TH1* hdum2 = new TH1F("hdum2","hdum2",1,0,1); hdum2->SetFillStyle(1001); hdum2->SetFillColor(5);
	TH1* hdum3 = new TH1F("hdum3","hdum3",1,0,1); hdum3->SetFillStyle(1001); hdum3->SetFillColor(30);
	TH1* hdum4 = new TH1F("hdum4","hdum4",1,0,1); hdum4->SetFillStyle(1001); hdum4->SetFillColor(18);

 	++ican; sprintf(buf,"ccan%d",ican);
 	ccan[ican] = new TCanvas(buf,buf,0,0,700,500);
 	ccan[ican]->SetFillColor(10);
 	ccan[ican]->cd(); ccan[ican]->Divide(1,1,0.0001,0.0001);
		TH1F *frame	=	gPad->DrawFrame(-21,0,(year[ndays-1]-11)*365+lastday+6,100);
			frame->GetXaxis()->SetTitle("day number");
			frame->GetYaxis()->SetTitle("global noise rate per channel (Hz)");
		for (int i=0;i<nmodes;i++){ modes[i]->Draw(); }
		gPad->RedrawAxis();
		gglobalnoise->Draw("LP");
		gglobalnoiseL->Draw("LP");
		gglobalnoiseH->Draw("LP");
		TLegend *lega = new TLegend(0.47,0.73,0.735,0.975);
			lega->SetTextSize(0.03);
			lega->SetFillStyle(0);
			lega->AddEntry(gglobalnoise ,"TOF, Total","P");
			lega->AddEntry(gglobalnoiseH,"TOF, ToT>18ns","P");
			lega->AddEntry(gglobalnoiseL,"TOF, ToT<18ns","P");
			lega->AddEntry(hdum1,"Recirc Mode, 450ccm","F");
			lega->AddEntry(hdum2,"Recirc Mode, 800ccm","F");
			lega->AddEntry(hdum3,"Purge Mode","F");
			lega->AddEntry(hdum4,"N2 Mode","F");
			lega->Draw();
	ccan[ican]->Update(); ccan[ican]->cd();
	ccan[ican]->Print("summary_noise.ps(");

 	++ican; sprintf(buf,"ccan%d",ican);
 	ccan[ican] = new TCanvas(buf,buf,0,0,700,500);
 	ccan[ican]->SetFillColor(10);
 	ccan[ican]->cd(); ccan[ican]->Divide(1,2,0.0001,0.0001);
	ccan[ican]->cd(1);
		frame	=	gPad->DrawFrame(320,0.0,(year[ndays-1]-11)*365+lastday+6,30);
			frame->GetXaxis()->SetTitle("day number");
			frame->GetYaxis()->SetTitle("ModuleInLoop, Intercept (Hz/cell)");
		gloopfitpara->Draw("P");
	ccan[ican]->cd(2);
		frame	=	gPad->DrawFrame(320,0.0,(year[ndays-1]-11)*365+lastday+6,0.30);
			frame->GetXaxis()->SetTitle("day number");
			frame->GetYaxis()->SetTitle("ModuleInLoop, Slope ([Hz/cell]/MRPCinLoop)");
		gloopfitparb->Draw("P");
	ccan[ican]->Update(); ccan[ican]->cd();
	ccan[ican]->Print("summary_noise.ps");

 	++ican; sprintf(buf,"ccan%d",ican);
 	ccan[ican] = new TCanvas(buf,buf,0,0,700,500);
 	ccan[ican]->SetFillColor(10);
 	ccan[ican]->cd(); ccan[ican]->Divide(1,1,0.0001,0.0001);
		frame	=	gPad->DrawFrame(-21,0,(year[ndays-1]-11)*365+lastday+6,8.5);
			frame->GetXaxis()->SetTitle("day number");
			frame->GetYaxis()->SetTitle("global noise rate per channel (Hz/cm^{2})");
		gglobalnoisecm2->Draw("P");
		gglobalnoisecm2_mtd[25]->Draw("P");
		gglobalnoisecm2_mtd[26]->Draw("P");
		gglobalnoisecm2_mtd[27]->Draw("P");
		TLegend *legb = new TLegend(0.13,0.93,0.94,0.98);
			legb->SetTextSize(0.04);
			legb->SetNColumns(4);
			legb->SetFillColor(0);
			//legb->SetFillStyle(0);
			legb->AddEntry(gglobalnoisecm2      ,"TOF","P");
			legb->AddEntry(gglobalnoisecm2_mtd[25],"MTD26","P");
			legb->AddEntry(gglobalnoisecm2_mtd[26],"MTD27","P");
			legb->AddEntry(gglobalnoisecm2_mtd[27],"MTD28","P");
			legb->Draw();
	ccan[ican]->Update(); ccan[ican]->cd();
	ccan[ican]->Print("summary_noise.ps");


	++ican; sprintf(buf,"ccan%d",ican);
 	ccan[ican] = new TCanvas(buf,buf,0,0,700,500);
 	ccan[ican]->SetFillColor(10);
 	ccan[ican]->cd(); ccan[ican]->Divide(1,1,0.0001,0.0001);
		frame	=	gPad->DrawFrame(1493,0,(year[ndays-1]-11)*365+lastday+6,8.5);
			frame->GetXaxis()->SetTitle("Run-14 day number (x=1460 is Jan 1, 2014)  ");
			frame->GetYaxis()->SetTitle("global noise rate per channel (Hz/cm^{2})");
		gglobalnoisecm2->Draw("PL");
		gglobalnoisecm2_mtd[25]->Draw("PL");
		gglobalnoisecm2_mtd[26]->Draw("PL");
		gglobalnoisecm2_mtd[27]->Draw("PL");
		TLegend *legc = new TLegend(0.13,0.93,0.94,0.98);
			legc->SetTextSize(0.04);
			legc->SetNColumns(4);
			legc->SetFillColor(0);
			//legc->SetFillStyle(0);
			legc->AddEntry(gglobalnoisecm2      ,"TOF","P");
			legc->AddEntry(gglobalnoisecm2_mtd[25],"MTD26","P");
			legc->AddEntry(gglobalnoisecm2_mtd[26],"MTD27","P");
			legc->AddEntry(gglobalnoisecm2_mtd[27],"MTD28","P");
			legc->Draw();
	ccan[ican]->Update(); ccan[ican]->cd();
	ccan[ican]->Print("summary_noise.ps");


	++ican; sprintf(buf,"ccan%d",ican);
 	ccan[ican] = new TCanvas(buf,buf,0,0,700,500);
 	ccan[ican]->SetFillColor(10);
 	ccan[ican]->cd(); ccan[ican]->Divide(1,1,0.0001,0.0001);
		frame	=	gPad->DrawFrame(1493,0,(year[ndays-1]-11)*365+lastday+6,4);
			frame->GetXaxis()->SetTitle("Run-14 day number (x=1460 is Jan 1, 2014)  ");
			frame->GetYaxis()->SetTitle("global noise rate per channel (Hz/cm^{2})");
		TLegend *legd = new TLegend(0.13,0.80,0.94,0.98);
			legd->SetTextSize(0.03);
			legd->SetFillColor(0);
			legd->SetNColumns(5);
		for (int i=0;i<30;i++){
			if (activeBL[i]){
				gglobalnoisecm2_mtd[i]->Draw("PL");
				sprintf(buf,"BL %d",i+1);
				legd->AddEntry(gglobalnoisecm2_mtd[i],buf,"P");
			}
		}
		legd->Draw();
	ccan[ican]->Update(); ccan[ican]->cd();
	ccan[ican]->Print("summary_noise.ps");


 	++ican; sprintf(buf,"ccan%d",ican);
 	ccan[ican] = new TCanvas(buf,buf,0,0,700,500);
 	ccan[ican]->SetFillColor(10);
 	ccan[ican]->cd(); ccan[ican]->Divide(1,1,0.0001,0.0001);
		frame	=	gPad->DrawFrame(320,17,(year[ndays-1]-11)*365+lastday+6,27);
			frame->GetXaxis()->SetTitle("day number");
			frame->GetYaxis()->SetTitle("#LTTot#GT");
		gmeantot->Draw("P");
		gmeantot_mtd[25]->Draw("P");
		gmeantot_mtd[26]->Draw("P");
		gmeantot_mtd[27]->Draw("P");
		TLegend *lege = new TLegend(0.13,0.93,0.94,0.98);
			lege->SetTextSize(0.04);
			lege->SetFillColor(0);
			lege->SetNColumns(5);
			lege->AddEntry(gmeantot_mtd[25],"MTD26","P");
			lege->AddEntry(gmeantot_mtd[26],"MTD27","P");
			lege->AddEntry(gmeantot_mtd[27],"MTD28","P");
			lege->AddEntry(gmeantot      ,"TOF","P");
			lege->Draw();
	ccan[ican]->Update(); ccan[ican]->cd();
	ccan[ican]->Print("summary_noise.ps");


	gStyle->SetPadRightMargin(0.1);
	gStyle->SetPadTopMargin(0.065);
	gStyle->SetPadBottomMargin(0.04);
	gStyle->SetPadLeftMargin(0.06);
	gStyle->SetPadGridY(0);
	TLine* lmodules[4];
	for (int il=0;il<4;il++){
		lmodules[il]	= new TLine(-0.5,((il+1)*24)-0.5,14.5,((il+1)*24)-0.5);
	}
	//----- end of all processing......

	cout<<"..... Writing summary.root "<<endl;
	TFile *f = new TFile("summary.root","RECREATE");
		f->cd();
		gMCW->Write();
		gMCW_day->Write();
		gglobalnoise->Write();
		gglobalnoiseL->Write();
		gglobalnoiseH->Write();
		gglobalnoisecm2->Write();
		gmeantot->Write();
		for (int i=0;i<30;i++){
			gglobalnoisecm2_mtd[i]->Write();
			gmeantot_mtd[i]->Write();
		}
		for (int iday=0;iday<ndays;iday++){
			htot[iday]->Write();
			for (int i=0;i<30;i++){
				htot_mtd[i][iday]->Write();
			}
		}
	f->Close();
	//
	ccan[ican]->Print("summary_noise.ps]");

	sprintf(buf,"ps2pdf %s.ps %s.pdf","summary_noise","summary_noise");
	cout<<"..... Executing ... "<<buf<<endl;
	gSystem->Exec(buf);
	sprintf(buf,"cp %s.pdf ~/WWW/noise/","summary_noise");
	cout<<"..... Executing ... "<<buf<<endl;
	gSystem->Exec(buf);

	cout<<"..... Done...."<<endl;

}
