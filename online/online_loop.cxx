#include "online_class.h"

int	NDETSIDE			= 19;
int KeyChannelOffsets	=  2;	// last channel on each side!!!

//const Float_t BBCcutEW_L = 25000;		// AuAu 200
//const Float_t BBCcutEW_H = 34000;		// AuAu 200
//const Float_t ADCMIN     =    10;		// AuAu 200
//
//const Float_t BBCcutEW_L = 10000;		// AuAu 62.4
//const Float_t BBCcutEW_H = 28000;		// AuAu 62.4
//const Float_t ADCMIN     =    16;		// AuAu 62.4
//
const Float_t BBCcutEW_L = 10000;		// AuAu 39
const Float_t BBCcutEW_H = 28000;		// AuAu 39
const Float_t ADCMIN     =    16;		// AuAu 39

Double_t funres(Double_t *x, Double_t *par){
	Double_t resval = 0.0;
	if (x[0]>0){
		resval	= TMath::Sqrt(x[0]/(1.+x[0]));
		return par[0]/resval;
	} else {
		return 0.0;
	}
}

void online::Loop()
{
	gErrorIgnoreLevel = 1001;

	char	filestring[100];
	char 	buf[80];
	char 	buf2[80];
	int		ican = -1;
	int		ne,nw,idetew,key;
	int		vpdadcsume,vpdadcsumw,vpdadcsum;
	float	vpdtotsume,vpdtotsumw,vpdtotsum;
	int		bbcadcsume,bbcadcsumw,bbcadcsum;
	int		zdcadcsume,zdcadcsumw,zdcadcsum;
	int		runkeep;

	sprintf(filestring,"online_%d",kRunUse);
	TString namebase	= TString(filestring);
	TString nameextr	= TString(".root");
	TString nameroot	= namebase+nameextr;
	TString nameextp	= TString(".ps");
	TString nameposc	= namebase+nameextp;
	cout<<"..... online::loop Opening "<<nameroot.Data()<<endl;
	TFile *fout = new TFile(nameroot.Data(),"RECREATE");
	fout->cd();
	TH1D *hpvpdtof    = new TH1D("hpvpdtof","hpvpdtof",38,-0.5,37.5);
	TH1D *hpvpdtrg_BBQ    = new TH1D("hpvpdtrg_BBQ","hpvpdtrg_BBQ",38,-0.5,37.5);
	TH1D *hpvpdtrg_MXQ    = new TH1D("hpvpdtrg_MXQ","hpvpdtrg_MXQ",38,-0.5,37.5);
	TH2D *hpvpdtrgtof = new TH2D("hpvpdtrgtof","hpvpdtrgtof",38,-0.5,37.5,38,-0.5,37.5);

	float vpdside_max	= 15000.;
	float bbcside_max	= 70000.;
	float zdcside_max	= 12000.;
	float vpd_max	= 2.*vpdside_max;
	float bbc_max	= 2.*bbcside_max;
	float zdc_max	= 2.*zdcside_max;

// 	TH2D *hvpdew      = new TH2D("hvpdew" ,"vpd ADC east vs west",100,0.0, 30000.0,100,0.0, 30000.0);
// 	TH2D *hbbcew      = new TH2D("hbbcew" ,"bbc ADC east vs west",100,0.0, 80000.0,100,0.0, 80000.0);
// 	TH2D *hzdcew      = new TH2D("hzdcew" ,"zdc ADC east vs west",100,0.0, 15000.0,100,0.0, 15000.0);
	TH2D *hvpdew      = new TH2D("hvpdew" ,"vpd ADC sum east vs west",100,0.0,vpdside_max,100,0.0,vpdside_max);
	TH2D *hbbcew      = new TH2D("hbbcew" ,"bbc ADC sum east vs west",100,0.0,bbcside_max,100,0.0,bbcside_max);
	TH2D *hzdcew      = new TH2D("hzdcew" ,"zdc ADC sum east vs west",100,0.0,zdcside_max,100,0.0,zdcside_max);

//	TH2D *hzdcbbc     = new TH2D("hzdcbbc","ADC zdc vs bbc"      ,100,0.0,120000.0,100,0.0, 25000.0);
//	TH2D *hvpdbbc     = new TH2D("hvpdbbc","ADC vpd vs bbc"      ,100,0.0,120000.0,100,0.0, 35000.0);
//	TH2D *hvpdzdc     = new TH2D("hvpdzdc","ADC vpd vs zdc"      ,100,0.0, 25000.0,100,0.0, 35000.0);
	TH2D *hzdcbbc     = new TH2D("hzdcbbc","ADC sum zdc vs bbc"  ,100,0.0,bbc_max,100,0.0,zdc_max);
	TH2D *hvpdbbc     = new TH2D("hvpdbbc","ADC sum vpd vs bbc"  ,100,0.0,bbc_max,100,0.0,vpd_max);
	TH2D *hvpdzdc     = new TH2D("hvpdzdc","ADC sum vpd vs zdc"  ,100,0.0,zdc_max,100,0.0,vpd_max);

	TH2D *hzdcbbce     = new TH2D("hzdcbbce","ADC sum zdcE vs bbcE"  ,100,0.0, bbcside_max,100,0.0,  zdcside_max);
	TH2D *hvpdbbce     = new TH2D("hvpdbbce","ADC sum vpdE vs bbcE"  ,100,0.0, bbcside_max,100,0.0, vpdside_max);
	TH2D *hvpdzdce     = new TH2D("hvpdzdce","ADC sum vpdE vs zdcE"  ,100,0.0,  zdcside_max,100,0.0, vpdside_max);
	TH2D *hzdcbbcw     = new TH2D("hzdcbbcw","ADC sum zdcW vs bbcW"  ,100,0.0, bbcside_max,100,0.0,  zdcside_max);
	TH2D *hvpdbbcw     = new TH2D("hvpdbbcw","ADC sum vpdW vs bbcW"  ,100,0.0, bbcside_max,100,0.0, vpdside_max);
	TH2D *hvpdzdcw     = new TH2D("hvpdzdcw","ADC sum vpdW vs zdcW"  ,100,0.0,  zdcside_max,100,0.0, vpdside_max);

	TH2D* hnvpde_bbce  = new TH2D("hnvpde_bbce","hnvpde_bbce",50,0.0,bbcside_max,20,-0.5,19.5);
	TH2D* hnvpdw_bbcw  = new TH2D("hnvpdw_bbcw","hnvpdw_bbcw",50,0.0,bbcside_max,20,-0.5,19.5);
	TH2D* hnvpd_bbc    = new TH2D("hnvpd_bbc"  ,"hnvpd_bbc"  ,50,0.0,bbc_max,39,-0.5,38.5);

	TH2D* htote_adce   = new TH2D("htote_adce","htote_adce",50,0.0, vpdside_max,50,0.0,400.0);
	TH2D* htotw_adcw   = new TH2D("htotw_adce","htotw_adce",50,0.0, vpdside_max,50,0.0,400.0);
	TH2D* htot_adc     = new TH2D("htot_adc"  ,"htot_adc"  ,50,0.0, vpd_max,50,0.0,800.0);

	TH1D *htot[38];
	TH1D *hadc[38];
	TH1D *htot_bbccut[38];
	TH1D *hadc_bbccut[38];
	TH1D *htot_bbccuth[38];
	TH1D *hadc_bbccuth[38];
	TH2D *hadctot[38];
	TH2D *hadcbbc[38];
	TH1D *htac[38];
	TH1D *hadchi[38];
	TH1D *htachi[38];
	TH1D *htaco[38];
	TH1D *htachio[38];
	//
	for (int ic=0;ic<38;ic++){
		sprintf(buf,"htot%d",ic);
			htot[ic]	= new TH1D(buf,buf,192,0.0,96.0);
		sprintf(buf,"hadc%d",ic);
			hadc[ic]	= new TH1D(buf,buf,512,0.0,4096.0);
		sprintf(buf,"htac%d",ic);
			htac[ic]	= new TH1D(buf,buf,512,0.0,4096.0);
		sprintf(buf,"hadchi%d",ic);
			hadchi[ic]	= new TH1D(buf,buf,512,0.0,4096.0);
			hadchi[ic]->SetLineColor(4);
		sprintf(buf,"htachi%d",ic);
			htachi[ic]	= new TH1D(buf,buf,512,0.0,4096.0);
			htachi[ic]->SetLineColor(4);
		sprintf(buf,"hadctot%d",ic);
			hadctot[ic]	= new TH2D(buf,buf,192,0.0,96.0,128,0.0,4096.0);
		sprintf(buf,"hadcbbc%d",ic);
			hadcbbc[ic]	= new TH2D(buf,buf,100,0.0,100000.0,256,0.0,4096.0);
		sprintf(buf,"htot_bbccut%d",ic);
			htot_bbccut[ic]	= new TH1D(buf,buf,192,0.0,96.0);
			htot_bbccut[ic]->SetLineColor(2);
		sprintf(buf,"hadc_bbccut%d",ic);
			hadc_bbccut[ic]	= new TH1D(buf,buf,512,0.0,4096.0);
			hadc_bbccut[ic]->SetLineColor(2);
		sprintf(buf,"htot_bbccuth%d",ic);
			htot_bbccuth[ic]	= new TH1D(buf,buf,192,0.0,96.0);
			htot_bbccuth[ic]->SetLineColor(3);
		sprintf(buf,"hadc_bbccuth%d",ic);
			hadc_bbccuth[ic]	= new TH1D(buf,buf,512,0.0,4096.0);
			hadc_bbccuth[ic]->SetLineColor(3);
		//
		sprintf(buf,"htaco%d",ic);
			htaco[ic]	= new TH1D(buf,buf,512,0.0,4096.0);
			htaco[ic]->SetLineColor(6);
		sprintf(buf,"htachio%d",ic);
			htachio[ic]	= new TH1D(buf,buf,512,0.0,4096.0);
			htachio[ic]->SetLineColor(7);
	}	
	//
	TH1D *hbbc_adc[48];
	TH1D *hbbc_tac[48];
	for (int ic=0;ic<48;ic++){
		sprintf(buf,"hbbc_adc%d",ic);
			hbbc_adc[ic]	= new TH1D(buf,buf,512,0.0,4096.0);
		sprintf(buf,"hbbc_tac%d",ic);
			hbbc_tac[ic]	= new TH1D(buf,buf,512,0.0,4096.0);
	}
	TH1D *hzdc_adc[6];
	TH1D *hzdc_tac[6];
	for (int ic=0;ic<6;ic++){
		sprintf(buf,"hzdc_adc%d",ic);
			hzdc_adc[ic]	= new TH1D(buf,buf,512,0.0,4096.0);
		sprintf(buf,"hzdc_tac%d",ic);
			hzdc_tac[ic]	= new TH1D(buf,buf,512,0.0,4096.0);
	}
	//
	TH1D *hvpdmeantot			= new TH1D("hvpdmeantot","hvpdmeantot",38,-0.5,37.5);
	TH1D *hvpdmeanadc			= new TH1D("hvpdmeanadc","hvpdmeanadc",38,-0.5,37.5);
	TH1D *hvpdmeantot_bbccut	= new TH1D("hvpdmeantot_bbccut","hvpdmeantot_bbccut",38,-0.5,37.5);
	TH1D *hvpdmeanadc_bbccut	= new TH1D("hvpdmeanadc_bbccut","hvpdmeanadc_bbccut",38,-0.5,37.5);
	TH1D *hvpdmeantot_bbccuth	= new TH1D("hvpdmeantot_bbccuth","hvpdmeantot_bbccuth",38,-0.5,37.5);
	TH1D *hvpdmeanadc_bbccuth	= new TH1D("hvpdmeanadc_bbccuth","hvpdmeanadc_bbccuth",38,-0.5,37.5);
	hvpdmeantot_bbccut->SetLineColor(2);		
	hvpdmeanadc_bbccut->SetLineColor(2);		
	hvpdmeantot_bbccuth->SetLineColor(3);		
	hvpdmeanadc_bbccuth->SetLineColor(3);		
	TH1D *hvpdmeanadc_bbccuth_NOF	= new TH1D("hvpdmeanadc_bbccuth_NOF","hvpdmeanadc_bbccuth_NOF",38,-0.5,37.5);
	hvpdmeanadc_bbccuth_NOF->SetLineColor(6);		
	TH1D *hvpdmeantot1D			= new TH1D("hvpdmeantot1D","hvpdmeantot1D",40,17.5,27.5);
	TH1D *hvpdmeanadc1D			= new TH1D("hvpdmeanadc1D","hvpdmeanadc1D",40,200,400);

	TH2D *hinitialoffsetsTAC	= new TH2D("hinitialoffsetsTAC","hinitialoffsetsTAC",38,-0.5,37.5,200,-400,400);
	TH2D *hinitialoffsetsTAChi	= new TH2D("hinitialoffsetsTAChi","hinitialoffsetsTAChi",38,-0.5,37.5,200,-400,400);
	TH1D *hinitialoffsetsTAC1D	= new TH1D("hinitialoffsetsTAC1D","hinitialoffsetsTAC1D",38,-0.5,37.5);
	TH1D *hinitialoffsetsTAChi1D= new TH1D("hinitialoffsetsTAChi1D","hinitialoffsetsTAChi1D",38,-0.5,37.5);
	hinitialoffsetsTAC1D->SetLineColor(2);
	hinitialoffsetsTAChi1D->SetLineColor(2);
	Bool_t	Use_TAC[38];
	Bool_t	Use_TAChi[38];
	Float_t	toffsets[38];
	Float_t	toffsetshi[38];
	Float_t	toffsetsuse[38];
	Float_t	toffsetshiuse[38];

	cout<<fChain<<endl;
	if (fChain == 0) return;
	Long64_t nentries = fChain->GetEntries();
	cout<<"..... online::loop Nentries = "<<nentries<<endl;
	//nentries = 100000;
	Long64_t nb = 0;
	for (Long64_t jentry=0; jentry<nentries;jentry++) {
		Long64_t ientry = LoadTree(jentry);
		if (ientry < 0) break;
		nb = fChain->GetEntry(jentry);
		if (jentry%10000==0){cout<<"1 Processing "<<jentry<<"  run "<<run<<" "<<kRunUse<<endl;}
		runkeep	= run;

		bbcadcsume = bbcadcsumw = 0;
		for (int ic=0;ic<24;ic++){
			bbcadcsumw	+= bbc_adc[ic];
			bbcadcsume	+= bbc_adc[ic+24];
			hbbc_adc[ic]->Fill(bbc_adc[ic]);
			hbbc_tac[ic]->Fill(bbc_tac[ic]);
			hbbc_adc[ic+24]->Fill(bbc_adc[ic+24]);
			hbbc_tac[ic+24]->Fill(bbc_tac[ic+24]);
		}
		bbcadcsum		= bbcadcsume + bbcadcsumw;
		hbbcew->Fill(bbcadcsumw,bbcadcsume);

		zdcadcsume = zdcadcsumw = 0;
		for (int ic=0;ic<3;ic++){
			zdcadcsumw	+= zdc_adc[ic];
			zdcadcsume	+= zdc_adc[ic+3];
			hzdc_adc[ic]->Fill(zdc_adc[ic]);
			hzdc_tac[ic]->Fill(zdc_tac[ic]);
			hzdc_adc[ic+3]->Fill(zdc_adc[ic+3]);
			hzdc_tac[ic+3]->Fill(zdc_tac[ic+3]);
		}
		zdcadcsum		= zdcadcsume + zdcadcsumw;
		hzdcew->Fill(zdcadcsumw,zdcadcsume);

		nw = ne = 0;
		vpdadcsume = vpdadcsumw =   0;
		vpdtotsume = vpdtotsumw = 0.0;
		for (int ic=0;ic<38;ic++){
			Use_TAC[ic]		= kFALSE;
			Use_TAChi[ic]	= kFALSE;
			if (vpd_tot[ic]>9){
				hpvpdtof->Fill(ic);
				htot[ic]->Fill(vpd_tot[ic]);
				if (ic<19){ ++nw; }else{ ++ne; }
				if (bbcadcsume>BBCcutEW_L&&bbcadcsumw>BBCcutEW_L){
					htot_bbccut[ic]->Fill(vpd_tot[ic]);
				}
				if (bbcadcsume>BBCcutEW_H&&bbcadcsumw>BBCcutEW_H){
					htot_bbccuth[ic]->Fill(vpd_tot[ic]);
				}
			}
			if (vpd_adc[ic]>ADCMIN && vpd_tac[ic]>100){
				if (ic<19){ vpdadcsumw+=vpd_adc[ic];     }else{ vpdadcsume+=vpd_adc[ic]; }				
				if (ic<19){ vpdtotsumw+=vpd_tot[ic]-9.5; }else{ vpdtotsume+=vpd_tot[ic]-9.5; }		
				hpvpdtrg_BBQ->Fill(ic);
				hadc[ic]->Fill(vpd_adc[ic]);
				htac[ic]->Fill(vpd_tac[ic]);
				hadcbbc[ic]->Fill(bbcadcsum,vpd_adc[ic]);
				if (bbcadcsume>BBCcutEW_L&&bbcadcsumw>BBCcutEW_L){
					hadc_bbccut[ic]->Fill(vpd_adc[ic]);
				}
				if (bbcadcsume>BBCcutEW_H&&bbcadcsumw>BBCcutEW_H){
					hadc_bbccuth[ic]->Fill(vpd_adc[ic]);
				}
			}
			if (vpd_tot[ic]>0 && vpd_adc[ic]>0){
				hadctot[ic]->Fill(vpd_tot[ic],vpd_adc[ic]);
			}
			if (vpd_adchi[ic]>ADCMIN && vpd_tachi[ic]>100){
				hpvpdtrg_MXQ->Fill(ic);
			}
			if (vpd_adchi[ic]>0){
				hadchi[ic]->Fill(vpd_adchi[ic]);
				htachi[ic]->Fill(vpd_tachi[ic]);
			}
			if (vpd_tac[ic]  >300&&vpd_tac[ic]  <4000){ Use_TAC[ic]   = kTRUE; }
			if (vpd_tachi[ic]>300&&vpd_tachi[ic]<4000){ Use_TAChi[ic] = kTRUE; }
		}
		vpdadcsum		= vpdadcsume + vpdadcsumw;
		vpdtotsum		= vpdtotsume + vpdtotsumw;
		hvpdew->Fill(vpdadcsumw,vpdadcsume);
		
		if (nw>0&&nw<5&&ne>0&&ne<5){
			for (int ictof=0;ictof<38;ictof++){
				if (vpd_tot[ictof]>10){
					for (int ictrg=0;ictrg<38;ictrg++){
						if (vpd_adc[ictrg]>100){
							hpvpdtrgtof->Fill(ictof,ictrg);
						}
					}
				}
			}
		}	
		
		hzdcbbc->Fill(bbcadcsum,zdcadcsum);
		hvpdbbc->Fill(bbcadcsum,vpdadcsum);
		hvpdzdc->Fill(zdcadcsum,vpdadcsum);

		hzdcbbce->Fill(bbcadcsume,zdcadcsume);
		hvpdbbce->Fill(bbcadcsume,vpdadcsume);
		hvpdzdce->Fill(zdcadcsume,vpdadcsume);
		hzdcbbcw->Fill(bbcadcsumw,zdcadcsumw);
		hvpdbbcw->Fill(bbcadcsumw,vpdadcsumw);
		hvpdzdcw->Fill(zdcadcsumw,vpdadcsumw);

		hnvpde_bbce->Fill(bbcadcsume,ne);
		hnvpdw_bbcw->Fill(bbcadcsumw,nw);
		hnvpd_bbc->Fill(bbcadcsum,ne+nw);
		htote_adce->Fill(vpdadcsume,vpdtotsume);
		htotw_adcw->Fill(vpdadcsumw,vpdtotsumw);
		htot_adc->Fill(vpdadcsum,vpdtotsum);

		//---- TAC offsets....
// 		if (Use_TAC[KeyChannelOffsets]||Use_TAC[KeyChannelOffsets+NDETSIDE]){
// 			for (int is=0;is<2;is++){
// 				key	= KeyChannelOffsets + is*NDETSIDE;
// 				for (int idet=0;idet<NDETSIDE;idet++){
// 					idetew 		= idet + is*NDETSIDE;
// 					if (Use_TAC[key] && Use_TAC[idetew]){
// 						hinitialoffsetsTAC->Fill(idetew,vpd_tac[idetew]-vpd_tac[key],1.0);
// 					}
// 				}
// 			}
// 		}
// 		if (Use_TAChi[KeyChannelOffsets]||Use_TAChi[KeyChannelOffsets+NDETSIDE]){
// 			for (int is=0;is<2;is++){
// 				key	= KeyChannelOffsets + is*NDETSIDE;
// 				for (int idet=0;idet<NDETSIDE;idet++){
// 					idetew 		= idet + is*NDETSIDE;
// 					if (Use_TAChi[key] && Use_TAChi[idetew]){
// 						hinitialoffsetsTAChi->Fill(idetew,vpd_tachi[idetew]-vpd_tachi[key],1.0);
// 					}
// 				}
// 			}
// 		}

	}
	//---- end of event loop1....

	//-------------------------------------------------------------------------

	//TProfile *hpinitialoffsetsTAC = (TProfile*)hinitialoffsetsTAC->ProfileX();
	//TProfile *hpinitialoffsetsTAChi = (TProfile*)hinitialoffsetsTAChi->ProfileX();
// 	int 	ibin,ibinpeak;
// 	float	val,vala,valb;
// 	TH1D 	*hoffsetstemp,*hoffsetstemphi;
// 	float	valpeak;
// 	hinitialoffsetsTAC->FitSlicesY();
// 	hinitialoffsetsTAChi->FitSlicesY();
// 	hoffsetstemp	= (TH1D*)gDirectory->Get("hinitialoffsetsTAC_1");
// 	hoffsetstemphi	= (TH1D*)gDirectory->Get("hinitialoffsetsTAChi_1");
// 	for (int is=0;is<2;is++){
// 		for (int idet=0;idet<NDETSIDE;idet++){
// 			idetew				= idet + is*NDETSIDE;
// 			ibin				= idetew + 1;
//  			valpeak				= hoffsetstemp->GetBinContent(ibin);
//  			toffsets[idetew]	= valpeak;
// 			hinitialoffsetsTAC1D->Fill(idetew,valpeak);
//  			valpeak				= hoffsetstemphi->GetBinContent(ibin);
//  			toffsetshi[idetew]	= valpeak;
// 			hinitialoffsetsTAChi1D->Fill(idetew,valpeak);
// 			//			
// 			//cout<<"... Offset Values "<<idetew<<" "
// 			//	<<toffsets[idetew]<<" "<<toffsetshi[idetew]<<endl;
// 		}
// 	}
// 	if (runkeep==11005010){
// 		ofstream offout;
// 		cout<<"..... online::loop Writing ./tacoffsets_11005010.dat"<<endl;
// 		offout.open("./tacoffsets_11005010.dat", ofstream::out | ofstream::trunc);	
// 		for (int iside=0; iside<2; iside++){
// 			for (int idet=0; idet<19; idet++){
// 				int id = idet + iside*19;
// 				offout<<id<<" "<<toffsets[id]<<" "<<toffsetshi[id]<<endl;
// 			}
// 		}
// 		offout.close();		
// 	}
// 	cout<<"..... online::loop Reading ./tacoffsets_11005010.dat"<<endl;
// 	ifstream parin;
// 	parin.open("./tacoffsets_11005010.dat", ifstream::in);
// 	for (int id=0;id<38;id++){
// 			parin>>idetew>>vala>>valb;
// 			toffsetsuse[id]		= vala; 
// 			toffsetshiuse[id]	= valb; 
// 	}
// 	parin.close();
// 
	double	amean;
	for (int ic=0;ic<38;ic++){
		amean	= htot[ic]->GetMean();			hvpdmeantot->Fill(ic,amean);
		amean	= hadc[ic]->GetMean();			hvpdmeanadc->Fill(ic,amean);
		amean	= htot_bbccut[ic]->GetMean();	hvpdmeantot_bbccut->Fill(ic,amean);
		amean	= hadc_bbccut[ic]->GetMean();	hvpdmeanadc_bbccut->Fill(ic,amean);
		amean	= htot_bbccuth[ic]->GetMean();	hvpdmeantot_bbccuth->Fill(ic,amean);
		amean	= hadc_bbccuth[ic]->GetMean();	hvpdmeanadc_bbccuth->Fill(ic,amean);
		//
		hadc_bbccuth[ic]->GetXaxis()->SetRangeUser(0.,3980.);
		amean	= hadc_bbccuth[ic]->GetMean();	hvpdmeanadc_bbccuth_NOF->Fill(ic,amean);
		hadc_bbccuth[ic]->GetXaxis()->SetRangeUser(0.,-1.);
		//
		amean	= htot[ic]->GetMean();			hvpdmeantot1D->Fill(amean);
		amean	= hadc[ic]->GetMean();			hvpdmeanadc1D->Fill(amean);
		//
	}
// 
// 	//-------------------------------------------------------------------------
// 
// 	Float_t hishift[2]	= { 67, 434 };
// 	Float_t dthresh = 0.0;
// 	if (runkeep==11005010){ dthresh	=   0.0;}
// 	if (runkeep==11005011){ dthresh	=  20.0;}
// 	if (runkeep==11005012){ dthresh	=  50.0;}
// 	if (runkeep==11005013){ dthresh	= 100.0;}
// 	if (runkeep==11005014){ dthresh	= 150.0;}
// 	Float_t	vpd_taco[38];
// 	Float_t	vpd_tachio[38];
// 	Float_t	vpd_tac0C[38];
// 	Float_t	vpd_leslope[38];
// 	Float_t	dtac;
// 	Int_t	nvpd[2];
// 	Int_t	nvpdhi[2];
// 	Int_t	nvpdlohi[2];
// 	Int_t	iside,jdetew;
// 	Int_t	ntmp,ntmp0C;
// 	Float_t	ttmp,ttmp0C;
// 	//
// 	fout->cd();
// 	TH2D *htacdiff  = new TH2D("htacdiff","TACdiff Lo-Hi"  ,38,-0.5,37.5,150,-150,150);
// 	TH2D *htacslope = new TH2D("htacslope","TACslope Lo-Hi",38,-0.5,37.5,100,-2.0,2.0);
// 	TH1D *hptac		= new TH1D("hptac"  ,"hptac"  ,38,-0.5,37.5);
// 			hptac->SetLineColor(6);
// 	TH1D *hptachi	= new TH1D("hptachi","hptachi",38,-0.5,37.5);
// 			hptachi->SetLineColor(7);
// 	TH1D *hptachieff= new TH1D("hptachieff","hptachieff",38,-0.5,37.5);;
// 	TH1D *hptachieff1D= new TH1D("hptachieff1D","hptachieff1D",100,0.2,1.2);
// 	TH2D *hslopeadc[38];
// 	TH2D *hdt_tac_n[38];
// 	TH2D *hdt_tac0C_n[38];
// 	for (int ic=0;ic<38;ic++){
// 		sprintf(buf,"hslopeadc%d",ic);
// 		hslopeadc[ic]	= new TH2D(buf,buf,128,0.0,4096.,100,-2.0,2.0);
// 		sprintf(buf,"hdt_tac_n%d",ic);
// 		hdt_tac_n[ic]	= new TH2D(buf,buf,
// 					NDETSIDE,-0.5,((Float_t)(NDETSIDE))-0.5,500,-250,250);
// 		sprintf(buf,"hdt_tac0C_n%d",ic);
// 		hdt_tac0C_n[ic]	= new TH2D(buf,buf,
// 					NDETSIDE,-0.5,((Float_t)(NDETSIDE))-0.5,500,-250,250);
// 	}
// 	//
// 	for (Long64_t jentry=0; jentry<nentries;jentry++) {
// 		Long64_t ientry = LoadTree(jentry);
// 		if (ientry < 0) break;
// 		nb = fChain->GetEntry(jentry);
// 		if (jentry%10000==0){cout<<"2 Processing "<<jentry<<"  run "<<run<<endl;}
// 		//
// 		nvpd[0] = nvpd[1] = nvpdhi[0] = nvpdhi[1] = nvpdlohi[0] = nvpdlohi[1] = 0;
// 		for (int ic=0;ic<38;ic++){
// 			iside			= ic/NDETSIDE;
// 			Use_TAC[ic]		= kFALSE;
// 			Use_TAChi[ic]	= kFALSE;
// 			if (vpd_tac[ic]  >300&&vpd_tac[ic]  <4000){ Use_TAC[ic]   = kTRUE; }
// 			if (vpd_tachi[ic]>300&&vpd_tachi[ic]<4000){ Use_TAChi[ic] = kTRUE; }
// 			if (Use_TAC[ic]){ 
// 				vpd_taco[ic]   = vpd_tac[ic]-toffsetsuse[ic]; 
// 				hptac->Fill(ic);
// 				++nvpd[iside]; 
// 			} else { vpd_taco[ic]   = 0.0; }
// 			if (Use_TAChi[ic]){ 
// 				vpd_tachio[ic] = vpd_tachi[ic]-toffsetshiuse[ic]+hishift[iside]; 
// 				hptachi->Fill(ic);
// 				++nvpdhi[iside]; 
// 			} else { vpd_tachio[ic] = 0.0; }
// 			//
// 			htaco[ic]->Fill(vpd_taco[ic]);
// 			htachio[ic]->Fill(vpd_tachio[ic]);
// 			if (Use_TAC[ic]&&Use_TAChi[ic]){
// 				dtac	= vpd_taco[ic]-vpd_tachio[ic];
// 				htacdiff->Fill(ic,dtac);
// 				++nvpdlohi[iside]; 
// 				if (dthresh>0.0){
// 					htacslope->Fill(ic,dtac/dthresh);
// 					hslopeadc[ic]->Fill(vpd_adc[ic],dtac/dthresh);
// 					vpd_leslope[ic]	= dtac/dthresh;
// 					vpd_tac0C[ic]	= vpd_tac[ic] + 50.*vpd_leslope[ic];
// 				} else {
// 					vpd_leslope[ic]	= 0.0;
// 					vpd_tac0C[ic]	= vpd_tac[ic];
// 				}
// 			}
// 		}
// 		for (int is=0;is<2;is++){
// 			if (nvpdlohi[is]){
// 				for (int id=0;id<NDETSIDE;id++){
// 					idetew	= id + is*NDETSIDE;
// 					if (Use_TAC[idetew] && Use_TAChi[idetew]){
// 						ntmp = ntmp0C = 0; ttmp = ttmp0C = 0.0;
// 						for (int jd=0;jd<NDETSIDE;jd++){
// 							jdetew	= jd + is*NDETSIDE;
// 							if (jd!=id && Use_TAC[jdetew] && Use_TAChi[jdetew]){
// 								ntmp	+= 1;
// 								ttmp	+= vpd_tac[jdetew];
// 								ntmp0C	+= 1;
// 								ttmp0C	+= vpd_tac0C[jdetew];
// 							}
// 						}
// 						if (ntmp   > 0){ ttmp	/= (Float_t)ntmp;   }
// 						if (ntmp0C > 0){ ttmp0C	/= (Float_t)ntmp0C; }
// 						hdt_tac_n[idetew]->Fill(  ntmp,vpd_tac[idetew]  -ttmp  );
// 						hdt_tac0C_n[idetew]->Fill(ntmp,vpd_tac0C[idetew]-ttmp0C);
// 					}
// 				}
// 			}
// 		}
// 	}
// 	//---- end event loop 2
// 
// 	for (int ic=0;ic<38;ic++){
// 		vala	= hptac->GetBinContent(ic+1);
// 		valb	= hptachi->GetBinContent(ic+1);
// 		if (vala>0){
// 			hptachieff->Fill(ic,valb/vala);
// 			hptachieff1D->Fill(valb/vala);
// 		}
// 	}
// 
// 	TH1D *hdt_tac_n_sigm[38];
// 	TH1D *hdt_tac0C_n_sigm[38];
// 	TF1 *gg = new TF1("gg","gaus",-250,250);
// 		 gg->SetRange(-100,100);
// 	for (int ic=0;ic<38;ic++){	
// 		hdt_tac_n[ic]->FitSlicesY(gg,1,0,3);
// 			sprintf(buf,"hdt_tac_n%d_2",ic);
// 			sprintf(buf2,"hdt_tac_n_sigm%d",ic);
// 			hdt_tac_n_sigm[ic]	= (TH1D*)gDirectory->Get(buf);
// 			hdt_tac_n_sigm[ic]->SetName(buf2);
// 			hdt_tac_n_sigm[ic]->SetMarkerStyle(21);
// 			hdt_tac_n_sigm[ic]->SetMarkerColor( 1);
// 		hdt_tac0C_n[ic]->FitSlicesY(gg,1,0,3);
// 			sprintf(buf,"hdt_tac0C_n%d_2",ic);
// 			sprintf(buf2,"hdt_tac0C_n_sigm%d",ic);
// 			hdt_tac0C_n_sigm[ic]	= (TH1D*)gDirectory->Get(buf);
// 			hdt_tac0C_n_sigm[ic]->SetName(buf2);
// 			hdt_tac0C_n_sigm[ic]->SetMarkerStyle(21);
// 			hdt_tac0C_n_sigm[ic]->SetMarkerColor( 4);
// 	}
// 	TF1 *fres[2*NDETSIDE];
// 	TF1 *fresh[2*NDETSIDE];
// 	TH1D *hsigma0_tac_fit	= new TH1D("hsigma0_tac_fit","hsigma0_tac_fit",38,-0.5,37.5);
// 	TH1D *hsigma0_tac0C_fit	= new TH1D("hsigma0_tac0C_fit","hsigma0_tac0C_fit",38,-0.5,37.5);
// 	TH1D *hsigma0_tac_lit	= new TH1D("hsigma0_tac_lit","hsigma0_tac_lit",38,-0.5,37.5);
// 	TH1D *hsigma0_tac0C_lit	= new TH1D("hsigma0_tac0C_lit","hsigma0_tac0C_lit",38,-0.5,37.5);
// 		hsigma0_tac_fit->SetMarkerStyle(20);	hsigma0_tac_fit->SetMarkerColor(1);		hsigma0_tac_fit->SetMarkerSize(1.5);
// 		hsigma0_tac0C_fit->SetMarkerStyle(20);	hsigma0_tac0C_fit->SetMarkerColor(4);	hsigma0_tac0C_fit->SetMarkerSize(1.5);
// 		hsigma0_tac_lit->SetMarkerStyle(24);	hsigma0_tac_lit->SetMarkerColor(1);		hsigma0_tac_lit->SetMarkerSize(1.5);	
// 		hsigma0_tac0C_lit->SetMarkerStyle(24);	hsigma0_tac0C_lit->SetMarkerColor(4);	hsigma0_tac0C_lit->SetMarkerSize(1.5);
// 	TH1D *hsigma0_tac_fit1D		= new TH1D("hsigma0_tac_fit1D","hsigma0_tac_fit1D",40,0.0,80.0);
// 	TH1D *hsigma0_tac0C_fit1D	= new TH1D("hsigma0_tac0C_fit1D","hsigma0_tac0C_fit1D",40,0.0,80.0);
// 	TH1D *hsigma0_tac_lit1D		= new TH1D("hsigma0_tac_lit1D","hsigma0_tac_lit1D",40,0.0,80.0);
// 	TH1D *hsigma0_tac0C_lit1D	= new TH1D("hsigma0_tac0C_lit1D","hsigma0_tac0C_lit1D",40,0.0,80.0);
// 		hsigma0_tac_fit1D->SetLineColor(1);
// 		hsigma0_tac_lit1D->SetLineColor(1);
// 		hsigma0_tac0C_fit1D->SetLineColor(4);
// 		hsigma0_tac0C_lit1D->SetLineColor(4);
// 	for (int i=0;i<2*NDETSIDE;i++){
// 		sprintf(buf,"fres%d",i);
// 		fres[i]		= new TF1(buf,funres,0.5,11.5,1);
// 			fres[i]->SetLineColor(1);
// 			fres[i]->SetLineWidth(1);
// 		sprintf(buf,"fresh%d",i);
// 		fresh[i]	= new TF1(buf,funres,0.5,11.5,1);
// 			fresh[i]->SetLineColor(7);
// 			fresh[i]->SetLineWidth(1);
// 	}
// 	Double_t 	frespar[1],frespare[1],chisq,chisq_dof;
// 	Double_t 	freshpar[1],freshpare[1],chisqh,chisqh_dof;
// 	Int_t		npfit,npfith;
// 	for (int ic=0;ic<38;ic++){	
// 		sprintf(buf,"fres%d",ic);
// 		hdt_tac_n_sigm[ic]->Fit(buf,"NQR");	
// 			fres[ic]->GetParameters(frespar);
// 			val	= hdt_tac_n_sigm[ic]->GetBinContent(15);
// 			hsigma0_tac_fit->Fill(ic,frespar[0]);
// 			hsigma0_tac_lit->Fill(ic,val);
// 			hsigma0_tac_fit1D->Fill(frespar[0]);
// 			hsigma0_tac_lit1D->Fill(val);
// 		sprintf(buf,"fresh%d",ic);
// 		hdt_tac0C_n_sigm[ic]->Fit(buf,"NQR");		
// 			fresh[ic]->GetParameters(freshpar);
// 			val	= hdt_tac0C_n_sigm[ic]->GetBinContent(15);
// 			hsigma0_tac0C_fit->Fill(ic,freshpar[0]);
// 			hsigma0_tac0C_lit->Fill(ic,val);
// 			hsigma0_tac0C_fit1D->Fill(freshpar[0]);
// 			hsigma0_tac0C_lit1D->Fill(val);
// 	}

	//-------------------------------------------------------------------------
	//---- make plots....
	//
	gStyle->SetOptStat(0);
	gStyle->SetOptFit(1111);
	gStyle->SetPadBottomMargin(0.07);
	gStyle->SetPadTopMargin(0.05);
	gStyle->SetPadLeftMargin(0.08);
	gStyle->SetPadRightMargin(0.05);
	//
 	++ican; sprintf(buf,"ccan%d",ican);
 	ccan[ican] = new TCanvas(buf,buf,0,0,1200,800);
 	ccan[ican]->SetFillColor(10);
 	ccan[ican]->cd(); ccan[ican]->Divide(2,1,0.0001,0.0001);
		ccan[ican]->cd(1);
			hpvpdtof->SetMinimum(1);
			hpvpdtof->Draw();
			gPad->SetLogy(1);
		ccan[ican]->cd(2);
			hpvpdtrg_BBQ->SetMinimum(1);
			hpvpdtrg_BBQ->SetLineColor(1);
			hpvpdtrg_BBQ->Draw();
			hpvpdtrg_MXQ->SetLineColor(6);
			hpvpdtrg_MXQ->Draw("same");
			gPad->SetLogy(1);
	ccan[ican]->cd(); ccan[ican]->Update();
	ccan[ican]->Print("online.ps(");
	//
 	++ican; sprintf(buf,"ccan%d",ican);
 	ccan[ican] = new TCanvas(buf,buf,0,0,1200,800);
 	ccan[ican]->SetFillColor(10);
 	ccan[ican]->cd(); ccan[ican]->Divide(2,1,0.0001,0.0001);
		ccan[ican]->cd(1);
			hpvpdtof->SetMinimum(1);
			hpvpdtof->Draw();
			gPad->SetLogy(0);
		ccan[ican]->cd(2);
			hpvpdtrg_BBQ->SetMinimum(1);
			hpvpdtrg_BBQ->SetLineColor(1);
			hpvpdtrg_BBQ->Draw();
			hpvpdtrg_MXQ->SetLineColor(6);
			hpvpdtrg_MXQ->Draw("same");
			gPad->SetLogy(0);
	ccan[ican]->cd(); ccan[ican]->Update();
	ccan[ican]->Print("online.ps(");
	//
//  	++ican; sprintf(buf,"ccan%d",ican);
//  	ccan[ican] = new TCanvas(buf,buf,0,0,1200,800);
//  	ccan[ican]->SetFillColor(10);
//  	ccan[ican]->cd(); ccan[ican]->Divide(1,1,0.0001,0.0001);
// 		ccan[ican]->cd(1);
// 			hpvpdtrgtof->SetStats(0);
// //			hpvpdtrgtof->SetMinimum(10000);
// 			hpvpdtrgtof->Draw();
// 	ccan[ican]->cd(); ccan[ican]->Update();
// 	ccan[ican]->Print("online.ps");
	//
//  	++ican; sprintf(buf,"ccan%d",ican);
//  	ccan[ican] = new TCanvas(buf,buf,0,0,1200,800);
//  	ccan[ican]->SetFillColor(10);
//  	ccan[ican]->cd(); ccan[ican]->Divide(2,1,0.0001,0.0001);
// 		ccan[ican]->cd(1);
// 			hvpdmeantot_bbccuth->Draw();
// 			hvpdmeantot_bbccut->Draw("same");
// 			hvpdmeantot->Draw("same");
// 		ccan[ican]->cd(2);
// 			hvpdmeanadc_bbccuth->Draw();
// 			hvpdmeanadc_bbccuth_NOF->Draw("same");
// 			hvpdmeanadc_bbccut->Draw("same");
// 			hvpdmeanadc->Draw("same");
// 	ccan[ican]->cd(); ccan[ican]->Update();
// 	ccan[ican]->Print("online.ps");
	//
//  	++ican; sprintf(buf,"ccan%d",ican);
//  	ccan[ican] = new TCanvas(buf,buf,0,0,1200,800);
//  	ccan[ican]->SetFillColor(10);
//  	ccan[ican]->cd(); ccan[ican]->Divide(2,1,0.0001,0.0001);
// 		ccan[ican]->cd(1);
// 			//hvpdmeantot1D->Draw();
// 			hvpdmeantot1D->Fit("gaus","Q");
// 		ccan[ican]->cd(2);
// 			//hvpdmeanadc1D->Draw();
// 			hvpdmeanadc1D->Fit("gaus","Q");
// 	ccan[ican]->cd(); ccan[ican]->Update();
// 	ccan[ican]->Print("online.ps");
	//----
	
	
	
	//----
	++ican; sprintf(buf,"ccan%d",ican);
	ccan[ican] = new TCanvas(buf,buf,0,0,1200,800);
	ccan[ican]->SetFillColor(10);
	ccan[ican]->cd(); ccan[ican]->Divide(5,4,0.0001,0.0001);
	for (int ic=0;ic<19;ic++){
		ccan[ican]->cd(1+ic);
		if (htot[ic]->GetEntries()){
			htot[ic]->Draw();
			htot_bbccut[ic]->Draw("same");
			htot_bbccuth[ic]->Draw("same");
		}
		//gPad->SetLogy(1);
	}
	ccan[ican]->cd(); ccan[ican]->Update();
	ccan[ican]->Print("online.ps");
	//----
	++ican; sprintf(buf,"ccan%d",ican);
	ccan[ican] = new TCanvas(buf,buf,0,0,1200,800);
	ccan[ican]->SetFillColor(10);
	ccan[ican]->cd(); ccan[ican]->Divide(5,4,0.0001,0.0001);
	for (int ic=19;ic<38;ic++){
		ccan[ican]->cd(1+ic-19);
		if (htot[ic]->GetEntries()){
			htot[ic]->Draw();
			htot_bbccut[ic]->Draw("same");
			htot_bbccuth[ic]->Draw("same");
		}
		//gPad->SetLogy(1);
	}
	ccan[ican]->cd(); ccan[ican]->Update();
	ccan[ican]->Print("online.ps");
	//----

// VPD ADC with BBC cuts (works...)
// 	++ican; sprintf(buf,"ccan%d",ican);
// 	ccan[ican] = new TCanvas(buf,buf,0,0,1200,800);
// 	ccan[ican]->SetFillColor(10);
// 	ccan[ican]->cd(); ccan[ican]->Divide(5,4,0.0001,0.0001);
// 	for (int ic=0;ic<19;ic++){
// 		ccan[ican]->cd(1+ic);
// 		if (hadc[ic]->GetEntries()){
// 			hadc[ic]->Draw();
// 			hadc_bbccut[ic]->Draw("same");
// 			hadc_bbccuth[ic]->Draw("same");
// 		}
// 		gPad->SetLogy(1);
// 	}
// 	ccan[ican]->cd(); ccan[ican]->Update();
// 	ccan[ican]->Print("online.ps");
// 	//----
// 	++ican; sprintf(buf,"ccan%d",ican);
// 	ccan[ican] = new TCanvas(buf,buf,0,0,1200,800);
// 	ccan[ican]->SetFillColor(10);
// 	ccan[ican]->cd(); ccan[ican]->Divide(5,4,0.0001,0.0001);
// 	for (int ic=19;ic<38;ic++){
// 		ccan[ican]->cd(1+ic-19);
// 		if (hadc[ic]->GetEntries()){
// 			hadc[ic]->Draw();
// 			hadc_bbccut[ic]->Draw("same");
// 			hadc_bbccuth[ic]->Draw("same");
// 		}
// 		gPad->SetLogy(1);
// 	}
// 	ccan[ican]->cd(); ccan[ican]->Update();
// 	ccan[ican]->Print("online.ps");

	//----
// 	++ican; sprintf(buf,"ccan%d",ican);
// 	ccan[ican] = new TCanvas(buf,buf,0,0,1200,800);
// 	ccan[ican]->SetFillColor(10);
// 	ccan[ican]->cd(); ccan[ican]->Divide(5,4,0.0001,0.0001);
// 	for (int ic=0;ic<19;ic++){
// 		ccan[ican]->cd(1+ic);
// 		hadctot[ic]->Draw("col");
// 		gPad->SetLogz(1);
// 	}
// 	ccan[ican]->cd(); ccan[ican]->Update();
// 	ccan[ican]->Print("online.ps");
// 	//----
// 	++ican; sprintf(buf,"ccan%d",ican);
// 	ccan[ican] = new TCanvas(buf,buf,0,0,1200,800);
// 	ccan[ican]->SetFillColor(10);
// 	ccan[ican]->cd(); ccan[ican]->Divide(5,4,0.0001,0.0001);
// 	for (int ic=19;ic<38;ic++){
// 		ccan[ican]->cd(1+ic-19);
// 		hadctot[ic]->Draw("col");
// 		gPad->SetLogz(1);
// 	}
// 	ccan[ican]->cd(); ccan[ican]->Update();
// 	ccan[ican]->Print("online.ps");
	//----
	
	//----
 	++ican; sprintf(buf,"ccan%d",ican);
 	ccan[ican] = new TCanvas(buf,buf,0,0,1200,800);
 	ccan[ican]->SetFillColor(10);
 	ccan[ican]->cd(); ccan[ican]->Divide(2,2,0.0001,0.0001);
		ccan[ican]->cd(1);
			gPad->SetLogz(1);
			hvpdew->SetStats(0);
			hvpdew->Draw("colz");
		ccan[ican]->cd(2);
			gPad->SetLogz(1);
			hbbcew->SetStats(0);
			hbbcew->Draw("colz");
		ccan[ican]->cd(3);
			gPad->SetLogz(1);
			hzdcew->SetStats(0);
			hzdcew->Draw("colz");
	ccan[ican]->cd(); ccan[ican]->Update();
	ccan[ican]->Print("online.ps");
	//----
 	++ican; sprintf(buf,"ccan%d",ican);
 	ccan[ican] = new TCanvas(buf,buf,0,0,1200,800);
 	ccan[ican]->SetFillColor(10);
 	ccan[ican]->cd(); ccan[ican]->Divide(2,2,0.0001,0.0001);
		ccan[ican]->cd(1);
			gPad->SetLogz(1);
			hzdcbbc->SetStats(0);
			hzdcbbc->Draw("colz");
		ccan[ican]->cd(2);
			gPad->SetLogz(1);
			hvpdbbc->SetStats(0);
			hvpdbbc->Draw("colz");
		ccan[ican]->cd(3);
			gPad->SetLogz(1);
			hvpdzdc->SetStats(0);
			hvpdzdc->Draw("colz");
	ccan[ican]->cd(); ccan[ican]->Update();
	ccan[ican]->Print("online.ps");

 	++ican; sprintf(buf,"ccan%d",ican);
 	ccan[ican] = new TCanvas(buf,buf,0,0,1200,800);
 	ccan[ican]->SetFillColor(10);
 	ccan[ican]->cd(); ccan[ican]->Divide(3,2,0.0001,0.0001);
		ccan[ican]->cd(1);
			gPad->SetLogz(1);
			hzdcbbce->SetStats(0);
			hzdcbbce->Draw("colz");
		ccan[ican]->cd(2);
			gPad->SetLogz(1);
			hvpdbbce->SetStats(0);
			hvpdbbce->Draw("colz");
		ccan[ican]->cd(3);
			gPad->SetLogz(1);
			hvpdzdce->SetStats(0);
			hvpdzdce->Draw("colz");
		ccan[ican]->cd(4);
			gPad->SetLogz(1);
			hzdcbbcw->SetStats(0);
			hzdcbbcw->Draw("colz");
		ccan[ican]->cd(5);
			gPad->SetLogz(1);
			hvpdbbcw->SetStats(0);
			hvpdbbcw->Draw("colz");
		ccan[ican]->cd(6);
			gPad->SetLogz(1);
			hvpdzdcw->SetStats(0);
			hvpdzdcw->Draw("colz");
	ccan[ican]->cd(); ccan[ican]->Update();
	ccan[ican]->Print("online.ps");
	
 	++ican; sprintf(buf,"ccan%d",ican);
 	ccan[ican] = new TCanvas(buf,buf,0,0,1200,800);
 	ccan[ican]->SetFillColor(10);
 	ccan[ican]->cd(); ccan[ican]->Divide(2,2,0.0001,0.0001);
		ccan[ican]->cd(1);
			gPad->SetLogz(1);
			hnvpde_bbce->SetStats(0);
			hnvpde_bbce->Draw("colz");
		ccan[ican]->cd(2);
			gPad->SetLogz(1);
			hnvpdw_bbcw->SetStats(0);
			hnvpdw_bbcw->Draw("colz");
		ccan[ican]->cd(3);
			gPad->SetLogz(1);
			hnvpd_bbc->SetStats(0);
			hnvpd_bbc->Draw("colz");
	ccan[ican]->cd(); ccan[ican]->Update();
	ccan[ican]->Print("online.ps");

 	++ican; sprintf(buf,"ccan%d",ican);
 	ccan[ican] = new TCanvas(buf,buf,0,0,1200,800);
 	ccan[ican]->SetFillColor(10);
 	ccan[ican]->cd(); ccan[ican]->Divide(2,2,0.0001,0.0001);
		ccan[ican]->cd(1);
			gPad->SetLogz(1);
			htote_adce->SetStats(0);
			htote_adce->Draw("colz");
		ccan[ican]->cd(2);
			gPad->SetLogz(1);
			htotw_adcw->SetStats(0);
			htotw_adcw->Draw("colz");
		ccan[ican]->cd(3);
			gPad->SetLogz(1);
			htot_adc->SetStats(0);
			htot_adc->Draw("colz");
	ccan[ican]->cd(); ccan[ican]->Update();
	ccan[ican]->Print("online.ps");

	
	//----
// 	++ican; sprintf(buf,"ccan%d",ican);
// 	ccan[ican] = new TCanvas(buf,buf,0,0,1200,800);
// 	ccan[ican]->SetFillColor(10);
// 	ccan[ican]->cd(); ccan[ican]->Divide(5,4,0.0001,0.0001);
// 	for (int ic=0;ic<19;ic++){
// 		ccan[ican]->cd(1+ic);
// 		if (hadcbbc[ic]->GetEntries()){
// 			hadcbbc[ic]->Draw("col");
// 		}
// 	}
// 	ccan[ican]->cd(); ccan[ican]->Update();
// 	ccan[ican]->Print("online.ps");
// 	//----
// 	++ican; sprintf(buf,"ccan%d",ican);
// 	ccan[ican] = new TCanvas(buf,buf,0,0,1200,800);
// 	ccan[ican]->SetFillColor(10);
// 	ccan[ican]->cd(); ccan[ican]->Divide(5,4,0.0001,0.0001);
// 	for (int ic=19;ic<38;ic++){
// 		ccan[ican]->cd(1+ic-19);
// 		if (hadcbbc[ic]->GetEntries()){
// 			hadcbbc[ic]->Draw("col");
// 		}
// 	}
// 	ccan[ican]->cd(); ccan[ican]->Update();
// 	ccan[ican]->Print("online.ps");
	//----
	//----

	//----
	// VPD ADC Lo and Hi thresholds
	//----
	++ican; sprintf(buf,"ccan%d",ican);
	ccan[ican] = new TCanvas(buf,buf,0,0,1200,800);
	ccan[ican]->SetFillColor(10);
	ccan[ican]->cd(); ccan[ican]->Divide(5,4,0.0001,0.0001);
	for (int ic=0;ic<19;ic++){
		ccan[ican]->cd(1+ic);
		if (hadc[ic]->GetEntries()){
			hadc[ic]->Draw();
			hadchi[ic]->Draw("same");
		}
		gPad->SetLogy(1);
	}
	ccan[ican]->cd(); ccan[ican]->Update();
	ccan[ican]->Print("online.ps");
	//----
	++ican; sprintf(buf,"ccan%d",ican);
	ccan[ican] = new TCanvas(buf,buf,0,0,1200,800);
	ccan[ican]->SetFillColor(10);
	ccan[ican]->cd(); ccan[ican]->Divide(5,4,0.0001,0.0001);
	for (int ic=19;ic<38;ic++){
		ccan[ican]->cd(1+ic-19);
		if (hadc[ic]->GetEntries()){
			hadc[ic]->Draw();
			hadchi[ic]->Draw("same");
		}
		gPad->SetLogy(1);
	}
	ccan[ican]->cd(); ccan[ican]->Update();
	ccan[ican]->Print("online.ps");
	//----
		
	++ican; sprintf(buf,"ccan%d",ican);
	ccan[ican] = new TCanvas(buf,buf,0,0,1200,800);
	ccan[ican]->SetFillColor(10);
	ccan[ican]->cd(); ccan[ican]->Divide(5,4,0.0001,0.0001);
	for (int ic=0;ic<19;ic++){
		ccan[ican]->cd(1+ic);
		if (htac[ic]->GetEntries()){
			htac[ic]->Draw();
			htachi[ic]->Draw("same");
		}
		gPad->SetLogy(1);
	}
	ccan[ican]->cd(); ccan[ican]->Update();
	ccan[ican]->Print("online.ps");
	//----
	++ican; sprintf(buf,"ccan%d",ican);
	ccan[ican] = new TCanvas(buf,buf,0,0,1200,800);
	ccan[ican]->SetFillColor(10);
	ccan[ican]->cd(); ccan[ican]->Divide(5,4,0.0001,0.0001);
	for (int ic=19;ic<38;ic++){
		ccan[ican]->cd(1+ic-19);
		if (htac[ic]->GetEntries()){
			htac[ic]->Draw();
			htachi[ic]->Draw("same");
		}
		gPad->SetLogy(1);
	}
	ccan[ican]->cd(); ccan[ican]->Update();
	ccan[ican]->Print("online.ps");

	++ican; sprintf(buf,"ccan%d",ican);
	ccan[ican] = new TCanvas(buf,buf,0,0,1200,800);
	ccan[ican]->SetFillColor(10);
	ccan[ican]->cd(); ccan[ican]->Divide(5,5,0.0001,0.0001);
	for (int ic=0;ic<24;ic++){
		ccan[ican]->cd(1+ic);
		if (hbbc_adc[ic]->GetEntries()){
			hbbc_adc[ic]->Draw();
		}
		gPad->SetLogy(1);
	}
	ccan[ican]->cd(); ccan[ican]->Update();
	ccan[ican]->Print("online.ps");
	//----
	++ican; sprintf(buf,"ccan%d",ican);
	ccan[ican] = new TCanvas(buf,buf,0,0,1200,800);
	ccan[ican]->SetFillColor(10);
	ccan[ican]->cd(); ccan[ican]->Divide(5,5,0.0001,0.0001);
	for (int ic=24;ic<48;ic++){
		ccan[ican]->cd(1+ic-24);
		if (hbbc_adc[ic]->GetEntries()){
			hbbc_adc[ic]->Draw();
		}
		gPad->SetLogy(1);
	}
	ccan[ican]->cd(); ccan[ican]->Update();
	ccan[ican]->Print("online.ps");
	//
	++ican; sprintf(buf,"ccan%d",ican);
	ccan[ican] = new TCanvas(buf,buf,0,0,1200,800);
	ccan[ican]->SetFillColor(10);
	ccan[ican]->cd(); ccan[ican]->Divide(5,5,0.0001,0.0001);
	for (int ic=0;ic<24;ic++){
		ccan[ican]->cd(1+ic);
		if (hbbc_tac[ic]->GetEntries()){
			hbbc_tac[ic]->Draw();
		}
		gPad->SetLogy(1);
	}
	ccan[ican]->cd(); ccan[ican]->Update();
	ccan[ican]->Print("online.ps");
	//----
	++ican; sprintf(buf,"ccan%d",ican);
	ccan[ican] = new TCanvas(buf,buf,0,0,1200,800);
	ccan[ican]->SetFillColor(10);
	ccan[ican]->cd(); ccan[ican]->Divide(5,5,0.0001,0.0001);
	for (int ic=24;ic<48;ic++){
		ccan[ican]->cd(1+ic-24);
		if (hbbc_tac[ic]->GetEntries()){
			hbbc_tac[ic]->Draw();
		}
		gPad->SetLogy(1);
	}
	ccan[ican]->cd(); ccan[ican]->Update();
	ccan[ican]->Print("online.ps");


	++ican; sprintf(buf,"ccan%d",ican);
	ccan[ican] = new TCanvas(buf,buf,0,0,1200,800);
	ccan[ican]->SetFillColor(10);
	ccan[ican]->cd(); ccan[ican]->Divide(3,2,0.0001,0.0001);
	for (int ic=0;ic<6;ic++){
		ccan[ican]->cd(1+ic);
		if (hzdc_adc[ic]->GetEntries()){
			hzdc_adc[ic]->Draw();
		}
		gPad->SetLogy(1);
	}
	ccan[ican]->cd(); ccan[ican]->Update();
	ccan[ican]->Print("online.ps");
	//
	++ican; sprintf(buf,"ccan%d",ican);
	ccan[ican] = new TCanvas(buf,buf,0,0,1200,800);
	ccan[ican]->SetFillColor(10);
	ccan[ican]->cd(); ccan[ican]->Divide(3,2,0.0001,0.0001);
	for (int ic=0;ic<6;ic++){
		ccan[ican]->cd(1+ic);
		if (hzdc_tac[ic]->GetEntries()){
			hzdc_tac[ic]->Draw();
		}
		gPad->SetLogy(1);
	}
	ccan[ican]->cd(); ccan[ican]->Update();
	ccan[ican]->Print("online.ps");

	

	//
//  	++ican; sprintf(buf,"ccan%d",ican);
//  	ccan[ican] = new TCanvas(buf,buf,0,0,1200,800);
//  	ccan[ican]->SetFillColor(10);
//  	ccan[ican]->cd(); ccan[ican]->Divide(1,1,0.0001,0.0001);
// 		ccan[ican]->cd(1);
// 			hinitialoffsetsTAC->SetStats(0);
// 			hinitialoffsetsTAC->Draw();
// 			hinitialoffsetsTAC1D->Draw("same");
// 	ccan[ican]->cd(); ccan[ican]->Update();
// 	ccan[ican]->Print("online.ps");
	//
//  	++ican; sprintf(buf,"ccan%d",ican);
//  	ccan[ican] = new TCanvas(buf,buf,0,0,1200,800);
//  	ccan[ican]->SetFillColor(10);
//  	ccan[ican]->cd(); ccan[ican]->Divide(1,1,0.0001,0.0001);
// 		ccan[ican]->cd(1);
// 			hinitialoffsetsTAChi->SetStats(0);
// 			hinitialoffsetsTAChi->Draw();
// 			hinitialoffsetsTAChi1D->Draw("same");
// 	ccan[ican]->cd(); ccan[ican]->Update();
// 	ccan[ican]->Print("online.ps");
	//
	//----
// 	++ican; sprintf(buf,"ccan%d",ican);
// 	ccan[ican] = new TCanvas(buf,buf,0,0,1200,800);
// 	ccan[ican]->SetFillColor(10);
// 	ccan[ican]->cd(); ccan[ican]->Divide(5,4,0.0001,0.0001);
// 	for (int ic=0;ic<19;ic++){
// 		ccan[ican]->cd(1+ic);
// 		if (htaco[ic]->GetEntries()){
// 			htaco[ic]->Draw();
// 			htachio[ic]->Draw("same");
// 		}
// 		gPad->SetLogy(1);
// 	}
// 	ccan[ican]->cd(); ccan[ican]->Update();
// 	ccan[ican]->Print("online.ps");
	//----
// 	++ican; sprintf(buf,"ccan%d",ican);
// 	ccan[ican] = new TCanvas(buf,buf,0,0,1200,800);
// 	ccan[ican]->SetFillColor(10);
// 	ccan[ican]->cd(); ccan[ican]->Divide(5,4,0.0001,0.0001);
// 	for (int ic=19;ic<38;ic++){
// 		ccan[ican]->cd(1+ic-19);
// 		if (htaco[ic]->GetEntries()){
// 			htaco[ic]->Draw();
// 			htachio[ic]->Draw("same");
// 		}
// 		gPad->SetLogy(1);
// 	}
// 	ccan[ican]->cd(); ccan[ican]->Update();
// 	ccan[ican]->Print("online.ps");
	//
	//
// 	gStyle->SetOptStat(1111);
//  	++ican; sprintf(buf,"ccan%d",ican);
//  	ccan[ican] = new TCanvas(buf,buf,0,0,1200,800);
//  	ccan[ican]->SetFillColor(10);
//  	ccan[ican]->cd(); ccan[ican]->Divide(1,2,0.0001,0.0001);
// 		ccan[ican]->cd(1);
// 			hptac->SetStats(0);
// 			hptac->Draw();
// 			hptachi->SetStats(0);
// 			hptachi->Draw("same");
// 		ccan[ican]->cd(2);
// 			hptachieff1D->Draw();
// 	ccan[ican]->cd(); ccan[ican]->Update();
// 	ccan[ican]->Print("online.ps");
// 	gStyle->SetOptStat(0);
// 	//
//  	++ican; sprintf(buf,"ccan%d",ican);
//  	ccan[ican] = new TCanvas(buf,buf,0,0,1200,800);
//  	ccan[ican]->SetFillColor(10);
//  	ccan[ican]->cd(); ccan[ican]->Divide(1,1,0.0001,0.0001);
// 		ccan[ican]->cd(1);
// 			htacdiff->SetStats(0);
// 			htacdiff->Draw("col");
// 	ccan[ican]->cd(); ccan[ican]->Update();
// 	ccan[ican]->Print("online.ps");
// 	//
//  	++ican; sprintf(buf,"ccan%d",ican);
//  	ccan[ican] = new TCanvas(buf,buf,0,0,1200,800);
//  	ccan[ican]->SetFillColor(10);
//  	ccan[ican]->cd(); ccan[ican]->Divide(1,1,0.0001,0.0001);
// 		ccan[ican]->cd(1);
// 			htacslope->SetStats(0);
// 			htacslope->Draw("col");
// 	ccan[ican]->cd(); ccan[ican]->Update();
// 	ccan[ican]->Print("online.ps");
// 	//----
// 	++ican; sprintf(buf,"ccan%d",ican);
// 	ccan[ican] = new TCanvas(buf,buf,0,0,1200,800);
// 	ccan[ican]->SetFillColor(10);
// 	ccan[ican]->cd(); ccan[ican]->Divide(5,4,0.0001,0.0001);
// 	for (int ic=0;ic<19;ic++){
// 		ccan[ican]->cd(1+ic);
// 		if (hslopeadc[ic]->GetEntries()){
// 			hslopeadc[ic]->GetXaxis()->SetRangeUser(0.0,1500.0);
// 			hslopeadc[ic]->Draw("col");
// 		}
// 	}
// 	ccan[ican]->cd(); ccan[ican]->Update();
// 	ccan[ican]->Print("online.ps");
// 	//----
// 	++ican; sprintf(buf,"ccan%d",ican);
// 	ccan[ican] = new TCanvas(buf,buf,0,0,1200,800);
// 	ccan[ican]->SetFillColor(10);
// 	ccan[ican]->cd(); ccan[ican]->Divide(5,4,0.0001,0.0001);
// 	for (int ic=19;ic<38;ic++){
// 		ccan[ican]->cd(1+ic-19);
// 		if (hslopeadc[ic]->GetEntries()){
// 			hslopeadc[ic]->GetXaxis()->SetRangeUser(0.0,1500.0);
// 			hslopeadc[ic]->Draw("col");
// 		}
// 	}
// 	ccan[ican]->cd(); ccan[ican]->Update();
// 	ccan[ican]->Print("online.ps");
// 	//
// 	//
//  	++ican; sprintf(buf,"ccan%d",ican);
//  	ccan[ican] = new TCanvas(buf,buf,0,0,1200,800);
//  	ccan[ican]->SetFillColor(10);
//  	ccan[ican]->cd(); ccan[ican]->Divide(1,1,0.0001,0.0001);
// 		ccan[ican]->cd(1);
// 			htacslope->SetStats(0);
// 			htacslope->Draw("col");
// 	ccan[ican]->cd(); ccan[ican]->Update();
// 	ccan[ican]->Print("online.ps");
// 	//
// 	//----
// 	++ican; sprintf(buf,"ccan%d",ican);
// 	ccan[ican] = new TCanvas(buf,buf,0,0,1200,800);
// 	ccan[ican]->SetFillColor(10);
// 	ccan[ican]->cd(); ccan[ican]->Divide(5,4,0.0001,0.0001);
// 	for (int ic=0;ic<19;ic++){
// 		ccan[ican]->cd(1+ic);
// 		hdt_tac_n_sigm[ic]->SetMinimum(  0);
// 		hdt_tac_n_sigm[ic]->SetMaximum(100);
// 		hdt_tac_n_sigm[ic]->Draw();
// 		hdt_tac0C_n_sigm[ic]->Draw("same");
// 		fres[ic]->Draw("same");
// 		fresh[ic]->Draw("same");
// 	}
// 	ccan[ican]->cd(); ccan[ican]->Update();
// 	ccan[ican]->Print("online.ps");
// 	//----
// 	++ican; sprintf(buf,"ccan%d",ican);
// 	ccan[ican] = new TCanvas(buf,buf,0,0,1200,800);
// 	ccan[ican]->SetFillColor(10);
// 	ccan[ican]->cd(); ccan[ican]->Divide(5,4,0.0001,0.0001);
// 	for (int ic=19;ic<38;ic++){
// 		ccan[ican]->cd(1+ic-19);
// 		hdt_tac_n_sigm[ic]->SetMinimum(  0);
// 		hdt_tac_n_sigm[ic]->SetMaximum(100);
// 		hdt_tac_n_sigm[ic]->Draw();
// 		hdt_tac0C_n_sigm[ic]->Draw("same");
// 		fres[ic]->Draw("same");
// 		fresh[ic]->Draw("same");
// 	}
// 	ccan[ican]->cd(); ccan[ican]->Update();
// 	ccan[ican]->Print("online.ps");
// 	//
//  	++ican; sprintf(buf,"ccan%d",ican);
//  	ccan[ican] = new TCanvas(buf,buf,0,0,1200,800);
//  	ccan[ican]->SetFillColor(10);
//  	ccan[ican]->cd(); ccan[ican]->Divide(1,1,0.0001,0.0001);
// 		ccan[ican]->cd(1);
// 			hsigma0_tac_fit->SetStats(0);
// 			hsigma0_tac_lit->SetStats(0);
// 			hsigma0_tac0C_fit->SetStats(0);
// 			hsigma0_tac0C_lit->SetStats(0);
// 			hsigma0_tac_fit->SetMinimum( 0.0);
// 			hsigma0_tac_fit->SetMaximum(75.0);
// 			hsigma0_tac_fit->Draw("P");
// 			hsigma0_tac_lit->Draw("P same");
// 			hsigma0_tac0C_fit->Draw("P same");
// 			hsigma0_tac0C_lit->Draw("P same");
// 			TLegend *ll = new TLegend(0.19,0.1,0.55,0.28);
// 				ll->SetTextSize(0.03);
// 				ll->AddEntry(hsigma0_tac_fit,"#sigma(1-<N>) Fit, Raw","P");
// 				ll->AddEntry(hsigma0_tac_lit,"#sigma(1-<14>), Raw","P");
// 				ll->AddEntry(hsigma0_tac0C_fit,"#sigma(1-<N>) Fit, 0-crossing","P");
// 				ll->AddEntry(hsigma0_tac0C_lit,"#sigma(1-<14>), 0-crossing","P");
// 				ll->Draw();
// 	ccan[ican]->cd(); ccan[ican]->Update();
// 	ccan[ican]->Print("online.ps");
// 	//
//  	++ican; sprintf(buf,"ccan%d",ican);
//  	ccan[ican] = new TCanvas(buf,buf,0,0,1200,800);
//  	ccan[ican]->SetFillColor(10);
//  	ccan[ican]->cd(); ccan[ican]->Divide(1,2,0.0001,0.0001);
// 		ccan[ican]->cd(1);
// 			hsigma0_tac_fit1D->Draw();
// 			hsigma0_tac0C_fit1D->Draw("same");
// 		ccan[ican]->cd(2);
// 			hsigma0_tac_lit1D->Draw();
// 			hsigma0_tac0C_lit1D->Draw("same");
// 	ccan[ican]->cd(); ccan[ican]->Update();
// 	ccan[ican]->Print("online.ps");


	//----- end of all processing......
	ccan[ican]->Print("online.ps]");
	fout->Write();
	sprintf(buf,"/bin/mv %s.ps %s.ps","online",namebase.Data());
	cout<<"..... online::loop Executing ... "<<buf<<endl;
	gSystem->Exec(buf);
	sprintf(buf,"/usr/bin/ps2pdf %s.ps %s.pdf",namebase.Data(),namebase.Data());
	cout<<"..... online::loop Executing ... "<<buf<<endl;
	gSystem->Exec(buf);
	sprintf(buf,"/bin/cp %s.pdf /star/u/geurts/WWW/files/",namebase.Data());
	cout<<"..... online::loop Executing ... "<<buf<<endl;
	gSystem->Exec(buf);
	cout<<"..... online::loop Done...."<<endl;	

}
//---- end of routine....
