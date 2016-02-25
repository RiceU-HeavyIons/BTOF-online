void trigcross_compare(){

	char buf[100];
//	const int nruns	= 9;
//	int runs[nruns] = {14061044,14061045,14061046,14061047,14061048,14061049,14061051,14061052,14061055};
//	const int nruns	= 5;
//	int runs[nruns] = {14062095,14062096,14063017,14063090,14063091};
//	const int nruns	= 3;
//	int runs[nruns] = {14065031,14065032,14065042};
//	const int nruns	= 3;
//	int runs[nruns] = {14067053,14067061,14067065};
//	const int nruns	= 10;
//	int runs[nruns] = {14067051,14067053,14067054,14067055,14067057,14067059,14067061,14067063,14067065,14068133};

	const int nruns	= 6;
	int runs[nruns] = {14069121,14069123,14069120,14070007,14070026,14070027};

	int ican	= -1;
	TCanvas*	ccan[100];
	
	const char* rdo[6] = {"THUB1","THUB2","THUB3","THUB4","VPD-W","VPD-E"};

	TH1D *htrgdtime[6][nruns];
	TH1D *htrgdtimekeep[6][nruns];
	for (int ir=0;ir<nruns;ir++){
		sprintf(buf,"/star/institutions/rice/llope/dd/daqdoer_%d_tof.root",runs[ir]);
		TFile *f = new TFile(buf,"READ");
		//
		for (int idet=0;idet<6;idet++){
			sprintf(buf,"htrgdtime%d",idet); 
			//cout<<buf<<endl;
			htrgdtime[idet][ir]	= (TH1D*)f->Get(buf);
			htrgdtime[idet][ir]->SetDirectory(0);
			sprintf(buf,"htrgdtime%d%d",idet,ir);
			//cout<<buf<<endl;
			htrgdtime[idet][ir]->SetName(buf);
			sprintf(buf,"%s hit #Deltatime, %d",rdo[idet],runs[ir]);
			//cout<<buf<<endl;
			htrgdtime[idet][ir]->SetTitle(buf);
			sprintf(buf,"htrgdtimekeep%d",idet);
			//cout<<buf<<endl;
			htrgdtimekeep[idet][ir]	= (TH1D*)f->Get(buf);
			htrgdtimekeep[idet][ir]->SetDirectory(0);
			sprintf(buf,"htrgdtimekeep%d%d",idet,ir);
			//cout<<buf<<endl;
			htrgdtimekeep[idet][ir]->SetName(buf);
			sprintf(buf,"%s hit #Deltatime keep, %d",rdo[idet],runs[ir]);
			//cout<<buf<<endl;
			htrgdtimekeep[idet][ir]->SetTitle(buf);
		}
		//
		f->Close();
	}

	gStyle->SetOptStat(0);
	gStyle->SetPadGridX(0);
	gStyle->SetPadGridY(0);
	
	for (int idet=0;idet<6;idet++){
		++ican; sprintf(buf,"ccan%d",ican);
		ccan[ican] = new TCanvas(buf,buf,0,0,750.,750.*(8.5/11.));
		ccan[ican]->SetFillColor(10);
		ccan[ican]->cd(); ccan[ican]->Divide(3,2,0.0001,0.0001);
		for (int ir=0;ir<nruns;ir++){
			ccan[ican]->cd(1+ir);
			gPad->SetLogy(1);
			if (idet<4){
				htrgdtime[idet][ir]->GetXaxis()->SetRangeUser(2600,3100);
			} else {
				htrgdtime[idet][ir]->GetXaxis()->SetRangeUser(0,500);
			}
			htrgdtime[idet][ir]->Draw();
			htrgdtimekeep[idet][ir]->Draw("same");
		}
		ccan[ican]->cd(); ccan[ican]->Update();
		if (idet==0){ ccan[ican]->Print("trigcross_compare.ps("); }
		       else { ccan[ican]->Print("trigcross_compare.ps"); }
	}


	ccan[ican]->Print("trigcross_compare.ps]");
	sprintf(buf,"ps2pdf trigcross_compare.ps trigcross_compare.pdf");
	gSystem->Exec(buf);
	sprintf(buf,"cp -v trigcross_compare.pdf /star/u/llope/WWW/files/");
	gSystem->Exec(buf);


}