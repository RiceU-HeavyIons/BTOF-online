
const float tofarea =  20.;
const float mtd9area = 218.;
const float mtd11area = 331.;

const float TRAYCUTOFF = 4.;

void summary(){

TH1::AddDirectory(kFALSE);

	gErrorIgnoreLevel	= 2000;

	char		buf[100];
	char		tbuf[500];
	TCanvas*	ccan[500];
	int			ican	= -1;

	int       activeBL[30]	= {1,2,3,4,5,6,7,0,0,10,11,12,13,14,15,16,17,18,19,20,21,22,0,24,25,26,27,28,29,30};
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

	const int 	ndays	=  239;
	int year[ndays]	= { 
	 10, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11,
	 11, 11, 11, 11, 11, 11, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 
	 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 13,
	 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 
	 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 
	 13, 13, 13, 13, 13, 13, 13,
	 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 
	 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14,
	 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 
	 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 
	 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 
	 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15
	};
	int day[ndays]	= {350, 7,13,43,55,83,97,104,112,118,125,133,139,147,155,159,166,174,180,200,204,207,
	327,328,333,341,346,348, 22, 26, 27, 32, 42, 61, 63, 65, 66, 71, 73, 77, 89, 95, 98,103,104,
	109,110,111,118,119,124,131,133,138,144,146,152,166,180,181,182,184,
	343,347,361, 31, 32, 33, 35, 40, 45, 46, 60, 67, 72, 74, 80, 88, 97,102,110,114,122,124,130,134,136,
	137,138,139,141,144,151,153,154,156,157,158,160,165,166,169,171,172,174,175,176,177,179,1791,1792,180,1801,181,1811,182,185,
	 37, 39, 42, 45, 59, 67, 74, 77, 79, 80, 81, 83, 86, 87, 88, 89, 90, 96, 98,101,103,107,
	109,112,116,119,125,128,131,139,143,148,156,158,160,162,
	 31, 32, 35, 36, 38, 39, 40, 50, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 66, 67, 68, 69, 70,
	 73, 75, 77, 82, 83, 85, 86, 87, 88, 89, 90, 93, 95, 96, 99,106,107,108,109,111,115,116,117,118,120,
	 121,122,127,128,130,131,133,134,136,137,1371,138,141,143,145,146,148,149,150,153, 
//125 removed
	 154,156,158,160,161,164,166,172,174,175,178,
	 181,182,184,186,187
	};
	float gday[ndays]	= {-15, 7,13,43,55,83,97,104,112,118,125,133,139,147,155,159,166,174,180,200,204,207,
	327,328,333,341,346,348,387,391,392,397,407,426,428,430,431,436,438,442,454,460,463,468,469,
	474,475,476,483,484,489,496,498,503,509,511,517,531,545,546,547,549, 
	708,712,726,761,762,763,765,770,775,776,790,797,802,804,810,818,827,832,840,844,852,854,860,864,866,
	867,868,869,871,874,881,883,884,886,887,888,890,895,896,899,901,902,904,905,906,907,909,909.1,909.2,910,910.1,911,911.1,912,915,
	1132,1134,1137,1140,1154,1166,1173,1176,1178,1179,1180,1182,1185,1186,1187,1188,1189,1195,1197,1200,1202,1206,
	1208,1211,1215,1218,1224,1227,1230,1238,1242,1247,1255,1257,1259,1261,
	1495,1496,1499,1500,1502,1503,1504,1514,1516,1517,1518,1519,1520,1521,1522,1523,1524,1525,1526,1527,1530,1531,1532,1533,1534,
	1537,1539,1541,1546,1547,1549,1550,1551,1552,1553,1554,1557,1559,1560,1563,1570,1571,1572,1573,1575,1579,1580,1581,1582,1584,
	1585,1586,1591,1592,1594,1595,1597,1598,1600,1601,1601,1602,1605,1607,1609,1610,1612,1613,1614,1617,
//1589 removed
	1618,1620,1622,1624,1625,1628,1630,1636,1638,1639,1642,
	1645,1646,1648,1650,1651
	};
	float		lastday	= day[ndays-1];
	if (lastday>1000){ lastday	/= 10.; }
	float 	timetotalTOF[ndays]	= {0};
	float 	timetotalMTD[ndays]	= {0};
	TH1D*	hinfo[ndays];
	TH1D*	hrate_tray[ndays];
	TH1D*	htot[ndays];
	TH1D*	htot_mtd[30][ndays];
	TH2D*	hmtdstripsbl[200];				//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	TGraph *gglobalnoise	= new TGraph();
			gglobalnoise->SetMarkerStyle(20);
			gglobalnoise->SetMarkerColor(1);
			gglobalnoise->SetLineColor(1);
			gglobalnoise->SetName("gglobalnoise");
	TGraph *gglobalnoiseL	= new TGraph();
			gglobalnoiseL->SetMarkerStyle(20);
			gglobalnoiseL->SetMarkerColor(4);
			gglobalnoiseL->SetLineColor(4);
			gglobalnoiseL->SetName("gglobalnoiseL");
	TGraph *gglobalnoiseH	= new TGraph();
			gglobalnoiseH->SetMarkerStyle(20);
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

	//---- get MCW data (Run-11).....
	//
    ifstream    isc;
	string 		str;
    int			iline=0, ikeep=-1;
	int			iy,imo,id,ih,imi,is,ifrac,idaynum;
	UInt_t		starttime,thistime;
    float		tMCW;
    int 		num_tMCW[200]	= {0};
    float		avg_tMCW[200]	= {0};
    isc.open("./MCW.txt" ,ifstream::in);
    while (isc.good()){
		getline(isc,str); ++iline;
		sscanf(str.c_str(),"%d/%d/%d %d:%d:%d.%d %f",&imo,&id,&iy,&ih,&imi,&is,&ifrac,&tMCW);
 		TDatime*	da = new TDatime(iy,imo,id,ih,imi,is);
 		thistime	= da->Convert();
		if (iline==1){
			starttime	= thistime;
		}
		Bool_t keepit	= kTRUE;
		if (tMCW<20.){ keepit = kFALSE; }
		if (keepit){
			++ikeep;
			idaynum		= GetDayNum(imo,id);
			++num_tMCW[idaynum];
			avg_tMCW[idaynum]	+= tMCW;
			gMCW->SetPoint(ikeep,thistime-starttime,tMCW);
		}
		delete da;
    }
    int ndays_MCW	= 0;
	for (int i=1;i<200;i++){
		if (num_tMCW[i]){
			avg_tMCW[i]	/= (float)num_tMCW[i];
			gMCW_day->SetPoint(ndays_MCW,365+i,avg_tMCW[i]);	// run-11 offset!!!!
			//cout<<365+i<<" "<<avg_tMCW[i]<<endl;
			++ndays_MCW;
		}
	}
	//cout<<"Nlines="<<iline<<"\t Nkept="<<ikeep+1<<endl;
	//cout<<"starttime="<<starttime<<"\t stop="<<thistime<<"\t interval="<<thistime-starttime<<endl;
	//cout<<"Nbins="<<(thistime-starttime)/500<<endl;
	//
	//---- end MCW data....


	TLatex *thistext	= new TLatex();
		thistext->SetTextSize(0.175);
		thistext->SetNDC();
	int kNmtdstripsbl	= 0;
	int kFileID;
	for (int iday=0;iday<ndays;iday++){
		int	thisntot_mtd[30]	= {0};
		int thisday				= day[iday];
		if (day[iday]<10  ){ 
			sprintf(buf," /star/institutions/rice/llope/noise/noise_%d00%d.root",year[iday],day[iday]); 
			kFileID				= year[iday]*1000 + day[iday];
		} else if (day[iday]<100 ){ 
			sprintf(buf," /star/institutions/rice/llope/noise/noise_%d0%d.root", year[iday],day[iday]); 
			kFileID				= year[iday]*1000 + day[iday];
		} else if (day[iday]<1000){ 
			sprintf(buf," /star/institutions/rice/llope/noise/noise_%d%d.root",  year[iday],day[iday]); 
			kFileID				= year[iday]*1000 + day[iday];
		} else { 
			thisday	/= 10;
			kFileID				= year[iday]*1000 + thisday;
			sprintf(buf," /star/institutions/rice/llope/noise/noise_%d%d.root",  year[iday],thisday); 
		}
		cout<<"i="<<iday<<"\t y="<<year[iday]<<"\t d="<<day[iday]<<" kFileID="<<kFileID<<"\t"<<buf<<endl;
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
			//
			if (kFileID>=14042){
				hmtdstripsbl[kNmtdstripsbl]	= (TH2D*)gDirectory->Get("hmtdstripsbl");
					sprintf(buf,"hmtdstripsbl%d",kNmtdstripsbl);
					hmtdstripsbl[kNmtdstripsbl]->SetName(buf);
					hmtdstripsbl[kNmtdstripsbl]->SetDirectory(0);
				++kNmtdstripsbl;
				if (kNmtdstripsbl>=200){
					cout<<"...you need more indices for hmtdstripsbl[]....."<endl;
					exit(0);
				}
			}
			//
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
//			if (i==26)cout<<iday<<" "<<gday[iday]<<" "<<gnmtd[i]/mtd11area<<endl;
			if (thisntot_mtd[i]>50){
				gmeantot_mtd[i]->SetPoint(iday,gday[iday],ameantot_mtd[i]);
			}
		}
		//
	}
	cout<<"Found "<<kNmtdstripsbl<<" instances of hmtdstripsbl...."<<endl;

	gStyle->SetOptStat(0);
	gStyle->SetPadRightMargin(0.02);
	gStyle->SetPadTopMargin(0.02);
	gStyle->SetPadBottomMargin(0.09);
	gStyle->SetPadLeftMargin(0.09);
	gStyle->SetTitleOffset(0.90,"X");
	gStyle->SetTitleOffset(0.90,"Y");

	const int nmodes=15;
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
	modes[14]	= new TBox(1500,0,gday[ndays-1]+6,70); 	// recirc
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
	TH1* hdum1 = new TH1F("hdum1","hdum1",1,0,1); hdum1->SetFillStyle(1001); hdum1->SetFillColor(41);
	TH1* hdum2 = new TH1F("hdum2","hdum2",1,0,1); hdum2->SetFillStyle(1001); hdum2->SetFillColor(5);
	TH1* hdum3 = new TH1F("hdum3","hdum3",1,0,1); hdum3->SetFillStyle(1001); hdum3->SetFillColor(30);
	TH1* hdum4 = new TH1F("hdum4","hdum4",1,0,1); hdum4->SetFillStyle(1001); hdum4->SetFillColor(18);

 	++ican; sprintf(buf,"ccan%d",ican);
 	ccan[ican] = new TCanvas(buf,buf,0,0,700,500);
 	ccan[ican]->SetFillColor(10);
 	ccan[ican]->cd(); ccan[ican]->Divide(1,1,0.0001,0.0001);
		frame	=	gPad->DrawFrame(-21,0,(year[ndays-1]-11)*365+lastday+6,70);
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
	//
