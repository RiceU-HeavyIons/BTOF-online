
//----------------------------------------------------------------------------------
//
void zz() {

	TH1::AddDirectory(kFALSE);

	float x[1000] 	= {0};
	float n[1000] 	= {0};
	int nbx;


	TFile *f = new TFile("root/caleval_run13pp500_ic0.root","READ");
		f->cd();
		TH2D *hzz_run13pp500 = (TH2D*)gDirectory->Get("hzz_calavg");
			hzz_run13pp500->SetName("hzz_run13pp500");
		TH1D *hdzz_run13pp500 = (TH1D*)gDirectory->Get("hdzz_calavg");
			hdzz_run13pp500->SetName("hdzz_run13pp500");
			hdzz_run13pp500->SetLineColor(1);
			hdzz_run13pp500->SetLineWidth(2);
	f->Close();
	TF1 *gg_run13pp500	= new TF1("gg_run13pp500","gaus",-2.5,3.5);
		gg_run13pp500->SetLineColor(16);
		gg_run13pp500->SetLineWidth(2);
	hdzz_run13pp500->Fit("gg_run13pp500","Q0NR");
		float avgdzz_run13pp500	= gg_run13pp500->GetParameter(1);
		float sigdzz_run13pp500	= gg_run13pp500->GetParameter(2);
		cout<<"dzz_run13pp500 = "<<avgdzz_run13pp500<<"  "<<sigdzz_run13pp500<<endl;	
	nbx = hdzz_run13pp500->GetNbinsX();
	for (int ibx=1;ibx<=nbx;ibx++){
		x[ibx-1]	= hdzz_run13pp500->GetBinCenter(ibx) - avgdzz_run13pp500;
		n[ibx-1]	= hdzz_run13pp500->GetBinContent(ibx);
	} 
	for (int ibx=1;ibx<=nbx;ibx++){
		int kbx		= hdzz_run13pp500->FindBin(x[ibx-1]);
		hdzz_run13pp500->SetBinContent(kbx,n[ibx-1]);
	} 
	gg_run13pp500->SetRange(-3,3);
	hdzz_run13pp500->Fit("gg_run13pp500","Q0NR");
		avgdzz_run13pp500	= gg_run13pp500->GetParameter(1);
		sigdzz_run13pp500	= gg_run13pp500->GetParameter(2);
	gg_run13pp500->SetRange(-20,20);

	TFile *f = new TFile("root/caleval_run14auau200_ic0.root","READ");
		f->cd();
		TH2D *hzz_run14auau200 = (TH2D*)gDirectory->Get("hzz_calavg");
			hzz_run14auau200->SetName("hzz_run14auau200");
		TH1D *hdzz_run14auau200 = (TH1D*)gDirectory->Get("hdzz_calavg");
			hdzz_run14auau200->SetName("hdzz_run14auau200");
			hdzz_run14auau200->SetLineColor(1);
			hdzz_run14auau200->SetLineWidth(2);
	f->Close();
	TF1 *gg_run14auau200	= new TF1("gg_run14auau200","gaus",-2.5,3.5);
		gg_run14auau200->SetLineColor(16);
		gg_run14auau200->SetLineWidth(2);
	hdzz_run14auau200->Fit("gg_run14auau200","Q0NR");
		float avgdzz_run14auau200	= gg_run14auau200->GetParameter(1);
		float sigdzz_run14auau200	= gg_run14auau200->GetParameter(2);
		cout<<"dzz_run14auau200 = "<<avgdzz_run14auau200<<"  "<<sigdzz_run14auau200<<endl;	
	nbx = hdzz_run14auau200->GetNbinsX();
	for (int ibx=1;ibx<=nbx;ibx++){
		x[ibx-1]	= hdzz_run14auau200->GetBinCenter(ibx) - avgdzz_run14auau200;
		n[ibx-1]	= hdzz_run14auau200->GetBinContent(ibx);
	} 
	for (int ibx=1;ibx<=nbx;ibx++){
		int kbx		= hdzz_run14auau200->FindBin(x[ibx-1]);
		hdzz_run14auau200->SetBinContent(kbx,n[ibx-1]);
	} 
	gg_run14auau200->SetRange(-3,3);
	hdzz_run14auau200->Fit("gg_run14auau200","Q0NR");
		avgdzz_run14auau200	= gg_run14auau200->GetParameter(1);
		sigdzz_run14auau200	= gg_run14auau200->GetParameter(2);
	gg_run14auau200->SetRange(-20,20);


	
	gStyle->SetOptStat(0);
	gStyle->SetPaperSize(TStyle::kUSLetter);
	gStyle->SetLabelSize(0.045,"X");
	gStyle->SetLabelSize(0.045,"Y");
	gStyle->SetTitleXSize(0.055);
	gStyle->SetTitleYSize(0.055);
	gStyle->SetTitleOffset(0.7,"X");
	gStyle->SetTitleOffset(0.95,"Y");
	gStyle->SetErrorX(0.0);
	gStyle->SetLegendFont(132);
	gStyle->SetTitleFont(132);
	gStyle->SetPalette();
	const UInt_t Number = 10;
	Double_t Red[Number];
	Double_t Green[Number];
	Double_t Blue[Number];
	Double_t Stops[Number];
	double dcol = 1/double(Number);
	double grey = 1;
	for (int j = 0; j < Number; j++){  
		Stops[j]=double(j)/double(Number-1);
		Red[j]=grey;
		Blue[j]=grey;
		Green[j]=grey;
		grey = grey - dcol;
	}
	Int_t nb=100;
	TColor::CreateGradientColorTable(Number,Stops,Red,Green,Blue,nb);
	gROOT->ForceStyle();

	TLatex *t_run13pp500 = new TLatex();
			t_run13pp500->SetNDC();
			t_run13pp500->SetTextFont(132);			
	TLatex *t_run14auau200 = new TLatex();
			t_run14auau200->SetNDC();
			t_run14auau200->SetTextFont(132);			
	TLatex *tb_run13pp500 = new TLatex();
			tb_run13pp500->SetNDC();
			tb_run13pp500->SetTextFont(132);			
	TLatex *tb_run14auau200 = new TLatex();
			tb_run14auau200->SetNDC();
			tb_run14auau200->SetTextFont(132);			


	gStyle->SetPadBottomMargin(0.12);
	gStyle->SetPadLeftMargin(0.14);
	gStyle->SetPadTopMargin(0.01);
	gStyle->SetPadRightMargin(0.005);
	TCanvas *zz_1 = new TCanvas("zz_1","zz_1",10,10,1000,460);
	zz_1->cd(0);  zz_1->Divide(2,1,0.0001,0.0001);
		zz_1->cd(1);
		hzz_run13pp500->GetXaxis()->SetRangeUser(-50.,50.);
		hzz_run13pp500->GetYaxis()->SetRangeUser(-50.,50.);
		hzz_run13pp500->SetTitle("");
		hzz_run13pp500->Draw("col");
			hzz_run13pp500->SetXTitle("Z_{vtx}^{TPC}  (cm)");
			hzz_run13pp500->SetYTitle("Z_{vtx}^{VPD}  (cm)");
			hzz_run13pp500->GetXaxis()->SetTitleFont(132);
			hzz_run13pp500->GetYaxis()->SetTitleFont(132);
			hzz_run13pp500->GetXaxis()->SetLabelFont(132);
			hzz_run13pp500->GetYaxis()->SetLabelFont(132);
			hzz_run13pp500->GetXaxis()->SetNdivisions(9);
			hzz_run13pp500->GetXaxis()->SetTitleSize(0.07);
			hzz_run13pp500->GetYaxis()->SetTitleSize(0.07);
			hzz_run13pp500->GetXaxis()->SetLabelSize(0.06);
			hzz_run13pp500->GetYaxis()->SetLabelSize(0.06);
			hzz_run13pp500->GetXaxis()->SetLabelOffset(-0.004);
			hzz_run13pp500->GetXaxis()->SetTitleOffset(0.7);
			hzz_run13pp500->GetYaxis()->SetTitleOffset(0.9);
			tb_run13pp500->SetTextSize(0.07);
			tb_run13pp500->DrawLatex(0.30,0.93,"p+p 510 GeV, Run-13");
			gPad->Update();
			//
			TPad *subpad1 = new TPad("subpad","",0.58,0.14,0.98,0.50);
				subpad1->Draw();
				subpad1->cd();
				subpad1->SetMargin(0.14,0.02,0.18,0.02);
			hdzz_run13pp500->GetXaxis()->SetRangeUser(-19.,19.);
			hdzz_run13pp500->SetTitle("");
			hdzz_run13pp500->GetXaxis()->SetTitle("#DeltaZ (cm)");
			hdzz_run13pp500->GetXaxis()->SetTitleFont(132);
			hdzz_run13pp500->GetYaxis()->SetTitleFont(132);
			hdzz_run13pp500->GetXaxis()->SetLabelFont(132);
			hdzz_run13pp500->GetYaxis()->SetLabelFont(132);
			hdzz_run13pp500->GetXaxis()->SetNdivisions(9);
			hdzz_run13pp500->GetXaxis()->SetTitleSize(0.12);
			hdzz_run13pp500->GetYaxis()->SetTitleSize(0.09);
			hdzz_run13pp500->GetXaxis()->SetLabelSize(0.09);
			hdzz_run13pp500->GetYaxis()->SetLabelSize(0.08);
			hdzz_run13pp500->GetXaxis()->SetLabelOffset(-0.004);
			hdzz_run13pp500->GetXaxis()->SetTitleOffset(0.7);
			hdzz_run13pp500->GetYaxis()->SetTitleOffset(1.1);
			hdzz_run13pp500->SetMaximum(4200);
			hdzz_run13pp500->Draw();
			gg_run13pp500->Draw("same");
			t_run13pp500->SetTextSize(0.12);
			t_run13pp500->DrawLatex(0.30,0.88,Form("#sigma(#DeltaZ)=%3.1fcm",sigdzz_run13pp500));
		//
		zz_1->cd(2);
		hzz_run14auau200->GetXaxis()->SetRangeUser(-50.,50.);
		hzz_run14auau200->GetYaxis()->SetRangeUser(-50.,50.);
		hzz_run14auau200->SetTitle("");
		hzz_run14auau200->Draw("col");
			hzz_run14auau200->SetXTitle("Z_{vtx}^{TPC}  (cm)");
			hzz_run14auau200->SetYTitle("Z_{vtx}^{VPD}  (cm)");
			hzz_run14auau200->GetXaxis()->SetTitleFont(132);
			hzz_run14auau200->GetYaxis()->SetTitleFont(132);
			hzz_run14auau200->GetXaxis()->SetLabelFont(132);
			hzz_run14auau200->GetYaxis()->SetLabelFont(132);
			hzz_run14auau200->GetXaxis()->SetNdivisions(9);
			hzz_run14auau200->GetXaxis()->SetTitleSize(0.07);
			hzz_run14auau200->GetYaxis()->SetTitleSize(0.07);
			hzz_run14auau200->GetXaxis()->SetLabelSize(0.06);
			hzz_run14auau200->GetYaxis()->SetLabelSize(0.06);
			hzz_run14auau200->GetXaxis()->SetLabelOffset(-0.004);
			hzz_run14auau200->GetXaxis()->SetTitleOffset(0.7);
			hzz_run14auau200->GetYaxis()->SetTitleOffset(0.9);
			tb_run14auau200->SetTextSize(0.07);
			tb_run14auau200->DrawLatex(0.28,0.93,"Au+Au 200 GeV, Run-10");
			gPad->Update();
			//
			TPad *subpad2 = new TPad("subpad","",0.58,0.14,0.98,0.50);
				subpad2->Draw();
				subpad2->cd();
				subpad2->SetMargin(0.16,0.02,0.18,0.02);
			hdzz_run14auau200->GetXaxis()->SetRangeUser(-19.,19.);
			hdzz_run14auau200->SetTitle("");
			hdzz_run14auau200->GetXaxis()->SetTitle("#DeltaZ (cm)");
			hdzz_run14auau200->GetXaxis()->SetTitleFont(132);
			hdzz_run14auau200->GetYaxis()->SetTitleFont(132);
			hdzz_run14auau200->GetXaxis()->SetLabelFont(132);
			hdzz_run14auau200->GetYaxis()->SetLabelFont(132);
			hdzz_run14auau200->GetXaxis()->SetNdivisions(9);
			hdzz_run14auau200->GetXaxis()->SetTitleSize(0.12);
			hdzz_run14auau200->GetYaxis()->SetTitleSize(0.09);
			hdzz_run14auau200->GetXaxis()->SetLabelSize(0.09);
			hdzz_run14auau200->GetYaxis()->SetLabelSize(0.08);
			hdzz_run14auau200->GetXaxis()->SetLabelOffset(-0.004);
			hdzz_run14auau200->GetXaxis()->SetTitleOffset(0.7);
			hdzz_run14auau200->GetYaxis()->SetTitleOffset(1.1);
			hdzz_run14auau200->SetMaximum(60000);
			hdzz_run14auau200->Draw();
			gg_run14auau200->Draw("same");
			t_run14auau200->SetTextSize(0.12);
			t_run14auau200->DrawLatex(0.30,0.88,Form("#sigma(#DeltaZ)=%3.1fcm",sigdzz_run14auau200));
	zz_1->Update(); zz_1->cd();
	zz_1->Print("zz.eps");


	gSystem->Exec("/bin/cp -v zz.eps /star/u/geurts/WWW/files/");

}




// 		TLegend *leg = new TLegend(0.57,0.75,0.99,0.99);
// 			leg->SetTextSize(0.04);
// 			leg->SetMargin(0.08);
// 			leg->SetTextAlign(12);
// 			leg->AddEntry(hsauau200,Form("Au+Au 200GeV 2010,   #LT#sigma_{0}#GT=%3.0fps",1000.*mauau200),"P");
// 			leg->AddEntry(hsauau062,Form("Au+Au 62.4GeV 2010,   #LT#sigma_{0}#GT=%3.0fps",1000.*mauau062),"P");
// 			leg->AddEntry(hsauau039,Form("Au+Au 39GeV 2010,   #LT#sigma_{0}#GT=%3.0fps",1000.*mauau039),"P");
// 			//leg->AddEntry(hsauau007,Form("Au+Au 7.7GeV 2010,   #LT#sigma_{0}#GT=%3.0fps",1000.*mauau007),"P");
// 			leg->AddEntry(hspp500,Form("p+p 510GeV 2013,   #LT#sigma_{0}#GT=%3.0fps",1000.*mpp500),"P");
// 			leg->Draw();
















