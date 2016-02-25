void TH1D_Subtract(TH1D *h1, TH1D *h2, TH1D *hd);

void bory(){

	//Run 14112011  Mon Apr 22 03:40:24 EDT - Mon Apr 22 03:51:06 EDT	NO BEAM	
	//Run 14112036  Mon Apr 22 13:31:50 EDT - Mon Apr 22 13:41:42 EDT	BEAM ON	
	//Run 14112047  Mon Apr 22 16:10:03 EDT - Mon Apr 22 16:20:00 EDT	BEAM ON

	int ican = -1; 
	char buf[100];
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
	
	int kcoli[3]	= {1,4,6};
	int kcol[2]		= {4,6};

	TH1D *hmtdcell[5][30][3];
	TFile *f;
	cout<<"Read files..."<<endl;
	for (int ifile=0;ifile<3;ifile++){
		if (ifile==0){ f = new TFile("/star/institutions/rice/llope/noise/noise_14158.root","read");	} else	// noise
		if (ifile==1){ f = new TFile("/star/institutions/rice/llope/noise/noise_14157049.root","read");	}		// B only
		if (ifile==2){ f = new TFile("/star/institutions/rice/llope/noise/noise_14158018.root","read");	}		// Y only
		for (int ibl=0;ibl<30;ibl++){
			for (int i=0;i<5;i++){
				sprintf(buf,"hmtdcell%d%d",i,ibl);
				hmtdcell[i][ibl][ifile]	= (TH1D*)f->Get(buf);
				sprintf(buf,"hmtdcell%d%d%d",i,ibl,ifile);
				hmtdcell[i][ibl][ifile]->SetName(buf);
				hmtdcell[i][ibl][ifile]->SetDirectory(0);
				hmtdcell[i][ibl][ifile]->SetFillStyle(0);
				hmtdcell[i][ibl][ifile]->SetLineColor(kcoli[ifile]);
				//
			}
		}
		f->Close();
	}

	cout<<"Analyze..."<<endl;
	TH1D *hmtdcellrat[5][30][2];
	TH1D *hmtdcelldiff[5][30][2];
	TF1  *fmtdcellrat[5][30][2];
	TH2D *hrat2D[2];
	for (int if=0;if<2;if++){
		sprintf(buf,"hrat2D%d",if);
		hrat2D[if]	= new TH2D(buf,"B or Y / noise by (BL,pos)",15,0.5,15.5,5,0.5,5.5);
		for (int ibl=0;ibl<30;ibl++){
			for (int i=0;i<5;i++){
				//
				sprintf(buf,"hmtdcellrat%d%d%d",i,ibl,if);
				hmtdcellrat[i][ibl][if]	= (TH1D*)hmtdcell[i][ibl][if+1]->Clone(buf);
				hmtdcellrat[i][ibl][if]->Divide(hmtdcell[i][ibl][0]);
				hmtdcellrat[i][ibl][if]->SetLineColor(kcol[if]);
				hmtdcellrat[i][ibl][if]->SetLineWidth(2);
				hmtdcellrat[i][ibl][if]->SetFillStyle(0);
				//
				sprintf(buf,"fmtdcellrat%d%d%d",i,ibl,if);
				fmtdcellrat[i][ibl][if]	= new TF1(buf,"pol0",3.5,8.5);
				fmtdcellrat[i][ibl][if]->SetLineColor(kcol[if]);
				fmtdcellrat[i][ibl][if]->SetLineWidth(2);
				if (hmtdcellrat[i][ibl][if]->GetEntries()){
					hmtdcellrat[i][ibl][if]->Fit(buf,"0QNR");
					cout<<ibl+1<<" "<<i+1<<"\t"<<fmtdcellrat[i][ibl][if]->GetParameter(0)<<endl;
					//
					int ix	= ActiveRun13[ibl];
					int iy	= i + 1;
					hrat2D[if]->Fill(ix,iy,fmtdcellrat[i][ibl][if]->GetParameter(0));
					//
				}
				//
				sprintf(buf,"hmtdcelldiff%d%d%d",i,ibl,if);
				hmtdcelldiff[i][ibl][if]	= new TH1D(buf,buf,24,-0.5,23.5);
				hmtdcelldiff[i][ibl][if]->SetLineColor(kcol[if]);
				hmtdcelldiff[i][ibl][if]->SetLineWidth(2);
				hmtdcelldiff[i][ibl][if]->SetFillStyle(0);
				TH1D_Subtract(hmtdcell[i][ibl][if+1],hmtdcell[i][ibl][0],hmtdcelldiff[i][ibl][if]);
				//
			}
		}
	}

	cout<<"Plot..."<<endl;
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
				hmtdcell[i][ibl][1]->SetMinimum(0.5);
				hmtdcell[i][ibl][1]->Draw();
				hmtdcell[i][ibl][2]->Draw("same");
				hmtdcell[i][ibl][0]->Draw("same");
				hmtdcell[i][ibl][0]->GetYaxis()->SetLabelSize(0.08);
				mtdplotlabels[i][ibl]->Draw("same");
				gPad->Update();
			}
			++kactive;
		}
	}
	ccan[ican]->cd(); ccan[ican]->Update();
	if (ican==0){ ccan[ican]->Print("bory.ps("); } 
		   else { ccan[ican]->Print("bory.ps"); } 
	gStyle->SetPadLeftMargin(0.09);	

