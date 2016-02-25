float FindEdgeLower(TH1D* h, int ilh, float co);
float FindEdgeUpper(TH1D* h, int ilh, float co);

void fitzhit(){

	char buf[100],bufb[100];
	TCanvas *ccan[1000];
	int		ican		= -1;
	gStyle->SetPadGridX(0);
	gStyle->SetPadGridY(0);
	gStyle->SetPadTopMargin(0.06);	
	gStyle->SetPadBottomMargin(0.06);	
	gStyle->SetPadRightMargin(0.02);	
	gStyle->SetPadLeftMargin(0.06);	
	
	int ActiveRun13[30] = {1,1,1,1,1,1,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,1,1,1,1,1};
	
	TString noiserunID	= TString("1404277");
	TString rootout		= TString(Form("./fitzhit_%s.root",noiserunID.Data()));
	TString psout		= TString(Form("./fitzhit_%s.ps"  ,noiserunID.Data()));
	TString psoutO		= TString(Form("./fitzhit_%s.ps(" ,noiserunID.Data()));
	TString psoutC		= TString(Form("./fitzhit_%s.ps]" ,noiserunID.Data()));
	TString pdfout		= TString(Form("./fitzhit_%s.pdf" ,noiserunID.Data()));
	
	sprintf(buf,"/star/institutions/rice/llope/noise/noise_%s.root",noiserunID.Data());	
	cout<<"Opening "<<buf<<endl;
	TFile *f = new TFile(buf,"read");
	TH1D *hZstrips[12][5][30];
	for (int is=0;is<12;is++){
 		for (int ib=0;ib<30;ib++){
 			for (int ip=0;ip<5;ip++){
				sprintf(buf,"hZstrips%d_%d_%d",is,ip,ib);
				sprintf(bufb,"Z str=%d ip=%d BL=%d",is+1,ip+1,ib+1);
				hZstrips[is][ip][ib]	= (TH1D*)f->Get(buf);
				hZstrips[is][ip][ib]->SetDirectory(0);
				hZstrips[is][ip][ib]->SetTitle(bufb);
				//
			}
		}
		//
	}
	f->Close();


	TH1D 	*hZstripsb[12][5][30];
	TLine 	*lcutoff[12][5][30];
	TLine 	*ledgel[12][5][30];
	TLine 	*ledgeu[12][5][30];
	TGraph	*gzrange[5][30];
	float 	area;
	float 	cutoff;
	//
	TH3D *xmeansuse = new TH3D("xmeansuse","xmeansuse",30,-0.5,29.5,5,-0.5,4.5,12,-0.5,11.5);
	TH3D *xrangeuse = new TH3D("xrangeuse","xrangeuse",30,-0.5,29.5,5,-0.5,4.5,12,-0.5,11.5);
	//
	for (int ib=0;ib<30;ib++){
		int ipl	= 0;
		int ipu	= 5;
		//if (ib==25){ ipl=1; ipu=4; }
		for (int ip=ipl;ip<ipu;ip++){
			//
			sprintf(buf,"gzrange%d%d",ip,ib);
			gzrange[ip][ib]	= new TGraph();
			gzrange[ip][ib]->SetName(buf);
			sprintf(buf,"Z_{LOCAL} posn=%d BL=%d",ip+1,ib+1);
			gzrange[ip][ib]->SetTitle(buf);
			gzrange[ip][ib]->SetMarkerStyle(20);
			gzrange[ip][ib]->SetMarkerSize(1);
			//				
			if (hZstrips[0][ip][ib]->GetEntries()<10){ continue; }
			if (!ActiveRun13[ib]){ continue; }
			//
			++ican; sprintf(buf,"ccan%d",ican);
			ccan[ican] = new TCanvas(buf,buf,0,0,800.,800.*(7.5/10.));
			ccan[ican]->SetFillColor(10);
			ccan[ican]->cd(); ccan[ican]->Divide(4,3,0.0001,0.0001);
			//
			for (int is=0;is<12;is++){
				//
				area	= hZstrips[is][ip][ib]->Integral(1,560);
				cutoff	= area/15000.;
				if (cutoff<2) cutoff=2;
				lcutoff[is][ip][ib] = new TLine(-7.0,cutoff,7.0,cutoff);
				lcutoff[is][ip][ib]->SetLineColor(2);
				//
				ccan[ican]->cd(1+is);
				gPad->SetLogy(1);
				hZstrips[is][ip][ib]->Draw();
				lcutoff[is][ip][ib]->Draw("same");
				//
				float edgelower = FindEdgeLower(hZstrips[is][ip][ib],0,cutoff);
				float edgeupper = FindEdgeLower(hZstrips[is][ip][ib],1,cutoff);
				//
				//---- special treatment for some strips....
// 				if (is== 0&&ip==1&&ib==26){ edgelower = -4.65; }
// 				if (is== 1&&ip==1&&ib==26){ edgelower = -4.55; }
// 				if (is== 2&&ip==1&&ib==26){ edgelower = -5.45; }
// 				if (is== 3&&ip==1&&ib==26){ edgelower = -4.3; }
				//
				//if (is== 8&&ip==4&&ib==26){ edgelower = -4.85; }
				//if (is== 9&&ip==4&&ib==26){ edgelower = -3.75; }
				//if (is==10&&ip==4&&ib==26){ edgelower = -3.7; }
				//if (is==11&&ip==4&&ib==26){ edgelower = -4.2; }
				//
//				if (is== 0&&ip==3&&ib==27){ edgeupper = 5.2; }
//				if (is== 1&&ip==3&&ib==27){ edgeupper = 5.7; }
//				if (is== 2&&ip==3&&ib==27){ edgeupper = 4.6; }
				//
				float xmean		= int(1000.*(edgeupper+edgelower)/2.)/1000.;
				float xrange	= (edgeupper-edgelower)/2.;
				gzrange[ip][ib]->SetPoint(is,is,xrange);
				cout<<ib+1<<" "<<ip+1<<" "<<is<<" "<<edgelower<<" "<<edgeupper
					<<"\t"<<xmean<<" "<<xrange<<" "<<area<<endl;
				ledgel[is][ip][ib] = new TLine(edgelower,0.5,edgelower,1000.);
				ledgel[is][ip][ib]->SetLineColor(2);
				ledgel[is][ip][ib]->Draw("same");
				ledgeu[is][ip][ib] = new TLine(edgeupper,0.5,edgeupper,1000.);
				ledgeu[is][ip][ib]->SetLineColor(2);
				ledgeu[is][ip][ib]->Draw("same");
				//
				xmeansuse->Fill(ib,ip,is,xmean);
				xrangeuse->Fill(ib,ip,is,xrange);
				//
				sprintf(buf,"hZstripsb%d_%d_%d",is,ip,ib);
				sprintf(bufb,"Z str=%d pos=%d BL=%d",is+1,ip+1,ib+1);
				hZstripsb[is][ip][ib] = new TH1D(buf,bufb,560,-7.0,7.0);
				hZstripsb[is][ip][ib]->SetLineColor(4);
				for (int ibx=1;ibx<560;ibx++){
					float xbin	= hZstrips[is][ip][ib]->GetBinCenter(ibx);
					float nbin	= hZstrips[is][ip][ib]->GetBinContent(ibx);
					float xbin2	= xbin - xmean;
					if (TMath::Abs(xbin2)<5.2){
						int ibn		= hZstripsb[is][ip][ib]->GetXaxis()->FindBin(xbin2);
						hZstripsb[is][ip][ib]->SetBinContent(ibn,nbin);
					}
				}
				//
			}
			ccan[ican]->cd(); ccan[ican]->Update();
			if (ican==0){ ccan[ican]->Print(psoutO.Data()); }
				   else { ccan[ican]->Print(psout.Data());  }
		}
	}

	cout<<"Writing "<<rootout.Data()<<endl;
	TFile *fout	= new TFile(rootout.Data(),"recreate");
	 xmeansuse->Write();
	 xrangeuse->Write();
	fout->Close();
	
	int ipl	= 0;
	int ipu	= 5;

	for (int ib=0;ib<30;ib++){
		if (!ActiveRun13[ib]){ continue; }
		for (int ip=ipl;ip<ipu;ip++){
			//
			++ican; sprintf(buf,"ccan%d",ican);
			ccan[ican] = new TCanvas(buf,buf,0,0,800.,800.*(7.5/10.));
			ccan[ican]->SetFillColor(10);
			ccan[ican]->cd(); ccan[ican]->Divide(4,3,0.0001,0.0001);
			for (int is=0;is<12;is++){
				//
				ccan[ican]->cd(1+is);
				gPad->SetLogy(1);
				hZstripsb[is][ip][ib]->Draw();
			}
			ccan[ican]->cd(); ccan[ican]->Update();
			ccan[ican]->Print(psout.Data()); 
		}
	}

	int kcolor[5]	= {1,2,3,4,6};
	++ican; sprintf(buf,"ccan%d",ican);
	ccan[ican] = new TCanvas(buf,buf,0,0,800.,800.*(7.5/10.));
	ccan[ican]->SetFillColor(10);
	ccan[ican]->cd(); ccan[ican]->Divide(5,6,0.0001,0.0001);
	for (int ib=0;ib<30;ib++){
		if (!ActiveRun13[ib]){ continue; }
		ccan[ican]->cd(1+(ib-25));
		TH1F *frame = gPad->DrawFrame(-0.5,4.6,11.5,5.6);
		for (int ip=ipl;ip<ipu;ip++){
			gzrange[ip][ib]->SetMarkerColor(kcolor[ip]);
			gzrange[ip][ib]->SetLineColor(kcolor[ip]);
			gzrange[ip][ib]->Draw("PL");
		}
	}
	ccan[ican]->cd(); ccan[ican]->Update();
	ccan[ican]->Print(psout.Data()); 

	TLine *vert	= new TLine(-4.4,0.5,-4.4,1000);
		vert->SetLineWidth(1);
		vert->SetLineColor(17);
	for (int ib=1;ib<30;ib++){
		if (!ActiveRun13[ib]){ continue; }
		for (int ip=ipl;ip<ipu;ip++){
			++ican; sprintf(buf,"ccan%d",ican);
			ccan[ican] = new TCanvas(buf,buf,0,0,800.,800.*(7.5/10.));
			ccan[ican]->SetFillColor(10);
			ccan[ican]->cd(); ccan[ican]->Divide(1,3,0.0001,0.0001);
				ccan[ican]->cd(1);
				gPad->SetLogy(1);
					hZstripsb[0][ip][ib]->Draw();
					vert->Draw("same");
					hZstripsb[0][ip][ib]->Draw("same");
					hZstripsb[1][ip][ib]->Draw("same");
					hZstripsb[2][ip][ib]->Draw("same");
					hZstripsb[3][ip][ib]->Draw("same");
				ccan[ican]->cd(2);
				gPad->SetLogy(1);
					hZstripsb[4][ip][ib]->Draw();
					vert->Draw("same");
					hZstripsb[4][ip][ib]->Draw("same");
					hZstripsb[5][ip][ib]->Draw("same");
					hZstripsb[6][ip][ib]->Draw("same");
					hZstripsb[7][ip][ib]->Draw("same");
				ccan[ican]->cd(3);
				gPad->SetLogy(1);
					hZstripsb[8][ip][ib]->Draw();
					vert->Draw("same");
					hZstripsb[8][ip][ib]->Draw("same");
					hZstripsb[9][ip][ib]->Draw("same");
					hZstripsb[10][ip][ib]->Draw("same");
					hZstripsb[11][ip][ib]->Draw("same");
			ccan[ican]->cd(); ccan[ican]->Update();
			ccan[ican]->Print(psout.Data());
		}
	}


	ccan[ican]->Print(psoutC.Data());
	sprintf(buf,"/usr/bin/ps2pdf %s %s",psout.Data(),pdfout.Data());
	cout<<"..... Executing ... "<<buf<<endl;
	gSystem->Exec(buf);
	sprintf(buf,"/bin/cp -v %s /star/u/llope/WWW/files/",pdfout.Data());
	cout<<"..... Executing ... "<<buf<<endl;
	gSystem->Exec(buf);

}

float FindEdgeLower(TH1D *h, int ilohi, float cutoff){

	float edge	=   0;
	int   kl	=   1;
	int   ku	= 560;

	int ibx,inc;
	for (int kbx=kl;kbx<ku;kbx++){
		if (ilohi==0){ 
			ibx=kbx; inc=1; 		// lower edge, increment
		}else{ 
			ibx=ku-kbx; inc=-1; 	// upper edge, decrement
		}
		float xbin	= h->GetBinCenter(ibx);
		float nbin	= h->GetBinContent(ibx);
		float nbin1	= h->GetBinContent(ibx+inc);
		float nbin2	= h->GetBinContent(ibx+2*inc);
		if (edge==0&&nbin>cutoff&&nbin1>cutoff+5&&nbin2>cutoff+10){ 
				edge = xbin; 
				break; 
		}
	}
	return edge;
		
}