//  	++ican; sprintf(buf,"ccan%d",ican);
//  	ccan[ican] = new TCanvas(buf,buf,0,0,700,500);
//  	ccan[ican]->SetFillColor(10);
//  	ccan[ican]->cd(); ccan[ican]->Divide(1,1,0.0001,0.0001);
// 		frame	=	gPad->DrawFrame(1130,0,1270,8.5);
// 			frame->GetXaxis()->SetTitle("Run-13 day number (x=1095 is Jan 1, 2013)  ");
// 			frame->GetYaxis()->SetTitle("global noise rate per channel (Hz/cm^{2})");
// 		gglobalnoisecm2->Draw("PL");
// 		gglobalnoisecm2_mtd[25]->Draw("PL");
// 		gglobalnoisecm2_mtd[26]->Draw("PL");
// 		gglobalnoisecm2_mtd[27]->Draw("PL");
// 		TLegend *legc = new TLegend(0.13,0.93,0.94,0.98);
// 			legc->SetTextSize(0.04);
// 			legc->SetNColumns(4);
// 			legc->SetFillColor(0);
// 			//legc->SetFillStyle(0);
// 			legc->AddEntry(gglobalnoisecm2      ,"TOF","P");
// 			legc->AddEntry(gglobalnoisecm2_mtd[25],"MTD26","P");
// 			legc->AddEntry(gglobalnoisecm2_mtd[26],"MTD27","P");
// 			legc->AddEntry(gglobalnoisecm2_mtd[27],"MTD28","P");
// 			legc->Draw();
// 	ccan[ican]->Update(); ccan[ican]->cd();
// 	ccan[ican]->Print("summary_noise.ps");
// 	//
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
	//
