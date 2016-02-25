#include "cal_class.h"
#include <fstream>

Double_t funres(Double_t *x, Double_t *par){
	Double_t resval = 0.0;
	if (x[0]>0){
		resval	= TMath::Sqrt(x[0]/(1.+x[0]));
		return par[0]/resval;
	} else {
		return 0.0;
	}
}
TF1 *funm = new TF1("funm","20.+pow(x,2.15)"    ,0.1,60.1);
TF1 *funu = new TF1("funu","15.+2.00*pow(x,2.0)",0.1,60.1);
TF1 *funl = new TF1("funl","15.+0.75*pow(x,2.0)",0.1,60.1);

static const unsigned BIT_LENGTH = 64;
unsigned int daqbitpattern[BIT_LENGTH];

const int LLa	= 1;
const int LLb	= 5;

float FindPeak(TH1D*);

//
//=======================================================================================
//
void cal::Loop(Int_t ntodo)
{

	bool	IsDAQ	= false;
	bool	IsFO	= false;
#ifdef muapp
	IsFO	= true;
	cout <<".... Loop Starting MUAPP......... "<<(int)IsFO<<" "<<(int)IsDAQ<<endl;
#else
	IsDAQ	= true;
	cout <<".... Loop Starting DAQDOER....... "<<(int)IsFO<<" "<<(int)IsDAQ<<endl;
#endif

	cout<<"kChanKeyOffsets = "<<kChanKeyOffsets<<endl;
	cout<<"T_min           = "<<T_min<<endl;
	cout<<"T_max           = "<<T_max<<endl;

	char		buf[60],buf2[60],buftext[60];

	//sprintf(buf,"SplineFitDB_ic%d.rdb",iChoice);
	//	TSplineFit::NameFile(buf);

	TCanvas 	*ccan[100];
	Int_t		ncan=-1,ipad=0;
	//TSplineFit 	*fx[8];
	//TH1D			*hdt_end_xp_1dm[8];
	//TCanvas 		*ccanxm;
	//TH1D			*hdt_end_xp_1dr[8];
	//TCanvas 		*ccanxr;
	//TSplineFit 	*fadctot[38][2];

	gStyle->SetPaperSize(TStyle::kUSLetter);
	gStyle->SetPadBottomMargin(0.08);
	gStyle->SetPadLeftMargin(0.10);
	gStyle->SetPadTopMargin(0.06);
	gStyle->SetPadRightMargin(0.02);
	gStyle->SetPalette(1);
	gStyle->SetOptStat(11);
	gROOT->ForceStyle();

	Int_t		ipass,keyCh,keyChprev,keyCtr,keyLR,idete,idetw,idetew,jdetew,indew,idetu,kk;
	Int_t		iCTR,iLR,ind,idt,idtime;
	Int_t		newraw[2];
	Int_t		newe[2],neweb[2],neweu[2],ntmp;
	Float_t		tavg[2],tavgu[2],ttmp;
	Float_t		valcor,xpos,thisval,maxval,dzdval;
	Float_t		tavCtr,tavCtre,tavCtrw;
	Bool_t		keyCtrLit[2];
	Float_t		tearliest[100][2];
	Float_t		tsofar[100][2];
	Float_t		dtsofar[100][2];
	Bool_t		useDet[100][2];
	Bool_t		passedAAcut;

	Bool_t		Egood;
	Bool_t		Wgood;
	Bool_t		EWcoincidence;
	Int_t		n_valid_pairs;
	Int_t		valid_pairs[2][4000];
	Int_t		n_valid_pairsb;
	Int_t		valid_pairsb[2][4000];
	Bool_t		useDetZV[100][2];
	
	Bool_t		LastIter;
	Bool_t		FirstIter;
	
	Float_t		te,tw,dt,zvt;
//	Float_t		thit_a[2],thit_avg,dt_zv,dt_zvcut,thist,dtavg;
//	Int_t		thit_n[2];

	//const Int_t		thisKeyCh[8]	= {0,1,2,3,4,5,6,7};
	//const Int_t		prevKeyCh[8]	= {7,0,1,2,3,4,5,6};
	
	const int thisKeyCh_pass0[19] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
	                                 10,11,12,13,14,15,16,17,18     };
	const int thisKeyCh_pass1[19] = {18,17,16,15,14,13,12,11,10, 9,
								      8, 7, 6, 5, 4, 3, 2, 1, 0     };
	const int thisKeyCh_pass2[19] = {10, 0,11, 1,12, 2,13, 3,14, 4,
								     15, 5,16, 6,17, 7,18, 8, 9     };

	if (fChain == 0) return;
	Long64_t nb = 0;
	Long64_t nentries;
	//cout <<fChain->GetCurrentFile()->GetName()<<endl;
	if (ntodo==0){
		nentries	= nentriestree;
	} else {
		nentries	= ntodo;
	}
	cout<<".... Nev in Chain = "<<nentriestree<<endl;
	cout<<".... Nev to do    = "<<nentries    <<endl;
	float ZVertexMax	= 50.;
	cout<<".... Loop   iRhicRun        = "<<iRhicRun<<endl;
	cout<<".... Loop   ZVertexMax      = "<<ZVertexMax<<endl;
	cout<<".... Loop   Nprimarycut     = "<<Nprimarycut<<endl;
	cout<<".... Loop   refmultcut      = "<<refmultcut<<endl;
	cout<<".... Loop   vtx_spot_x      = "<<vtx_spot_x<<endl;
	cout<<".... Loop   vtx_spot_y      = "<<vtx_spot_y<<endl;
	cout<<".... Loop   ADC Lower Limit = "<<ADCminimum<<endl;
	
	TLatex		*text = new TLatex();
	text->SetNDC();
	text->SetTextSize(0.044);
	
	//----- OFFSETS ---------------------------------------------------------------------
	//
	float DZDoffset=0.0;
	float DZDoffsetA=0.0,DZDoffsetB=0.0,DZDoffsetC=0.0;
	//
// 	if        (iRhicRun==10 && iEnergy==200){
// 		if (iChoice==0||iChoice==2){ DZDoffsetA=  1.0; DZDoffsetB=  3.0; } else
// 		if (iChoice==1||iChoice==3){ DZDoffsetA= 12.0; DZDoffsetB= 11.0; } 
// 	} else if (iRhicRun==10 && iEnergy== 62){
// 		if (iChoice==0||iChoice==2){ DZDoffsetA=  1.0; DZDoffsetB=  2.0; } else
// 		if (iChoice==1||iChoice==3){ DZDoffsetA= 43.0; DZDoffsetB= 43.0; } 
// 	} else if (iRhicRun==10 && iEnergy== 39){
// 		if (iChoice==0||iChoice==2){ DZDoffsetA=  0.0; DZDoffsetB=  1.0; } else
// 		if (iChoice==1||iChoice==3){ DZDoffsetA= 43.0; DZDoffsetB= 43.0; } 
// 	} else if (iRhicRun==10 && iEnergy==  7){
// 		if (iChoice==0||iChoice==2){ DZDoffsetA=  0.0; DZDoffsetB=  2.0; } else
// 		if (iChoice==1||iChoice==3){ DZDoffsetA= 43.0; DZDoffsetB= 43.0; } 
// 	//
// 	} else if (iRhicRun==11 && iEnergy== 19){
// 		if (iChoice==0||iChoice==2){ DZDoffsetA=  7.0; DZDoffsetB=  7.5; } else
// 		if (iChoice==1||iChoice==3){ DZDoffsetA=-33.0; DZDoffsetB=-29.0; } 
// 	//
// 	} else if (iRhicRun==13 && iEnergy==500){
// 		if (iChoice==0||iChoice==2){ DZDoffsetA= -2.5; DZDoffsetB= -2.5; } else
// 		if (iChoice==1||iChoice==3){ DZDoffsetA= -3.5; DZDoffsetB= -3.5; } 
// 	//
// 	} else if (iRhicRun==14 && iEnergy==200){
// 		if (iTrgCrate==0){				// BBQ
// 			if (iChoice==0||iChoice==2){ DZDoffsetA=  1.0; DZDoffsetB=  3.0; DZDoffsetC=  3.0; } else
// 			if (iChoice==1||iChoice==3){ DZDoffsetA= 14.0; DZDoffsetB= 13.9; DZDoffsetC= 12.0; } 
// 		} else if (iTrgCrate==1){		// MXQ
// 			if (iChoice==0||iChoice==2){ DZDoffsetA=  1.0; DZDoffsetB=  2.0; DZDoffsetC=  2.0; } else
// 			if (iChoice==1||iChoice==3){ DZDoffsetA= 58.9; DZDoffsetB= 56.9; DZDoffsetC= 55.1; } 
// 		}
// 	}
//	cout<<".... Loop      DZD offset A = "<<DZDoffsetA<<endl;
//	cout<<".... Loop      DZD offset B = "<<DZDoffsetB<<endl;
//	cout<<".... Loop      DZD offset C = "<<DZDoffsetC<<endl;
	//
	Float_t	DTOUTLIERCUTlower ,DTOUTLIERCUTupper ;
	Float_t	DTOUTLIERCUTlower0,DTOUTLIERCUTupper0;
	Float_t	DTOUTLIERCUTlower1,DTOUTLIERCUTupper1;
	Float_t	DTOUTLIERCUTlower2,DTOUTLIERCUTupper2;
	DTOUTLIERCUTlower   = -10.0;
	DTOUTLIERCUTupper   =  10.0;
	DTOUTLIERCUTlower0  = -10.0;
	DTOUTLIERCUTupper0  =  10.0;
	DTOUTLIERCUTlower1  = -10.0;
	DTOUTLIERCUTupper1  =  10.0;
	DTOUTLIERCUTlower2  = -10.0;
	DTOUTLIERCUTupper2  =  10.0;	
// 	if (iChoice==0||iChoice==2){
// 		DTOUTLIERCUTlower0  =  -2.0;
// 		DTOUTLIERCUTupper0  =   3.0;
// 		DTOUTLIERCUTlower1  =  -1.0;
// 		DTOUTLIERCUTupper1  =   1.5;
// 		DTOUTLIERCUTlower2  =  -0.6;
// 		DTOUTLIERCUTupper2  =   1.0;	
// 	} else if (iChoice==1||iChoice==3||iChoice==5){
// 		DTOUTLIERCUTlower0  =  -4.0;
// 		DTOUTLIERCUTupper0  =   3.0;
// 		DTOUTLIERCUTlower1  =  -3.0;
// 		DTOUTLIERCUTupper1  =   2.0;
// 		DTOUTLIERCUTlower2  =  -1.5;
// 		DTOUTLIERCUTupper2  =   1.0;	
// 	}
	if (iAlgorithm==2){
		if (iChoice==0||iChoice==2){
			DTOUTLIERCUTlower0  =  -2.0;
			DTOUTLIERCUTupper0  =   2.0;
			DTOUTLIERCUTlower1  =  -1.0;
			DTOUTLIERCUTupper1  =   1.0;
			DTOUTLIERCUTlower2  =  -0.6;
			DTOUTLIERCUTupper2  =   0.6;	
		} else if (iChoice==1||iChoice==3||iChoice==5){
			DTOUTLIERCUTlower0  =  -3.0;
			DTOUTLIERCUTupper0  =   3.0;
			DTOUTLIERCUTlower1  =  -2.0;
			DTOUTLIERCUTupper1  =   2.0;
			DTOUTLIERCUTlower2  =  -1.0;
			DTOUTLIERCUTupper2  =   1.0;	
		}
	}
	//
	Float_t 	toffsets[100];
	ULong64_t	itimemincheck=-1;
	for (int i=0;i<100;i++){ toffsets[i] = -99999; }
	//
	for (Long64_t jentry=0; jentry<nentriestree;jentry++) {
		//
		Long64_t ientry = LoadTree(jentry);
		if (ientry < 0) break;
		nb = fChain->GetEntry(jentry);
		if (jentry%100000==0){cout	<<"... Offsets     ...Processing "<<jentry
									<<"  kChanKeyOffsets="<<kChanKeyOffsets<<endl;}
		//
		if (itimemincheck==   -1){ itimemincheck	= itime; } else
		if (itimemincheck> itime){ itimemincheck	= itime; } 
//		if (nprimary<Nprimarycut){ continue; }
//		if (grefmult<refmultcut){ continue; }
//		if (TMath::Abs(vtx[2])>5.0){ continue; }				// restrict to Zvtx=0 for initial offsets!
//		if (TMath::Sqrt(vtx[0]*vtx[0]+vtx[1]*vtx[1])>1.5){ continue; }
		//
		if (T[kChanKeyOffsets]> T_min && T[kChanKeyOffsets]<=T_max && A[kChanKeyOffsets]> A_min){
			for (int is=0;is<2;is++){
				for (int idet=0;idet<NDETSIDE;idet++){
					idetew 		= idet + is*NDETSIDE;
					if (T[idetew]> T_min && T[idetew]<=T_max && A[idetew]> A_min ){
						//
						//---- initial offsets.....
						hinitialoffsets->Fill(idetew,TACtoNS*(T[idetew]-T[kChanKeyOffsets]),1.0);
						if (idet+is*NDETSIDE != kChanKeyOffsets){
							hinitialoffsetsEW[is]->Fill(TACtoNS*(T[idetew]-T[kChanKeyOffsets]),1.0);
						}
						//
						//---- ADC vs ToT.....
						if (iChoice==0||iChoice==3){			// A is TOT, Aother is ADC....
							adctot[idet][is]->Fill(A[idetew],Aother[idetew],1.);
							hadc[idet][is]->Fill(Aother[idetew],1.);
							htot[idet][is]->Fill(A[idetew],1.);
						} else if (iChoice==1||iChoice==2){		// A is ADC, Aother is TOT....
							adctot[idet][is]->Fill(Aother[idetew],A[idetew],1.);
							hadc[idet][is]->Fill(A[idetew],1.);
							htot[idet][is]->Fill(Aother[idetew],1.);
						}
						//
					}
				}
			}
		}
		//
		//---- coarse counter for vpd... (needed for FO data)....
		for (int is=0;is<2;is++){
			for (int idet=0;idet<19;idet++){
				idetew 	= idet + is*19;
				if (vpd_cco[idetew]!=0){
					if (is==0){ hcoco_vpd_w->Fill(vpd_cco[idetew],1.0); } else
					if (is==1){ hcoco_vpd_e->Fill(vpd_cco[idetew],1.0); }
				}
			}
		}
		//---- coarse counter for p2p... (needed for FO data)....
		//for (int is=0;is<2;is++){
		//	for (int idet=0;idet<8;idet++){
		//		idetew 	= idet + is*8;
		//		if (p2p_cco[idetew]!=0){
		//			if (is==0){ hcoco_p2p_w->Fill(p2p_cco[idetew],1.0); } else
		//			if (is==1){ hcoco_p2p_e->Fill(p2p_cco[idetew],1.0); }
		//		}
		//	}
		//}
		//
	}	//----- end of event loop....
	//
	TProfile *hpinitialoffsets = (TProfile*)hinitialoffsets->ProfileX();
	int 	ibin;
	float	val;
	for (int is=0;is<2;is++){
		for (int idet=0;idet<NDETSIDE;idet++){
			idetew				= idet + is*NDETSIDE;
			ibin				= idetew + 1;
			TH1D *hoffsetstemp	= (TH1D*)hinitialoffsets->ProjectionY("hoffsetstemp",ibin,ibin,"");
			if (hoffsetstemp->GetEntries()<100){
				delete hoffsetstemp;
				continue;
			}
			//val	 			= hpinitialoffsets->GetBinContent(ibin);
			//---- take offset as center of highest bin
			//ibin				= hoffsetstemp->GetMaximumBin();
			//val				= hoffsetstemp->GetBinCenter(ibin);
			//---- take offset as hist mean value
			//val					= hoffsetstemp->GetMean();
			//---- take offset as fit around center of highest bin
			ibin				= hoffsetstemp->GetMaximumBin();
			val					= hoffsetstemp->GetBinCenter(ibin);
			TF1 *thisgaus	= new TF1("thisgaus","gaus",val-10,val+10);
			hoffsetstemp->Fit("thisgaus","Q0NR");
			val				= thisgaus->GetParameter(1);
			delete thisgaus;
			//
			toffsets[idetew]	= val;
			hinitialoffsetvals->Fill(idetew,toffsets[idetew]);
			if (is==0){
				cout<<"... Offset Values "<<idetew<<" "<<val<<endl;
			} else {
				cout<<"... Offset Values "<<idetew<<" "<<val<<" "<<val-toffsets[idetew-NDETSIDE]<<endl;
			}
			delete hoffsetstemp;
		}
	}
	//
	float trel0[2] = {0};
	for (int is=0;is<2;is++){
		trel0[is]	= FindPeak(hinitialoffsetsEW[is]);
		cout<<"averaged ... is="<<is<<" \t  tmean="<<hinitialoffsetsEW[is]->GetMean()<<"  tpeak="<<trel0[is]<<endl;
	}	
