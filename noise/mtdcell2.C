void TH1D_Subtract(TH1D *h1, TH1D *h2, TH1D *hd);

void mtdcell2(){

	//Run 14112011  Mon Apr 22 03:40:24 EDT - Mon Apr 22 03:51:06 EDT	NO BEAM	
	//Run 14112036  Mon Apr 22 13:31:50 EDT - Mon Apr 22 13:41:42 EDT	BEAM ON	
	//Run 14112047  Mon Apr 22 16:10:03 EDT - Mon Apr 22 16:20:00 EDT	BEAM ON

	// 14158049
	// 14161021
	
	int ican = -1; 
	char buf[100],bufb[100];
	TCanvas* ccan[100];
						// 0, 1, 2, 3, 4, 5, 6, 7
	int ModToEtaPosn[8] = {1, 2, 3, 9, 5, 4, 9, 9};	// index is tdig [0,1,2,4,5], value is eta position [1,5] 
	int ActiveRun13[30] = {8,9,10,11,12,13,14,0,0,15,0,0,0,0,0,0,0,0,0,0,0,1,0,0,2,3,4,5,6,7};
	int ActiveLabels[15]= {22,25,26,27,28,29,30,1,2,3,4,5,6,7,10};
	TLatex *mtdplotlabels[5][30];
	for (int ibl=0;ibl<30;ibl++){
		if (ActiveRun13[ibl]>0){
			for (int i=0;i<5;i++){
				sprintf(buf,"BL%d-%d",ibl+1,i+1);
				mtdplotlabels[i][ibl] = new TLatex(0.15,0.17,buf);
				mtdplotlabels[i][ibl]->SetTextSize(0.3);
				mtdplotlabels[i][ibl]->SetNDC();				
			}
		}
	}
 	const Int_t NRGBs = 5;
 	      Int_t NCont = 20;
	//Double_t stops[NRGBs] = { 0.00, 0.34, 0.61, 0.84, 1.00 };
 	Double_t stops[NRGBs] = { 0.00, 0.10, 0.61, 0.99, 1.00 };
 	Double_t red[NRGBs]   = { 0.00, 0.00, 0.87, 1.00, 0.51 };
 	Double_t green[NRGBs] = { 0.00, 0.81, 1.00, 0.20, 0.00 };
 	Double_t blue[NRGBs]  = { 0.51, 1.00, 0.12, 0.00, 0.00 };
 	int icolstart16 = TColor::CreateGradientColorTable(NRGBs, stops, red, green, blue, NCont);
 	cout<<"sixteen contour color index start = "<<icolstart16<<endl;	
	

	TH1D *hmtdcell[5][30][2];
	TH1D *hmtdcellrat[5][30];
	TFile *f;
	cout<<"Read files..."<<endl;
	for (int ifile=0;ifile<2;ifile++){
		if (ifile==0){ f = new TFile("/star/institutions/rice/llope/noise/noise_14158049.root","read");	} else
		if (ifile==1){ f = new TFile("/star/institutions/rice/llope/noise/noise_14161021.root","read");	}
		for (int ibl=0;ibl<30;ibl++){
			for (int i=0;i<5;i++){
				sprintf(buf,"hmtdcell%d%d",i,ibl);
				hmtdcell[i][ibl][ifile]	= (TH1D*)f->Get(buf);
				sprintf(buf,"hmtdcell%d%d%d",i,ibl,ifile);
				hmtdcell[i][ibl][ifile]->SetName(buf);
				hmtdcell[i][ibl][ifile]->SetDirectory(0);
				//
			}
		}
		f->Close();
	}

	cout<<"Analyze..."<<endl;
	TF1  *fmtdcellrat[5][30];
	TH1D *hmtdcelldiff[5][30];
	TH2D *hrat2D	= new TH2D("hrat2D","Run14158049/Run14161021 by (BL,pos)",15,0.5,15.5,5,0.5,5.5);
	for (int ibl=0;ibl<30;ibl++){
		for (int i=0;i<5;i++){
			sprintf(buf,"hmtdcellrat%d%d",i,ibl);
			hmtdcellrat[i][ibl]	= (TH1D*)hmtdcell[i][ibl][0]->Clone(buf);
			hmtdcellrat[i][ibl]->Divide(hmtdcell[i][ibl][1]);
			//
			sprintf(buf,"hmtdcelldiff%d%d",i,ibl);
			//hmtdcelldiff[i][ibl]	= (TH1D*)hmtdcell[i][ibl][0]->Clone(buf);
			//if (hmtdcell[i][ibl][0]->GetEntries()&&hmtdcell[i][ibl][1]->GetEntries()){
			//	hmtdcelldiff[i][ibl]->Add(hmtdcell[i][ibl][1],-1.0);
			//}
			hmtdcelldiff[i][ibl]	= new TH1D(buf,buf,24,-0.5,23.5);
			TH1D_Subtract(hmtdcell[i][ibl][0],hmtdcell[i][ibl][1],hmtdcelldiff[i][ibl]);
			//
			sprintf(buf,"fmtdcellrat%d%d",i,ibl);
			fmtdcellrat[i][ibl]	= new TF1(buf,"pol0",3.5,8.5);
			fmtdcellrat[i][ibl]->SetLineColor(1);
			fmtdcellrat[i][ibl]->SetLineWidth(2);
			if (hmtdcellrat[i][ibl]->GetEntries()){
				hmtdcellrat[i][ibl]->Fit(buf,"0QNR");
				cout<<ibl+1<<" "<<i+1<<"\t"<<fmtdcellrat[i][ibl]->GetParameter(0)<<endl;
				//
				int ix	= ActiveRun13[ibl];
				int iy	= i + 1;
				hrat2D->Fill(ix,iy,fmtdcellrat[i][ibl]->GetParameter(0));
				//
			}
		}
	}

	gStyle->SetOptStat(0);
	int kactive = 0;
	gStyle->SetPadLeftMargin(0.12);
 	++ican; sprintf(buf,"ccan%d",ican);
 	ccan[ican] = new TCanvas(buf,buf,0,0,800.,800.*(7.5/10.));
 	ccan[ican]->SetFillColor(10);
 	ccan[ican]->cd(); ccan[ican]->Divide(5,15,0.0001,0.0001);
	for (int ibl=0;ibl<30;ibl++){
		if (ActiveRun13[ibl]>0){
			for (int i=0;i<5;i++){
				ccan[ican]->cd(5*kactive+1+i);
				gPad->SetLogy(1);
				gPad->SetGridx(0);
				gPad->SetGridy(0);
				hmtdcell[i][ibl][0]->SetMinimum(0.5);
				hmtdcell[i][ibl][0]->SetLineColor(2);
				hmtdcell[i][ibl][0]->SetFillStyle(0);
				hmtdcell[i][ibl][0]->Draw();
				hmtdcell[i][ibl][1]->SetLineColor(4);
				hmtdcell[i][ibl][1]->SetFillStyle(0);
				hmtdcell[i][ibl][1]->Draw("same");
				hmtdcell[i][ibl][1]->GetYaxis()->SetLabelSize(0.08);
				mtdplotlabels[i][ibl]->Draw("same");
				gPad->Update();
			}
			++kactive;
		}
	}
	ccan[ican]->cd(); ccan[ican]->Update();
	ccan[ican]->Print("mtdcell2.ps(");
	gStyle->SetPadLeftMargin(0.09);	

 	gStyle->SetOptStat(0);
 	kactive = 0;
 	gStyle->SetPadLeftMargin(0.12);
  	++ican; sprintf(buf,"ccan%d",ican);
  	ccan[ican] = new TCanvas(buf,buf,0,0,800.,800.*(7.5/10.));
  	ccan[ican]->SetFillColor(10);
  	ccan[ican]->cd(); ccan[ican]->Divide(5,15,0.0001,0.0001);
 	for (int ibl=0;ibl<30;ibl++){
 		if (ActiveRun13[ibl]>0){
 			for (int i=0;i<5;i++){
 				ccan[ican]->cd(5*kactive+1+i);
 				gPad->SetLogy(1);
 				gPad->SetGridx(0);
 				gPad->SetGridy(0);
 				hmtdcellrat[i][ibl]->SetMinimum(0.5);
 				hmtdcellrat[i][ibl]->SetMaximum(50.);
 				hmtdcellrat[i][ibl]->SetLineColor(6);
 				hmtdcellrat[i][ibl]->SetFillStyle(0);
 				hmtdcellrat[i][ibl]->Draw();
 				fmtdcellrat[i][ibl]->Draw("same");
 				hmtdcellrat[i][ibl]->GetYaxis()->SetLabelSize(0.08);
 				mtdplotlabels[i][ibl]->Draw("same");
 				gPad->Update();
 			}
 			++kactive;
 		}
 	}
 	ccan[ican]->cd(); ccan[ican]->Update();
 	ccan[ican]->Print("mtdcell2.ps");
 	gStyle->SetPadLeftMargin(0.09);	

	for (int i=0;i<15;i++){
		sprintf(buf,"%d",ActiveLabels[i]);
		hrat2D->GetXaxis()->SetBinLabel(i+1,buf);
	}	
 	gStyle->SetOptStat(0);
 	gStyle->SetPadLeftMargin(0.12);
  	++ican; sprintf(buf,"ccan%d",ican);
  	ccan[ican] = new TCanvas(buf,buf,0,0,800.,800.*(7.5/10.));
  	ccan[ican]->SetFillColor(10);
  	ccan[ican]->cd(); ccan[ican]->Divide(1,1,0.0001,0.0001);
		ccan[ican]->cd(1);
		gPad->SetGridx(0);
		gPad->SetGridy(0);
		hrat2D->Draw("colz");
		gPad->Update();
 	ccan[ican]->cd(); ccan[ican]->Update();
 	ccan[ican]->Print("mtdcell2.ps");
 	gStyle->SetPadLeftMargin(0.09);	
//
//   	++ican; sprintf(buf,"ccan%d",ican);
//   	ccan[ican] = new TCanvas(buf,buf,0,0,800.,800.*(7.5/10.));
//   	ccan[ican]->SetFillColor(10);
//   	ccan[ican]->cd(); ccan[ican]->Divide(1,1,0.0001,0.0001);
// 		ccan[ican]->cd(1);
// 		gPad->SetGridx(0);
// 		gPad->SetGridy(0);
// 		hrat2D->Draw("lego3");
// 		gPad->Update();
//  	ccan[ican]->cd(); ccan[ican]->Update();
//  	ccan[ican]->Print("mtdcell.ps");
//  	gStyle->SetPadLeftMargin(0.09);	

 	gStyle->SetOptStat(0);
 	kactive = 0;
 	gStyle->SetPadLeftMargin(0.12);
  	++ican; sprintf(buf,"ccan%d",ican);
  	ccan[ican] = new TCanvas(buf,buf,0,0,800.,800.*(7.5/10.));
  	ccan[ican]->SetFillColor(10);
  	ccan[ican]->cd(); ccan[ican]->Divide(5,15,0.0001,0.0001);
 	for (int ibl=0;ibl<30;ibl++){
 		if (ActiveRun13[ibl]>0){
 			for (int i=0;i<5;i++){
 				ccan[ican]->cd(5*kactive+1+i);
 				gPad->SetLogy(0);
 				gPad->SetGridx(0);
 				gPad->SetGridy(0);
 				hmtdcelldiff[i][ibl]->SetMinimum(0.1);
 				hmtdcelldiff[i][ibl]->SetMaximum(350.);
 				hmtdcelldiff[i][ibl]->SetLineColor(1);
 				hmtdcelldiff[i][ibl]->SetFillColor(5);
 				hmtdcelldiff[i][ibl]->Draw();
 				hmtdcelldiff[i][ibl]->GetYaxis()->SetLabelSize(0.08);
 				mtdplotlabels[i][ibl]->Draw("same");
 				gPad->Update();
 			}
 			++kactive;
 		}
 	}
 	ccan[ican]->cd(); ccan[ican]->Update();
 	ccan[ican]->Print("mtdcell2.ps");
 	gStyle->SetPadLeftMargin(0.09);	



	ccan[ican]->Print("mtdcell2.ps]");
	gSystem->Exec("/usr/bin/ps2pdf ./mtdcell2.ps ./mtdcell2.pdf");
	gSystem->Exec("/bin/cp ./mtdcell2.pdf /star/u/llope/WWW/files/");

}

TH1D *TH1D_Subtract(TH1D *h1, TH1D *h2, TH1D *hd){
	if (!h1 || !h2){ return 0; }
	if (h1->GetNbinsX()!=h1->GetNbinsX()){ return 0; }
	for (int ibx=1;ibx<=h1->GetNbinsX();ibx++){
		y1	= h1->GetBinContent(ibx);
		y2	= h2->GetBinContent(ibx);
		hd->SetBinContent(ibx,y1-y2);
		//cout<<ibx<<" "<<y1<<" "<<y2<<" "<<y1-y2<<endl;
	}
}






