#define mtdview_cxx
#include "mtdview.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TError.h>

float linemax = 100.;

//---- run11 pp500
//float dtoffsets[12] = {	-0.231934,1.06979,-1.11621,-0.520996,1.16933,-0.693493,
//							-0.158425,0.803373,-0.951308,0.173516,0.514547,0};
//---- run11 auau200
float dtoffsets[12] = {-0.48669,1.14094,-1.11119,-0.333116,1.16939,-0.674893,
						-0.104004,0.837646,-0.844355,-0.0129825,0.425754,0};

void mtdview::Loop(){

	char 		buf[100];
	char 		tbuf[100];
	int			ican = -1;
	TCanvas*	ccan[100];
	
	gErrorIgnoreLevel	= 2000;
	
	const char* modnames[4] = {"MTD26E","MTD26C","MTD26W","MTD01"};
	
	char type[30];
	sprintf(type,"run11auau200");
	
	char psfileopen[100];
	char psfile[100];
	char pdffile[100];
	char psfileclose[100];
	sprintf(psfileopen,"mtdview_%s.ps(",type);
	sprintf(psfile,"mtdview_%s.ps",type);
	sprintf(pdffile,"mtdview_%s.pdf",type);
	sprintf(psfileclose,"mtdview_%s.ps]",type);
	cout<<type<<endl;
	cout<<psfileopen<<endl;
	cout<<psfile<<endl;
	cout<<psfileclose<<endl;
	cout<<pdffile<<endl;

//	TFile *fd	= new TFile("daqdoer_12076029.root","READ");
	TFile *fd	= new TFile("/star/institutions/rice/llope/dd/daqdoer_12127004_mtd.root","READ");
	TH1D *hcoco_vpde		= (TH1D*)gDirectory->Get("hcoco_vpde");
	TH1D *hcoco_vpdw		= (TH1D*)gDirectory->Get("hcoco_vpdw");
	TH1D *hcocokeep_vpde	= (TH1D*)gDirectory->Get("hcocokeep_vpde");
	TH1D *hcocokeep_vpdw	= (TH1D*)gDirectory->Get("hcocokeep_vpdw");
	TH1D *hcoco_mtd1		= (TH1D*)gDirectory->Get("hcoco_mtd1");
	TH1D *hcoco_mtd26		= (TH1D*)gDirectory->Get("hcoco_mtd26");
	TH1D *hcocokeep_mtd1	= (TH1D*)gDirectory->Get("hcocokeep_mtd1");
	TH1D *hcocokeep_mtd26	= (TH1D*)gDirectory->Get("hcocokeep_mtd26");
	hcocokeep_vpde->SetFillStyle(1001); hcocokeep_vpde->SetFillColor(3);
	hcocokeep_vpdw->SetFillStyle(1001); hcocokeep_vpdw->SetFillColor(3);
	hcocokeep_mtd1->SetFillStyle(1001); hcocokeep_mtd1->SetFillColor(3);
	hcocokeep_mtd26->SetFillStyle(1001); hcocokeep_mtd26->SetFillColor(3);
	hcoco_vpde->SetTitle("Trigger Time (25ns), VPD-E");
	hcoco_vpdw->SetTitle("Trigger Time (25ns), VPD-W");
	hcoco_mtd1->SetTitle("Trigger Time (25ns), MTD01");
	hcoco_mtd26->SetTitle("Trigger Time (25ns), MTD26");

	TH1D *hp[4];
	TH1D *htot[4];
	TH1D *hpsl[4];
	TH1D *hpsr[4];
	TH1D *hpphibin[4];
	for (int i=0;i<4;i++){
		sprintf(buf,"hp%d",i);
		sprintf(tbuf,"HitPattern by Chan, %s",modnames[i]);
		hp[i]	= new TH1D(buf,tbuf,36,-0.5,35.5);
		sprintf(buf,"hpsl%d",i);
		sprintf(tbuf,"HitPattern by Strip, %s",modnames[i]);
		hpsl[i]	= new TH1D(buf,tbuf,18,-0.5,17.5);
		sprintf(buf,"hpsr%d",i);
		sprintf(tbuf,"HitPattern by Strip, %s",modnames[i]);
		hpsr[i]	= new TH1D(buf,tbuf,18,-0.5,17.5);
		sprintf(buf,"hpphibin%d",i);
		sprintf(tbuf,"HitPattern by #Phi bin, %s",modnames[i]);
		hpphibin[i]	= new TH1D(buf,tbuf,18,-0.5,17.5);
		sprintf(buf,"htot%d",i);
		sprintf(tbuf,"ToT, %s",modnames[i]);
		htot[i]	= new TH1D(buf,tbuf,59,0.,59.);
	}
	
	TH2D *hdtcorr	= new TH2D("hdtcorr","Neighboring Strips (t_{L}-t_{R}) Correlation",50,-10.0,10.0,50,-10.0,10.0);
	TH1D *hdtcorr1D	= new TH1D("hdtcorr1D","Neighboring Strips #Delta(t_{L}-t_{R})",80,-10.0,10.0);
	TH2D *hdtcorr1Dstrip[4];
	TProfile *hdtcorr1DstripPr[4];
	for (int i=0;i<4;i++){
		sprintf(buf,"hdtcorr1Dstrip%d",i);
		sprintf(tbuf,"#Delta(t_{L}-t_{R}) vs strip, %s",modnames[i]);
		hdtcorr1Dstrip[i] = new TH2D(buf,tbuf,18,-0.5,17.5,80,-10.0,10.0);
		sprintf(buf,"hdtcorr1DstripPr%d",i);
		sprintf(tbuf,"#LT#Delta(t_{L}-t_{R})#GT vs strip, %s",modnames[i]);
		hdtcorr1DstripPr[i] = new TProfile(buf,tbuf,18,-0.5,17.5,-2.0,2.0);
			hdtcorr1DstripPr[i]->SetMarkerStyle(20);
	}
	TProfile *hdtcorr1DstripPr_mtd26;
		sprintf(buf,"hdtcorr1DstripPr_mtd26",i);
		sprintf(tbuf,"#LT#Delta(t_{L}-t_{R})#GT vs strip, MTD26");
		hdtcorr1DstripPr_mtd26 = new TProfile(buf,tbuf,12,-0.5,11.5,-2.0,2.0);
			hdtcorr1DstripPr_mtd26->SetMarkerStyle(20);
			hdtcorr1DstripPr_mtd26->SetMarkerColor(3);
	TH2D *hdtcorroff_mtd26	= new TH2D("hdtcorroff_mtd26","Neighboring Strips (t_{L}-t_{R}) Correlation, w/ Offsets",50,-10.0,10.0,50,-10.0,10.0);
	TH1D *hdtcorr1Doff_mtd26 = new TH1D("hdtcorr1Doff_mtd26","Neighboring Strips #Delta(t_{L}-t_{R}), w/ Offsets",80,-10.0,10.0);
	//
	TH2D *htotldt_mtd26 = new TH2D("htotldt_mtd26","ToT vs (t_{L}-t_{R}), MTD26",48,-6.,6.,57,2.,59.);

	int nevtseen = 0;
	float 	time[18][2][4];
	float 	totlr[18][2][4];

	if (fChain == 0) return;
	Long64_t nentries = fChain->GetEntries();
	cout<<" nentries = "<<nentries<<endl;
	Long64_t nb=0;
	for (Long64_t jentry=0; jentry<nentries;jentry++) {
		Long64_t ientry = LoadTree(jentry);
		if (ientry<0) break;
		nb = fChain->GetEntry(jentry);
		if (jentry%10000==0){ cout<<"Processing "<<jentry<<endl; }

		++nevtseen;
		for (int im=0;im<4;im++){					// module
			for (int is=0;is<18;is++){				// strip
				for (int ilr=0;ilr<2;ilr++){		// strip end
					time[is][ilr][im]	= 0.0;
					totlr[is][ilr][im]	= 0.0;
				}
			}
		}
		
		int nh	= nMtdHits;
		for (int ih=0;ih<nh;ih++){
			int itray	= tray[ih];
			int imodule	= module[ih];		// MTD26E, C, W, MTD9
			int imodch	= modch[ih] - 1;
			int istrip	= strip[ih] - 1;
			int istrlr	= strlr[ih];
			int iphi	= istrip;
			if (imodule==2){ iphi = 11-istrip; }	// phi flip MTD26W....
			if (imodule==3){ 						// module-phi flip MTD01...
				int imrpc		= istrip/6;
				int istripmrpc	= istrip - 6*imrpc;
				int iphimrpc	= 5 - istripmrpc;
				iphi			= 6*imrpc + iphimrpc;
			}
			//
			//cout<<ih<<" "<<istrip<<" "<<istrlr<<" "<<imodule<<" "<<tle[ih]<<endl;
			time[istrip][istrlr][imodule]	= tle[ih];
			totlr[istrip][istrlr][imodule]	= tot[ih];
			//
			hp[imodule]->Fill(imodch);
			if (istrlr==0){
				hpsl[imodule]->Fill(istrip);
				hpphibin[imodule]->Fill(iphi,0.5);
			} else if (istrlr==1){
				hpsr[imodule]->Fill(istrip);
				hpphibin[imodule]->Fill(iphi,0.5);
			}
			htot[imodule]->Fill(tot[ih]);
			//
		}
		//---- end hit loop...

		float 	dt[18];
		float 	totl[18];
		float 	totr[18];
		float 	at[18];
		int 	nhmod[4];
		int 	nstrn;
		int 	istrid[2];
		float	dtpair[2];
		float	atpair[2];
		for (int im=0;im<4;im++){				// module
			nhmod[im]		= 0;
			nstrn			= 0;
			for (int is=0;is<18;is++){			// strip		
					dt[is]		= 0.0;
					at[is]		= 0.0;
					totl[is]	= 0.0;
					totr[is]	= 0.0;
			}
			for (int is=0;is<2;is++){			// strip		
					dtpair[is]	= 0.0;
					atpair[is]	= 0.0;
			}
			for (int is=0;is<18;is++){			// strip
				if (time[is][0][im]>0. && time[is][1][im]>0.){
					at[is]	= (time[is][0][im] + time[is][1][im])/2.;
					dt[is]	= (time[is][0][im] - time[is][1][im]);
					totl[is]= totlr[is][0][im];
					totr[is]= totlr[is][1][im];
					nhmod[im]++;
				}
			}
			if (nhmod[im]>0 && im<=2){
				for (int is=0;is<12;is++){		// strip
					htotldt_mtd26->Fill(dt[is],totl[is]);
				}
			}			
			if (nhmod[im]==2){
				//cout<<im<<" nhmod="<<nhmod[im]<<"\t";
				for (int is=0;is<18;is++){		// strip
					//cout<<dt[is]<<" ";
					if (at[is]>0.&&nstrn<2){
						istrid[nstrn]	= is;
						dtpair[nstrn]	= dt[is];
						atpair[nstrn]	= at[is];
						nstrn++;
						//cout<<is<<" "<<time[is][0][im]<<" "<<time[is][1][im]<<" ][ ";
						//cout<<im<<" "<<is<<" "<<dt[is]<<" "<<at[is]<<endl;
					}
				}	//cout<<endl;
				if ((istrid[1]-istrid[0])==1){	// require the two strips are neighbors...
					//cout<<"RES... "<<atpair[0]<<" "<<atpair[1]<<"\t"<<dtpair[0]<<" "<<dtpair[1]<<endl;
					hdtcorr->Fill(dtpair[0],dtpair[1]);	
					hdtcorr1D->Fill(dtpair[0]-dtpair[1]);	
					hdtcorr1Dstrip[im]->Fill(istrid[0],dtpair[0]-dtpair[1]);	
					hdtcorr1DstripPr[im]->Fill(istrid[0],dtpair[0]-dtpair[1]);	
					if (im<=2){
						hdtcorr1DstripPr_mtd26->Fill(istrid[0],dtpair[0]-dtpair[1]);	
						float offset	= dtoffsets[istrid[0]];
						hdtcorroff_mtd26->Fill(dtpair[0],dtpair[1]+offset);	
						hdtcorr1Doff_mtd26->Fill(dtpair[0]-dtpair[1]-offset);	
					}
				}
			}
		}	
		
	}
	cout<<"nevtseen = "<<nevtseen<<endl;
	//---- end event loop...

	//---- paint plots
	//
	gStyle->SetPaperSize(TStyle::kUSLetter);
	gStyle->SetOptStat(11);
	gStyle->SetTitleX(0.17);
	gStyle->SetPadBottomMargin(0.10);
	gStyle->SetPadLeftMargin(0.10);
	gStyle->SetPadTopMargin(0.05);
	gStyle->SetPadRightMargin(0.01);
	gStyle->SetLabelSize(0.045,"X");
	gStyle->SetLabelSize(0.045,"Y");
	gStyle->SetTitleXSize(0.045);
	gStyle->SetTitleYSize(0.045);
	//
	TLatex *thistext	= new TLatex();
		thistext->SetTextSize(0.06);
		thistext->SetNDC();
	TLatex *thistextb	= new TLatex();
		thistextb->SetTextSize(0.04);
		thistextb->SetNDC();
	TLine *thisline		= new TLine();
		thisline->SetLineColor(5);
		thisline->SetLineWidth(2);
	TF1 *fgaus			= new TF1("fgaus","gaus",-2.5,2.5);
	double gpars[3];

 	++ican; sprintf(buf,"ccan%d",ican);
 	ccan[ican] = new TCanvas(buf,buf,0,0,750.,750.*(8.5/11.));
 	ccan[ican]->SetFillColor(10);
 	ccan[ican]->cd(); ccan[ican]->Divide(2,2,0.0001,0.0001);
	ccan[ican]->cd(1);
		hcoco_vpde->GetXaxis()->SetRangeUser(-0.5,200.5);
		hcoco_vpde->Draw();
		hcocokeep_vpde->Draw("same");
	ccan[ican]->cd(2);
		hcoco_vpdw->GetXaxis()->SetRangeUser(-0.5,200.5);
		hcoco_vpdw->Draw();
		hcocokeep_vpdw->Draw("same");
	ccan[ican]->cd(3);
		hcoco_mtd1->GetXaxis()->SetRangeUser(-0.5,200.5);
		hcoco_mtd1->Draw();
		hcocokeep_mtd1->Draw("same");
	ccan[ican]->cd(4);
		hcoco_mtd26->GetXaxis()->SetRangeUser(-0.5,200.5);
		hcoco_mtd26->Draw();
		hcocokeep_mtd26->Draw("same");
	ccan[ican]->cd(); ccan[ican]->Update();
	ccan[ican]->Print(psfileopen);

 	++ican; sprintf(buf,"ccan%d",ican);
 	ccan[ican] = new TCanvas(buf,buf,0,0,750.,750.*(8.5/11.));
 	ccan[ican]->SetFillColor(10);
 	ccan[ican]->cd(); ccan[ican]->Divide(2,2,0.0001,0.0001);
	for (int i=0;i<4;i++){
		ccan[ican]->cd(1+i);
		hp[i]->SetMinimum(0);		
		hp[i]->SetFillStyle(1001);		
		hp[i]->SetFillColor(5);		
		hp[i]->Draw();
	}
	ccan[ican]->cd(); ccan[ican]->Update();
	ccan[ican]->Print(psfile);
	//
 	++ican; sprintf(buf,"ccan%d",ican);
 	ccan[ican] = new TCanvas(buf,buf,0,0,750.,750.*(8.5/11.));
 	ccan[ican]->SetFillColor(10);
 	ccan[ican]->cd(); ccan[ican]->Divide(2,2,0.0001,0.0001);
	for (int i=0;i<4;i++){
		ccan[ican]->cd(1+i);
		hpsl[i]->SetMinimum(0);		
		hpsl[i]->SetLineColor(4);		
		hpsl[i]->SetLineWidth(2);		
		hpsl[i]->Draw();
		hpsr[i]->SetLineColor(6);		
		hpsr[i]->SetLineWidth(2);		
		hpsr[i]->Draw("same");
		if (i==3){
			thisline->DrawLine( 5.5,00, 5.5,linemax);
			thisline->DrawLine(11.5,00,11.5,linemax);
		}
	}
	ccan[ican]->cd(); ccan[ican]->Update();
	ccan[ican]->Print(psfile);
	//
	gStyle->SetOptStat(0);
 	++ican; sprintf(buf,"ccan%d",ican);
 	ccan[ican] = new TCanvas(buf,buf,0,0,750.,750.*(8.5/11.));
 	ccan[ican]->SetFillColor(10);
 	ccan[ican]->cd(); ccan[ican]->Divide(1,1,0.0001,0.0001);
		ccan[ican]->cd(1);
		hpphibin[1]->GetXaxis()->SetRangeUser(-0.5,11.49);		
		hpphibin[1]->SetLineColor(2);		
		hpphibin[1]->SetLineWidth(2);		
		hpphibin[1]->SetMinimum(0);
		hpphibin[1]->Draw();
		hpphibin[0]->SetLineColor(1);		
		hpphibin[0]->SetLineWidth(2);		
		hpphibin[0]->Draw("same");
		hpphibin[2]->SetLineColor(3);		
		hpphibin[2]->SetLineWidth(2);		
		hpphibin[2]->Draw("same");
		thistext->DrawLatex(0.13,0.87,"MTD26W #Phi-Flipped");
		TLegend *leg	= new TLegend(0.55,0.81,0.70,0.93);
			leg->SetFillStyle(0);
			leg->AddEntry(hpphibin[0],"MTD26E","L");
			leg->AddEntry(hpphibin[1],"MTD26C","L");
			leg->AddEntry(hpphibin[2],"MTD26W","L");
			leg->Draw();
	ccan[ican]->cd(); ccan[ican]->Update();
	ccan[ican]->Print(psfile);
	gStyle->SetOptStat(11);

	gStyle->SetOptStat(0);
 	++ican; sprintf(buf,"ccan%d",ican);
 	ccan[ican] = new TCanvas(buf,buf,0,0,750.,750.*(8.5/11.));
 	ccan[ican]->SetFillColor(10);
 	ccan[ican]->cd(); ccan[ican]->Divide(1,1,0.0001,0.0001);
		ccan[ican]->cd(1);
		hpphibin[3]->GetXaxis()->SetRangeUser(-0.5,17.49);		
		hpphibin[3]->SetLineColor(6);		
		hpphibin[3]->SetLineWidth(2);		
		hpphibin[3]->SetMinimum(0);
		hpphibin[3]->Draw();
		thistext->DrawLatex(0.13,0.87,"MTD01 MRPC#Phi-Flipped");
		TLegend *leg	= new TLegend(0.65,0.89,0.80,0.93);
			leg->SetFillStyle(0);
			leg->AddEntry(hpphibin[3],"MTD01","L");
			leg->Draw();
	ccan[ican]->cd(); ccan[ican]->Update();
	ccan[ican]->Print(psfile);
	gStyle->SetOptStat(11);

	if (nevtseen>0){
		for (int i=0;i<4;i++){
			hp[i]->Scale(1./nevtseen);		
			hpsl[i]->Scale(1./nevtseen);		
			hpsr[i]->Scale(1./nevtseen);
			hpphibin[i]->Scale(1./nevtseen);		
		}
	}
	//
//  	++ican; sprintf(buf,"ccan%d",ican);
//  	ccan[ican] = new TCanvas(buf,buf,0,0,750.,750.*(8.5/11.));
//  	ccan[ican]->SetFillColor(10);
//  	ccan[ican]->cd(); ccan[ican]->Divide(2,2,0.0001,0.0001);
// 	for (int i=0;i<4;i++){
// 		ccan[ican]->cd(1+i);
// 		hp[i]->SetMinimum(0);		
// 		hp[i]->SetFillStyle(1001);		
// 		hp[i]->SetFillColor(7);		
// 		hp[i]->Draw();
// 		thistext->DrawLatex(0.13,0.87,"Normalized");
// 	}
// 	ccan[ican]->cd(); ccan[ican]->Update();
// 	ccan[ican]->Print("mtdview.ps");
// 	//
//  	++ican; sprintf(buf,"ccan%d",ican);
//  	ccan[ican] = new TCanvas(buf,buf,0,0,750.,750.*(8.5/11.));
//  	ccan[ican]->SetFillColor(10);
//  	ccan[ican]->cd(); ccan[ican]->Divide(2,2,0.0001,0.0001);
// 	for (int i=0;i<4;i++){
// 		ccan[ican]->cd(1+i);
// 		hpsl[i]->SetMinimum(0);		
// 		hpsl[i]->SetLineColor(4);		
// 		hpsl[i]->Draw();
// 		hpsr[i]->SetLineColor(6);		
// 		hpsr[i]->Draw("same");
// 		thistext->DrawLatex(0.13,0.87,"Normalized");
// 		if (i==3){
// 			thisline->DrawLine( 5.5,0, 5.5,linemax/(float)nevtseen);
// 			thisline->DrawLine(11.5,0,11.5,linemax/(float)nevtseen);
// 		}
// 	}
// 	ccan[ican]->cd(); ccan[ican]->Update();
// 	ccan[ican]->Print("mtdview.ps");
	//
	gStyle->SetOptStat(0);
 	++ican; sprintf(buf,"ccan%d",ican);
 	ccan[ican] = new TCanvas(buf,buf,0,0,750.,750.*(8.5/11.));
 	ccan[ican]->SetFillColor(10);
 	ccan[ican]->cd(); ccan[ican]->Divide(1,1,0.0001,0.0001);
		ccan[ican]->cd(1);
		hpphibin[1]->GetXaxis()->SetRangeUser(-0.5,11.49);		
		hpphibin[1]->SetLineColor(2);		
		hpphibin[1]->SetLineWidth(2);		
		hpphibin[1]->SetMinimum(0);
		hpphibin[1]->Draw();
		hpphibin[0]->SetLineColor(1);		
		hpphibin[0]->SetLineWidth(2);		
		hpphibin[0]->Draw("same");
		hpphibin[2]->SetLineColor(3);		
		hpphibin[2]->SetLineWidth(2);		
		hpphibin[2]->Draw("same");
		thistext->DrawLatex(0.13,0.87,"MTD26W #Phi-Flipped");
		thistextb->DrawLatex(0.35,0.82,"Normalized");
		TLegend *leg	= new TLegend(0.55,0.81,0.70,0.93);
			leg->SetFillStyle(0);
			leg->AddEntry(hpphibin[0],"MTD26E","L");
			leg->AddEntry(hpphibin[1],"MTD26C","L");
			leg->AddEntry(hpphibin[2],"MTD26W","L");
			leg->Draw();
	ccan[ican]->cd(); ccan[ican]->Update();
	ccan[ican]->Print(psfile);
	gStyle->SetOptStat(11);

	gStyle->SetOptStat(0);
 	++ican; sprintf(buf,"ccan%d",ican);
 	ccan[ican] = new TCanvas(buf,buf,0,0,750.,750.*(8.5/11.));
 	ccan[ican]->SetFillColor(10);
 	ccan[ican]->cd(); ccan[ican]->Divide(1,1,0.0001,0.0001);
		ccan[ican]->cd(1);
		hpphibin[3]->GetXaxis()->SetRangeUser(-0.5,17.49);		
		hpphibin[3]->SetLineColor(6);		
		hpphibin[3]->SetLineWidth(2);		
		hpphibin[3]->SetMinimum(0);
		hpphibin[3]->Draw();
		thistext->DrawLatex(0.13,0.87,"MTD01 MRPC#Phi-Flipped");
		thistextb->DrawLatex(0.35,0.82,"Normalized");
		TLegend *leg	= new TLegend(0.65,0.89,0.80,0.93);
			leg->SetFillStyle(0);
			leg->AddEntry(hpphibin[3],"MTD01","L");
			leg->Draw();
	ccan[ican]->cd(); ccan[ican]->Update();
	ccan[ican]->Print(psfile);
	gStyle->SetOptStat(11);


 	++ican; sprintf(buf,"ccan%d",ican);
 	ccan[ican] = new TCanvas(buf,buf,0,0,750.,750.*(8.5/11.));
 	ccan[ican]->SetFillColor(10);
 	ccan[ican]->cd(); ccan[ican]->Divide(2,2,0.0001,0.0001);
	for (int i=0;i<4;i++){
		ccan[ican]->cd(1+i);
		htot[i]->SetFillStyle(1001);		
		htot[i]->SetFillColor(5);		
		htot[i]->Draw();
	}
	ccan[ican]->cd(); ccan[ican]->Update();
	ccan[ican]->Print(psfile);
	//
 	++ican; sprintf(buf,"ccan%d",ican);
 	ccan[ican] = new TCanvas(buf,buf,0,0,750.,750.*(8.5/11.));
 	ccan[ican]->SetFillColor(10);
 	ccan[ican]->cd(); ccan[ican]->Divide(1,1,0.0001,0.0001);
	ccan[ican]->cd(1);
		htotldt_mtd26->Draw("col");
	ccan[ican]->cd(); ccan[ican]->Update();
	ccan[ican]->Print(psfile);

 	++ican; sprintf(buf,"ccan%d",ican);
 	ccan[ican] = new TCanvas(buf,buf,0,0,750.,750.*(8.5/11.));
 	ccan[ican]->SetFillColor(10);
 	ccan[ican]->cd(); ccan[ican]->Divide(2,1,0.0001,0.0001);
		ccan[ican]->cd(1);
			hdtcorr->Draw("col");
		ccan[ican]->cd(2);
			hdtcorr1D->Fit("fgaus","Q0NR");
			hdtcorr1D->GetXaxis()->SetRangeUser(-4.,4.);
			hdtcorr1D->SetLineColor(4);
			hdtcorr1D->Draw();
			fgaus->Draw("same");
			fgaus->GetParameters(gpars);
			sprintf(buf,"#sigma = %3.0f ps",1000.*gpars[2]);
			thistext->DrawLatex(0.14,0.87,buf);
			sprintf(tbuf,"#sigma_{0} = %3.0f ps",1000.*gpars[2]/2.);
			thistext->DrawLatex(0.14,0.82,tbuf);			
	ccan[ican]->cd(); ccan[ican]->Update();
	ccan[ican]->Print(psfile);
	//
 	++ican; sprintf(buf,"ccan%d",ican);
 	ccan[ican] = new TCanvas(buf,buf,0,0,750.,750.*(8.5/11.));
 	ccan[ican]->SetFillColor(10);
 	ccan[ican]->cd(); ccan[ican]->Divide(2,2,0.0001,0.0001);
	for (int i=0;i<4;i++){
		ccan[ican]->cd(1+i);
		hdtcorr1Dstrip[i]->Draw("col");
		hdtcorr1DstripPr[i]->Draw("same");
		if (i<=2){
			hdtcorr1DstripPr_mtd26->Draw("same");
		}
	}
	ccan[ican]->cd(); ccan[ican]->Update();
	ccan[ican]->Print(psfile);
	//
//  	++ican; sprintf(buf,"ccan%d",ican);
//  	ccan[ican] = new TCanvas(buf,buf,0,0,750.,750.*(8.5/11.));
//  	ccan[ican]->SetFillColor(10);
//  	ccan[ican]->cd(); ccan[ican]->Divide(2,2,0.0001,0.0001);
// 	for (int i=0;i<4;i++){
// 		ccan[ican]->cd(1+i);
// 		hdtcorr1Dstrip[i]->Draw("lego");
// 	}
// 	ccan[ican]->cd(); ccan[ican]->Update();
// 	ccan[ican]->Print("mtdview.ps");
	//
 	++ican; sprintf(buf,"ccan%d",ican);
 	ccan[ican] = new TCanvas(buf,buf,0,0,750.,750.*(8.5/11.));
 	ccan[ican]->SetFillColor(10);
 	ccan[ican]->cd(); ccan[ican]->Divide(2,1,0.0001,0.0001);
		ccan[ican]->cd(1);
			hdtcorroff_mtd26->Draw("col");
		ccan[ican]->cd(2);
			hdtcorr1Doff_mtd26->Fit("fgaus","Q0NR");
			hdtcorr1Doff_mtd26->GetXaxis()->SetRangeUser(-4.,4.);
			hdtcorr1Doff_mtd26->Draw();
			fgaus->Draw("same");
			//hdtcorr1D->Draw("same");
			hdtcorr1Doff_mtd26->Draw("same");
			fgaus->GetParameters(gpars);
			sprintf(buf,"#sigma = %3.0f ps",1000.*gpars[2]);
			thistext->DrawLatex(0.14,0.87,buf);
			sprintf(tbuf,"#sigma_{0} = %3.0f ps",1000.*gpars[2]/2.);
			thistext->DrawLatex(0.14,0.82,tbuf);			
	ccan[ican]->cd(); ccan[ican]->Update();
	ccan[ican]->Print(psfile);

	//---- end
	ccan[ican]->Print(psfileclose);
	//
	int nbx	= hdtcorr1DstripPr_mtd26->GetNbinsX();
	cout<<endl;
	cout<<"float dtoffsets[12] = {";
	for (int ib=1;ib<=nbx;ib++){
		cout<<hdtcorr1DstripPr_mtd26->GetBinContent(ib);
		if (ib!=nbx) { cout<<","; }
		if (ib==6)   { cout<<endl; cout<<"\t\t\t\t\t\t"; }
		if (ib==nbx) { cout<<"};"<<endl; }
	}	
	cout<<endl;
	//
	sprintf(buf,"ps2pdf %s %s",psfile,pdffile);
	gSystem->Exec(buf);
	sprintf(buf,"cp -v %s /star/u/llope/WWW/files/",pdffile);
	gSystem->Exec(buf);

}
