
Double_t funres(Double_t *x, Double_t *par){
	Double_t resval = 0.0;
	if (x[0]>0){
		resval	= TMath::Sqrt(x[0]/(1.+x[0]));
		return par[0]/resval;
	} else {
		return 0.0;
	}
}
void doit(TString);


//--------------------------------------------------------------------------------
void sigma0(){

//	doit("run14auau015");
	doit("run14auhe200");

}

//--------------------------------------------------------------------------------
void doit(const char* dataname) {

cout<<"..............runstring="<<dataname<<endl;

TH1::AddDirectory(kFALSE);
char buf[200];

TFile *f = new TFile(Form("root/cal_%s_ic0_ia2.root",dataname),"READ");
	f->cd();
	TH1D *hs00 = (TH1D*)gDirectory->Get("hsigma0");
		hs00->SetName("hs00");
f->Close();
TFile *f = new TFile(Form("root/cal_%s_ic1_ia2_bbq.root",dataname),"READ");
	f->cd();
	TH1D *hs10 = (TH1D*)gDirectory->Get("hsigma0");
		hs10->SetName("hs10");
f->Close();
TFile *f = new TFile(Form("root/cal_%s_ic1_ia2_mxq.root",dataname),"READ");
	f->cd();
	TH1D *hs20 = (TH1D*)gDirectory->Get("hsigma0");
		hs20->SetName("hs10");
f->Close();

hs00->SetMarkerColor(1);	hs00->SetMarkerStyle(20);
hs10->SetMarkerColor(2);	hs10->SetMarkerStyle(20);
hs20->SetMarkerColor(3);	hs20->SetMarkerStyle(20);
//hs30->SetMarkerColor(4);	hs30->SetMarkerStyle(20);
hs00->SetStats(0);	hs00->SetTitle("");
hs10->SetStats(0);	hs10->SetTitle("");
hs20->SetStats(0);	hs20->SetTitle("");
//hs30->SetStats(0);	hs30->SetTitle("");

gStyle->SetOptStat(0);
gStyle->SetPadBottomMargin(0.10);
gStyle->SetPadLeftMargin(0.10);
gStyle->SetPadTopMargin(0.025);
gStyle->SetPadRightMargin(0.01);
gStyle->SetPaperSize(TStyle::kUSLetter);
gStyle->SetLabelSize(0.045,"X");
gStyle->SetLabelSize(0.045,"Y");
gStyle->SetTitleXSize(0.055);
gStyle->SetTitleYSize(0.055);
gStyle->SetTitleOffset(0.8,"X");
gStyle->SetTitleOffset(0.9,"Y");

TLatex *lt	= new TLatex(0.,0.," ");
	lt->SetNDC();
	lt->SetTextSize(0.065);

gROOT->ForceStyle();

TCanvas *sigma0_1 = new TCanvas("sigma0_1","sigma0_1",10,10,640,460);
sigma0_1->cd(0);  //sigma0_1->Divide(1,1,0.0001,0.0001);
	sigma0_1->cd(1);
		frame = gPad->DrawFrame(-1.5,0.025,38.5,0.45);
			frame->SetXTitle("upVPD Channel");
			frame->SetYTitle("#sigma_{0} (ns)");
	hs00->Draw("same");
	hs10->Draw("same");
	hs20->Draw("same");
//	hs30->Draw("same");
	cout<<"(0.62,0.82,0.99,0.975)"<<endl;
	TLegend *leg = new TLegend(0.62,0.82,0.99,0.975);
		leg->SetTextSize(0.045);
		leg->AddEntry(hs00,"ToT & tLE","P");
		leg->AddEntry(hs10,"ADC & TAC (BBQ)","P");
		leg->AddEntry(hs20,"ADC & TAC (MXQ)","P");
//		leg->AddEntry(hs30,"ToT & TAC","P");
		leg->Draw();
		lt->DrawLatex(0.22,0.92,Form("%s",dataname));
			//tt->DrawLatex(0.2,0.9,"d NJ");
sigma0_1->Update(); sigma0_1->cd();
sigma0_1->Print(Form("./ps/sigma0_%s.ps",dataname));

delete sigma0_1;

sprintf(buf,"/bin/cp -v %s /star/u/geurts/WWW/files/",Form("./ps/sigma0_%s.ps",dataname));
gSystem->Exec(buf);


}




















