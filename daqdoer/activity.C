#define activity_cxx
#include "activity.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

void activity::Loop()
{
	cout<<"Detector = "<<Detector<<endl;

	TH1D *hnevts_time	= new TH1D("hnevts_time","hnevts_time",5400,0.0,54000);
	TH1D *hnhits_time	= new TH1D("hnhits_time","hnhits_time",5400,0.0,54000);

	if (fChain == 0) return;

	//Long64_t nentries = fChain->GetEntries();
	Long64_t nentries = 9479760;
//	Long64_t nentries = 947976;
	cout<<"nentries = "<<nentries<<endl;

	Long64_t nbytes = 0, nb = 0;
	for (Long64_t jentry=0; jentry<nentries;jentry++) {
		Long64_t ientry = LoadTree(jentry);
		if (ientry < 0) break;
		if (jentry%100000==0) cout<<"Processing "<<jentry<<endl;
		nb = fChain->GetEntry(jentry);   nbytes += nb;

		hnevts_time->Fill(time,1);
		hnhits_time->Fill(time,nHitsLE);
		//cout<<time<<" "<<nHitsLE<<endl;
		
	}

	cout<<hnevts_time->GetEntries()<<endl;
	cout<<hnhits_time->GetEntries()<<endl;
	
	hnhits_time->Divide(hnevts_time);
	
	hnevts_time->SetLineColor(4);
	hnevts_time->SetMinimum(0.1);
	hnevts_time->Draw();
	hnhits_time->Draw("same");
	gPad->SetLogy(1);
	
}
