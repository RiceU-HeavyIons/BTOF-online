#define tptest_cxx
#include "tptest.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

void tptest::Loop()
{
	int			ican=-1;
	int			ipad;
	TCanvas*	ccan[100];
	char		buf[100];

	char psfileopen[100];
	char psfile[100];
	char pdffile[100];
	char psfileclose[100];
	sprintf(psfileopen,"tptest.ps(");
	sprintf(psfile,"tptest.ps");
	sprintf(pdffile,"tptest.pdf");
	sprintf(psfileclose,"tptest.ps]");

	TFile *f = new TFile("tptest.root","RECREATE");
	TH1D *hp_p2p_trg    	= new TH1D("hp_p2p_trg","hp_p2p_trg",16,-0.5,15.5);	
	TH1D *hp_p2p_tof    	= new TH1D("hp_p2p_tof","hp_p2p_tof",16,-0.5,15.5);	
	TH2D *map_p2p_trgtof	= new TH2D("map_p2p_trgtof","map_p2p_trgtof",16,-0.5,15.5,16,-0.5,15.5);
	TH1D *hp_vpd_trg    	= new TH1D("hp_vpd_trg","hp_vpd_trg",38,-0.5,37.5);	
	TH1D *hp_vpd_tof    	= new TH1D("hp_vpd_tof","hp_vpd_tof",38,-0.5,37.5);	
	TH2D *map_vpd_trgtof	= new TH2D("map_vpd_trgtof","map vpd trg vs tof",38,-0.5,37.5,38,-0.5,37.5);

	TH1D *hvpdadc[38];
	TH1D *hvpdtot[38];
	for (int i=0;i<38;i++){
		sprintf(buf,"hvpdadc%d",i);
			hvpdadc[i]	= new TH1D(buf,buf,100,0,100);
			hvpdadc[i]->SetLineColor(6);
		sprintf(buf,"hvpdtot%d",i);
			hvpdtot[i]	= new TH1D(buf,buf,100,0,100);
			hvpdtot[i]->SetLineColor(4);
	}

	if (fChain == 0) return;
	Long64_t nentries = fChain->GetEntries();
	
	nentries = 20000;
	
	Long64_t nbytes = 0, nb = 0;
	for (Long64_t jentry=0; jentry<nentries;jentry++) {
		Long64_t ientry = LoadTree(jentry);
		if (ientry < 0) break;
		nb = fChain->GetEntry(jentry);   nbytes += nb;
		// if (Cut(ientry) < 0) continue;

		if (ientry%10000==0){ cout<<"processing "<<ientry<<endl; }

		for (int i=0;i<16;i++){
			if (p2p_tle[i]>  0){ hp_p2p_tof->Fill(i); }
			if (p2p_tac[i]>100){ hp_p2p_trg->Fill(i); }
		}		
		for (int i=0;i<16;i++){					// tof loop....
			if (p2p_tle[i]>0){
				for (int j=0;j<16;j++){			// trg loop....
					if (p2p_tac[j]>100.0){
						map_p2p_trgtof->Fill(i,j,1.0);
					}
				}
			}
		}		

		int nhtof[2] = {0};
		int nhtrg[2] = {0};
		for (int i=0;i<38;i++){
			int kew	= i/19;
			if (vpd_tle[i]>  0                 ){ hp_vpd_tof->Fill(i); nhtof[kew]++; }
			if (vpd_tac[i]>100&&vpd_tac[i]<3800){ hp_vpd_trg->Fill(i); nhtrg[kew]++; }
			//
			if (vpd_adc[i]>0) hvpdadc[i]->Fill(vpd_adc[i]);
			if (vpd_tot[i]>0) hvpdtot[i]->Fill(vpd_tot[i]);
			//
			//if (i==5||i==13||i==14){
			//	cout<<i<<" "<<vpd_tac[i]<<endl;
			//}
		}
		if ( nhtof[0]==1 && nhtof[1]==1 ){
			for (int i=0;i<38;i++){					// tof loop....
				if (vpd_tle[i]>0){
					for (int j=0;j<38;j++){			// trg loop....
						if (vpd_tac[j]>100.0&&vpd_tac[j]<3800){
							map_vpd_trgtof->Fill(i,j,1.0);
						}
					}
				}
			}		
		}

	}	//---- end event loop...

	gStyle->SetOptStat(0);

 	++ican; sprintf(buf,"ccan%d",ican);
 	ccan[ican] = new TCanvas(buf,buf,0,0,750.,750.*(8.5/11.));
 	ccan[ican]->SetFillColor(10);
 	ccan[ican]->cd(); ccan[ican]->Divide(1,1,0.0001,0.0001);
		map_vpd_trgtof->Draw("colz");
	ccan[ican]->cd(); ccan[ican]->Update();
	ccan[ican]->Print(psfileopen);

 	++ican; sprintf(buf,"ccan%d",ican);
 	ccan[ican] = new TCanvas(buf,buf,0,0,750.,750.*(8.5/11.));
 	ccan[ican]->SetFillColor(10);
 	ccan[ican]->cd(); ccan[ican]->Divide(5,4,0.0001,0.0001);
 	for (int i=0;i<19;i++){
		ccan[ican]->cd(1+i);
		gPad->SetLogy(1);
		hvpdtot[i]->Draw();
		hvpdadc[i]->Draw("same");
	}
	ccan[ican]->cd(); ccan[ican]->Update();
	ccan[ican]->Print(psfile);
	//
 	++ican; sprintf(buf,"ccan%d",ican);
 	ccan[ican] = new TCanvas(buf,buf,0,0,750.,750.*(8.5/11.));
 	ccan[ican]->SetFillColor(10);
 	ccan[ican]->cd(); ccan[ican]->Divide(5,4,0.0001,0.0001);
 	for (int i=0;i<19;i++){
		ccan[ican]->cd(1+i);
		gPad->SetLogy(1);
		hvpdtot[i+19]->Draw();
		hvpdadc[i+19]->Draw("same");
	}
	ccan[ican]->cd(); ccan[ican]->Update();
	ccan[ican]->Print(psfile);
	

	//---- end...
	//
	ccan[ican]->Print(psfileclose);
	sprintf(buf,"ps2pdf %s %s",psfile,pdffile);
	gSystem->Exec(buf);
	sprintf(buf,"cp -v %s /star/u/llope/WWW/files/",pdffile);
	gSystem->Exec(buf);

	f->Write();
	f->Close();
		
}