//  	gStyle->SetOptStat(0);
//  	kactive = 0;
//  	gStyle->SetPadLeftMargin(0.12);
//   	++ican; sprintf(buf,"ccan%d",ican);
//   	ccan[ican] = new TCanvas(buf,buf,0,0,800.,800.*(7.5/10.));
//   	ccan[ican]->SetFillColor(10);
//   	ccan[ican]->cd(); ccan[ican]->Divide(5,15,0.0001,0.0001);
//  	for (int ibl=0;ibl<30;ibl++){
//  		if (ActiveRun13[ibl]>0){
//  			for (int i=0;i<5;i++){
//  				ccan[ican]->cd(5*kactive+1+i);
//  				gPad->SetLogy(1);
//  				gPad->SetGridx(0);
//  				gPad->SetGridy(0);
//  				hmtdcellrat[i][ibl]->SetMinimum(0.5);
//  				hmtdcellrat[i][ibl]->SetMaximum(50.);
//  				hmtdcellrat[i][ibl]->SetLineColor(6);
//  				hmtdcellrat[i][ibl]->SetFillStyle(0);
//  				hmtdcellrat[i][ibl]->Draw();
//  				fmtdcellrat[i][ibl]->Draw("same");
//  				hmtdcellrat[i][ibl]->GetYaxis()->SetLabelSize(0.08);
//  				mtdplotlabels[i][ibl]->Draw("same");
//  				gPad->Update();
//  			}
//  			++kactive;
//  		}
//  	}
//  	ccan[ican]->cd(); ccan[ican]->Update();
//  	ccan[ican]->Print("bory.ps");
//  	gStyle->SetPadLeftMargin(0.09);	
// 
// 	for (int i=0;i<15;i++){
// 		sprintf(buf,"%d",ActiveLabels[i]);
// 		hrat2D->GetXaxis()->SetBinLabel(i+1,buf);
// 	}	
//  	gStyle->SetOptStat(0);
//  	gStyle->SetPadLeftMargin(0.12);
//   	++ican; sprintf(buf,"ccan%d",ican);
//   	ccan[ican] = new TCanvas(buf,buf,0,0,800.,800.*(7.5/10.));
//   	ccan[ican]->SetFillColor(10);
//   	ccan[ican]->cd(); ccan[ican]->Divide(1,1,0.0001,0.0001);
// 		ccan[ican]->cd(1);
// 		gPad->SetGridx(0);
// 		gPad->SetGridy(0);
// 		hrat2D->Draw("colz");
// 		gPad->Update();
//  	ccan[ican]->cd(); ccan[ican]->Update();
//  	ccan[ican]->Print("bory.ps");
//  	gStyle->SetPadLeftMargin(0.09);	
// //
// //   	++ican; sprintf(buf,"ccan%d",ican);
// //   	ccan[ican] = new TCanvas(buf,buf,0,0,800.,800.*(7.5/10.));
// //   	ccan[ican]->SetFillColor(10);
// //   	ccan[ican]->cd(); ccan[ican]->Divide(1,1,0.0001,0.0001);
// // 		ccan[ican]->cd(1);
// // 		gPad->SetGridx(0);
// // 		gPad->SetGridy(0);
// // 		hrat2D->Draw("lego3");
// // 		gPad->Update();
// //  	ccan[ican]->cd(); ccan[ican]->Update();
// //  	ccan[ican]->Print("bory.ps");
// //  	gStyle->SetPadLeftMargin(0.09);	

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
  				hmtdcelldiff[i][ibl][0]->SetMinimum(-100);
  				hmtdcelldiff[i][ibl][0]->SetMaximum( 100.);
  				hmtdcelldiff[i][ibl][0]->Draw();
  				hmtdcelldiff[i][ibl][0]->GetYaxis()->SetLabelSize(0.08);
  				hmtdcelldiff[i][ibl][1]->Draw("same");
  				mtdplotlabels[i][ibl]->Draw("same");
  				gPad->Update();
  			}
  			++kactive;
  		}
  	}
  	ccan[ican]->cd(); ccan[ican]->Update();
  	ccan[ican]->Print("bory.ps");
  	gStyle->SetPadLeftMargin(0.09);	
// 


	ccan[ican]->Print("bory.ps]");
	gSystem->Exec("/usr/bin/ps2pdf ./bory.ps ./bory.pdf");
	gSystem->Exec("/bin/cp ./bory.pdf /star/u/llope/WWW/files/");

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






