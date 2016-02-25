const float tofarea =  20.;
const float mtd9area = 218.;
const float mtd11area = 331.;

float GetAverage(TH1D* h);

void bgmtd(){

	char		buf[100];
	char		tbuf[100];
	TCanvas*	ccan[1000];
	TH1F*		frame[1000];
	TLatex*		text[1000];
	int			ican	= -1;
	int			iframe	= -1;
	int			itext	= -1;
	gStyle->SetPadLeftMargin(0.11);
	gStyle->SetPadTopMargin(0.03);
	gStyle->SetPadRightMargin(0.01);

	const int nfiles 				= 6;
	const int kColors[nfiles] 		= {7,4,2,6,1,3};
	const char *filenames[nfiles] 	= {
		"/star/institutions/rice/llope/noise/noise_13175.root",
		"/star/institutions/rice/llope/noise/noise_13176.root",
		"/star/institutions/rice/llope/noise/noise_13175029.root",
		"/star/institutions/rice/llope/noise/noise_13176010.root",
		"/star/institutions/rice/llope/noise/noise_13176026.root",
		"/star/institutions/rice/llope/noise/noise_13176042.root"
	};
	TH1D	*hinfo[nfiles];
	TH1D 	*hmtdcell[5][30][nfiles];
	TH1D 	*hmtdcellnorm[5][30][nfiles];
	TH1D 	*hmtdcelldiff[5][30][nfiles];
	float	timetotal[nfiles] = {0};
	for (int ifile=0;ifile<nfiles;ifile++){
		sprintf(buf,"%s",filenames[ifile]);
		cout<<"Reading "<<buf<<"\t";
		TFile *fin = new TFile(buf,"READ");
		//
		hinfo[ifile]	= (TH1D*)gDirectory->Get("hinfo");
			sprintf(buf,"hinfo%d",ifile);
			hinfo[ifile]->SetName(buf);
			hinfo[ifile]->SetDirectory(0);
		timetotal[ifile]	= hinfo[ifile]->GetBinContent(2);
		cout<<timetotal[ifile]<<endl;
		//
		for (int ibl=25;ibl<=27;ibl++){
			for (int iet=0;iet<5;iet++){
				//
				sprintf(buf,"hmtdcell%d%d",iet,ibl);
				hmtdcell[iet][ibl][ifile]	= (TH1D*)gDirectory->Get(buf);
				sprintf(buf,"hmtdcell%d%d%d",iet,ibl,ifile);	
				hmtdcell[iet][ibl][ifile]->SetName(buf);
				sprintf(buf,"Rate (Hz), BL=%d, Pos=%d",ibl+1,iet+1,ifile);	
				hmtdcell[iet][ibl][ifile]->SetTitle(buf);
				hmtdcell[iet][ibl][ifile]->SetDirectory(0);
				hmtdcell[iet][ibl][ifile]->SetLineColor(kColors[ifile]);
				hmtdcell[iet][ibl][ifile]->SetLineWidth(2);
				//
				sprintf(buf,"hmtdcell%d%d",iet,ibl);
				hmtdcellnorm[iet][ibl][ifile]	= (TH1D*)gDirectory->Get(buf);
				sprintf(buf,"hmtdcellnorm%d%d%d",iet,ibl,ifile);	
				hmtdcellnorm[iet][ibl][ifile]->SetName(buf);
				sprintf(buf,"Ratio, BL=%d, Pos=%d",ibl+1,iet+1,ifile);	
				hmtdcellnorm[iet][ibl][ifile]->SetTitle(buf);
				hmtdcellnorm[iet][ibl][ifile]->SetDirectory(0);
				hmtdcellnorm[iet][ibl][ifile]->SetLineColor(kColors[ifile]);
				hmtdcellnorm[iet][ibl][ifile]->SetLineWidth(2);
				//
				sprintf(buf,"hmtdcell%d%d",iet,ibl);
				hmtdcelldiff[iet][ibl][ifile]	= (TH1D*)gDirectory->Get(buf);
				sprintf(buf,"hmtdcelldiff%d%d%d",iet,ibl,ifile);	
				hmtdcelldiff[iet][ibl][ifile]->SetName(buf);
				sprintf(buf,"Difference (Hz), BL=%d, Pos=%d",ibl+1,iet+1,ifile);	
				hmtdcelldiff[iet][ibl][ifile]->SetTitle(buf);
				hmtdcelldiff[iet][ibl][ifile]->SetDirectory(0);
				hmtdcelldiff[iet][ibl][ifile]->SetLineColor(kColors[ifile]);
				hmtdcelldiff[iet][ibl][ifile]->SetLineWidth(2);
			}
		}
		fin->Close();
	}

	for (int ifile=0;ifile<nfiles;ifile++){
		for (int ibl=25;ibl<=27;ibl++){
			for (int iet=0;iet<5;iet++){
				hmtdcellnorm[iet][ibl][ifile]->Divide(hmtdcell[iet][ibl][0]);
				hmtdcelldiff[iet][ibl][ifile]->Add(hmtdcell[iet][ibl][0],-1.);
			}
		}
	}
	for (int it=0;it<1000;it++){
		text[it]	= new TLatex(0.0,0.0,"");
		text[it]->SetTextSize(0.08);
		text[it]->SetNDC();
	}

	TText *thisline[20];
	TPaveText *pave	= new TPaveText(0.01,0.30,0.98,0.85);
		pave->SetFillColor(0);
		thisline[5]	= pave->AddText("13176042, fill-late");	 	thisline[5]->SetTextColor(3); thisline[5]->SetTextSize(0.08);
		thisline[4]	= pave->AddText("13176026, fill-middle");	thisline[4]->SetTextColor(1); thisline[4]->SetTextSize(0.08);
		thisline[3]	= pave->AddText("13176010, fill-early");	thisline[3]->SetTextColor(6); thisline[3]->SetTextSize(0.08);
		thisline[2]	= pave->AddText("13175029, fill-early"); 	thisline[2]->SetTextColor(2); thisline[2]->SetTextSize(0.08);
		thisline[1]	= pave->AddText("Day 13176, no beam"); 		thisline[1]->SetTextColor(4); thisline[1]->SetTextSize(0.08);
		thisline[0]	= pave->AddText("Day 13175, no beam"); 		thisline[0]->SetTextColor(7); thisline[0]->SetTextSize(0.08);

	gStyle->SetTitleX(0.3);
	++ican; sprintf(buf,"ccan%d",ican);
	ccan[ican] = new TCanvas(buf,buf,0,0,1100.,1100.*(8.5/11.));
	ccan[ican]->SetFillColor(10);
	ccan[ican]->cd(); ccan[ican]->Divide(5,3,0.0001,0.0001);
		for (int i=0;i<5;i++){
			ccan[ican]->cd(1+i);
			gPad->SetLogy(1);
			++iframe; frame[iframe] = (TH1F*)gPad->DrawFrame(-0.5,18,23.5,20000.);
			frame[iframe]->SetTitle(hmtdcell[i][28-1][0]->GetTitle());
			for (int ifile=0;ifile<nfiles;ifile++){
				hmtdcell[i][28-1][ifile]->Draw("same");
			}
			frame[iframe]->GetYaxis()->SetLabelSize(0.07);
			frame[iframe]->GetXaxis()->SetLabelSize(0.06);
			gPad->Update();
		}
		for (int i=0;i<5;i++){
			ccan[ican]->cd(6+i);
			gPad->SetLogy(1);
			++iframe; frame[iframe] = (TH1F*)gPad->DrawFrame(-0.5,18,23.5,20000.);
			frame[iframe]->SetTitle(hmtdcell[i][27-1][0]->GetTitle());
			for (int ifile=0;ifile<nfiles;ifile++){
				hmtdcell[i][27-1][ifile]->Draw("same");
			}
			frame[iframe]->GetYaxis()->SetLabelSize(0.07);
			frame[iframe]->GetXaxis()->SetLabelSize(0.06);
			gPad->Update();
		}
		//
		ccan[ican]->cd(11);
		pave->Draw();
		//
		for (int i=1;i<4;i++){
			ccan[ican]->cd(11+i);
			gPad->SetLogy(1);
			++iframe; frame[iframe] = (TH1F*)gPad->DrawFrame(-0.5,18,23.5,20000.);
			frame[iframe]->SetTitle(hmtdcell[i][26-1][0]->GetTitle());
			for (int ifile=0;ifile<nfiles;ifile++){
				hmtdcell[i][26-1][ifile]->Draw("same");
			}
			frame[iframe]->GetYaxis()->SetLabelSize(0.07);
			frame[iframe]->GetXaxis()->SetLabelSize(0.06);
			gPad->Update();
		}
	ccan[ican]->cd(); ccan[ican]->Update();
	ccan[ican]->Print("bgmtd.ps(");	

	gStyle->SetTitleX(0.2);
	++ican; sprintf(buf,"ccan%d",ican);
	ccan[ican] = new TCanvas(buf,buf,0,0,1100.,1100.*(8.5/11.));
	ccan[ican]->SetFillColor(10);
	ccan[ican]->cd(); ccan[ican]->Divide(5,3,0.0001,0.0001);
		for (int i=0;i<5;i++){
			ccan[ican]->cd(1+i);
			gPad->SetLogy(1);
			++iframe; frame[iframe] = (TH1F*)gPad->DrawFrame(-0.5,0.5,23.5,10.5);
			frame[iframe]->SetTitle(hmtdcellnorm[i][28-1][0]->GetTitle());
			for (int ifile=0;ifile<nfiles;ifile++){
				hmtdcellnorm[i][28-1][ifile]->Draw("same");
				sprintf(buf,"%5.2f",GetAverage(hmtdcellnorm[i][28-1][ifile]));
				++itext; text[itext]->SetTextColor(kColors[ifile]); text[itext]->DrawLatex(0.15,0.58+ifile*0.06,buf);
			}
			frame[iframe]->GetYaxis()->SetLabelSize(0.07);
			frame[iframe]->GetXaxis()->SetLabelSize(0.06);
			gPad->Update();
		}
		for (int i=0;i<5;i++){
			ccan[ican]->cd(6+i);
			gPad->SetLogy(1);
			++iframe; frame[iframe] = (TH1F*)gPad->DrawFrame(-0.5,0.5,23.5,10.5);
			frame[iframe]->SetTitle(hmtdcellnorm[i][27-1][0]->GetTitle());
			for (int ifile=0;ifile<nfiles;ifile++){
				hmtdcellnorm[i][27-1][ifile]->Draw("same");
				sprintf(buf,"%5.2f",GetAverage(hmtdcellnorm[i][27-1][ifile]));
				++itext; text[itext]->SetTextColor(kColors[ifile]); text[itext]->DrawLatex(0.15,0.58+ifile*0.06,buf);
			}
			frame[iframe]->GetYaxis()->SetLabelSize(0.07);
			frame[iframe]->GetXaxis()->SetLabelSize(0.06);
			gPad->Update();
		}
		//
		ccan[ican]->cd(11);
		pave->Draw();
		//
		for (int i=1;i<4;i++){
			ccan[ican]->cd(11+i);
			gPad->SetLogy(1);
			++iframe; frame[iframe] = (TH1F*)gPad->DrawFrame(-0.5,0.5,23.5,10.5);
			frame[iframe]->SetTitle(hmtdcellnorm[i][26-1][0]->GetTitle());
			for (int ifile=0;ifile<nfiles;ifile++){
				hmtdcellnorm[i][26-1][ifile]->Draw("same");
				sprintf(buf,"%5.2f",GetAverage(hmtdcellnorm[i][26-1][ifile]));
				++itext; text[itext]->SetTextColor(kColors[ifile]); text[itext]->DrawLatex(0.15,0.58+ifile*0.06,buf);
			}
			frame[iframe]->GetYaxis()->SetLabelSize(0.07);
			frame[iframe]->GetXaxis()->SetLabelSize(0.06);
			gPad->Update();
		}
	ccan[ican]->cd(); ccan[ican]->Update();
	ccan[ican]->Print("bgmtd.ps");	

	gStyle->SetPadLeftMargin(0.15);
	gStyle->SetTitleX(0.2);
	++ican; sprintf(buf,"ccan%d",ican);
	ccan[ican] = new TCanvas(buf,buf,0,0,1100.,1100.*(8.5/11.));
	ccan[ican]->SetFillColor(10);
	ccan[ican]->cd(); ccan[ican]->Divide(5,3,0.0001,0.0001);
		for (int i=0;i<5;i++){
			ccan[ican]->cd(1+i);
			//gPad->SetLogy(1);
			++iframe; frame[iframe] = (TH1F*)gPad->DrawFrame(-0.5,-10.,23.5,3000);
			frame[iframe]->SetTitle(hmtdcelldiff[i][28-1][0]->GetTitle());
			for (int ifile=0;ifile<nfiles;ifile++){
				hmtdcelldiff[i][28-1][ifile]->Draw("same");
				sprintf(buf,"%5.1f",GetAverage(hmtdcelldiff[i][28-1][ifile]));
				++itext; text[itext]->SetTextColor(kColors[ifile]); text[itext]->DrawLatex(0.19,0.58+ifile*0.06,buf);
			}
			frame[iframe]->GetYaxis()->SetLabelSize(0.07);
			frame[iframe]->GetXaxis()->SetLabelSize(0.06);
			gPad->Update();
		}
		for (int i=0;i<5;i++){
			ccan[ican]->cd(6+i);
			//gPad->SetLogy(1);
			++iframe; frame[iframe] = (TH1F*)gPad->DrawFrame(-0.5,-10.,23.5,3000);
			frame[iframe]->SetTitle(hmtdcelldiff[i][27-1][0]->GetTitle());
			for (int ifile=0;ifile<nfiles;ifile++){
				hmtdcelldiff[i][27-1][ifile]->Draw("same");
				sprintf(buf,"%5.1f",GetAverage(hmtdcelldiff[i][27-1][ifile]));
				++itext; text[itext]->SetTextColor(kColors[ifile]); text[itext]->DrawLatex(0.35,0.58+ifile*0.06,buf);
			}
			frame[iframe]->GetYaxis()->SetLabelSize(0.07);
			frame[iframe]->GetXaxis()->SetLabelSize(0.06);
			gPad->Update();
		}
		//
		ccan[ican]->cd(11);
		pave->Draw();
		//
		for (int i=1;i<4;i++){
			ccan[ican]->cd(11+i);
			//gPad->SetLogy(1);
			++iframe; frame[iframe] = (TH1F*)gPad->DrawFrame(-0.5,-10.,23.5,3000);
			frame[iframe]->SetTitle(hmtdcelldiff[i][26-1][0]->GetTitle());
			for (int ifile=0;ifile<nfiles;ifile++){
				hmtdcelldiff[i][26-1][ifile]->Draw("same");
				sprintf(buf,"%5.1f",GetAverage(hmtdcelldiff[i][26-1][ifile]));
				++itext; text[itext]->SetTextColor(kColors[ifile]); text[itext]->DrawLatex(0.19,0.58+ifile*0.06,buf);
			}
			frame[iframe]->GetYaxis()->SetLabelSize(0.07);
			frame[iframe]->GetXaxis()->SetLabelSize(0.06);
			gPad->Update();
		}
	ccan[ican]->cd(); ccan[ican]->Update();
	ccan[ican]->Print("bgmtd.ps");	

	ccan[ican]->Print("bgmtd.ps]");
	sprintf(buf,"/usr/bin/ps2pdf %s.ps %s.pdf","bgmtd","bgmtd");
	cout<<"..... Executing ... "<<buf<<endl;
	gSystem->Exec(buf);
	sprintf(buf,"/bin/cp %s.pdf /star/u/llope/WWW/files/","bgmtd");
	cout<<"..... Executing ... "<<buf<<endl;
	gSystem->Exec(buf);
	cout<<"..... Done...."<<endl;	
}

float GetAverage(TH1D* h){
	float avg	= 0;
	int nbx		= h->GetNbinsX();
	for (int ibx=1;ibx<=nbx;ibx++){
		avg += h->GetBinContent(ibx);
	}	avg	/= 24;
	return avg;
}