//  	++ican; sprintf(buf,"ccan%d",ican);
//  	ccan[ican] = new TCanvas(buf,buf,0,0,700,500);
//  	ccan[ican]->SetFillColor(10);
//  	ccan[ican]->cd(); ccan[ican]->Divide(1,1,0.0001,0.0001);
// 		frame	=	gPad->DrawFrame(1130,0,1270,4);
// 			frame->GetXaxis()->SetTitle("Run-13 day number (x=1095 is Jan 1, 2013)  ");
// 			frame->GetYaxis()->SetTitle("global noise rate per channel (Hz/cm^{2})");
// 		TLegend *legd = new TLegend(0.13,0.80,0.94,0.98);
// 			legd->SetTextSize(0.03);
// 			legd->SetFillColor(0);
// 			legd->SetNColumns(5);
// 		for (int i=0;i<30;i++){
// 			if (activeBL[i]){
// 				gglobalnoisecm2_mtd[i]->Draw("PL");
// 				sprintf(buf,"BL %d",i+1);
// 				legd->AddEntry(gglobalnoisecm2_mtd[i],buf,"P");
// 			}
// 		}
// 		legd->Draw();
// 	ccan[ican]->Update(); ccan[ican]->cd();
// 	ccan[ican]->Print("summary_noise.ps");
	//
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

	//---- MCW
	//