//	if (iChoice==0||iChoice==2){
		DZDoffset = 0.0;									//!!!!!!!!!!!!!!!!!!!!!!!
//	} else if (iChoice==1||iChoice==3||iChoice==5){
//		DZDoffset = (trel0[0]-trel0[1])*29.98/2.;			//!!!!!!!!!!!!!!!!!!!!!!!
//	}
	cout<<"DZDoffset = "<<DZDoffset<<endl;
	//
	ipad = 0;
	gStyle->SetOptStat(0);
	++ncan;
	sprintf(buf,"ccan%d",ncan);
	ccan[ncan] = new TCanvas(buf,buf,0,0,800,600);
	ccan[ncan]->SetFillColor(10);
	ccan[ncan]->cd(); //ccan[ncan]->Divide(1,1,0.0001,0.0001);
	sprintf(buf,"ps/%s.ps",outfilebase);						//
	cout<<".... Loop   psfile = "<<buf<<endl; 
	psout = new TPostScript(buf,112);							//
	psout->Range(26,20);										//
		ccan[ncan]->cd(1);
			hinitialoffsets->Draw();
			hinitialoffsetvals->SetMarkerStyle(24);
			hinitialoffsetvals->SetMarkerColor(2);
			hinitialoffsetvals->Draw("P same");
		sprintf(buf,"Days used here: %d-%d",iDayMin,iDayMax);
		text->DrawLatex(0.19,0.82,buf);
	ccan[ncan]->cd();
	ccan[ncan]->Update();
	//
	gStyle->SetOptStat(1111);
	++ncan;
	psout->NewPage();
	sprintf(buf,"ccan%d",ncan);
	ccan[ncan] = new TCanvas(buf,buf,0,0,800,600);
	ccan[ncan]->SetFillColor(10);
	ccan[ncan]->cd(); ccan[ncan]->Divide(2,1,0.0001,0.0001);
		ccan[ncan]->cd(1);
			hinitialoffsetsEW[0]->Draw();
		ccan[ncan]->cd(2);
			hinitialoffsetsEW[1]->Draw();
	ccan[ncan]->Update();
	gStyle->SetOptStat(0);

	cout<<"itimemin (set in *_utils) = "<<itimemin<<endl;
	cout<<"itimemincheck             = "<<itimemincheck<<endl;
	if (itimemincheck != itimemin){ cout<<"\a \a"<<endl; }
	
	//----- ITERATIONS -----------------------------------------------------------------
	//
	//----- start iteration loop....
	for (int iter=0;iter<NITERATIONS;iter++){	
		ipass		= iter;
		keyCh		= -1;			// no longer used...
		LastIter	= kFALSE;
		FirstIter	= kFALSE;
		if (iter==0            ){ FirstIter	= kTRUE; }
		if (iter==NITERATIONS-1){ LastIter	= kTRUE; }
		//
//		if (iter==0){ DZDoffset=DZDoffsetA; } else
//		if (iter==1){ DZDoffset=DZDoffsetB; } else
//		if (iter>=2){ DZDoffset=DZDoffsetC; }
		if (iter==0){ DZDoffsetA=DZDoffset; } else
		if (iter==1){ DZDoffsetB=DZDoffset; } else
		if (iter>=2){ DZDoffsetC=DZDoffset; }
		//
		if (iter==0){ DTOUTLIERCUTlower=DTOUTLIERCUTlower0; DTOUTLIERCUTupper=DTOUTLIERCUTupper0; } else
		if (iter==1){ DTOUTLIERCUTlower=DTOUTLIERCUTlower1; DTOUTLIERCUTupper=DTOUTLIERCUTupper1; } else
		if (iter>=2){ DTOUTLIERCUTlower=DTOUTLIERCUTlower2; DTOUTLIERCUTupper=DTOUTLIERCUTupper2; }
		//
		//--------------------------------------------------------------------------
		//
		for (Long64_t jentry=0; jentry<nentries;jentry++) {
			Long64_t ientry = LoadTree(jentry);
			if (ientry < 0) break;
			nb = fChain->GetEntry(jentry);
			int year	= run/1000000;
			int day		= (run - (year*1000000))/1000;
			if (jentry%250000==0){
				cout<<"... Iteration="<<iter<<" ...keyCh="<<keyCh<<" ...Processing "<<jentry
					<<"\t day="<<day<<"  relday="<<day-iDayMin
					<<"\t DTOUTLIERCUTs ="<<DTOUTLIERCUTlower<<" "<<DTOUTLIERCUTupper<<endl;
			}
			//
			if (nprimary<Nprimarycut){ continue; }
			if (TMath::Abs(vtx[2])>ZVertexMax){ continue; }
			if (grefmult<refmultcut){ continue; }
			float vtxx	= vtx[0] - vtx_spot_x;
			float vtxy	= vtx[1] - vtx_spot_y;
			if (TMath::Sqrt(vtxx*vtxx+vtxy*vtxy)>1.5){ continue; }
			//
			std::bitset<BIT_LENGTH> b = daqbits;			// decode daqbits
			if (iter==0){
				for (int ibit=0;ibit<BIT_LENGTH;ibit++){
					if (b[ibit]){ hnev_daqbits->Fill(ibit,1.0); }
				}
				hnev_day->Fill(day,1.);						// number of events/day
			}
			//
			//---- IDENTIFY USABLE CHANNELS.......................................
			//
			Egood		= kFALSE;
			Wgood		= kFALSE;
			for (int is=0;is<2;is++){
				newraw[is] 	= 0; 	
				newe[is]  	= 0; 	
				for (int idet=0;idet<NDETSIDE;idet++){
					idetew				= idet + is*NDETSIDE;
					useDet[idet][is]	= kFALSE;
					useDetZV[idet][is]	= kFALSE;
					if ( T[idetew]        >  T_min 
					  && T[idetew]        <= T_max 
					  && A[idetew]        >  A_min
					  && toffsets[idetew] > -9000 ){ 
						++newraw[is];
						useDet[idet][is]	= kTRUE; 
						if (is==0){ Wgood	= kTRUE; } else
						if (is==1){ Egood	= kTRUE; } 
					}
				}	//---- end idet loop...
			}	//---- end iside loop....
			//
//			if (  useDet[keyCh][0]){ hcnts[0]->Fill(iter,1.); }
//			if (  useDet[keyCh][1]){ hcnts[1]->Fill(iter,1.); }
//			if ( !useDet[keyCh][0] && !useDet[keyCh][1] && !LastIter ){
//				continue;
//			}			
			//
			//---- begin iAlgorithm-based outlier rejection ......................................
			//
			if (iAlgorithm==1){
				for (int is=0;is<2;is++){
					for (int idet=0;idet<NDETSIDE;idet++){
						if (useDet[idet][is]){
							useDetZV[idet][is]	= kTRUE;
						}
					}
				}
			} else if (iAlgorithm==2){
				n_valid_pairs	= 0;
				if (vtxe[2]>0.0 && vtxe[2]<Zerrorcut) {
					//
					for (idetw=0;idetw<NDETSIDE;idetw++){
						for (idete=0;idete<NDETSIDE;idete++){
							if (useDet[idetw][0] && useDet[idete][1]){
								tw		= TACtoNS*T[idetw]          - toffsets[idetw];
								te		= TACtoNS*T[idete+NDETSIDE] - toffsets[idete+NDETSIDE];
								if (iChoice==0||iChoice==2){
									dt		= te - tw;
								} else if (iChoice==1||iChoice==3||iChoice==5){
									dt		= tw - te;
								}
								zvt		= (29.98/2.)*dt;
								//
								if (iter==0){
									if (newraw[0]==1 && newraw[1]==1){
										//cout<<newraw[0]<<" "<<newraw[1]<<" "<<vtx[2]<<" "<<dt<<" "<<zvt<<endl;
										hdtzv_11->Fill(zvt-vtx[2],1.0);
										if (idetw==0){ hdtzv_11e->Fill(idete,zvt-vtx[2],1.0); }
										if (idete==0){ hdtzv_11w->Fill(idetw,zvt-vtx[2],1.0); }
									}
									if ( TMath::Abs(vtx[2])<5.0 
									  && TMath::Abs(zvt-vtx[2])<100.0 ){
										hdzd->Fill(idetw,idete,zvt-vtx[2]);
										hdzd3D->Fill(idetw,idete,zvt-vtx[2]);
										//if (idetw==0&&idete==0){ cout<<zvt<<" "<<vtx[2]<<endl; }
									}
									//
 									hddzvzt_A_all->Fill(zvt-vtx[2]-DZDoffsetA); 
 									hddzvzt_A_evt->Fill(jentry,zvt-vtx[2]-DZDoffsetA); 
 									hddzvzt_A_day->Fill(day,zvt-vtx[2]-DZDoffsetA); 
									//
								}
								//
								if (iter==0){ if (idetw==0&&idete==0){ hdtzv_nn_00->Fill(vtx[2],zvt,1.0); hdtzv1D_nn_00->Fill(zvt-vtx[2],1.0); } }
								if (iter==1){ if (idetw==1&&idete==1){ hdtzv_nn_11->Fill(vtx[2],zvt,1.0); hdtzv1D_nn_11->Fill(zvt-vtx[2],1.0); } }
								if (iter==2){ if (idetw==2&&idete==2){ hdtzv_nn_22->Fill(vtx[2],zvt,1.0); hdtzv1D_nn_22->Fill(zvt-vtx[2],1.0); } }
								if (iter==3){ if (idetw==3&&idete==3){ hdtzv_nn_33->Fill(vtx[2],zvt,1.0); hdtzv1D_nn_33->Fill(zvt-vtx[2],1.0); } }
								//
								//---- find valid channels....
								if ( ((zvt-vtx[2]-DZDoffsetA)>=Zdiffcut_l) 
								  && ((zvt-vtx[2]-DZDoffsetA)<=Zdiffcut_u) ){
									if (iter==0){ hddzvzt_A_keep->Fill(zvt-vtx[2]-DZDoffsetA); }
									kk					= n_valid_pairs;
									valid_pairs[0][kk]	= idetw;
									valid_pairs[1][kk]	= idete;
									++n_valid_pairs;
									//
									if (iter==0){ if (idetw==0&&idete==0){ hdtzv_nn_00_keep->Fill(vtx[2],zvt,1.0); hdtzv1D_nn_00_keep->Fill(zvt-vtx[2],1.0); } }
									if (iter==1){ if (idetw==1&&idete==1){ hdtzv_nn_11_keep->Fill(vtx[2],zvt,1.0); hdtzv1D_nn_11_keep->Fill(zvt-vtx[2],1.0); } }
									if (iter==2){ if (idetw==2&&idete==2){ hdtzv_nn_22_keep->Fill(vtx[2],zvt,1.0); hdtzv1D_nn_22_keep->Fill(zvt-vtx[2],1.0); } }
									if (iter==3){ if (idetw==3&&idete==3){ hdtzv_nn_33_keep->Fill(vtx[2],zvt,1.0); hdtzv1D_nn_33_keep->Fill(zvt-vtx[2],1.0); } }
									//
								}
								//
							}
						}				
					}
				}
				//
				//cout<<n_valid_pairs<<" "<<newraw[0]<<" "<<newraw[1]<<endl;
				if (newraw[0]>0 && newraw[1]>0){
					hnvalidpairs->Fill(newraw[0]*newraw[1],n_valid_pairs,1.0);
					//cout <<newraw[0]<<" "<<newraw[1]<<" "<<n_valid_pairs<<endl;
				}
				//
				//if (n_valid_pairs==0){ continue; }
				//
				Wgood = Egood = kFALSE;
				for (int idet=0;idet<NDETSIDE;idet++){
					useDetZV[idet][0]	= kFALSE;
					useDetZV[idet][1]	= kFALSE;
				}
				for (int ip=0;ip<n_valid_pairs;ip++){
					idetw				= valid_pairs[0][ip];
					idete				= valid_pairs[1][ip];
					useDetZV[idetw][0]	= kTRUE;
					useDetZV[idete][1]	= kTRUE;
					Wgood = Egood 		= kTRUE;
				}
			}	
//			if (  useDetZV[keyCh][0]){ hcnts[2]->Fill(iter,1.); }
//			if (  useDetZV[keyCh][1]){ hcnts[3]->Fill(iter,1.); }
//			if ( !useDetZV[keyCh][0] && !useDetZV[keyCh][1] && !LastIter ){
//				continue;
//			}			
			//
			//---- end iAlgorithm-based outlier rejection section .............................
			//
			for (int is=0;is<2;is++){
				for (int idet=0;idet<NDETSIDE;idet++){
					if (useDetZV[idet][is]){ 
						++newe[is]; 
					}
				}
			}
			//
			//---- CALIBRATION -------------------------------------------------
			//
//			if (Wgood){ hcnts[4]->Fill(iter,1.); }
//			if (Egood){ hcnts[5]->Fill(iter,1.); }
//			if (Wgood&&Egood){ hcnts[6]->Fill(iter,1.); }
			//
			for (int is=0;is<2;is++){
				//
//				if (keyCtrLit[is]){	hcnts[7+is]->Fill(iter,1.); }
//				if (newe[is]>0){ hcnts[9+is]->Fill(iter,1.); }				
//				if (keyCtrLit[is]&&newe[is]>0){	hcnts[11+is]->Fill(iter,1.); }
//				if (Wgood&&Egood&&keyCtrLit[is]&&newe[is]>0){ hcnts[13+is]->Fill(iter,1.); }
				//
				//----- rebuild history ..................................................................
				//
				if (Wgood && Egood){ 		// require 1.and.1 coincidence... (possibly unnecessary depending on input source)
					//
					//---- initial offsets....
					for (int idet=0;idet<NDETSIDE;idet++){
						idetew	= idet + is*NDETSIDE;
						if (useDetZV[idet][is]){
							tsofar[idet][is]	= TACtoNS*T[idetew] - toffsets[idetew];
						} else {
							tsofar[idet][is]	= 0.0;
						}
					}
					//----- calibration functions from previous passes...
					if (iter>0){
						for (int jter=0;jter<=iter-1;jter++){
							for (int idet=0;idet<NDETSIDE;idet++){
								idetew	= idet + is*NDETSIDE;
								if (useDetZV[idet][is]){
									valcor			 = ff[idet+jter*NDETSIDE][is]->V(A[idetew]);
									tsofar[idet][is] = tsofar[idet][is] - valcor;
								}
							}
						}
					}
				}
			}
			//
			//---- ITER>0 IDENTIFY USABLE TIMES AGAIN....................................
			//
			if (iter>=1 && iAlgorithm==2){
				//
				Egood			= kFALSE;
				Wgood			= kFALSE;
				n_valid_pairsb	= 0;
				if (vtxe[2]>0.0 && vtxe[2]<Zerrorcut) {
					//
					for (idetw=0;idetw<NDETSIDE;idetw++){
						for (idete=0;idete<NDETSIDE;idete++){
							if (useDetZV[idetw][0] && useDetZV[idete][1]){
								tw		= tsofar[idetw][0];
								te		= tsofar[idete][1];
									//=================================
									if (iter==1 && useDetZV[idetw][0] && tw==0.0 ){
										cout<<" HUH 2 W !?! "<<ientry<<" "<<idetw<<" "<<tsofar[idetw][0]<<endl;
									}
									if (iter==1 && useDetZV[idete][1] && te==0.0 ){
										cout<<" HUH 2 E !?! "<<ientry<<" "<<idete<<" "<<tsofar[idete][1]<<endl;
									}				
									//=================================
								if (iChoice==0||iChoice==2){
									dt		= te - tw;
								} else if (iChoice==1||iChoice==3||iChoice==5){
									dt		= tw - te;
								}
								zvt		= (29.98/2.)*dt;
								//
								if (iter==1){
									if (idetw==0&&idete==0){ hdtzvb_nn_00->Fill(vtx[2],zvt,1.0); hdtzvb1D_nn_00->Fill(zvt-vtx[2],1.0); } 
									if (idetw==1&&idete==1){ hdtzvb_nn_11->Fill(vtx[2],zvt,1.0); hdtzvb1D_nn_11->Fill(zvt-vtx[2],1.0); } 
									if (idetw==2&&idete==2){ hdtzvb_nn_22->Fill(vtx[2],zvt,1.0); hdtzvb1D_nn_22->Fill(zvt-vtx[2],1.0); } 
									if (idetw==3&&idete==3){ hdtzvb_nn_33->Fill(vtx[2],zvt,1.0); hdtzvb1D_nn_33->Fill(zvt-vtx[2],1.0); } 
 									hddzvzt_B_all->Fill(zvt-vtx[2]-DZDoffsetB); 
 									hddzvzt_B_evt->Fill(jentry,zvt-vtx[2]-DZDoffsetB); 
 									hddzvzt_B_day->Fill(day,zvt-vtx[2]-DZDoffsetB); 
								}
								if (iter==2){
									if (idetw==0&&idete==0){ hdtzvc_nn_00->Fill(vtx[2],zvt,1.0); hdtzvb1D_nn_00_keep->Fill(zvt-vtx[2],1.0); } 
									if (idetw==1&&idete==1){ hdtzvc_nn_11->Fill(vtx[2],zvt,1.0); hdtzvb1D_nn_11_keep->Fill(zvt-vtx[2],1.0); } 
									if (idetw==2&&idete==2){ hdtzvc_nn_22->Fill(vtx[2],zvt,1.0); hdtzvb1D_nn_22_keep->Fill(zvt-vtx[2],1.0); } 
									if (idetw==3&&idete==3){ hdtzvc_nn_33->Fill(vtx[2],zvt,1.0); hdtzvb1D_nn_33_keep->Fill(zvt-vtx[2],1.0); } 
 									hddzvzt_C_all->Fill(zvt-vtx[2]-DZDoffsetC); 
 									hddzvzt_C_evt->Fill(jentry,zvt-vtx[2]-DZDoffsetC); 
 									hddzvzt_C_day->Fill(day,zvt-vtx[2]-DZDoffsetC); 
								}
								//
								if ( (iter==1 && ( (zvt-vtx[2]-DZDoffsetB)>=Zdiffcutb_l&&(zvt-vtx[2]-DZDoffsetA)<=Zdiffcutb_u) )
								 ||  (iter> 1 && ( (zvt-vtx[2]-DZDoffsetC)>=Zdiffcutc_l&&(zvt-vtx[2]-DZDoffsetA)<=Zdiffcutc_u) ) ){
								 	if (iter==1){ hddzvzt_B_keep->Fill(zvt-vtx[2]-DZDoffsetB); }
									if (iter==2){ hddzvzt_C_keep->Fill(zvt-vtx[2]-DZDoffsetC); }
									kk					= n_valid_pairsb;
									valid_pairsb[0][kk]	= idetw;
									valid_pairsb[1][kk]	= idete;
									++n_valid_pairsb;
									if (iter==1){
										if (idetw==0&&idete==0){ hdtzvb_nn_00_keep->Fill(vtx[2],zvt,1.0); hdtzvc1D_nn_00->Fill(zvt-vtx[2],1.0); } 
										if (idetw==1&&idete==1){ hdtzvb_nn_11_keep->Fill(vtx[2],zvt,1.0); hdtzvc1D_nn_11->Fill(zvt-vtx[2],1.0); } 
										if (idetw==2&&idete==2){ hdtzvb_nn_22_keep->Fill(vtx[2],zvt,1.0); hdtzvc1D_nn_22->Fill(zvt-vtx[2],1.0); } 
										if (idetw==3&&idete==3){ hdtzvb_nn_33_keep->Fill(vtx[2],zvt,1.0); hdtzvc1D_nn_33->Fill(zvt-vtx[2],1.0); } 
									}
									if (iter==2){
										if (idetw==0&&idete==0){ hdtzvc_nn_00_keep->Fill(vtx[2],zvt,1.0); hdtzvc1D_nn_00_keep->Fill(zvt-vtx[2],1.0); } 
										if (idetw==1&&idete==1){ hdtzvc_nn_11_keep->Fill(vtx[2],zvt,1.0); hdtzvc1D_nn_11_keep->Fill(zvt-vtx[2],1.0); } 
										if (idetw==2&&idete==2){ hdtzvc_nn_22_keep->Fill(vtx[2],zvt,1.0); hdtzvc1D_nn_22_keep->Fill(zvt-vtx[2],1.0); } 
										if (idetw==3&&idete==3){ hdtzvc_nn_33_keep->Fill(vtx[2],zvt,1.0); hdtzvc1D_nn_33_keep->Fill(zvt-vtx[2],1.0); } 
									}
								}
								//
							}
						}				
					}
				}
				//
				//cout<<n_valid_pairsb<<" "<<newe[0]<<" "<<newe[1]<<endl;
				if (newe[0]>0 && newe[1]>0){
					hnvalidpairsb->Fill(newe[0]*newe[1],n_valid_pairsb,1.0);
					//cout <<newe[0]<<" "<<newe[1]<<" "<<n_valid_pairsb<<endl;
				}
				//
				//if (n_valid_pairsb==0){ continue; }
				//
				for (int idet=0;idet<NDETSIDE;idet++){
					useDetZV[idet][0]	= kFALSE;
					useDetZV[idet][1]	= kFALSE;
				}
				for (int ip=0;ip<n_valid_pairsb;ip++){
					idetw	= valid_pairsb[0][ip];
					idete	= valid_pairsb[1][ip];
					useDetZV[idetw][0]	= kTRUE;
					useDetZV[idete][1]	= kTRUE;
					Wgood	= kTRUE;
					Egood	= kTRUE;
				}
				//
				for (int is=0;is<2;is++){
					newe[is]		= 0;
					for (int idet=0;idet<NDETSIDE;idet++){
						if (useDetZV[idet][is]){ 
							++newe[is]; 
						}
					}
				}
			}	//---- end iter>=1 check....
			//
			for (int is=0;is<2;is++){			
				if (iter==0){ hnvpdrm[iter][is]->Fill(grefmult,newraw[is]); } else
				if (iter> 0){ hnvpdrm[iter][is]->Fill(grefmult,newe[is]  ); }
			}
			//
			//---- END ITER>0 IDENTIFY USABLE TIMES AGAIN....................................
			//
//			if (Wgood){ hcnts[15]->Fill(iter,1.); }
//			if (Egood){ hcnts[16]->Fill(iter,1.); }
//			if (Wgood&&Egood){ hcnts[17]->Fill(iter,1.); }
			//
			//---- START FILLING....................................
			//
			for (int is=0;is<2;is++){
				for (int idet=0;idet<NDETSIDE;idet++){
					//
					neweu[is] 	= 0; 	
					tavgu[is] 	= 0.0;
					if (!useDetZV[idet][is]){ continue; }
					//
	//				if (keyCtrLit[is]){	hcnts[18+is]->Fill(iter,1.); }
	//				if (newe[is]>0){ hcnts[20+is]->Fill(iter,1.); }				
	//				if (keyCtrLit[is]&&newe[is]>0){	hcnts[22+is]->Fill(iter,1.); }
	//				if (Wgood&&Egood&&keyCtrLit[is]&&newe[is]>0){ hcnts[24+is]->Fill(iter,1.); }
					//
					if ( Wgood && Egood ){
						//
						//---- calculate <N> for jdet != idet....
						ntmp = 0; ttmp = 0.0;
						for (int jdet=0;jdet<NDETSIDE;jdet++){
							if (useDetZV[jdet][is] && jdet!=idet){
								++ntmp; 
								ttmp	+= tsofar[jdet][is];
							}
						}
						if(ntmp>0){ ttmp /= (Float_t)ntmp; }							// this is "<N>" w/out the asymmetric cut...
						     else { ttmp  = -9999; }
						//
						//----- look at channel variation inside the <N> calculation
						if (ntmp>0){
							for (int jdet=0;jdet<NDETSIDE;jdet++){
								if (useDetZV[jdet][is] && jdet!=idet){
									h1mn_n[iter][idet][is]->Fill(ntmp,tsofar[jdet][is]-ttmp);
								}
							}
						}
						//
						//----- recalculate avg time on the side using updated times with asymmetric cut...
						for (int jdet=0;jdet<NDETSIDE;jdet++){
							if (useDetZV[jdet][is] && jdet!=idet){
								if (ttmp>-9990){
									if (tsofar[jdet][is]-ttmp>DTOUTLIERCUTlower
									 && tsofar[jdet][is]-ttmp<DTOUTLIERCUTupper){	
										++neweu[is]; 
										tavgu[is]	+= tsofar[jdet][is];
									}
								}
							}
						}
						if(neweu[is]){ tavgu[is] /= (Float_t)neweu[is]; }	// this is "<N>" w/ the asymmetric cut...
						        else { tavgu[is]  = -9999; }
						//
						//----- fill fit plots...
						//
						idetew	= idet + is*NDETSIDE;
						hAiter[is]->Fill(iter,A[idetew],1.0);
						//
						//--- THE MONEY CUT IS HERE
						if( (NREQUIRE>0 && neweu[is]>=  NREQUIRE) 		//!!!!!!!!!!!!!!!!!
						 || (NREQUIRE<0 && neweu[is]== -NREQUIRE) ){	//!!!!!!!!!!!!!!!!!
						 	if (tavgu[is]>-9990){
//								hcnts[26+is]->Fill(iter,1.); 
								hdt_A[idet+iter*NDETSIDE][is]->Fill(A[idetew], tsofar[idet][is]-tavgu[is] );
								hdt_A_1D[idet+iter*NDETSIDE][is]->Fill(tsofar[idet][is]-tavgu[is]);
							}
						}
						//if (ipass==0){
						//	  htavPmtRel->Fill(keyCh,tearliest[keyCh]-tavgw);
						//	htavPmtRelsh->Fill(keyCh,tearliest[keyCh]-tavgw-PMTdtoffset[keyCh]);
						//}
						//
						//----- fill end of iteration plots...
	 					if (LastIter && tavgu[is]>-9990){
							//
  							hdt_end[idet][is]->Fill(tsofar[idet][is]-tavgu[is]);
  							hdt_end_n[idet][is]->Fill(neweu[is],tsofar[idet][is]-tavgu[is]);
  							//
							if (tavgu[is]>=LLa){
								hdt_end_chan_nLLa->Fill(idet+is*NDETSIDE,tsofar[idet][is]-tavgu[is]);
								if (tavgu[is]>=LLb){
									hdt_end_chan_nLLb->Fill(idet+is*NDETSIDE,tsofar[idet][is]-tavgu[is]);
								}
							}
							idt		= day - iDayMin;
							idtime	= (itime - itimemin)/4/60/60;
							if ((idt>=0    && idt<(iDayMax-iDayMin+1))
							 && (idtime>=0 && idtime<=dtimenbins     )){
								if (idet==0){
									hdt_end_n_det0_day[idt][is]->Fill(neweu[is],tsofar[idet][is]-tavgu[is]);
									hdt_end_n_det0_time[idtime][is]->Fill(neweu[is],tsofar[idet][is]-tavgu[is]);
									if (neweu[is]>=6){
										hdt_end_det0_zvtx[is]->Fill(vtx[2],tsofar[idet][is]-tavgu[is]);
										hdt_end_det0_grefmult[is]->Fill(grefmult,tsofar[idet][is]-tavgu[is]);
									}
								}else if (idet==1){
									hdt_end_n_det1_day[idt][is]->Fill(neweu[is],tsofar[idet][is]-tavgu[is]);
									hdt_end_n_det1_time[idtime][is]->Fill(neweu[is],tsofar[idet][is]-tavgu[is]);
								}else if (idet==2){
									hdt_end_n_det2_day[idt][is]->Fill(neweu[is],tsofar[idet][is]-tavgu[is]);
									hdt_end_n_det2_time[idtime][is]->Fill(neweu[is],tsofar[idet][is]-tavgu[is]);
								}else if (idet==3){
									hdt_end_n_det3_day[idt][is]->Fill(neweu[is],tsofar[idet][is]-tavgu[is]);
									hdt_end_n_det3_time[idtime][is]->Fill(neweu[is],tsofar[idet][is]-tavgu[is]);
								}
							}
  							//
	 					}	//---- end lass iter check...
						//
					}	//----- end E/Wgood check....
					//
				}	//---- end idet loop.....
				//
			}	//----- end iside loop.....
			//
			if (LastIter){
				//
				bool goodside[2]		= {0};
				EWcoincidence			= false;
				float twest[100]		= {0};
				float teast[100]		= {0};
				float twestsort[100]	= {0};
				float teastsort[100]	= {0};
				float latetime;
				if (iChoice==0||iChoice==2            ){ latetime = 9999999.; }
				if (iChoice==1||iChoice==3||iChoice==5){ latetime =       0.; }
				for (int idet=0;idet<NDETSIDE;idet++){
					if (useDetZV[idet][0]){ twest[idet] = tsofar[idet][0]; goodside[0]=true; } else { twest[idet] = latetime; }
					if (useDetZV[idet][1]){ teast[idet] = tsofar[idet][1]; goodside[1]=true; } else { teast[idet] = latetime; }
					twestsort[idet]	= twest[idet];
					teastsort[idet]	= teast[idet];
				}
				if (goodside[0]&&goodside[1]){ EWcoincidence = true; }
				//
				if (EWcoincidence){
					//
					if (iChoice==0||iChoice==2){
						std::sort(twestsort, twestsort+NDETSIDE, std::less<float>());
						std::sort(teastsort, teastsort+NDETSIDE, std::less<float>());
					} else if (iChoice==1||iChoice==3||iChoice==5){
						std::sort(twestsort, twestsort+NDETSIDE, std::greater<float>());
						std::sort(teastsort, teastsort+NDETSIDE, std::greater<float>());
					}
					//for (int idet=0;idet<NDETSIDE;idet++){
					//	cout<<idet<<" \t "<<twest[idet]<<" "<<twestsort[idet]<<" \t "
					//		<<teast[idet]<<" "<<teastsort[idet]<<" \t "<<endl;
					//}
					//
					if (iChoice==0||iChoice==2){
						zvt	= (29.98/2.)*(teastsort[0] - twestsort[0]);		// east-west
					} else if (iChoice==1||iChoice==3||iChoice==5){
						zvt	= (29.98/2.)*(twestsort[0] - teastsort[0]);		// west-east
					}
					hzvt_end_zvtx_all->Fill(vtx[2],zvt,1.0);
					if (vtxe[2]<0.5){
						hzvt_end_zvtx_cut->Fill(vtx[2],zvt,1.0);
					}
					hdzvtx_11->Fill(zvt-vtx[2]);
					hdzvtx_11_zvtx->Fill(vtx[2],zvt-vtx[2]);					
					//
				}	//---- end coincidence check...
				//
			}	//---- end lastiter check...
			//
		}	
		//---- end event loop...

		//----- start fitting section....
		//
		for (int is=0;is<2;is++){
			//
			++ncan;
			ipad = 0;
			psout->NewPage();
			sprintf(buf,"ccan%d",ncan);
			ccan[ncan] = new TCanvas(buf,buf,0,0,800,600);
			ccan[ncan]->SetFillColor(10);
			ccan[ncan]->cd(); 
			ccan[ncan]->Divide(5,4,0.0001,0.0001);
			//cout<<"new canvas  "<<ncan<<"  iter="<<iter<<" is="<<0<<" key="<<keyCh<<" "<<endl;
			//
			for (int idet=0;idet<NDETSIDE;idet++){
				//
				ccan[ncan]->cd(idet+1);
				//
				if (hdt_A[idet+iter*NDETSIDE][is]->GetEntries()>0){
					hdt_A[idet+iter*NDETSIDE][is]->FitSlicesY();
					hdt_A[idet+iter*NDETSIDE][is]->Draw("col");
					sprintf(buf,"hdt_A%d%d_1",idet+iter*NDETSIDE,is);
					hdt_A_fit[idet+iter*NDETSIDE][is]	= (TH1D*)gDirectory->Get(buf);
						sprintf(buf,"hdt_A_fit%d%d",idet+iter*NDETSIDE,is);
						hdt_A_fit[idet+iter*NDETSIDE][is]->SetName(buf);
						hdt_A_fit[idet+iter*NDETSIDE][is]->SetTitle(buf);
						hdt_A_fit[idet+iter*NDETSIDE][is]->SetDirectory(0);
						//hdt_A_fit[idet+iter*NDETSIDE]->SetLineColor(icolors[idet%8]);
						//hdt_A_fit[idet+iter*NDETSIDE]->SetMarkerColor(icolors[idet%8]);
						//hdt_A_fit[idet+iter*NDETSIDE]->SetMarkerSize(0.6);
						//hdt_A_fit[idet+iter*NDETSIDE]->SetMarkerStyle(20);
						//hdt_A_fit[idet+iter*NDETSIDE]->Draw("hist P same");
					//
					//---- 5th constructor....
					sprintf(buf,"ff%d%d",idet+iter*NDETSIDE,is);
					//cout<<"\t\t\t splinefit "<<idet+iter*NDETSIDE<<" "<<is<<" "<<hdt_A_fit[idet+iter*NDETSIDE][is]->GetEntries()<<endl;
					ff[idet+iter*NDETSIDE][is] = new TSplineFit(buf, buf, A_nbin, 2, hdt_A_fit[idet+iter*NDETSIDE][is]);  
					ff[idet+iter*NDETSIDE][is]->DrawHere(dT_min,dT_max);	//---- requires existing gPad now!!!!
					//		
					//if (idet==0){ ff[idet][is]->UpdateFile(kTRUE);  }
					//else        { ff[idet][is]->UpdateFile(kFALSE); }
					//
					sprintf(buftext,"idet=%d is=%d key=%d",idet+iter*NDETSIDE,is,keyCh);
					//cout<<"\t\t\t    text     "<<buftext<<endl;
					text->DrawLatex(0.18,0.82,buftext);
					ccan[ncan]->Update();
					//
				}	// end entries check...
			}	// end idet...
		}	// end is...
		//---- end side loop for fitting....	
		cout<<"... End of Iteration "<<iter<<" ................................................."<<endl;
	}
	//---- end iteration loop.....
	
	//---- get zv offsets by pair 
	for (int ix=1;ix<=NDETSIDE;ix++){
		for (int iy=1;iy<=NDETSIDE;iy++){
			maxval	= 0.0;
			dzdval	= 0.0;
			for (int iz=1;iz<=100;iz++){
				thisval	= hdzd3D->GetBinContent(ix,iy,iz);
				if (thisval>maxval){ 
					maxval = thisval; 
					dzdval = hdzd3D->GetZaxis()->GetBinCenter(iz);
				};
			}	
			hdzd3Dvals->SetBinContent(ix,iy,dzdval);
			//cout<<"DZDoffsets \t"<<ix<<" "<<iy<<" "<<dzdval<<endl;
		}
	}
	//
	//---- get DZD offsets from 1Ds...
	float zpeakA = FindPeak(hddzvzt_A_all);
	float zpeakB = FindPeak(hddzvzt_B_all);
	float zpeakC = FindPeak(hddzvzt_C_all);
	cout<<"Found DZDoffsetA = "<<zpeakA<<endl;
	cout<<"Found DZDoffsetB = "<<zpeakB<<endl;
	cout<<"Found DZDoffsetC = "<<zpeakC<<endl;
		
	//=============================================================================	
	//
	cout<<"..... Loop  Writing objects to root file...."<<endl;	
	//
	fout->cd();
	hcoco_vpd_w->Write();
	hcoco_vpd_e->Write();
	hcoco_p2p_w->Write();
	hcoco_p2p_e->Write();
	hinitialoffsets->Write();
	//hpinitialoffsets->Write();
	hinitialoffsetvals->Write();
	hnev_day->Write();
	hnev_daqbits->Write();
	//
	hdtzv_11->Write();
	hdtzv_11w->Write();
	hdtzv_11e->Write();
	hdzd->Write();
	hdzd3D->Write();
	hdzd3Dvals->Write();
	//
	hdtzv_nn_00->Write();
	hdtzv_nn_11->Write();
	hdtzv_nn_22->Write();
	hdtzv_nn_33->Write();
	hdtzv_nn_00_keep->Write();
	hdtzv_nn_11_keep->Write();
	hdtzv_nn_22_keep->Write();
	hdtzv_nn_33_keep->Write();
	hnvalidpairs->Write();
	hdtzvb_nn_00->Write();
	hdtzvb_nn_11->Write();
	hdtzvb_nn_22->Write();
	hdtzvb_nn_33->Write();
	hdtzvb_nn_00_keep->Write();
	hdtzvb_nn_11_keep->Write();
	hdtzvb_nn_22_keep->Write();
	hdtzvb_nn_33_keep->Write();
	hnvalidpairsb->Write();
	hdtzvc_nn_00->Write();
	hdtzvc_nn_11->Write();
	hdtzvc_nn_22->Write();
	hdtzvc_nn_33->Write();
	hdtzvc_nn_00_keep->Write();
	hdtzvc_nn_11_keep->Write();
	hdtzvc_nn_22_keep->Write();
	hdtzvc_nn_33_keep->Write();
	hnvalidpairsc->Write();
	//
	hdtzv1D_nn_00->Write();
	hdtzv1D_nn_11->Write();
	hdtzv1D_nn_22->Write();
	hdtzv1D_nn_33->Write();
	hdtzv1D_nn_00_keep->Write();
	hdtzv1D_nn_11_keep->Write();
	hdtzv1D_nn_22_keep->Write();
	hdtzv1D_nn_33_keep->Write();
	hdtzvb1D_nn_00->Write();
	hdtzvb1D_nn_11->Write();
	hdtzvb1D_nn_22->Write();
	hdtzvb1D_nn_33->Write();
	hdtzvb1D_nn_00_keep->Write();
	hdtzvb1D_nn_11_keep->Write();
	hdtzvb1D_nn_22_keep->Write();
	hdtzvb1D_nn_33_keep->Write();
	hdtzvc1D_nn_00->Write();
	hdtzvc1D_nn_11->Write();
	hdtzvc1D_nn_22->Write();
	hdtzvc1D_nn_33->Write();
	hdtzvc1D_nn_00_keep->Write();
	hdtzvc1D_nn_11_keep->Write();
	hdtzvc1D_nn_22_keep->Write();
	hdtzvc1D_nn_33_keep->Write();	
	//
	for (int i=0;i<40;i++){
		hcnts[i]->SetMinimum(0.0);
		hcnts[i]->Write();
	}	
	dir_zvtx->cd();
	htemp1->Write();
	htemp2->Write();
	htemp3->Write();
	hthist_zvtx->Write();
	hzvt_end_zvtx_all->Write();
	hzvt_end_zvtx_cut->Write();
	hdzvtx_11->Write();
	hdzvtx_11_zvtx->Write();
	//
	fout->cd();
	hdt_end_det0_zvtx[0]->Write();
	hdt_end_det0_zvtx[1]->Write();
	hdt_end_det0_grefmult[0]->Write();
	hdt_end_det0_grefmult[1]->Write();
	//
	dir_A->cd();
	for (int is=0;is<2;is++){
		for (int idet=0;idet<NDETSIDE;idet++){
			adctot[idet][is]->Write();
			htot[idet][is]->Write();
			hadc[idet][is]->Write();
		}
	}	
	//
	dir_nvpd->cd();
	for (int is=0;is<2;is++){
		for (int iter=0;iter<NITERATIONS;iter++){
			hnvpdrm[iter][is]->Write();
		}
	}
	//
	dir_ffun->cd();
	float	xval;
	float 	bwid	= (out_ffun_x2-out_ffun_x1)/(float)out_ffun_nb;
	//	cout<<"  out_ffun_nb = "<<out_ffun_nb<<endl;
	//	cout<<"  out_ffun_x1 = "<<out_ffun_x1<<endl;
	//	cout<<"  out_ffun_x2 = "<<out_ffun_x2<<endl;
	//	cout<<"         bwid = "<<bwid<<endl;
	for (int is=0;is<2;is++){
		for (int i=0;i<NDETSIDE*NITERATIONS;i++){
			int iter	= i/NDETSIDE;
			int idet	= i%NDETSIDE;
			if (ff[i][is] && ff[i][is]->GetNbOfMeas()>1 ) {
				float ioff		= 0;
				if (iter==0){
					ioff		= hinitialoffsetvals->GetBinContent(1+idet+is*NDETSIDE);
				}
				for (int ixb=0; ixb<out_ffun_nb; ixb++){
					xval	= bwid*(0.5+((float)ixb));
					valcor	= ff[i][is]->V(xval);
					ffun_iter[i][is]->Fill(xval,valcor);
					if (iter==0){ hcorrfinal[idet][is]->Fill(xval,ioff); }
								  hcorrfinal[idet][is]->Fill(xval,valcor);
//if (idet==0&&is==1){
//	if (iter==0){ cout<<"idet="<<idet<<" is="<<is<<"  ioff="<<ioff<<"  valcorr="<<valcor<<endl; } else 
//	if (iter> 0){ cout<<"idet="<<idet<<" is="<<is<<"  valcorr="<<valcor<<endl; } 
//}
				}
			} else {
				for (int ixb=0; ixb<out_ffun_nb; ixb++){
					xval		= bwid*(0.5+((float)ixb));
					valcor		= 0.0;
					ffun_iter[i][is]->Fill(xval,valcor);
				}
			}
		}
	}
	for (int is=0;is<2;is++){
		for (int i=0;i<NDETSIDE*NITERATIONS;i++){
			ffun_iter[i][is]->Write();
		}
		for (int idet=0;idet<NDETSIDE;idet++){
			hcorrfinal[idet][is]->Write();
		}
	}
	//
	dir_1mn->cd();
	for (int i=0;i<NITERATIONS;i++){
		for (int is=0;is<2;is++){
			for (int idet=0;idet<NDETSIDE;idet++){
				h1mn_n[i][idet][is]->Write();
			}
		}	
	}
	//
	//=============================================================================	

	cout<<"..... Loop  Writing plots to postscript file...."<<endl;	
	TLatex *thistext	= new TLatex();
		thistext->SetNDC();
		thistext->SetTextSize(0.05);
	ipad = 0;
		
	gStyle->SetOptStat(1111);
	++ncan;
	psout->NewPage();
	sprintf(buf,"ccan%d",ncan);
	ccan[ncan] = new TCanvas(buf,buf,0,0,800,600);
	ccan[ncan]->SetFillColor(10);
	ccan[ncan]->cd(); ccan[ncan]->Divide(3,1,0.0001,0.0001);
	ccan[ncan]->cd(1);
		gPad->SetLogy(1);
		hddzvzt_A_all->SetStats(1);
		hddzvzt_A_all->Draw();
		hddzvzt_A_keep->SetFillStyle(1001);
		hddzvzt_A_keep->SetFillColor(3);
		hddzvzt_A_keep->Draw("same");
		sprintf(buf,"DZDoffsetA = %4.1f",DZDoffsetA);
		thistext->DrawLatex(0.16,0.91,buf);
	ccan[ncan]->cd(2);
		gPad->SetLogy(1);
		hddzvzt_B_all->SetStats(1);
		hddzvzt_B_all->Draw();
		hddzvzt_B_keep->SetFillStyle(1001);
		hddzvzt_B_keep->SetFillColor(3);
		hddzvzt_B_keep->Draw("same");
		sprintf(buf,"DZDoffsetB = %4.1f",DZDoffsetB);
		thistext->DrawLatex(0.16,0.91,buf);
	ccan[ncan]->cd(3);
		gPad->SetLogy(1);
		hddzvzt_C_all->SetStats(1);
		hddzvzt_C_all->Draw();
		hddzvzt_C_keep->SetFillStyle(1001);
		hddzvzt_C_keep->SetFillColor(3);
		hddzvzt_C_keep->Draw("same");
		sprintf(buf,"DZDoffsetC = %4.1f",DZDoffsetC);
		thistext->DrawLatex(0.16,0.91,buf);
	ccan[ncan]->Update();
	gStyle->SetOptStat(0);

	gStyle->SetOptStat(1111);
	++ncan;
	psout->NewPage();
	sprintf(buf,"ccan%d",ncan);
	ccan[ncan] = new TCanvas(buf,buf,0,0,800,600);
	ccan[ncan]->SetFillColor(10);
	ccan[ncan]->cd(); ccan[ncan]->Divide(2,2,0.0001,0.0001);
	ccan[ncan]->cd(1);
		hdzvtx_11->Draw();
	ccan[ncan]->cd(2);
		hdzvtx_11_zvtx->Draw("colz");
	ccan[ncan]->cd(3);
		hzvt_end_zvtx_all->Draw("colz");
	ccan[ncan]->cd(4);
		hzvt_end_zvtx_cut->Draw("colz");
	ccan[ncan]->Update();
	gStyle->SetOptStat(0);

