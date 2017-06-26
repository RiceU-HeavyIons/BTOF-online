

// get the projection for a given tray
// TH2 *h2   : the 2D le vs. tray
// int tray  : 0 indexed tray number
TH1 * projection( TH2 * h2, int tray ){
	stringstream sstr;
	sstr << "tray_le_" << tray+1;
	return h2->ProjectionY( sstr.str().c_str(), tray+1, tray+1 );
}


void writeParamFile( string rn, vector<int> &le, vector<int> &te ){
	ofstream fout( ("tof_datawindows_" + rn + ".dat").c_str() );

	fout << "double cutlow[122] ={\n";
	string delim = " ";
	for ( int i = 0; i < le.size(); i++ ){
		fout << delim << le[i];
		delim = ", ";
	}


	fout << "};";
	fout << "\n\ndouble cuthi[122] ={\n";
	delim = " ";
	for ( int i = 0; i < te.size(); i++ ){
		fout << delim << te[i];
		delim = ", ";
	}
	fout << "};";
}


void extract_windows( string rn, bool zoomRange = true  ){

	cout << "Run# " << rn << endl;
	string fileName = "tof_" + rn + ".root";
	cout << "Looking for input file : " << fileName << endl;
	TFile * f = new TFile( fileName.c_str() ) ;

	if ( NULL == f ){
		cout << "Cannot find input file" << endl;
		return;
	} else {
		cout << "Found input file : " << fileName << endl; 
	}

	string rpName = "tof_datawindows_" + rn + ".pdf";
	cout << "Generating report : " << rpName << endl;
	TCanvas * can = new TCanvas( "tof datawindows" );
	can->Print( ( rpName + "[" ).c_str() );

	TH2 * le_vs_tray = (TH2*)f->Get( "leTrg" );
	if ( NULL == le_vs_tray ){
		cout << "Cannot find \"leTrg\" in input file" << endl;
		return;
	}

	le_vs_tray->GetXaxis()->SetTitle( "Tray #" );
	le_vs_tray->GetYaxis()->SetTitle( "LE Time (ns)" );
	le_vs_tray->Draw( "colz" );
	can->Print( rpName.c_str() );


	int size = 500;
	int signalWidth = 85;
	int backOff = 30;
	cout << "Using window size : " << signalWidth << " ns" << endl;

	vector<int> lowEdges, highEdges;

	for ( int tId = 0; tId < 122; tId++ ){
		TH1 * trayLe = projection( le_vs_tray, tId );
		


		if ( zoomRange )
			trayLe->GetXaxis()->SetRange( trayLe->GetMaximumBin() - size, trayLe->GetMaximumBin() + size );
		trayLe->SetTitle( ("Run# " + rn + "; LE time (ns); count").c_str() );
		trayLe->Draw();


		// TLatex latex;
		// latex.SetTextSize(0.025);
		// latex.SetTextAlign(13);  //align at top
		// latex.DrawLatexNDC(0.2,.9,("Run# " +rn).c_str() );

		if (trayLe->GetEntries() < 1000 ){
			cout << "WARNING: Tray " << tId+1 << " is empty" << endl;
			lowEdges.push_back( 0 );
			highEdges.push_back( 0 );
			can->SetLogy(1);
			can->Print( rpName.c_str() );
			continue;
		}

		TF1 * ff = new TF1( "ff", "landau", trayLe->GetMaximumBin() - 50, trayLe->GetMaximumBin() + 50 );
		ff->SetLineColor( kRed );
		ff->SetLineWidth( 3 );
		trayLe->Fit( ff, "RQ" );

		int cutLow = ff->GetParameter(1) - backOff;
		int cutHigh = ff->GetParameter(1) + signalWidth;

		TLine l1( cutLow, 0, cutLow, trayLe->GetMaximum() );
		l1.SetLineWidth( 3 );
		l1.SetLineColor( kRed );
		l1.Draw();

		TLine l2( cutHigh, 0, cutHigh, trayLe->GetMaximum() );
		l2.SetLineWidth( 3 );
		l2.SetLineColor( kRed );
		l2.Draw();

		lowEdges.push_back( cutLow );
		highEdges.push_back( cutHigh );

		
		can->SetLogy(1);
		can->Print( rpName.c_str() );


	}

	writeParamFile( rn, lowEdges, highEdges );

	// close the report
	can->Print( ( rpName + "]" ).c_str() );
}