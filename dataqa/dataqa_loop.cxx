#include "dataqa_class.h"

float spot_x	=  0;
float spot_y	= -1;

bool PLOTGOODEVENTS	= false;

void dataqa::Loop()
{

	char		buf[100],tbuf[100];
	TCanvas 	*ccan[1000];
	TLatex		*text[1000];
	TH1F		*frame[1000];
	Int_t		ican=-1,itext=-1,iframe=-1;
	for (int i=0;i<1000;i++){
		text[i]	= new TLatex();
		text[i]->SetNDC();
		text[i]->SetTextSize(0.06);
	}
	//
	cout<<".... kRunNum         = "<<kRunNum<<endl; 
	cout<<".... StreamName      = "<<StreamName.Data()<<endl; 
	sprintf(buf,"dataqa_%s_%d",StreamName.Data(),kRunNum);
	TString base	= TString(buf);
	cout<<".... filename base   = "<<base<<endl; 
	TString rootfile,psfile,psfileO,psfileC,pdffile;
		rootfile	= TString("./root/") + base + TString(".root");
		psfile		= TString("./ps/")   + base + TString(".ps");
		psfileO		= TString("./ps/")   + base + TString(".ps(");
		psfileC		= TString("./ps/")   + base + TString(".ps]");
		pdffile		= TString("./ps/")   + base + TString(".pdf");
	TString	WebDirectory	= TString("/star/u/geurts/WWW/files/");		// RCF
	cout<<"....        rootfile = "<<rootfile<<endl; 
	cout<<"....        psfile   = "<<psfile<<endl; 
	cout<<"....        psfileO  = "<<psfileO<<endl; 
	cout<<"....        psfileC  = "<<psfileC<<endl; 
	cout<<"....        pdffile  = "<<pdffile<<endl; 

	const int 	NTrig		= 15;
	int 		TrigBits[NTrig]		= {	 0,		//1
										 1,		//2
								 		 2,		//4
									 	 3,		//8
									 	 4,     //10
									 	 6,		//40
									 	 7, 	//80
									 	// 8, 	//100
									 	// 9,		//200
									 	//10,		//400
									 	11,		//800
									 	12,		//1000
									 	13,		//2000
	                                    14,		//4000
	                                    15,		//8000
	                                    16,		//10000
	                                    //19,		//80000
	                                    25,		//2000000
	                                    26};	//4000000
	const char* TrigNames[NTrig] 	= { "bbc_mon_tof",				// 1
										"hlt_good_vpd",				// 2
										"hlt-bes-mon",				// 4 
										"tofmult4",					// 8
										"mtdsingle",				// 10
										"mtd-double",				// 40
										"mtd-e-muon",				// 80
										//"fixed-target",			// 100
										//"fixed-target-hltgood",	// 200
										//"fixed-target-hltmon",	// 400
										"bbc_mon_tof_hltgood",		// 800
										"tofmult4-hltgood",			// 1000
										"BBC_mb-hltgood",			// 2000
	                                    "BBC_mb",					// 4000
	                                    "VPD_mb",					// 8000
	                                    "ZDC-mb",					// 10000
	                                    //"Zero-bias",				// 80000
	                                    "UPC_cosmic",				// 2000000
	                                    "VPD_mb-emcped"};			// 4000000
//	int 		TrigDraw[NTrig]		= {1,1,1,1, 
//	                                   1,1,1,0,
//	                                   };

	float nevt_seen[NTrig]			= {0};
	float nevt_good[NTrig]			= {0};
	float nevt_seen2D[NTrig][NTrig]	= {0};
	float nevt_good2D[NTrig][NTrig]	= {0};

	TFile *fout		= new TFile(rootfile.Data(),"RECREATE");
	fout->cd();
	//
	TH2D *hvtxyx[NTrig];
	TH2D *hvtxyz[NTrig];
	TH1D *hvtxr[NTrig];
	TH1D *hvtxz[NTrig];
	TH2D *hinvbetap[NTrig];
	TH1D *hmsq[NTrig];
	TH1D *htraymatched[NTrig];
	TH1D *htraypid[NTrig];
	TH1D *heta[NTrig];
	TH1D *hphi[NTrig];
	TH1D *hpt[NTrig];
	TH1D *hrefmult[NTrig];
	TH1D *hrefmultwide[NTrig];
	TH1D *hnprimary[NTrig];
	TH2D *hnprimaryzvtx[NTrig];
	TH2D *hnbtofzvtx[NTrig];
	TH2D *hnvpdrefmult[2][NTrig];
	TH1D *hpnvpd1refmult[2][NTrig];
	TH1D *hpnvpd11refmult[NTrig];
	for (int it=0;it<NTrig;it++){
		sprintf(buf,"hvtxyx%d",it);
		sprintf(tbuf,"Vertex y vs x");
		hvtxyx[it]		= new TH2D(buf,tbuf,100,-5,5,100,-5,5);
		sprintf(buf,"hvtxyz%d",it);
		sprintf(tbuf,"Vertex y vs z");
		hvtxyz[it]		= new TH2D(buf,tbuf,100,-200,200,100,-5,5);
		sprintf(buf,"hvtxr%d",it);
		sprintf(tbuf,"Vertex R");
		hvtxr[it]		= new TH1D(buf,tbuf,100,0,5);
		sprintf(buf,"hvtxz%d",it);
		sprintf(tbuf,"Vertex Z");
		hvtxz[it]		= new TH1D(buf,tbuf,200,-200,200);
		sprintf(buf,"hinvbetap%d",it);
		sprintf(tbuf,"TOF 1/#beta vs p");
		hinvbetap[it]	= new TH2D(buf,tbuf,300,0.,3.,300,0.8,3.8);
		sprintf(buf,"hmsq%d",it);
		sprintf(tbuf,"TOF M^{2}");
		hmsq[it]		= new TH1D(buf,tbuf,200,-0.5,1.5);
		sprintf(buf,"htraymatched%d",it);
		sprintf(tbuf,"TOF TrayID");
		htraymatched[it]= new TH1D(buf,tbuf,121,-0.5,120.5);
		sprintf(buf,"htraypid%d",it);
		sprintf(tbuf,"TOF TrayID");
		htraypid[it]	= new TH1D(buf,tbuf,121,-0.5,120.5);
		sprintf(buf,"heta%d",it);
		sprintf(tbuf,"Track #eta");
		heta[it]		= new TH1D(buf,tbuf,200,-2,2);
		sprintf(buf,"hphi%d",it);
		sprintf(tbuf,"Track #phi");
		hphi[it]		= new TH1D(buf,tbuf,63,-3.15,3.15);
		sprintf(buf,"hpt%d",it);
		sprintf(tbuf,"Track P_{T}");
		hpt[it]			= new TH1D(buf,tbuf,100,0,10);
		sprintf(buf,"hrefmult%d",it);
		sprintf(tbuf,"RefMult");
		hrefmult[it]	= new TH1D(buf,tbuf,100,0,400);
		sprintf(buf,"hrefmultwide%d",it);
		sprintf(tbuf,"RefMult wide bins");
		hrefmultwide[it]= new TH1D(buf,tbuf,50,0,400);
		sprintf(buf,"hnprimary%d",it);
		sprintf(tbuf,"Nprimary");
		hnprimary[it]	= new TH1D(buf,tbuf,100,0,900);
		sprintf(buf,"hnprimaryzvtx%d",it);
		sprintf(tbuf,"Nprimary vs Vertex Z");
		hnprimaryzvtx[it]	= new TH2D(buf,tbuf,50,-200,200,100,0,900);
		sprintf(buf,"hnbtofzvtx%d",it);
		sprintf(tbuf,"NTofCells vs Vertex Z");
		hnbtofzvtx[it]	= new TH2D(buf,tbuf,50,-200,200,100,0,1200);
		//
		for (int is=0;is<2;is++){
			sprintf(buf,"hnvpdrefmult%d%d",is,it);
			if (is==0){ sprintf(tbuf,"NvpdE vs Refmult"); } else
			if (is==1){ sprintf(tbuf,"NvpdW vs Refmult"); } 
			hnvpdrefmult[is][it]	= new TH2D(buf,tbuf,50,0,400,17,-0.5,16.5);
			sprintf(buf,"hpnvpd1refmult%d%d",is,it);
			if (is==0){ sprintf(tbuf,"Prob(NvpdE)>0 vs Refmult"); } else
			if (is==1){ sprintf(tbuf,"Prob(NvpdW)>0 vs Refmult"); } 
			hpnvpd1refmult[is][it]	= new TH1D(buf,tbuf,50,0,400);
		}
		sprintf(buf,"hpnvpd11refmult%d",it);
		sprintf(tbuf,"Prob(NvpdE&&W)>0 vs Refmult");
		hpnvpd11refmult[it]	= new TH1D(buf,tbuf,50,0,400);
		//
	}
	//
	TH1D *hnvertex		= new TH1D("hnvertex","hnvertex",20,-0.5,19.5);
	TH1D *hvtxidrankmax	= new TH1D("vtxidrankmax","vtxidrankmax",20,-0.5,19.5);
	TH1D *hvtxidntrkmax	= new TH1D("vtxidntrkmax","vtxidntrkmax",20,-0.5,19.5);
	TH1D *hvtx_rvtx[10];
	TH1D *hvtx_zvtx[10];
	TH1D *hvtx_rank[10];
	TH1D *hvtx_ntrk[10];
	for (int iv=0;iv<3;iv++){
		sprintf(buf,"hvtx_rvtx%d",iv);
		sprintf(tbuf,"Vertex%d R",iv);
		hvtx_rvtx[iv]		= new TH1D(buf,tbuf,100,0,5);
		sprintf(buf,"hvtx_zvtx%d",iv);
		sprintf(tbuf,"Vertex%d Z",iv);
		hvtx_zvtx[iv]		= new TH1D(buf,tbuf,200,-200,200);
		sprintf(buf,"hvtx_rank%d",iv);
		sprintf(tbuf,"Vertex%d Rank",iv);
		hvtx_rank[iv]		= new TH1D(buf,tbuf,160,-15.,25.);
		sprintf(buf,"hvtx_ntrk%d",iv);
		sprintf(tbuf,"Vertex%d Ntrack",iv);
		hvtx_ntrk[iv]		= new TH1D(buf,tbuf,100,0,900);
	}

	//---- start event loop;
	//
	if (fChain == 0) return;
	Long64_t nentriestree = fChain->GetEntries();
	Long64_t nentries;
	if (kNToDo==           0){ nentries	= nentriestree; } else
	if (kNToDo <nentriestree){ nentries	= kNToDo; } else
	if (kNToDo>=nentriestree){ nentries	= nentriestree; }
	cout<<"nentriestree="<<nentriestree<<endl;
	cout<<"nentries    ="<<nentries    <<endl;
	int ngood	= 0;
	//
	//nentries	= 1000;
	//
	Long64_t nbytes = 0, nb = 0;
	for (Long64_t jentry=0; jentry<nentries;jentry++) {
		Long64_t ientry = LoadTree(jentry);
		if (ientry < 0) break;
		nb = fChain->GetEntry(jentry);   nbytes += nb;
		if (jentry%2000==0){cout<<"Processing "<<jentry<<" of "<<nentries<<endl; }
		//
 		unsigned int idata	= MuEvent_mTriggerIdCollection_mNTriggerId_mMask[0];
 		int trgbits[32];
 		bool fired[NTrig]			= {0};
 		bool fired2D[NTrig][NTrig]	= {0};
 		//cout<<idata<<"\t";
 		for (int i=31;i>=0;i--){
 			if ((idata>>i) & 1){
 				//cout<<1;
 				trgbits[i]		= 1;
 				bool foundone	= false;
 				for (int it=0;it<NTrig;it++){
 					if (i==TrigBits[it]){ 
 						fired[it] = true; foundone = true; 
 					}
 				}
 				//if (!foundone){ cout<<i<<endl; }
 			} else {
 				trgbits[i]	= 0;
 				//cout<<0;
 			}
 		}		//cout<<endl;
		//
		float xvtx = MuEvent_mEventSummary_mPrimaryVertexPos_mX1[0];
		float yvtx = MuEvent_mEventSummary_mPrimaryVertexPos_mX2[0];
		float zvtx = MuEvent_mEventSummary_mPrimaryVertexPos_mX3[0];
		float rvtx = std::sqrt(xvtx*xvtx + yvtx*yvtx);
		float rvtx_spot = std::sqrt((xvtx-spot_x)*(xvtx-spot_x) + (yvtx-spot_y)*(yvtx-spot_y));
		int   nprimary = MuEvent_mEventSummary_mNumberOfGoodPrimaryTracks[0];
		//
		//if (rvtx_spot>1.0) continue; 
		//
		//
		bool goodevent  = false;
		if (rvtx_spot<1.5 && fabs(zvtx)<70.0 && fabs(zvtx)>0.001 && nprimary>5){
//		if (rvtx_spot<1.5 && fabs(zvtx)<30.0 && fabs(zvtx)>0.001){
			goodevent	= true;
			++ngood;
		}
		//
		//---- primary vertex plots
		float vtx_rvtx[100];
		float vtx_zvtx[100];
		float vtx_rank[100];
		float vtx_ntrk[100];
		hnvertex->Fill(PrimaryVertices_);
		int rankmax		= 0;
		int iv_rankmax	= 0;
		int ntrkmax		= 0;
		int iv_ntrkmax	= 0;
		for (int iv=0;iv<PrimaryVertices_;iv++){
			float x			= PrimaryVertices_mPosition_mX1[iv];
			float y			= PrimaryVertices_mPosition_mX2[iv];
			float z			= PrimaryVertices_mPosition_mX3[iv];
			float vtxrank	= PrimaryVertices_mRanking[iv];
			float vtxrankvar;
			if (vtxrank>1.0e6){
				vtxrankvar	= log10(vtxrank-1.0e6)+10.;
			} else if (vtxrank>0.0){
				vtxrankvar	= log10(vtxrank);
			} else if (vtxrank<0.0){
				vtxrankvar	= log10(vtxrank+1.0e6)-10.;
			}
			if (vtxrankvar>rankmax){
				rankmax		= vtxrankvar;
				iv_rankmax	= iv;
			}
			int ntrack	= PrimaryVertices_mNTracksUsed[iv];
			if (ntrack>ntrkmax){
				ntrkmax		= ntrack;
				iv_ntrkmax	= iv;
			}
			vtx_rvtx[iv]	= std::sqrt(x*x + y*y);
			vtx_zvtx[iv]	= z;
			vtx_rank[iv]	= vtxrankvar;
			vtx_ntrk[iv]	= ntrack;
			if (iv<3 && ((PLOTGOODEVENTS&&goodevent)||(!PLOTGOODEVENTS))){
				hvtx_rvtx[iv]->Fill(vtx_rvtx[iv]);
				hvtx_zvtx[iv]->Fill(vtx_zvtx[iv]);
				hvtx_rank[iv]->Fill(vtx_rank[iv]);
				hvtx_ntrk[iv]->Fill(vtx_ntrk[iv]);
			}
		}
		hvtxidrankmax->Fill(iv_rankmax);
		hvtxidntrkmax->Fill(iv_ntrkmax);
		//
		//for (int it=0;it<NTrig;it++){
		//	if (fired[it]){ 
		//		if ((PLOTGOODEVENTS&&goodevent)||(!PLOTGOODEVENTS)){
		//			hnvertex[it]->Fill(PrimaryVertices_);
		//		}
		//	}
		//}
		//
		for (int it=0;it<NTrig;it++){
			if (fired[it]){ 
				nevt_seen[it]		+= 1.0;
				if (goodevent){
					nevt_good[it]	+= 1.0;
				}
				if ((PLOTGOODEVENTS&&goodevent)||(!PLOTGOODEVENTS)){
					hvtxyx[it]->Fill(xvtx,yvtx);
					hvtxyz[it]->Fill(zvtx,yvtx);
					hvtxr[it]->Fill(rvtx);
					hvtxz[it]->Fill(zvtx);
				}
				//
				for (int jt=0;jt<NTrig;jt++){
					if (fired[jt] && jt<=it){
						nevt_seen2D[it][jt]		+= 1.0;
						if (goodevent){
							nevt_good2D[it][jt]	+= 1.0;
						}
					} 
				}
				//
			}
		}
		
		//---- track loop...
		int refmult	= 0;
		for (int k=0;k<PrimaryTracks_;k++){
			//
			int nhitsfit	= PrimaryTracks_mNHitsFit[k];
			//if (nhitsfit<20) continue;
			float dcax		= PrimaryTracks_mDCAGlobal_mX1[k];
			float dcay		= PrimaryTracks_mDCAGlobal_mX2[k];
			float dcaz		= PrimaryTracks_mDCAGlobal_mX3[k];
			float dca		= std::sqrt(dcax*dcax + dcay*dcay + dcaz*dcaz);
			//if (dca>1) continue;
			float px		= PrimaryTracks_mP_mX1[k];
			float py		= PrimaryTracks_mP_mX2[k];
			float pz		= PrimaryTracks_mP_mX3[k];
			float pt		= std::sqrt(px*px + py*py);
			float ptot		= std::sqrt(pt*pt + pz*pz);
			float eta		= PrimaryTracks_mEta[k];
			float phi		= PrimaryTracks_mPhi[k];
			//
			if (fabs(eta)<0.5) ++refmult;
			//
			for (int it=0;it<NTrig;it++){
				if ( ((PLOTGOODEVENTS&&goodevent)||(!PLOTGOODEVENTS)) && fired[it]){ 
					heta[it]->Fill(eta);
					hphi[it]->Fill(phi);
					hpt[it]->Fill(pt);
				}
			}
			//
			if (PrimaryTracks_mBTofPidTraits_mMatchFlag[k]>0){
				float ylocal	= PrimaryTracks_mBTofPidTraits_mYLocal[k];
				float beta		= PrimaryTracks_mBTofPidTraits_mBeta[k];
				int   ind		= PrimaryTracks_mIndex2BTofHit[k];
				int   itray		= BTofHit_mTray[ind];
				float invbeta	= -9;
				float msq		= -9;
				for (int it=0;it<NTrig;it++){
					if ( ((PLOTGOODEVENTS&&goodevent)||(!PLOTGOODEVENTS)) && fired[it]){ 
						htraymatched[it]->Fill(itray);
						if (beta>0.2 && fabs(ylocal)<1.8){
							invbeta		= 1./beta;
							msq			= ptot*ptot*(invbeta*invbeta - 1.0);
							hinvbetap[it]->Fill(ptot,invbeta);
							hmsq[it]->Fill(msq);
							htraypid[it]->Fill(itray);
						}
					}
				}
			}
			//
		}
		//---- end track loop...

		//---- global mult
		//
		for (int it=0;it<NTrig;it++){
			if (((PLOTGOODEVENTS&&goodevent)||(!PLOTGOODEVENTS)) && fired[it]){ 
				hrefmult[it]->Fill(refmult);
				hrefmultwide[it]->Fill(refmult);
				hnprimary[it]->Fill(PrimaryTracks_);
				hnprimaryzvtx[it]->Fill(zvtx,PrimaryTracks_);
				hnbtofzvtx[it]->Fill(zvtx,BTofHit_);
			}
		}
		//---- vpd
		//
		int vpdnew[2]	= {0};
		for (int is=0;is<2;is++){
			for (int idet=0;idet<16;idet++){
				float adc	= MuEvent_mVpdTriggerDetector_mADC[0][is][idet];
				float tac	= MuEvent_mVpdTriggerDetector_mTDC[0][is][idet];
				if (tac>250&&tac<3000){
					vpdnew[is]++;
				}
			}
			for (int it=0;it<NTrig;it++){
				if (((PLOTGOODEVENTS&&goodevent)||(!PLOTGOODEVENTS)) && fired[it]){ 
					hnvpdrefmult[is][it]->Fill(refmult,vpdnew[is]);
					if (vpdnew[is]>0){ hpnvpd1refmult[is][it]->Fill(refmult); }
					if (is==1&&vpdnew[0]>0&&vpdnew[1]>0){ hpnvpd11refmult[it]->Fill(refmult); }
				}
			}
		}
	}
	//---- end event loop...
	TText *txt;
	TPaveText *pave	= new TPaveText(0.01,0.1,0.99,0.9);
		pave->SetTextAlign(12);
		pave->SetMargin(0.02);
		pave->SetTextFont(82);
		pave->SetFillColor(18);
	sprintf(buf,"Run Number = %8d,  Nevts = %d",kRunNum,nentries);
		cout<<buf<<endl;
		txt = (TText*)pave->AddText(buf); txt->SetTextAlign(12);
	sprintf(buf,"Nevt good  = %7d,   fraction = %4.1f%%",ngood,100*(float)ngood/(float)nentries);
		cout<<buf<<endl;
		txt = (TText*)pave->AddText(buf); txt->SetTextAlign(12);
	sprintf(buf," ");
		txt = (TText*)pave->AddText(buf); txt->SetTextAlign(12);
	for (int it=0;it<NTrig;it++){
		float efftrig	= 0;
		float efftot	= 0;
		if (nevt_seen[it]){ efftrig = 100.*nevt_good[it]/nevt_seen[it]; }
		if (nentries     ){ efftot  = 100.*nevt_good[it]/((float)nentries); }
		sprintf(buf,"%-21s  Ntrig=%7d  Ngood=%7d  Ngood/Ntrig=%4.1f%%  Ngood/Nevt=%4.1f%%",
					TrigNames[it],(int)nevt_seen[it],(int)nevt_good[it],efftrig,efftot);
		cout<<buf<<endl;
		txt = (TText*)pave->AddText(buf); txt->SetTextAlign(12);
	}
	
	TString Top = TString("            ");
	TString Row[NTrig];
	for (int it=0;it<NTrig;it++){
		Top		+= TString(Form("%-19s ",TrigNames[it]));
		Row[it]  = TString(Form("%-19s ",TrigNames[it]));
		for (int jt=0;jt<NTrig;jt++){
			//cout<<it<<" "<<jt<<" "<<nevt_good2D[it][jt]<<endl;
			Row[it]	+= TString(Form("%-19d ",(int)nevt_good2D[it][jt]));
		}
	}
	TPaveText *pave2	= new TPaveText(0.01,0.1,0.99,0.9);
		pave2->SetTextAlign(12);
		pave2->SetMargin(0.02);
		pave2->SetTextFont(82);
		pave2->SetFillColor(18);
	sprintf(buf,"Run Number = %8d,  Nevts = %d",kRunNum,nentries);
		txt = (TText*)pave2->AddText(buf); txt->SetTextAlign(12);
	pave2->AddText(" ");
	cout<<Top.Data()<<endl;
		txt = (TText*)pave2->AddText(Top.Data()); txt->SetTextAlign(12);
	for (int it=0;it<NTrig;it++){
		cout<<Row[it].Data()<<endl;
			txt = (TText*)pave2->AddText(Row[it].Data()); txt->SetTextAlign(12);
	}
	
	//---- Painting...
	//
	gROOT->SetStyle("Plain"); 
	gStyle->SetPadBottomMargin(0.07);
	gStyle->SetPadTopMargin(0.05);
	gStyle->SetPadLeftMargin(0.11);
	gStyle->SetPadRightMargin(0.02);
	gStyle->SetOptStat(0);
	gStyle->SetOptFit(0);
	gStyle->SetPalette(1);
	gStyle->SetErrorX(0);	
	gStyle->SetTitleSize(0.1,"t");
	TLine *lone = new TLine(0,1.0,400,1.0);	lone->SetLineWidth(2); lone->SetLineColor(16);
	//
	//--------------------------------------------------------------
	++ican; sprintf(buf,"ccan%d",ican);
	ccan[ican] = new TCanvas(buf,buf,0,0,800.,800.*(7.5/10.));
	ccan[ican]->SetFillColor(10);
	ccan[ican]->cd(); 
		pave->Draw();
	ccan[ican]->cd(); ccan[ican]->Update();
	ccan[ican]->Print(psfileO.Data());
	//
	++ican; sprintf(buf,"ccan%d",ican);
	ccan[ican] = new TCanvas(buf,buf,0,0,800.,800.*(7.5/10.));
	ccan[ican]->SetFillColor(10);
	ccan[ican]->cd(); 
		pave2->Draw();
	ccan[ican]->cd(); ccan[ican]->Update();
	ccan[ican]->Print(psfile.Data());	
	//
	++ican; sprintf(buf,"ccan%d",ican);
	ccan[ican] = new TCanvas(buf,buf,0,0,800.,800.*(7.5/10.));
	ccan[ican]->SetFillColor(10); ccan[ican]->Divide(4,4,0.0001,0.0001);
	ccan[ican]->cd(1); 
		hnvertex->Draw();
	ccan[ican]->cd(2); 
		hvtxidrankmax->Draw();
	ccan[ican]->cd(3); 
		hvtxidntrkmax->Draw();
	for (int iv=0;iv<3;iv++){
		ccan[ican]->cd(5 + iv*4);
			hvtx_rvtx[iv]->Draw();
		ccan[ican]->cd(6 + iv*4);
			hvtx_zvtx[iv]->Draw();
		ccan[ican]->cd(7 + iv*4);
			gPad->SetLogy(1);
			hvtx_rank[iv]->Draw();
		ccan[ican]->cd(8 + iv*4);
			gPad->SetLogy(1);
			hvtx_ntrk[iv]->Draw();
	}
	ccan[ican]->cd(); ccan[ican]->Update();
	ccan[ican]->Print(psfile.Data());	
	//--------------------------------------------------------------
	//
	for (int it=0;it<NTrig;it++){
		//
		++ican; sprintf(buf,"ccan%d",ican);
		ccan[ican] = new TCanvas(buf,buf,0,0,800.,800.*(7.5/10.));
		ccan[ican]->SetFillColor(10);
		ccan[ican]->cd(); 
		TPad *pad1 = new TPad("pad1","This is pad1",0.05,0.93,0.95,0.99);
		TPad *pad2 = new TPad("pad2","This is pad2",0.05,0.01,0.95,0.92);
		pad1->Draw();
		pad2->Draw();
		//
		pad1->cd();
		++itext; 
		text[itext]->SetTextSize(0.5);
		text[itext]->DrawLatex(0.01,0.5,Form("Run=%d, %s, nevt=%d",kRunNum,TrigNames[it],(int)nevt_seen[it]));
		//
		pad2->cd();
		pad2->Divide(4,4,0.0001,0.0001);
		int iframe=0;
			pad2->cd(++iframe);
				hvtxyx[it]->Draw("colz");
			pad2->cd(++iframe);
				hvtxyz[it]->Draw("colz");
			pad2->cd(++iframe);
				hvtxr[it]->SetFillColor(18);
				hvtxr[it]->Draw();
			pad2->cd(++iframe);
				hvtxz[it]->SetFillColor(18);
				hvtxz[it]->Draw();
			pad2->cd(++iframe);
				gPad->SetLogy(1);
				hnprimary[it]->SetFillColor(18);
				hnprimary[it]->Draw();
			pad2->cd(++iframe);
				gPad->SetLogz(1);
				hnprimaryzvtx[it]->Draw("colz");
			pad2->cd(++iframe);
				gPad->SetLogz(1);
				hnbtofzvtx[it]->Draw("colz");
			pad2->cd(++iframe);
				gPad->SetLogy(1);
				hrefmult[it]->SetFillColor(18);
				hrefmult[it]->Draw();
			pad2->cd(++iframe);
				heta[it]->SetFillColor(18);
				heta[it]->Draw();
			pad2->cd(++iframe);
				hphi[it]->SetMinimum(0);
				hphi[it]->SetFillColor(18);
				hphi[it]->Draw();
			pad2->cd(++iframe);
				gPad->SetLogy(1);
				hpt[it]->SetFillColor(18);
				hpt[it]->Draw();
			pad2->cd(++iframe);
				gPad->SetLogz(1);
				hinvbetap[it]->Draw("colz");
			pad2->cd(++iframe);
				gPad->SetLogy(1);
				hmsq[it]->SetFillColor(18);
				hmsq[it]->Draw();
			pad2->cd(++iframe);
				htraymatched[it]->SetLineColor(1);
				htraymatched[it]->SetFillStyle(1001);
				htraymatched[it]->SetFillColor(18);
				htraymatched[it]->Draw();
				htraypid[it]->SetFillStyle(1001);
				htraypid[it]->SetFillColor(10);
				htraypid[it]->Draw("same");
			//pad2->cd(13);
			//	gPad->SetLogz(1);
			//	hnvpdrefmult[0][it]->Draw("colz");
			//pad2->cd(14);
			//	gPad->SetLogz(1);
			//	hnvpdrefmult[1][it]->Draw("colz");
			pad2->cd(++iframe);
				gPad->SetLogz(1);
				hpnvpd1refmult[0][it]->SetMinimum(0.0);
				hpnvpd1refmult[0][it]->SetMaximum(1.2);
				hpnvpd1refmult[0][it]->Divide(hrefmultwide[it]);
				hpnvpd1refmult[0][it]->SetLineColor(4);
				hpnvpd1refmult[0][it]->SetMarkerColor(4);
				hpnvpd1refmult[0][it]->SetMarkerStyle(20);
				hpnvpd1refmult[0][it]->SetMarkerSize(0.5);
				hpnvpd1refmult[0][it]->Draw();
				hpnvpd1refmult[1][it]->Divide(hrefmultwide[it]);
				hpnvpd1refmult[1][it]->SetLineColor(6);
				hpnvpd1refmult[1][it]->SetMarkerColor(6);
				hpnvpd1refmult[1][it]->SetMarkerStyle(20);
				hpnvpd1refmult[1][it]->SetMarkerSize(0.5);
				hpnvpd1refmult[1][it]->Draw("same");
				lone->Draw("same");
				hpnvpd1refmult[0][it]->Draw("same");
				hpnvpd1refmult[1][it]->Draw("same");
			pad2->cd(++iframe);
				gPad->SetLogz(1);
				hpnvpd11refmult[it]->SetMinimum(0.0);
				hpnvpd11refmult[it]->SetMaximum(1.2);
				hpnvpd11refmult[it]->Divide(hrefmultwide[it]);
				hpnvpd11refmult[it]->SetLineColor(1);
				hpnvpd11refmult[it]->SetMarkerColor(1);
				hpnvpd11refmult[it]->SetMarkerStyle(20);
				hpnvpd11refmult[it]->SetMarkerSize(0.5);
				hpnvpd11refmult[it]->Draw();
				lone->Draw("same");
				hpnvpd11refmult[it]->Draw("same");
		//
		ccan[ican]->cd(); ccan[ican]->Update();
		ccan[ican]->Print(psfile.Data());
		//
	}	
	
	//---- close-out	
	//
 	cout<<"..... Loop  Writing plots to root file...."<<endl;	
 	fout->Write();
 	fout->Close();
 	cout<<"..... Loop  Closing ps file...."<<endl;
	ccan[ican]->Print(psfileC.Data());
 	sprintf(buf,"/usr/bin/ps2pdf %s %s",psfile.Data(),pdffile.Data());
// 	sprintf(buf,"/usr/bin/ps2pdf %s %s",psfile.Data(),"temp.pdf");
 	cout<<"..... Loop  Executing ... "<<buf<<endl;
 	gSystem->Exec(buf);
 	sprintf(buf,"/bin/cp %s %s",pdffile.Data(),WebDirectory.Data());
// 	sprintf(buf,"/bin/cp %s %s","temp.pdf",WebDirectory.Data());
 	cout<<"..... Loop  Executing ... "<<buf<<endl;
 	gSystem->Exec(buf);
	cout<<"..... Loop  Done...."<<endl;	

	
}
