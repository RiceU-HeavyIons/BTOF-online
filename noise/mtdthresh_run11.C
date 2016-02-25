
const float tofarea =  20.;
const float mtd9area = 218.;
const float mtd11area = 331.;

const float TRAYCUTOFF = 4.;

Double_t power(Double_t *x, Double_t *par) {
  Float_t 	xx	= x[0];
  Double_t 	f	= par[0]*TMath::Power(10.,par[1]*xx);
  //cout<<xx<<" "<<par[0]<<" "<<par[1]<<" "<<f<<endl;
  return f;
}
 

void mtdthresh(){

	TH1::AddDirectory(kFALSE);
	gErrorIgnoreLevel	= 2000;

	char		buf[100];
	char		tbuf[100];
	TCanvas*	ccan[100];
	int			ican	= -1;
	
	const int nruns    = 18;
	int iorder[nruns]  = {  6, 15,  8, 16,  5, 17,  9, 10, 11,   4,  12,   3,   2,   1,   7,  13,  18,  14};
	int runnums[nruns] = { 30, 41, 32, 42, 29, 43, 33, 34, 35,  28,  36,  27,  26,  25,  31,  37,  45,  40};
	float thresh[nruns]= {250,312,375,437,500,562,625,750,875,1000,1250,1500,2000,2500,2500,2500,2500,3000};
	float itime_byorder[nruns] = {10.0,16.75,23.5,30.,36.25,42.5,49.,55.5,62.0,68.25,74.75,82.5,88.75,
                                      103.5,109.5,115.75,121.75,137.25};
	
	TH1D*	hinfo[nruns];
	TH1D*	hrate_tray[nruns];
	TH1D*	htot[nruns];
	TH1D*	htot_mtd9[nruns];
	TH1D*	htot_mtd11e[nruns];
	TH1D*	htot_mtd11c[nruns];
	TH1D*	htot_mtd11w[nruns];

	TGraph *gglobalnoise	= new TGraph();
			gglobalnoise->SetMarkerStyle(20);
			gglobalnoise->SetMarkerColor(1);
			gglobalnoise->SetLineColor(1);
			gglobalnoise->SetName("gglobalnoise");
	TGraph *gglobalnoise_cm2	= new TGraph();
			gglobalnoise_cm2->SetMarkerStyle(20);
			gglobalnoise_cm2->SetMarkerColor(1);
			gglobalnoise_cm2->SetLineColor(1);
			gglobalnoise_cm2->SetName("gglobalnoise_cm2");
	TGraph *gglobalnoise_mtd9_cm2	= new TGraph();
			gglobalnoise_mtd9_cm2->SetMarkerStyle(20);
			gglobalnoise_mtd9_cm2->SetMarkerColor(1);
			gglobalnoise_mtd9_cm2->SetLineColor(1);
			gglobalnoise_mtd9_cm2->SetName("gglobalnoise_mtd9_cm2");
	TGraph *gglobalnoise_mtd11e_cm2	= new TGraph();
			gglobalnoise_mtd11e_cm2->SetMarkerStyle(21);
			gglobalnoise_mtd11e_cm2->SetMarkerColor(2);
			gglobalnoise_mtd11e_cm2->SetLineColor(2);
			gglobalnoise_mtd11e_cm2->SetName("gglobalnoise_mtd11_cm2");
	TGraph *gglobalnoise_mtd11c_cm2	= new TGraph();
			gglobalnoise_mtd11c_cm2->SetMarkerStyle(22);
			gglobalnoise_mtd11c_cm2->SetMarkerColor(4);
			gglobalnoise_mtd11c_cm2->SetLineColor(4);
			gglobalnoise_mtd11c_cm2->SetName("gglobalnoise_mtd11_cm2");
	TGraph *gglobalnoise_mtd11w_cm2	= new TGraph();
			gglobalnoise_mtd11w_cm2->SetMarkerStyle(23);
			gglobalnoise_mtd11w_cm2->SetMarkerColor(6);
			gglobalnoise_mtd11w_cm2->SetLineColor(6);
			gglobalnoise_mtd11w_cm2->SetName("gglobalnoise_mtd11_cm2");

	TGraph *gglobalnoisethresh_mtd9_cm2	= new TGraph();
			gglobalnoisethresh_mtd9_cm2->SetMarkerStyle(20);
			gglobalnoisethresh_mtd9_cm2->SetMarkerColor(1);
			gglobalnoisethresh_mtd9_cm2->SetLineColor(1);
			gglobalnoisethresh_mtd9_cm2->SetName("gglobalnoisethresh_mtd9_cm2");
	TGraph *gglobalnoisethresh_mtd11e_cm2	= new TGraph();
			gglobalnoisethresh_mtd11e_cm2->SetMarkerStyle(21);
			gglobalnoisethresh_mtd11e_cm2->SetMarkerColor(2);
			gglobalnoisethresh_mtd11e_cm2->SetLineColor(2);
			gglobalnoisethresh_mtd11e_cm2->SetName("gglobalnoisethresh_mtd11_cm2");
	TGraph *gglobalnoisethresh_mtd11c_cm2	= new TGraph();
			gglobalnoisethresh_mtd11c_cm2->SetMarkerStyle(22);
			gglobalnoisethresh_mtd11c_cm2->SetMarkerColor(4);
			gglobalnoisethresh_mtd11c_cm2->SetLineColor(4);
			gglobalnoisethresh_mtd11c_cm2->SetName("gglobalnoisethresh_mtd11_cm2");
	TGraph *gglobalnoisethresh_mtd11w_cm2	= new TGraph();
			gglobalnoisethresh_mtd11w_cm2->SetMarkerStyle(23);
			gglobalnoisethresh_mtd11w_cm2->SetMarkerColor(6);
			gglobalnoisethresh_mtd11w_cm2->SetLineColor(6);
			gglobalnoisethresh_mtd11w_cm2->SetName("gglobalnoisethresh_mtd11_cm2");

	TLatex *thistext	= new TLatex();
		thistext->SetTextSize(0.06);
		thistext->SetNDC();

	for (int irun=0;irun<nruns;irun++){
		int irn	= runnums[irun];
		sprintf(buf,"/star/institutions/rice/llope/noise/noise_121380%d.root",irn); 
		TFile *fin = new TFile(buf,"READ");
			fin->cd();
			hrate_tray[irun]	= (TH1D*)gDirectory->Get("hrate_tray");
				sprintf(buf,"hrate_tray%d",irun);
				hrate_tray[irun]->SetName(buf);
			hinfo[irun]	= (TH1D*)gDirectory->Get("hinfo");
				sprintf(buf,"hinfo%d",irun);
				hinfo[irun]->SetName(buf);
			htot[irun]	= (TH1D*)gDirectory->Get("htot");
				sprintf(buf,"htot%d",irun);
				sprintf(buf,"ToT TOF, Run=%d",runnums[irun]);
				if (htot[irun]){
					htot[irun]->SetName(buf);
					htot[irun]->SetTitle(buf);
				} else {
					htot[irun]	= new TH1D(buf,tbuf,160,0.0,40.0);
				}
			htot_mtd9[irun]	= (TH1D*)gDirectory->Get("htot_mtd9");
				sprintf(buf,"htot_mtd9%d",irun);
				sprintf(tbuf,"ToT MTD9, Thresh=%d",thresh[irun]);
				if (htot_mtd9[irun]){
					htot_mtd9[irun]->SetName(buf);
					htot_mtd9[irun]->SetTitle(tbuf);
					//thisntot_mtd9	= htot_mtd9[irun]->GetEntries();
				} else {
					htot_mtd9[irun]	= new TH1D(buf,tbuf,160,0.0,40.0);
				}
			htot_mtd11e[irun]	= (TH1D*)gDirectory->Get("htot_mtd11e");
				sprintf(buf,"htot_mtd11e%d",irun);
				sprintf(tbuf,"ToT MTD11e, Thresh=%d",thresh[irun]);
				if (htot_mtd11e[irun]){
					htot_mtd11e[irun]->SetName(buf);
					htot_mtd11e[irun]->SetTitle(tbuf);
					//thisntot_mtd11e	= htot_mtd11e[irun]->GetEntries();
				} else {
					htot_mtd11e[irun]	= new TH1D(buf,tbuf,160,0.0,40.0);
				}
			htot_mtd11c[irun]	= (TH1D*)gDirectory->Get("htot_mtd11c");
				sprintf(buf,"htot_mtd11c%d",irun);
				sprintf(tbuf,"ToT MTD11c, Thresh=%d",thresh[irun]);
				if (htot_mtd11c[irun]){
					htot_mtd11c[irun]->SetName(buf);
					htot_mtd11c[irun]->SetTitle(tbuf);
					//thisntot_mtd11c	= htot_mtd11c[irun]->GetEntries();
				} else {
					htot_mtd11c[irun]	= new TH1D(buf,tbuf,160,0.0,40.0);
				}
			htot_mtd11w[irun]	= (TH1D*)gDirectory->Get("htot_mtd11w");
				sprintf(buf,"htot_mtd11w%d",irun);
				sprintf(tbuf,"ToT MTD11w, Thresh=%d",thresh[irun]);
				if (htot_mtd11w[irun]){
					htot_mtd11w[irun]->SetName(buf);
					htot_mtd11w[irun]->SetTitle(tbuf);
					//thisntot_mtd11w	= htot_mtd11w[irun]->GetEntries();
				} else {
					htot_mtd11w[irun]	= new TH1D(buf,tbuf,160,0.0,40.0);
				}
		fin->Close();
		//
		float ntrays = 0.0;
		for (int ib=1;ib<=120;ib++){
			if (hrate_tray[irun]->GetBinContent(ib) > TRAYCUTOFF){ ++ntrays; }
		}
		float gnle	= hinfo[irun]->GetBinContent(3)/ntrays/192.;
		float gnte	= hinfo[irun]->GetBinContent(4)/ntrays/192.;
		float gn	= hinfo[irun]->GetBinContent(5)/ntrays/192.;
		float gnL	= hinfo[irun]->GetBinContent(6)/ntrays/192.;
		float gnH	= hinfo[irun]->GetBinContent(7)/ntrays/192.;
		float gnT	= hinfo[irun]->GetBinContent(8)/ntrays/192.;
		float gnmtd9	= hinfo[irun]->GetBinContent(12)/36.;
		float gnmtd11	= hinfo[irun]->GetBinContent(15)/72.;
		float gnmtd11e	= hinfo[irun]->GetBinContent(16)/24.;
		float gnmtd11c	= hinfo[irun]->GetBinContent(17)/24.;
		float gnmtd11w	= hinfo[irun]->GetBinContent(18)/24.;
		float loopslo	=0.;
		float loopint	=0.;
		float ameantot	=0.;
		float ameantot9 =0.;
		float ameantot11=0.;
		if (hinfo[irun]->GetNbinsX()>25){
			loopint		= hinfo[irun]->GetBinContent(21);
			loopslo		= hinfo[irun]->GetBinContent(22);
			ameantot	= hinfo[irun]->GetBinContent(23);
			ameantot9	= hinfo[irun]->GetBinContent(24);
			ameantot11	= hinfo[irun]->GetBinContent(25);
		}
 		gglobalnoise->SetPoint(irun,runnums[irun],gn);
 		gglobalnoise_cm2->SetPoint(irun,runnums[irun],gn/tofarea);
 		gglobalnoise_mtd9_cm2->SetPoint(irun,runnums[irun],gnmtd9/mtd9area);
 		gglobalnoise_mtd11e_cm2->SetPoint(irun,runnums[irun],gnmtd11e/mtd11area);
 		gglobalnoise_mtd11c_cm2->SetPoint(irun,runnums[irun],gnmtd11c/mtd11area);
 		gglobalnoise_mtd11w_cm2->SetPoint(irun,runnums[irun],gnmtd11w/mtd11area);
 		gglobalnoisethresh_mtd9_cm2->SetPoint(irun,thresh[irun],gnmtd9/mtd9area);
 		gglobalnoisethresh_mtd11e_cm2->SetPoint(irun,thresh[irun],gnmtd11e/mtd11area);
 		gglobalnoisethresh_mtd11c_cm2->SetPoint(irun,thresh[irun],gnmtd11c/mtd11area);
 		gglobalnoisethresh_mtd11w_cm2->SetPoint(irun,thresh[irun],gnmtd11w/mtd11area);

		cout<<irun<<"\t runnum="<<irn<<"\t"<<thresh[irun]<<"\t"	
			<<gn<<" "<<gnmtd11e<<" "<<gnmtd11c<<" "<<gnmtd11w<<" "<<gnmtd9<<endl;

// 		gmeantot->SetPoint(iday,gday[iday],ameantot);
// 		if (thisntot_mtd9>50){
// 			gmeantot_mtd9->SetPoint(iday,gday[iday],ameantot9);
// 		}
// 		if (thisntot_mtd11>50){
// 			gmeantot_mtd11->SetPoint(iday,gday[iday],ameantot11);
// 		}
		//
	}

	gStyle->SetOptStat(0);
	gStyle->SetPadRightMargin(0.02);
	gStyle->SetPadTopMargin(0.04);
	gStyle->SetPadBottomMargin(0.09);
	gStyle->SetPadLeftMargin(0.12);
	gStyle->SetTitleOffset(0.90,"X");
	gStyle->SetTitleOffset(0.90,"Y");

 	++ican; sprintf(buf,"ccan%d",ican);
 	ccan[ican] = new TCanvas(buf,buf,0,0,700,500);
 	ccan[ican]->SetFillColor(10);
 	ccan[ican]->cd(); ccan[ican]->Divide(3,2,0.0001,0.0001);
// 		frame	=	gPad->DrawFrame(-21,0,(year[ndays-1]-11)*365+day[ndays-1]+6,70);
// 			frame->GetXaxis()->SetTitle("run number");
// 			frame->GetYaxis()->SetTitle("global noise rate per channel (Hz)");
// 		for (int i=0;i<nmodes;i++){ modes[i]->Draw(); }
// 		gPad->RedrawAxis();
	ccan[ican]->cd(1);
		gPad->SetLogy(0);
		gglobalnoise_cm2->SetMinimum(1.0);
		gglobalnoise_cm2->SetMaximum(1.7);
		gglobalnoise_cm2->Draw("AP");
		thistext->DrawLatex(0.4,0.9,"TOF (Hz/cm^2/ch)");
	ccan[ican]->cd(3);
		gPad->SetLogy(1);
		gglobalnoise_mtd9_cm2->SetMinimum(0.01);
		gglobalnoise_mtd9_cm2->Draw("AP");
		thistext->DrawLatex(0.4,0.9,"MTD9 (Hz/cm^2/ch)");
	ccan[ican]->cd(4);
		gPad->SetLogy(1);
		gglobalnoise_mtd11e_cm2->SetMinimum(0.01);
		gglobalnoise_mtd11e_cm2->Draw("AP");
		thistext->DrawLatex(0.4,0.9,"MTD11E (Hz/cm^2/ch)");
	ccan[ican]->cd(5);
		gPad->SetLogy(1);
		gglobalnoise_mtd11c_cm2->SetMinimum(0.01);
		gglobalnoise_mtd11c_cm2->Draw("AP");
		thistext->DrawLatex(0.4,0.9,"MTD11C (Hz/cm^2/ch)");
	ccan[ican]->cd(6);
		gPad->SetLogy(1);
		gglobalnoise_mtd11w_cm2->SetMinimum(0.01);
		gglobalnoise_mtd11w_cm2->Draw("AP");
		thistext->DrawLatex(0.4,0.9,"MTD11W (Hz/cm^2/ch)");
	ccan[ican]->Update(); ccan[ican]->cd();
	ccan[ican]->Print("mtdthresh.ps(");

	TF1 *fexp_high[4];
	TF1 *fexp_midd[4];
	TF1 *fexp_highw[4];
	TF1 *fexp_middw[4];
	for (int i=0;i<4;i++){
		sprintf(buf,"fexp_high%d",i);
		fexp_high[i]	= new TF1(buf,power,1400.,3200.,2);
			fexp_high[i]->SetParameter(0,1);
			fexp_high[i]->SetParameter(1,-0.005);
			fexp_high[i]->SetLineWidth(1);
		sprintf(buf,"fexp_highw%d",i);
		fexp_highw[i]	= new TF1(buf,power,100.,3200.,2);
		sprintf(buf,"fexp_midd%d",i);
		fexp_midd[i]	= new TF1(buf,power,490.,1010.,2);
			fexp_midd[i]->SetParameter(0,100);
			fexp_midd[i]->SetParameter(1,-0.01);
			fexp_midd[i]->SetLineWidth(1);
		sprintf(buf,"fexp_middw%d",i);
		fexp_middw[i]	= new TF1(buf,power,100.,3200.,2);
	}

 	++ican; sprintf(buf,"ccan%d",ican);
 	ccan[ican] = new TCanvas(buf,buf,0,0,700,500);
 	ccan[ican]->SetFillColor(10);
 	ccan[ican]->cd(); ccan[ican]->Divide(2,2,0.0001,0.0001);
// 		frame	=	gPad->DrawFrame(-21,0,(year[ndays-1]-11)*365+day[ndays-1]+6,70);
// 			frame->GetXaxis()->SetTitle("run number");
// 			frame->GetYaxis()->SetTitle("global noise rate per channel (Hz)");
// 		for (int i=0;i<nmodes;i++){ modes[i]->Draw(); }
// 		gPad->RedrawAxis();
	ccan[ican]->cd(1);
		gPad->SetLogy(1);
		gglobalnoisethresh_mtd9_cm2->SetMinimum(0.01);
		gglobalnoisethresh_mtd9_cm2->Draw("AP");
		gglobalnoisethresh_mtd9_cm2->Fit("fexp_high0","Q0NR");
		gglobalnoisethresh_mtd9_cm2->Fit("fexp_midd0","Q0NR");		
		fexp_high[0]->SetRange(200,3200);
		fexp_high[0]->Draw("same");
		fexp_midd[0]->SetRange(200,3200);
		fexp_midd[0]->Draw("same");
		thistext->DrawLatex(0.4,0.9,"MTD9 (Hz/cm^2/ch)");
	ccan[ican]->cd(2);
		gPad->SetLogy(1);
		gglobalnoisethresh_mtd11e_cm2->SetMinimum(0.01);
		gglobalnoisethresh_mtd11e_cm2->Draw("AP");
		gglobalnoisethresh_mtd11e_cm2->Fit("fexp_high1","Q0NR");
		gglobalnoisethresh_mtd11e_cm2->Fit("fexp_midd1","Q0NR");		
		fexp_high[1]->SetRange(200,3200);
		fexp_high[1]->Draw("same");
		fexp_midd[1]->SetRange(200,3200);
		fexp_midd[1]->Draw("same");
		thistext->DrawLatex(0.4,0.9,"MTD11E (Hz/cm^2/ch)");
	ccan[ican]->cd(3);
		gPad->SetLogy(1);
		gglobalnoisethresh_mtd11c_cm2->SetMinimum(0.01);
		gglobalnoisethresh_mtd11c_cm2->SetMaximum(0.5);
		gglobalnoisethresh_mtd11c_cm2->Draw("AP");
		gglobalnoisethresh_mtd11c_cm2->Fit("fexp_high2","Q0NR");
		gglobalnoisethresh_mtd11c_cm2->Fit("fexp_midd2","Q0NR");		
		fexp_high[2]->SetRange(200,3200);
		fexp_high[2]->Draw("same");
		fexp_midd[2]->SetRange(200,3200);
		fexp_midd[2]->Draw("same");
		thistext->DrawLatex(0.4,0.9,"MTD11C (Hz/cm^2/ch)");
	ccan[ican]->cd(4);
		gPad->SetLogy(1);
		gglobalnoisethresh_mtd11w_cm2->SetMinimum(0.01);
		gglobalnoisethresh_mtd11w_cm2->SetMaximum(0.5);
		gglobalnoisethresh_mtd11w_cm2->Draw("AP");
		gglobalnoisethresh_mtd11w_cm2->Fit("fexp_high3","Q0NR");
		gglobalnoisethresh_mtd11w_cm2->Fit("fexp_midd3","Q0NR");		
		fexp_high[3]->SetRange(200,3200);
		fexp_high[3]->Draw("same");
		fexp_midd[3]->SetRange(200,3200);
		fexp_midd[3]->Draw("same");
		thistext->DrawLatex(0.4,0.9,"MTD11W (Hz/cm^2/ch)");
	ccan[ican]->Update(); ccan[ican]->cd();
	ccan[ican]->Print("mtdthresh.ps");


 	++ican; sprintf(buf,"ccan%d",ican);
 	ccan[ican] = new TCanvas(buf,buf,0,0,700,500);
 	ccan[ican]->SetFillColor(10);
 	ccan[ican]->cd(); ccan[ican]->Divide(5,4,0.0001,0.0001);
	for (int ir=0;ir<nruns;ir++){
		ccan[ican]->cd(1+ir);
		htot_mtd9[ir]->SetFillStyle(1001);
		htot_mtd9[ir]->SetFillColor(7);
		htot_mtd9[ir]->Draw();
	}
	ccan[ican]->Update(); ccan[ican]->cd();
	ccan[ican]->Print("mtdthresh.ps");

 	++ican; sprintf(buf,"ccan%d",ican);
 	ccan[ican] = new TCanvas(buf,buf,0,0,700,500);
 	ccan[ican]->SetFillColor(10);
 	ccan[ican]->cd(); ccan[ican]->Divide(5,4,0.0001,0.0001);
	for (int ir=0;ir<nruns;ir++){
		ccan[ican]->cd(1+ir);
		htot_mtd11e[ir]->SetFillStyle(1001);
		htot_mtd11e[ir]->SetFillColor(3);
		htot_mtd11e[ir]->Draw();
	}
	ccan[ican]->Update(); ccan[ican]->cd();
	ccan[ican]->Print("mtdthresh.ps");

 	++ican; sprintf(buf,"ccan%d",ican);
 	ccan[ican] = new TCanvas(buf,buf,0,0,700,500);
 	ccan[ican]->SetFillColor(10);
 	ccan[ican]->cd(); ccan[ican]->Divide(5,4,0.0001,0.0001);
	for (int ir=0;ir<nruns;ir++){
		ccan[ican]->cd(1+ir);
		htot_mtd11c[ir]->SetFillStyle(1001);
		htot_mtd11c[ir]->SetFillColor(4);
		htot_mtd11c[ir]->Draw();
	}
	ccan[ican]->Update(); ccan[ican]->cd();
	ccan[ican]->Print("mtdthresh.ps");


 	++ican; sprintf(buf,"ccan%d",ican);
 	ccan[ican] = new TCanvas(buf,buf,0,0,700,500);
 	ccan[ican]->SetFillColor(10);
 	ccan[ican]->cd(); ccan[ican]->Divide(5,4,0.0001,0.0001);
	for (int ir=0;ir<nruns;ir++){
		ccan[ican]->cd(1+ir);
		htot_mtd11w[ir]->SetFillStyle(1001);
		htot_mtd11w[ir]->SetFillColor(6);
		htot_mtd11w[ir]->Draw();
	}
	ccan[ican]->Update(); ccan[ican]->cd();
	ccan[ican]->Print("mtdthresh.ps");

// 		TLegend *lega = new TLegend(0.47,0.73,0.735,0.975);
// 			lega->SetTextSize(0.03);
// 			lega->SetFillStyle(0);
// 			lega->AddEntry(gglobalnoise ,"TOF, Total","P");
// 			lega->AddEntry(gglobalnoiseH,"TOF, ToT>18ns","P");
// 			lega->AddEntry(gglobalnoiseL,"TOF, ToT<18ns","P");
// 			lega->AddEntry(hdum1,"Recirc Mode, 450ccm","F");
// 			lega->AddEntry(hdum2,"Recirc Mode, 800ccm","F");
// 			lega->AddEntry(hdum3,"Purge Mode","F");
// 			lega->AddEntry(hdum4,"N2 Mode","F");
// 			lega->Draw();


	//
	ccan[ican]->Print("mtdthresh.ps]");
	sprintf(buf,"/usr/bin/ps2pdf %s.ps %s.pdf","mtdthresh","mtdthresh");
	cout<<"..... Executing ... "<<buf<<endl;
	gSystem->Exec(buf);
	sprintf(buf,"/bin/cp %s.pdf /star/u/llope/WWW/files/","mtdthresh");
	cout<<"..... Executing ... "<<buf<<endl;
	gSystem->Exec(buf);
	cout<<"..... Done...."<<endl;	
	
}

