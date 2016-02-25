
const float tofarea =  20.;
const float mtd9area = 218.;
const float mtd11area = 331.;

const float TRAYCUTOFF = 4.;

void summary_run12(){

TH1::AddDirectory(kFALSE);

	gErrorIgnoreLevel	= 2000;

	char		buf[100];
	char		tbuf[100];
	TCanvas*	ccan[1000];
	int			ican	= -1;

	const int 	ndays	=  48;
	int year[ndays]	= { 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13,
	 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13,
	 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13
	};
	int day[ndays]	= { 31, 32, 33, 35, 40, 45, 46, 60, 67, 72, 74, 80, 88, 97,102,
	110,114,122,124,130,134,136,137,138,139,141,144,151,153,154,156,157,158,160,165,
	166,169,171,172,174,175,176,177,179,180,181,182,185
	};
	int	gday[ndays]	= {761,762,763,765,770,775,776,790,797,802,804,810,818,827,832,
	840,844,852,854,860,864,866,867,868,869,871,874,881,883,884,886,887,888,890,895,
	896,899,901,902,904,905,906,907,909,910,911,912,915
	};
	float 	thisthres;
	float 	timetotalTOF[ndays]	= {0};
	float 	timetotalMTD[ndays]	= {0};

	//---------------------------------------------------------------------------
	int ndaysuse	= ndays;		// just for testing...
//	int ndaysuse	= 31;		// to 156
	//---------------------------------------------------------------------------

	TLine *lthr2500a		= new TLine(846.0,0.0,846.0,7.99);
		 lthr2500a->SetLineColor(1);
	TLatex *thr2500a	= new TLatex(845.9,4,"Threshold 2500#rightarrow1200");
		thr2500a->SetTextSize(0.03);
		thr2500a->SetTextAngle(90);
	TLine *lthr2500b		= new TLine(846.0,17.01,846.0,25.99);
		 lthr2500a->SetLineColor(1);
	TLatex *thr2500b	= new TLatex(845.9,22.0,"Threshold 2500#rightarrow1200");
		thr2500b->SetTextSize(0.03);
		thr2500b->SetTextAngle(90);

	TLine *lgasdira		= new TLine(866.0,0.0,866.0,7.99);
		 lgasdira->SetLineColor(1);
	TLatex *gasdira	= new TLatex(865.9,2,"Gas Direction Revision");
		gasdira->SetTextSize(0.03);
		gasdira->SetTextAngle(90);
	TLine *lgasdirb		= new TLine(866.0,17.01,866.0,25.99);
		 lgasdira->SetLineColor(1);
	TLatex *gasdirb	= new TLatex(865.9,11.0,"Gas Direction Revision");
		gasdirb->SetTextSize(0.03);
		gasdirb->SetTextAngle(90);
	
	TH1D*	hinfo[ndays];
	TH1D*	hrate_tray[ndays];
	TH1D*	htot[ndays];
	TH1D*	htot_mtd26[ndays];
	TH1D*	htot_mtd27[ndays];
	TH1D*	htot_mtd28[ndays];
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
			gglobalnoisecm2->SetLineColor(1);
			gglobalnoisecm2->SetName("gglobalnoisecm2");
	TGraph *gglobalnoisecm2_mtd26	= new TGraph();
			gglobalnoisecm2_mtd26->SetMarkerStyle(20);
			gglobalnoisecm2_mtd26->SetMarkerColor(6);
			gglobalnoisecm2_mtd26->SetLineColor(1);
			gglobalnoisecm2_mtd26->SetName("gglobalnoisecm2_mtd26");
	TGraph *gglobalnoisecm2_mtd27	= new TGraph();
			gglobalnoisecm2_mtd27->SetMarkerStyle(20);
			gglobalnoisecm2_mtd27->SetMarkerColor(4);
			gglobalnoisecm2_mtd27->SetLineColor(1);
			gglobalnoisecm2_mtd27->SetName("gglobalnoisecm2_mtd27");
	TGraph *gglobalnoisecm2_mtd28	= new TGraph();
			gglobalnoisecm2_mtd28->SetMarkerStyle(20);
			gglobalnoisecm2_mtd28->SetMarkerColor(7);
			gglobalnoisecm2_mtd28->SetLineColor(1);
			gglobalnoisecm2_mtd28->SetName("gglobalnoisecm2_mtd28");

	TGraph *gmeantot		= new TGraph();
			gmeantot->SetMarkerStyle(20);
			gmeantot->SetMarkerColor(3);
			gmeantot->SetLineColor(3);
			gmeantot->SetName("gmeantot");
	TGraph *gmeantot_mtd26	= new TGraph();
			gmeantot_mtd26->SetMarkerStyle(20);
			gmeantot_mtd26->SetMarkerColor(6);
			gmeantot_mtd26->SetLineColor(6);
			gmeantot_mtd26->SetName("gmeantot_mtd26");
	TGraph *gmeantot_mtd27	= new TGraph();
			gmeantot_mtd27->SetMarkerStyle(20);
			gmeantot_mtd27->SetMarkerColor(4);
			gmeantot_mtd27->SetLineColor(4);
			gmeantot_mtd27->SetName("gmeantot_mtd27");
	TGraph *gmeantot_mtd28	= new TGraph();
			gmeantot_mtd28->SetMarkerStyle(20);
			gmeantot_mtd28->SetMarkerColor(7);
			gmeantot_mtd28->SetLineColor(7);
			gmeantot_mtd28->SetName("gmeantot_mtd28");

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
		thistext->SetTextSize(0.08);
		thistext->SetNDC();
	TLatex *thistextb	= new TLatex();
		thistextb->SetTextSize(0.066);
		thistextb->SetNDC();

	int	thisntot_mtd26;
	int	thisntot_mtd27;
	int	thisntot_mtd28;
	TH1D 	*hmtdcell[5][30][ndays];
	TH2D	*hmtdhitz_strip_zoff427[ndays];
	TGraph	*gmtdmodavg_vsday[5][30];
	TGraph	*gmtdmodavg_vseta[ndays][30];
	TGraph	*gmtdmodavg_vseta_ratprev[ndays][30];
	int kmarkereta[5]	= {20,21,22,23,34};
	TF1 *fmtdcell = new TF1("fmtdcell","pol0",-0.5,23.5);
	double lpar[1];
	for (int ibl=25;ibl<=27;ibl++){
		for (int iet=0;iet<5;iet++){
			gmtdmodavg_vsday[iet][ibl]	= new TGraph();
				gmtdmodavg_vsday[iet][ibl]->SetMarkerStyle(kmarkereta[iet]);
				if (ibl+1==26){ gmtdmodavg_vsday[iet][ibl]->SetMarkerColor(6); gmtdmodavg_vsday[iet][ibl]->SetLineColor(6); } else
				if (ibl+1==27){ gmtdmodavg_vsday[iet][ibl]->SetMarkerColor(4); gmtdmodavg_vsday[iet][ibl]->SetLineColor(4); } else
				if (ibl+1==28){ gmtdmodavg_vsday[iet][ibl]->SetMarkerColor(7); gmtdmodavg_vsday[iet][ibl]->SetLineColor(7); }
		}
		for (int iday=0;iday<ndaysuse;iday++){
			gmtdmodavg_vseta[iday][ibl]	= new TGraph();
				gmtdmodavg_vseta[iday][ibl]->SetMarkerStyle(20);
				if (ibl+1==26){ gmtdmodavg_vseta[iday][ibl]->SetMarkerColor(6); gmtdmodavg_vseta[iday][ibl]->SetLineColor(6); } else
				if (ibl+1==27){ gmtdmodavg_vseta[iday][ibl]->SetMarkerColor(4); gmtdmodavg_vseta[iday][ibl]->SetLineColor(4); } else
				if (ibl+1==28){ gmtdmodavg_vseta[iday][ibl]->SetMarkerColor(7); gmtdmodavg_vseta[iday][ibl]->SetLineColor(7); }
			gmtdmodavg_vseta_ratprev[iday][ibl]	= new TGraph();
				gmtdmodavg_vseta_ratprev[iday][ibl]->SetMarkerStyle(20);
				if (ibl+1==26){ gmtdmodavg_vseta_ratprev[iday][ibl]->SetMarkerColor(6); gmtdmodavg_vseta_ratprev[iday][ibl]->SetLineColor(6); } else
				if (ibl+1==27){ gmtdmodavg_vseta_ratprev[iday][ibl]->SetMarkerColor(4); gmtdmodavg_vseta_ratprev[iday][ibl]->SetLineColor(4); } else
				if (ibl+1==28){ gmtdmodavg_vseta_ratprev[iday][ibl]->SetMarkerColor(7); gmtdmodavg_vseta_ratprev[iday][ibl]->SetLineColor(7); }
		}
	}
	for (int iday=0;iday<ndaysuse;iday++){
		if (day[iday]<10  ){ sprintf(buf,"/star/institutions/rice/llope/noise/noise_%d00%d.root",year[iday],day[iday]); } else
		if (day[iday]<100 ){ sprintf(buf,"/star/institutions/rice/llope/noise/noise_%d0%d.root", year[iday],day[iday]); } else
		if (day[iday]<1000){ sprintf(buf,"/star/institutions/rice/llope/noise/noise_%d%d.root",  year[iday],day[iday]); } 
		if (day[iday]<852){ thisthres = 2500; }else{ thisthres = 1200; }
		cout<<"Opening "<<buf<<endl;
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
				sprintf(tbuf,"ToT TOF, Day=%d",day[iday]);
				if (htot[iday]){
					htot[iday]->SetName(buf);
					htot[iday]->SetTitle(buf);
				} else {
					htot[iday]	= new TH1D(buf,tbuf,160,0.0,40.0);
				}
			htot_mtd26[iday]	= (TH1D*)gDirectory->Get("htot_mtd26");
				sprintf(buf,"htot_mtd26%d",iday);
				sprintf(tbuf,"ToT MTD26, Day=%d, Thresh=%d",day[iday],thisthres);
				if (htot_mtd26[iday]){
					htot_mtd26[iday]->SetName(buf);
					htot_mtd26[iday]->SetTitle(tbuf);
					thisntot_mtd26	= htot_mtd26[iday]->GetEntries();
				} else {
					htot_mtd26[iday]	= new TH1D(buf,tbuf,160,0.0,40.0);
				}
			htot_mtd27[iday]	= (TH1D*)gDirectory->Get("htot_mtd27");
				sprintf(buf,"htot_mtd27%d",iday);
				sprintf(tbuf,"ToT MTD27, Day=%d, Thresh=%d",day[iday],thisthres);
				if (htot_mtd27[iday]){
					htot_mtd27[iday]->SetName(buf);
					htot_mtd27[iday]->SetTitle(tbuf);
					thisntot_mtd27	= htot_mtd27[iday]->GetEntries();
				} else {
					htot_mtd27[iday]	= new TH1D(buf,tbuf,160,0.0,40.0);
				}
			htot_mtd28[iday]	= (TH1D*)gDirectory->Get("htot_mtd28");
				sprintf(buf,"htot_mtd28%d",iday);
				sprintf(tbuf,"ToT MTD28, Day=%d, Thresh=%d",day[iday],thisthres);
				if (htot_mtd28[iday]){
					htot_mtd28[iday]->SetName(buf);
					htot_mtd28[iday]->SetTitle(tbuf);
					thisntot_mtd28	= htot_mtd28[iday]->GetEntries();
				} else {
					htot_mtd28[iday]	= new TH1D(buf,tbuf,160,0.0,40.0);
				}
			//
			for (int ibl=25;ibl<=27;ibl++){
				for (int iet=0;iet<5;iet++){
					sprintf(buf,"hmtdcell%d%d",iet,ibl);
					hmtdcell[iet][ibl][iday]	= (TH1D*)gDirectory->Get(buf);
					sprintf(buf,"hmtdcell%d%d%d",iet,ibl,iday);	
					hmtdcell[iet][ibl][iday]->SetName(buf);
					sprintf(buf,"Backleg=%d, Posn=%d, Day=%d",ibl+1,iet+1,day[iday]);	
					hmtdcell[iet][ibl][iday]->SetTitle(buf);
					hmtdcell[iet][ibl][iday]->SetDirectory(0);
					//
					hmtdcell[iet][ibl][iday]->SetFillStyle(1001);
					hmtdcell[iet][ibl][iday]->SetFillColor(5);
					//
					//fmtdcell->SetParameter(0,0);
					//hmtdcell[iet][ibl][iday]->Fit("fmtdcell","QN0");
					//fmtdcell->GetParameters(lpar);
					//
					lpar[0] = GetMeanY(hmtdcell[iet][ibl][iday]);
					//
					//cout<<iday<<" "<<day[iday]<<"....... "<<ibl+1<<" "<<iet+1<<" "<<lpar[0]<<endl;
					gmtdmodavg_vsday[iet][ibl]->SetPoint(iday,day[iday],lpar[0]);
					//
					double adum,prevval;
					if (iday>=1 && lpar[0]>10){
						int kkk1 = iet;
						if (ibl==25){ kkk1 = iet-1; }
						gmtdmodavg_vseta[iday-1][ibl]->GetPoint(kkk1,adum,prevval);
						//cout<<" get "<<kkk1<<"\t"<<ibl<<" "<<iday<<" "<<iet<<" "<<prevval<<" "<<lpar[0]<<endl;
					}
					if (lpar[0]>10){
						int kkk = gmtdmodavg_vseta[iday][ibl]->GetN();
						//cout<<" set "<<kkk<<"\t"<<ibl<<" "<<iday<<" "<<iet<<" "<<prevval<<" "<<lpar[0]<<endl;
						gmtdmodavg_vseta[iday][ibl]->SetPoint(kkk,iet+1,lpar[0]);
						if (iday>=1){
							int kkk2 = gmtdmodavg_vseta_ratprev[iday][ibl]->GetN();
							gmtdmodavg_vseta_ratprev[iday][ibl]->SetPoint(kkk2,iet+1,lpar[0]/prevval);
						}
					}
					//
				}
			}
			//
			sprintf(buf,"hmtdhitz_strip_zoff427");
			hmtdhitz_strip_zoff427[iday]	= (TH2D*)gDirectory->Get(buf);
			sprintf(buf,"hmtdhitz_strip_zoff427%d",iday);
			hmtdhitz_strip_zoff427[iday]->SetName(buf);	
			//hmtdhitz_strip_zoff427[iday]->SetDirectory(0);	
			sprintf(buf,"MTD Zhit vs strip BL=28, pos=5, iday=%d",day[iday]);
			hmtdhitz_strip_zoff427[iday]->SetTitle(buf);	
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
		float gnmtd26	= hinfo[iday]->GetBinContent(16)/72.;
		float gnmtd27	= hinfo[iday]->GetBinContent(17)/120.;
		float gnmtd28	= hinfo[iday]->GetBinContent(18)/120.;
		float loopslo	=0.;
		float loopint	=0.;
		float ameantot	=0.;
		float ameantot26=0.;
		float ameantot27=0.;
		float ameantot28=0.;
		if (hinfo[iday]->GetNbinsX()>25){
			loopint		= hinfo[iday]->GetBinContent(21);
			loopslo		= hinfo[iday]->GetBinContent(22);
			ameantot	= hinfo[iday]->GetBinContent(23);
			ameantot26	= hinfo[iday]->GetBinContent(24);
			ameantot27	= hinfo[iday]->GetBinContent(25);
			ameantot28	= hinfo[iday]->GetBinContent(26);
		}
// 		cout<<"i="<<iday<<"\t y="<<year[iday]<<"\t d="<<day[iday]<<"\t D="<<gday[iday]
// 			<<"\t ntray="<<ntrays<<"\t rate: "<<gnT<<" "<<gnmtd26<<" "<<gnmtd27<<" "<<gnmtd28
// 			<<"\t loop a,b :"<<loopint<<" "<<loopslo
// 			<<"\t <ToT> :"<<ameantot<<" "<<ameantot26<<" "<<ameantot27<<" "<<ameantot28
// 			<<endl;
		gglobalnoise->SetPoint(iday,gday[iday],gn);
		gglobalnoiseL->SetPoint(iday,gday[iday],gnL);
		gglobalnoiseH->SetPoint(iday,gday[iday],gnH);
		gloopfitpara->SetPoint(iday,gday[iday],loopint);
		gloopfitparb->SetPoint(iday,gday[iday],loopslo);
		gglobalnoisecm2->SetPoint(iday,gday[iday],gn/tofarea);
		gglobalnoisecm2_mtd26->SetPoint(iday,gday[iday],gnmtd26/mtd11area);
		gglobalnoisecm2_mtd27->SetPoint(iday,gday[iday],gnmtd27/mtd11area);
		gglobalnoisecm2_mtd28->SetPoint(iday,gday[iday],gnmtd28/mtd11area);
		gmeantot->SetPoint(iday,gday[iday],ameantot);
		if (thisntot_mtd26>50){
			gmeantot_mtd26->SetPoint(iday,gday[iday],ameantot26);
		}
		if (thisntot_mtd27>50){
			gmeantot_mtd27->SetPoint(iday,gday[iday],ameantot27);
		}
		if (thisntot_mtd28>50){
			gmeantot_mtd28->SetPoint(iday,gday[iday],ameantot28);
		}
		//
	}

	gStyle->SetOptStat(0);
	gStyle->SetPadRightMargin(0.02);
	gStyle->SetPadTopMargin(0.00);
	gStyle->SetPadBottomMargin(0.09);
	gStyle->SetPadLeftMargin(0.10);
	gStyle->SetTitleOffset(0.90,"X");
	gStyle->SetTitleOffset(0.90,"Y");
	gStyle->SetPalette(1);

	const int nmodes=10;
	TBox *modes[nmodes];
	modes[0]	= new TBox(-21,0,113,70); 
	modes[1]	= new TBox(113,0,147,70); 
	modes[2]	= new TBox(147,0,210,70);
	modes[3]	= new TBox(210,0,264,70);
	modes[4]	= new TBox(264,0,315,70);
	modes[5]	= new TBox(315,0,553,70);
	//run-12
	modes[6]	= new TBox(553,0,701,70);	// n2
	modes[7]	= new TBox(701,0,727,70);   // freon prechristmas
	modes[8]	= new TBox(727,0,756,70);   // off, holidays
	modes[9]	= new TBox(757,0,gday[ndays-1]+6,70); // freon, run-12
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
	TH1* hdum1 = new TH1F("hdum1","hdum1",1,0,1); hdum1->SetFillStyle(1001); hdum1->SetFillColor(41);
	TH1* hdum2 = new TH1F("hdum2","hdum2",1,0,1); hdum2->SetFillStyle(1001); hdum2->SetFillColor(5);
	TH1* hdum3 = new TH1F("hdum3","hdum3",1,0,1); hdum3->SetFillStyle(1001); hdum3->SetFillColor(30);
	TH1* hdum4 = new TH1F("hdum4","hdum4",1,0,1); hdum4->SetFillStyle(1001); hdum4->SetFillColor(18);

 	++ican; sprintf(buf,"ccan%d",ican);
 	ccan[ican] = new TCanvas(buf,buf,0,0,700,500);
 	ccan[ican]->SetFillColor(10);
 	ccan[ican]->cd(); ccan[ican]->Divide(1,1,0.0001,0.0001);
		frame	=	gPad->DrawFrame(755,0,(year[ndays-1]-11)*365+day[ndays-1]+6,39);
			frame->GetXaxis()->SetTitle("day number");
			frame->GetYaxis()->SetTitle("global noise rate per channel (Hz)");
		//for (int i=0;i<nmodes;i++){ modes[i]->Draw(); }
		gPad->RedrawAxis();
		gglobalnoise->Draw("LP");
		gglobalnoiseL->Draw("LP");
		gglobalnoiseH->Draw("LP");
		TLegend *lega = new TLegend(0.17,0.73,0.435,0.975);
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
	ccan[ican]->Print("summary_run12.ps(");

 	++ican; sprintf(buf,"ccan%d",ican);
 	ccan[ican] = new TCanvas(buf,buf,0,0,700,500);
 	ccan[ican]->SetFillColor(10);
 	ccan[ican]->cd(); ccan[ican]->Divide(1,2,0.0001,0.0001);
	ccan[ican]->cd(1);
		frame	=	gPad->DrawFrame(755,0.0,(year[ndays-1]-11)*365+day[ndays-1]+6,25);
			frame->GetXaxis()->SetTitle("day number");
			frame->GetYaxis()->SetTitle("ModuleInLoop, Intercept (Hz/cell)");
		gloopfitpara->Draw("P");
	ccan[ican]->cd(2);
		frame	=	gPad->DrawFrame(755,0.0,(year[ndays-1]-11)*365+day[ndays-1]+6,0.20);
			frame->GetXaxis()->SetTitle("day number");
			frame->GetYaxis()->SetTitle("ModuleInLoop, Slope ([Hz/cell]/MRPCinLoop)");
		gloopfitparb->Draw("P");
	ccan[ican]->Update(); ccan[ican]->cd();
	ccan[ican]->Print("summary_run12.ps");

//  	++ican; sprintf(buf,"ccan%d",ican);
//  	ccan[ican] = new TCanvas(buf,buf,0,0,700,500);
//  	ccan[ican]->SetFillColor(10);
//  	ccan[ican]->cd(); ccan[ican]->Divide(1,1,0.0001,0.0001);
// 		frame	=	gPad->DrawFrame(755,0,(year[ndays-1]-11)*365+day[ndays-1]+6,4);
// 			frame->GetXaxis()->SetTitle("day number");
// 			frame->GetYaxis()->SetTitle("global noise rate per channel (Hz/cm^{2})");
// 		gglobalnoisecm2->Draw("P");
// 		gglobalnoisecm2_mtd26->Draw("P");
// 		gglobalnoisecm2_mtd27->Draw("P");
// 		gglobalnoisecm2_mtd28->Draw("P");
// 		TLegend *legb = new TLegend(0.56,0.81,0.95,0.97);
// 			legb->SetTextSize(0.04);
// 			legb->SetFillStyle(0);
// 			legb->AddEntry(gglobalnoisecm2      ,"TOF, 20 cm^{2}/pad","P");
// 			legb->AddEntry(gglobalnoisecm2_mtd26,"MTD26, 331 cm^{2}/pad","P");
// 			legb->AddEntry(gglobalnoisecm2_mtd27,"MTD27, 331 cm^{2}/pad","P");
// 			legb->AddEntry(gglobalnoisecm2_mtd28,"MTD28, 331 cm^{2}/pad","P");
// 			legb->Draw();
// 	ccan[ican]->Update(); ccan[ican]->cd();
// 	ccan[ican]->Print("summary_run12.ps");
	//
 	++ican; sprintf(buf,"ccan%d",ican);
 	ccan[ican] = new TCanvas(buf,buf,0,0,700,500);
 	ccan[ican]->SetFillColor(10);
 	ccan[ican]->cd(); ccan[ican]->Divide(2,1,0.0001,0.0001);
 	ccan[ican]->cd(1);
		frame	=	gPad->DrawFrame(755,0,(year[ndays-1]-11)*365+day[ndays-1]+6,7.99);
			frame->GetXaxis()->SetTitle("Run-12 day number (x=730 is Jan 1, 2012)  ");
			frame->GetYaxis()->SetTitle("global noise rate per channel (Hz/cm^{2})");
			frame->GetYaxis()->SetTitleOffset(1.2);
		gglobalnoisecm2->SetLineColor(3);
		gglobalnoisecm2_mtd26->SetLineColor(6);
		gglobalnoisecm2_mtd27->SetLineColor(4);
		gglobalnoisecm2_mtd28->SetLineColor(7);
		gglobalnoisecm2->Draw("PL");
		gglobalnoisecm2_mtd26->Draw("PL");
		gglobalnoisecm2_mtd27->Draw("PL");
		gglobalnoisecm2_mtd28->Draw("PL");
		lthr2500a->Draw("same");
		thr2500a->Draw("same");
		lgasdira->Draw("same");
		gasdira->Draw("same");
		TLegend *legc = new TLegend(0.16,0.81,0.71,0.97);
			legc->SetTextSize(0.04);
			legc->SetFillStyle(0);
			legc->AddEntry(gglobalnoisecm2      ,"TOF, 20 cm^{2}/pad","P");
			legc->AddEntry(gglobalnoisecm2_mtd26,"MTD26, 331 cm^{2}/pad","P");
			legc->AddEntry(gglobalnoisecm2_mtd27,"MTD27, 331 cm^{2}/pad","P");
			legc->AddEntry(gglobalnoisecm2_mtd28,"MTD28, 331 cm^{2}/pad","P");
			legc->Draw();
	ccan[ican]->cd(2);
			frame	=	gPad->DrawFrame(755,17.01,(year[ndays-1]-11)*365+day[ndays-1]+6,25.99);
			frame->GetXaxis()->SetTitle("Run-12 day number (x=730 is Jan 1, 2012)  ");
			frame->GetYaxis()->SetTitle("#LTTot#GT");
			frame->GetYaxis()->SetTitleOffset(1.2);
		gmeantot->Draw("PL");
		gmeantot_mtd26->Draw("PL");
		gmeantot_mtd27->Draw("PL");
		gmeantot_mtd28->Draw("PL");
		lthr2500b->Draw("same");
		thr2500b->Draw("same");
		lgasdirb->Draw("same");
		gasdirb->Draw("same");
	ccan[ican]->Update(); ccan[ican]->cd();
	ccan[ican]->Print("summary_run12.ps");

	//---- MCW
	//
//  	++ican; sprintf(buf,"ccan%d",ican);
//  	ccan[ican] = new TCanvas(buf,buf,0,0,700,500);
//  	ccan[ican]->SetFillColor(10);
//  	ccan[ican]->cd(); ccan[ican]->Divide(1,1,0.0001,0.0001);
// 		frame	=	gPad->DrawFrame(320,50,(year[ndays-1]-11)*365+day[ndays-1]+6,70);
// 			frame->GetXaxis()->SetTitle("Run-11 day number");
// 			frame->GetYaxis()->SetTitle("MCW Temperature (def F)");
// 		gMCW_day->Draw("PL");
// 	ccan[ican]->Update(); ccan[ican]->cd();
// 	ccan[ican]->Print("summary_run12.ps");

//  	++ican; sprintf(buf,"ccan%d",ican);
//  	ccan[ican] = new TCanvas(buf,buf,0,0,700,500);
//  	ccan[ican]->SetFillColor(10);
//  	ccan[ican]->cd(); ccan[ican]->Divide(1,1,0.0001,0.0001);
// 		frame	=	gPad->DrawFrame(755,17.01,(year[ndays-1]-11)*365+day[ndays-1]+6,26.99);
// 			frame->GetXaxis()->SetTitle("day number");
// 			frame->GetYaxis()->SetTitle("#LTTot#GT");
// 		gmeantot->Draw("PL");
// 		gmeantot_mtd26->Draw("PL");
// 		gmeantot_mtd27->Draw("PL");
// 		gmeantot_mtd28->Draw("PL");
// 		TLegend *legd = new TLegend(0.56,0.81,0.95,0.97);
// 			legd->SetTextSize(0.04);
// 			legd->SetFillStyle(0);
// 			legd->AddEntry(gmeantot_mtd26,"MTD26","P");
// 			legd->AddEntry(gmeantot_mtd27,"MTD27","P");
// 			legd->AddEntry(gmeantot_mtd28,"MTD28","P");
// 			legd->AddEntry(gmeantot      ,"TOF","P");
// 			legd->Draw();
// 	ccan[ican]->Update(); ccan[ican]->cd();
// 	ccan[ican]->Print("summary_run12.ps");

	//---- tof tot page 1
 	++ican; sprintf(buf,"ccan%d",ican);
 	ccan[ican] = new TCanvas(buf,buf,0,0,700,500);
 	ccan[ican]->SetFillColor(10);
 	ccan[ican]->cd(); ccan[ican]->Divide(7,7,0.0001,0.0001);
	for (int iday=0;iday<ndaysuse;iday++){
		ccan[ican]->cd(1+iday);
		htot[iday]->SetFillStyle(1001);
		htot[iday]->SetFillColor(3);
		htot[iday]->Draw();
		sprintf(buf,"%d %d",year[iday],day[iday]);
		thistext->DrawLatex(0.69,0.85,buf);
		sprintf(buf,"thresh=1200");
		thistextb->DrawLatex(0.64,0.76,buf);
	}
	ccan[ican]->Update(); ccan[ican]->cd();
	ccan[ican]->Print("summary_run12.ps");
	//
// 	if (ndays>72){
// 		++ican; sprintf(buf,"ccan%d",ican);
// 		ccan[ican] = new TCanvas(buf,buf,0,0,700,500);
// 		ccan[ican]->SetFillColor(10);
// 		ccan[ican]->cd(); ccan[ican]->Divide(5,4,0.0001,0.0001);
// 		for (int iday=0;iday<ndaysuse;iday++){
// 			ccan[ican]->cd(1+iday);
// 			htot[iday]->SetFillStyle(1001);
// 			htot[iday]->SetFillColor(3);
// 			htot[iday]->Draw();
// 			sprintf(buf,"%d %d",year[iday],day[iday]);
// 			thistext->DrawLatex(0.69,0.85,buf);
// 			sprintf(buf,"thresh=1200");
// 			thistextb->DrawLatex(0.66,0.76,buf);
// 		}
// 		ccan[ican]->Update(); ccan[ican]->cd();
// 		ccan[ican]->Print("summary_run12.ps");
// 	}
	//
 	++ican; sprintf(buf,"ccan%d",ican);
 	ccan[ican] = new TCanvas(buf,buf,0,0,700,500);
 	ccan[ican]->SetFillColor(10);
 	ccan[ican]->cd(); ccan[ican]->Divide(7,7,0.0001,0.0001);
	for (int iday=0;iday<ndaysuse;iday++){
		ccan[ican]->cd(1+iday);
		htot_mtd26[iday]->SetFillStyle(1001);
		htot_mtd26[iday]->SetFillColor(6);
		htot_mtd26[iday]->Draw();
		sprintf(buf,"%d %d",year[iday],day[iday]);
		thistext->DrawLatex(0.69,0.85,buf);
		if (day[iday]<852){ thisthres = 2500; }else{ thisthres = 1200; }
		sprintf(buf,"thresh=%d",thisthres);
		thistextb->DrawLatex(0.64,0.76,buf);
	}
	ccan[ican]->Update(); ccan[ican]->cd();
	ccan[ican]->Print("summary_run12.ps");
	//
 	++ican; sprintf(buf,"ccan%d",ican);
 	ccan[ican] = new TCanvas(buf,buf,0,0,700,500);
 	ccan[ican]->SetFillColor(10);
 	ccan[ican]->cd(); ccan[ican]->Divide(7,7,0.0001,0.0001);
	for (int iday=0;iday<ndaysuse;iday++){
		ccan[ican]->cd(1+iday);
		htot_mtd27[iday]->SetFillStyle(1001);
		htot_mtd27[iday]->SetFillColor(4);
		htot_mtd27[iday]->Draw();
		sprintf(buf,"%d %d",year[iday],day[iday]);
		thistext->DrawLatex(0.69,0.85,buf);
		if (day[iday]<852){ thisthres = 2500; }else{ thisthres = 1200; }
		sprintf(buf,"thresh=%d",thisthres);
		thistextb->DrawLatex(0.64,0.76,buf);
	}
	ccan[ican]->Update(); ccan[ican]->cd();
	ccan[ican]->Print("summary_run12.ps");
	//
 	++ican; sprintf(buf,"ccan%d",ican);
 	ccan[ican] = new TCanvas(buf,buf,0,0,700,500);
 	ccan[ican]->SetFillColor(10);
 	ccan[ican]->cd(); ccan[ican]->Divide(7,7,0.0001,0.0001);
	for (int iday=0;iday<ndaysuse;iday++){
		ccan[ican]->cd(1+iday);
		htot_mtd28[iday]->SetFillStyle(1001);
		htot_mtd28[iday]->SetFillColor(7);
		htot_mtd28[iday]->Draw();
		sprintf(buf,"%d %d",year[iday],day[iday]);
		thistext->DrawLatex(0.69,0.85,buf);
		if (day[iday]<852){ thisthres = 2500; }else{ thisthres = 1200; }
		sprintf(buf,"thresh=%d",thisthres);
		thistextb->DrawLatex(0.64,0.76,buf);
	}
	ccan[ican]->Update(); ccan[ican]->cd();
	ccan[ican]->Print("summary_run12.ps");


	TLine *vert	= new TLine();
		vert->SetLineWidth(3);
		vert->SetLineColor(17);
		
	gStyle->SetPadTopMargin(0.01);
	gSystem->Exec("rm -v summary_run12_tot.gif");
	for (int iday=0;iday<ndaysuse;iday++){
	 	++ican; sprintf(buf,"ccan%d",ican);
		ccan[ican] = new TCanvas(buf,buf,0,0,1100.,(8.5/11.)*1100.);
		ccan[ican]->SetFillColor(10);
		ccan[ican]->cd(); ccan[ican]->Divide(2,2,0.0001,0.0001);
		ccan[ican]->cd(1);
			htot_mtd26[iday]->Draw();
		ccan[ican]->cd(2);
			htot_mtd27[iday]->Draw();
		ccan[ican]->cd(3);
			htot_mtd28[iday]->Draw();
		ccan[ican]->cd(4);
			frame	=	gPad->DrawFrame(755,0,(year[ndays-1]-11)*365+day[ndays-1]+6,7.99);
				frame->GetXaxis()->SetTitle("Run-12 day number (x=730 is Jan 1, 2012)  ");
				frame->GetYaxis()->SetTitle("global noise rate per channel (Hz/cm^{2})");
			lthr2500a->Draw("same");
			thr2500a->Draw("same");
			lgasdira->Draw("same");
			gasdira->Draw("same");
			vert->DrawLine(gday[iday],0,gday[iday],7.99);			
			gglobalnoisecm2_mtd26->Draw("PL");
			gglobalnoisecm2_mtd27->Draw("PL");
			gglobalnoisecm2_mtd28->Draw("PL");
		ccan[ican]->Update(); ccan[ican]->cd();
		ccan[ican]->Print("summary_run12_tot.gif+50");	
	}

	gStyle->SetPadLeftMargin(0.12);
	gSystem->Exec("rm -v summary_run12_modrate.gif");
	for (int iday=0;iday<ndaysuse;iday++){
		++ican; sprintf(buf,"ccan%d",ican);
		ccan[ican] = new TCanvas(buf,buf,0,0,1100.,1100.*(8.5/11.));
		ccan[ican]->SetFillColor(10);
		ccan[ican]->cd(); ccan[ican]->Divide(5,3,0.0001,0.0001);
			for (int i=0;i<5;i++){
				ccan[ican]->cd(1+i);
				gPad->SetLogy(1);
				hmtdcell[i][28-1][iday]->SetMinimum(   0.5);
				hmtdcell[i][28-1][iday]->SetMaximum(10000.0);
				hmtdcell[i][28-1][iday]->Draw();
				hmtdcell[i][28-1][iday]->GetYaxis()->SetLabelSize(0.08);
				gPad->Update();
			}
			for (int i=0;i<5;i++){
				ccan[ican]->cd(6+i);
				gPad->SetLogy(1);
				hmtdcell[i][27-1][iday]->SetMinimum(   0.5);
				hmtdcell[i][27-1][iday]->SetMaximum(10000.0);
				hmtdcell[i][27-1][iday]->Draw();
				hmtdcell[i][27-1][iday]->GetYaxis()->SetLabelSize(0.08);
				gPad->Update();
			}
			//
			ccan[ican]->cd(11);
				sprintf(buf,"Day=%d",day[iday]);
				thistext->SetTextSize(0.12);			
				thistext->DrawLatex(0.2,0.6,buf);			
				if (day[iday]<852){ thisthres = 2500; }else{ thisthres = 1200; }
				sprintf(buf,"Threshold = %d",thisthres);
				thistextb->SetTextSize(0.10);			
				thistextb->DrawLatex(0.11,0.50,buf);			
			//
			for (int i=1;i<4;i++){
				ccan[ican]->cd(11+i);
				gPad->SetLogy(1);
				hmtdcell[i][26-1][iday]->SetMinimum(   0.5);
				hmtdcell[i][26-1][iday]->SetMaximum(10000.0);
				hmtdcell[i][26-1][iday]->Draw();
				hmtdcell[i][26-1][iday]->GetYaxis()->SetLabelSize(0.08);
				gPad->Update();
			}
			//
			ccan[ican]->cd(15);
			frame	=	gPad->DrawFrame(755,0,(year[ndays-1]-11)*365+day[ndays-1]+6,7.99);
				//frame->GetXaxis()->SetTitle("Run-12 day number (x=730 is Jan 1, 2012)  ");
				//frame->GetYaxis()->SetTitle("global noise rate per channel (Hz/cm^{2})");
			lthr2500a->Draw("same");
			thr2500a->Draw("same");
			lgasdira->Draw("same");
			gasdira->Draw("same");
			vert->DrawLine(gday[iday],0,gday[iday],7.99);			
			gglobalnoisecm2_mtd26->Draw("PL");
			gglobalnoisecm2_mtd27->Draw("PL");
			gglobalnoisecm2_mtd28->Draw("PL");
			//
		ccan[ican]->cd(); ccan[ican]->Update();
		ccan[ican]->Print("summary_run12_modrate.gif+50");	
	}

	gStyle->SetPadGridX(0);
	gStyle->SetPadGridY(0);

	thistext->SetTextSize(0.076);
	++ican; sprintf(buf,"ccan%d",ican);
	ccan[ican] = new TCanvas(buf,buf,0,0,900.,(8.5/11.)*900.);
	ccan[ican]->SetFillColor(10);
	ccan[ican]->cd(); ccan[ican]->Divide(1,3,0.0001,0.0001);
	ccan[ican]->cd(1);
		gPad->SetLogy(1);
		frame	=	gPad->DrawFrame(25,1,day[ndays-1]+5,5000);
			frame->GetXaxis()->SetTitle("day");
			frame->GetYaxis()->SetTitle("rate (Hz)");
			frame->GetXaxis()->SetLabelSize(0.07);
			frame->GetYaxis()->SetLabelSize(0.07);
			frame->GetXaxis()->SetTitleSize(0.00001);
			frame->GetYaxis()->SetTitleSize(0.07);
			frame->GetYaxis()->SetTitleOffset(0.4);
		for (int iet=0;iet<5;iet++){
			gmtdmodavg_vsday[iet][27]->Draw("PL");			
		}
		TLegend *lege = new TLegend(0.37,0.68,0.44,0.99);
			lege->SetTextSize(0.06);
			lege->SetFillStyle(0);
			for (int iet=0;iet<5;iet++){
				lege->AddEntry(gmtdmodavg_vsday[iet][27],Form("ieta=%d",iet+1),"P");
			}
			lege->Draw("same");
		//thistext->DrawLatex(0.8,0.2,"Flow: 5#rightarrow1");
	ccan[ican]->cd(2);
		gPad->SetLogy(1);
		frame	=	gPad->DrawFrame(25,1,day[ndays-1]+5,5000);
			frame->GetXaxis()->SetTitle("day");
			frame->GetYaxis()->SetTitle("rate (Hz)");
			frame->GetXaxis()->SetLabelSize(0.07);
			frame->GetYaxis()->SetLabelSize(0.07);
			frame->GetXaxis()->SetTitleSize(0.00001);
			frame->GetYaxis()->SetTitleSize(0.07);
			frame->GetYaxis()->SetTitleOffset(0.4);
		for (int iet=0;iet<5;iet++){
			gmtdmodavg_vsday[iet][26]->Draw("PL");			
		}
		//thistext->DrawLatex(0.8,0.2,"Flow: 1#rightarrow5");
	ccan[ican]->cd(3);
		gPad->SetLogy(1);
		frame	=	gPad->DrawFrame(25,1,day[ndays-1]+5,5000);
			frame->GetXaxis()->SetTitle("day");
			frame->GetYaxis()->SetTitle("rate (Hz)");
			frame->GetXaxis()->SetLabelSize(0.07);
			frame->GetYaxis()->SetLabelSize(0.07);
			frame->GetXaxis()->SetTitleSize(0.00001);
			frame->GetYaxis()->SetTitleSize(0.07);
			frame->GetYaxis()->SetTitleOffset(0.4);
		for (int iet=0;iet<5;iet++){
			gmtdmodavg_vsday[iet][25]->Draw("PL");			
		}
		//thistext->DrawLatex(0.8,0.2,"Flow: 4#rightarrow2");
	ccan[ican]->Update(); ccan[ican]->cd();
	ccan[ican]->Print("summary_run12.ps");
	//
	thistext->SetTextSize(0.076);
	++ican; sprintf(buf,"ccan%d",ican);
	ccan[ican] = new TCanvas(buf,buf,0,0,900.,(8.5/11.)*900.);
	ccan[ican]->SetFillColor(10);
	ccan[ican]->cd(); ccan[ican]->Divide(1,3,0.0001,0.0001);
	ccan[ican]->cd(1);
		gPad->SetGridx(1);
		gPad->SetGridy(1);
		frame	=	gPad->DrawFrame(25,1,day[ndays-1]+5,3499);
			frame->GetXaxis()->SetTitle("day");
			frame->GetYaxis()->SetTitle("rate (Hz)");
			frame->GetXaxis()->SetLabelSize(0.07);
			frame->GetYaxis()->SetLabelSize(0.06);
			frame->GetXaxis()->SetTitleSize(0.00001);
			frame->GetYaxis()->SetTitleSize(0.07);
			frame->GetYaxis()->SetTitleOffset(0.4);
		for (int iet=0;iet<5;iet++){
			gmtdmodavg_vsday[iet][27]->Draw("PL");			
		}
		TLegend *lege = new TLegend(0.37,0.68,0.44,0.99);
			lege->SetTextSize(0.06);
			lege->SetFillStyle(0);
			for (int iet=0;iet<5;iet++){
				lege->AddEntry(gmtdmodavg_vsday[iet][27],Form("ieta=%d",iet+1),"P");
			}
			lege->Draw("same");
		//thistext->DrawLatex(0.8,0.2,"Flow: 5#rightarrow1");
	ccan[ican]->cd(2);
		gPad->SetGridx(1);
		gPad->SetGridy(1);
		frame	=	gPad->DrawFrame(25,1,day[ndays-1]+5,2999);
			frame->GetXaxis()->SetTitle("day");
			frame->GetYaxis()->SetTitle("rate (Hz)");
			frame->GetXaxis()->SetLabelSize(0.07);
			frame->GetYaxis()->SetLabelSize(0.06);
			frame->GetXaxis()->SetTitleSize(0.00001);
			frame->GetYaxis()->SetTitleSize(0.07);
			frame->GetYaxis()->SetTitleOffset(0.4);
		for (int iet=0;iet<5;iet++){
			gmtdmodavg_vsday[iet][26]->Draw("PL");			
		}
		//thistext->DrawLatex(0.8,0.2,"Flow: 1#rightarrow5");
	ccan[ican]->cd(3);
		gPad->SetGridx(1);
		gPad->SetGridy(1);
		frame	=	gPad->DrawFrame(25,1,day[ndays-1]+5,1499);
			frame->GetXaxis()->SetTitle("day");
			frame->GetYaxis()->SetTitle("rate (Hz)");
			frame->GetXaxis()->SetLabelSize(0.07);
			frame->GetYaxis()->SetLabelSize(0.06);
			frame->GetXaxis()->SetTitleSize(0.00001);
			frame->GetYaxis()->SetTitleSize(0.07);
			frame->GetYaxis()->SetTitleOffset(0.4);
		for (int iet=0;iet<5;iet++){
			gmtdmodavg_vsday[iet][25]->Draw("PL");			
		}
		//thistext->DrawLatex(0.8,0.2,"Flow: 4#rightarrow2");
	ccan[ican]->Update(); ccan[ican]->cd();
	ccan[ican]->Print("summary_run12.ps");
	

	//---- hitZ by strip 427
	gStyle->SetPadRightMargin(0.02);
	gStyle->SetPadTopMargin(0.01);
	gStyle->SetPadBottomMargin(0.09);
	gStyle->SetPadLeftMargin(0.10);
	gSystem->Exec("rm -v summary_run12_zhitstrip.gif");
	for (int iday=0;iday<ndaysuse;iday++){
		float time	= timetotalMTD[iday];
		cout<<iday<<" "<<day[iday]<<" "<<time<<endl;
	 	++ican; sprintf(buf,"ccan%d",ican);
		ccan[ican] = new TCanvas(buf,buf,0,0,1200.,800.);
		ccan[ican]->SetFillColor(10);
		ccan[ican]->cd(); ccan[ican]->Divide(2,1,0.0001,0.0001);
		ccan[ican]->cd(1);
			gPad->SetLogz(1);
			gPad->SetTopMargin(0.06);
			hmtdhitz_strip_zoff427[iday]->Scale(1./time);
			hmtdhitz_strip_zoff427[iday]->GetYaxis()->SetRangeUser(-5.2,5.2);
			hmtdhitz_strip_zoff427[iday]->Draw("col");
		ccan[ican]->cd(2);
			frame	=	gPad->DrawFrame(755,0,(year[ndays-1]-11)*365+day[ndays-1]+6,7.99);
				frame->GetXaxis()->SetTitle("Run-12 day number (x=730 is Jan 1, 2012)  ");
				frame->GetYaxis()->SetTitle("global noise rate per channel (Hz/cm^{2})");
			lthr2500a->Draw("same");
			thr2500a->Draw("same");
			lgasdira->Draw("same");
			gasdira->Draw("same");
			vert->DrawLine(gday[iday],0,gday[iday],7.99);			
			gglobalnoisecm2_mtd28->Draw("PL");
		ccan[ican]->Update(); ccan[ican]->cd();
		ccan[ican]->Print("summary_run12_zhitstrip.gif+50");	
	}

	int istrip;

	//---- hitZ by strip 427, strip 1
	TH1D *hmtdhitz_strip_zoff427_strip1[ndays];	istrip=1;
	gStyle->SetPadRightMargin(0.02);
	gStyle->SetPadTopMargin(0.01);
	gStyle->SetPadBottomMargin(0.09);
	gStyle->SetPadLeftMargin(0.10);
	gSystem->Exec("rm -v summary_run12_zhitstrip1.gif");
	for (int iday=0;iday<ndaysuse;iday++){
		float time	= timetotalMTD[iday];
		//cout<<iday<<" "<<day[iday]<<" "<<time<<endl;
	 	++ican; sprintf(buf,"ccan%d",ican);
		ccan[ican] = new TCanvas(buf,buf,0,0,1200.,800.);
		ccan[ican]->SetFillColor(10);
		ccan[ican]->cd(); ccan[ican]->Divide(2,1,0.0001,0.0001);
		ccan[ican]->cd(1);
			gPad->SetLogy(1);
			gPad->SetTopMargin(0.06);
			hmtdhitz_strip_zoff427_strip1[iday] = (TH1D*)hmtdhitz_strip_zoff427[iday]->ProjectionY(Form("hmtdhitz_strip_zoff427_strip1%d",iday),istrip,istrip);
			hmtdhitz_strip_zoff427_strip1[iday]->Scale(1./time);
			hmtdhitz_strip_zoff427_strip1[iday]->SetMinimum(0.0005);
			hmtdhitz_strip_zoff427_strip1[iday]->SetMaximum(10.0);
			hmtdhitz_strip_zoff427_strip1[iday]->GetXaxis()->SetRangeUser(-5.2,5.2);
			hmtdhitz_strip_zoff427_strip1[iday]->Draw();
		ccan[ican]->cd(2);
			frame	=	gPad->DrawFrame(755,0,(year[ndays-1]-11)*365+day[ndays-1]+6,7.99);
				frame->GetXaxis()->SetTitle("Run-12 day number (x=730 is Jan 1, 2012)  ");
				frame->GetYaxis()->SetTitle("global noise rate per channel (Hz/cm^{2})");
			lthr2500a->Draw("same");
			thr2500a->Draw("same");
			lgasdira->Draw("same");
			gasdira->Draw("same");
			vert->DrawLine(gday[iday],0,gday[iday],7.99);			
			gglobalnoisecm2_mtd28->Draw("PL");
		ccan[ican]->Update(); ccan[ican]->cd();
		ccan[ican]->Print("summary_run12_zhitstrip1.gif+50");	
	}
	//---- hitZ by strip 427, strip 6
	TH1D *hmtdhitz_strip_zoff427_strip6[ndays];	istrip=6;
	gStyle->SetPadRightMargin(0.02);
	gStyle->SetPadTopMargin(0.01);
	gStyle->SetPadBottomMargin(0.09);
	gStyle->SetPadLeftMargin(0.10);
	gSystem->Exec("rm -v summary_run12_zhitstrip6.gif");
	for (int iday=0;iday<ndaysuse;iday++){
		float time	= timetotalMTD[iday];
		//cout<<iday<<" "<<day[iday]<<" "<<time<<endl;
	 	++ican; sprintf(buf,"ccan%d",ican);
		ccan[ican] = new TCanvas(buf,buf,0,0,1200.,800.);
		ccan[ican]->SetFillColor(10);
		ccan[ican]->cd(); ccan[ican]->Divide(2,1,0.0001,0.0001);
		ccan[ican]->cd(1);
			gPad->SetLogy(1);
			gPad->SetTopMargin(0.06);
			hmtdhitz_strip_zoff427_strip6[iday] = (TH1D*)hmtdhitz_strip_zoff427[iday]->ProjectionY(Form("hmtdhitz_strip_zoff427_strip6%d",iday),istrip,istrip);
			hmtdhitz_strip_zoff427_strip6[iday]->Scale(1./time);
			hmtdhitz_strip_zoff427_strip6[iday]->SetMinimum(0.0005);
			hmtdhitz_strip_zoff427_strip6[iday]->SetMaximum(10.0);
			hmtdhitz_strip_zoff427_strip6[iday]->GetXaxis()->SetRangeUser(-5.2,5.2);
			hmtdhitz_strip_zoff427_strip6[iday]->Draw();
		ccan[ican]->cd(2);
			frame	=	gPad->DrawFrame(755,0,(year[ndays-1]-11)*365+day[ndays-1]+6,7.99);
				frame->GetXaxis()->SetTitle("Run-12 day number (x=730 is Jan 1, 2012)  ");
				frame->GetYaxis()->SetTitle("global noise rate per channel (Hz/cm^{2})");
			lthr2500a->Draw("same");
			thr2500a->Draw("same");
			lgasdira->Draw("same");
			gasdira->Draw("same");
			vert->DrawLine(gday[iday],0,gday[iday],7.99);			
			gglobalnoisecm2_mtd28->Draw("PL");
		ccan[ican]->Update(); ccan[ican]->cd();
		ccan[ican]->Print("summary_run12_zhitstrip6.gif+50");	
	}
	//---- hitZ by strip 427, strip 12
	TH1D *hmtdhitz_strip_zoff427_strip12[ndays];	istrip=12;
	gStyle->SetPadRightMargin(0.02);
	gStyle->SetPadTopMargin(0.01);
	gStyle->SetPadBottomMargin(0.09);
	gStyle->SetPadLeftMargin(0.10);
	gSystem->Exec("rm -v summary_run12_zhitstrip12.gif");
	for (int iday=0;iday<ndaysuse;iday++){
		float time	= timetotalMTD[iday];
		//cout<<iday<<" "<<day[iday]<<" "<<time<<endl;
	 	++ican; sprintf(buf,"ccan%d",ican);
		ccan[ican] = new TCanvas(buf,buf,0,0,1200.,800.);
		ccan[ican]->SetFillColor(10);
		ccan[ican]->cd(); ccan[ican]->Divide(2,1,0.0001,0.0001);
		ccan[ican]->cd(1);
			gPad->SetLogy(1);
			gPad->SetTopMargin(0.06);
			hmtdhitz_strip_zoff427_strip12[iday] = (TH1D*)hmtdhitz_strip_zoff427[iday]->ProjectionY(Form("hmtdhitz_strip_zoff427_strip12%d",iday),istrip,istrip);
			hmtdhitz_strip_zoff427_strip12[iday]->Scale(1./time);
			hmtdhitz_strip_zoff427_strip12[iday]->SetMinimum(0.0005);
			hmtdhitz_strip_zoff427_strip12[iday]->SetMaximum(10.0);
			hmtdhitz_strip_zoff427_strip12[iday]->GetXaxis()->SetRangeUser(-5.2,5.2);
			hmtdhitz_strip_zoff427_strip12[iday]->Draw();
		ccan[ican]->cd(2);
			frame	=	gPad->DrawFrame(755,0,(year[ndays-1]-11)*365+day[ndays-1]+6,7.99);
				frame->GetXaxis()->SetTitle("Run-12 day number (x=730 is Jan 1, 2012)  ");
				frame->GetYaxis()->SetTitle("global noise rate per channel (Hz/cm^{2})");
			lthr2500a->Draw("same");
			thr2500a->Draw("same");
			lgasdira->Draw("same");
			gasdira->Draw("same");
			vert->DrawLine(gday[iday],0,gday[iday],7.99);			
			gglobalnoisecm2_mtd28->Draw("PL");
		ccan[ican]->Update(); ccan[ican]->cd();
		ccan[ican]->Print("summary_run12_zhitstrip12.gif+50");	
	}

	TLegend *legeta	= new TLegend(0.14,0.1,0.51,0.24);
		legeta->SetTextSize(0.05);
		legeta->SetFillColor(0);
	gStyle->SetPadTopMargin(0.01);
	gStyle->SetPadLeftMargin(0.12);
	gSystem->Exec("rm -v summary_run12_mtdmodavg_vseta.gif");
	for (int iday=0;iday<ndaysuse;iday++){
	 	++ican; sprintf(buf,"ccan%d",ican);
		ccan[ican] = new TCanvas(buf,buf,0,0,1100.,(8.5/11.)*1100.);
		ccan[ican]->SetFillColor(10);
		ccan[ican]->cd(); ccan[ican]->Divide(3,1,0.0001,0.0001);
		ccan[ican]->cd(1);
			frame	=	gPad->DrawFrame(0.8,3.0,5.2,9999.);
				frame->GetXaxis()->SetTitle("eta position  ");
				frame->GetYaxis()->SetTitle("avg. rate/module (Hz)   ");
			//frame->GetXaxis()->SetNdivisions(5,kFALSE);
			gPad->SetLogy(1);
			gmtdmodavg_vseta[iday][25]->Draw("PL");
			gmtdmodavg_vseta[iday][26]->Draw("PL");
			gmtdmodavg_vseta[iday][27]->Draw("PL");
			if (iday==0){
				legeta->AddEntry(gmtdmodavg_vseta[iday][27],"Backleg 28","PL");
				legeta->AddEntry(gmtdmodavg_vseta[iday][26],"Backleg 27","PL");
				legeta->AddEntry(gmtdmodavg_vseta[iday][25],"Backleg 26","PL");
			}
			legeta->Draw("same");
		ccan[ican]->cd(2);
			frame	=	gPad->DrawFrame(0.8,0.1,5.2,10);
				frame->GetXaxis()->SetTitle("eta position  ");
				frame->GetYaxis()->SetTitle("Ratio wrt previous measurement  ");
			//frame->GetXaxis()->SetNdivisions(5,kFALSE);
			gPad->SetLogy(1);
			if (iday>=1){
				gmtdmodavg_vseta_ratprev[iday][25]->Draw("PL");
				gmtdmodavg_vseta_ratprev[iday][26]->Draw("PL");
				gmtdmodavg_vseta_ratprev[iday][27]->Draw("PL");
			}
		ccan[ican]->cd(3);
			frame	=	gPad->DrawFrame(755,0,(year[ndays-1]-11)*365+day[ndays-1]+6,7.99);
				frame->GetXaxis()->SetTitle("Run-12 day number (x=730 is Jan 1, 2012)  ");
				frame->GetYaxis()->SetTitle("global noise rate per channel (Hz/cm^{2})   ");
			lthr2500a->Draw("same");
			thr2500a->Draw("same");
			lgasdira->Draw("same");
			gasdira->Draw("same");
			vert->DrawLine(gday[iday],0,gday[iday],7.99);			
			gglobalnoisecm2_mtd26->Draw("PL");
			gglobalnoisecm2_mtd27->Draw("PL");
			gglobalnoisecm2_mtd28->Draw("PL");
		ccan[ican]->Update(); ccan[ican]->cd();
		ccan[ican]->Print("summary_run12_mtdmodavg_vseta.gif+50");	
	}


	//----- end of all processing......
	//
	cout<<"..... Writing summary.root "<<endl;
	TFile *f = new TFile("summary.root","RECREATE");
		f->cd();
		gMCW->Write();
		gMCW_day->Write();
		gglobalnoise->Write();
		gglobalnoiseL->Write();
		gglobalnoiseH->Write();
		gglobalnoisecm2->Write();
		gglobalnoisecm2_mtd26->Write();
		gglobalnoisecm2_mtd27->Write();
		gglobalnoisecm2_mtd28->Write();
		gmeantot->Write();
		gmeantot_mtd26->Write();
		gmeantot_mtd27->Write();
		gmeantot_mtd28->Write();
		for (int iday=0;iday<ndaysuse;iday++){
			htot[iday]->Write();
			htot_mtd26[iday]->Write();
			htot_mtd27[iday]->Write();
			htot_mtd28[iday]->Write();
		}
	f->Close();
	//
	ccan[ican]->Print("summary_run12.ps]");
	sprintf(buf,"/usr/bin/ps2pdf %s.ps %s.pdf","summary_run12","summary_run12");
	cout<<"..... Executing ... "<<buf<<endl;
	gSystem->Exec(buf);
	sprintf(buf,"/bin/cp %s.pdf /star/u/llope/WWW/files/","summary_run12");
	cout<<"..... Executing ... "<<buf<<endl;
	gSystem->Exec(buf);
	//
	gSystem->Exec("/bin/cp -v summary_run12_tot.gif /star/u/llope/WWW/files/");
	gSystem->Exec("/bin/cp -v summary_run12_modrate.gif /star/u/llope/WWW/files/");
	gSystem->Exec("/bin/cp -v summary_run12_zhitstrip.gif /star/u/llope/WWW/files/");
	gSystem->Exec("/bin/cp -v summary_run12_zhitstrip1.gif /star/u/llope/WWW/files/");
	gSystem->Exec("/bin/cp -v summary_run12_zhitstrip6.gif /star/u/llope/WWW/files/");
	gSystem->Exec("/bin/cp -v summary_run12_zhitstrip12.gif /star/u/llope/WWW/files/");
	gSystem->Exec("/bin/cp -v summary_run12_mtdmodavg_vseta.gif /star/u/llope/WWW/files/");
	//
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

Float_t GetMeanY(TH1D *h){
	int n		= h->GetNbinsX();
	double sum	= 0;
	for (int i=1;i<=n;i++){
		double y	 = h->GetBinContent(i);
		sum			+= y;
	}
	return sum/24.;
}