#include <fstream>
#include <iostream>
#include <iomanip>
using namespace std;

Bool_t 	UsesToT;
Bool_t 	UsestLE;

Double_t myExt(Double_t *x, Double_t *par){
	Float_t xx		= x[0];
	Double_t f		= par[0] + par[1]*xx + par[2]*xx*xx ;
	return f;
}
  

//-----------------------------------------------------------------
void collapse() {

const int NDETSIDE  	= 19;
const int NITERATIONS	= 10;		// max value
int NITERATIONSSEEN 	=  0;

float plotymin	= -5;
float plotymax	=  5;

TH1::AddDirectory(kFALSE);
int		idet,idetew,iter;
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

float Amax,AmaxFit;
Float_t	Fraction;
if (strstr(infilename,"_ic0")||strstr(infilename,"_ic3")){
	cout<<" Uses ToT..................."<<endl;
	UsesToT		= kTRUE;
	Fraction	= 0.7;
	Amax		= 64.;
	AmaxFit		= 56.;
}else{
	cout<<" Uses ADC..................."<<endl;
	UsesToT		= kFALSE;
	Fraction	= 0.80;
	Amax		= 4096;
	AmaxFit		= 3600.;
}
if (strstr(infilename,"_ic0")||strstr(infilename,"_ic2")){
	UsestLE		= true;
} else {
	UsestLE		= false;
}

TH1D *hsskeep[19][2];
TH1D *hA[19][2];
TH1D *ffun_iter[200][2];
TH1D *ffun_chan[38];
TH1D *hinitialoffsetvals;
TH1D *hdt_end_chan_nLLa_mean;
TH1D *hdt_end_chan_nLLb_mean;
//
TH1D *ffun_chan_inito[38];
TH1D *ffun_chan_final[38];
TH1D *ffun_chan_finalext[38];
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
		sprintf(buf,"ffun_chan_finalext%d",idet);
		ffun_chan_finalext[idet]= new TH1D(buf,buf,128,0.,  64.0);
		ffun_chan_finalext[idet]->SetLineColor(4);
	} else {
		sprintf(buf,"ffun_chan%d",idet);
		ffun_chan[idet]			= new TH1D(buf,buf,128,0.,4096.0);
		sprintf(buf,"ffun_chan_inito%d",idet);
		ffun_chan_inito[idet]	= new TH1D(buf,buf,128,0.,4096.0);
		sprintf(buf,"ffun_chan_final%d",idet);
		ffun_chan_final[idet]	= new TH1D(buf,buf,128,0.,4096.0);
		sprintf(buf,"ffun_chan_finalext%d",idet);
		ffun_chan_finalext[idet]= new TH1D(buf,buf,128,0.,4096.0);
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
//hdt_end_chan_nLLb_mean = (TH1D*)f->Get("hdt_end_chan_nLLb_mean");
//cout<<hdt_end_chan_nLLb_mean<<endl;
//hdt_end_chan_nLLa_mean = (TH1D*)f->Get("hdt_end_chan_nLLa_mean");
//cout<<hdt_end_chan_nLLa_mean<<endl;
//
f->cd("ffun");
int icolors[8] = {2,3,4,6,7,32,16,38};

//---- collapse the iterations w/out initial offsets (?)
for (int i=0;i<NDETSIDE*NITERATIONS;i++){
	idet	= i%NDETSIDE;
	iter	= i/NDETSIDE;
	for (int is=0;is<2;is++){
		idetew	= idet + NDETSIDE*is;
		//
//		double val_off_fin	= hdt_end_chan_n10_mean->GetBinContent(idetew+1);
//		double val_off_fin	= hdt_end_chan_nLLa_mean->GetBinContent(idetew+1);
		//
		sprintf(buf,"ffun/ffun_iter%d%d",i,is);
			ffun_iter[i][is] = (TH1D*)f->Get(buf);
		if (ffun_iter[i][is]){
			ffun_iter[i][is]->SetLineColor(icolors[iter]);
			if (idet==0&&is==0){ ++NITERATIONSSEEN; }
			int nbx	= ffun_iter[i][is]->GetNbinsX();
			for (int ib=1;ib<=nbx;ib++){
				xval	 = ffun_iter[i][is]->GetXaxis()->GetBinCenter(ib);
				fval	 = ffun_iter[i][is]->GetBinContent(ib);
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
//
//---- not add in the initial offsets...
for (idet=0;idet<NDETSIDE;idet++){
	for (int is=0;is<2;is++){
		idetew	= idet + NDETSIDE*is;
		//
		double val_off_raw	= hinitialoffsetvals->GetBinContent(idetew+1);
		initoffvals[idetew]	= val_off_raw;
//		double val_off_fin	= hdt_end_chan_n10_mean->GetBinContent(idetew+1);
//		double val_off_fin	= hdt_end_chan_nLLa_mean->GetBinContent(idetew+1);
		double val_off_fin	= 0;						// cleanup offsets actually hurt!!!!
		//
		if (ffun_chan[idetew]){
		//if (is==1){
		//	cout<<is<<" "<<idetew<<" "
		//		<<val_off_raw<<" "<<val_off_fin<<endl;
		//}
			int nbx	= ffun_chan[idetew]->GetNbinsX();
			for (int ib=1;ib<=nbx;ib++){
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
//					ffun_chan_final[idetew]->Fill(xval,val_off_fin);
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
		if (UsesToT){ sprintf(buf,"A/htot%d%d",idet,is); } else 
		            { sprintf(buf,"A/hadc%d%d",idet,is); }
		hA[idet][is] = (TH1D*)f->Get(buf);
		sprintf(buf,"hA%d%d",idet,is);
		hA[idet][is]->SetName(buf);
		if (UsesToT){ hA[idet][is]->SetLineColor(2); } else 
		            { hA[idet][is]->SetLineColor(4); }
		if (!hA[idet][is]){
			cout<<" NO TOT!!!!! "<<idet<<" "<<is<<endl;
		}
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

//---- now find ADC upper limits....
cout<<"Finding limits"<<endl;
float totlim[38];
float valtotlim[38];
double csum,integral,rintegral,contents,xbin;
for (int idet=0;idet<19;idet++){
	for (int is=0;is<2;is++){
		idetew	= idet + NDETSIDE*is;
		totlim[idetew]	= -1;
		csum			= 0.0;
		integral 		= hA[idet][is]->GetEntries();
		for (int ibin=1;ibin<=128;ibin++){
			xbin		 = hA[idet][is]->GetBinCenter(ibin);
			hA[idet][is]->GetXaxis()->SetRange(1,ibin);
			rintegral	 = hA[idet][is]->Integral();
			contents	 = hA[idet][is]->GetBinContent(ibin);
			csum		+= contents;
			if (integral>0){
				//cout<<idet<<" "<<is<<" "<<integral<<"\t "
				//	<<ibin<<" "<<contents<<" "<<csum<<" "<<rintegral<<" \t "
				//	<<rintegral/integral
				//	<<endl;
				if (rintegral/integral<Fraction){
					hsskeep[idet][is]->SetBinContent(ibin,contents);
				} else {
					if (totlim[idetew]==-1){
						//if (UsesToT){
						//	xbin	= ((float)ibin)/2.0;
						//} else {
						//	xbin	= ((float)ibin)*32.0;
						//}
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

//---- plot slewing curves...
cout <<"plot slewing"<<endl;
for (int is=0;is<2;is++){
	++ican; sprintf(buf,"ccan%d",ican);
	ccan[ican] = new TCanvas(buf,buf,10+is*30,10+is*30,640,460);
	ccan[ican]->cd(); ccan[ican]->Divide(5,4,0.0001,0.0001);
	for (int idet=0;idet<NDETSIDE;idet++){
		idetew	= idet + NDETSIDE*is;
		ccan[ican]->cd(idet+1);
		for (int i=0;i<NDETSIDE*NITERATIONSSEEN;i++){
			iter	= i%NDETSIDE;
			if (ffun_iter[i][is]){
				ffun_iter[i][is]->SetMinimum(plotymin);
				ffun_iter[i][is]->SetMaximum(plotymax);
				if (iter==0) ffun_iter[i][is]->Draw();
				if (iter> 0) ffun_iter[i][is]->Draw("same");
			}
			ffun_chan[idetew]->SetLineWidth(2);
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
		gPad->SetLogy(1);
		hA[idet][is]->Draw();
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

//gStyle->SetOptStat(0);
//++ican; sprintf(buf,"ccan%d",ican);
//ccan[ican] = new TCanvas(buf,buf,130,130,640,460);
//ccan[ican]->cd(); ccan[ican]->Divide(1,1,0.0001,0.0001);
//	ccan[ican]->cd(1);
//	hdt_end_chan_nLLb_mean->Draw();
//	hdt_end_chan_nLLa_mean->Draw();
//ccan[ican]->cd(); ccan[is]->Update();
//sprintf(buf,"ps/%s.ps",ouf);
//ccan[ican]->Print(buf);

TLine *lvaltotlim[38];
for (int is=0;is<2;is++){
	for (int idet=0;idet<NDETSIDE;idet++){
		idetew	= idet + NDETSIDE*is;
		if (ffun_chan_inito[idetew]){
			//ffun_chan_inito[idetew]->SetMinimum(initoffvals[idetew]-3.);
			//ffun_chan_inito[idetew]->SetMaximum(initoffvals[idetew]+4.);
			lvaltotlim[idetew]	= new TLine(valtotlim[idetew],initoffvals[idetew]+plotymin,
			                                valtotlim[idetew],initoffvals[idetew]+plotymax);
		}
	}
}


//---- linear extend...................
//
TF1 *fext[38];
for (int is=0;is<2;is++){
	for (int idet=0;idet<NDETSIDE;idet++){
		//
		idetew	= idet + NDETSIDE*is;
		sprintf(buf,"fext%d",idetew);
		//
		if (UsesToT){
			fext[idetew]= new TF1(buf,myExt,valtotlim[idetew],AmaxFit,3);
			fext[idetew]->SetParLimits(2,0.,0.0012);
		} else {
			fext[idetew]= new TF1(buf,myExt,valtotlim[idetew],valtotlim[idetew]+AmaxFit/3,3);
		}
		fext[idetew]->SetLineColor(2);
		fext[idetew]->SetLineWidth(1);
		//
		if (valtotlim[idetew]<1) continue;
		//
		idetew		= idet + NDETSIDE*is;
		for (int ibin=1;ibin<=128;ibin++){
			float val	= ffun_chan_final[idetew]->GetBinContent(ibin);
			ffun_chan_finalext[idetew]->SetBinContent(ibin,val);
			float nent	= hA[idet][is]->GetBinContent(ibin);
			float err 	= 3.0;
			if (nent>0){ err	/= std::sqrt(nent); }
			ffun_chan_finalext[idetew]->SetBinError(ibin,err);
		}
		float xlim	= valtotlim[idetew];
		int klim	= ffun_chan_finalext[idetew]->FindBin(xlim);
		float fp	= ffun_chan_finalext[idetew]->GetBinContent(klim);
		float fpe	= ffun_chan_finalext[idetew]->GetBinError(klim);
		float fpe2	= fpe/10;
		if (valtotlim[idetew]>0){
			ffun_chan_finalext[idetew]->SetBinError(klim,fpe2);
			fpe		= ffun_chan_finalext[idetew]->GetBinError(klim+1);
			fpe2	= fpe/10;
			ffun_chan_finalext[idetew]->SetBinError(klim+1,fpe2);
		}
		//
		ffun_chan_finalext[idetew]->Fit(buf,"Q0NR");
		fext[idetew]->SetRange(valtotlim[idetew],Amax);
		//
		double pars[3];
		fext[idetew]->GetParameters(pars);
		cout<<idetew<<" "<<pars[0]<<" "<<pars[1]<<" "<<pars[2]<<endl;
		//
		float yprev,yuse;
		for (int ibin=klim;ibin<=128;ibin++){
			float xev	= ffun_chan_finalext[idetew]->GetBinCenter(ibin);
			float ynew	= fext[idetew]->Eval(xev);
			bool update	= false;
			if (ibin>klim){
				if ( UsestLE && ynew<=yprev) update = true;
				if (!UsestLE && ynew>=yprev) update = true;
			} else if (ibin==klim){
				update	= true;
				yuse	= ynew;
			}
			if (update){ yuse = ynew; }
			ffun_chan_finalext[idetew]->SetBinContent(ibin,yuse);
			yprev		= yuse;
		}
		//
	}
}
//
for (int is=0;is<2;is++){
	++ican; sprintf(buf,"ccan%d",ican);
	ccan[ican] = new TCanvas(buf,buf,160+is*30,160+is*30,640,460);
	ccan[ican]->cd(); ccan[ican]->Divide(5,4,0.0001,0.0001);
	for (int idet=0;idet<NDETSIDE;idet++){
		idetew	= idet + NDETSIDE*is;
		ccan[ican]->cd(idet+1);
		if (ffun_chan_final[idetew]){
			//
			ffun_chan_final[idetew]->SetMinimum(initoffvals[idetew]+plotymin);
			ffun_chan_final[idetew]->SetMaximum(initoffvals[idetew]+plotymax);
			ffun_chan_final[idetew]->SetLineWidth(2);
			ffun_chan_final[idetew]->SetLineColor(3);
			ffun_chan_final[idetew]->Draw();
			ffun_chan_finalext[idetew]->Draw("HIST same");
			lvaltotlim[idetew]->Draw("same");
			fext[idetew]->Draw("same");
			//
			//cout<<idetew<<" "<<valtotlim[idetew]<<endl;
			//
		}
	}
	ccan[ican]->cd(); ccan[is]->Update();
	sprintf(buf,"ps/%s.ps",ouf);
	ccan[ican]->Print(buf);
}




//--------------------------------------------------------------------------
//
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
			textout<<setw(16)<<ffun_chan_finalext[idetew]->GetBinLowEdge(ib)<<" ";
		} textout<<endl;
		for (int ib=1;ib<=128;ib++){
			float yval	= ffun_chan_finalext[idetew]->GetBinContent(ib);
			if (yval<-3000){ yval = 0.0; }
			textout<<setw(16)<<yval<<" ";
		} textout<<endl;
		//
	}
}
textout.close();

sprintf(buf,"ps/%s.ps]",ouf);
ccan[ican]->Print(buf);
//sprintf(buf,"/usr/bin/ps2pdf ps/%s.ps ps/%s.pdf",ouf,ouf);
sprintf(buf,"/usr/bin/ps2pdf ps/%s.ps ps/%s.pdf",ouf,ouf);
 	cout<<"executing unix command ... "<<buf<<endl;
 	gSystem->Exec(buf);
sprintf(buf,"/bin/cp ps/%s.pdf /star/u/geurts/WWW/files/",ouf);
 	cout<<"executing unix command ... "<<buf<<endl;
 	gSystem->Exec(buf);
 	
//---- build output root files and put them in WWW-accessible area....
sprintf(buf,"root/%s.root",ouf);
cout<<"writing output file    ... "<<buf<<endl;
TFile *fout = new TFile(buf,"RECREATE");
	TDirectory *f_off = fout->mkdir("offsets"); f_off->cd();
	//fout->cd();
	hinitialoffsetvals->Write();
	//hdt_end_chan_nLLb_mean->Write();
	//hdt_end_chan_nLLa_mean->Write();
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
			ffun_chan_finalext[idetew]->Write();
		}
	TDirectory *f_A = fout->mkdir("A"); f_A->cd();
	for (int idet=0;idet<19;idet++){
		for (int is=0;is<2;is++){
			hA[idet][is]->Write();
			hsskeep[idet][is]->Write();
		}
	}
	fout->cd();
fout->Close();
sprintf(buf,"/bin/cp root/%s.root /star/u/geurts/WWW/files/",ouf);
 	cout<<"executing unix command ... "<<buf<<endl;
 	gSystem->Exec(buf);
sprintf(buf,"/bin/cp root/%s.txt /star/u/geurts/WWW/files/",ouf);
 	cout<<"executing unix command ... "<<buf<<endl;
 	gSystem->Exec(buf);

}
//---- end of routine...

