// $Id: doer_tof.cc,v 1.2 2016/02/29 21:41:36 geurts Exp $
// Author: W.J. Llope 2009/06/01

#include <stdio.h>
#include <unistd.h>
#include <getopt.h>
#include <sys/types.h>
#include <stdlib.h>
#include <iostream>

#include <rtsLog.h>
#include <TH1D.h>
#include <TH2D.h>

#include "doer.h"
#include "TsPacket.h"

bool DEBUG = false;

const int MAXTOFHITS	= 2500;

unsigned int GetLETime(TsPacket *p, int bunch);
unsigned int GetTrgDTime(TsPacket *p, int bunch);
int GetTHUB(int trayid);

int	cococut_p2p[16];
int	cococut_p2p_width;
int	cococut_p2p_500[16] = {119,119,119,119,119,120,119,119,
                           120,120,120,120,119,120,119,119};
int	cococut_p2p_200[16] = {120,120,120,120,
						   120,120,120,120,
                           120,120,120,120,
                           119,120,119,119};

//---- Run-14 AuAu 200 gev
int cococut_vpd_l		= 11;	// peak is at 10....
int cococut_vpd_u		= 13;

int icall = 0;


char	buf[255];
daq_dta *dd;
tof_t 	*tof;
int		kk;
//	int 	chbase;
int 	trayid,moduleid,cellid;
int 	half;
int 	bunch;
int		bunchid;
int		bunchid_tray[2][122];
int		pid;
int		vpd_ch;
int		coco,edgetime,cocouse,trgdtime;
float	cocous,edgetimens;
int		COARSESHIFT;
int		iCh_vpd,iCh_p2p,iCh_ind;
bool	isVPDhit,isP2Phit;
bool	cocogood;
TH1D	*h1;
TH2D	*h2;
int		vpd_cco[38];
float	vpd_tle[38];
float	vpd_tte[38];
float	vpd_tot[38];
int		p2p_cco[16];
float	p2p_tle[16];
float	p2p_tte[16];
float	p2p_tot[16];

int		telist_n[54];
float	telist_t[50][54];

bool	bigevent	= false;
unsigned int		nwordstouse[4];

int	daynum;
int	runnum;

bool toomanyLE;
bool toomanyTE;

int		nLEseen			=  0;
int		nTEseen			=  0;
int		nTofHits		=  0;
int		nTofHitsLE		=  0;
int		nTofHitsTE		=  0;
int 	latestLEtray	= -1;
int 	latestLEchan	= -1;
float	latestLE_coco;
float	latestLE_tle;

int		LE_tray[MAXTOFHITS];
int		LE_chan[MAXTOFHITS];
float	LE_coco[MAXTOFHITS];
float	LE_bunchid[MAXTOFHITS];
float	LE_trgdtime[MAXTOFHITS];
float	LE_time[MAXTOFHITS];
int		TE_tray[MAXTOFHITS];
int		TE_chan[MAXTOFHITS];
//float	TE_coco[MAXTOFHITS];
//float	TE_trgdtime[MAXTOFHITS];
float	TE_time[MAXTOFHITS];
	
//cout<<"==========================="<<endl;

	bool liveP2Ptrg_e = false;
	bool liveP2Ptrg_w = false;
	