// 	++ncan;
// 	psout->NewPage();
// 	sprintf(buf,"ccan%d",ncan);
// 	ccan[ncan] = new TCanvas(buf,buf,0,0,800,600);
// 	ccan[ncan]->SetFillColor(10);
// 	ccan[ncan]->cd(); ccan[ncan]->Divide(3,2,0.0001,0.0001);
// 	ccan[ncan]->cd(1);
// 		gPad->SetLogz(1);
// 		hddzvzt_A_evt->Draw("col");
// 	ccan[ncan]->cd(2);
// 		gPad->SetLogz(1);
// 		hddzvzt_B_evt->Draw("col");
// 	ccan[ncan]->cd(3);
// 		gPad->SetLogz(1);
// 		hddzvzt_C_evt->Draw("col");
// 	ccan[ncan]->cd(4);
// 		gPad->SetLogz(1);
// 		hddzvzt_A_day->Draw("col");
// 	ccan[ncan]->cd(5);
// 		gPad->SetLogz(1);
// 		hddzvzt_B_day->Draw("col");
// 	ccan[ncan]->cd(6);
// 		gPad->SetLogz(1);
// 		hddzvzt_C_day->Draw("col");
// 	ccan[ncan]->Update();
// 	gStyle->SetOptStat(0);

	for (int i=0;i<NITERATIONS;i++){
		//
		int kdet;
		int ks;
		int kn;
		//
		++ncan;
		psout->NewPage();
		sprintf(buf,"ccan%d",ncan);
		ccan[ncan] = new TCanvas(buf,buf,0,0,800,600);
		ccan[ncan]->SetFillColor(10);
		ccan[ncan]->cd(); ccan[ncan]->Divide(4,2,0.0001,0.0001);
			//
			kdet=0; ks=0;
			//
			ccan[ncan]->cd(1);
				gPad->SetLogz(1);
				h1mn_n[i][kdet][ks]->Draw("colz");
			ccan[ncan]->cd(2);
				gPad->SetLogy(1);
				kn=2;
				h1mn_n[i][kdet][ks]->ProjectionY(Form("h1mn_n%d%d%d_py%d",i,kdet,ks,kn),kn,kn)->Draw();
			ccan[ncan]->cd(3);
				gPad->SetLogy(1);
				kn=5;
				h1mn_n[i][kdet][ks]->ProjectionY(Form("h1mn_n%d%d%d_py%d",i,kdet,ks,kn),kn,kn)->Draw();
			ccan[ncan]->cd(4);
				gPad->SetLogy(1);
				kn=8;
				h1mn_n[i][kdet][ks]->ProjectionY(Form("h1mn_n%d%d%d_py%d",i,kdet,ks,kn),kn,kn)->Draw();
			//
			kdet=0; ks=1;
			//
			ccan[ncan]->cd(5);
				gPad->SetLogz(1);
				h1mn_n[i][kdet][ks]->Draw("colz");
			ccan[ncan]->cd(6);
				gPad->SetLogy(1);
				kn=2;
				h1mn_n[i][kdet][ks]->ProjectionY(Form("h1mn_n%d%d%d_py%d",i,kdet,ks,kn),kn,kn)->Draw();
			ccan[ncan]->cd(7);
				gPad->SetLogy(1);
				kn=5;
				h1mn_n[i][kdet][ks]->ProjectionY(Form("h1mn_n%d%d%d_py%d",i,kdet,ks,kn),kn,kn)->Draw();
			ccan[ncan]->cd(8);
				gPad->SetLogy(1);
				kn=8;
				h1mn_n[i][kdet][ks]->ProjectionY(Form("h1mn_n%d%d%d_py%d",i,kdet,ks,kn),kn,kn)->Draw();
			//
		ccan[ncan]->Update();
		gStyle->SetOptStat(0);
	}

	TF1 *gg = new TF1("gg","gaus",-1.,1.);		//!!!!!!!!!! was +/-0.35
		 gg->SetRange(-1.0,1.0);
	//
	fout->cd();
	for (int is=0;is<2;is++){
 		hAiter[is]->Write();
		for (int idet=0;idet<NDETSIDE;idet++){
			dir_res->cd();
				hdt_beg[idet][is]->Write();
				hdt_end[idet][is]->Write();
				hdt_beg_n[idet][is]->Write();
				hdt_end_n[idet][is]->Write();
				//	
//				cout<<"hdt_end fits.... "<<is<<" "<<idet<<" "<<hdt_end_n[idet][is]->GetEntries()<<endl;
//				if (hdt_end_n[idet][is]->GetEntries()>50){
					hdt_end_n[idet][is]->FitSlicesY(gg,0,-1,10);
					sprintf(buf,"hdt_end_n%d%d_0",idet,is);
					sprintf(buf2,"hdt_end_n_cons%d%d",idet,is);
						hdt_end_n_cons[idet][is] = (TH1D*)gDirectory->Get(buf);
						hdt_end_n_cons[idet][is]->SetName(buf2);
						hdt_end_n_cons[idet][is]->Write();
					sprintf(buf,"hdt_end_n%d%d_1",idet,is);
					sprintf(buf2,"hdt_end_n_mean%d%d",idet,is);
						hdt_end_n_mean[idet][is] = (TH1D*)gDirectory->Get(buf);
						hdt_end_n_mean[idet][is]->SetName(buf2);
						hdt_end_n_mean[idet][is]->Write();
					sprintf(buf,"hdt_end_n%d%d_2",idet,is);
					sprintf(buf2,"hdt_end_n_sigm%d%d",idet,is);
						hdt_end_n_sigm[idet][is] = (TH1D*)gDirectory->Get(buf);
						hdt_end_n_sigm[idet][is]->SetName(buf2);
						hdt_end_n_sigm[idet][is]->Write();
//				}
		}
		for (int i=0;i<NITERATIONS*NDETSIDE;i++){
			dir_fits->cd();
				hdt_A[i][is]->Write();
				hdt_A_1D[i][is]->Write();
			dir_fitslices->cd();
				hdt_A[i][is]->FitSlicesY();
				sprintf(buf,"hdt_A%d%d_0",i,is);
				sprintf(buf2,"hdt_A_cons%d%d",i,is);
					hdt_A_cons[i][is] = (TH1D*)gDirectory->Get(buf);
					hdt_A_cons[i][is]->SetName(buf2);
					hdt_A_cons[i][is]->Write();
				sprintf(buf,"hdt_A%d%d_1",i,is);
				sprintf(buf2,"hdt_A_mean%d%d",i,is);
					hdt_A_mean[i][is] = (TH1D*)gDirectory->Get(buf);
					hdt_A_mean[i][is]->SetName(buf2);
					hdt_A_mean[i][is]->Write();
				sprintf(buf,"hdt_A%d%d_2",i,is);
				sprintf(buf2,"hdt_A_sigm%d%d",i,is);
					hdt_A_sigm[i][is] = (TH1D*)gDirectory->Get(buf);
					hdt_A_sigm[i][is]->SetName(buf2);
					hdt_A_sigm[i][is]->Write();
		}
		for (int iday=iDayMin;iday<=iDayMax;iday++){
			dir_day->cd();
				idt	= iday - iDayMin;
				hdt_end_n_det0_day[idt][is]->Write();
				hdt_end_n_det1_day[idt][is]->Write();
				hdt_end_n_det2_day[idt][is]->Write();
				hdt_end_n_det3_day[idt][is]->Write();
				//	
//				cout<<"hdt_end DAY fits.... "<<is<<" "<<iday<<" "<<idt<<" "<<hdt_end_n_det0_day[idt][is]->GetEntries()<<endl;
//				if (hdt_end_n[idt][is]->GetEntries()>50){
					hdt_end_n_det0_day[idt][is]->FitSlicesY(gg,0,-1,10);
					sprintf(buf,"hdt_end_n_det0_day%d%d_0",idt,is);
					sprintf(buf2,"hdt_end_n_det0_day_cons%d%d",idt,is);
						hdt_end_n_det0_day_cons[idt][is] = (TH1D*)gDirectory->Get(buf);
						hdt_end_n_det0_day_cons[idt][is]->SetName(buf2);
						hdt_end_n_det0_day_cons[idt][is]->Write();
					sprintf(buf,"hdt_end_n_det0_day%d%d_1",idt,is);
					sprintf(buf2,"hdt_end_n_mean%d%d",idt,is);
						hdt_end_n_det0_day_mean[idt][is] = (TH1D*)gDirectory->Get(buf);
						hdt_end_n_det0_day_mean[idt][is]->SetName(buf2);
						hdt_end_n_det0_day_mean[idt][is]->Write();
					sprintf(buf,"hdt_end_n_det0_day%d%d_2",idt,is);
					sprintf(buf2,"hdt_end_n_det0_day_sigm%d%d",idt,is);
						hdt_end_n_det0_day_sigm[idt][is] = (TH1D*)gDirectory->Get(buf);
						hdt_end_n_det0_day_sigm[idt][is]->SetName(buf2);
						hdt_end_n_det0_day_sigm[idt][is]->Write();
//				}
//				if (hdt_end_n[idt][is]->GetEntries()>50){
					hdt_end_n_det1_day[idt][is]->FitSlicesY(gg,0,-1,10);
					sprintf(buf,"hdt_end_n_det1_day%d%d_0",idt,is);
					sprintf(buf2,"hdt_end_n_det1_day_cons%d%d",idt,is);
						hdt_end_n_det1_day_cons[idt][is] = (TH1D*)gDirectory->Get(buf);
						hdt_end_n_det1_day_cons[idt][is]->SetName(buf2);
						hdt_end_n_det1_day_cons[idt][is]->Write();
					sprintf(buf,"hdt_end_n_det1_day%d%d_1",idt,is);
					sprintf(buf2,"hdt_end_n_mean%d%d",idt,is);
						hdt_end_n_det1_day_mean[idt][is] = (TH1D*)gDirectory->Get(buf);
						hdt_end_n_det1_day_mean[idt][is]->SetName(buf2);
						hdt_end_n_det1_day_mean[idt][is]->Write();
					sprintf(buf,"hdt_end_n_det1_day%d%d_2",idt,is);
					sprintf(buf2,"hdt_end_n_det1_day_sigm%d%d",idt,is);
						hdt_end_n_det1_day_sigm[idt][is] = (TH1D*)gDirectory->Get(buf);
						hdt_end_n_det1_day_sigm[idt][is]->SetName(buf2);
						hdt_end_n_det1_day_sigm[idt][is]->Write();
//				}
//				if (hdt_end_n[idt][is]->GetEntries()>50){
					hdt_end_n_det2_day[idt][is]->FitSlicesY(gg,0,-1,10);
					sprintf(buf,"hdt_end_n_det2_day%d%d_0",idt,is);
					sprintf(buf2,"hdt_end_n_det2_day_cons%d%d",idt,is);
						hdt_end_n_det2_day_cons[idt][is] = (TH1D*)gDirectory->Get(buf);
						hdt_end_n_det2_day_cons[idt][is]->SetName(buf2);
						hdt_end_n_det2_day_cons[idt][is]->Write();
					sprintf(buf,"hdt_end_n_det2_day%d%d_1",idt,is);
					sprintf(buf2,"hdt_end_n_mean%d%d",idt,is);
						hdt_end_n_det2_day_mean[idt][is] = (TH1D*)gDirectory->Get(buf);
						hdt_end_n_det2_day_mean[idt][is]->SetName(buf2);
						hdt_end_n_det2_day_mean[idt][is]->Write();
					sprintf(buf,"hdt_end_n_det2_day%d%d_2",idt,is);
					sprintf(buf2,"hdt_end_n_det2_day_sigm%d%d",idt,is);
						hdt_end_n_det2_day_sigm[idt][is] = (TH1D*)gDirectory->Get(buf);
						hdt_end_n_det2_day_sigm[idt][is]->SetName(buf2);
						hdt_end_n_det2_day_sigm[idt][is]->Write();
//				}
//				if (hdt_end_n[idt][is]->GetEntries()>50){
					hdt_end_n_det3_day[idt][is]->FitSlicesY(gg,0,-1,10);
					sprintf(buf,"hdt_end_n_det3_day%d%d_0",idt,is);
					sprintf(buf2,"hdt_end_n_det3_day_cons%d%d",idt,is);
						hdt_end_n_det3_day_cons[idt][is] = (TH1D*)gDirectory->Get(buf);
						hdt_end_n_det3_day_cons[idt][is]->SetName(buf2);
						hdt_end_n_det3_day_cons[idt][is]->Write();
					sprintf(buf,"hdt_end_n_det3_day%d%d_1",idt,is);
					sprintf(buf2,"hdt_end_n_mean%d%d",idt,is);
						hdt_end_n_det3_day_mean[idt][is] = (TH1D*)gDirectory->Get(buf);
						hdt_end_n_det3_day_mean[idt][is]->SetName(buf2);
						hdt_end_n_det3_day_mean[idt][is]->Write();
					sprintf(buf,"hdt_end_n_det3_day%d%d_2",idt,is);
					sprintf(buf2,"hdt_end_n_det3_day_sigm%d%d",idt,is);
						hdt_end_n_det3_day_sigm[idt][is] = (TH1D*)gDirectory->Get(buf);
						hdt_end_n_det3_day_sigm[idt][is]->SetName(buf2);
						hdt_end_n_det3_day_sigm[idt][is]->Write();
//				}
		}
		//---- end day loop
		//
		for (int idt=0;idt<dtimenbins;idt++){
			dir_time->cd();
			//if (idt%25==0){ cout<<"resn vs time looping ... "<<is<<" "<<idt<<" "<<dtimenbins<<endl; }
				hdt_end_n_det0_time[idt][is]->Write();
				hdt_end_n_det1_time[idt][is]->Write();
				hdt_end_n_det2_time[idt][is]->Write();
				hdt_end_n_det3_time[idt][is]->Write();
				//	
//				cout<<"hdt_end TIME fits.... "<<is<<" "<<idt <<" "<<idt<<" "<<hdt_end_n_det0_time[idt][is]->GetEntries()<<endl;
//				if (hdt_end_n[idt][is]->GetEntries()>50){
					hdt_end_n_det0_time[idt][is]->FitSlicesY(gg,0,-1,10);
					sprintf(buf,"hdt_end_n_det0_time%d%d_0",idt,is);
					sprintf(buf2,"hdt_end_n_det0_time_cons%d%d",idt,is);
						hdt_end_n_det0_time_cons[idt][is] = (TH1D*)gDirectory->Get(buf);
						hdt_end_n_det0_time_cons[idt][is]->SetName(buf2);
						hdt_end_n_det0_time_cons[idt][is]->Write();
					sprintf(buf,"hdt_end_n_det0_time%d%d_1",idt,is);
					sprintf(buf2,"hdt_end_n_mean%d%d",idt,is);
						hdt_end_n_det0_time_mean[idt][is] = (TH1D*)gDirectory->Get(buf);
						hdt_end_n_det0_time_mean[idt][is]->SetName(buf2);
						hdt_end_n_det0_time_mean[idt][is]->Write();
					sprintf(buf,"hdt_end_n_det0_time%d%d_2",idt,is);
					sprintf(buf2,"hdt_end_n_det0_time_sigm%d%d",idt,is);
						hdt_end_n_det0_time_sigm[idt][is] = (TH1D*)gDirectory->Get(buf);
						hdt_end_n_det0_time_sigm[idt][is]->SetName(buf2);
						hdt_end_n_det0_time_sigm[idt][is]->Write();
//				}
//				if (hdt_end_n[idt][is]->GetEntries()>50){
					hdt_end_n_det1_time[idt][is]->FitSlicesY(gg,0,-1,10);
					sprintf(buf,"hdt_end_n_det1_time%d%d_0",idt,is);
					sprintf(buf2,"hdt_end_n_det1_time_cons%d%d",idt,is);
						hdt_end_n_det1_time_cons[idt][is] = (TH1D*)gDirectory->Get(buf);
						hdt_end_n_det1_time_cons[idt][is]->SetName(buf2);
						hdt_end_n_det1_time_cons[idt][is]->Write();
					sprintf(buf,"hdt_end_n_det1_time%d%d_1",idt,is);
					sprintf(buf2,"hdt_end_n_mean%d%d",idt,is);
						hdt_end_n_det1_time_mean[idt][is] = (TH1D*)gDirectory->Get(buf);
						hdt_end_n_det1_time_mean[idt][is]->SetName(buf2);
						hdt_end_n_det1_time_mean[idt][is]->Write();
					sprintf(buf,"hdt_end_n_det1_time%d%d_2",idt,is);
					sprintf(buf2,"hdt_end_n_det1_time_sigm%d%d",idt,is);
						hdt_end_n_det1_time_sigm[idt][is] = (TH1D*)gDirectory->Get(buf);
						hdt_end_n_det1_time_sigm[idt][is]->SetName(buf2);
						hdt_end_n_det1_time_sigm[idt][is]->Write();
//				}
//				if (hdt_end_n[idt][is]->GetEntries()>50){
					hdt_end_n_det2_time[idt][is]->FitSlicesY(gg,0,-1,10);
					sprintf(buf,"hdt_end_n_det2_time%d%d_0",idt,is);
					sprintf(buf2,"hdt_end_n_det2_time_cons%d%d",idt,is);
						hdt_end_n_det2_time_cons[idt][is] = (TH1D*)gDirectory->Get(buf);
						hdt_end_n_det2_time_cons[idt][is]->SetName(buf2);
						hdt_end_n_det2_time_cons[idt][is]->Write();
					sprintf(buf,"hdt_end_n_det2_time%d%d_1",idt,is);
					sprintf(buf2,"hdt_end_n_mean%d%d",idt,is);
						hdt_end_n_det2_time_mean[idt][is] = (TH1D*)gDirectory->Get(buf);
						hdt_end_n_det2_time_mean[idt][is]->SetName(buf2);
						hdt_end_n_det2_time_mean[idt][is]->Write();
					sprintf(buf,"hdt_end_n_det2_time%d%d_2",idt,is);
					sprintf(buf2,"hdt_end_n_det2_time_sigm%d%d",idt,is);
						hdt_end_n_det2_time_sigm[idt][is] = (TH1D*)gDirectory->Get(buf);
						hdt_end_n_det2_time_sigm[idt][is]->SetName(buf2);
						hdt_end_n_det2_time_sigm[idt][is]->Write();
//				}
//				if (hdt_end_n[idt][is]->GetEntries()>50){
					hdt_end_n_det3_time[idt][is]->FitSlicesY(gg,0,-1,10);
					sprintf(buf,"hdt_end_n_det3_time%d%d_0",idt,is);
					sprintf(buf2,"hdt_end_n_det3_time_cons%d%d",idt,is);
						hdt_end_n_det3_time_cons[idt][is] = (TH1D*)gDirectory->Get(buf);
						hdt_end_n_det3_time_cons[idt][is]->SetName(buf2);
						hdt_end_n_det3_time_cons[idt][is]->Write();
					sprintf(buf,"hdt_end_n_det3_time%d%d_1",idt,is);
					sprintf(buf2,"hdt_end_n_mean%d%d",idt,is);
						hdt_end_n_det3_time_mean[idt][is] = (TH1D*)gDirectory->Get(buf);
						hdt_end_n_det3_time_mean[idt][is]->SetName(buf2);
						hdt_end_n_det3_time_mean[idt][is]->Write();
					sprintf(buf,"hdt_end_n_det3_time%d%d_2",idt,is);
					sprintf(buf2,"hdt_end_n_det3_time_sigm%d%d",idt,is);
						hdt_end_n_det3_time_sigm[idt][is] = (TH1D*)gDirectory->Get(buf);
						hdt_end_n_det3_time_sigm[idt][is]->SetName(buf2);
						hdt_end_n_det3_time_sigm[idt][is]->Write();
//				}
		}
		//---- end day loop
	}
	//---- end iside loop

	fout->cd();
	for (int is=0;is<2;is++){
		hdt_end_det0_zvtx[is]->FitSlicesY(gg,0,-1,10);
		sprintf(buf,"hdt_end_det0_zvtx%d_0",is);
		sprintf(buf2,"hdt_end_det0_zvtx_cons%d",is);
			hdt_end_det0_zvtx_cons[is] = (TH1D*)gDirectory->Get(buf);
			hdt_end_det0_zvtx_cons[is]->SetName(buf2);
			hdt_end_det0_zvtx_cons[is]->SetTitle(buf2);
			hdt_end_det0_zvtx_cons[is]->Write();
		sprintf(buf,"hdt_end_det0_zvtx%d_1",is);
		sprintf(buf2,"hdt_end_det0_zvtx_mean%d",is);
			hdt_end_det0_zvtx_mean[is] = (TH1D*)gDirectory->Get(buf);
			hdt_end_det0_zvtx_mean[is]->SetName(buf2);
			hdt_end_det0_zvtx_mean[is]->SetTitle(buf2);
			hdt_end_det0_zvtx_mean[is]->Write();
		sprintf(buf,"hdt_end_det0_zvtx%d_2",is);
		sprintf(buf2,"hdt_end_det0_zvtx_sigm%d",is);
			hdt_end_det0_zvtx_sigm[is] = (TH1D*)gDirectory->Get(buf);
			hdt_end_det0_zvtx_sigm[is]->SetName(buf2);
			hdt_end_det0_zvtx_sigm[is]->SetTitle(buf2);
			hdt_end_det0_zvtx_sigm[is]->Write();
	}
	for (int is=0;is<2;is++){
		hdt_end_det0_grefmult[is]->FitSlicesY(gg,0,-1,10);
		sprintf(buf,"hdt_end_det0_grefmult%d_0",is);
		sprintf(buf2,"hdt_end_det0_grefmult_cons%d",is);
			hdt_end_det0_grefmult_cons[is] = (TH1D*)gDirectory->Get(buf);
			hdt_end_det0_grefmult_cons[is]->SetName(buf2);
			hdt_end_det0_grefmult_cons[is]->SetTitle(buf2);
			hdt_end_det0_grefmult_cons[is]->Write();
		sprintf(buf,"hdt_end_det0_grefmult%d_1",is);
		sprintf(buf2,"hdt_end_det0_grefmult_mean%d",is);
			hdt_end_det0_grefmult_mean[is] = (TH1D*)gDirectory->Get(buf);
			hdt_end_det0_grefmult_mean[is]->SetName(buf2);
			hdt_end_det0_grefmult_mean[is]->SetTitle(buf2);
			hdt_end_det0_grefmult_mean[is]->Write();
		sprintf(buf,"hdt_end_det0_grefmult%d_2",is);
		sprintf(buf2,"hdt_end_det0_grefmult_sigm%d",is);
			hdt_end_det0_grefmult_sigm[is] = (TH1D*)gDirectory->Get(buf);
			hdt_end_det0_grefmult_sigm[is]->SetName(buf2);
			hdt_end_det0_grefmult_sigm[is]->SetTitle(buf2);
			hdt_end_det0_grefmult_sigm[is]->Write();
	}

	hdt_end_chan_nLLa->FitSlicesY();
		sprintf(buf,"hdt_end_chan_nLLa_0");
		sprintf(buf2,"hdt_end_chan_nLLa_cons");
			hdt_end_chan_nLLa_cons = (TH1D*)gDirectory->Get(buf);
			hdt_end_chan_nLLa_cons->SetName(buf2);
			hdt_end_chan_nLLa_cons->SetTitle(buf2);
			hdt_end_chan_nLLa_cons->Write();
		sprintf(buf,"hdt_end_chan_nLLa_1");
		sprintf(buf2,"hdt_end_chan_nLLa_mean");
			hdt_end_chan_nLLa_mean = (TH1D*)gDirectory->Get(buf);
			hdt_end_chan_nLLa_mean->SetName(buf2);
			hdt_end_chan_nLLa_mean->SetTitle(buf2);
			hdt_end_chan_nLLa_mean->Write();
		sprintf(buf,"hdt_end_chan_nLLa_2");
		sprintf(buf2,"hdt_end_chan_nLLa_sigm");
			hdt_end_chan_nLLa_sigm = (TH1D*)gDirectory->Get(buf);
			hdt_end_chan_nLLa_sigm->SetName(buf2);
			hdt_end_chan_nLLa_sigm->SetTitle(buf2);
			hdt_end_chan_nLLa_sigm->Write();
	hdt_end_chan_nLLb->FitSlicesY();
		sprintf(buf,"hdt_end_chan_nLLb_0");
		sprintf(buf2,"hdt_end_chan_nLLb_cons");
			hdt_end_chan_nLLb_cons = (TH1D*)gDirectory->Get(buf);
			hdt_end_chan_nLLb_cons->SetName(buf2);
			hdt_end_chan_nLLb_cons->SetTitle(buf2);
			hdt_end_chan_nLLb_cons->Write();
		sprintf(buf,"hdt_end_chan_nLLb_1");
		sprintf(buf2,"hdt_end_chan_nLLb_mean");
			hdt_end_chan_nLLb_mean = (TH1D*)gDirectory->Get(buf);
			hdt_end_chan_nLLb_mean->SetName(buf2);
			hdt_end_chan_nLLb_mean->SetTitle(buf2);
			hdt_end_chan_nLLb_mean->Write();
		sprintf(buf,"hdt_end_chan_nLLb_2");
		sprintf(buf2,"hdt_end_chan_nLLb_sigm");
			hdt_end_chan_nLLb_sigm = (TH1D*)gDirectory->Get(buf);
			hdt_end_chan_nLLb_sigm->SetName(buf2);
			hdt_end_chan_nLLb_sigm->SetTitle(buf2);
			hdt_end_chan_nLLb_sigm->Write();

	text->SetTextSize(0.06);

