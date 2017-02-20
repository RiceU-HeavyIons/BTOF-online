// $Id: doer_mtd.cc,v 1.1 2016/02/25 16:02:52 geurts Exp $
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

unsigned int GetLETime(TsPacket *p, int bunch);
unsigned int GetTrgDTime(TsPacket *p, int bunch);
int tdcchan2globalstrip(int,int,int,int,int);
	
const int MAXMTDHITS = 1200;

int mtd9map[3][8];

int mtd_doer(daqReader *rdr, Int_t iday, Int_t kDataChoice, 
				struct mtd_st *MTD, struct INFO_st *MYINFO ){
	char	buf[40];
    daq_dta *ddm;
    mtd_t 	*mtd;
    int		kk;
//	int 	chbase;
	int 	trayid,trayiddata,moduleid,cellid,stripid;
	int 	half;
	int 	bunch;
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
	
//	int		telist_n[54];
//	float	telist_t[50][54];
	
	unsigned int		nwordstouse[4];
	
	int	daynum;
	int	runnum;

	COARSESHIFT	= 0;
	int cococut_mtd_l		= 115;	// peak is at 115+116....
	int cococut_mtd_u		= 117;

//	for (int ic=0;ic<54;ic++){ telist_n[ic] = 0; }
// 	for (int it=0;it<3;it++){ 
// 		for (int ic=0;ic<8;ic++){ 
// 			mtd9map[it][ic]	= 999;
// 		}
// 	}
// 	mtd9map[2][4]	= 1;
// 	mtd9map[1][2]	= 2;
// 	mtd9map[0][0]	= 3;
// 	mtd9map[2][7]	= 4;
// 	mtd9map[1][7]	= 5;	
// 	mtd9map[0][7]	= 6;
// 	mtd9map[2][2]	= 7;
// 	mtd9map[1][0]	= 8;
// 	mtd9map[2][0]	= 9;
// 	mtd9map[1][4]	= 10;
// 	mtd9map[0][1]	= 11;
// 	mtd9map[2][1]	= 12;
// 	mtd9map[2][3]	= 13;
// 	mtd9map[1][3]	= 14;
// 	mtd9map[0][6]	= 15;
// 	mtd9map[2][6]	= 16;
// 	mtd9map[1][6]	= 17;
// 	mtd9map[0][5]	= 18;

	int		nMtdHits		=  0;
	int		nMtdHitsLE		=  0;
	int		nMtdHitsTE		=  0;
	int		nMtdHitsBL[30]	=  {0};

	int		LE_tray[MAXMTDHITS];
	int		LE_strip[MAXMTDHITS];
	int		LE_chip[MAXMTDHITS];
	int		LE_chan[MAXMTDHITS];
	float	LE_coco[MAXMTDHITS];
	float	LE_trgdtime[MAXMTDHITS];
	float	LE_time[MAXMTDHITS];
	int		TE_tray[MAXMTDHITS];
//	int		TE_strip[MAXMTDHITS];
	int		TE_chan[MAXMTDHITS];
	float	TE_coco[MAXMTDHITS];
	float	TE_time[MAXMTDHITS];

	ddm = rdr->det("mtd")->get("legacy") ;
	if(!ddm) return -1;
	  
 	//---- event processing --------------------------------------------
 	while(ddm->iterate()) {
 		mtd = (mtd_t *)ddm->Void;
 
		for (int r = 0; r < 2; r++) {		// all 2 fibers....
			nwordstouse[r]	= mtd->ddl_words[r];	
		}
//TEST		cout<<nwordstouse[0]<<" "<<nwordstouse[1]<<endl;
		
 		//---- loop over fibers....
 		for (int r = 0; r < 2; r++) {		// all 2 fibers....
 
 			//---- loop over words in this DDLR...
 			for (u_int i=0; i<nwordstouse[r]; i++) {
 			
//TEST 				if (r==1){
//TEST 					cout<<hex<<mtd->ddl[r][i]<<" "<<dec<<endl;
//TEST 				}
 
				TsPacket *p = new TsPacket(mtd->ddl[r][i]);
				pid 		= p->GetId();
				isVPDhit 	= isP2Phit 	= false;
				cocogood	= false;

				if (pid == 0x0c) {
					trayiddata	= p->GetTrayId();
					trayid		= trayiddata;
					half 		= p->GetHalf();
					//cout<<trayiddata<<" "<<half<<endl;
				} else if (pid == 0xe) {
					// do nothing...
				//
				//---- LE hit....................................................
				} else if (p->IsLE()) { 					// leading edge
					int ch 	= 96*half + p->GetChannelTray5();
					if (trayid <= 120) { 						
						moduleid	= ch/6;
						//cellid		= ch - 6*moduleid;
						int tdcid	=(mtd->ddl[r][i] & 0x0F000000)>>24;		// 0-15
						int tdigboardid	= tdcid/4;   						// for halftray 0, 0-2 for tdig0; 4-6 for tdig1
						//if (half==1){ tdigboardid=4; }						// for halftray1, 0-2 for tdig 4
						//if (trayid==26){
						//	cout<<half*96+ch<<"\t"<<tdcid<<"\t"<<tdigboardid<<endl;
						//}
						int tdcchan		=(mtd->ddl[r][i] & 0x00E00000)>>21;	// tdcchan is 0-7 here.
						int istrip		= tdcchan2globalstrip(iday,tdigboardid,tdcid,tdcchan,trayid);
						coco 		= GetLETime(p, bunch);
						trgdtime	= GetTrgDTime(p, bunch);
						cocous		= ((float)coco)*COARSEBIN2NS/1000.;
						edgetime	= p->GetEdgeTime();
						edgetimens	= (25./1024.)*(Float_t)edgetime;
						cocouse		= coco - COARSESHIFT;
						//
						//if (istrip==-1){
						//if (trayid==26){
						//	cout<<"tray="<<trayid<<" half="<<half<<" tdc="<<tdcid
						//		<<" board="<<tdigboardid<<" tdcchan="<<tdcchan
						//		<<" strip="<<istrip<<" ch="<<ch<<endl;
						//}
						//}
						//
//						if(trayid==1){
//							h1=(TH1D*)gDirectory->Get("hcoco_mtd1"); h1->Fill(cocouse,1.0);
//						} else 
						sprintf(buf,"hcoco_mtd%d",trayid-1);
						h1=(TH1D*)gDirectory->Get(buf); h1->Fill(cocouse,1.0);
						if (cocouse>=cococut_mtd_l 
						 && cocouse<=cococut_mtd_u){
							cocogood	= true;
							sprintf(buf,"hcocokeep_mtd%d",trayid-1);
							h1=(TH1D*)gDirectory->Get(buf); h1->Fill(cocouse,1.0);
						}
						if (kDataChoice==1||kDataChoice==3){ cocogood = true; }
						if (cocogood){
						  if (nMtdHitsLE<MAXMTDHITS-1){
							LE_tray[nMtdHitsLE]		= trayid;
							LE_strip[nMtdHitsLE]	= istrip;
							LE_chan[nMtdHitsLE]		= ch;
							LE_chip[nMtdHitsLE]		= tdcid;
							LE_trgdtime[nMtdHitsLE]	= trgdtime;
							LE_coco[nMtdHitsLE]		= coco;
							LE_time[nMtdHitsLE]		= edgetimens;
							++nMtdHitsLE;
							++nMtdHitsBL[trayid-1];
						  } else {
						    cout<<"Too Many LE hits (MTD section)!!!! max=" << MAXMTDHITS << endl;
						  }
						}
 					}	//---- end LE trayid check...
 					//
 				//---- TE hit....................................................
				} else if (p->IsTE()) { 					// trailing edge
					int ch 	= 96*half + p->GetChannelTray5();
					if (trayid <= 120){
						moduleid	= ch/6;
						//cellid		= ch - 6*moduleid;
						coco 		= GetLETime(p, bunch);
						cocous		= ((float)coco)*COARSEBIN2NS/1000.;
						edgetime	= p->GetEdgeTime();
						edgetimens	= (25./1024.)*(Float_t)edgetime;
						cocouse		= coco - COARSESHIFT;
						if (cocouse>=cococut_mtd_l 
						 && cocouse<=cococut_mtd_u){		// add 25ns to high cut
							cocogood	= true;
						}
						if (kDataChoice==1||kDataChoice==3){ cocogood = true; }
						if (cocogood){
						  if (nMtdHitsTE<MAXMTDHITS-1){
							TE_tray[nMtdHitsTE]	= trayid;
							TE_chan[nMtdHitsTE]	= ch;
							TE_coco[nMtdHitsTE]	= coco;
							TE_time[nMtdHitsTE]	= edgetimens;
							++nMtdHitsTE;
						  } else {
						    cout << "Too many TE hits (MTD section)!!!! max=" << MAXMTDHITS << endl;
						  }
						}
					}	//---- end TE trayid check...
 					//
 				} else if (pid==0x02) {
 					bunch = p->GetBunchId();
				}	//---- end packetype check...
				delete p;

 			}	//---- end loop over words in this DDLR....
 		}	//---- end  loop over DDLRs....
  	}	//---- end dd iterator...

  	float	tle,tte,diffmin;
   
 	//---- post-event processing --------------------------------------------
	// 	
// 	const int MAXNHITPERCELL	= 20;
// 	const int MAXBUFFER			= 23040;
// 	int	BUF_nle[MAXBUFFER];							
// 	int	BUF_nte[MAXBUFFER];
// 	float	BUF_tle[MAXNHITPERCELL][MAXBUFFER];		
// 	float	BUF_tte[MAXNHITPERCELL][MAXBUFFER];		
// 	float	BUF_striple[MAXNHITPERCELL][MAXBUFFER];		
// 	float	BUF_chiple[MAXNHITPERCELL][MAXBUFFER];		
// 	for (int i=0;i<MAXBUFFER;i++){						
// 		BUF_nle[i] = BUF_nte[i] = 0;
// 	}
// 	for (int ihl=0;ihl<nMtdHitsLE;ihl++){
// 		int	i					= LE_tray[ihl]*192 + LE_chan[ihl];
// 		int k					= BUF_nle[i];
// 		++BUF_nle[i];
// 		if (k<MAXNHITPERCELL){
// 			BUF_tle[k][i]		= LE_time[ihl];
// 			BUF_striple[k][i]	= LE_strip[ihl];
// 			BUF_chiple[k][i]	= LE_chip[ihl];
// 		} else {
// 			cout<<"too many LE hits?!?!?!? "<<nMtdHitsLE<<"\t "<<LE_tray[ihl]<<" "<<LE_chan[ihl]<<"\t "<<BUF_nle[i]<<endl;
// 		}
// 	}
// 	for (int iht=0;iht<nMtdHitsTE;iht++){
// 		int	i					= TE_tray[iht]*192 + TE_chan[iht];
// 		int k					= BUF_nte[i];
// 		++BUF_nte[i];
// 		if (k<MAXNHITPERCELL){
// 			BUF_tte[k][i]		= TE_time[iht];
// 		} else {
// 			cout<<"too many TE hits?!?!?!? "<<nMtdHitsTE<<"\t "<<TE_tray[iht]<<" "<<TE_chan[iht]<<"\t "<<BUF_nte[i]<<endl;
// 		}
// 	}
// 

 	int		ktray,kmodule,kcell,ihtmintot;
 	float	thistot;

// 	for (int i=0;i<MAXBUFFER;i++){
// 		//
// 		if (BUF_nle[i]==0 || BUF_nte[i]==0){ continue; }
// 		//
// 		ktray		= (i/192);
// 		kmodule		= (i - ktray*192)/6;
// 		kcell		= (i - ktray*192 - kmodule*6);
// //cout<<nwordstouse[0]<<" "<<nMtdHitsLE<<" "<<nMtdHitsTE<<"\t"
// //	<<ktray<<" "<<kmodule<<" "<<kcell<<"\t"
// //	<<i<<" "<<BUF_nle[i]<<" "<<BUF_nte[i]
// //	<<endl;
// 		//
// 		if (BUF_nle[i]==1 && BUF_nte[i]==1){
// 			thistot	= BUF_tte[0][i] - BUF_tle[0][i];
// 			if (thistot>0.0){
// 				int k	= nMtdHits;
// 				++nMtdHits;	
// 				MTD->tray[k]	= ktray;
// 				MTD->module[k]	= kmodule;
// 				MTD->cell[k]	= BUF_striple[0][i];
// 				MTD->chip[k]	= BUF_chiple[0][i];
// 				MTD->tle[k]		= BUF_tle[0][i];
// 				MTD->tte[k]		= BUF_tte[0][i];
// 				MTD->tot[k]		= BUF_tte[0][i] - BUF_tle[0][i];
// 			}
// 		//
// 		} else if (BUF_nle[i]>1 || BUF_nte[i]>1){
// 			for (int ihl=0;ihl<BUF_nle[i];ihl++){
// 				tle		= BUF_tle[ihl][i];
// 				if (tle==0){ continue; }		//????????????????????????
// 				ihtmintot	= -1;
// 				thistot		= 999999.;
// 				if (BUF_nte[i] >= MAXNHITPERCELL){ 
// 					cout<<"too many hits in buffer?!?!?! "<<i<<" "<<BUF_nle[i]<<" "<<ihl<<" "<<tle<<"\t "<<BUF_nte[i]<<endl;
// 					continue;
// 				}
// 				for (int iht=0;iht<BUF_nte[i];iht++){
// 					tte		= BUF_tte[iht][i];
// 					if (tte>=0.0){
// 						if (tte-tle>0.0 && tte-tle<thistot){
// 							thistot		= tte-tle;
// 							ihtmintot	= iht;
// 						}
// 					}
// 				}
// 				if (ihtmintot>-1 && thistot>0){
// 					int k	= nMtdHits;
// 					++nMtdHits;	
// 					MTD->tray[k]			= ktray;
// 					MTD->module[k]			= kmodule;
// 					MTD->cell[k]			= BUF_striple[ihl][i];
// 					MTD->chip[k]			= BUF_chiple[ihl][i];
// 					MTD->tle[k]				= BUF_tle[ihl][i];
// 					MTD->tte[k]				= BUF_tte[ihtmintot][i];
// 					//MTD->tot[k]			= BUF_tte[ihtmintot][i] - BUF_tle[ihl][i];
// 					MTD->tot[k]				= thistot;
// 					BUF_tte[ihtmintot][i]	= -1.;		// insure this TE is not reused....
// 				}
// 			}
// 		}
// 	}

	int modch;
	for (int ihl=0;ihl<nMtdHitsLE;ihl++){
		int k				= nMtdHits;
		int kModule			= -1;
		MTD->tray[k]		= LE_tray[ihl];
		MTD->cell[k]		= LE_chan[ihl];
		int stripid,zendid;
// 		if (LE_tray[ihl]==1){
// 			kModule			= 3;							// MTD9
// 			stripid			= LE_strip[ihl];
// 			if (LE_chan[ihl]<96){ zendid=0; } else { zendid=1; }
// 			modch			= LE_strip[ihl] + 18*zendid;
// 		} else 
//		if (LE_tray[ihl]==26){
			stripid			= (LE_strip[ihl]-1)%12+1;
			zendid			= (LE_strip[ihl]-1)/12;
//			if (LE_chan[ihl]<24){ kModule = 0; } else		// MTD26E
//			if (LE_chan[ihl]<48){ kModule = 1; } else		// MTD26C
//								{ kModule = 2; } 			// MTD26W
			kModule			= LE_chan[ihl]/24;
			modch			= LE_strip[ihl];
//		}
		MTD->module[k]		= kModule;
		MTD->modch[k]		= modch;				// channel in a module
		MTD->strip[k]		= stripid;				// strip ID
		MTD->strlr[k]		= zendid;				// left/right side of a strip
		MTD->coco[k]		= LE_coco[ihl];
		MTD->trgdtime[k]	= LE_trgdtime[ihl];
		MTD->tle[k]			= LE_time[ihl];
		MTD->tot[k]			= 60.0;
// 	if (kModule==1||kModule==2){
// 		cout<<kModule<<" "<<ihl<<" "<<LE_time[ihl]<<"......................................."<<endl;
// 	}
// 		if (mismatch){
// 			cout<<ihl<<" MISMATCH... "<<kModule<<" "<<modch<<" "<<stripid<<" "<<zendid<<" "
// 				<<LE_trgdtime[ihl]<<" "<<LE_coco[ihl]<<" "<<LE_time[ihl]
// 				<<endl;
// 		}
		if (LE_trgdtime[ihl]<0.||LE_trgdtime[ihl]>25000.){
 			cout<<ihl<<" trg time? "<<kModule<<" "<<modch<<" "<<stripid<<" "<<zendid<<" "
 				<<LE_trgdtime[ihl]<<" "<<LE_coco[ihl]<<" "<<LE_time[ihl]
 				<<endl;   
		}
		for (int iht=0;iht<nMtdHitsTE;iht++){
			if ( TE_tray[iht] == LE_tray[ihl]
			  && TE_chan[iht] == LE_chan[ihl]){
				thistot		= TE_time[iht] - LE_time[ihl];
// 	if (kModule==1||kModule==2){
// 		cout<<iht<<" "<<TE_time[iht]<<" "<<thistot<<endl;
// 	}
				if (thistot>0.0 && thistot<60.0){
					if ((MTD->tot[k]> 59.9) || 
					    (MTD->tot[k]<=59.9&&thistot<MTD->tot[k])){
						MTD->tte[k]	= TE_time[iht];
						MTD->tot[k]	= thistot;
					}
				}
			}
		}
		++nMtdHits;
	}
	
 	MTD->nMtdHits			= nMtdHits;
 	MTD->nMtdHitsLE			= nMtdHitsLE;
 	MTD->nMtdHitsTE			= nMtdHitsTE;
 	for (int i=0;i<30;i++){
		MYINFO->nrawhits_mtd[i]	= nMtdHitsBL[i];
	}
	return 0;
}	
//---- end mtd doer....

