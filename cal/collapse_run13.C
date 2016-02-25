
#include <fstream>
#include <iostream>
#include <iomanip>
using namespace std;

void collapse() {

const int NDETSIDE = 19;
const int NITERATIONS = 100;
int NITERATIONSSEEN = 0;

TH1::AddDirectory(kFALSE);
int		idet,idetew,ipass;
float	xval,fval;
char 	buf[120];
char 	ouf[120];
Int_t	ican = -1;
TCanvas *ccan[100];

char infilename[120];
cout<<" enter filename"<<endl;
cin>>infilename;
TString dir	= TString("./root/");
TString infilestring = TString(infilename);
TString head = TString("collapse_");
TString exten = TString(".root");
TString pathin	= dir+infilestring+exten;
TString pathout	= head+infilestring;
cout<<"path in..... "<<pathin.Data()<<endl;
cout<<"path out.... "<<pathout.Data()<<endl;

Bool_t 	UsesToT;
Float_t	Fraction;
if (strstr(infilename,"_ic0")||strstr(infilename,"_ic3")){
	cout<<" Uses ToT..................."<<endl;
	UsesToT		= kTRUE;
	Fraction	= 0.9;
}else{
	cout<<" Uses ADC..................."<<endl;
	UsesToT		= kFALSE;
	Fraction	= 0.99;
}

TH1D *htot[19][2];
TH1D *hsskeep[19][2];
TH1D *hadc[19][2];
TH1D *ffun_iter[100][2];
TH1D *ffun_chan[38];
TH1D *hinitialoffsetvals;
TH1D *hdt_end_chan_nLLa_mean;
TH1D *hdt_end_chan_nLLb_mean;
//
TH1D *ffun_chan_inito[38];
TH1D *ffun_chan_final[38];
float initoffvals[38];
//
for (int idet=0;idet<38;idet++){
	if (UsesToT){
		sprintf(buf,"ffun_chan%d",idet);
		ffun_chan[idet]			= new TH1D(buf,buf,128,0.,  64.0);
		sprintf(buf,"ffun_chan_inito%d",idet);
		ffun_chan_inito[idet]	= new TH1D(buf,buf,128,0.,  64.0);
		sprintf(buf,"ffun_chan_final%d",idet);
		ffun_chan_final[idet]	= new TH1D(buf,buf,128,0.,  64.0);
	} else {
		sprintf(buf,"ffun_chan%d",idet);
		ffun_chan[idet]			= new TH1D(buf,buf,128,0.,4096.0);
		sprintf(buf,"ffun_chan_inito%d",idet);
		ffun_chan_inito[idet]	= new TH1D(buf,buf,128,0.,4096.0);
		sprintf(buf,"ffun_chan_final%d",idet);
		ffun_chan_final[idet]	= new TH1D(buf,buf,128,0.,4096.0);
	}
}
sprintf(buf,"%s",pathin.Data());
sprintf(ouf,"%s",pathout.Data());
TFile *f	= new TFile(buf,"READ");
cout<<"Opening input file  ... "<<buf<<endl;
cout<<"        output base ... "<<ouf<<endl;
//
hinitialoffsetvals = (TH1D*)f->Get("hinitialoffsetvals");
//cout<<hinitialoffsetvals<<endl;
hdt_end_chan_nLLb_mean = (TH1D*)f->Get("hdt_end_chan_nLLb_mean");
//cout<<hdt_end_chan_nLLb_mean<<endl;
hdt_end_chan_nLLa_mean = (TH1D*)f->Get("hdt_end_chan_nLLa_mean");
//cout<<hdt_end_chan_nLLa_mean<<endl;
//
f->cd("ffun");
int icolors[5] = {2,3,4,6,7};

for (int iter=0;iter<NITERATIONS;iter++){
	idet	= iter%NDETSIDE;
	ipass	= iter/NDETSIDE;
	for (int is=0;is<2;is++){
		idetew	= idet + NDETSIDE*is;
		//
//		double val_off_fin	= hdt_end_chan_n10_mean->GetBinContent(idetew+1);
		double val_off_fin	= hdt_end_chan_nLLa_mean->GetBinContent(idetew+1);
		//
		sprintf(buf,"ffun/ffun_iter%d%d",iter,is);
			ffun_iter[iter][is] = (TH1D*)f->Get(buf);
		if (ffun_iter[iter][is]){
			ffun_iter[iter][is]->SetLineColor(icolors[ipass]);
			if (is==0){ ++NITERATIONSSEEN; }
			for (int ib=1;ib<=120;ib++){
				xval	 = ffun_iter[iter][is]->GetXaxis()->GetBinCenter(ib);
				fval	 = ffun_iter[iter][is]->GetBinContent(ib);
				//cout<<iter<<" "<<is<<" "<<idet<<" "<<idetew<<" "<<xval<<" "<<fval<<endl;
				if (xval>=9.0){
					ffun_chan[idetew]->Fill(xval,fval);
					ffun_chan_inito[idetew]->Fill(xval,fval);
					ffun_chan_final[idetew]->Fill(xval,fval);
				} else {
					ffun_chan[idetew]->Fill(xval,-1000);
					ffun_chan_inito[idetew]->Fill(xval,-1200);
					ffun_chan_final[idetew]->Fill(xval,-1200);
				}
			}
		}
	}
}
cout<<" NITERATIONSSEEN = "<<NITERATIONSSEEN<<endl;

for (idet=0;idet<NDETSIDE;idet++){
	for (int is=0;is<2;is++){
		idetew	= idet + NDETSIDE*is;
		//
		double val_off_raw	= hinitialoffsetvals->GetBinContent(idetew+1);
		initoffvals[idetew]	= val_off_raw;
//		double val_off_fin	= hdt_end_chan_n10_mean->GetBinContent(idetew+1);
		double val_off_fin	= hdt_end_chan_nLLa_mean->GetBinContent(idetew+1);
		//
		if (ffun_chan[idetew]){
		//if (is==1){
		//	cout<<is<<" "<<idetew<<" "
		//		<<val_off_raw<<" "<<val_off_fin<<endl;
		//}
			for (int ib=1;ib<=120;ib++){
				xval	 = ffun_chan[idetew]->GetXaxis()->GetBinCenter(ib);
				fval	 = ffun_chan[idetew]->GetBinContent(ib);
				//if (ib==30&&is==1){
				//		cout<<is<<" "<<idetew<<" "<<fval<<"\t "
				//			<<val_off_raw<<" "<<val_off_fin<<"\t "
				//			<<fval+val_off_raw+val_off_fin
				//			<<endl;
				//}
				if (fval>-900){
					ffun_chan_inito[idetew]->Fill(xval,val_off_raw);
					ffun_chan_final[idetew]->Fill(xval,val_off_raw);
					ffun_chan_final[idetew]->Fill(xval,val_off_fin);
				}
			}
		}
	}
}


cout <<"get adc dists"<<endl;
f->cd("A");
for (int idet=0;idet<19;idet++){
	for (int is=0;is<2;is++){
		idetew	= idet + NDETSIDE*is;
		sprintf(buf,"A/htot%d%d",idet,is);
			htot[idet][is] = (TH1D*)f->Get(buf);
			if (!htot[idet][is]){
				cout<<" NO TOT!!!!! "<<idet<<" "<<is<<endl;
			}
			htot[idet][is]->SetLineColor(2);
		sprintf(buf,"A/hadc%d%d",idet,is);
			hadc[idet][is] = (TH1D*)f->Get(buf);
			hadc[idet][is]->SetLineColor(4);		
	}
}
f->Close();
for (int idet=0;idet<19;idet++){
	for (int is=0;is<2;is++){
		sprintf(buf,"hsskeep%d%d",idet,is);
		if (UsesToT){
			hsskeep[idet][is] = new TH1D(buf,buf,128,0.0,  64.0);
		} else {
			hsskeep[idet][is] = new TH1D(buf,buf,128,0.0,4096.0);
		}
		hsskeep[idet][is]->SetFillColor(3);
	}
}
TH1D *hsslimits	= new TH1D("hsslimits","hsslimits",38,-0.5,37.5);

//for (int ib=1;ib<=38;ib++){
//	cout<<ib<<"\t "<<hinitialoffsetvals->GetBinContent(ib)<<endl;
//}

//---- now find 90% ADC limits!
cout<<"Finding limits"<<endl;
float totlim[38];
float valtotlim[38];
double csum,integral,rintegral,contents,xbin;
for (int idet=0;idet<19;idet++){
	for (int is=0;is<2;is++){
		idetew	= idet + NDETSIDE*is;
		totlim[idetew]	= -1;
		csum			= 0.0;
		integral 		= htot[idet][is]->GetEntries();
		for (int ibin=1;ibin<=128;ibin++){
			if (UsesToT){
				htot[idet][is]->GetXaxis()->SetRange(1,ibin);
				rintegral = htot[idet][is]->Integral();
				contents  = htot[idet][is]->GetBinContent(ibin);
			} else {
				hadc[idet][is]->GetXaxis()->SetRange(1,ibin);
				rintegral = hadc[idet][is]->Integral();
				contents  = hadc[idet][is]->GetBinContent(ibin);
			}
			csum			+= contents;
			//cout<<idet<<" "<<is<<" "<<integral<<"\t "
			//	<<ibin<<" "<<contents<<" "<<csum<<" "<<rintegral<<endl;
			if (integral>0){
				if (rintegral/integral<Fraction){
					hsskeep[idet][is]->SetBinContent(ibin,contents);
				} else {
					if (totlim[idetew]==-1){
						if (UsesToT){
							xbin	= ((float)ibin)/2.0;
						} else {
							xbin	= ((float)ibin)*32.0;
						}
						totlim[idetew]		= ibin;
						valtotlim[idetew]	= xbin;
						hsslimits->Fill(idetew,xbin);
					}
				}
			} else {
				hsskeep[idet][is]->SetBinContent(ibin,0.0);
				hsslimits->Fill(idetew,0.0);
			}
		}
	}
}


sprintf(buf,"root/%s.txt",ouf);
ofstream textout;	textout.open(buf,ios::out);
cout<<"writing dB output file    ... "<<buf<<endl;
for (int is=0;is<2;is++){
	for (int idet=0;idet<NDETSIDE;idet++){
		idetew	= idet + NDETSIDE*is;
		//
		textout<<setw(4)<<idetew+1<<endl;
		textout<<setw(4)<<128<<endl;
		for (int ib=1;ib<=128;ib++){
			textout<<setw(16)<<ffun_chan_final[idetew]->GetBinLowEdge(ib)<<" ";
		} textout<<endl;
		for (int ib=1;ib<=128;ib++){
			float yval	= ffun_chan_final[idetew]->GetBinContent(ib);
			if (yval<-3000){ yval = 0.0; }
			textout<<setw(16)<<yval<<" ";
		} textout<<endl;
		//
	}
}
textout.close();




//---- plot slewing curves...
cout <<"plot slewing"<<endl;
for (int is=0;is<2;is++){
	++ican; sprintf(buf,"ccan%d",ican);
	ccan[ican] = new TCanvas(buf,buf,10+is*30,10+is*30,640,460);
	ccan[ican]->cd(); ccan[ican]->Divide(5,4,0.0001,0.0001);
	for (int idet=0;idet<NDETSIDE;idet++){
		idetew	= idet + NDETSIDE*is;
		ccan[ican]->cd(idet+1);
		ffun_iter[idet][is]->SetMinimum(-4);
		ffun_iter[idet][is]->SetMaximum( 6);
		if (ffun_iter[idet][is]){
			ffun_iter[idet][is]->Draw();
 			if (NITERATIONSSEEN>19){
 				ffun_iter[idet+NDETSIDE][is]->Draw("same");
 				if (NITERATIONSSEEN>38){
 					ffun_iter[idet+2*NDETSIDE][is]->Draw("same");
// 					if (NITERATIONSSEEN>57){
// 						ffun_iter[idet+3*NDETSIDE][is]->Draw("same");
// 					}
 				}
 			}
			ffun_chan[idetew]->Draw("same");
		}
	}
	ccan[ican]->cd(); ccan[is]->Update();
	if (ican==0){ sprintf(buf,"ps/%s.ps(",ouf); } else	// print west....
	            { sprintf(buf,"ps/%s.ps",ouf);  }		// print east....
	ccan[ican]->Print(buf);
}

//---- plot ToT
cout<<"Plotting Signal Sizes....................."<<endl;
for (int is=0;is<2;is++){
	++ican; sprintf(buf,"ccan%d",ican);
	ccan[ican] = new TCanvas(buf,buf,70+is*30,70+is*30,640,460);
	ccan[ican]->cd(); ccan[ican]->Divide(5,4,0.0001,0.0001);
	for (int idet=0;idet<NDETSIDE;idet++){
		idetew	= idet + NDETSIDE*is;
		ccan[ican]->cd(idet+1);
		if (UsesToT){
			htot[idet][is]->Draw();
		} else {
			hadc[idet][is]->Draw();
		}
		hsskeep[idet][is]->Draw("same");
	}
	ccan[ican]->cd(); ccan[is]->Update();
	sprintf(buf,"ps/%s.ps",ouf);
	ccan[ican]->Print(buf);
}
//---- plot ToT Limits
gStyle->SetOptStat(0);
++ican; sprintf(buf,"ccan%d",ican);
ccan[ican] = new TCanvas(buf,buf,100,100,640,460);
ccan[ican]->cd(); ccan[ican]->Divide(1,1,0.0001,0.0001);
	ccan[ican]->cd(1);
	hsslimits->SetMinimum(0.0);
	hsslimits->Draw();
ccan[ican]->cd(); ccan[is]->Update();
sprintf(buf,"ps/%s.ps",ouf);
ccan[ican]->Print(buf);

gStyle->SetOptStat(0);
++ican; sprintf(buf,"ccan%d",ican);
ccan[ican] = new TCanvas(buf,buf,130,130,640,460);
ccan[ican]->cd(); ccan[ican]->Divide(1,1,0.0001,0.0001);
	ccan[ican]->cd(1);
	hdt_end_chan_nLLb_mean->Draw();
	hdt_end_chan_nLLa_mean->Draw();
ccan[ican]->cd(); ccan[is]->Update();
sprintf(buf,"ps/%s.ps",ouf);
ccan[ican]->Print(buf);

//---- plot slewing curves...
TLine *lvaltotlim[38];
for (int is=0;is<2;is++){
	++ican; sprintf(buf,"ccan%d",ican);
	ccan[ican] = new TCanvas(buf,buf,160+is*30,160+is*30,640,460);
	ccan[ican]->cd(); ccan[ican]->Divide(5,4,0.0001,0.0001);
	for (int idet=0;idet<NDETSIDE;idet++){
		idetew	= idet + NDETSIDE*is;
		ccan[ican]->cd(idet+1);
		if (ffun_chan_inito[idetew]){
			ffun_chan_inito[idetew]->SetMinimum(initoffvals[idetew]-3.);
			ffun_chan_inito[idetew]->SetMaximum(initoffvals[idetew]+4.);
			lvaltotlim[idetew]	= new TLine(valtotlim[idetew],initoffvals[idetew]-3.,
			                                valtotlim[idetew],initoffvals[idetew]+4.);
			ffun_chan_inito[idetew]->Draw();
			ffun_chan_final[idetew]->SetLineWidth(2);
			ffun_chan_final[idetew]->SetLineColor(3);
			ffun_chan_final[idetew]->Draw("same");
			lvaltotlim[idetew]->Draw();
		}
	}
	ccan[ican]->cd(); ccan[is]->Update();
	sprintf(buf,"ps/%s.ps",ouf);
	ccan[ican]->Print(buf);
}


sprintf(buf,"ps/%s.ps]",ouf);
ccan[ican]->Print(buf);
sprintf(buf,"/usr/bin/ps2pdf ps/%s.ps ps/%s.pdf",ouf,ouf);
 	cout<<"executing unix command ... "<<buf<<endl;
 	gSystem->Exec(buf);
sprintf(buf,"/bin/cp ps/%s.pdf /star/u/llope/WWW/files/",ouf);
 	cout<<"executing unix command ... "<<buf<<endl;
 	gSystem->Exec(buf);
 	
//---- build output root files and put them in WWW-accessible area....
sprintf(buf,"root/%s.root",ouf);
cout<<"writing output file    ... "<<buf<<endl;
TFile *fout = new TFile(buf,"RECREATE");
	TDirectory *f_off = fout->mkdir("offsets"); f_off->cd();
	//fout->cd();
	hinitialoffsetvals->Write();
	hdt_end_chan_nLLb_mean->Write();
	hdt_end_chan_nLLa_mean->Write();
	hsslimits->Write();
	TDirectory *f_iter = fout->mkdir("ITER"); f_iter->cd();
		for (int iter=0;iter<NITERATIONSSEEN;iter++){
			for (int is=0;is<2;is++){
				ffun_iter[iter][is]->SetMinimum(-4);
				ffun_iter[iter][is]->SetMaximum( 4);
				ffun_iter[iter][is]->Write();
			}
		}
	TDirectory *f_chan = fout->mkdir("CHAN"); f_chan->cd();
		for (int idetew=0;idetew<38;idetew++){
			ffun_chan[idetew]->Write();
			ffun_chan_inito[idetew]->Write();
			ffun_chan_final[idetew]->Write();
		}
	TDirectory *f_tot = fout->mkdir("ToT"); f_tot->cd();
	for (int idet=0;idet<19;idet++){
		for (int is=0;is<2;is++){
			htot[idet][is]->Write();
			hsskeep[idet][is]->Write();
		}
	}
	fout->cd();
fout->Close();
sprintf(buf,"/bin/cp root/%s.root /star/u/llope/WWW/files/",ouf);
 	cout<<"executing unix command ... "<<buf<<endl;
 	gSystem->Exec(buf);
sprintf(buf,"/bin/cp root/%s.txt /star/u/llope/WWW/files/",ouf);
 	cout<<"executing unix command ... "<<buf<<endl;
 	gSystem->Exec(buf);

}
//---- end of routine...