//	cout<<"..... Loop  Writing some more postscript...."<<endl;
//	ipad	= 0;
	for (int is=0;is<2;is++){
		for (int idet=0;idet<NDETSIDE;idet++){
			idetew	= idet + is*NDETSIDE;
// 			if (ipad==0 || ipad==8){
// 				++ncan;
// 				ipad = 0;
// 				psout->NewPage();
// 				sprintf(buf,"ccan%d",ncan);
// 				ccan[ncan] = new TCanvas(buf,buf,0,0,800,600);
// 				ccan[ncan]->SetFillColor(10);
// 				ccan[ncan]->cd(); 
// 				ccan[ncan]->Divide(4,2,0.0001,0.0001);
// 			}
//			ccan[ncan]->cd(++ipad);
			hdt_end[idet][is]->GetXaxis()->SetRangeUser(-rangeDt_1,rangeDt_1);
			if (hdt_end[idet][is]->GetEntries() > 0){
				hdt_end[idet][is]->Fit("gg","NQR");				// DO NOT DRAW........
//					sprintf(buftext,"idet=%d",idet);
//						text->DrawLatex(0.15,0.82,buftext);
//					sprintf(buftext,"iside=%d",is);
//						text->DrawLatex(0.15,0.76,buftext);
			}
//			if (ipad==8 || (is==1&&idet==NDETSIDE-1)){
//				ccan[ncan]->Update();
//			}
		}
	}
	
	//myStyle->SetOptStat(0);
	
	TF1 *fres[2*NDETSIDE];
	TF1 *fresh[2*NDETSIDE];
	for (int i=0;i<2*NDETSIDE;i++){
		sprintf(buf,"fres%d",i);
		fres[i]	= new TF1(buf,funres,0.5,19.5,1);
			fres[i]->SetRange(0.5,19.5);
			fres[i]->SetLineColor(1);
		sprintf(buf,"fresh%d",i);
		fresh[i]	= new TF1(buf,funres,5.5,10.5,1);
			fresh[i]->SetRange(5.5,10.5);
			fresh[i]->SetLineColor(7);
	}
	Double_t 	frespar[1],frespare[1],chisq,chisq_dof;
	Double_t 	freshpar[1],freshpare[1],chisqh,chisqh_dof;
	Int_t		npfit,npfith;
	TH1D *hsigma0 = new TH1D("hsigma0","hsigma0",2*NDETSIDE,-0.5,((Float_t)(2*NDETSIDE))-0.5);
			hsigma0->SetMarkerStyle(20);
			hsigma0->SetMarkerColor( 4);
			hsigma0->SetLineColor(   4);
	TH1D *hchisq0 = new TH1D("hchisq0","hchisq0",2*NDETSIDE,-0.5,((Float_t)(2*NDETSIDE))-0.5);
			hchisq0->SetMarkerStyle(21);
			hchisq0->SetMarkerColor( 4);
			hchisq0->SetLineColor(   4);
	TH1D *hsigma0h = new TH1D("hsigma0h","hsigma0h",2*NDETSIDE,-0.5,((Float_t)(2*NDETSIDE))-0.5);
			hsigma0h->SetMarkerStyle(24);
			hsigma0h->SetMarkerColor( 4);
			hsigma0h->SetLineColor(   4);
	TH1D *hchisq0h = new TH1D("hchisq0h","hchisq0h",2*NDETSIDE,-0.5,((Float_t)(2*NDETSIDE))-0.5);
			hchisq0h->SetMarkerStyle(25);
			hchisq0h->SetMarkerColor( 4);
			hchisq0h->SetLineColor(   4);
	ipad = 0;
	for (int is=0;is<2;is++){
		for (int idet=0;idet<NDETSIDE;idet++){
			idetew	= idet + is*NDETSIDE;
			if (ipad==0 || ipad==8 ){ 		// || (is==0&&idet==0)){
				++ncan;
				ipad = 0;
				psout->NewPage();
				sprintf(buf,"ccan%d",ncan);
				ccan[ncan] = new TCanvas(buf,buf,0,0,800,600);
				ccan[ncan]->SetFillColor(10);
				ccan[ncan]->cd(); 
				ccan[ncan]->Divide(4,2,0.0001,0.0001);
			}
			ccan[ncan]->cd(++ipad);
			hdt_end_n_sigm[idet][is]->GetYaxis()->SetRangeUser(-0.2,0.4); 
			if (hdt_end_n_sigm[idet][is]->GetEntries() > 0){
				hdt_end_n_sigm[idet][is]->SetMarkerStyle(20);
				hdt_end_n_sigm[idet][is]->SetMarkerColor( 4);
				hdt_end_n_sigm[idet][is]->Draw("P");
				hdt_end_n_mean[idet][is]->SetMarkerStyle(24);
				hdt_end_n_mean[idet][is]->SetMarkerColor( 6);
				hdt_end_n_mean[idet][is]->Draw("P same");
				//
				sprintf(buf,"fres%d",idetew);
				hdt_end_n_sigm[idet][is]->Fit(buf,"NQR");
					fres[idetew]->Draw("same");
					hdt_end_n_sigm[idet][is]->Draw("P same");
					fres[idetew]->GetParameters(frespar);
					frespare[0] = fres[idetew]->GetParError(0);
					chisq		= fres[idetew]->GetChisquare();
					npfit		= fres[idetew]->GetNumberFitPoints();
					if (npfit>1){
						chisq_dof	= chisq/((Float_t)(npfit-1));	// one parameter fit....
					}
				//
					hsigma0->SetBinContent(idetew+1,frespar[0]);
					hsigma0->SetBinError(  idetew+1,frespare[0]);
					hchisq0->SetBinContent(idetew+1,chisq_dof);
				//
				sprintf(buf,"fresh%d",idetew);
				hdt_end_n_sigm[idet][is]->Fit(buf,"+NQR");
					fresh[idetew]->Draw("same");
					//hdt_end_n_sigm[idet][is]->Draw("P same");
					fresh[idetew]->GetParameters(freshpar);
					freshpare[0] = fresh[idetew]->GetParError(0);
					chisqh		 = fresh[idetew]->GetChisquare();
					npfith		 = fresh[idetew]->GetNumberFitPoints();
					if (npfith>1){
						chisqh_dof	= chisqh/((Float_t)(npfith-1));	// one parameter fit....
					}
					hsigma0h->SetBinContent(idetew+1,freshpar[0]);
					hsigma0h->SetBinError(  idetew+1,freshpare[0]);
					hchisq0h->SetBinContent(idetew+1,chisqh_dof);
				//
				//cout<<is<<" "<<idet	<<" ][ "<<npfit <<"\t "<<frespar[0] <<" +/- "<<frespare[0]
				//					<<" ][ "<<npfith<<"\t "<<freshpar[0]<<" +/- "<<freshpare[0]
				//					<<endl;
				//
				sprintf(buftext,"idet= %d",idet);
					text->DrawLatex(0.15,0.86,buftext);
				sprintf(buftext,"iside= %d",is);
					text->DrawLatex(0.15,0.81,buftext);
				sprintf(buftext,"idetew= %d",idetew);
					text->DrawLatex(0.15,0.76,buftext);
				sprintf(buftext,"#sigma=%6.3f",frespar[0]);
					text->DrawLatex(0.15,0.71,buftext);
				sprintf(buftext,"#chi^{2}/DOF=%6.3f",chisq_dof);
					text->DrawLatex(0.15,0.66,buftext);
			}
			if (ipad==8 || (is==1&&idet==NDETSIDE-1)){
				ccan[ncan]->Update();
			}
		}
	}

	int nDay	= (iDayMax-iDayMin+1);
	TF1 *fres_det0_day[2*nDay];
	TF1 *fres_det1_day[2*nDay];
	TF1 *fres_det2_day[2*nDay];
	TF1 *fres_det3_day[2*nDay];
	for (int i=0;i<2*nDay;i++){
		sprintf(buf,"fres_det0_day%d",i);
		fres_det0_day[i]	= new TF1(buf,funres,0.5,19.5,1);
			fres_det0_day[i]->SetRange(0.5,19.5);
			fres_det0_day[i]->SetLineColor(1);
		sprintf(buf,"fres_det1_day%d",i);
		fres_det1_day[i]	= new TF1(buf,funres,0.5,19.5,1);
			fres_det1_day[i]->SetRange(0.5,19.5);
			fres_det1_day[i]->SetLineColor(3);
		sprintf(buf,"fres_det2_day%d",i);
		fres_det2_day[i]	= new TF1(buf,funres,0.5,19.5,1);
			fres_det2_day[i]->SetRange(0.5,19.5);
			fres_det2_day[i]->SetLineColor(3);
		sprintf(buf,"fres_det3_day%d",i);
		fres_det3_day[i]	= new TF1(buf,funres,0.5,19.5,1);
			fres_det3_day[i]->SetRange(0.5,19.5);
			fres_det3_day[i]->SetLineColor(3);
	}
	TH1D *hsigma0_is0_det0_day = new TH1D("hsigma0_is0_det0_day","hsigma0_is0_det0_day",nDay,((float)iDayMin)-0.5,((float)iDayMax)+0.5);
			hsigma0_is0_det0_day->SetMarkerStyle(20);
			hsigma0_is0_det0_day->SetMarkerColor( 1);
	TH1D *hsigma0_is1_det0_day = new TH1D("hsigma0_is1_det0_day","hsigma0_is1_det0_day",nDay,((float)iDayMin)-0.5,((float)iDayMax)+0.5);
			hsigma0_is1_det0_day->SetMarkerStyle(21);
			hsigma0_is1_det0_day->SetMarkerColor( 2);
	TH1D *hsigma0_is0_det1_day = new TH1D("hsigma0_is0_det1_day","hsigma0_is0_det1_day",nDay,((float)iDayMin)-0.5,((float)iDayMax)+0.5);
			hsigma0_is0_det1_day->SetMarkerStyle(22);
			hsigma0_is0_det1_day->SetMarkerColor( 4);
	TH1D *hsigma0_is1_det1_day = new TH1D("hsigma0_is1_det1_day","hsigma0_is1_det1_day",nDay,((float)iDayMin)-0.5,((float)iDayMax)+0.5);
			hsigma0_is1_det1_day->SetMarkerStyle(23);
			hsigma0_is1_det1_day->SetMarkerColor( 6);
	TH1D *hsigma0_is0_det2_day = new TH1D("hsigma0_is0_det2_day","hsigma0_is0_det2_day",nDay,((float)iDayMin)-0.5,((float)iDayMax)+0.5);
			hsigma0_is0_det2_day->SetMarkerStyle(20);
			hsigma0_is0_det2_day->SetMarkerColor( 1);
	TH1D *hsigma0_is1_det2_day = new TH1D("hsigma0_is1_det2_day","hsigma0_is1_det2_day",nDay,((float)iDayMin)-0.5,((float)iDayMax)+0.5);
			hsigma0_is1_det2_day->SetMarkerStyle(21);
			hsigma0_is1_det2_day->SetMarkerColor( 2);
	TH1D *hsigma0_is0_det3_day = new TH1D("hsigma0_is0_det3_day","hsigma0_is0_det3_day",nDay,((float)iDayMin)-0.5,((float)iDayMax)+0.5);
			hsigma0_is0_det3_day->SetMarkerStyle(22);
			hsigma0_is0_det3_day->SetMarkerColor( 4);
	TH1D *hsigma0_is1_det3_day = new TH1D("hsigma0_is1_det3_day","hsigma0_is1_det3_day",nDay,((float)iDayMin)-0.5,((float)iDayMax)+0.5);
			hsigma0_is1_det3_day->SetMarkerStyle(23);
			hsigma0_is1_det3_day->SetMarkerColor( 6);
	ipad = 0;
	int	idtew;
	for (int is=0;is<2;is++){
		//
		//---- day loop....
		for (int iday=iDayMin;iday<=iDayMax;iday++){
			idt		= iday - iDayMin;
			idtew	= idt + is*nDay;
			//
			if (hdt_end_n_det0_day_sigm[idt][is]->GetEntries()==0){ 
				cout<<"no Day entries... "<<iday<<" "<<is<<endl;
				continue; 
			}
			//
// 			if (ipad==0 || ipad==8 ){ 		// || (is==0&&idet==0)){
// 				++ncan;
// 				ipad = 0;
// 				psout->NewPage();
// 				sprintf(buf,"ccan%d",ncan);
// 				ccan[ncan] = new TCanvas(buf,buf,0,0,800,600);
// 				ccan[ncan]->SetFillColor(10);
// 				ccan[ncan]->cd(); 
// 				ccan[ncan]->Divide(4,2,0.0001,0.0001);
// 			}
//			ccan[ncan]->cd(++ipad);
			hdt_end_n_det0_day_sigm[idt][is]->GetYaxis()->SetRangeUser(0.0,0.4); 
			hdt_end_n_det1_day_sigm[idt][is]->GetYaxis()->SetRangeUser(0.0,0.4); 
			hdt_end_n_det2_day_sigm[idt][is]->GetYaxis()->SetRangeUser(0.0,0.4); 
			hdt_end_n_det3_day_sigm[idt][is]->GetYaxis()->SetRangeUser(0.0,0.4); 
			if (hdt_end_n_det0_day_sigm[idt][is]->GetEntries() > 0){
				hdt_end_n_det0_day_sigm[idt][is]->SetMarkerStyle(22);
				hdt_end_n_det0_day_sigm[idt][is]->SetMarkerColor( 1);
//				hdt_end_n_det0_day_sigm[idt][is]->Draw("P");
				//
				sprintf(buf,"fres_det0_day%d",idtew);
				hdt_end_n_det0_day_sigm[idt][is]->Fit(buf,"NQR");
//					fres_det0_day[idtew]->Draw("same");
//					hdt_end_n_det0_day_sigm[idt][is]->Draw("P same");
					fres_det0_day[idtew]->GetParameters(frespar);
					frespare[0] = fres_det0_day[idtew]->GetParError(0);
					chisq		= fres_det0_day[idtew]->GetChisquare();
					npfit		= fres_det0_day[idtew]->GetNumberFitPoints();
					if (npfit>1){
						chisq_dof	= chisq/((Float_t)(npfit-1));	// one parameter fit....
					}
					//cout<<" DAY "<<iday<<" ... ch=0,is="<<is<<"\t "<<frespar[0]<<" +/- "<<frespare[0]<<"\t "
					//	<<npfit<<" "<<chisq<<" "<<chisq_dof<<endl;	
					if (is==0){
						hsigma0_is0_det0_day->SetBinContent(idt+1,frespar[0]);
						hsigma0_is0_det0_day->SetBinError(  idt+1,frespare[0]); 
					} else if (is==1){
						hsigma0_is1_det0_day->SetBinContent(idt+1,frespar[0]);
						hsigma0_is1_det0_day->SetBinError(  idt+1,frespare[0]);
					}
				//
// 				sprintf(buftext,"iDay= %d",iday);
// 					text->DrawLatex(0.15,0.84,buftext);
// 				sprintf(buftext,"iside= %d",is);
// 					text->DrawLatex(0.15,0.79,buftext);
// 				sprintf(buftext,"idtew= %d",idtew);
// 					text->DrawLatex(0.15,0.74,buftext);
// 				sprintf(buftext,"#chi^{2}/DOF=%6.3f",chisq_dof);
// 					text->DrawLatex(0.15,0.69,buftext);
			}
			if (hdt_end_n_det1_day_sigm[idt][is]->GetEntries() > 0){
				hdt_end_n_det1_day_sigm[idt][is]->SetMarkerStyle(23);
				hdt_end_n_det1_day_sigm[idt][is]->SetMarkerColor( 3);
//				hdt_end_n_det1_day_sigm[idt][is]->Draw("P same");
				//
				sprintf(buf,"fres_det1_day%d",idtew);
				hdt_end_n_det1_day_sigm[idt][is]->Fit(buf,"NQR");
//					fres_det1_day[idtew]->Draw("same");
//					hdt_end_n_det1_day_sigm[idt][is]->Draw("P same");
					fres_det1_day[idtew]->GetParameters(frespar);
					frespare[0] = fres_det1_day[idtew]->GetParError(0);
					chisq		= fres_det1_day[idtew]->GetChisquare();
					npfit		= fres_det1_day[idtew]->GetNumberFitPoints();
					if (npfit>1){
						chisq_dof	= chisq/((Float_t)(npfit-1));	// one parameter fit....
					}
					if (is==0){
						hsigma0_is0_det1_day->SetBinContent(idt+1,frespar[0]);
						hsigma0_is0_det1_day->SetBinError(  idt+1,frespare[0]); 
					} else if (is==1){
						hsigma0_is1_det1_day->SetBinContent(idt+1,frespar[0]);
						hsigma0_is1_det1_day->SetBinError(  idt+1,frespare[0]);
					}
			}
			if (hdt_end_n_det2_day_sigm[idt][is]->GetEntries() > 0){
				hdt_end_n_det2_day_sigm[idt][is]->SetMarkerStyle(23);
				hdt_end_n_det2_day_sigm[idt][is]->SetMarkerColor( 3);
//				hdt_end_n_det2_day_sigm[idt][is]->Draw("P same");
				//
				sprintf(buf,"fres_det2_day%d",idtew);
				hdt_end_n_det2_day_sigm[idt][is]->Fit(buf,"NQR");
					fres_det2_day[idtew]->GetParameters(frespar);
					frespare[0] = fres_det2_day[idtew]->GetParError(0);
					chisq		= fres_det2_day[idtew]->GetChisquare();
					npfit		= fres_det2_day[idtew]->GetNumberFitPoints();
					if (npfit>1){
						chisq_dof	= chisq/((Float_t)(npfit-1));	// one parameter fit....
					}
					if (is==0){
						hsigma0_is0_det2_day->SetBinContent(idt+1,frespar[0]);
						hsigma0_is0_det2_day->SetBinError(  idt+1,frespare[0]); 
					} else if (is==1){
						hsigma0_is1_det2_day->SetBinContent(idt+1,frespar[0]);
						hsigma0_is1_det2_day->SetBinError(  idt+1,frespare[0]);
					}
			}
			if (hdt_end_n_det3_day_sigm[idt][is]->GetEntries() > 0){
				hdt_end_n_det3_day_sigm[idt][is]->SetMarkerStyle(23);
				hdt_end_n_det3_day_sigm[idt][is]->SetMarkerColor( 3);
//				hdt_end_n_det3_day_sigm[idt][is]->Draw("P same");
				//
				sprintf(buf,"fres_det3_day%d",idtew);
				hdt_end_n_det3_day_sigm[idt][is]->Fit(buf,"NQR");
					fres_det3_day[idtew]->GetParameters(frespar);
					frespare[0] = fres_det3_day[idtew]->GetParError(0);
					chisq		= fres_det3_day[idtew]->GetChisquare();
					npfit		= fres_det3_day[idtew]->GetNumberFitPoints();
					if (npfit>1){
						chisq_dof	= chisq/((Float_t)(npfit-1));	// one parameter fit....
					}
					if (is==0){
						hsigma0_is0_det3_day->SetBinContent(idt+1,frespar[0]);
						hsigma0_is0_det3_day->SetBinError(  idt+1,frespare[0]); 
					} else if (is==1){
						hsigma0_is1_det3_day->SetBinContent(idt+1,frespar[0]);
						hsigma0_is1_det3_day->SetBinError(  idt+1,frespare[0]);
					}
			}
// 			if (ipad==8 || (is==1&&idt==nDay-1)){
// 				ccan[ncan]->Update();
// 			}
		}
	}

	TF1 *fres_det0_time[2*dtimenbins];
	TF1 *fres_det1_time[2*dtimenbins];
	TF1 *fres_det2_time[2*dtimenbins];
	TF1 *fres_det3_time[2*dtimenbins];
	for (int i=0;i<2*dtimenbins;i++){
		sprintf(buf,"fres_det0_time%d",i);
		fres_det0_time[i]	= new TF1(buf,funres,0.5,19.5,1);
			fres_det0_time[i]->SetRange(0.5,19.5);
			fres_det0_time[i]->SetLineColor(1);
		sprintf(buf,"fres_det1_time%d",i);
		fres_det1_time[i]	= new TF1(buf,funres,0.5,19.5,1);
			fres_det1_time[i]->SetRange(0.5,19.5);
			fres_det1_time[i]->SetLineColor(3);
		sprintf(buf,"fres_det2_time%d",i);
		fres_det2_time[i]	= new TF1(buf,funres,0.5,19.5,1);
			fres_det2_time[i]->SetRange(0.5,19.5);
			fres_det2_time[i]->SetLineColor(3);
		sprintf(buf,"fres_det3_time%d",i);
		fres_det3_time[i]	= new TF1(buf,funres,0.5,19.5,1);
			fres_det3_time[i]->SetRange(0.5,19.5);
			fres_det3_time[i]->SetLineColor(3);
	}
	TH1D *hsigma0_is0_det0_time = new TH1D("hsigma0_is0_det0_time","hsigma0_is0_det0_time",dtimenbins,-0.5,((float)dtimenbins)-0.5);
			hsigma0_is0_det0_time->SetMarkerStyle(20);
			hsigma0_is0_det0_time->SetMarkerColor( 1);
	TH1D *hsigma0_is1_det0_time = new TH1D("hsigma0_is1_det0_time","hsigma0_is1_det0_time",dtimenbins,-0.5,((float)dtimenbins)-0.5);
			hsigma0_is1_det0_time->SetMarkerStyle(21);
			hsigma0_is1_det0_time->SetMarkerColor( 2);
	TH1D *hsigma0_is0_det1_time = new TH1D("hsigma0_is0_det1_time","hsigma0_is0_det1_time",dtimenbins,-0.5,((float)dtimenbins)-0.5);
			hsigma0_is0_det1_time->SetMarkerStyle(22);
			hsigma0_is0_det1_time->SetMarkerColor( 4);
	TH1D *hsigma0_is1_det1_time = new TH1D("hsigma0_is1_det1_time","hsigma0_is1_det1_time",dtimenbins,-0.5,((float)dtimenbins)-0.5);
			hsigma0_is1_det1_time->SetMarkerStyle(23);
			hsigma0_is1_det1_time->SetMarkerColor( 6);
	TH1D *hsigma0_is0_det2_time = new TH1D("hsigma0_is0_det2_time","hsigma0_is0_det2_time",dtimenbins,-0.5,((float)dtimenbins)-0.5);
			hsigma0_is0_det2_time->SetMarkerStyle(20);
			hsigma0_is0_det2_time->SetMarkerColor( 1);
	TH1D *hsigma0_is1_det2_time = new TH1D("hsigma0_is1_det2_time","hsigma0_is1_det2_time",dtimenbins,-0.5,((float)dtimenbins)-0.5);
			hsigma0_is1_det2_time->SetMarkerStyle(21);
			hsigma0_is1_det2_time->SetMarkerColor( 2);
	TH1D *hsigma0_is0_det3_time = new TH1D("hsigma0_is0_det3_time","hsigma0_is0_det3_time",dtimenbins,-0.5,((float)dtimenbins)-0.5);
			hsigma0_is0_det3_time->SetMarkerStyle(22);
			hsigma0_is0_det3_time->SetMarkerColor( 4);
	TH1D *hsigma0_is1_det3_time = new TH1D("hsigma0_is1_det3_time","hsigma0_is1_det3_time",dtimenbins,-0.5,((float)dtimenbins)-0.5);
			hsigma0_is1_det3_time->SetMarkerStyle(23);
			hsigma0_is1_det3_time->SetMarkerColor( 6);
	ipad = 0;
	for (int is=0;is<2;is++){
		//
		//---- day loop....
		for (int idt=0;idt<dtimenbins;idt++){
			idtew	= idt + is*nDay;
			//
			if (hdt_end_n_det0_time_sigm[idt][is]->GetEntries()==0){ continue; }
			//
// 			if (ipad==0 || ipad==8 ){ 		// || (is==0&&idet==0)){
// 				++ncan;
// 				ipad = 0;
// 				psout->NewPage();
// 				sprintf(buf,"ccan%d",ncan);
// 				ccan[ncan] = new TCanvas(buf,buf,0,0,800,600);
// 				ccan[ncan]->SetFillColor(10);
// 				ccan[ncan]->cd(); 
// 				ccan[ncan]->Divide(4,2,0.0001,0.0001);
// 			}
//			ccan[ncan]->cd(++ipad);
			hdt_end_n_det0_time_sigm[idt][is]->GetYaxis()->SetRangeUser(0.0,0.4); 
			hdt_end_n_det1_time_sigm[idt][is]->GetYaxis()->SetRangeUser(0.0,0.4); 
			hdt_end_n_det2_time_sigm[idt][is]->GetYaxis()->SetRangeUser(0.0,0.4); 
			hdt_end_n_det3_time_sigm[idt][is]->GetYaxis()->SetRangeUser(0.0,0.4); 
			if (hdt_end_n_det0_time_sigm[idt][is]->GetEntries() > 0){
				hdt_end_n_det0_time_sigm[idt][is]->SetMarkerStyle(22);
				hdt_end_n_det0_time_sigm[idt][is]->SetMarkerColor( 1);
//				hdt_end_n_det0_time_sigm[idt][is]->Draw("P");
				//
				sprintf(buf,"fres_det0_time%d",idtew);
				hdt_end_n_det0_time_sigm[idt][is]->Fit(buf,"NQR");
//					fres_det0_time[idtew]->Draw("same");
//					hdt_end_n_det0_time_sigm[idt][is]->Draw("P same");
					fres_det0_time[idtew]->GetParameters(frespar);
					frespare[0] = fres_det0_time[idtew]->GetParError(0);
					chisq		= fres_det0_time[idtew]->GetChisquare();
					npfit		= fres_det0_time[idtew]->GetNumberFitPoints();
					if (npfit>1){
						chisq_dof	= chisq/((Float_t)(npfit-1));	// one parameter fit....
					}
					//cout<<"TIME "<<idt<<" ... ch=0,is="<<is<<"\t "<<frespar[0]<<" +/- "<<frespare[0]<<"\t "
					//	<<npfit<<" "<<chisq<<" "<<chisq_dof<<endl;	
					if (is==0){
						hsigma0_is0_det0_time->SetBinContent(idt+1,frespar[0]);
						hsigma0_is0_det0_time->SetBinError(  idt+1,frespare[0]); 
					} else if (is==1){
						hsigma0_is1_det0_time->SetBinContent(idt+1,frespar[0]);
						hsigma0_is1_det0_time->SetBinError(  idt+1,frespare[0]);
					}
				//
// 				sprintf(buftext,"idtime= %d",idt);
// 					text->DrawLatex(0.15,0.84,buftext);
// 				sprintf(buftext,"iside= %d",is);
// 					text->DrawLatex(0.15,0.79,buftext);
// 				sprintf(buftext,"idtew= %d",idtew);
// 					text->DrawLatex(0.15,0.74,buftext);
// 				sprintf(buftext,"#chi^{2}/DOF=%6.3f",chisq_dof);
// 					text->DrawLatex(0.15,0.69,buftext);
			}
			if (hdt_end_n_det1_time_sigm[idt][is]->GetEntries() > 0){
				hdt_end_n_det1_time_sigm[idt][is]->SetMarkerStyle(23);
				hdt_end_n_det1_time_sigm[idt][is]->SetMarkerColor( 3);
//				hdt_end_n_det1_time_sigm[idt][is]->Draw("P same");
				//
				sprintf(buf,"fres_det1_time%d",idtew);
				hdt_end_n_det1_time_sigm[idt][is]->Fit(buf,"NQR");
//					fres_det1_time[idtew]->Draw("same");
//					hdt_end_n_det1_time_sigm[idt][is]->Draw("P same");
					fres_det1_time[idtew]->GetParameters(frespar);
					frespare[0] = fres_det1_time[idtew]->GetParError(0);
					chisq		= fres_det1_time[idtew]->GetChisquare();
					npfit		= fres_det1_time[idtew]->GetNumberFitPoints();
					if (npfit>1){
						chisq_dof	= chisq/((Float_t)(npfit-1));	// one parameter fit....
					}
					if (is==0){
						hsigma0_is0_det1_time->SetBinContent(idt+1,frespar[0]);
						hsigma0_is0_det1_time->SetBinError(  idt+1,frespare[0]); 
					} else if (is==1){
						hsigma0_is1_det1_time->SetBinContent(idt+1,frespar[0]);
						hsigma0_is1_det1_time->SetBinError(  idt+1,frespare[0]);
					}
			}
			if (hdt_end_n_det2_time_sigm[idt][is]->GetEntries() > 0){
				hdt_end_n_det2_time_sigm[idt][is]->SetMarkerStyle(23);
				hdt_end_n_det2_time_sigm[idt][is]->SetMarkerColor( 3);
//				hdt_end_n_det2_time_sigm[idt][is]->Draw("P same");
				//
				sprintf(buf,"fres_det2_time%d",idtew);
				hdt_end_n_det2_time_sigm[idt][is]->Fit(buf,"NQR");
					fres_det2_time[idtew]->GetParameters(frespar);
					frespare[0] = fres_det2_time[idtew]->GetParError(0);
					chisq		= fres_det2_time[idtew]->GetChisquare();
					npfit		= fres_det2_time[idtew]->GetNumberFitPoints();
					if (npfit>1){
						chisq_dof	= chisq/((Float_t)(npfit-1));	// one parameter fit....
					}
					if (is==0){
						hsigma0_is0_det2_time->SetBinContent(idt+1,frespar[0]);
						hsigma0_is0_det2_time->SetBinError(  idt+1,frespare[0]); 
					} else if (is==1){
						hsigma0_is1_det2_time->SetBinContent(idt+1,frespar[0]);
						hsigma0_is1_det2_time->SetBinError(  idt+1,frespare[0]);
					}
			}
			if (hdt_end_n_det3_time_sigm[idt][is]->GetEntries() > 0){
				hdt_end_n_det3_time_sigm[idt][is]->SetMarkerStyle(23);
				hdt_end_n_det3_time_sigm[idt][is]->SetMarkerColor( 3);
//				hdt_end_n_det3_time_sigm[idt][is]->Draw("P same");
				//
				sprintf(buf,"fres_det3_time%d",idtew);
				hdt_end_n_det3_time_sigm[idt][is]->Fit(buf,"NQR");
					fres_det3_time[idtew]->GetParameters(frespar);
					frespare[0] = fres_det3_time[idtew]->GetParError(0);
					chisq		= fres_det3_time[idtew]->GetChisquare();
					npfit		= fres_det3_time[idtew]->GetNumberFitPoints();
					if (npfit>1){
						chisq_dof	= chisq/((Float_t)(npfit-1));	// one parameter fit....
					}
					if (is==0){
						hsigma0_is0_det3_time->SetBinContent(idt+1,frespar[0]);
						hsigma0_is0_det3_time->SetBinError(  idt+1,frespare[0]); 
					} else if (is==1){
						hsigma0_is1_det3_time->SetBinContent(idt+1,frespar[0]);
						hsigma0_is1_det3_time->SetBinError(  idt+1,frespare[0]);
					}
			}
// 			if (ipad==8 || (is==1&&idt==nDay-1)){
// 				ccan[ncan]->Update();
// 			}
		}
	}

	text->SetTextSize(0.05);
	ipad = 0;
	++ncan;
	psout->NewPage();
	sprintf(buf,"ccan%d",ncan);
	ccan[ncan] = new TCanvas(buf,buf,0,0,800,600);
	ccan[ncan]->SetFillColor(10);
	ccan[ncan]->cd(); ccan[ncan]->Divide(1,1,0.0001,0.0001);
	ccan[ncan]->cd(1);
		hsigma0->SetMinimum(0.0);
		hsigma0->SetMaximum(0.4);
		hsigma0->SetStats(0);
		hsigma0h->SetStats(0);
		sprintf(buftext,"#sigma_{0} (ns) by detector channel...");
		hsigma0->Draw("P");
		hsigma0h->Draw("P same");
			text->DrawLatex(0.19,0.82,buftext);
	ccan[ncan]->Update();
	ipad = 0;
	++ncan;
	psout->NewPage();
	sprintf(buf,"ccan%d",ncan);
	ccan[ncan] = new TCanvas(buf,buf,0,0,800,600);
	ccan[ncan]->SetFillColor(10);
	ccan[ncan]->cd(); ccan[ncan]->Divide(1,1,0.0001,0.0001);
	ccan[ncan]->cd(1);
		hchisq0->SetMinimum( 0.05);
		hchisq0->SetMaximum(50.00);
		hchisq0->SetStats(0);
		hchisq0h->SetStats(0);
		sprintf(buftext,"#chi^{2}/DOF of #sigma_{0} fit by detector channel...");
		hchisq0->Draw("P");
		hchisq0h->Draw("P same");
			gPad->SetLogy(1);
			text->DrawLatex(0.19,0.82,buftext);
	ccan[ncan]->Update();
	ipad = 0;
	++ncan;
	psout->NewPage();
	sprintf(buf,"ccan%d",ncan);
	ccan[ncan] = new TCanvas(buf,buf,0,0,800,600);
	ccan[ncan]->SetFillColor(10);
	ccan[ncan]->cd(); ccan[ncan]->Divide(2,2,0.0001,0.0001);
	float amax;
	if (iChoice==0||iChoice==2){ amax = 0.20; } else { amax = 0.5; }
		hsigma0_is0_det0_day->SetMinimum(0.07);
		hsigma0_is1_det0_day->SetMinimum(0.07);
		hsigma0_is0_det1_day->SetMinimum(0.07);
		hsigma0_is1_det1_day->SetMinimum(0.07);
		hsigma0_is0_det2_day->SetMinimum(0.07);
		hsigma0_is1_det2_day->SetMinimum(0.07);
		hsigma0_is0_det3_day->SetMinimum(0.07);
		hsigma0_is1_det3_day->SetMinimum(0.07);
		hsigma0_is0_det0_day->SetMaximum(amax);
		hsigma0_is1_det0_day->SetMaximum(amax);
		hsigma0_is0_det1_day->SetMaximum(amax);
		hsigma0_is1_det1_day->SetMaximum(amax);
		hsigma0_is0_det2_day->SetMaximum(amax);
		hsigma0_is1_det2_day->SetMaximum(amax);
		hsigma0_is0_det3_day->SetMaximum(amax);
		hsigma0_is1_det3_day->SetMaximum(amax);
		hsigma0_is0_det0_day->SetStats(0);
		hsigma0_is1_det0_day->SetStats(0);
		hsigma0_is0_det1_day->SetStats(0);
		hsigma0_is1_det1_day->SetStats(0);
		hsigma0_is0_det2_day->SetStats(0);
		hsigma0_is1_det2_day->SetStats(0);
		hsigma0_is0_det3_day->SetStats(0);
		hsigma0_is1_det3_day->SetStats(0);
		sprintf(buftext,"#sigma_{0} (ns) by Day Number");
	ccan[ncan]->cd(1);
		hsigma0_is0_det0_day->Draw("P");
			text->DrawLatex(0.19,0.82,buftext);
	ccan[ncan]->cd(2);
		hsigma0_is1_det0_day->Draw("P");
	ccan[ncan]->cd(3);
		hsigma0_is0_det1_day->Draw("P");
	ccan[ncan]->cd(4);
		hsigma0_is1_det1_day->Draw("P");
	ccan[ncan]->Update();

	ipad = 0;
	++ncan;
	psout->NewPage();
	sprintf(buf,"ccan%d",ncan);
	ccan[ncan] = new TCanvas(buf,buf,0,0,800,600);
	ccan[ncan]->SetFillColor(10);
	ccan[ncan]->cd(); ccan[ncan]->Divide(2,2,0.0001,0.0001);
	if (iChoice==0||iChoice==2){ amax = 0.20; } else { amax = 0.5; }
		hsigma0_is0_det0_time->SetMinimum(0.07);
		hsigma0_is1_det0_time->SetMinimum(0.07);
		hsigma0_is0_det1_time->SetMinimum(0.07);
		hsigma0_is1_det1_time->SetMinimum(0.07);
		hsigma0_is0_det2_time->SetMinimum(0.07);
		hsigma0_is1_det2_time->SetMinimum(0.07);
		hsigma0_is0_det3_time->SetMinimum(0.07);
		hsigma0_is1_det3_time->SetMinimum(0.07);
		hsigma0_is0_det0_time->SetMaximum(amax);
		hsigma0_is1_det0_time->SetMaximum(amax);
		hsigma0_is0_det1_time->SetMaximum(amax);
		hsigma0_is1_det1_time->SetMaximum(amax);
		hsigma0_is0_det2_time->SetMaximum(amax);
		hsigma0_is1_det2_time->SetMaximum(amax);
		hsigma0_is0_det3_time->SetMaximum(amax);
		hsigma0_is1_det3_time->SetMaximum(amax);
		hsigma0_is0_det0_time->SetStats(0);
		hsigma0_is1_det0_time->SetStats(0);
		hsigma0_is0_det1_time->SetStats(0);
		hsigma0_is1_det1_time->SetStats(0);
		hsigma0_is0_det2_time->SetStats(0);
		hsigma0_is1_det2_time->SetStats(0);
		hsigma0_is0_det3_time->SetStats(0);
		hsigma0_is1_det3_time->SetStats(0);
		sprintf(buftext,"#sigma_{0} (ns) by 4hr time bin");
	ccan[ncan]->cd(1);
		hsigma0_is0_det0_time->Draw("P");
			text->DrawLatex(0.19,0.82,buftext);
	ccan[ncan]->cd(2);
		hsigma0_is1_det0_time->Draw("P");
	ccan[ncan]->cd(3);
		hsigma0_is0_det1_time->Draw("P");
	ccan[ncan]->cd(4);
		hsigma0_is1_det1_time->Draw("P");
	ccan[ncan]->Update();

	ipad = 0;
	++ncan;
	psout->NewPage();
	sprintf(buf,"ccan%d",ncan);
	ccan[ncan] = new TCanvas(buf,buf,0,0,800,600);
	ccan[ncan]->SetFillColor(10);
	ccan[ncan]->cd(); ccan[ncan]->Divide(1,2,0.0001,0.0001);
	ccan[ncan]->cd(1);
		hdt_end_det0_zvtx_mean[0]->SetMarkerStyle(20);
		hdt_end_det0_zvtx_mean[1]->SetMarkerStyle(21);
		hdt_end_det0_zvtx_mean[0]->SetMarkerSize(0.8);
		hdt_end_det0_zvtx_mean[1]->SetMarkerSize(0.8);
		hdt_end_det0_zvtx_mean[0]->SetMarkerColor(2);
		hdt_end_det0_zvtx_mean[1]->SetMarkerColor(4);
		hdt_end_det0_zvtx_mean[0]->SetLineColor(2);
		hdt_end_det0_zvtx_mean[1]->SetLineColor(4);
		hdt_end_det0_zvtx_mean[0]->SetMinimum(-0.2);
		hdt_end_det0_zvtx_mean[0]->SetMaximum( 0.2);
		hdt_end_det0_zvtx_mean[0]->Draw();
		hdt_end_det0_zvtx_mean[1]->Draw("same");
		sprintf(buftext,"#mu_{0} (ns) vs Z_{vtx}");
			text->DrawLatex(0.19,0.82,buftext);
	ccan[ncan]->cd(2);
		hdt_end_det0_zvtx_sigm[0]->SetMarkerStyle(20);
		hdt_end_det0_zvtx_sigm[1]->SetMarkerStyle(21);
		hdt_end_det0_zvtx_sigm[0]->SetMarkerSize(0.8);
		hdt_end_det0_zvtx_sigm[1]->SetMarkerSize(0.8);
		hdt_end_det0_zvtx_sigm[0]->SetMarkerColor(2);
		hdt_end_det0_zvtx_sigm[1]->SetMarkerColor(4);
		hdt_end_det0_zvtx_sigm[0]->SetLineColor(2);
		hdt_end_det0_zvtx_sigm[1]->SetLineColor(4);
		hdt_end_det0_zvtx_sigm[0]->SetMinimum( 0.0);
		hdt_end_det0_zvtx_sigm[0]->SetMaximum( 0.4);
		hdt_end_det0_zvtx_sigm[0]->Draw();
		hdt_end_det0_zvtx_sigm[1]->Draw("same");
		sprintf(buftext,"#sigma_{0} (ns) vs Z_{vtx}");
			text->DrawLatex(0.19,0.82,buftext);
	ccan[ncan]->Update();

	ipad = 0;
	++ncan;
	psout->NewPage();
	sprintf(buf,"ccan%d",ncan);
	ccan[ncan] = new TCanvas(buf,buf,0,0,800,600);
	ccan[ncan]->SetFillColor(10);
	ccan[ncan]->cd(); ccan[ncan]->Divide(1,2,0.0001,0.0001);
	ccan[ncan]->cd(1);
		hdt_end_chan_nLLb_mean->SetMinimum(-0.20);
		hdt_end_chan_nLLb_mean->SetMaximum( 0.20);
		hdt_end_chan_nLLb_mean->SetMarkerStyle(20);
		hdt_end_chan_nLLb_mean->Draw();
		hdt_end_chan_nLLa_mean->SetMarkerStyle(21);
		hdt_end_chan_nLLa_mean->SetMarkerColor(3);
		hdt_end_chan_nLLa_mean->Draw("same");
		sprintf(buftext,"#mu_{0} (ns) N#geq%d,N#geq%d by channel",LLa,LLb);
			text->DrawLatex(0.19,0.82,buftext);
	ccan[ncan]->cd(2);
		hdt_end_chan_nLLb_sigm->SetMinimum( 0.00);
		hdt_end_chan_nLLb_sigm->SetMaximum( 0.40);
		hdt_end_chan_nLLb_sigm->SetMarkerStyle(20);
		hdt_end_chan_nLLb_sigm->Draw();
		hdt_end_chan_nLLa_sigm->SetMarkerStyle(21);
		hdt_end_chan_nLLa_sigm->SetMarkerColor(3);
		hdt_end_chan_nLLa_sigm->Draw("same");
		sprintf(buftext,"#sigma_{0} (ns) N#geq%d,N#geq%d by channel",LLa,LLb);
			text->DrawLatex(0.19,0.82,buftext);
	ccan[ncan]->Update();

	ipad = 0;
	++ncan;
	psout->NewPage();
	sprintf(buf,"ccan%d",ncan);
	ccan[ncan] = new TCanvas(buf,buf,0,0,800,600);
	ccan[ncan]->SetFillColor(10);
	ccan[ncan]->cd(); ccan[ncan]->Divide(1,2,0.0001,0.0001);
	ccan[ncan]->cd(1);
		hdt_end_det0_grefmult_mean[0]->SetMarkerStyle(20);
		hdt_end_det0_grefmult_mean[1]->SetMarkerStyle(21);
		hdt_end_det0_grefmult_mean[0]->SetMarkerSize(0.8);
		hdt_end_det0_grefmult_mean[1]->SetMarkerSize(0.8);
		hdt_end_det0_grefmult_mean[0]->SetMarkerColor(2);
		hdt_end_det0_grefmult_mean[1]->SetMarkerColor(4);
		hdt_end_det0_grefmult_mean[0]->SetLineColor(2);
		hdt_end_det0_grefmult_mean[1]->SetLineColor(4);
		hdt_end_det0_grefmult_mean[0]->SetMinimum(-0.4);
		hdt_end_det0_grefmult_mean[0]->SetMaximum( 0.4);
		hdt_end_det0_grefmult_mean[0]->Draw();
		hdt_end_det0_grefmult_mean[1]->Draw("same");
		sprintf(buftext,"#mu_{0} (ns) vs refmult");
			text->DrawLatex(0.19,0.82,buftext);
	ccan[ncan]->cd(2);
		hdt_end_det0_grefmult_sigm[0]->SetMarkerStyle(20);
		hdt_end_det0_grefmult_sigm[1]->SetMarkerStyle(21);
		hdt_end_det0_grefmult_sigm[0]->SetMarkerSize(0.8);
		hdt_end_det0_grefmult_sigm[1]->SetMarkerSize(0.8);
		hdt_end_det0_grefmult_sigm[0]->SetMarkerColor(2);
		hdt_end_det0_grefmult_sigm[1]->SetMarkerColor(4);
		hdt_end_det0_grefmult_sigm[0]->SetLineColor(2);
		hdt_end_det0_grefmult_sigm[1]->SetLineColor(4);
		hdt_end_det0_grefmult_sigm[0]->SetMinimum( 0.0);
		hdt_end_det0_grefmult_sigm[0]->SetMaximum( 0.4);
		hdt_end_det0_grefmult_sigm[0]->Draw();
		hdt_end_det0_grefmult_sigm[1]->Draw("same");
		sprintf(buftext,"#sigma_{0} (ns) vs refmult");
			text->DrawLatex(0.19,0.82,buftext);
	ccan[ncan]->Update();

	fout->cd();
		hsigma0->Write();
		hchisq0->Write();
		hsigma0h->Write();
		hchisq0h->Write();
		hsigma0_is0_det0_day->Write();
		hsigma0_is1_det0_day->Write();
		hsigma0_is0_det1_day->Write();
		hsigma0_is1_det1_day->Write();
		hsigma0_is0_det2_day->Write();
		hsigma0_is1_det2_day->Write();
		hsigma0_is0_det3_day->Write();
		hsigma0_is1_det3_day->Write();	

	cout<<"..... Loop  Closing ps file...."<<endl;
	psout->Close();
	//
	cout<<"..... Loop  outfilebase = "<<outfilebase<<endl;
	sprintf(buf,"/usr/bin/ps2pdf ps/%s.ps ps/%s.pdf",outfilebase,outfilebase);
	cout<<"..... Loop  Executing ... "<<buf<<endl;
	gSystem->Exec(buf);
	sprintf(buf,"/bin/cp ps/%s.pdf /star/u/llope/WWW/files/",outfilebase);
	cout<<"..... Loop  Executing ... "<<buf<<endl;
	gSystem->Exec(buf);
	cout<<"..... Loop  Done...."<<endl;	
	
	//fout->Close();	
	
}

