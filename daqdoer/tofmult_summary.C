#include <iostream>
#include <iomanip>
#include "TError.h"
#include "TMath.h"
using namespace std;

Int_t KeepTray(int itray);

void tofmult_summary() {

	gErrorIgnoreLevel	= 2000;
	char 		buf[100];
	char 		tbuf[100];
	int			ican=-1;
	int			ipad;
	TCanvas*	ccan[100];
	char 		type[20];

// 	const int	nphases		= 16;
// 	const char* rphases[nphases] 	= {"0","1","2","3","4","5","6","7","8","9",
// 									   "A","B","C","D","E","F"};
// 	const char* rstrings[nphases] = {
// 		"14070165_tof",		// 0
// 		"14070164_tof",		// 1
// 		"14070166_tof",		// 2
// 		"14070167_tof",		// 3
// 		"14070168_tof",		// 4
// 		"14070169_tof",		// 5
// 		"14070170_tof",		// 6
// 		"14070171_tof",		// 7
// 		"14070172_tof",		// 8
// 		"14070173_tof",		// 9
// 		"14070174_tof",		// a
// 		"14070175_tof",		// b
// 		"14070176_tof",		// c
// 		"14070177_tof",		// d
// 		"14070178_tof",		// e
// 		"14070179_tof"		// f
// 	};
	const int	nphases		= 8;
	const char* rphases[nphases] 	= {"0","1","3","7","B","D","E","F"};
	const char* rstrings[nphases] = {
		"15048041",		// 0
		"15048037",		// 1
		"15048039",		// 3
		"15048042",		// 7
		"15048038",		// B
		"15048036",		// D
		"15048040",		// E
		"15048035"		// F
	};



	TLatex *rnames[nphases];
	for (int iph=0;iph<nphases;iph++){
		sprintf(buf,"Phase=%s",rphases[iph]);
		rnames[iph]	= new TLatex(0.72,0.93,buf);
		rnames[iph]->SetNDC();
		rnames[iph]->SetTextSize(0.052);
	}
	TLatex *thistext = new TLatex();
		thistext->SetTextSize(0.06);
		thistext->SetTextColor(4);
		thistext->SetTextAlign(21);
		thistext->SetNDC();

	sprintf(type,"summary");
	char psfileopen[100];
	char psfile[100];
	char pdffile[100];
	char psfileclose[100];
	char rootfile[100];
	sprintf(psfileopen,"tofmult_%s.ps(",type);
	sprintf(psfile,"tofmult_%s.ps",type);
	sprintf(pdffile,"tofmult_%s.pdf",type);
	sprintf(psfileclose,"tofmult_%s.ps]",type);
	cout<<type<<endl;
	cout<<psfileopen<<endl;
	cout<<psfile<<endl;
	cout<<psfileclose<<endl;
	cout<<pdffile<<endl;
		
	
	TH1::AddDirectory(kFALSE);
	
	TH1D *hmultcorrtray_avg[nphases];
	TH1D *hmultcorrtray_rms[nphases]; 	
	TH1D *hmultcorrtray_skew[nphases];	
	TH1D *hncorrtray[nphases];    		
	TH1D *hncorrtraydiag[nphases]; 		
	TH1D *hdiagfrac[nphases]; 	 		
	TH1D *hmultcorrtray_avg_1D[nphases];  
	TH1D *hmultcorrtray_rms_1D[nphases];  
	TH1D *hmultcorrtray_skew_1D[nphases]; 
	TH1D *hdiagfrac_1D[nphases]; 	
	TH2D *htraytotmult_trgvstof[nphases];		
	//
	for (int iph=0;iph<nphases;iph++){
		sprintf(buf,"./tofmult_%s_tof.root",rstrings[iph]);
		TFile *f = new TFile(buf,"READ");
		cout<<buf<<endl;
		//
		hmultcorrtray_avg[iph]	= (TH1D*)f->Get("hmultcorrtray_avg");
		sprintf(buf,"hmultcorrtray_avg%d",iph);
		hmultcorrtray_avg[iph]->SetName(buf);
		hmultcorrtray_rms[iph]	= (TH1D*)f->Get("hmultcorrtray_rms");
		sprintf(buf,"hmultcorrtray_rms%d",iph);
		hmultcorrtray_rms[iph]->SetName(buf);
		hmultcorrtray_skew[iph]	= (TH1D*)f->Get("hmultcorrtray_skew");
		sprintf(buf,"hmultcorrtray_skew%d",iph);
		hmultcorrtray_skew[iph]->SetName(buf);
		//
		hmultcorrtray_avg_1D[iph]	= (TH1D*)f->Get("hmultcorrtray_avg_1D");
		sprintf(buf,"hmultcorrtray_avg_1D%d",iph);
		hmultcorrtray_avg_1D[iph]->SetName(buf);
		hmultcorrtray_rms_1D[iph]	= (TH1D*)f->Get("hmultcorrtray_rms_1D");
		sprintf(buf,"hmultcorrtray_rms_1D%d",iph);
		hmultcorrtray_rms_1D[iph]->SetName(buf);
		hmultcorrtray_skew_1D[iph]	= (TH1D*)f->Get("hmultcorrtray_skew_1D");
		sprintf(buf,"hmultcorrtray_skew_1D%d",iph);
		hmultcorrtray_skew_1D[iph]->SetName(buf);
		//
		hdiagfrac_1D[iph]	= (TH1D*)f->Get("hdiagfrac_1D");
		sprintf(buf,"hdiagfrac_1D%d",iph);
		hdiagfrac_1D[iph]->SetName(buf);
		//
		htraytotmult_trgvstof[iph]	= (TH2D*)f->Get("htraytotmult_trgvstof");
		sprintf(buf,"htraytotmult_trgvstof%d",iph);
		htraytotmult_trgvstof[iph]->SetName(buf);
	}

	//-------------------------------------------------------
	gStyle->SetPalette(1);
	gStyle->SetOptStat(0);
	gStyle->SetPadGridX(kFALSE);
	gStyle->SetPadGridY(kFALSE);
	gStyle->SetPadTopMargin(0.03);
	gStyle->SetPadRightMargin(0.03);

 	++ican; sprintf(buf,"ccan%d",ican);
 	ccan[ican] = new TCanvas(buf,buf,0,0,750.,750.*(8.5/11.));
 	ccan[ican]->SetFillColor(10);
 	ccan[ican]->cd(); ccan[ican]->Divide(4,4,0.0001,0.0001);
	for (int iph=0;iph<nphases;iph++){
		ccan[ican]->cd(1+iph);
		gPad->SetLogz(1);
		htraytotmult_trgvstof[iph]->Draw("colz");
		rnames[iph]->Draw();
		thistext->DrawLatex(0.4,0.84,"#SigmaTRG vs #SigmaTOFnino");
	}
	ccan[ican]->cd(); ccan[ican]->Update();
	ccan[ican]->Print(psfileopen);
		
//  	++ican; sprintf(buf,"ccan%d",ican);
//  	ccan[ican] = new TCanvas(buf,buf,0,0,750.,750.*(8.5/11.));
//  	ccan[ican]->SetFillColor(10);
//  	ccan[ican]->cd(); ccan[ican]->Divide(4,4,0.0001,0.0001);
// 	for (int iph=0;iph<nphases;iph++){
// 		ccan[ican]->cd(1+iph);
// 		hmultcorrtray_avg[iph]->Draw();
// 		rnames[iph]->Draw();
// 		thistext->DrawLatex(0.55,0.89,"#LTN_{nino}-L0mult#GT vs tray");
// 	}
// 	ccan[ican]->cd(); ccan[ican]->Update();
// 	ccan[ican]->Print(psfile);
// 
//  	++ican; sprintf(buf,"ccan%d",ican);
//  	ccan[ican] = new TCanvas(buf,buf,0,0,750.,750.*(8.5/11.));
//  	ccan[ican]->SetFillColor(10);
//  	ccan[ican]->cd(); ccan[ican]->Divide(4,4,0.0001,0.0001);
// 	for (int iph=0;iph<nphases;iph++){
// 		ccan[ican]->cd(1+iph);
// 		hmultcorrtray_rms[iph]->Draw();
// 		rnames[iph]->Draw();
// 		thistext->DrawLatex(0.55,0.89,"#sigma(N_{nino}-L0mult) vs tray");
// 	}
// 	ccan[ican]->cd(); ccan[ican]->Update();
// 	ccan[ican]->Print(psfile);
// 
//  	++ican; sprintf(buf,"ccan%d",ican);
//  	ccan[ican] = new TCanvas(buf,buf,0,0,750.,750.*(8.5/11.));
//  	ccan[ican]->SetFillColor(10);
//  	ccan[ican]->cd(); ccan[ican]->Divide(4,4,0.0001,0.0001);
// 	for (int iph=0;iph<nphases;iph++){
// 		ccan[ican]->cd(1+iph);
// 		hmultcorrtray_skew[iph]->Draw();
// 		rnames[iph]->Draw();
// 		thistext->DrawLatex(0.55,0.89,"Skewness(N_{nino}-L0mult) vs tray");
// 	}
// 	ccan[ican]->cd(); ccan[ican]->Update();
// 	ccan[ican]->Print(psfile);


 	++ican; sprintf(buf,"ccan%d",ican);
 	ccan[ican] = new TCanvas(buf,buf,0,0,750.,750.*(8.5/11.));
 	ccan[ican]->SetFillColor(10);
 	ccan[ican]->cd(); ccan[ican]->Divide(4,4,0.0001,0.0001);
	for (int iph=0;iph<nphases;iph++){
		ccan[ican]->cd(1+iph);
		hmultcorrtray_avg_1D[iph]->Draw();
		rnames[iph]->Draw();
		thistext->DrawLatex(0.55,0.89,"#LTN_{nino}-L0mult#GT");
	}
	ccan[ican]->cd(); ccan[ican]->Update();
	ccan[ican]->Print(psfileopen);

 	++ican; sprintf(buf,"ccan%d",ican);
 	ccan[ican] = new TCanvas(buf,buf,0,0,750.,750.*(8.5/11.));
 	ccan[ican]->SetFillColor(10);
 	ccan[ican]->cd(); ccan[ican]->Divide(4,4,0.0001,0.0001);
	for (int iph=0;iph<nphases;iph++){
		ccan[ican]->cd(1+iph);
		hmultcorrtray_rms_1D[iph]->Draw();
		rnames[iph]->Draw();
		thistext->DrawLatex(0.55,0.89,"#sigma(N_{nino}-L0mult)");
	}
	ccan[ican]->cd(); ccan[ican]->Update();
	ccan[ican]->Print(psfile);

 	++ican; sprintf(buf,"ccan%d",ican);
 	ccan[ican] = new TCanvas(buf,buf,0,0,750.,750.*(8.5/11.));
 	ccan[ican]->SetFillColor(10);
 	ccan[ican]->cd(); ccan[ican]->Divide(4,4,0.0001,0.0001);
	for (int iph=0;iph<nphases;iph++){
		ccan[ican]->cd(1+iph);
		hmultcorrtray_skew_1D[iph]->Draw();
		rnames[iph]->Draw();
		thistext->DrawLatex(0.55,0.89,"Skewness(N_{nino}-L0mult)");
	}
	ccan[ican]->cd(); ccan[ican]->Update();
	ccan[ican]->Print(psfile);

 	++ican; sprintf(buf,"ccan%d",ican);
 	ccan[ican] = new TCanvas(buf,buf,0,0,750.,750.*(8.5/11.));
 	ccan[ican]->SetFillColor(10);
 	ccan[ican]->cd(); ccan[ican]->Divide(4,4,0.0001,0.0001);
	for (int iph=0;iph<nphases;iph++){
		ccan[ican]->cd(1+iph);
		hdiagfrac_1D[iph]->Draw();
		hdiagfrac_1D[iph]->SetStats(0);
		rnames[iph]->Draw();
		thistext->DrawLatex(0.30,0.85,Form("DiagFrac=%4.3f",hdiagfrac_1D[iph]->GetMean()));
	}
	ccan[ican]->cd(); ccan[ican]->Update();
	ccan[ican]->Print(psfile);

	
	ccan[ican]->Print(psfileclose);
	sprintf(buf,"ps2pdf %s %s",psfile,pdffile);
	gSystem->Exec(buf);
	sprintf(buf,"cp -v %s /star/u/llope/WWW/files/",pdffile);
	gSystem->Exec(buf);

}

Int_t KeepTray(int it){
		if (it== 95){ return 0; }
		if (it==102){ return 0; }
		//if (it<61){ return 1; }
		//if (it>72){ return 1; }
		return 1;
}