#define view_cxx
#include "view.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

char *names[16] = {	"RPWHI1",
					"RPWHI2",
					"RPWHO1",
					"RPWHO2",
					"RPWVU1",
					"RPWVU2",
					"RPWVD1",
					"RPWVD2",
					"RPEHO1",
					"RPEHO2",
					"RPEHI1",
					"RPEHI2",
					"RPEVU1",
					"RPEVU2",
					"RPEVD1",
					"RPEVD2"};

void view::Loop()
{

	char buf[60];
	
	TFile *fout	= new TFile("view.root","RECREATE");
		fout->cd();
	TH1D *adc[16],*tac[16],*adc_wtac[16],*adc_notac[16],*adc_oftac[16];
	for (int i=0;i<16;i++){
		sprintf(buf,"adc%d",i);
		adc[i]	= new TH1D(buf,buf,4096,-0.5,4095.5);
			adc[i]->SetLineColor(1);							// black adc all
		sprintf(buf,"tac%d",i);
		tac[i]	= new TH1D(buf,buf,4096,-0.5,4095.5);
			tac[i]->SetLineColor(4);							// blue tac all
		sprintf(buf,"adc_wtac%d",i);
		adc_wtac[i]	= new TH1D(buf,buf,4096,-0.5,4095.5);
			adc_wtac[i]->SetLineColor(3);						// green adc w/ tac
		sprintf(buf,"adc_notac%d",i);
		adc_notac[i]	= new TH1D(buf,buf,4096,-0.5,4095.5);
			adc_notac[i]->SetLineColor(2);						// red adc w/out tac
		sprintf(buf,"adc_oftac%d",i);
		adc_oftac[i]	= new TH1D(buf,buf,4096,-0.5,4095.5);
			adc_oftac[i]->SetLineColor(6);						// magenta w/ OF tac
	}

	if (fChain == 0) return;
	Long64_t nentries = fChain->GetEntriesFast();
	nentries=100001;
	Long64_t nbytes = 0, nb = 0;
	for (Long64_t jentry=0; jentry<nentries;jentry++) {
		Long64_t ientry = LoadTree(jentry);
		if (ientry < 0) break;
		nb = fChain->GetEntry(jentry);   nbytes += nb;
		// if (Cut(ientry) < 0) continue;
		if ((ientry+1)%50000==0){cout<<"Processing "<<ientry<<endl;}
		
		float ulim;
		for (int i=0;i<16;i++){
			adc[i]->Fill(p2p_adc[i],1.0);					// black
			tac[i]->Fill(p2p_tac[i],1.0);					// blue
			ulim	= 1700.;
			if (i==10){ ulim=1200.; }
			if (p2p_tac[i]>=100 && p2p_tac[i]<ulim){		// green
				adc_wtac[i]->Fill(p2p_adc[i],1.0); 
			} else if (p2p_tac[i]<100) {		 			// red
				adc_notac[i]->Fill(p2p_adc[i],1.0);
			} else if (p2p_tac[i]>=ulim) {				 	// magenta 
				adc_oftac[i]->Fill(p2p_adc[i],1.0);
			}
		}
		
	}	//---- end event loop.....

	gStyle->SetOptStat(0);
	gStyle->SetPadLeftMargin(0.08);
	gStyle->SetPadRightMargin(0.02);
	gStyle->SetPadTopMargin(0.02);
	gStyle->SetPadBottomMargin(0.06);
	TLatex *tt = new TLatex(0.0,0.0," ");
		tt->SetNDC();
		tt->SetTextSize(0.15);
		tt->SetTextColor(6);

	TCanvas *view_1 = new TCanvas("view_1","view_1",10,10,640,460);
			 view_1->Clear(); view_1->Divide(4,4,0.0001,0.0001);
	for (int i=0;i<16;i++){
		view_1->cd(1+i);
		adc[i]->SetMinimum(0.1);
		adc[i]->SetTitle(0);
		adc[i]->Draw();
			gPad->SetLogy(1);
			tt->DrawLatex(0.27,0.79,names[i]);
	}
	view_1->cd(); view_1->Update(); 
	view_1->Print("view_1.eps");

	TCanvas *view_2 = new TCanvas("view_2","view_2",35,35,640,460);
			 view_2->Clear(); view_2->Divide(4,4,0.001,0.001);
	for (int i=0;i<16;i++){
		view_2->cd(1+i);
		tac[i]->SetMinimum(0.1);
		tac[i]->SetTitle(0);
		tac[i]->Draw();
			gPad->SetLogy(1);
			tt->DrawLatex(0.27,0.79,names[i]);
	}
	view_2->cd(); view_2->Update(); 
	view_2->Print("view_2.eps");
	
	TCanvas *view_3 = new TCanvas("view_3","view_3",60,60,960,690);
			 view_3->Clear(); view_3->Divide(4,4,0.0001,0.0001);
	for (int i=0;i<16;i++){
		view_3->cd(1+i);
		adc[i]->SetMinimum(0.1);
		adc[i]->GetXaxis()->SetRangeUser(-0.5,59.5);
		adc[i]->SetTitle(0);
		adc[i]->Draw();
		adc_wtac[i]->Draw("same");
		adc_notac[i]->Draw("same");
		adc_oftac[i]->Draw("same");
			gPad->SetLogy(1);
			tt->DrawLatex(0.27,0.79,names[i]);
	}
	view_3->cd(); view_3->Update(); 
	view_3->Print("view_3.eps");

	TCanvas *view_4 = new TCanvas("view_4","view_4",85,85,960,690);
			 view_4->Clear(); view_4->Divide(4,4,0.001,0.001);
	for (int i=0;i<16;i++){
		view_4->cd(1+i);
		tac[i]->SetMinimum(0.1);
		tac[i]->GetXaxis()->SetRangeUser(-0.5,119.5);
		tac[i]->SetTitle(0);
		tac[i]->Draw();
			gPad->SetLogy(1);
			tt->DrawLatex(0.27,0.79,names[i]);
	}
	view_4->cd(); view_4->Update(); 
	view_4->Print("view_4.eps");
	


	fout->Write();

}