Bool_t cal::AdcTotCut(Float_t tot, Float_t adc)
{
	Bool_t decision	= kFALSE;
// 	if (tot<3.0){
// 		if (adc<50){ decision  = kTRUE; }
// 	} else if (tot<4.0){
// 		if (adc<70){ decision  = kTRUE; }
// 	} else if (tot<5.0){
// 		if (adc<90){ decision  = kTRUE; }
// 	} else if (tot<5.0){
// 		if (adc<100){ decision = kTRUE; }
// 	} else if (tot<6.0){
// 		if (adc<110){ decision = kTRUE; }
// 	} else if (tot<8.0){
// 		if (adc<130){ decision = kTRUE; }
// 	} else if (tot<10.0){
// 		if (adc<200){ decision = kTRUE; }
// 	} else {
// 		decision = kTRUE;
// 	}

	float valm	= funm->Eval(tot);
	float valu	= funu->Eval(tot);
	float vall	= funl->Eval(tot);
	if (  (adc<=valm+valu)
	   && (adc>=valm-vall) ){
		decision	= kTRUE;
	}
	return decision;
}

float FindPeak(TH1D* h){
	if (h->GetEntries()<100){ return 0; }
	int nbx = h->GetNbinsX();
	int kbx = h->GetMaximumBin();
	float ax = 0;
	float an = 0;
	for (int ibx=kbx-2;ibx<=kbx+2;ibx++){
		float x = h->GetBinCenter(ibx);
		float n = h->GetBinContent(ibx);
		ax += x*n;
		an += n;
	}	
	if (an>0){ ax /= an; } else { ax = 0; }
	return ax;
}






