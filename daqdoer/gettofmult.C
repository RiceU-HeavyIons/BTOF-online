void gettofmult(){
	
	int krun	= 13114037;
	TH1::AddDirectory(kFALSE);
	
	char buf[200];
	sprintf(buf,"/star/institutions/rice/llope/dd/daqdoer_%d_tof.root",krun);
	TFile *f = new TFile(buf,"read");
		TH1D *hnhitstof_tof	= (TH1D*)gDirectory->Get("hnhitstof_tof");
		TH1D *hnhitstof_trg	= (TH1D*)gDirectory->Get("hnhitstof_trg");
		TH2D *hnhitstof_trgtof	= (TH2D*)gDirectory->Get("hnhitstof_trgtof");
		hnhitstof_tof->SetDirectory(0);
		hnhitstof_trg->SetDirectory(0);
		hnhitstof_trgtof->SetDirectory(0);
		TH1D *hnhitstof_tof_low	= (TH1D*)gDirectory->Get("hnhitstof_tof_low");
		TH1D *hnhitstof_trg_low	= (TH1D*)gDirectory->Get("hnhitstof_trg_low");
		TH2D *hnhitstof_trgtof_low	= (TH2D*)gDirectory->Get("hnhitstof_trgtof_low");
		hnhitstof_tof_low->SetDirectory(0);
		hnhitstof_trg_low->SetDirectory(0);
		hnhitstof_trgtof_low->SetDirectory(0);
	f->Close();
	
	hnhitstof_tof->Draw();
	
	TFile *fout = new TFile("nhitstofuu.root","recreate");
	fout->cd();
		hnhitstof_tof->Write();
		hnhitstof_trg->Write();
		hnhitstof_trgtof->Write();
		hnhitstof_tof_low->Write();
		hnhitstof_trg_low->Write();
		hnhitstof_trgtof_low->Write();
	fout->ls();
	fout->Close();
	
	gSystem->Exec("cp -v nhitstofuu.root ~/glauberUU/");
}