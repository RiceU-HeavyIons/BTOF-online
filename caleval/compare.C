void compare(){

	char		buf[100],tbuf[100];
	TCanvas 	*ccan[1000];
	TLatex		*text[1000];
	Int_t		ican=-1,itext=-1;
	for (int i=0;i<1000;i++){
		text[i]	= new TLatex();
		text[i]->SetNDC();
		text[i]->SetTextSize(0.06);
	}
	TString psfileO	= TString("caleval_compare.ps(");
	TString psfile	= TString("caleval_compare.ps");
	TString psfileC	= TString("caleval_compare.ps]");
	TString pdffile	= TString("caleval_compare.pdf");
		
	const int	NDS = 5;
	const char* runstrings[NDS] = {"run13pp500","run10auau007","run10auau039","run10auau062","run14auau200"};
	const int	NMETH = 10;
	const char* methods[NMETH]	= {"ch0 ch0","earl hit raw","earl hit cal","cal lit avg",
	                            	"steps","lines","curve avg",
	                            	"steps#LT4#GT","lines#LT4#GT","curve avg#LT4#GT"};
	int			NPOINTS	= NDS*NMETH;
	TString 	filename;
	TH1D*		hresults_mean[2][NDS];
	TH1D*		hresults_sigm[2][NDS];
	TH1D*		hresults_mean_peri[2][NDS];
	TH1D*		hresults_sigm_peri[2][NDS];
	TH1D*		hresults_mean_cent[2][NDS];
	TH1D*		hresults_sigm_cent[2][NDS];
	TH1D*		hresultssummary_sigm[2];
	TH1D*		hresultssummary_sigm_peri[2];
	TH1D*		hresultssummary_sigm_cent[2];
	TH2D*		hnermnorm[NDS];
	for (int ids=0;ids<NDS;ids++){
		for (int ic=0;ic<2;ic++){		// iChoice
			//
			if (ic==0){
				filename 	= TString(Form("./root/caleval_%s_ic%d.root",runstrings[ids],ic));
			} else if (ic==1){
				filename 	= TString(Form("./root/caleval_%s_ic%d_bbq.root",runstrings[ids],ic));
			}
			//cout<<filename.Data()<<endl;
			TFile *f	= new TFile(filename.Data(),"read");
			//
			if (ids==0){
				sprintf(buf,"hresultssummary_sigm%d",ic);
				hresultssummary_sigm[ic]	= new TH1D(buf,buf,NPOINTS,0.5,NPOINTS+0.5);
				hresultssummary_sigm[ic]->SetMarkerStyle(24-ic*4);
				hresultssummary_sigm[ic]->SetMarkerSize(1.5);
				sprintf(buf,"hresultssummary_sigm_peri%d",ic);
				hresultssummary_sigm_peri[ic]	= new TH1D(buf,buf,NPOINTS,0.5,NPOINTS+0.5);
				hresultssummary_sigm_peri[ic]->SetMarkerStyle(24-ic*4);
				hresultssummary_sigm_peri[ic]->SetMarkerSize(1.5);
				hresultssummary_sigm_peri[ic]->SetMarkerColor(4);
				sprintf(buf,"hresultssummary_sigm_cent%d",ic);
				hresultssummary_sigm_cent[ic]	= new TH1D(buf,buf,NPOINTS,0.5,NPOINTS+0.5);
				hresultssummary_sigm_cent[ic]->SetMarkerStyle(24-ic*4);
				hresultssummary_sigm_cent[ic]->SetMarkerSize(1.5);
				hresultssummary_sigm_cent[ic]->SetMarkerColor(2);
			}
			//
			hresults_mean[ic][ids]	= (TH1D*)f->Get("hresults_mean");
			sprintf(buf,"hresults_mean%d%d",ic,ids);
			hresults_mean[ic][ids]->SetName(buf);
			hresults_mean[ic][ids]->SetDirectory(0);
			hresults_sigm[ic][ids]	= (TH1D*)f->Get("hresults_sigm");
			sprintf(buf,"hresults_sigm%d%d",ic,ids);
			hresults_sigm[ic][ids]->SetName(buf);
			hresults_sigm[ic][ids]->SetDirectory(0);
			hresults_mean_peri[ic][ids]	= (TH1D*)f->Get("hresults_mean_peri");
			sprintf(buf,"hresults_mean_peri%d%d",ic,ids);
			hresults_mean_peri[ic][ids]->SetName(buf);
			hresults_mean_peri[ic][ids]->SetDirectory(0);
			hresults_sigm_peri[ic][ids]	= (TH1D*)f->Get("hresults_sigm_peri");
			sprintf(buf,"hresults_sigm_peri%d%d",ic,ids);
			hresults_sigm_peri[ic][ids]->SetName(buf);
			hresults_sigm_peri[ic][ids]->SetDirectory(0);
			hresults_mean_cent[ic][ids]	= (TH1D*)f->Get("hresults_mean_cent");
			sprintf(buf,"hresults_mean_cent%d%d",ic,ids);
			hresults_mean_cent[ic][ids]->SetName(buf);
			hresults_mean_cent[ic][ids]->SetDirectory(0);
			hresults_sigm_cent[ic][ids]	= (TH1D*)f->Get("hresults_sigm_cent");
			sprintf(buf,"hresults_sigm_cent%d%d",ic,ids);
			hresults_sigm_cent[ic][ids]->SetName(buf);
			hresults_sigm_cent[ic][ids]->SetDirectory(0);
			//
			float val,vale;
			for (int imeth=0;imeth<NMETH;imeth++){
				val		= hresults_sigm[ic][ids]->GetBinContent(imeth+1);
				vale	= hresults_sigm[ic][ids]->GetBinError(imeth+1);
				int k	= ids*NMETH + imeth;
				hresultssummary_sigm[ic]->SetBinContent(1+k,val);
				cout<<ic<<" "<<ids<<" "<<runstrings[ids]<<" "<<imeth<<" "<<val<<" "<<vale<<" "<<NPOINTS-k<<endl;
				val		= hresults_sigm_peri[ic][ids]->GetBinContent(imeth+1);
				vale	= hresults_sigm_peri[ic][ids]->GetBinError(imeth+1);
				int k	= ids*NMETH + imeth;
				hresultssummary_sigm_peri[ic]->SetBinContent(1+k,val);				
				val		= hresults_sigm_cent[ic][ids]->GetBinContent(imeth+1);
				vale	= hresults_sigm_cent[ic][ids]->GetBinError(imeth+1);
				int k	= ids*NMETH + imeth;
				hresultssummary_sigm_cent[ic]->SetBinContent(1+k,val);
			}
			//
			if (ic==0){		// VPD E mult vs refmult from TOF data....
				hnermnorm[ids]	= (TH2D*)f->Get("hnermnorm");
				sprintf(buf,"hnermnorm%d",ids);
				hnermnorm[ids]->SetName(buf);
				hnermnorm[ids]->SetDirectory(0);
			}
			//
		}	//---- end ichoice loop
	}
	
	float ymax	= 40;
	TLine* vlines[NDS];
	TLatex* tdata[NDS];
	for (int ids=0;ids<NDS;ids++){
		vlines[ids]	= new TLine(0.5+(ids+1)*NMETH,0,0.5+(ids+1)*NMETH,ymax);
		vlines[ids]->SetLineColor(17);
		vlines[ids]->SetLineWidth(2);
		tdata[ids]	= new TLatex();
		tdata[ids]->SetTextSize(0.035);
	}
	TLegend *leg	= new TLegend(0.77,0.83,0.99,0.95);
		leg->SetTextSize(0.05);
		leg->AddEntry(hresultssummary_sigm[0],"TOF TDIG","P");
		leg->AddEntry(hresultssummary_sigm[1],"TRG TAC","P");
	TLegend *legb	= new TLegend(0.55,0.81,0.99,0.95);
		legb->SetTextSize(0.05);
		legb->AddEntry(hresultssummary_sigm[0],"TOF TDIG","P");
		//legb->AddEntry(hresultssummary_sigm[1],"TRG TAC","P");
		legb->AddEntry(hresultssummary_sigm_peri[1],"TRG TAC, peripheral","P");
		legb->AddEntry(hresultssummary_sigm_cent[1],"TRG TAC, central","P");
	//
	gROOT->SetStyle("Plain"); 
	gStyle->SetPadBottomMargin(0.17);
	gStyle->SetPadTopMargin(0.04);
	gStyle->SetPadLeftMargin(0.077);
	gStyle->SetPadRightMargin(0.01);
	gStyle->SetOptStat(0);
	gStyle->SetOptFit(0);
	gStyle->SetPalette(1);
	gStyle->SetErrorX(0);
	
	float xmax[NDS]	= {50,276,421,496,646};
	TProfile* hnermnormproj[NDS];
	//
	gStyle->SetPadBottomMargin(0.06);
	gStyle->SetPadLeftMargin(0.14);
	gStyle->SetPadRightMargin(0.02);
	gStyle->SetPadTopMargin(0.01);
	++ican; sprintf(buf,"ccan%d",ican);
 	ccan[ican] = new TCanvas(buf,buf,0,0,800.,800.*(7.5/10.));
 	ccan[ican]->SetFillColor(10);
 	ccan[ican]->cd(); ccan[ican]->Divide(5,1,0.0001,0.0001);
	for (int ids=0;ids<NDS;ids++){
		ccan[ican]->cd(1+ids);	
			hnermnorm[ids]->SetTitle("");
			hnermnorm[ids]->GetXaxis()->SetRangeUser(0,xmax[ids]);
			hnermnorm[ids]->Draw("col");
   			hnermnormproj[ids]	= (TProfile*)hnermnorm[ids]->ProfileX(Form("hnermnormproj%d",ids));
   					hnermnormproj[ids]->SetLineColor(6);
   					hnermnormproj[ids]->SetLineWidth(5);
   					hnermnormproj[ids]->Draw("HIST same");
			hnermnorm[ids]->GetXaxis()->SetLabelSize(0.1);
			hnermnorm[ids]->GetYaxis()->SetLabelSize(0.1);
			hnermnorm[ids]->GetXaxis()->SetLabelOffset(-0.05);
			hnermnorm[ids]->GetXaxis()->SetNdivisions(4);
			gPad->Update();
	}
 	ccan[ican]->cd(); ccan[ican]->Update();
 	ccan[ican]->Print(psfileO.Data());
	
	gStyle->SetPadBottomMargin(0.17);
	gStyle->SetPadTopMargin(0.04);
	gStyle->SetPadLeftMargin(0.077);
	gStyle->SetPadRightMargin(0.01);
	++ican; sprintf(buf,"ccan%d",ican);
 	ccan[ican] = new TCanvas(buf,buf,0,0,800.,800.*(7.5/10.));
 	ccan[ican]->SetFillColor(10);
 	ccan[ican]->cd(); ccan[ican]->Divide(1,1,0.0001,0.0001);
		ccan[ican]->cd(1);	
			hresultssummary_sigm[0]->SetTitle("");	
			hresultssummary_sigm[0]->SetMaximum(ymax);
			hresultssummary_sigm[0]->Draw("P");
			hresultssummary_sigm[1]->Draw("P same");
			gPad->SetGrid(0,1);
			hresultssummary_sigm[0]->GetYaxis()->SetTitle("Vertex Resolution: #sigma(Z_{VPD} - Z_{TPC})");
			for (int ids=0;ids<NDS;ids++){
				if (ids<4) vlines[ids]->Draw("same");
				tdata[ids]->SetTextAlign(21);
				tdata[ids]->DrawLatex(((float)NMETH/2.)+0.5+(ids*NMETH),ymax+0.5,Form("%s",runstrings[ids]));
			}
			for (int ibx=1;ibx<=NDS*NMETH;ibx++){
				int imeth	= (ibx-1)%NMETH;
				sprintf(buf,"%s",methods[imeth]);
				hresultssummary_sigm[0]->GetXaxis()->SetBinLabel(ibx,buf);
			}
			hresultssummary_sigm[0]->GetXaxis()->LabelsOption("v");
			hresultssummary_sigm[0]->GetXaxis()->SetLabelSize(0.045);
			gPad->Update();
			leg->Draw();
 	ccan[ican]->cd(); ccan[ican]->Update();
 	ccan[ican]->Print(psfile.Data());
	//	
	gStyle->SetPadBottomMargin(0.17);
	gStyle->SetPadTopMargin(0.04);
	gStyle->SetPadLeftMargin(0.077);
	gStyle->SetPadRightMargin(0.01);
	++ican; sprintf(buf,"ccan%d",ican);
 	ccan[ican] = new TCanvas(buf,buf,0,0,800.,800.*(7.5/10.));
 	ccan[ican]->SetFillColor(10);
 	ccan[ican]->cd(); ccan[ican]->Divide(1,1,0.0001,0.0001);
		ccan[ican]->cd(1);	
			hresultssummary_sigm[0]->SetTitle("");	
			hresultssummary_sigm[0]->SetMaximum(ymax);
			hresultssummary_sigm[1]->SetTitle("");	
			hresultssummary_sigm[1]->SetMaximum(ymax);
			hresultssummary_sigm[0]->Draw("P");
			//hresultssummary_sigm[1]->Draw("P same");
			hresultssummary_sigm_peri[1]->Draw("P same");
			hresultssummary_sigm_cent[1]->Draw("P same");
			gPad->SetGrid(0,1);
			hresultssummary_sigm[0]->GetYaxis()->SetTitle("Vertex Resolution: #sigma(Z_{VPD} - Z_{TPC})");
			for (int ids=0;ids<NDS;ids++){
				if (ids<4) vlines[ids]->Draw("same");
				tdata[ids]->SetTextAlign(21);
				tdata[ids]->DrawLatex(((float)NMETH/2.)+0.5+(ids*NMETH),ymax+0.5,Form("%s",runstrings[ids]));
			}
			for (int ibx=1;ibx<=NDS*NMETH;ibx++){
				int imeth	= (ibx-1)%NMETH;
				sprintf(buf,"%s",methods[imeth]);
				hresultssummary_sigm[0]->GetXaxis()->SetBinLabel(ibx,buf);
			}
			hresultssummary_sigm[0]->GetXaxis()->LabelsOption("v");
			hresultssummary_sigm[0]->GetXaxis()->SetLabelSize(0.045);
			gPad->Update();
			legb->Draw();
 	ccan[ican]->cd(); ccan[ican]->Update();
 	ccan[ican]->Print(psfile.Data());
	
	
	//=============================================================================	
	//
 	cout<<"..... Loop  Closing ps file...."<<endl;
	ccan[ican]->Print(psfileC.Data());
 	sprintf(buf,"/usr/bin/pstopdf %s -o %s",psfile.Data(),pdffile.Data());
 	cout<<"..... Loop  Executing ... "<<buf<<endl;
 	gSystem->Exec(buf);
 	sprintf(buf,"/bin/cp %s /star/u/geurts/WWW/files/",pdffile.Data());
 	cout<<"..... Loop  Executing ... "<<buf<<endl;
 	gSystem->Exec(buf);
	cout<<"..... Loop  Done...."<<endl;	
	
	

}
