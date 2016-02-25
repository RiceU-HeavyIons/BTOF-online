#include "readout_class.h"

const Int_t	NDETSIDE	= 19;

void readout::Loop(Int_t ntodo)
{

	gErrorIgnoreLevel = 1001;
	char	filestring[100];
	char 	buf[80];
	char 	buf2[80];
	int		ican = -1;
	TCanvas *ccan[100];
	int		idetew;
	float	ear[2];
	float 	vtxerror;
	int		nvpd[2];
	float	zdcsum,bbcsum,vpdsum;
	
	Float_t	tdoffset_vpd =  50.;
	Float_t	tdoffset_bbc = -50.;

	sprintf(filestring,"readout_%d",kDayUse);
	TString namebase	= TString(filestring);
	TString nameextr	= TString(".root");
	TString nameroot	= namebase+nameextr;
	TString nameextp	= TString(".ps");
	TString nameposc	= namebase+nameextp;
	cout<<"..... readout::loop Opening "<<nameroot.Data()<<endl;
	TFile *fout = new TFile(nameroot.Data(),"RECREATE");
	fout->cd();
	TH1D *hvtxe		= new TH1D("hvtxe","hvtxe",1000,0,10);
 	TH1D *havpd		= new TH1D("havpd","havpd",1000,0,4000);
 	TH1D *habbc		= new TH1D("habbc","habbc",1000,0,4000);
 	TH1D *havpd_wtac= new TH1D("havpd_wtac","havpd_wtac",1000,0,4000);
 	TH1D *habbc_wtac= new TH1D("habbc_wtac","habbc_wtac",1000,0,4000);
 	TH1D *htvpd		= new TH1D("htvpd","htvpd",1000,0,4000);
 	TH1D *htbbc		= new TH1D("htbbc","htbbc",1000,0,4000);
 	TH2D *htdvpd	= new TH2D("htdvpd","htdvpd",80,-80,80,75,-300,300);
 	TH2D *htdbbc	= new TH2D("htdbbc","htdbbc",80,-80,80,75,-300,300);
	TH2D *hnvpde_bbc = new TH2D("hnvpde_bbc","Nvpd E vs BBC ADC sum",50,0.0,150000,19,-0.5,18.5);
	TH2D *hnvpdw_bbc = new TH2D("hnvpdw_bbc","Nvpd E vs BBC ADC sum",50,0.0,150000,19,-0.5,18.5);
	TH2D *hnvpd_bbc  = new TH2D("hnvpd_bbc" ,"Nvpd vs BBC ADC sum"  ,50,0.0,150000,38,-0.5,37.5);
	TH2D *hnvpde_zdc = new TH2D("hnvpde_zdc","Nvpd E vs ZDC ADC sum",50,0.0,50000,19,-0.5,18.5);
	TH2D *hnvpdw_zdc = new TH2D("hnvpdw_zdc","Nvpd E vs ZDC ADC sum",50,0.0,50000,19,-0.5,18.5);
	TH2D *hnvpd_zdc  = new TH2D("hnvpd_zdc" ,"Nvpd vs ZDC ADC sum"  ,50,0.0,50000,38,-0.5,37.5);

	//---- event loop....
	if (fChain == 0) return;
	Long64_t nentries = fChain->GetEntries();
	cout<<"..... readout::loop Nentries = "<<nentries<<endl;
	//nentries = 100000;
	Long64_t nb = 0;
	for (Long64_t jentry=0; jentry<nentries;jentry++) {
		Long64_t ientry = LoadTree(jentry);
		if (ientry < 0) break;
		nb = fChain->GetEntry(jentry);
		if (jentry%10000==0){cout<<"1 Processing "<<jentry<<"  run "<<run<<endl;}

		vtxerror = vtxe[2];
		hvtxe->Fill(vtxerror);
		zdcsum = bbcsum = vpdsum = 0.0;
		
		if (vtxerror>0.0 && vtxerror<0.15){
			ear[0] = ear[1] = 0.0;
			for (int is=0;is<2;is++){
				for (int id=0;id<19;id++){
					idetew	= id + is*NDETSIDE;
					havpd->Fill(vpd_adc[idetew],1.0);
					htvpd->Fill(vpd_tac[idetew],1.0);
					if ( vpd_tac[idetew] >  100
					  && vpd_tac[idetew] < 3500
					  && vpd_adc[idetew] >   10 ){
						havpd_wtac->Fill(vpd_adc[idetew],1.0);
						vpdsum	+= vpd_adc[idetew];
						if ( vpd_tac[idetew] > ear[is] ){
							ear[is]	= vpd_tac[idetew];
						}
					}
				}
			}			
			if (ear[0]>0&&ear[1]>0){
				htdvpd->Fill(vtx[2],ear[0]-ear[1]-tdoffset_vpd);
			}
			//
			ear[0] = ear[1] = 0.0;
			for (int is=0;is<2;is++){
				for (int id=0;id<24;id++){
					idetew	= id + is*24;
					habbc->Fill(bbc_adc[idetew],1.0);
					htbbc->Fill(bbc_tac[idetew],1.0);
					if ( bbc_tac[idetew] >  100
					  && bbc_tac[idetew] < 3200
					  && bbc_adc[idetew] >   10 ){
						habbc_wtac->Fill(bbc_adc[idetew],1.0);
						bbcsum	+= bbc_adc[idetew];
						if ( bbc_tac[idetew] > ear[is] ){
							ear[is]	= bbc_tac[idetew];
						}
					}
				}
			}			
			if (ear[0]>0&&ear[1]>0&&ear[0]!=ear[1]){
				htdbbc->Fill(vtx[2],ear[0]-ear[1]-tdoffset_bbc);
			}
		}

		nvpd[0]	= nvpd[1] = 0;
		for (int is=0;is<2;is++){
			for (int id=0;id<19;id++){
				idetew	= id + is*NDETSIDE;
				if ( vpd_tac[idetew] >  100
				  && vpd_tac[idetew] < 3500
				  && vpd_adc[idetew] >   10 ){
				  	++nvpd[is];
				}				
			}
		}
		for (int is=0;is<2;is++){
			for (int id=0;id<8;id++){
				idetew	= id + is*8;
				zdcsum += zdc_adc[idetew];
			}
		}
		hnvpdw_bbc->Fill(bbcsum,nvpd[0]);
		hnvpde_bbc->Fill(bbcsum,nvpd[1]);
		hnvpd_bbc->Fill(bbcsum,nvpd[0]+nvpd[1]);
		hnvpdw_zdc->Fill(zdcsum,nvpd[0]);
		hnvpde_zdc->Fill(zdcsum,nvpd[1]);
		hnvpd_zdc->Fill(zdcsum,nvpd[0]+nvpd[1]);
		

	}
	//---- end event loop....

	//-------------------------------------------------------------------------
	//---- make plots....
	//
	gStyle->SetOptStat(0);
	gStyle->SetOptFit(1111);
	gStyle->SetPadBottomMargin(0.07);
	gStyle->SetPadTopMargin(0.05);
	gStyle->SetPadLeftMargin(0.08);
	gStyle->SetPadRightMargin(0.11);
	//
	//---- TAC bin widths...
	Double_t	lpars[2];
	TF1 *flin	= new TF1("flin","pol1",-21,21);
    TLatex *tt	= new TLatex();
	 	  	tt->SetTextSize(0.05);
	 		tt->SetNDC(kTRUE);
	Double_t cal;
	
	htdvpd->FitSlicesY();
	TH1D *htdvpd_pfx = (TH1D*)gDirectory->Get("htdvpd_1");
			htdvpd_pfx->SetMarkerStyle(20);
			htdvpd_pfx->SetMarkerColor(2);
	htdvpd_pfx->Fit("flin","NQR");
		flin->GetParameters(lpars);
		cal = 30.0*lpars[1]/2.0;
		cout<<"VPD .... "<<lpars[0]<<" "<<lpars[1]<<"\t >>--> "<<1000./cal<<" ps/bin"<<endl;
 	++ican; sprintf(buf,"ccan%d",ican);
 	ccan[ican] = new TCanvas(buf,buf,0,0,1200,800);
 	ccan[ican]->SetFillColor(10);
 	ccan[ican]->cd(); ccan[ican]->Divide(1,1,0.0001,0.0001);
		ccan[ican]->cd(1);
			htdvpd->Draw("colz");
			htdvpd_pfx->Draw("same");
			flin->Draw("same");
	ccan[ican]->cd(); ccan[ican]->Update();
		sprintf(buf,"slope=%4.2f tacbins/cm  #Rightarrow  %4.2f ps/tacbin",lpars[1],1000./cal);
		   	tt->DrawLatex(0.18,0.91,buf);
		sprintf(buf,"Tac #Deltat offset = %d",(int)tdoffset_vpd);
	 	  	tt->SetTextSize(0.038);
		   	tt->DrawLatex(0.12,0.84,buf);
	ccan[ican]->Print("readout.ps(");
	//
	flin->SetRange(-40,40);
	tt->SetTextSize(0.05);
	htdbbc->FitSlicesY();
	TH1D *htdbbc_pfx = (TH1D*)gDirectory->Get("htdbbc_1");
			htdbbc_pfx->SetMarkerStyle(20);
			htdbbc_pfx->SetMarkerColor(2);
	htdbbc_pfx->Fit("flin","NQR");
		flin->GetParameters(lpars);
		cal = 30.0*lpars[1]/2.0;
		cout<<"BBC .... "<<lpars[0]<<" "<<lpars[1]<<"\t >>--> "<<1000./cal<<" ps/bin"<<endl;
	++ican; sprintf(buf,"ccan%d",ican);
	ccan[ican] = new TCanvas(buf,buf,0,0,1200,800);
	ccan[ican]->SetFillColor(10);
	ccan[ican]->cd(); ccan[ican]->Divide(1,1,0.0001,0.0001);
		ccan[ican]->cd(1);
			htdbbc->Draw("colz");
			htdbbc_pfx->Draw("same");
			flin->Draw("same");
	ccan[ican]->cd(); ccan[ican]->Update();
		sprintf(buf,"slope=%4.2f tacbins/cm  #Rightarrow  %4.2f ps/tacbin",lpars[1],1000./cal);
			tt->DrawLatex(0.18,0.91,buf);
		sprintf(buf,"Tac #Deltat offset = %d",(int)tdoffset_bbc);
	 	  	tt->SetTextSize(0.038);
		   	tt->DrawLatex(0.12,0.84,buf);
	ccan[ican]->Print("readout.ps");

	TProfile *hnvpdw_bbc_pr = (TProfile*)hnvpdw_bbc->ProfileX();
	TProfile *hnvpde_bbc_pr = (TProfile*)hnvpde_bbc->ProfileX();
	TProfile *hnvpd_bbc_pr  = (TProfile*)hnvpd_bbc->ProfileX();
		hnvpdw_bbc_pr->SetMarkerStyle(20);	hnvpdw_bbc_pr->SetMarkerColor(7);
		hnvpde_bbc_pr->SetMarkerStyle(20);	hnvpde_bbc_pr->SetMarkerColor(7);
		hnvpd_bbc_pr->SetMarkerStyle(20);	hnvpd_bbc_pr->SetMarkerColor(7);
 	ccan[ican] = new TCanvas(buf,buf,0,0,1200,800);
 	ccan[ican]->SetFillColor(10);
 	ccan[ican]->cd(); ccan[ican]->Divide(2,2,0.0001,0.0001);
		ccan[ican]->cd(1);
			hnvpdw_bbc->Draw("colz");
			hnvpdw_bbc_pr->Draw("same");
		ccan[ican]->cd(2);
			hnvpde_bbc->Draw("colz");
			hnvpde_bbc_pr->Draw("same");
		ccan[ican]->cd(3);
			hnvpd_bbc->Draw("colz");
			hnvpd_bbc_pr->Draw("same");
	ccan[ican]->cd(); ccan[ican]->Update();
	ccan[ican]->Print("readout.ps");
	//	
 	ccan[ican] = new TCanvas(buf,buf,0,0,1200,800);
 	ccan[ican]->SetFillColor(10);
 	ccan[ican]->cd(); ccan[ican]->Divide(2,2,0.0001,0.0001);
		ccan[ican]->cd(1);
			hnvpdw_zdc->Draw("colz");
		ccan[ican]->cd(2);
			hnvpde_zdc->Draw("colz");
		ccan[ican]->cd(3);
			hnvpd_zdc->Draw("colz");
	ccan[ican]->cd(); ccan[ican]->Update();
	ccan[ican]->Print("readout.ps");
	
	//----- end of all processing......
 	ccan[ican]->Print("readout.ps]");
 	fout->Write();
	sprintf(buf,"/bin/mv %s.ps %s.ps","readout",namebase.Data());
	cout<<"..... readout::loop Executing ... "<<buf<<endl;
	gSystem->Exec(buf);
	sprintf(buf,"/usr/bin/ps2pdf %s.ps %s.pdf",namebase.Data(),namebase.Data());
	cout<<"..... readout::loop Executing ... "<<buf<<endl;
	gSystem->Exec(buf);
	sprintf(buf,"/bin/cp %s.pdf /star/u/geurts/WWW/files/",namebase.Data());
	cout<<"..... readout::loop Executing ... "<<buf<<endl;
	gSystem->Exec(buf);
	cout<<"..... readout::loop Done...."<<endl;	

	
}
