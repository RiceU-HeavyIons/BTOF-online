void cocoday(){

	char buf[100];
	Int_t	ican = -1;
	TCanvas *ccan[100];

	TH1D *hcoco_vpd_w[100];
	TH1D *hcoco_vpd_e[100];
	TH1D *hcocokeep_vpd_w[100];
	TH1D *hcocokeep_vpd_e[100];
	for (int id=74;id<=94;id++){
		int	i = id-74;
		sprintf(buf,"/star/institutions/rice/llope/muapp/run13/out_0%d.root",id);
		cout<<buf<<endl;
		TFile *f = new TFile(buf,"read");
		hcoco_vpd_w[i]		= (TH1D*)f->Get("hcoco_vpd_w");
		hcoco_vpd_w[i]->SetName(Form("hcoco_vpd_w%d",i));
		hcoco_vpd_w[i]->SetTitle(Form("hcoco_vpd_w %d",id));
		hcoco_vpd_w[i]->SetDirectory(0);
		hcoco_vpd_e[i]		= (TH1D*)f->Get("hcoco_vpd_e");
		hcoco_vpd_e[i]->SetName(Form("hcoco_vpd_e%d",i));
		hcoco_vpd_e[i]->SetTitle(Form("hcoco_vpd_e %d",id));
		hcoco_vpd_e[i]->SetDirectory(0);
		hcocokeep_vpd_w[i]	= (TH1D*)f->Get("hcocokeep_vpd_w");
		hcocokeep_vpd_w[i]->SetName(Form("hcocokeep_vpd_w%d",i));
		hcocokeep_vpd_w[i]->SetDirectory(0);
		hcocokeep_vpd_e[i]	= (TH1D*)f->Get("hcocokeep_vpd_e");
		hcocokeep_vpd_e[i]->SetName(Form("hcocokeep_vpd_e%d",i));
		hcocokeep_vpd_e[i]->SetDirectory(0);
		f->Close();
	}

	gStyle->SetOptStat(10);

	++ican; sprintf(buf,"ccan%d",ican);
	ccan[ican] = new TCanvas(buf,buf,0,0,1000,800);
	ccan[ican]->cd(); ccan[ican]->Divide(5,4,0.0001,0.0001);
	for (int id=74;id<=94;id++){
		int	i = id-74;
		ccan[ican]->cd(i+1);
		hcoco_vpd_e[i]->GetXaxis()->SetRangeUser(100,132);
		hcoco_vpd_e[i]->Draw();
		hcocokeep_vpd_e[i]->Draw("same");
		hcoco_vpd_w[i]->Draw("same");
	}
	ccan[ican]->cd(); ccan[ican]->Update();
//	sprintf(buf,"ps/%s.ps",ouf);
//	ccan[ican]->Print(buf);
	

}