//  	++ican; sprintf(buf,"ccan%d",ican);
//  	ccan[ican] = new TCanvas(buf,buf,0,0,700,500);
//  	ccan[ican]->SetFillColor(10);
//  	ccan[ican]->cd(); ccan[ican]->Divide(1,1,0.0001,0.0001);
// 		frame	=	gPad->DrawFrame(320,50,(year[ndays-1]-11)*365+lastday+6,70);
// 			frame->GetXaxis()->SetTitle("Run-11 day number");
// 			frame->GetYaxis()->SetTitle("MCW Temperature (def F)");
// 		gMCW_day->Draw("PL");
// 	ccan[ican]->Update(); ccan[ican]->cd();
// 	ccan[ican]->Print("summary_noise.ps");



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

	//---- tof tot page 1
//  	++ican; sprintf(buf,"ccan%d",ican);
//  	ccan[ican] = new TCanvas(buf,buf,0,0,700,500);
//  	ccan[ican]->SetFillColor(10);
//  	ccan[ican]->cd(); ccan[ican]->Divide(9,8,0.0001,0.0001);
// 	for (int iday=0;iday<72;iday++){
// 		ccan[ican]->cd(1+iday);
// 		htot[iday]->SetFillStyle(1001);
// 		htot[iday]->SetFillColor(3);
// 		htot[iday]->Draw();
// 		sprintf(buf,"%d %d",year[iday],day[iday]);
// 		thistext->DrawLatex(0.59,0.8,buf);
// 	}
// 	ccan[ican]->Update(); ccan[ican]->cd();
// 	ccan[ican]->Print("summary_noise.ps");
// 	++ican; sprintf(buf,"ccan%d",ican);
// 	ccan[ican] = new TCanvas(buf,buf,0,0,700,500);
// 	ccan[ican]->SetFillColor(10);
// 	ccan[ican]->cd(); ccan[ican]->Divide(9,8,0.0001,0.0001);
// 	for (int iday=72;iday<ndays;iday++){
// 		ccan[ican]->cd(1+iday-72);
// 		htot[iday]->SetFillStyle(1001);
// 		htot[iday]->SetFillColor(3);
// 		htot[iday]->Draw();
// 		sprintf(buf,"%d %d",year[iday],day[iday]);
// 		thistext->DrawLatex(0.59,0.8,buf);
// 	}
// 	ccan[ican]->Update(); ccan[ican]->cd();
// 	ccan[ican]->Print("summary_noise.ps");
// 	//
// 	int mtdiday0	= 65;
// 	int mtdndays	= ndays-mtdiday0+1;
// 	//
//  	++ican; sprintf(buf,"ccan%d",ican);
//  	ccan[ican] = new TCanvas(buf,buf,0,0,700,500);
//  	ccan[ican]->SetFillColor(10);
//  	ccan[ican]->cd(); ccan[ican]->Divide(9,8,0.0001,0.0001);
// 	for (int iday=mtdiday0;iday<mtdiday0+72;iday++){
// 		ccan[ican]->cd(1+iday-mtdiday0);
// 		htot_mtd[25][iday]->SetFillStyle(1001);
// 		htot_mtd[25][iday]->SetFillColor(kcolors_bl[25]);
// 		htot_mtd[25][iday]->Draw();
// 		sprintf(buf,"%d %d",year[iday],day[iday]);
// 		thistext->DrawLatex(0.59,0.8,buf);
// 	}
// 	ccan[ican]->Update(); ccan[ican]->cd();
// 	ccan[ican]->Print("summary_noise.ps");
// 	++ican; sprintf(buf,"ccan%d",ican);
// 	ccan[ican] = new TCanvas(buf,buf,0,0,700,500);
// 	ccan[ican]->SetFillColor(10);
// 	ccan[ican]->cd(); ccan[ican]->Divide(9,8,0.0001,0.0001);
// 	for (int iday=mtdiday0+72;iday<ndays;iday++){
// 		ccan[ican]->cd(1+iday-(mtdiday0+72));
// 		htot_mtd[25][iday]->SetFillStyle(1001);
// 		htot_mtd[25][iday]->SetFillColor(kcolors_bl[25]);
// 		htot_mtd[25][iday]->Draw();
// 		sprintf(buf,"%d %d",year[iday],day[iday]);
// 		thistext->DrawLatex(0.59,0.8,buf);
// 	}
// 	ccan[ican]->Update(); ccan[ican]->cd();
// 	ccan[ican]->Print("summary_noise.ps");
// 


