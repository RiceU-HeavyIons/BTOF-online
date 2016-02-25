
void SplitPathName(TString pathname,TString *path,TString *name);

Double_t arg,result;
Double_t fpower(Double_t *x, Double_t *par){
	arg		= x[0]; 
  	result	= par[0]*TMath::Power(arg,par[1]);
	return result;
} 

int		NPTS		= 3;
float	TARGETADC	= 300;		// target for adc value
float	TARGETTOT	=  16.7;	// target for tot value	19.7

double  ADDTOF      = 0.;

int		DOCHECK		= 0;

void gainfit() {

	char hstringadc[40];
	char hstringtot[40];
	//sprintf(hstringadc,"hvpdmeanadc_bbccuth");
	//sprintf(hstringtot,"hvpdmeantot_bbccuth");
	sprintf(hstringadc,"hvpdmeanadc");
	sprintf(hstringtot,"hvpdmeantot");
	cout<<" Using ..."<<hstringadc<<endl;
	cout<<" Using ..."<<hstringtot<<endl;
	
	char  buf[60];
	float hvA[48];
	float hvB[48];
	float hvC[48];
	float hvX[48];
	float hvmA[38];
	float hvmB[38];
	float hvmC[38];
	float hvmX[38];
	int	k = -1;
	int m = -1;
	float dadc[10][38];
	float dtot[10][38];
	TGraph *ghvA = new TGraph();
	TGraph *ghvB = new TGraph();
	TGraph *ghvC = new TGraph();
	TGraph *ghvX = new TGraph();
	ghvA->SetMarkerColor(7);
	ghvB->SetMarkerColor(4);
	ghvC->SetMarkerColor(7);
	ghvX->SetMarkerColor(3);
	ghvA->SetLineColor(7);
	ghvB->SetLineColor(4);
	ghvC->SetLineColor(7);
	ghvX->SetLineColor(3);
	ghvA->SetLineWidth(2);
	ghvB->SetLineWidth(2);
	ghvC->SetLineWidth(2);
	ghvX->SetLineWidth(2);
	int mask[48];
	ifstream infA; infA.open("./setA.save",ifstream::in);
	ifstream infB; infB.open("./setB.save",ifstream::in);
	ifstream infC; infC.open("./setC.save",ifstream::in);
	ifstream infX; if (DOCHECK) infX.open("./setX.save",ifstream::in);
	float kkkdum;
	for (int is=0;is<48;is++){
		infA>>kkkdum;		hvA[is]=kkkdum;
		infB>>kkkdum;		hvB[is]=kkkdum;
		infC>>kkkdum;		hvC[is]=kkkdum;
		if (DOCHECK) {infX>>kkkdum;		hvX[is]=kkkdum;}
		mask[is] 		= -1;
		if (hvA[is]>0){ 
			++k; 
			mask[is]	= k;
			hvmA[k]		= hvA[is];
			hvmB[k]		= hvB[is];
			hvmC[k]		= hvC[is];
			hvmX[k]		= hvX[is];
			ghvA->SetPoint(k,k,hvmA[k]);
			ghvB->SetPoint(k,k,hvmB[k]);
			ghvC->SetPoint(k,k,hvmC[k]);
			ghvX->SetPoint(k,k,hvmX[k]);
		}
		cout<<is<<" "<<k<<" "<<hvA[is]<<" "<<hvB[is]<<" "<<hvC[is]<<"\t "<<hvX[is]<<endl;
	}
	infA.close();
	infB.close();
	infC.close();
	if (DOCHECK) infX.close();
	
	TH1D *hpvpdtofA;
	TH1D *hpvpdtofB;
	TH1D *hpvpdtofC;
	TH1D *hpvpdtofX;
	
	float vala,valt;
	TFile *f1	= new TFile("online_16122004.root","READ");
		f1->cd();
		TH1D *hadc = (TH1D*)gDirectory->Get(hstringadc);
		      hadc->SetName("hadc");
		TH1D *htot = (TH1D*)gDirectory->Get(hstringtot);
		      htot->SetName("htot");
		hpvpdtofA = (TH1D*)gDirectory->Get("hpvpdtof");
		      hpvpdtofA->SetName("hpvpdtofA");
		      hpvpdtofA->SetDirectory(0);
		for (int ib=1;ib<=38;ib++){
			vala	= hadc->GetBinContent(ib);			
			valt	= htot->GetBinContent(ib);
			if (ib-1<19){ m=(ib-1)+19; }else{ m=(ib-1)-19; }
			dadc[0][m]	= vala;
			dtot[0][m]	= valt;
		}
	f1->Close();
	TFile *f1	= new TFile("online_16122002.root","READ");
		f1->cd();
		TH1D *hadc = (TH1D*)gDirectory->Get(hstringadc);
		      hadc->SetName("hadc");
		TH1D *htot = (TH1D*)gDirectory->Get(hstringtot);
		      htot->SetName("htot");
		hpvpdtofB = (TH1D*)gDirectory->Get("hpvpdtof");
		      hpvpdtofB->SetName("hpvpdtofB");
		      hpvpdtofB->SetDirectory(0);
		for (int ib=1;ib<=38;ib++){
			vala	= hadc->GetBinContent(ib);			
			valt	= htot->GetBinContent(ib);			
			if (ib-1<19){ m=(ib-1)+19; }else{ m=(ib-1)-19; }
			dadc[1][m]	= vala;
			dtot[1][m]	= valt;
		}
	f1->Close();
	TFile *f1	= new TFile("online_16122003.root","READ");
		f1->cd();
		TH1D *hadc = (TH1D*)gDirectory->Get(hstringadc);
		      hadc->SetName("hadc");
		TH1D *htot = (TH1D*)gDirectory->Get(hstringtot);
		      htot->SetName("htot");
		hpvpdtofC = (TH1D*)gDirectory->Get("hpvpdtof");
		      hpvpdtofC->SetName("hpvpdtofC");
		      hpvpdtofC->SetDirectory(0);
		for (int ib=1;ib<=38;ib++){
			vala	= hadc->GetBinContent(ib);			
			valt	= htot->GetBinContent(ib);			
			if (ib-1<19){ m=(ib-1)+19; }else{ m=(ib-1)-19; }
			dadc[2][m]	= vala;
			dtot[2][m]	= valt;
		}
	f1->Close();

//---- check runs....
	if (DOCHECK){
		TGraph *ptscheckadc[38];	
		TGraph *ptschecktot[38];	
		TFile *f1	= new TFile("online_15073033.root","READ");
			f1->cd();
			TH1D *hadc = (TH1D*)gDirectory->Get(hstringadc);
				  hadc->SetName("hadc");
			TH1D *htot = (TH1D*)gDirectory->Get(hstringtot);
				  htot->SetName("htot");
			hpvpdtofX = (TH1D*)gDirectory->Get("hpvpdtof");
				  hpvpdtofX->SetName("hpvpdtofX");
				  hpvpdtofX->SetDirectory(0);
			for (int ib=1;ib<=38;ib++){
				vala	= hadc->GetBinContent(ib);			
				valt	= htot->GetBinContent(ib);			
				if (ib-1<19){ m=(ib-1)+19; }else{ m=(ib-1)-19; }
				dadc[3][m]	= vala;
				dtot[3][m]	= valt;
				ptscheckadc[m] = new TGraph();
				ptscheckadc[m]->SetPoint(0,TMath::Log(hvmX[m]),TMath::Log(vala));
				ptscheckadc[m]->SetMarkerStyle(24);
				ptschecktot[m] = new TGraph();
				ptschecktot[m]->SetPoint(0,TMath::Log(hvmX[m]),TMath::Log(valt));
				ptschecktot[m]->SetMarkerStyle(24);
			}
		f1->Close();
	}



	TGraphErrors *gainadc[38];
	TGraphErrors *gaintot[38];
//	float  dv[3]	= {0,-300,-400};
//	float  dv2[3]	= {-100, 0, 100};
	for (int ic=0;ic<38;ic++){
		gainadc[ic]			= new TGraphErrors();
		gaintot[ic]			= new TGraphErrors();
		if (dadc[0][ic]>0){
// 			gainadc[ic]->SetPoint(0,TMath::Log(hvm[ic]+dv[2]),TMath::Log(dadc[2][ic]));
// 			gainadc[ic]->SetPoint(1,TMath::Log(hvm[ic]+dv[1]),TMath::Log(dadc[1][ic]));
// 			gainadc[ic]->SetPoint(2,TMath::Log(hvm[ic]+dv[0]),TMath::Log(dadc[0][ic]));
// 			if (NPTS==4) gainadc[ic]->SetPoint(3,TMath::Log(hvm2[ic]),TMath::Log(dadc[3][ic]));
// 			gainadc[ic]->SetPointError(0,0,0.01*TMath::Log(dadc[2][ic]));
// 			gainadc[ic]->SetPointError(1,0,0.01*TMath::Log(dadc[1][ic]));
// 			gainadc[ic]->SetPointError(2,0,0.10*TMath::Log(dadc[0][ic]));
// 			if (NPTS==4) gainadc[ic]->SetPointError(3,0,0.01*TMath::Log(dadc[3][ic]));
			gainadc[ic]->SetPoint(0,TMath::Log(hvmA[ic]),TMath::Log(dadc[0][ic]));
			gainadc[ic]->SetPoint(1,TMath::Log(hvmB[ic]),TMath::Log(dadc[1][ic]));
			gainadc[ic]->SetPoint(2,TMath::Log(hvmC[ic]),TMath::Log(dadc[2][ic]));
			gainadc[ic]->SetPointError(0,0,0.01*TMath::Log(dadc[0][ic]));
			gainadc[ic]->SetPointError(1,0,0.01*TMath::Log(dadc[1][ic]));
			gainadc[ic]->SetPointError(2,0,0.01*TMath::Log(dadc[2][ic]));
if (ic>=36){
cout<<"raw data... "<<ic<<" \t "<<hvmA[ic]<<" "<<dadc[0][ic]
		<<" \t "<<hvmB[ic]<<" "<<dadc[1][ic]
		<<" \t "<<hvmC[ic]<<" "<<dadc[2][ic]
	<<endl;
}
		}
		if (dtot[0][ic]>0){
			gaintot[ic]->SetPoint(0,TMath::Log(hvmA[ic]),TMath::Log(dtot[0][ic]));
			gaintot[ic]->SetPoint(1,TMath::Log(hvmB[ic]),TMath::Log(dtot[1][ic]));
			gaintot[ic]->SetPoint(2,TMath::Log(hvmC[ic]),TMath::Log(dtot[2][ic]));
			gaintot[ic]->SetPointError(0,0,0.005*TMath::Log(dtot[0][ic]));
			gaintot[ic]->SetPointError(1,0,0.005*TMath::Log(dtot[1][ic]));
			gaintot[ic]->SetPointError(2,0,0.005*TMath::Log(dtot[2][ic]));
		}
		//
		gainadc[ic]->SetMarkerStyle(20);
		gainadc[ic]->SetMarkerColor(1);
		gaintot[ic]->SetMarkerStyle(20);
		gaintot[ic]->SetMarkerColor(4);
		//
	}
	
	TF1 *funpoweradc[38];
	TF1 *funpowertot[38];
	TH1D *hgainslopeadc	= new TH1D("hgainslopeadc","hgainslopeadc",100,0.0,10.0);
	TGraph *gainslopeadc = new TGraph();
			gainslopeadc->SetMarkerStyle(20);
			gainslopeadc->SetMarkerColor(1);
	TH1D *hgainslopetot	= new TH1D("hgainslopetot","hgainslopetot",100,0.0,2.5);
	TGraph *gainslopetot = new TGraph();
			gainslopetot->SetMarkerStyle(20);
			gainslopetot->SetMarkerColor(1);
		
	TH1D *hgaininteradc	= new TH1D("hgaininteradc","hgaininteradc",100,-60.0,-20.0);
	TGraph *gaininteradc = new TGraph();
			gaininteradc->SetMarkerStyle(20);
			gaininteradc->SetMarkerColor(1);
	TH1D *hgainintertot	= new TH1D("hgainintertot","hgainintertot",100,-12.0,-2.0);
	TGraph *gainintertot = new TGraph();
			gainintertot->SetMarkerStyle(20);
			gainintertot->SetMarkerColor(1);

	Double_t	fpars[2];
	double xtmp,ytmp,useval,iuseval,adum;
	float xminadc[38],yminadc[38],xmaxadc[38],ymaxadc[38];
	float xmintot[38],ymintot[38],xmaxtot[38],ymaxtot[38];
	//
	//---- ADC
	k	= -1;
	for (int ic=0;ic<38;ic++){
		sprintf(buf,"funpoweradc%d",ic);
		funpoweradc[ic] = new TF1(buf,"pol1",1,10);
		funpoweradc[ic]->SetLineWidth(2); funpoweradc[ic]->SetLineColor(2);
		if (gainadc[ic]->GetN()){
			//
			//sprintf(buf,"funpoweradc%d",ic);
			//funpoweradc[ic] = new TF1(buf,fpower,100,2300,2);
			cout<<ic<<" -----------------"<<endl;
			gainadc[ic]->Print();
			gainadc[ic]->Fit(buf,"QNR");
				funpoweradc[ic]->GetParameters(fpars);
				++k;
				gaininteradc->SetPoint(k,ic,fpars[0]);
				hgaininteradc->Fill(fpars[0]);
				gainslopeadc->SetPoint(k,ic,fpars[1]);
				hgainslopeadc->Fill(fpars[1]);
			//
			useval	= 99999.;
			for (int ip=0;ip<NPTS;ip++){
				gainadc[ic]->GetPoint(ip,xtmp,ytmp);
				if (xtmp<useval){ useval=xtmp; iuseval=ip; }
			}
			xminadc[ic]	= 0.987*useval;
			//
			useval	= 99999.;
			for (int ip=0;ip<NPTS;ip++){
				gainadc[ic]->GetPoint(ip,xtmp,ytmp);
				if (ytmp<useval){ useval=ytmp; iuseval=ip; }
			}
			yminadc[ic]	= 0.85*useval;
			//
			useval	= 0.;
			for (int ip=0;ip<NPTS;ip++){
				gainadc[ic]->GetPoint(ip,xtmp,ytmp);
				if (xtmp>useval){ useval=xtmp; iuseval=ip; }
			}
			xmaxadc[ic]	= 1.013*useval;
			//xmaxadc[ic]	= 1.03*useval;
			//
			useval	= 0.;
			for (int ip=0;ip<NPTS;ip++){
				gainadc[ic]->GetPoint(ip,xtmp,ytmp);
				if (ytmp>useval){ useval=ytmp; iuseval=ip; }
			}
			ymaxadc[ic]	= 1.15*useval;
			//ymaxadc[ic]	= 1.40*useval;
			//
			//cout<<"RANGE "<<ic<<" "<<xmin[ic]<<" "<<ymin[ic]
			//		<<" "<<xmax[ic]<<" "<<ymax[ic]<<endl;
		}
	}
	//
	//---- TOT
	k	= -1;
	for (int ic=0;ic<38;ic++){
		sprintf(buf,"funpowertot%d",ic);
		funpowertot[ic] = new TF1(buf,"pol1",1,10);
		funpowertot[ic]->SetLineWidth(2); funpowertot[ic]->SetLineColor(2);
		if (gaintot[ic]->GetN()){
			//
			//sprintf(buf,"funpowertot%d",ic);
			//funpowertot[ic] = new TF1(buf,fpower,100,2300,2);
			gaintot[ic]->Fit(buf,"QNR");
				funpowertot[ic]->GetParameters(fpars);
				++k;
				gainintertot->SetPoint(k,ic,fpars[0]);
				hgainintertot->Fill(fpars[0]);
				gainslopetot->SetPoint(k,ic,fpars[1]);
				hgainslopetot->Fill(fpars[1]);
			//
			useval	= 99999.;
			for (int ip=0;ip<NPTS;ip++){
				gaintot[ic]->GetPoint(ip,xtmp,ytmp);
				if (xtmp<useval){ useval=xtmp; iuseval=ip; }
			}
			xmintot[ic]	= 0.987*useval;
			//
			useval	= 99999.;
			for (int ip=0;ip<NPTS;ip++){
				gaintot[ic]->GetPoint(ip,xtmp,ytmp);
				if (ytmp<useval){ useval=ytmp; iuseval=ip; }
			}
			ymintot[ic]	= 0.920*useval;
			//
			useval	= 0.;
			for (int ip=0;ip<NPTS;ip++){
				gaintot[ic]->GetPoint(ip,xtmp,ytmp);
				if (xtmp>useval){ useval=xtmp; iuseval=ip; }
			}
			xmaxtot[ic]	= 1.02*useval;
			//xmaxtot[ic]	= 1.03*useval;
			//
			useval	= 0.;
			for (int ip=0;ip<NPTS;ip++){
				gaintot[ic]->GetPoint(ip,xtmp,ytmp);
				if (ytmp>useval){ useval=ytmp; iuseval=ip; }
			}
			ymaxtot[ic]	= 1.08*useval;
			//ymaxtot[ic]	= 1.40*useval;
			//
			cout<<"RANGE "<<ic<<" "<<xmintot[ic]<<" "<<ymintot[ic]
					<<" "<<xmaxtot[ic]<<" "<<ymaxtot[ic]<<endl;
		}
	}

	float xuse,yval,yact;
	
	TGraph *SPadc[38];
	float HVuseadc[38];
	float xuse,yval,yact;
	TGraph *newhvadc = new TGraph();
		newhvadc->SetMarkerStyle(22);
		newhvadc->SetMarkerSize(2);
	for (int ic=0;ic<38;ic++){
		HVuseadc[ic]	= -1;
		SPadc[ic]		= new TGraph();
		for (int iv=500;iv<2300;iv++){
			if (HVuseadc[ic]<0 && gainadc[ic]->GetN()){
				xuse	= TMath::Log((float)iv);
				yval	= funpoweradc[ic]->Eval(xuse);
				yact	= TMath::Exp(yval);
				if (yact>=TARGETADC){
					cout<<"ADC.... "<<ic<<" "<<iv<<" "<<xuse<<" "<<yval<<" "<<yact<<endl;
					HVuseadc[ic]	= iv;
					SPadc[ic]->SetPoint(0,TMath::Log(HVuseadc[ic]),yval);
					SPadc[ic]->SetMarkerStyle(28);
					SPadc[ic]->SetMarkerColor(1);
					SPadc[ic]->SetMarkerSize(2);
					newhvadc->SetPoint(ic,ic,HVuseadc[ic]);
				}
			}
		}
	}
	TGraph *SPtot[38];
	float HVusetot[38];
	TGraph *newhvtot = new TGraph();
		newhvtot->SetMarkerStyle(23);
		newhvtot->SetMarkerSize(2);
	for (int ic=0;ic<38;ic++){
		HVusetot[ic]	= -1;
		SPtot[ic]		= new TGraph();
		for (int iv=500;iv<2300;iv++){
			if (HVusetot[ic]<0 && gaintot[ic]->GetN()){
				xuse	= TMath::Log((float)iv);
				yval	= funpowertot[ic]->Eval(xuse);
				yact	= TMath::Exp(yval);
				if (yact>=TARGETTOT){
					cout<<"ToT.... "<<ic<<" "<<iv<<" "<<xuse<<" "<<yval<<" "<<yact<<endl;
					HVusetot[ic]	= iv;
					SPtot[ic]->SetPoint(0,TMath::Log(HVusetot[ic]),yval);
					SPtot[ic]->SetMarkerStyle(28);
					SPtot[ic]->SetMarkerColor(1);
					SPtot[ic]->SetMarkerSize(2);
					newhvtot->SetPoint(ic,ic,HVusetot[ic]);
				}
			}
		}
	}

	int kkc	= 0;
	TGraph *hvadctotcomp	= new TGraph();
			hvadctotcomp->SetMarkerStyle(20);
			hvadctotcomp->SetMarkerColor(4);
	TH1D *hhvadctotcomp	= new TH1D("hhvadctotcomp","hhvadctotcomp",25,-200,200);
	for (int ic=0;ic<38;ic++){
		if (HVuseadc[ic]>500&&HVusetot[ic]>500){
			hvadctotcomp->SetPoint(kkc,HVuseadc[ic],HVusetot[ic]);
			++kkc;
			hhvadctotcomp->Fill(HVuseadc[ic]-HVusetot[ic]);
		}
	}	

	float 	hvval;
	int		ipval;
	float	vfinal[48];
	int		kadded = -1;
	TGraph  *added = new TGraph();
		added->SetMarkerStyle(20);
		added->SetMarkerColor(3);
		added->SetMarkerSize(2);
	for (int is=0;is<48;is++){
		ipval	= -1;
		ytmp	=  0.0;
		if (mask[is]>=0){
			ipval	= mask[is];
			newhvadc->GetPoint(ipval,xtmp,ytmp);		// Get value from ADC fits....
			vfinal[is]	= ytmp;
//			newhvtot->GetPoint(ipval,xtmp,ytmp);		// Get value from ToT fits....
		}
		if (hvA[is]>0.0&&ytmp<100.){
			newhvtot->GetPoint(ipval,xtmp,ytmp);
			added->SetPoint(++kadded,ipval,ytmp+ADDTOF);
			vfinal[is]	= ytmp+ADDTOF;
		}
		//cout<<is<<" "<<ipval<<" "<<hvA[is]<<" "<<vfinal[is]<<"\t diff="<<vfinal[is]-hvA[is]<<endl;	
	}
	
	cout<<endl;
	cout<<endl;
	for (int is=0;is<48;is++){
		if (vfinal[is]>0.0){
//			cout<<is+1<<" "<<vfinal[is]<<endl;
			cout<<vfinal[is]<<endl;
		} else {
//			cout<<is+1<<" "<<"0.0"<<endl;
			cout<<"0.0"<<endl;
		}
	}
	cout<<endl;

	//dadc[7][ic]	= mean adc from 10365009
	//hvm3[ic]		= set 3 (mismapped values)
	//HVuse[ic]		= set 4 (correctly mapped values)
// 	TGraph *errorcheck = new TGraph();
// 		errorcheck->SetMarkerStyle(20);
// 		errorcheck->SetMarkerSize(2);
// 		errorcheck->SetMarkerColor(4);
// 	TGraph *errorcheckb = new TGraph();
// 		errorcheckb->SetMarkerStyle(20);
// 		errorcheckb->SetMarkerSize(2);
// 		errorcheckb->SetMarkerColor(4);
// 	for (int ic=0;ic<38;ic++){
// 		errorcheck->SetPoint(ic,dadc[7][ic],hvm3[ic]-HVuse[ic]);
// 		errorcheckb->SetPoint(ic,dadc[7][ic],TMath::Log(hvm3[ic]-HVuse[ic]+500.));
// 		if (dadc[7][ic]>800){
// 			cout<<ic<<" "<<dadc[7][ic]<<" "<<hvm3[ic]-HVuse[ic]<<endl;
// 		}
// 	}

	//---------------------------------------------------------------------------
	
	gStyle->SetPadRightMargin(0.02);
	gStyle->SetPadTopMargin(0.02);
	gStyle->SetPadBottomMargin(0.07);
	gStyle->SetPadLeftMargin(0.07);


	TLatex *tt	= new TLatex();
			tt->SetNDC();
			tt->SetTextSize(0.10);

	TLine *lladc	= new TLine(6.6,TMath::Log(TARGETADC),7.7,TMath::Log(TARGETADC));
		lladc->SetLineStyle(2);
	TCanvas *gainfit_1 = new TCanvas("gainfit_1","gainfit_1",0,0,750,570);
	gainfit_1->cd(0);  gainfit_1->Divide(7,6,0.0001,0.0001);
	for (int ic=0;ic<38;ic++){
		gainfit_1->cd(1+ic);
		if (gainadc[ic]->GetN()){
			frame	=	gPad->DrawFrame(xminadc[ic],yminadc[ic],xmaxadc[ic],ymaxadc[ic]);
				//gPad->SetLogx(1);
				//gPad->SetLogy(1);
			gainadc[ic]->Draw("P");
			funpoweradc[ic]->Draw("same");
			SPadc[ic]->Draw("P");
			lladc->Draw("same");
			if (DOCHECK){
				ptscheckadc[ic]->Draw("P");
			}
			//
			sprintf(buf,"ic=%d",ic);
			tt->DrawLatex(0.13,0.86,buf);
			ghvB->GetPoint(ic,adum,xtmp);			// value of previous default!!!!!!
			newhvadc->GetPoint(ic,adum,ytmp);
			sprintf(buf,"%4.0f#rightarrow%4.0f",xtmp,ytmp);
			tt->DrawLatex(0.13,0.74,buf);
			SPadc[ic]->GetPoint(0,xtmp,adum);
			sprintf(buf,"New: %4.0f",TMath::Exp(xtmp));			
			tt->DrawLatex(0.59,0.15,buf);
		}
	}
	gainfit_1->Update(); gainfit_1->cd();
	gainfit_1->Print("gainfit.ps(");

	TLine *lltot	= new TLine(6.6,TMath::Log(TARGETTOT),7.7,TMath::Log(TARGETTOT));
		lltot->SetLineStyle(2);
	TCanvas *gainfit_1b = new TCanvas("gainfit_1b","gainfit_1b",30,30,750,570);
	gainfit_1b->cd(0);  gainfit_1b->Divide(7,6,0.0001,0.0001);
	for (int ic=0;ic<38;ic++){
		gainfit_1b->cd(1+ic);
		if (gaintot[ic]->GetN()){
			cout<<xmintot[ic]<<" "<<ymintot[ic]<<" "<<xmaxtot[ic]<<" "<<ymaxtot[ic]<<" "<<endl;
			frame	=	gPad->DrawFrame(xmintot[ic],ymintot[ic],xmaxtot[ic],ymaxtot[ic]);
				//gPad->SetLogx(1);
				//gPad->SetLogy(1);
			gaintot[ic]->Draw("P");
			funpowertot[ic]->Draw("same");
			SPtot[ic]->Draw("P");
			lltot->Draw("same");
			if (DOCHECK){
				ptschecktot[ic]->Draw("P");
			}
			//
			sprintf(buf,"ic=%d",ic);
			tt->DrawLatex(0.13,0.86,buf);
			ghvB->GetPoint(ic,adum,xtmp);			// value of previous default!!!!!!
			newhvtot->GetPoint(ic,adum,ytmp);
			sprintf(buf,"%4.0f#rightarrow%4.0f",xtmp,ytmp);
			tt->DrawLatex(0.13,0.74,buf);
			SPtot[ic]->GetPoint(0,xtmp,adum);
			sprintf(buf,"New: %4.0f",TMath::Exp(xtmp));			
			tt->DrawLatex(0.59,0.15,buf);
		}
	}
	gainfit_1b->Update(); gainfit_1b->cd();
	gainfit_1b->Print("gainfit.ps");

	
	TCanvas *gainfit_2 = new TCanvas("gainfit_2","gainfit_2",60,60,750,570);
	gainfit_2->cd(0);  gainfit_2->Divide(1,1,0.0001,0.0001);
	gainfit_2->cd(1);
		frame		= gPad->DrawFrame(-0.5,1000,37.5,2300);
		TLine *vert	= new TLine(18.5,1000,18.5,2300);
				vert->Draw();
		newhvadc->Draw("P");
		newhvtot->Draw("P");
		added->Draw("P");
		ghvA->Draw("LP");
		ghvB->Draw("LP");
		ghvC->Draw("LP");
		TLegend *leg	= new TLegend(0.10,0.11,0.46,0.28);
			leg->SetFillStyle(0);
			leg->SetTextSize(0.03);
			//leg->AddEntry(ghv3,"10363010, 2nd set","PL");
			leg->AddEntry(ghvC,"set C","PL");
			leg->AddEntry(ghvB,"set B","PL");
			leg->AddEntry(ghvA,"set A","PL");
			leg->AddEntry(newhvadc,"New Values ADC","P");
			leg->AddEntry(newhvtot,"New Values ToT","P");
			leg->Draw();
	gainfit_2->Update(); gainfit_2->cd();
	gainfit_2->Print("gainfit.ps");

	gStyle->SetPadRightMargin(0.03);
	gStyle->SetPadTopMargin(0.02);
	gStyle->SetPadBottomMargin(0.09);
	gStyle->SetPadLeftMargin(0.07);
	
	TCanvas *gainfit_3 = new TCanvas("gainfit_3","gainfit_3",90,90,750,570);
	gainfit_3->cd(0);  gainfit_3->Divide(2,1,0.0001,0.0001);
	gainfit_3->cd(1);
		frame		= gPad->DrawFrame(900,900,2300,2300);
		TLine *diag	= new TLine(900,900,2300,2300);
			diag->Draw();
		hvadctotcomp->Draw("P");
	gainfit_3->cd(2);
		hhvadctotcomp->Draw();
	gainfit_3->Update(); gainfit_3->cd();
	gainfit_3->Print("gainfit.ps");

	gStyle->SetErrorX(0);
	TCanvas *gainfit_4 = new TCanvas("gainfit_4","gainfit_4",120,120,750,570);
	gainfit_4->cd(0);  gainfit_4->Divide(1,1,0.0001,0.0001);
	gainfit_4->cd(1);
		hpvpdtofC->SetStats(0);
		hpvpdtofC->SetLineColor(2);
		hpvpdtofC->SetMarkerColor(2);
		hpvpdtofC->SetMarkerStyle(20);
		hpvpdtofC->Draw("P");
		hpvpdtofB->SetStats(0);
		hpvpdtofB->SetLineColor(4);
		hpvpdtofB->SetMarkerColor(4);
		hpvpdtofB->SetMarkerStyle(22);
		hpvpdtofB->Draw("P same");
		hpvpdtofA->SetStats(0);
		hpvpdtofA->SetLineColor(6);
		hpvpdtofA->SetMarkerColor(6);
		hpvpdtofA->SetMarkerStyle(23);
		hpvpdtofA->Draw("P same");
		if (DOCHECK){
			hpvpdtofX->SetStats(0);
			hpvpdtofX->SetLineColor(1);
			hpvpdtofX->Draw("hist same");
		}
	gainfit_4->Update(); gainfit_4->cd();
	gainfit_4->Print("gainfit.ps");



// 	TLine *l1 = new TLine(0,0,300,0);
// 		l1->SetLineWidth(2.0); l1->SetLineStyle(2);
// 	TLine *l2 = new TLine(300,-400,300,0);
// 		l2->SetLineWidth(2.0); l2->SetLineStyle(2);
// 	TCanvas *gainfit_3 = new TCanvas("gainfit_3","gainfit_3",60,60,750,570);
// 	gainfit_3->cd(0);  gainfit_3->Divide(1,1,0.0001,0.0001);
// 	gainfit_3->cd(1);
// 		frame	=	gPad->DrawFrame(0,-400,1700,400);
// 			frame->GetXaxis()->SetTitle("<ADC> 10365009 (using Set 3)");
// 			frame->GetYaxis()->SetTitle("[HV Set 3] - [HV Set 4]");
// 		errorcheck->Draw("P");
// 			l1->Draw();
// 			l2->Draw();
// 	gainfit_3->Update(); gainfit_3->cd();
// 	gainfit_3->Print("gainfit.ps)");

// 	TCanvas *gainfit_4 = new TCanvas("gainfit_4","gainfit_4",90,90,750,570);
// 	gainfit_4->cd(0);  gainfit_4->Divide(1,1,0.0001,0.0001);
// 	gainfit_4->cd(1);
// 		frame	=	gPad->DrawFrame(0,0,1700,10);
// 			frame->GetXaxis()->SetTitle("<ADC> 10365009 (using Set 3)");
// 			frame->GetYaxis()->SetTitle("LOG([HV Set 3] - [HV Set 4] + 500)");
// 		errorcheckb->Draw("P");
// 	gainfit_4->Update(); gainfit_4->cd();
// 	gainfit_4->Print("gainfit.ps)");


	TCanvas *gainfit_5 = new TCanvas("gainfit_5","gainfit_5",150,150,750,570);
	gainfit_5->cd(0);  gainfit_5->Divide(2,2,0.0001,0.0001);
	gainfit_5->cd(1);
		gainslopeadc->SetMinimum(0);
		gainslopeadc->SetMaximum(10);
		gainslopeadc->Draw("AP");
	gainfit_5->cd(2);
		hgainslopeadc->Draw();
	gainfit_5->cd(3);
		gainslopetot->SetMinimum(0);
		gainslopetot->SetMaximum(2.5);
		gainslopetot->Draw("AP");
	gainfit_5->cd(4);
		hgainslopetot->Draw();
	gainfit_5->Update(); gainfit_5->cd();
	gainfit_5->Print("gainfit.ps");

 	TCanvas *gainfit_6 = new TCanvas("gainfit_6","gainfit_6",180,180,750,570);
 	gainfit_6->cd(0);  gainfit_6->Divide(2,2,0.0001,0.0001);
 	gainfit_6->cd(1);
 		gaininteradc->SetMinimum(-60);
 		gaininteradc->SetMaximum(-20);
 		gaininteradc->Draw("AP");
 	gainfit_6->cd(2);
 		hgaininteradc->Draw();
 	gainfit_6->cd(3);
 		gainintertot->SetMinimum(-12);
 		gainintertot->SetMaximum(-2);
 		gainintertot->Draw("AP");
 	gainfit_6->cd(4);
 		hgainintertot->Draw();
 	gainfit_6->Update(); gainfit_6->cd();
 	gainfit_6->Print("gainfit.ps)");

	char* thisdir	= gSystem->WorkingDirectory();
	TString sthisdir= TString(thisdir);
	cout<<sthisdir.Data()<<endl;
	TString path;
	TString name;
	SplitPathName(sthisdir,&path,&name);
	cout<<name.Data()<<endl;
	TString outfile = name + TString(".root");
	cout<<outfile.Data()<<endl;
	TFile *f	= new TFile(outfile.Data(),"recreate");
		hgainslopeadc->Write();
		hgainslopetot->Write();
		gainslopeadc->SetName("gainslopeadc");
		gainslopetot->SetName("gainslopetot");
		gainslopeadc->Write();
		gainslopetot->Write();
		hgaininteradc->Write();
		hgainintertot->Write();
		gaininteradc->SetName("gaininteradc");
		gainintertot->SetName("gainintertot");
		gaininteradc->Write();
		gainintertot->Write();
		//
		newhvadc->SetName("newhvadc");
		newhvtot->SetName("newhvtot");
		added->SetName("added");
		ghvA->SetName("ghvA");
		ghvB->SetName("ghvB");
		ghvC->SetName("ghvC");
		newhvadc->Write();
		newhvtot->Write();
		added->Write();
		ghvA->Write();
		ghvB->Write();
		ghvC->Write();
		//
		f->ls();
	f->Close();

	//---------------------------------------------------------------------------	
	
}
void SplitPathName(TString pathname,TString *path,TString *name) {
    int slash = pathname.Last('/');
    *path = pathname(0,slash);
    *name = pathname(slash+1,pathname.Length()-slash-1);
}
