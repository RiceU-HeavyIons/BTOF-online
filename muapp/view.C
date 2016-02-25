#define view_cxx
#include "view.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

void view::Loop()
{

	char buf[80];
	char buft[100];
	char *trignames[32] = {"mb","mbslow","","ht-11","mtd",
						   "","","","vpd-mon","vpd-mon-tac",
						   "","","","","",
						   "","","zb","hlt","tof-mon",
						   "","","mb-mon","tof-mon-fast","vpd-tac-slow",
						   "cosmic","","","","",
						   "",""};
	TText *trn[100];
	for (int it=0;it<32;it++){
		trn[it] = new TText(-0.1+(float)it,0.75,trignames[it]);
		trn[it]->SetTextAngle(90);
		trn[it]->SetTextSize(0.04);
		trn[it]->SetTextAlign(12);		
	}
	int  	ncan = -1;
	TCanvas *ccan[100];

	TH1D *htrig = new TH1D("htrig","htrig",32,-0.5,31.5);
	TH1D *hzv[32];
	TH1D *hnprim[32];
	TH1D *hzv_nprim[32];
	TH1D *hvpdsum[32];
	TH1D *hbbcsum[32];
	TH2D *hnprim_vpdsum[32];
	TH2D *hnprim_bbcsum[32];
	TH2D *hnvpdew[32];
	TH2D *hnvpd_nprimary[32];
	TH1D *hnvpd_nprimarycuta[32];
	TH1D *hnvpd_nprimarycutb[32];
	TH1D *hnvpd_nprimarycutc[32];
	for (int it=0;it<32;it++){
		sprintf(buf,"hzv%d",it);
		sprintf(buft,"Zvtx, %s",trignames[it]);
		hzv[it]			= new TH1D(buf,buft,100,-200,200);
		sprintf(buf,"hnprim%d",it);
		sprintf(buft,"Nprimary, %s",trignames[it]);
		hnprim[it]		= new TH1D(buf,buft,100,0,600);
		sprintf(buf,"hzv_nprim%d",it);
		sprintf(buft,"Zvtx Nprimary>0, %s",trignames[it]);
		hzv_nprim[it]	= new TH1D(buf,buft,100,-200,200);
		sprintf(buf,"hvpdsum%d",it);
		sprintf(buft,"VPD ADC sum, %s",trignames[it]);
		hvpdsum[it]		= new TH1D(buf,buft,100,0,10000);
		sprintf(buf,"hbbcsum%d",it);
		sprintf(buft,"BBC ADC sum, %s",trignames[it]);
		hbbcsum[it]		= new TH1D(buf,buft,100,0,80000);

		sprintf(buf,"hnprim_vpdsum%d",it);
		sprintf(buft,"Nprimary vs VPDsum, %s",trignames[it]);
		hnprim_vpdsum[it]	= new TH2D(buf,buft,50,0,10000,50,0,600);
		sprintf(buf,"hnprim_bbcsum%d",it);
		sprintf(buft,"Nprimary vs bbcsum, %s",trignames[it]);
		hnprim_bbcsum[it]	= new TH2D(buf,buft,50,0,80000,50,0,600);

		sprintf(buf,"hnvpdew%d",it);
		sprintf(buft,"NvpdE vs NvpdW, %s",trignames[it]);
		hnvpdew[it]	= new TH2D(buf,buft,20,-0.5,19.5,20,-0.5,19.5);
		sprintf(buf,"hnvpd_nprimary%d",it);
		sprintf(buft,"NvpdE+W vs Nprimary, %s",trignames[it]);
		hnvpd_nprimary[it]	= new TH2D(buf,buft,50,0.0,600.0,39,-0.5,38.5);

		sprintf(buf,"hnvpd_nprimarycuta%d",it);
		sprintf(buft,"NvpdE+W, Nprimary<25, %s",trignames[it]);
		hnvpd_nprimarycuta[it]	= new TH1D(buf,buft,39,-0.5,38.5);
		sprintf(buf,"hnvpd_nprimarycutb%d",it);
		sprintf(buft,"NvpdE+W, Nprimary>25, %s",trignames[it]);
		hnvpd_nprimarycutb[it]	= new TH1D(buf,buft,39,-0.5,38.5);
		sprintf(buf,"hnvpd_nprimarycutc%d",it);
		sprintf(buft,"NvpdE+W, Nprimary>250, %s",trignames[it]);
		hnvpd_nprimarycutc[it]	= new TH1D(buf,buft,39,-0.5,38.5);

		
	}	


	if (fChain == 0) return;
	Long64_t nentries = fChain->GetEntries();
	cout<<" Nentries = "<<nentries<<endl;
	Long64_t nbytes = 0, nb = 0;
	for (Long64_t jentry=0; jentry<nentries;jentry++) {
		Long64_t ientry = LoadTree(jentry);
		if (ientry < 0) break;
		nb = fChain->GetEntry(jentry);   nbytes += nb;
		if (jentry%10000==0){ cout<<"Processing "<<jentry<<endl; }

		if (daqbits==0){ continue; }

		float bbcsum = 0.0;
		for (int i=0;i<48;i++){
			if (bbc_tac[i]>100.){
				bbcsum += bbc_adc[i];
			}
		}
		float vpdsum = 0.0;
		int ne=0, nw=0;
		for (int i=0;i<38;i++){
			if (vpd_tac[i]>100.){
				vpdsum += vpd_adc[i];
			}
			if (vpd_tle[i]>0){
				if (i<19){ ++nw; }else{ ++ne; }
			}
		}

		for (int ib=0;ib<32;ib++){
			if (daqbits & (int)pow(2,ib)){ 
				htrig->Fill(ib);
				hzv[ib]->Fill(vtx[2]);
				hnprim[ib]->Fill(nprimary);
				if (nprimary>0){
					hzv_nprim[ib]->Fill(vtx[2]);
					hnprim_vpdsum[ib]->Fill(vpdsum,nprimary);
					hnprim_bbcsum[ib]->Fill(bbcsum,nprimary);
				}
				hvpdsum[ib]->Fill(vpdsum);
				hbbcsum[ib]->Fill(bbcsum);
				hnvpdew[ib]->Fill(nw,ne);
				hnvpd_nprimary[ib]->Fill(nprimary,ne+nw);
				if (nprimary< 25){ hnvpd_nprimarycuta[ib]->Fill(ne+nw); }
				if (nprimary> 25){ hnvpd_nprimarycutb[ib]->Fill(ne+nw); }
				if (nprimary>250){ hnvpd_nprimarycutc[ib]->Fill(ne+nw); }
				
			}
		}

	}
	
	gStyle->SetOptStat(0);
	gStyle->SetTitleFontSize(0.08);
	gStyle->SetNdivisions(-4,"X");

	++ncan;
	sprintf(buf,"ccan%d",ncan);
	ccan[ncan] = new TCanvas(buf,buf,0,0,640,460);
	ccan[ncan]->SetFillColor(10);
	ccan[ncan]->cd(); ccan[ncan]->Divide(1,1,0.0001,0.0001);
		ccan[ncan]->cd(1);
		htrig->SetFillStyle(1001);
		htrig->SetFillColor(5);
		htrig->SetMinimum(0.5);
		htrig->Draw();
		gPad->SetLogy(1);
		for (int it=0;it<32;it++){
			trn[it]->Draw("same");
		}
	ccan[ncan]->cd(); ccan[ncan]->Update();
	ccan[ncan]->Print("view.ps(");

	int kwin;

// 	kwin = 0;
// 	++ncan;
// 	sprintf(buf,"ccan%d",ncan);
// 	ccan[ncan] = new TCanvas(buf,buf,0,0,640,460);
// 	ccan[ncan]->SetFillColor(10);
// 	ccan[ncan]->cd(); ccan[ncan]->Divide(5,2,0.0001,0.0001);
// 		for (int it=0;it<32;it++){
// 			if (hzv[it]->GetEntries()){
// 				++kwin;
// 				ccan[ncan]->cd(kwin);
// 				gPad->SetLogy(1);
// 				hzv[it]->SetFillStyle(1001);
// 				hzv[it]->SetFillColor(7);
// 				hzv[it]->Draw();
// 			}
// 		}
// 	ccan[ncan]->cd(); ccan[ncan]->Update();
// 	ccan[ncan]->Print("view.ps");

	kwin = 0;
	++ncan;
	sprintf(buf,"ccan%d",ncan);
	ccan[ncan] = new TCanvas(buf,buf,0,0,640,460);
	ccan[ncan]->SetFillColor(10);
	ccan[ncan]->cd(); ccan[ncan]->Divide(5,2,0.0001,0.0001);
		for (int it=0;it<32;it++){
			if (hzv[it]->GetEntries()){
				++kwin;
				ccan[ncan]->cd(kwin);
				gPad->SetLogy(1);
				hnprim[it]->SetFillStyle(1001);
				hnprim[it]->SetFillColor(7);
				hnprim[it]->Draw();
			}
		}
	ccan[ncan]->cd(); ccan[ncan]->Update();
	ccan[ncan]->Print("view.ps");

	kwin = 0;
	++ncan;
	sprintf(buf,"ccan%d",ncan);
	ccan[ncan] = new TCanvas(buf,buf,0,0,640,460);
	ccan[ncan]->SetFillColor(10);
	ccan[ncan]->cd(); ccan[ncan]->Divide(5,2,0.0001,0.0001);
		for (int it=0;it<32;it++){
			if (hzv[it]->GetEntries()){
				++kwin;
				ccan[ncan]->cd(kwin);
				gPad->SetLogy(1);
				hzv_nprim[it]->SetFillStyle(1001);
				hzv_nprim[it]->SetFillColor(7);
				hzv_nprim[it]->Draw();
			}
		}
	ccan[ncan]->cd(); ccan[ncan]->Update();
	ccan[ncan]->Print("view.ps");

	kwin = 0;
	++ncan;
	sprintf(buf,"ccan%d",ncan);
	ccan[ncan] = new TCanvas(buf,buf,0,0,640,460);
	ccan[ncan]->SetFillColor(10);
	ccan[ncan]->cd(); ccan[ncan]->Divide(5,2,0.0001,0.0001);
		for (int it=0;it<32;it++){
			if (hzv[it]->GetEntries()){
				++kwin;
				ccan[ncan]->cd(kwin);
				gPad->SetLogy(1);
				hvpdsum[it]->SetFillStyle(1001);
				hvpdsum[it]->SetFillColor(7);
				hvpdsum[it]->Draw();
			}
		}
	ccan[ncan]->cd(); ccan[ncan]->Update();
	ccan[ncan]->Print("view.ps");

	kwin = 0;
	++ncan;
	sprintf(buf,"ccan%d",ncan);
	ccan[ncan] = new TCanvas(buf,buf,0,0,640,460);
	ccan[ncan]->SetFillColor(10);
	ccan[ncan]->cd(); ccan[ncan]->Divide(5,2,0.0001,0.0001);
		for (int it=0;it<32;it++){
			if (hzv[it]->GetEntries()){
				++kwin;
				ccan[ncan]->cd(kwin);
				gPad->SetLogy(1);
				hbbcsum[it]->SetFillStyle(1001);
				hbbcsum[it]->SetFillColor(7);
				hbbcsum[it]->Draw();
			}
		}
	ccan[ncan]->cd(); ccan[ncan]->Update();
	ccan[ncan]->Print("view.ps");

	kwin = 0;
	++ncan;
	sprintf(buf,"ccan%d",ncan);
	ccan[ncan] = new TCanvas(buf,buf,0,0,640,460);
	ccan[ncan]->SetFillColor(10);
	ccan[ncan]->cd(); ccan[ncan]->Divide(5,2,0.0001,0.0001);
		for (int it=0;it<32;it++){
			if (hzv[it]->GetEntries()){
				++kwin;
				ccan[ncan]->cd(kwin);
				hnprim_vpdsum[it]->Draw("colz");
			}
		}
	ccan[ncan]->cd(); ccan[ncan]->Update();
	ccan[ncan]->Print("view.ps");

	kwin = 0;
	++ncan;
	sprintf(buf,"ccan%d",ncan);
	ccan[ncan] = new TCanvas(buf,buf,0,0,640,460);
	ccan[ncan]->SetFillColor(10);
	ccan[ncan]->cd(); ccan[ncan]->Divide(5,2,0.0001,0.0001);
		for (int it=0;it<32;it++){
			if (hzv[it]->GetEntries()){
				++kwin;
				ccan[ncan]->cd(kwin);
				hnprim_bbcsum[it]->Draw("colz");
			}
		}
	ccan[ncan]->cd(); ccan[ncan]->Update();
	ccan[ncan]->Print("view.ps");
	
	kwin = 0;
	++ncan;
	sprintf(buf,"ccan%d",ncan);
	ccan[ncan] = new TCanvas(buf,buf,0,0,640,460);
	ccan[ncan]->SetFillColor(10);
	ccan[ncan]->cd(); ccan[ncan]->Divide(5,2,0.0001,0.0001);
		for (int it=0;it<32;it++){
			if (hzv[it]->GetEntries()){
				++kwin;
				ccan[ncan]->cd(kwin);
				hnvpdew[it]->Draw("colz");
			}
		}
	ccan[ncan]->cd(); ccan[ncan]->Update();
	ccan[ncan]->Print("view.ps");

	kwin = 0;
	++ncan;
	sprintf(buf,"ccan%d",ncan);
	ccan[ncan] = new TCanvas(buf,buf,0,0,640,460);
	ccan[ncan]->SetFillColor(10);
	ccan[ncan]->cd(); ccan[ncan]->Divide(5,2,0.0001,0.0001);
		for (int it=0;it<32;it++){
			if (hzv[it]->GetEntries()){
				++kwin;
				ccan[ncan]->cd(kwin);
				hnvpd_nprimary[it]->Draw("colz");
			}
		}
	ccan[ncan]->cd(); ccan[ncan]->Update();
	ccan[ncan]->Print("view.ps");
	
	TText *txta = new TText(); txta->SetNDC(); txta->SetTextColor(4); txta->SetTextSize(0.08);
	TText *txtb = new TText(); txtb->SetNDC(); txtb->SetTextColor(6); txtb->SetTextSize(0.08);
	TText *txtc = new TText(); txtc->SetNDC(); txtc->SetTextColor(2); txtc->SetTextSize(0.08);
	kwin = 0;
	++ncan;
	sprintf(buf,"ccan%d",ncan);
	ccan[ncan] = new TCanvas(buf,buf,0,0,640,460);
	ccan[ncan]->SetFillColor(10);
	ccan[ncan]->cd(); ccan[ncan]->Divide(5,2,0.0001,0.0001);
		for (int it=0;it<32;it++){
			if (hzv[it]->GetEntries()){
				++kwin;
				ccan[ncan]->cd(kwin);
				gPad->SetLogy(1);
				hnvpd_nprimarycuta[it]->SetMaximum(40000);
				hnvpd_nprimarycuta[it]->SetLineColor(4);
				hnvpd_nprimarycuta[it]->Draw();
				hnvpd_nprimarycutb[it]->SetLineColor(6);
				hnvpd_nprimarycutb[it]->Draw("same");
				hnvpd_nprimarycutc[it]->SetLineColor(2);
				hnvpd_nprimarycutc[it]->Draw("same");
				if (kwin==1){
					txta->DrawText(0.36,0.84,"Nprimary<25");
					txtb->DrawText(0.36,0.77,"Nprimary>25");
					txtc->DrawText(0.36,0.70,"Nprimary>250");
				}
			}
		}
	ccan[ncan]->cd(); ccan[ncan]->Update();
	ccan[ncan]->Print("view.ps");
	
	
	
	
	sprintf(buf,"view.ps]");
	cout<<"..... Closing ps file.... "<<buf<<endl;
	ccan[ncan]->Print(buf);
	sprintf(buf,"/usr/bin/ps2pdf view.ps view.pdf");
	cout<<"..... Executing ... "<<buf<<endl;
	gSystem->Exec(buf);
	sprintf(buf,"/bin/cp view.pdf /star/u/geurts/WWW/files/");
	cout<<"..... Executing ... "<<buf<<endl;
	gSystem->Exec(buf);
	cout<<"..... Done...."<<endl;	
	
}