// 	const int nhv	 		=  7;
// 	const int valhv[nhv]	= {7000,6900,6800,6600,6400,6200,7000}; 
// 	const int valrun[nhv]	= {  34,  37,  45,  52,  54,  55,  59}; 
// 
// 	TH1D*	hinfo_hv[nhv];
// 	TH1D*	htot_hv[nhv];
// 	TGraph *gglobalnoise_hv	= new TGraph();
// 			gglobalnoise_hv->SetMarkerStyle(20);
// 			gglobalnoise_hv->SetMarkerColor(1);
// 			gglobalnoise_hv->SetLineColor(1);
// 			gglobalnoise_hv->SetName("gglobalnoise");
// 	TGraph *gglobalnoiseL_hv	= new TGraph();
// 			gglobalnoiseL_hv->SetMarkerStyle(20);
// 			gglobalnoiseL_hv->SetMarkerColor(4);
// 			gglobalnoiseL_hv->SetLineColor(4);
// 			gglobalnoiseL_hv->SetName("gglobalnoiseL");
// 	TGraph *gglobalnoiseH_hv	= new TGraph();
// 			gglobalnoiseH_hv->SetMarkerStyle(20);
// 			gglobalnoiseH_hv->SetMarkerColor(2);
// 			gglobalnoiseH_hv->SetLineColor(2);
// 			gglobalnoiseH_hv->SetName("gglobalnoiseH");
// 	TH1D*	hrate_tray_hv[nhv];
// 	TH1D*	hrate_tray_totH_hv[nhv];
// 	TH1D*	hrate_tray_totL_hv[nhv];
// 
// 	for (int ihv=0;ihv<nhv;ihv++){
// 		sprintf(buf,"noise_111120%d.root",valrun[ihv]);
// 		cout<<ihv<<" "<<valhv[ihv]<<" "<<buf<<endl;
// 		TFile *fin = new TFile(buf,"READ");
// 			fin->cd();
// 			hinfo_hv[ihv]	= (TH1D*)gDirectory->Get("hinfo");
// 				sprintf(buf,"hinfo_hv%d",ihv);
// 				hinfo_hv[ihv]->SetName(buf);
// 			htot_hv[ihv]	= (TH1D*)gDirectory->Get("htot");
// 				sprintf(buf,"htot_hv%d",ihv);
// 				htot_hv[ihv]->SetName(buf);
// 				sprintf(buf,"htot, HV=%d",valhv[ihv]);
// 				htot_hv[ihv]->SetTitle(buf);
// 			hrate_tray_hv[ihv]	= (TH1D*)gDirectory->Get("hrate_tray");
// 				sprintf(buf,"hrate_tray_hv%d",ihv);
// 				hrate_tray_hv[ihv]->SetName(buf);
// 				sprintf(buf,"hrate_tray, HV=%d",valhv[ihv]);
// 				hrate_tray_hv[ihv]->SetTitle(buf);
// 			hrate_tray_totH_hv[ihv]	= (TH1D*)gDirectory->Get("hrate_tray_totH");
// 				sprintf(buf,"hrate_tray_totH_hv%d",ihv);
// 				hrate_tray_totH_hv[ihv]->SetName(buf);
// 				sprintf(buf,"hrate_tray_totH, HV=%d",valhv[ihv]);
// 				hrate_tray_totH_hv[ihv]->SetTitle(buf);
// 			hrate_tray_totL_hv[ihv]	= (TH1D*)gDirectory->Get("hrate_tray_totL");
// 				sprintf(buf,"hrate_tray_totL_hv%d",ihv);
// 				hrate_tray_totL_hv[ihv]->SetName(buf);
// 				sprintf(buf,"hrate_tray_totL, HV=%d",valhv[ihv]);
// 				hrate_tray_totL_hv[ihv]->SetTitle(buf);
// 		fin->Close();
// 		//
// 		float gnle	= hinfo_hv[ihv]->GetBinContent(3)/23040.;
// 		float gnte	= hinfo_hv[ihv]->GetBinContent(4)/23040.;
// 		float gn	= hinfo_hv[ihv]->GetBinContent(5)/23040.;
// 		float gnL	= hinfo_hv[ihv]->GetBinContent(6)/23040.;
// 		float gnH	= hinfo_hv[ihv]->GetBinContent(7)/23040.;
// 		gglobalnoise_hv->SetPoint(ihv,valhv[ihv],gn);
// 		gglobalnoiseL_hv->SetPoint(ihv,valhv[ihv],gnL);
// 		gglobalnoiseH_hv->SetPoint(ihv,valhv[ihv],gnH);
// 	}
//
//  	++ican; sprintf(buf,"ccan%d",ican);
//  	ccan[ican] = new TCanvas(buf,buf,0,0,700,500);
//  	ccan[ican]->SetFillColor(10);
//  	ccan[ican]->cd(); ccan[ican]->Divide(1,1,0.0001,0.0001);
// 		frame	=	gPad->DrawFrame(6100,0,7100,70);
// 			frame->GetXaxis()->SetTitle("HV (V)");
// 			frame->GetYaxis()->SetTitle("Day 112 global noise rate per channel (Hz)");
// 		gglobalnoise_hv->Draw("P");
// 		gglobalnoiseL_hv->Draw("P");
// 		gglobalnoiseH_hv->Draw("P");
// 	ccan[ican]->Update(); ccan[ican]->cd();
// 	ccan[ican]->Print("summary_noise.ps");
// 
//  	++ican; sprintf(buf,"ccan%d",ican);
//  	ccan[ican] = new TCanvas(buf,buf,0,0,700,500);
//  	ccan[ican]->SetFillColor(10);
//  	ccan[ican]->cd(); ccan[ican]->Divide(3,3,0.0001,0.0001);
// 	for (int ihv=0;ihv<nhv;ihv++){
// 		ccan[ican]->cd(1+ihv);
// 		hrate_tray_hv[ihv]->SetStats(0);
// 		hrate_tray_totH_hv[ihv]->SetStats(0);
// 		hrate_tray_totL_hv[ihv]->SetStats(0);
// 		hrate_tray_hv[ihv]->Draw();
// 		hrate_tray_totH_hv[ihv]->Draw("same");
// 		hrate_tray_totL_hv[ihv]->Draw("same");
// 	}
// 	ccan[ican]->Update(); ccan[ican]->cd();
// 	ccan[ican]->Print("summary_noise.ps");


	gStyle->SetPadRightMargin(0.1);
	gStyle->SetPadTopMargin(0.065);
	gStyle->SetPadBottomMargin(0.04);
	gStyle->SetPadLeftMargin(0.06);
	gStyle->SetPadGridY(0);
	TLine* lmodules[4];
	for (int il=0;il<4;il++){
		lmodules[il]	= new TLine(-0.5,((il+1)*24)-0.5,14.5,((il+1)*24)-0.5);
	}
	for (int i=0;i<kNmtdstripsbl;i++){
		++ican; sprintf(buf,"ccan%d",ican);
		ccan[ican] = new TCanvas(buf,buf,0,0,700,500);
		ccan[ican]->SetFillColor(10);
		ccan[ican]->cd(); ccan[ican]->Divide(1,1,0.0001,0.0001);
		ccan[ican]->cd(1);
			gPad->SetLogz(1);
			hmtdstripsbl[i]->SetMaximum(5000);
			hmtdstripsbl[i]->SetMinimum( 0.5);
			hmtdstripsbl[i]->Draw("colz");
			for (int il=0;il<4;il++){
				lmodules[il]->Draw();
			}
		ccan[ican]->Update(); ccan[ican]->cd();
		ccan[ican]->Print("summary_noise.ps");
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
	sprintf(buf,"/usr/bin/ps2pdf %s.ps %s.pdf","summary_noise","summary_noise");
	cout<<"..... Executing ... "<<buf<<endl;
	gSystem->Exec(buf);
	sprintf(buf,"/bin/cp %s.pdf /star/u/llope/WWW/files/","summary_noise");
	cout<<"..... Executing ... "<<buf<<endl;
	gSystem->Exec(buf);
	cout<<"..... Done...."<<endl;	
	
}

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