int tdcchan2globalstrip(int iday,int tdigboardid,int tdcid,int tdcchan,int trayid)
{
	//
	int globalstripid=-1;
//	if ( trayid!=26 && trayid!=1 ){
//		cout<<"doer_mtd unknown tray number "<<trayid<<endl;
//		return globalstripid;
//	}
	int tdcid0 = tdcid;
	if(trayid>=1&&trayid<=30){
//		if (tdcid>3) tdcid=tdcid-4; //scale to H#
//		if (tdcid>3) tdcid=tdcid-4; //scale to H#			// needed for backleg 28??!?!?!?!?!?
		if (tdcid>3){ tdcid=tdcid%4; }
		int globaltdcchan=(tdcid+1)*10+tdcchan;
		int mtdstrip[24]={34,22,10,37,27,17,33,23,16,36,26,15,
			21,12,32,20,14,35,25,13,30,24,11,31};
		for(int i=0;i<24;i++){
			if(mtdstrip[i]==globaltdcchan) {globalstripid=i+1;break;}
		}
	}
// 	if(trayid==1){
// // 		int globaltdcchan=(tdcid+1)*10+tdcchan;
// // 		int mtdstrip[18]={34,22,10,37,27,17,16,36,32,20,14,35,25,13,30,24,11,31};
// // 		for(int i=0;i<18;i++){
// // 			if(mtdstrip[i]==globaltdcchan) {globalstripid=i+1;break;}
// // 		}
// 		globalstripid	= mtd9map[tdcid][tdcchan];
// 		if (globalstripid>18){
// //			cout<<"doer_mtd Map error "<<trayid<<" "<<tdigboardid<<" "<<tdcid<<" "
// //				<<tdcchan<<" "<<globalstripid<<endl;
// 			globalstripid	= -1;
// 		}
// 	}
	if (globalstripid==-1){ cout<<"Strip mapping error!!! ...tdig="<<tdigboardid
								<<" tdcid0="<<tdcid0<<" tdcid="<<tdcid<<" tdcchan="<<tdcchan
								<<" trayid="<<trayid<<endl; }
	return globalstripid;
}



