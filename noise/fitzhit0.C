void fitzhit(){

	char buf[100];
	TCanvas *ccan[100];
	int		ican		= -1;
	gStyle->SetPadGridX(0);
	gStyle->SetPadGridY(0);
	gStyle->SetPadTopMargin(0.06);	
	gStyle->SetPadBottomMargin(0.06);	
	gStyle->SetPadRightMargin(0.02);	
	gStyle->SetPadLeftMargin(0.06);	
	
	TFile *f = new TFile("/star/institutions/rice/llope/noise/noise_99999.root","read");
	TH1D *hZstrips[12][5][30];
	for (int is=0;is<12;is++){
 		for (int ib=25;ib<28;ib++){
 			for (int ip=0;ip<5;ip++){
				sprintf(buf,"hZstrips%d_%d_%d",is,ip,ib);
				hZstrips[is][ip][ib]	= (TH1D*)f->Get(buf);
				hZstrips[is][ip][ib]->SetDirectory(0);
				//
			}
		}
		//
	}
	f->Close();


//	float gl[12] = {4.7,5.3,4.2,5.4,6.0,4.6,-4.3,-4.3,-5.0,-4.3,-4.25,-4.9};
//	float gu[12] = {5.1,5.6,4.5,5.7,6.3,4.9,-4.0,-4.0,-4.8,-4.0,-3.95,-4.6};

	for (int ib=27;ib<28;ib++){		//!!!!!!
		int ipl	= 4;				//!!!!!!
		int ipu	= 5;
		if (ib==25){ ipl=1; ipu=4; }
		for (int ip=ipl;ip<ipu;ip++){
			++ican; sprintf(buf,"ccan%d",ican);
			ccan[ican] = new TCanvas(buf,buf,0,0,800.,800.*(7.5/10.));
			ccan[ican]->SetFillColor(10);
			ccan[ican]->cd(); ccan[ican]->Divide(4,3,0.0001,0.0001);
			float asum = 0;
			float asummax = 0; int ibmax; int ipmax;
			for (int is=0;is<12;is++){
				//
				//sprintf(buf,"g%d",i);
				//hZstrip[is][ip][ib]->Fit(buf,"0NQR");
				//g[is][ip][ib]->GetParameters(gpars);
				//cout<<i<<" "<<gpars[1]<<endl;
				//aoff[i]	= gpars[1];
				//if (i<6){ apos += gpars[1]/6.; }
				//   else { aneg += gpars[1]/6.; }
				//
				ccan[ican]->cd(1+is);
				gPad->SetLogy(1);
				hZstrips[is][ip][ib]->Draw();
				asum += hZstrips[is][ip][ib]->Integral(1,560);
				//
				//g[i]->Draw("same");
				//
			}
			//cout<<ib<<" "<<ip<<" "<<asum<<endl;
			if (asum>asummax){ asummax = asum; ipmax=ip; ibmax=ib; }
			ccan[ican]->cd(); ccan[ican]->Update();
			if (ican==0){ ccan[ican]->Print("fitzhit.ps("); }
				   else { ccan[ican]->Print("fitzhit.ps");  }
		}
	}
	cout<<ipmax<<" "<<ibmax<<" "<<asummax<<endl;

// 	cout<<"apos = "<<apos<<endl;
// 	cout<<"aneg = "<<aneg<<endl;
// 
// 	cout<<"float zmeansadj[12]={";
// 	for (int i=0;i<12;i++){
// 	         if (i< 6){ cout<<aoff[i]-apos<<","; }
// 		else if (i<11){ cout<<aoff[i]-aneg<<","; }
// 		else          { cout<<aoff[i]-aneg; }
// 	} cout<<"};"<<endl;

//                     0    1     2     3    4    5     6     7     8    9   10   11
	float pos[12] = {-4.3,-4.2,-5.35,-4.1,-3.7,-5.0,-4.35,-4.5,-5.0,-4.15,-4.15,-4.75};
	float shift[12];
	TH1D *hshifted[12];
	for (int is=0;is<12;is++){
		shift[is]	= pos[is]+4.3;
		cout<<is<<" "<<shift[is]<<endl;		
		//
		sprintf(buf,"hshifted%d",is);
		hshifted[is]	= new TH1D(buf,buf,560,-7.0,7.0);
		//
 		int ip	=  4;
 		int ib	= 27;
		for (int ibz=1;ibz<=560;ibz++){
			float zbin 		= hZstrips[is][ip][ib]->GetBinCenter(ibz);
			float nbin 		= hZstrips[is][ip][ib]->GetBinContent(ibz);
			float zbinsh	= zbin - shift[is];
			int ibzsh 		= hshifted[is]->GetXaxis()->FindBin(zbinsh);
			hshifted[is]->SetBinContent(ibzsh,nbin);
		}
		//
// 		++ican; sprintf(buf,"ccan%d",ican);
// 		ccan[ican] = new TCanvas(buf,buf,0,0,800.,800.*(7.5/10.));
// 		ccan[ican]->SetFillColor(10);
// 		ccan[ican]->cd(); ccan[ican]->Divide(1,1,0.0001,0.0001);
// 		ccan[ican]->cd(1);
// 			gPad->SetLogy(1);
// 			hshifted[is]->SetLineColor(1+(is%4));
// 			hshifted[is]->Draw();
//  		ccan[ican]->cd(); ccan[ican]->Update();
//  		ccan[ican]->Print("fitzhit.ps");
		//
	}

	TLine *vert	= new TLine(-4.4,0.5,-4.4,1000);
		vert->SetLineWidth(1);
		vert->SetLineColor(17);
	++ican; sprintf(buf,"ccan%d",ican);
	ccan[ican] = new TCanvas(buf,buf,0,0,800.,800.*(7.5/10.));
	ccan[ican]->SetFillColor(10);
	ccan[ican]->cd(); ccan[ican]->Divide(1,3,0.0001,0.0001);
		ccan[ican]->cd(1);
		gPad->SetLogy(1);
			hshifted[0]->Draw();
			vert->Draw("same");
			hshifted[0]->Draw("same");
			hshifted[1]->Draw("same");
			hshifted[2]->Draw("same");
			hshifted[3]->Draw("same");
		ccan[ican]->cd(2);
		gPad->SetLogy(1);
			hshifted[4]->Draw();
			vert->Draw("same");
			hshifted[4]->Draw("same");
			hshifted[5]->Draw("same");
			hshifted[6]->Draw("same");
			hshifted[7]->Draw("same");
		ccan[ican]->cd(3);
		gPad->SetLogy(1);
			hshifted[8]->Draw();
			vert->Draw("same");
			hshifted[8]->Draw("same");
			hshifted[9]->Draw("same");
			hshifted[10]->Draw("same");
			hshifted[11]->Draw("same");
	ccan[ican]->cd(); ccan[ican]->Update();
	ccan[ican]->Print("fitzhit.ps");

	cout<<"float zshift[12]={";
	for (int is=0;is<12;is++){
		if (is<11){ cout<<shift[is]<<","; }
		     else { cout<<shift[is]; }
	} cout<<"};"<<endl;


// 	double gpars[3];
// 	TF1 *g[12];
// 	float apos = 0;
// 	float aneg = 0;
// 	float aoff[12] = {0.};
// 	for (int is=0;is<12;is++){
// 		//
// 		int ip	=  4;
// 		int ib	= 27;
// 		//
// 		sprintf(buf,"g%d",is);
// 		g[is]	= new TF1(buf,"gaus",gl[is],gu[is]);
// 		g[is]->SetLineWidth(1);
// 		g[is]->SetLineColor(2);
// 		sprintf(buf,"g%d",i);
// 		hZstrip[is][ip][ib]->Fit(buf,"0NQR");
// 		g[is]->GetParameters(gpars);
// 		cout<<i<<" "<<gpars[1]<<endl;
// 		aoff[i]	= gpars[1];
// 		//
// 		//
// 		++ican; sprintf(buf,"ccan%d",ican);
// 		ccan[ican] = new TCanvas(buf,buf,0,0,800.,800.*(7.5/10.));
// 		ccan[ican]->SetFillColor(10);
// 		ccan[ican]->cd(); ccan[ican]->Divide(1,1,0.0001,0.0001);
// 		ccan[ican]->cd(1);
// 		gPad->SetLogy(1);
// 			hZstrips[is][ip][ib]->SetLineColor(20+(ib-25)*5+ip);
// 			hZstrips[is][ip][ib]->Draw();
// 		ccan[ican]->cd(); ccan[ican]->Update();
// 		ccan[ican]->Print("fitzhit.ps");
// 	}


	ccan[ican]->Print("fitzhit.ps]");
	sprintf(buf,"/usr/bin/ps2pdf %s.ps %s.pdf","fitzhit","fitzhit");
	cout<<"..... Executing ... "<<buf<<endl;
	gSystem->Exec(buf);
	sprintf(buf,"/bin/cp -v %s.pdf /star/u/llope/WWW/files/","fitzhit");
	cout<<"..... Executing ... "<<buf<<endl;
	gSystem->Exec(buf);

}
