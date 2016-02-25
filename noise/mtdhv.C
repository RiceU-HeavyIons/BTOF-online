TString modulenames[5][30];

void mtdhv(){
	
	TH1::AddDirectory(kFALSE);
	char	buf[120];
	char	tbuf[120];
	TCanvas *ccan[100];
	TH1F *frame[100];
	int		ican		= -1;
	int		iframe		= -1;

	SetNames();
	
	const int runs[3]	= {13020041,13020042,13020043};
	const int kcol[3]	= {1,4,6};
	TH1D* hmtdcell[5][30][3];
	for (int ir=0;ir<3;ir++){
		sprintf(buf,"/star/institutions/rice/llope/noise/noise_%d.root",runs[ir]);
		cout<<buf<<endl;
		TFile *f = new TFile(Form(buf,runs[ir]),"read");
		for (int ie=0;ie<5;ie++){
			for (int ibl=0;ibl<30;ibl++){
				sprintf(buf,"hmtdcell%d%d",ie,ibl);
				hmtdcell[ie][ibl][ir] = (TH1D*)gDirectory->Get(buf);
				sprintf(buf,"hmtdcell%d%d%d",ie,ibl,ir);
				hmtdcell[ie][ibl][ir]->SetName(buf);
				hmtdcell[ie][ibl][ir]->SetFillStyle(0);
				hmtdcell[ie][ibl][ir]->SetLineColor(kcol[ir]);
			}
		}
		f->Close();
	}

	gROOT->SetStyle("Plain"); 
	gStyle->SetOptStat(0);
	gStyle->SetOptFit(1111);
	gStyle->SetPadBottomMargin(0.04);
	gStyle->SetPadTopMargin(0.02);
	gStyle->SetPadLeftMargin(0.07);
	gStyle->SetPadRightMargin(0.02);
	gStyle->SetOptFit(0);

	TLatex *thistext	= new TLatex();
		thistext->SetNDC();
		thistext->SetTextSize(0.08);
		
	int ibl;
 	++ican; sprintf(buf,"ccan%d",ican);
 	ccan[ican] = new TCanvas(buf,buf,0,0,800.,800.*(7.5/10.));
 	ccan[ican]->SetFillColor(10);
 	ccan[ican]->cd(); ccan[ican]->Divide(5,3,0.0001,0.0001);
		for (int i=0;i<5;i++){
			ccan[ican]->cd(1+i);
			++iframe; frame[iframe] = (TH1F*)gPad->DrawFrame(-0.5,0.5,23.5,150.);
			ibl		= 28;
			for (int ir=0;ir<3;ir++){
				hmtdcell[i][ibl-1][ir]->SetMinimum(  0.5);
				hmtdcell[i][ibl-1][ir]->SetMaximum(200.);
				hmtdcell[i][ibl-1][ir]->Draw("same");
				thistext->DrawLatex(0.15,0.89,modulenames[i][ibl-1].Data());
			}
		}
		for (int i=0;i<5;i++){
			ccan[ican]->cd(6+i);
			++iframe; frame[iframe] = (TH1F*)gPad->DrawFrame(-0.5,0.5,23.5,150.);
			ibl		= 27;
			for (int ir=0;ir<3;ir++){
				hmtdcell[i][ibl-1][ir]->SetMinimum(  0.5);
				hmtdcell[i][ibl-1][ir]->SetMaximum(200.);
				hmtdcell[i][ibl-1][ir]->Draw("same");
				thistext->DrawLatex(0.15,0.89,modulenames[i][ibl-1].Data());
			}
		}
		for (int i=0;i<5;i++){
			ccan[ican]->cd(11+i);
			++iframe; frame[iframe] = (TH1F*)gPad->DrawFrame(-0.5,0.5,23.5,150.);
			ibl		= 26;
			for (int ir=0;ir<3;ir++){
				hmtdcell[i][ibl-1][ir]->SetMinimum(  0.5);
				hmtdcell[i][ibl-1][ir]->SetMaximum(200.);
				hmtdcell[i][ibl-1][ir]->Draw("same");
				thistext->DrawLatex(0.15,0.89,modulenames[i][ibl-1].Data());
			}
		}
	ccan[ican]->cd(); ccan[ican]->Update();
	ccan[ican]->Print("mtdhv.ps(");

	ccan[ican]->Print("mtdhv.ps]");
	sprintf(buf,"/usr/bin/ps2pdf %s.ps %s.pdf","mtdhv","mtdhv");
	cout<<"..... noise::loop Executing ... "<<buf<<endl;
	gSystem->Exec(buf);
	sprintf(buf,"/bin/cp %s.pdf /star/u/llope/WWW/files/","mtdhv");
	cout<<"..... noise::loop Executing ... "<<buf<<endl;
	gSystem->Exec(buf);
	
}

void SetNames(){
	for (int ie=0;ie<5;ie++){
		for (int ibl=0;ibl<30;ibl++){
			modulenames[ie][ibl] = TString(Form(" "));
		}
	}
	int ibl,ie;
	ibl=26; ie=2;	modulenames[ie-1][ibl-1]	= TString("000, USTC_000");
	ibl=26; ie=3;	modulenames[ie-1][ibl-1]	= TString("011, USTC_003");
	ibl=26; ie=4;	modulenames[ie-1][ibl-1]	= TString("010, USTC_002");
	ibl=27; ie=1;	modulenames[ie-1][ibl-1]	= TString("012, USTC_004");
	ibl=27; ie=2;	modulenames[ie-1][ibl-1]	= TString("007, THU_009");
	ibl=27; ie=3;	modulenames[ie-1][ibl-1]	= TString("006, THU_008");
	ibl=27; ie=4;	modulenames[ie-1][ibl-1]	= TString("004, THU_006");
	ibl=27; ie=5;	modulenames[ie-1][ibl-1]	= TString("009, USTC_001");
	ibl=28; ie=1;	modulenames[ie-1][ibl-1]	= TString("008, THU_010");
	ibl=28; ie=2;	modulenames[ie-1][ibl-1]	= TString("005, THU_007");
	ibl=28; ie=3;	modulenames[ie-1][ibl-1]	= TString("002, THU_004");
	ibl=28; ie=4;	modulenames[ie-1][ibl-1]	= TString("001, THU_003");
	ibl=28; ie=5;	modulenames[ie-1][ibl-1]	= TString("003, THU_005");
	
	
}