int tof_iterate( daqReader *rdr, P2P_st *P2P, doer_st *TOF,  Int_t iday, Int_t kDataChoice, stop_st *STOP, INFO_st *MYINFO ){
	// cout << "tof_iterate" << endl;
	COARSESHIFT	= 0;	// shift east VPD w.r.t. west VPD coarse counter!!!!

	nLEseen			=  0;
	nTEseen			=  0;
	nTofHits		=  0;
	nTofHitsLE		=  0;
	nTofHitsTE		=  0;
	latestLEtray	= -1;
	latestLEchan	= -1;

	dd = rdr->det("tof")->get("legacy") ;
	if(!dd) return -1;

	for (int ic=0;ic<38;ic++){ vpd_tle[ic]  = vpd_tte[ic] = vpd_tot[ic] = 0.0; }
	for (int ic=0;ic<16;ic++){ p2p_tle[ic]  = p2p_tte[ic] = p2p_tot[ic] = 0.0; }
	for (int ic=0;ic<54;ic++){ telist_n[ic] = 0; }


	if ( P2P->RPWVU1_TAC>100
	  || P2P->RPWVU2_TAC>100
	  || P2P->RPWVD1_TAC>100
	  || P2P->RPWVD2_TAC>100
	  || P2P->RPWHO1_TAC>100
	  || P2P->RPWHO2_TAC>100
	  || P2P->RPWHI1_TAC>100
	  || P2P->RPWHI2_TAC>100 ){ liveP2Ptrg_w = true; }
	if ( P2P->RPEVU1_TAC>100
	  || P2P->RPEVU2_TAC>100
	  || P2P->RPEVD1_TAC>100
	  || P2P->RPEVD2_TAC>100
	  || P2P->RPEHO1_TAC>100
	  || P2P->RPEHO2_TAC>100
	  || P2P->RPEHI1_TAC>100
	  || P2P->RPEHI2_TAC>100 ){ liveP2Ptrg_e = true; }
	dd = rdr->det("tof")->get("legacy") ;
	if(!dd) return -1;

		while(dd->iterate()) {
			tof = (tof_t *)dd->Void;

	 		icall	= icall + 1;
	 		toomanyLE = toomanyTE = false;

			bigevent	= false;
			for (int r = 0; r < 4; r++) {		// all 4 fibers....
				MYINFO->nwordsfiber[r]	= tof->ddl_words[r];
				nwordstouse[r]	= MYINFO->nwordsfiber[r];			 
				if (MYINFO->nwordsfiber[r] > 8000){
					nwordstouse[r]	= 8000;
					bigevent		= true;
				}
			}
			if (bigevent){
				cout<<"DDL_WORDS[i] .... call="<<icall<<"\t"
					<<tof->ddl_words[0]<<" "
					<<tof->ddl_words[1]<<" "
					<<tof->ddl_words[2]<<" "
					<<tof->ddl_words[3]<<"\t time="
					<<rdr->evt_time
					<<endl;
			}

			for (int it=0;it<122;it++){
				for (int ih=0;ih<2;ih++){
					bunchid_tray[ih][it]	= -9999;
				}
			}
			
			//---- loop over fibers....
			for (int r = 0; r < 4; r++) {		// all 4 fibers....
				if (bigevent){ cout<<r<<" "<<nwordstouse[r]<<endl; }
				//---- loop over words in this DDLR...
				for (u_int i=0; i<nwordstouse[r]; i++) {

					if (bigevent) {
						cout<<"trying to decode tof->ddl[r][i] .... event="<<icall
							<<", r="<<r<<", i="<<i<<"("<<nwordstouse[r]<<")"
							<<"\t ... word= "<<dec<<tof->ddl[r][i]<<"  "
							<<hex<<uppercase<<tof->ddl[r][i]<<dec
							<<endl;
					}
					TsPacket *p = new TsPacket(tof->ddl[r][i]);
					pid 		= p->GetId();
					isVPDhit 	= isP2Phit 	= false;
					cocogood	= false;

					if (pid == 0x0c) {
						trayid	= p->GetTrayId();
						half 	= p->GetHalf();
						if(bigevent)cout<<"0x0c .. trayid="<<trayid<<" half="<<half<<endl;
					} else if (pid == 0x2) {
						bunch 	= p->GetBunchId();			//
						bunchid = tof->ddl[r][i]&0xFFF;		// these two #s are the same
						if (trayid>=1 && trayid<=122){
							bunchid_tray[half][trayid-1] = bunchid;
						} else {
							cout<<"bunchid mapping problem ..."<<half<<" "<<trayid<<" "<<bunchid<<endl;
						}
						if(bigevent)cout<<"bunch ... "<<bunchid<<endl;
					} else if (pid == 0xe) {
						if(bigevent)cout<<"0x0e .. do nothing"<<endl;
					//
					//---- LE hit....................................................
					} else if (p->IsLE()) { 					// leading edge
						++nLEseen;
						int ch 	= 96*half + p->GetChannelTray5();
						if(bigevent)cout<<"LE   .. tray,ch="<<trayid<<" "<<ch<<endl;
						if (trayid <= 120) {	
							//
							moduleid	= ch/6;
							cellid		= ch - 6*moduleid;
							coco 		= GetLETime(p, bunch);
							trgdtime	= GetTrgDTime(p, bunch);
							edgetime	= p->GetEdgeTime();
							edgetimens	= (25./1024.)*(Float_t)edgetime;
							//
							//TEST
							//int timeinbin = ((tof->ddl[r][i]&0x7ffff)<<2)+((tof->ddl[r][i]>>19)&0x03);
							//cout<<edgetime<<" "<<timeinbin<<endl;
							//TEST
							//if(bigevent){
							//	cout<<"LE trayid<120 .. module="<<moduleid<<" edgetime="<<edgetime<<endl;
							//}
							if (nTofHitsLE<MAXTOFHITS-1){
								LE_tray[nTofHitsLE]		= trayid-1;
								LE_chan[nTofHitsLE]		= ch;
								LE_coco[nTofHitsLE]		= coco;
								LE_bunchid[nTofHitsLE]	= bunchid;
								LE_trgdtime[nTofHitsLE]	= trgdtime;
								LE_time[nTofHitsLE]		= edgetimens;
								++nTofHitsLE;
								latestLEtray	= trayid;
								latestLEchan	= ch;
								latestLE_coco	= coco;
								latestLE_tle	= edgetimens;
								//
								if (DEBUG){
								cout<<"Tray LE "<<nTofHitsLE<<" "<<trayid<<" "<<ch<<" "<<moduleid<<" "<<cellid<<"\t "
										<<edgetime<<" "<<edgetimens<<endl;
									}
								
							} else {
								cout<<"Too Many LE hits (TOF section)!!!!  icall="<<icall<<"  nh="<<nTofHitsLE<<endl;
							}

						} else {
							if (trayid!=121&&trayid!=122){
								cout<<"Tray ID???"<<trayid<<" ....skipping event"<<endl;
								return -1;
							}
							vpd_ch 		= TsPacket::upvpd_map(trayid, half, p->GetTdc(), p->GetRawChannel());
							coco 		= GetLETime(p, bunch);
							trgdtime	= GetTrgDTime(p, bunch);
							cocous		= ((float)coco)*COARSEBIN2NS/1000.;
							edgetime	= p->GetEdgeTime();
							edgetimens	= (25./1024.)*(Float_t)edgetime;
							cocouse		= coco;
							if (trayid==122){						// shift VPD east coarse counter!!!!
								cocouse		= coco - COARSESHIFT;
							}
							if(bigevent)cout<<"VPD LE  .. ch="<<vpd_ch<<" "<<edgetime<<" "<<coco<<endl;
							//
							if (nTofHitsLE<MAXTOFHITS-1){
								LE_tray[nTofHitsLE]		= trayid-1;
								LE_chan[nTofHitsLE]		= ch;
								LE_coco[nTofHitsLE]		= coco;
								LE_bunchid[nTofHitsLE]	= bunchid;
								LE_trgdtime[nTofHitsLE]	= trgdtime;
								LE_time[nTofHitsLE]		= edgetimens;
								++nTofHitsLE;
								latestLEtray	= trayid;
								latestLEchan	= ch;
								latestLE_coco	= coco;
								latestLE_tle	= edgetimens;
							} else {
								cout<<"Too Many LE hits (VPD section)!!!! "<<icall<<endl;
							}
							//
							if (vpd_ch<20){
								if (trayid==121){ iCh_vpd=vpd_ch-1    ; isVPDhit=true; }
								if (trayid==122){ iCh_vpd=vpd_ch-1+19 ; isVPDhit=true; }
							} else if (vpd_ch>=20) {
								if (trayid==121){ iCh_p2p=vpd_ch-20   ; isP2Phit=true; }
								if (trayid==122){ iCh_p2p=vpd_ch-20+8 ; isP2Phit=true; }
							}
							if (isVPDhit){ iCh_ind =    iCh_vpd; } else
							if (isP2Phit){ iCh_ind = 38+iCh_p2p; } 
							//
							if (isVPDhit){
	 							if(trayid==121){
	 								h1=(TH1D*)gDirectory->Get("hcoco_vpdw"); h1->Fill(cocouse,1.0);
	 							} else if (trayid==122){
	 								h1=(TH1D*)gDirectory->Get("hcoco_vpde"); h1->Fill(cocouse,1.0);
	 							}
	 							if (cocouse>=cococut_vpd_l 
	 							 && cocouse<=cococut_vpd_u){
	 								cocogood	= true;
	 							}
	 							if (kDataChoice==1){ cocogood = true; }
								if (cocogood && edgetimens>0.0){
		 							if(trayid==121){
		 								h1=(TH1D*)gDirectory->Get("hcocokeep_vpdw"); h1->Fill(cocouse,1.0);
		 							} else if (trayid==122){
		 								h1=(TH1D*)gDirectory->Get("hcocokeep_vpde"); h1->Fill(cocouse,1.0);
		 							}
									if((vpd_tle[iCh_vpd] < 0.001     )		// first hit in channel
									|| (vpd_tle[iCh_vpd] > edgetimens)){	// substitute for earliest hit
											vpd_cco[iCh_vpd] = cocouse;								
											vpd_tle[iCh_vpd] = edgetimens;
											//cout<<"LE ... "<<iCh_vpd<<" "<<edgetimens<<" "<<cocouse<<endl;
									}
								}
							} else if (isP2Phit){
	 							if(trayid==121){
	 								h1=(TH1D*)gDirectory->Get("hcoco_p2pw"); h1->Fill(cocouse,1.0);
	 							} else if (trayid==122){
	 								h1=(TH1D*)gDirectory->Get("hcoco_p2pe"); h1->Fill(cocouse,1.0);
	 							}
	 							sprintf(buf,"hcoco_p2p%d",iCh_p2p);
	 							h1=(TH1D*)gDirectory->Get(buf); h1->Fill(cocouse,1.0);	
	 							if (liveP2Ptrg_w && trayid==121){
	 								sprintf(buf,"hcoco_p2p_p2p%d",iCh_p2p);
	 								h1=(TH1D*)gDirectory->Get(buf); h1->Fill(cocouse,1.0);	
	 							}
								if (liveP2Ptrg_e && trayid==122){
	 								sprintf(buf,"hcoco_p2p_p2p%d",iCh_p2p);
	 								h1=(TH1D*)gDirectory->Get(buf); h1->Fill(cocouse,1.0);	
	 							}
								//
								//if (cocouse>=80 && cocouse<=125){
									cocogood	= true;
	 							if (kDataChoice==1){ cocogood = true; }
								//
								if (cocogood && edgetimens>0.0){
									if((p2p_tle[iCh_p2p]== 0.        )
									|| (p2p_tle[iCh_p2p] > edgetimens)){
											p2p_cco[iCh_p2p] = cocouse;								
											p2p_tle[iCh_p2p] = edgetimens;
											//cout<<iCh_p2p<<" "<<edgetimens<<" "<<cocouse<<endl;
									}
								}
							}
							//
						}	//---- end LE trayid check...
						//
					//---- TE hit....................................................
					} else if (p->IsTE()) { 					// trailing edge
						++nTEseen;
						int ch 	= 96*half + p->GetChannelTray5();
						if(bigevent)cout<<"TE   .. tray,ch="<<trayid<<" "<<ch<<endl;
						if (trayid <= 120 ){

							moduleid	= ch/6;
							cellid		= ch - 6*moduleid;
							coco 		= GetLETime(p, bunch);
							trgdtime	= GetTrgDTime(p, bunch);
							edgetime	= p->GetEdgeTime();
							edgetimens	= (25./1024.)*(Float_t)edgetime;
							if(bigevent)cout<<"VPD TE  .. ch="<<vpd_ch<<" "<<edgetime<<" "<<coco<<endl;
							if (nTofHitsTE<MAXTOFHITS-1){
								TE_tray[nTofHitsTE]	= trayid-1;
								TE_chan[nTofHitsTE]	= ch;
								//TE_coco[nTofHitsTE]	= coco;
								//TE_trgdtime[nTofHitsTE]	= trgdtime;
								TE_time[nTofHitsTE]	= edgetimens;
								++nTofHitsTE;
							} else {
								cout<<"Too Many TE hits (TOF section)!!!!  icall="<<icall<<"  nh="<<nTofHitsTE<<endl;
							}

						} else {		// VPD....
						  vpd_ch 		= TsPacket::upvpd_map(trayid, half, p->GetTdc(), p->GetRawChannel());
							vpd_ch		= TMath::Abs(vpd_ch);
							edgetime	= p->GetEdgeTime();
							edgetimens	= (25./1024.)*(Float_t)edgetime;
							//
							if (nTofHitsTE<MAXTOFHITS-1){
								TE_tray[nTofHitsTE]	= trayid-1;
								TE_chan[nTofHitsTE]	= ch;
								//TE_coco[nTofHitsTE]	= coco;
								//TE_trgdtime[nTofHitsTE]	= trgdtime;
								TE_time[nTofHitsTE]	= edgetimens;
								++nTofHitsTE;
							} else {
								cout<<"Too Many TE hits (VPD section)!!!! "<<icall<<endl;
							}
							//
							if (vpd_ch<20){
								if (trayid==121){ iCh_vpd=vpd_ch-1    ; isVPDhit=true; }
								if (trayid==122){ iCh_vpd=vpd_ch-1+19 ; isVPDhit=true; }
							} else if (vpd_ch>=20) {
								if (trayid==121){ iCh_p2p=vpd_ch-20   ; isP2Phit=true; }
								if (trayid==122){ iCh_p2p=vpd_ch-20+8 ; isP2Phit=true; }
							}
							if (isVPDhit){ iCh_ind =    iCh_vpd; } else
							if (isP2Phit){ iCh_ind = 38+iCh_p2p; } 
							//
							if(edgetimens>0.001&&iCh_ind>=0&&iCh_ind<38){
								kk			 			 = telist_n[iCh_ind];
								telist_t[kk][iCh_ind]	 = edgetimens;
								telist_n[iCh_ind]		+= 1;
							//} else if (edgetimens>0.){
							//	cout<<"TE VPD huh? "<<trayid<<" "
							//		<<vpd_ch<<" "<<iCh_ind<<" "<<edgetime<<endl;	
							}
							//
						}	//---- end LE trayid check...
						//
					}	//---- end packetype check...
					//
					if (trayid>122){ return -1; }
					if (bigevent) cout<<"moving to next word..."<<endl;
					delete p;
				}	//---- end loop over words in this DDLR....
			}	//---- end  loop over 4 DDLRs....
		}	//---- end dd iterator...
}


