void hzslope(){

	char		buf[100],tbuf[100];
	TCanvas 	*ccan[1000];
	TLatex		*text[1000];
	Int_t		ican=-1,itext=-1;
	for (int i=0;i<1000;i++){
		text[i]	= new TLatex();
		text[i]->SetNDC();
		text[i]->SetTextSize(0.06);
	}
	TString psfileO	= TString("hzslope.ps(");
	TString psfile	= TString("hzslope.ps");
	TString psfileC	= TString("hzslope.ps]");
	TString pdffile	= TString("hzslope.pdf");
		
	const int	NDS = 3;
	const char* runstrings[NDS] = {"run14auau200_ic0","run14auau200_ic1_bbq","run14auau200_ic1_mxq"};
	TString 	filename;
	TH1D*		hzslope[NDS];
	TH1D*		hzslope1D[NDS];
	int			colo[NDS]	= {1,4,6};
	for (int ids=0;ids<NDS;ids++){
			//
			filename 	= TString(Form("./root/caleval_%s.root",runstrings[ids]));
			cout<<filename.Data()<<endl;
			TFile *f	= new TFile(filename.Data(),"read");
			//
			hzslope[ids]	= (TH1D*)f->Get("hzslope");
			sprintf(buf,"hzslope%d",ids);
			hzslope[ids]->SetName(buf);
			hzslope[ids]->SetDirectory(0);
				hzslope[ids]->SetLineColor(colo[ids]);
				hzslope[ids]->SetMarkerColor(colo[ids]);
				hzslope[ids]->SetMarkerSize(1.5);				
			hzslope1D[ids]	= (TH1D*)f->Get("hzslope1D");
			sprintf(buf,"hzslope1D%d",ids);
			hzslope1D[ids]->SetName(buf);
			hzslope1D[ids]->SetDirectory(0);
				hzslope1D[ids]->SetLineColor(colo[ids]);
				hzslope1D[ids]->SetLineWidth(2);
	}
	
 	TLegend *leg	= new TLegend(0.24,0.81,0.97,0.97);
 		leg->SetTextSize(0.045);
 		leg->SetMargin(0.09);
 		leg->SetHeader("Run14 Au+Au 200 GeV");
 		leg->AddEntry(hzslope[0],"TOF TDIG, 24.4 ps/LSB","P");
 		leg->AddEntry(hzslope[1],"TRG TAC, BBQ, 17.73 ps/TACbin","P");
 		leg->AddEntry(hzslope[2],"TRG TAC, MXQ, 17.73 ps/TACbin","P");
	//
	gROOT->SetStyle("Plain"); 
	gStyle->SetOptStat(0);
	gStyle->SetOptFit(0);
	gStyle->SetPalette(1);
	gStyle->SetErrorX(0);
	
	//
	gStyle->SetPadBottomMargin(0.045);
	gStyle->SetPadLeftMargin(0.13);
	gStyle->SetPadRightMargin(0.025);
	gStyle->SetPadTopMargin(0.02);
	++ican; sprintf(buf,"ccan%d",ican);
 	ccan[ican] = new TCanvas(buf,buf,0,0,800.,800.*(7.5/10.));
 	ccan[ican]->SetFillColor(10);
 	ccan[ican]->cd(); ccan[ican]->Divide(2,1,0.0001,0.0001);
	ccan[ican]->cd(1);
		hzslope[0]->SetTitle("");
		hzslope[0]->SetMinimum(0.941);
		hzslope[0]->SetMaximum(1.059);
		hzslope[0]->GetXaxis()->SetLabelSize(0.06);		
		hzslope[0]->GetYaxis()->SetLabelSize(0.06);		
		hzslope[0]->GetXaxis()->SetLabelOffset(-0.01);		
		hzslope[0]->Draw();
		hzslope[1]->Draw("same");
		hzslope[2]->Draw("same");
		leg->Draw("same");
	ccan[ican]->cd(2);
		gPad->SetLogy(1);
		hzslope1D[0]->SetTitle("");
		hzslope1D[1]->SetTitle("");
		hzslope1D[2]->SetTitle("");
		hzslope1D[0]->SetStats(0);
		hzslope1D[1]->SetStats(0);
		hzslope1D[2]->SetStats(0);
		hzslope1D[0]->GetXaxis()->SetRangeUser(0.945,1.055);		
		hzslope1D[0]->GetXaxis()->SetLabelOffset(-0.01);		
		hzslope1D[0]->GetXaxis()->SetLabelSize(0.06);		
		hzslope1D[0]->GetYaxis()->SetLabelSize(0.06);		
		hzslope1D[0]->SetMinimum(0.5);
		hzslope1D[0]->SetMaximum(40);
		hzslope1D[0]->Draw();
		hzslope1D[1]->Draw("same");
		hzslope1D[2]->Draw("same");
 	ccan[ican]->cd(); ccan[ican]->Update();
 	ccan[ican]->Print(psfileO.Data());
	
	//=============================================================================	
	//
 	cout<<"..... Loop  Closing ps file...."<<endl;
	ccan[ican]->Print(psfileC.Data());
 	sprintf(buf,"/usr/bin/pstopdf %s -o %s",psfile.Data(),pdffile.Data());
 	cout<<"..... Loop  Executing ... "<<buf<<endl;
 	gSystem->Exec(buf);
 	sprintf(buf,"/bin/cp %s /star/u/llope/WWW/files/",pdffile.Data());
 	cout<<"..... Loop  Executing ... "<<buf<<endl;
 	gSystem->Exec(buf);
	cout<<"..... Loop  Done...."<<endl;	
	
	

}