int tof_analyze(daqReader *rdr,  P2P_st *P2P,  doer_st *TOF,   Int_t iday,  Int_t kDataChoice,  stop_st *STOP,  INFO_st *MYINFO){
	// cout << "START_tof_analyze" << endl;
	int refbunchid_h0	= bunchid_tray[0][0];
	int refbunchid_h1	= bunchid_tray[1][0];
	if (refbunchid_h0 != refbunchid_h1){
		cout<<"Ref bunch half mismatch ... "<<refbunchid_h0<<" "<<refbunchid_h1<<endl;
	}
	if (refbunchid_h0 > -9990 && refbunchid_h1 > -9990){
		for (int it=0;it<122;it++){
			int ithub	= GetTHUB(it+1);
			for (int ih=0;ih<2;ih++){
				if (it!=101&&it!=105){						// mask out trays 102,106 not in run-13.........
					int diff	= bunchid_tray[ih][it] - refbunchid_h0;
					if (diff> 2048){diff=diff-4096;} else 
					if (diff<-2048){diff=diff+4096;}
					int xval	= 2*it + ih;
					h2=(TH2D*)gDirectory->Get("hbunchidoffset_tray"); h2->Fill(xval,diff,1.0);
					//
					if (bunchid_tray[ih][it]<-9990){
//if (it!=37){
//!						cout<<" bunchid not found ... "<<it<<" "<<ih<<endl;
//!						sprintf(buf,"htray_nobunchid");
//!						h1=(TH1D*)gDirectory->Get(buf); h1->Fill(it,1.0);
//}
					}
					if (diff<-125 || diff>125){
//if (it!=37){
//!						cout<<"bunchid diff out of plot range "<<it<<" "<<ih<<"\t"
//!							<<bunchid_tray[ih][it]<<"\t"
//!							<<refbunchid_h0<<" "<<refbunchid_h1<<" "<<diff<<endl;
//}
					}
					sprintf(buf,"hbunchidoffset_thubhalf%d%d",ithub,ih);
					h1=(TH1D*)gDirectory->Get(buf); h1->Fill(diff,1.0);
					//
				}
			}
		}
	} else {
		cout<<"ref bunchids both not found ... skipping event ... "<<refbunchid_h0<<" "<<refbunchid_h1<<endl;	 
		return -1;
	}
	//


	//---- match VPD TE to LE....
	//
	float	tle,tte,diffmin;
	for (int ic=0;ic<38;ic++){
//cout<<" ic="<<ic<<" vpd_tle[ic]="<<vpd_tle[ic]
//	<<" telist_n[ic]="<<telist_n[ic];
		diffmin	= 999999.;
		if (vpd_tle[ic]>0.0 && telist_n[ic]>0){
			tle	= vpd_tle[ic];
			for (int ih=0;ih<telist_n[ic];ih++){
				tte	= telist_t[ih][ic]; 
//cout<<"\t "<<ih<<" "<<tte;
				if ((tte-tle)>0.0 && (tte-tle)<diffmin){
					diffmin		= tte-tle;
					vpd_tte[ic]	= tte;
				}
			}
//cout<<"\t "<<vpd_tte[ic]<<" "<<diffmin;
		}
//cout<<endl;
	}
//	cout<<"----------------------------------------"<<endl;
	for (int ic=0;ic<16;ic++){
		if (p2p_tle[ic]>0.0 && telist_n[ic+38]>0){
			tle		= p2p_tle[ic];
			diffmin	= 999999.;
			for (int ih=0;ih<telist_n[ic+38];ih++){
				tte	= telist_t[ih][ic+38]; 
				if ((tte-tle)>0.0 && (tte-tle)<diffmin){
					diffmin		= tte-tle;
					p2p_tte[ic]	= tte;
				}
			}
		}
	}

	for (int ic=0;ic<38;ic++){
		if (vpd_tle[ic]>0 && vpd_tte[ic]>0 && (vpd_tte[ic]-vpd_tle[ic])<55.0){
//		if (vpd_tle[ic]>0){
			TOF->vpd_cco[ic]	= vpd_cco[ic];
			TOF->vpd_tle[ic]	= vpd_tle[ic];
			TOF->vpd_tte[ic]	= vpd_tte[ic];
			TOF->vpd_tot[ic]	= vpd_tte[ic]-vpd_tle[ic];
		} else {
			TOF->vpd_cco[ic]	= 0;
			TOF->vpd_tle[ic]	= 0.0;
			TOF->vpd_tte[ic]	= 0.0;
			TOF->vpd_tot[ic]	= 0.0;
		}
	}
	for (int ic=0;ic<16;ic++){
		if (p2p_tle[ic]>0 && p2p_tte[ic]>0 ){ //&& (p2p_tte[ic]-p2p_tle[ic])<55.0){
			TOF->p2p_cco[ic]	= p2p_cco[ic];
			TOF->p2p_tle[ic]	= p2p_tle[ic];
			TOF->p2p_tte[ic]	= p2p_tte[ic];
			TOF->p2p_tot[ic]	= p2p_tte[ic]-p2p_tle[ic];
		} else {
			TOF->p2p_cco[ic]	= 0;
			TOF->p2p_tle[ic]	= 0.0;
			TOF->p2p_tte[ic]	= 0.0;
			TOF->p2p_tot[ic]	= 0.0;
		}
	}


	const int MAXNHITPERCELL	= 20;

	int		*BUF_nle = new int[23424];
	int		*BUF_nte = new int[23424];

	float	**BUF_tle = new float*[MAXNHITPERCELL];		
	float	**BUF_tte = new float*[MAXNHITPERCELL];		
	float	**BUF_coco = new float*[MAXNHITPERCELL];		
	float	**BUF_bunchid = new float*[MAXNHITPERCELL];		
	float	**BUF_trgdtime = new float*[MAXNHITPERCELL];		
	for ( int i = 0; i < MAXNHITPERCELL; i++ ){
		BUF_tle[i] = new float[23424];
		BUF_tte[i] = new float[23424];
		BUF_coco[i] = new float[23424];
		BUF_bunchid[i] = new float[23424];
		BUF_trgdtime[i] = new float[23424];
	}

	for (int i=0;i<23424;i++){					
		BUF_nle[i] = BUF_nte[i] = 0;
	}
	for (int ihl=0;ihl<nTofHitsLE;ihl++){
		int	i				= LE_tray[ihl]*192 + LE_chan[ihl];
		if (i<0||i>23424){
			cout<<"LE map error "<<nTofHitsLE<<" "<<ihl<<" "
				<<LE_tray[ihl]<<" "<<LE_chan[ihl]<<" "
				<<LE_coco[ihl]<<" "<<LE_time[ihl]<<endl;
				continue;
// 				STOP->nTofHits		= 0;
// 				STOP->nTofHitsLE	= 0;
// 				STOP->nTofHitsTE	= 0;
// 				return 0;
		}
		int k					= BUF_nle[i];
		++BUF_nle[i];
		if (k<MAXNHITPERCELL){
			BUF_tle[k][i]		= LE_time[ihl];
			BUF_coco[k][i]		= LE_coco[ihl];
			BUF_bunchid[k][i]	= LE_bunchid[ihl];
			BUF_trgdtime[k][i]	= LE_trgdtime[ihl];
		} else {
			toomanyLE	= true;
			//cout<<"too many LE hits?!?!?!? "<<nTofHitsLE<<" "<<ihl<<"\t "<<LE_tray[ihl]<<" "<<LE_chan[ihl]<<"\t "<<BUF_nle[i]<<endl;
		}
	}
	for (int iht=0;iht<nTofHitsTE;iht++){
		int	i				= TE_tray[iht]*192 + TE_chan[iht];
		if (i<0||i>23424){
			cout<<"TE map error "<<nTofHitsTE<<" "<<iht<<" "
				<<LE_tray[iht]<<" "<<LE_chan[iht]<<" "
				<<LE_coco[iht]<<" "<<LE_time[iht]<<endl;
				continue;
// 				STOP->nTofHits		= 0;
// 				STOP->nTofHitsLE	= 0;
// 				STOP->nTofHitsTE	= 0;
// 			return 0;
		}
		int k				= BUF_nte[i];
		++BUF_nte[i];
		if (k<MAXNHITPERCELL){
			BUF_tte[k][i]	= TE_time[iht];
		} else {
			toomanyTE	= true;
			//cout<<"too many TE hits?!?!?!? "<<nTofHitsTE<<" "<<iht<<"\t "<<TE_tray[iht]<<" "<<TE_chan[iht]<<"\t "<<BUF_nte[i]<<endl;
		}
	}

	int		ktray,kmodule,kcell,ihtmintot;
	float	thistot;
	for (int i=0;i<23424;i++){						// expanded for VPD....
		//
		if (BUF_nle[i]==0 || BUF_nte[i]==0){ continue; }
		//
		ktray		= (i/192);
		kmodule		= (i - ktray*192)/6;
		kcell		= (i - ktray*192 - kmodule*6);
		//
		if (BUF_nle[i]==1 && BUF_nte[i]==1){
			thistot	= BUF_tte[0][i] - BUF_tle[0][i];
			if (thistot>0.0){
				int k	= nTofHits;
				++nTofHits;	
				STOP->tray[k]		= ktray;
				STOP->module[k]		= kmodule;
				STOP->cell[k]		= kcell;
				STOP->tle[k]		= BUF_tle[0][i];
				STOP->tte[k]		= BUF_tte[0][i];
				STOP->tot[k]		= BUF_tte[0][i] - BUF_tle[0][i];
				STOP->coco[k]		= BUF_coco[0][i];
				STOP->bunchid[k]	= BUF_bunchid[0][i];
				STOP->trgdtime[k]	= BUF_trgdtime[0][i];
			}
		//
		} else if (BUF_nle[i]>1 || BUF_nte[i]>1){
			for (int ihl=0;ihl<TMath::Min(BUF_nle[i],MAXNHITPERCELL);ihl++){
				tle		= BUF_tle[ihl][i];
				if (tle==0){ continue; }		//????????????????????????
				ihtmintot	= -1;
				thistot		= 999999.;
				if (BUF_nte[i] >= MAXNHITPERCELL){ 
					//cout<<"too many hits in buffer?!?!?! "<<i<<" "<<BUF_nle[i]<<" "<<ihl<<" "<<tle<<"\t "<<BUF_nte[i]<<endl;
					continue;
				}
				for (int iht=0;iht<TMath::Min(BUF_nte[i],MAXNHITPERCELL);iht++){
					tte		= BUF_tte[iht][i];
					if (tte>=0.0){
						if (tte-tle>0.0 && tte-tle<thistot){
							thistot		= tte-tle;
							ihtmintot	= iht;
						}
					}
				}
				if (ihtmintot>-1 && thistot>0 && thistot<60){
					int k	= nTofHits;
					++nTofHits;	
					STOP->tray[k]			= ktray;
					STOP->module[k]			= kmodule;
					STOP->cell[k]			= kcell;
					STOP->tle[k]			= BUF_tle[ihl][i];
					STOP->tte[k]			= BUF_tte[ihtmintot][i];
					//STOP->tot[k]			= BUF_tte[ihtmintot][i] - BUF_tle[ihl][i];
					STOP->tot[k]			= thistot;
					STOP->coco[k]			= BUF_coco[ihl][i];
					STOP->bunchid[k]		= BUF_bunchid[ihl][i];
					STOP->trgdtime[k]		= BUF_trgdtime[ihl][i];
					BUF_tte[ihtmintot][i]	= -1.;		// insure this TE is not reused....
				}
			}
		}
	}
	
	//---- add VPD hits to STOP tree....
// 	for (int ic=0;ic<38;ic++){
// 		if ( vpd_tle[ic]>0.0 ){ //&& vpd_tte[ic]>0){
// 				int k	= nTofHits;
// 				int kt;
// 				if (ic<19) { kt=121; } else { kt=122; }
// 				thistot		= vpd_tte[ic] - vpd_tle[ic];
// 				if (DEBUG){
// 					cout<<"VPD hit "<<kt<<" "<<ic<<" le="<<vpd_tle[ic]<<" te="<< vpd_tte[ic]<<" tot="<<thistot<<endl;
// 				}
// 				if (thistot>2.&&thistot<60.){
// 					STOP->tray[k]		= kt;
// 					STOP->module[k]		= 0.0;
// 					STOP->cell[k]		= ic;
// 					STOP->tle[k]		= vpd_tle[ic];
// 					STOP->tte[k]		= vpd_tte[ic];
// 					STOP->tot[k]		= thistot;
// 					++nTofHits;
// 					++nTofHitsLE;
// 					++nTofHitsTE;
// 				}
// 		}
// 	}	
	STOP->nTofHits		= nTofHits;
	STOP->nTofHitsLE	= nTofHitsLE;
	STOP->nTofHitsTE	= nTofHitsTE;

	if (DEBUG){
		cout<<nLEseen<<" "<<nTofHitsLE<<"\t"<<nTEseen<<" "<<nTofHitsTE<<"\t"<<nTofHits<<endl;
	}

	//---- loop over structure results to get THUB-based trigger time windows...
	//
	int nhits_intime = 0;
	int nhits_early = 0;
	for (int ih=0;ih<nTofHits;ih++){
		int itray	= STOP->tray[ih] + 1;
		int ithub	= GetTHUB(itray);
		trgdtime	= STOP->trgdtime[ih];
		float tpeak	= TrayTrigTimingPeaks[itray-1];		// only used for counting
		float offset= tpeak;
		//cout<<itray<<" "<<ithub<<" "<<trgdtime<<endl;
		if (ithub>=0&&ithub<=3){
			if (trgdtime-offset<-50){
				++nhits_early;
			}
			if (trgdtime-offset>-40&&trgdtime-offset<40){
				++nhits_intime;
			}
		}
		sprintf(buf,"htrgdtime_rawall");
			h1=(TH1D*)gDirectory->Get(buf); h1->Fill(trgdtime,1.0);
		sprintf(buf,"htrgdtime%d",ithub);
			h1=(TH1D*)gDirectory->Get(buf); h1->Fill(trgdtime,1.0);
		float aliml	= trigwindowLow[ithub];
		float alimu	= trigwindowHigh[ithub];
		if (trgdtime>=aliml&&trgdtime<=alimu){
			sprintf(buf,"htrgdtimekeep%d",ithub);
				h1=(TH1D*)gDirectory->Get(buf); h1->Fill(trgdtime,1.0);
		}
		float aliml3	= trigwindowLow[ithub] - 107;
		float alimu3	= trigwindowHigh[ithub] + 107;
		if (trgdtime>=aliml3&&trgdtime<=alimu3){
			sprintf(buf,"htrgdtimekeep3%d",ithub);
				h1=(TH1D*)gDirectory->Get(buf); h1->Fill(trgdtime,1.0);
		}

		sprintf(buf,"htrgdtime_tray");
		h2=(TH2D*)gDirectory->Get(buf); h2->Fill(itray,trgdtime,1.0);			
		sprintf(buf,"htrgdtime_trayoff");
		h2=(TH2D*)gDirectory->Get(buf); h2->Fill(itray,trgdtime-offset,1.0);			
		//
		if (itray==121||itray==122){
			int ivpdch	= 19*(itray-121);
			sprintf(buf,"htrgdtime_VPD%d",ivpdch);
			h1=(TH1D*)gDirectory->Get(buf); h1->Fill(trgdtime,1.0);
		}
		//
	}
	sprintf(buf,"hnearly_nintime");
	h2=(TH2D*)gDirectory->Get(buf); h2->Fill(nhits_intime,nhits_early,1.0);			
	MYINFO->nhits_early		= nhits_early;
	MYINFO->nhits_intime	= nhits_intime;

// 	for (int ih=0;ih<2;ih++){
// 		for (int it=0;it<120;it++){
// 			cout<<ih<<" "<<it<<" "<<bunchid_tray[ih][it]<<endl;
// 		}
// 	}

	if (toomanyLE){ cout<<"Lots of LE Hits ... "<<icall<<endl; return -1;}
	if (toomanyTE){ cout<<"Lots of TE Hits ... "<<icall<<endl; return -1;}

	delete[] BUF_nle;
	delete[] BUF_nte;

	for ( int i = 0; i < MAXNHITPERCELL; i++ ){
		delete[] BUF_tle[i];
		delete[] BUF_tte[i];
		delete[] BUF_coco[i];
		delete[] BUF_bunchid[i];
		delete[] BUF_trgdtime[i];
	}

	delete[] BUF_tle;
	delete[] BUF_tte;
	delete[] BUF_coco;
	delete[] BUF_bunchid;
	delete[] BUF_trgdtime;

	// cout << "END_tof_analyze" << endl;

	return 0;
}


int tof_doer(	daqReader *rdr, 
				P2P_st *P2P, 
				doer_st *TOF,  
				Int_t iday, 
				Int_t kDataChoice, 
				stop_st *STOP, 
				INFO_st *MYINFO){
	// cout << "tof_doer" << endl;
	tof_iterate( rdr, P2P, TOF, iday, kDataChoice, STOP, MYINFO );
	tof_analyze( rdr, P2P, TOF, iday, kDataChoice, STOP, MYINFO );
	return 0;
	

	
}	
//---- end tof doer....




