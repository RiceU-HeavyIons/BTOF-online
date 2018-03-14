// $Id: main.cc,v 1.1 2016/02/25 16:02:52 geurts Exp $
// Author: W.J. Llope 2009/06/01

#include <stdio.h>
#include <unistd.h>
#include <getopt.h>
#include <sys/types.h>
#include <stdlib.h>
#include <iostream>

using namespace std;

#include <rtsLog.h>
#include "doer.h"

bool TakeAllData	= true;

float TOFMULTMAX	= 3200;

const int NEVTMAX	= -1;	 	// -1 for all, set lower for testing....

bool SkipTrgDetTree		= true;
bool SkipTofStopTree	= true;
bool SkipTofStopTreeFill= true;
bool SkipMtdStopTree	= true;
bool SkipMtdStopTreeFill= true;


//---- map of TRG digitised VPD channel to TOF digitised VPD channel
// index is TRG ID, result is TOF ID.
//---- RUN-15
Int_t map_vpd_trg2tof_west[16] =
	{  0,  1,  2,  3,
	   6,  7,  8,  9,
	  10, 11, 12,  4,
	  15, 16, 17, 18 };
Int_t map_vpd_trg2tof_east[16] =
	{  0,  1,  2,  3,
	   6,  7,  8,  9,
	  10, 12, 11,  4,
	  15, 16, 17, 18 };


//---- map of p2p Si SEQ ID to p2p cntr ID (which uses the TOF map!)
// index is SEQ, result is cntr ID.
Int_t map_seq_cntr[8] = { 5, 4, 6, 7, 0, 1, 3, 2 };

//---- definitions...
//
int tof_doer(daqReader *rdr, 	struct P2P_st *P2P, struct doer_st *TOF,
								Int_t iday, Int_t kChoice,
								struct stop_st *STOP, struct INFO_st *MYINFO);
//
int mtd_doer(daqReader *rdr,	Int_t iday, Int_t kChoice,
								struct mtd_st *MTD, struct INFO_st *MYINFO);
//
//int pp2pp_doer(daqReader *rdr, 	int StoreRaw, SVX_st *SVX, Hits_st *Hits, sili_st *si);
//
//int trg_doer(daqReader *rdr, 	struct bunch_crossing_st *xing, struct P2P_st *P2P,
//								struct DSM_st *DSM, struct VPD_st *VPD, struct TRG_st *TRG);
int trg_doer(daqReader *rdr, 	struct bunch_crossing_st *xing, struct P2P_st *P2P,
								struct VPD_st *VPD, struct TRG_st *TRG);
//
int tof_err_check(unsigned int words[], int bytes, int rdo1);

int GetTHUB(int trayid);
int GetNINO(int tdc,int chan);

//---- tray based windows....
//--- 16039022 run15 pp200
	float TrayTrigTimingPeaks[122] = {
	2852.3,2852.9,2853.2,2852.5,2860.3,2859.2,2859.7,2860.9,2860.1,2860.5,
	2860.3,2860.5,2859.3,2859.2,2860.8,2860.9,2853.6,2853.7,2854.4,2853.4,
	2780.1,2779.9,2780.8,2779.7,2776.9,2777.6,2785.6,2787.6,2784.5,2785.0,
	2784.7,2792.9,2794.7,2793.3,2794.2,2793.4,2795.2,  0.0,2793.9,2787.2,
	  0.0,2786.9,2787.1,2784.6,2777.6,2777.1,2778.9,2780.1,2779.0,2776.9,
	2835.2,2837.7,2835.4,2842.9,2843.8,2846.3,2845.7,2846.0,2852.8,2853.3,
	2848.9,2848.4,2840.2,2841.2,2841.3,2845.9,2843.2,2845.2,2843.6,2845.2,
	2844.6,2851.1,2853.4,2849.6,2851.8,2850.5,2858.4,2860.9,2860.6,2859.4,
	2858.5,2859.5,2859.0,2857.9,2850.0,2850.6,2850.3,2850.3,2850.1,2843.4,
	2842.9,2843.5,2844.1,2843.5,2843.8,2857.6,2856.8,2856.3,2856.9,2865.0,
	2865.0,  0.0,2865.2,2865.1,2864.5,2864.5,2863.9,2865.2,2864.8,2863.5,
	2864.3,2855.2,2858.8,2856.1,2855.3,2855.8,2856.7,2848.1,2847.6,2848.8,
	205.0,214.5
	};


//---- thub based windows....
//--- 16039022 run15 pp200
	int trigwindowLow[6] ={2760,2825,2825,2825,175,185};
	int trigwindowHigh[6]={2820,2885,2885,2885,245,245};

//---- crossing fit info....
	float crossingorigin = -2977.89;
	float crossingperiod = 106.463;

//--- MTD 14076018
//--- 15086055 run14 auau200
	int trigwindowLow_mtd[2]  = {2880,2825};	// run14 auau200
	int trigwindowHigh_mtd[2] = {2950,2895};

//---- use these if you need to gate the noise run in time....
	unsigned long itime_min_use 	= 0;
	unsigned long itime_max_use 	= 0;
	unsigned long itime_exclude_min	= 0;
	unsigned long itime_exclude_max	= 0;
//
	UInt_t	evttime_start	= 0;
	UInt_t	evttime;

//--------------------------------------------------------------------------------------------
//
int main(int argc, char **argv)
{

    extern char	*optarg ;
    extern int 	optind ;
    int 		c,istat,ievtnum=0;
    char 		_mountpoint[1024];
    char 		*mountpoint = NULL;
    char 		rootfilename[80];
    daqReader	*evp;	// the main guy
    int 		append;
    int 		SiRawFlag;
	char		buf[80];
	char		tbuf[100];
	int			nev_det[3] = {0,0,0};
	int			daynum,runnum;
	int			filenum=0;
	int			ictr,ilr,idet;
	char		ofilestr[40];
	int			nskipped	= 0;

	unsigned long itime_min = -1;
	unsigned long itime_max =  0;

	evp_data_st			evp_data;
	//sili_st				sili;
	//Hits_st				Hits;
	//SVX_st				SVX;
	bunch_crossing_st	xing;
	P2P_st				P2P;
	//DSM_st				DSM;
	VPD_st				*VPD = new VPD_st();
	TRG_st				*TRG = new TRG_st();
	doer_st				*TOF = new doer_st();
	stop_st				*STOP = new stop_st();
	mtd_st				*MTD = new mtd_st();
	INFO_st				*MYINFO = new INFO_st();


	unsigned int		daqbits;
	unsigned long long	daqbits64;

	int	nevt_seen		= 0;
	int	nevt_tofintime	= 0;
	int	nevt_tofearly	= 0;
	int ntrg_zdc 		= 0;
	int ntrg_vpd 		= 0;
	int ntrg_bbcsmall 	= 0;
	int nevt_bytrig[32] = {0};
	float aliml,alimu;
	int		nMtdHitsBL[30]			= {0};
	int		nMtdHitsBL_total[30]	= {0};

	int nhitstof_tof;
	int nhitstof_trg;

    // rtsLogOutput(RTS_LOG_STDERR) ;
    //rtsLogLevel(WARN) ;

	int kDataChoice 		= 0;
    cout<<"What kind of Data?"<<endl;
    	cout<<"\t 0=beam,  save trigger detector info, no trees..."<<endl;
    	cout<<"\t 1=noise, save TOF/MTD hits trees, no coarse counter cut..."<<endl;
    	cout<<"\t 2=beam,  save **MTD** hits trees, w/ coarse counter cut..."<<endl;
     	cout<<"\t 3=beam,  no coarse counter cut for trigger time plots (TOF&MTD)..."<<endl;
		cout<<endl;
		cout<<"0->online->gainfit, 1->noise, 2->mtdview (_mtd), 3->thub (_tof)"<<endl;
		cout<<endl;
    cin>>kDataChoice;

    cout<<"Enter run string for output file name"<<endl;
    cin>>ofilestr;
    cout<<"ofilestr        = "<<ofilestr<<endl;

	if (kDataChoice==0){
		SkipTrgDetTree		= kFALSE;
		SkipTofStopTree		= kTRUE;
		SkipTofStopTreeFill	= kTRUE;
		SkipMtdStopTree		= kTRUE;
		SkipMtdStopTreeFill	= kTRUE;
	} else if (kDataChoice==1){
		SkipTrgDetTree		= kTRUE;
		SkipTofStopTree		= kFALSE;
		SkipTofStopTreeFill	= kFALSE;
		SkipMtdStopTree		= kFALSE;
		SkipMtdStopTreeFill	= kFALSE;
	} else if (kDataChoice==2){
		SkipTrgDetTree		= kTRUE;
		SkipTofStopTree		= kTRUE;
		SkipTofStopTreeFill	= kTRUE;
		SkipMtdStopTree		= kFALSE;
		SkipMtdStopTreeFill	= kFALSE;
	} else if (kDataChoice==3){
		SkipTrgDetTree		= kFALSE;
		SkipTofStopTree		= kFALSE;
		SkipTofStopTreeFill	= kTRUE;
		SkipMtdStopTree		= kFALSE;
		SkipMtdStopTreeFill	= kTRUE;
	}
    cout<<"kDataChoice         = "<<kDataChoice<<endl;
    cout<<"TakeAllData         = "<<(int)TakeAllData<<endl;
    cout<<"SkipTrgDetTree      = "<<(int)SkipTrgDetTree<<endl;
    cout<<"SkipTofStopTree     = "<<(int)SkipTofStopTree<<endl;
    cout<<"SkipTofStopTreeFill = "<<(int)SkipTofStopTreeFill<<endl;
    cout<<"SkipMtdStopTree     = "<<(int)SkipMtdStopTree<<endl;
    cout<<"SkipMtdStopTreeFill = "<<(int)SkipMtdStopTreeFill<<endl;

    sprintf(rootfilename, "/gpfs01/star/subsysg/TOF/BTOF-online/dd/daqdoer_%s.root", ofilestr);
    cout<<"  out file name = "<<rootfilename<<endl;

    TFile *f = new TFile(rootfilename, "RECREATE", "DAQdoer tree");
	//
	TH1D *hnwordsfiber[4];
	for (int i=0;i<4;i++){
		sprintf(buf,"hnwordsfiber%d",i);
		hnwordsfiber[i]	= new TH1D(buf,buf,125,0,12500);
	}
	//
	TH1D *hp_vpd_tof = new TH1D("hp_vpd_tof","hp_vpd_tof",38,-0.5,37.5);
	TH1D *hp_p2p_tof = new TH1D("hp_p2p_tof","hp_p2p_tof",16,-0.5,15.5);
	TH1D *hp_vpd_trg = new TH1D("hp_vpd_trg","hp_vpd_trg",38,-0.5,37.5);
	TH1D *hp_p2p_trg = new TH1D("hp_p2p_trg","hp_p2p_trg",16,-0.5,15.5);
	//
	TH1D *hp_bbc = new TH1D("hp_bbc","hp_bbc",48,-0.5,47.5);
	TH1D *hp_bbc_tofintime = new TH1D("hp_bbc_tofintime","hp_bbc_tofintime",48,-0.5,47.5);
	TH1D *hp_bbc_tofearly = new TH1D("hp_bbc_tofearly","hp_bbc_tofearly",48,-0.5,47.5);
	TH1D *hnh_bbcl = new TH1D("hnh_bbcl","hnh_bbcl",17,-0.5,16.5);
	TH1D *hnh_bbcl_tofintime = new TH1D("hnh_bbcl_tofintime","hnh_bbcl_tofintime",17,-0.5,16.5);
	TH1D *hnh_bbcl_tofearly = new TH1D("hnh_bbcl_tofearly","hnh_bbcl_tofearly",17,-0.5,16.5);
	//TH1D *htrgbits = new TH1D("htrgbits","htrgbits",32,-0.5,31.5);
	//TH1D *htrgbits_tofintime = new TH1D("htrgbits_tofintime","htrgbits_tofintime",32,-0.5,31.5);
	//TH1D *htrgbits_tofearly = new TH1D("htrgbits_tofearly","htrgbits_tofearly",32,-0.5,31.5);
	TH1D *htrgbits = new TH1D("htrgbits","htrgbits",64,-0.5,63.5);
	TH1D *htrgbits_tofintime = new TH1D("htrgbits_tofintime","htrgbits_tofintime",64,-0.5,63.5);
	TH1D *htrgbits_tofearly = new TH1D("htrgbits_tofearly","htrgbits_tofearly",64,-0.5,63.5);
	//
	TH1D *hcoco_vpdw = new TH1D("hcoco_vpdw","hcoco_vpdw",256,-0.5,255.5);
	TH1D *hcoco_vpde = new TH1D("hcoco_vpde","hcoco_vpde",256,-0.5,255.5);
	TH1D *hcocokeep_vpdw = new TH1D("hcocokeep_vpdw","hcocokeep_vpdw",256,-0.5,255.5);
	TH1D *hcocokeep_vpde = new TH1D("hcocokeep_vpde","hcocokeep_vpde",256,-0.5,255.5);
	//
	TH1D *hcoco_mtd[30];
	TH1D *hcocokeep_mtd[30];
	for (int i=0;i<30;i++){
		sprintf(buf,"hcoco_mtd%d",i);
		hcoco_mtd[i]	= new TH1D(buf,buf,256,-0.5,255.5);
		sprintf(buf,"hcocokeep_mtd%d",i);
		hcocokeep_mtd[i]= new TH1D(buf,buf,256,-0.5,255.5);
	}
	//
	TH1D *hcoco_p2pw = new TH1D("hcoco_p2pw","hcoco_p2pw",256,-0.5,255.5);
	TH1D *hcoco_p2pe = new TH1D("hcoco_p2pe","hcoco_p2pe",256,-0.5,255.5);
 	TH1D *hcoco_p2p[16];
 	TH1D *hcoco_p2p_p2p[16];
 	for (int i=0;i<16;i++){
 		sprintf(buf,"hcoco_p2p%d",i);
 					 hcoco_p2p[i]		= new TH1D(buf,buf,55,80.5,135.5);
 		sprintf(buf,"hcoco_p2p_p2p%d",i);
 					 hcoco_p2p_p2p[i]	= new TH1D(buf,buf,55,80.5,135.5);
 					 hcoco_p2p_p2p[i]->SetLineColor(2);
 	}
 	//
 	TH1D *htrgdtime_rawall = new TH1D("htrgdtime_rawall","htrgdtime_rawall",600,-500.0,5500.0);
 	TH1D *htrgdtime[6];
 	TH1D *htrgdtimekeep[6];
 	TH1D *htrgdtimekeep3[6];
 	for (int i=0;i<6;i++){
		sprintf(buf,"htrgdtime%d",i);
		htrgdtime[i]		= new TH1D(buf,buf,5500,-250.5,5249.5);
		sprintf(buf,"htrgdtimekeep%d",i);
		htrgdtimekeep[i]	= new TH1D(buf,buf,5500,-250.5,5249.5);
			htrgdtimekeep[i]->SetFillStyle(1001);
			htrgdtimekeep[i]->SetFillColor(3);
		sprintf(buf,"htrgdtimekeep3%d",i);
		htrgdtimekeep3[i]	= new TH1D(buf,buf,5500,-250.5,5249.5);
			htrgdtimekeep3[i]->SetFillStyle(1001);
			htrgdtimekeep3[i]->SetFillColor(7);
 	}
 	TH1D *htrgdtime_traycorrected=new TH1D("htrgdtime_traycorrected","htrgdtime_traycorrected",6001,-3000.5,3000.5);
 	TH2D *htrgdtime_traycorrected_tray=new TH2D("htrgdtime_traycorrected_tray","htrgdtime_traycorrected_tray",122,0.5,122.5,6001,-3000.5,3000.5);
 	TH1D *hcrossing_traycorrected=new TH1D("hcrossing_traycorrected","hcrossing_traycorrected",1060,-31,22.);
 	TH1D *hcrossingkeep_traycorrected=new TH1D("hcrossingkeep_traycorrected","hcrossingkeep_traycorrected",1060,-33.,22.);
	//
 	TH1D *htrgdtime_VPD[38];
 	for (int i=0;i<38;i++){
		sprintf(buf,"htrgdtime_VPD%d",i);
		htrgdtime_VPD[i]	= new TH1D(buf,buf,5500,-250.5,5249.5);
	}
	//
 	TH2D *htrgdtime_tray=new TH2D("htrgdtime_tray","htrgdtime_tray",122,0.5,122.5,5201,-0.5,5200.5);
 	TH2D *htrgdtime_trayoff=new TH2D("htrgdtime_trayoff","htrgdtime_trayoff",122,0.5,122.5,6001,-3000.5,3000.5);
 	TH2D *hnearly_nintime=new TH2D("hnearly_nintime","hnearly_nintime",100,0.0,500.,100,0,500.);
	//
 	TH1D *htrgdtime0_bytrig[64];
 	TH1D *htrgdtime0keep_bytrig[64];
 	TH1D *htrgdtime_bytrig[6][64];
 	TH1D *htrgdtimekeep_bytrig[6][64];
 	TH1D *htrgdtime_traycorrected_bytrig[64];
 	TH1D *hnhits_prec_bytrig[64];
 	TH1D *hnhits_tric_bytrig[64];
 	TH1D *hnhits_posc_bytrig[64];
 	for (int i=0;i<64;i++){
		sprintf(buf,"htrgdtime0_bytrig%d",i);
		htrgdtime0_bytrig[i]		= new TH1D(buf,buf,5201,-0.5,5200.5);
		sprintf(buf,"htrgdtime0keep_bytrig%d",i);
		htrgdtime0keep_bytrig[i]		= new TH1D(buf,buf,5201,-0.5,5200.5);
			htrgdtime0keep_bytrig[i]->SetFillStyle(1001);
			htrgdtime0keep_bytrig[i]->SetFillColor(3);
		for (int ic=0;ic<6;ic++){
			sprintf(buf,"htrgdtime_bytrig%d%d",ic,i);
			htrgdtime_bytrig[ic][i]		= new TH1D(buf,buf,5201,-0.5,5200.5);
			sprintf(buf,"htrgdtimekeep_bytrig%d%d",ic,i);
			htrgdtimekeep_bytrig[ic][i]	= new TH1D(buf,buf,5201,-0.5,5200.5);
				htrgdtimekeep_bytrig[ic][i]->SetFillStyle(1001);
				htrgdtimekeep_bytrig[ic][i]->SetFillColor(3);
		}
		sprintf(buf,"htrgdtime_traycorrected_bytrig%d",i);
		htrgdtime_traycorrected_bytrig[i]		= new TH1D(buf,buf,5201,-0.5,5200.5);
		sprintf(buf,"hnhits_prec_bytrig%d",i);
		hnhits_prec_bytrig[i]		= new TH1D(buf,buf,200,0,TOFMULTMAX);
		sprintf(buf,"hnhits_tric_bytrig%d",i);
		hnhits_tric_bytrig[i]		= new TH1D(buf,buf,200,0,TOFMULTMAX);
		sprintf(buf,"hnhits_posc_bytrig%d",i);
		hnhits_posc_bytrig[i]		= new TH1D(buf,buf,200,0,TOFMULTMAX);
	}
	//
	TH2D *hbunchidoffset_tray = new TH2D("hbunchidoffset_tray","hbunchidoffset_tray",244,-0.5,243.5,51,-25.5,25.5);
	TH1D *hbunchidoffset_thubhalf[6][2];
	for (int ith=0;ith<6;ith++){
		for (int ih=0;ih<2;ih++){
			sprintf(buf,"hbunchidoffset_thubhalf%d%d",ith,ih);
			hbunchidoffset_thubhalf[ith][ih] = new TH1D(buf,buf,251,-125.5,125.5);
		}
	}
	TH1D *htray_nobunchid	= new TH1D("htray_nobunchid","htray_nobunchid",122,-0.5,121.5);

	TH2D *hbunchid_tray		= new TH2D("hbunchid_tray","hbunchid_tray",122,0.5,122.5,128,0.,4096.);

	//---- trg tofmult vs Nhits correlations...
	TH2D *hmultcorr_tray[122];
	TH1D *hmultcorr_tray_diff[122];
	for (int i=0;i<122;i++){
		sprintf(buf,"hmultcorr_tray%d",i);
		sprintf(tbuf,"Mult L0 vs TOF, tray=%d",i);
		hmultcorr_tray[i]		= new TH2D(buf,tbuf,50,-0.5,49.5,50,-0.5,49.5);
		sprintf(buf,"hmultcorr_tray_diff%d",i);
		sprintf(tbuf,"Mult TOF-L0, tray=%d",i);
		hmultcorr_tray_diff[i]	= new TH1D(buf,tbuf,101,-50.5,50.5);
	}
	//
	TH2D *hmultcorrtight_tray[122];
	TH1D *hmultcorrtight_tray_diff[122];
	for (int i=0;i<122;i++){
		sprintf(buf,"hmultcorrtight_tray%d",i);
		sprintf(tbuf,"Mult L0 vs TOF TIGHT, tray=%d",i);
		hmultcorrtight_tray[i]		= new TH2D(buf,tbuf,50,-0.5,49.5,50,-0.5,49.5);
		sprintf(buf,"hmultcorrtight_tray_diff%d",i);
		sprintf(tbuf,"Mult TOF-L0 TIGHT, tray=%d",i);
		hmultcorrtight_tray_diff[i]	= new TH1D(buf,tbuf,101,-50.5,50.5);
	}
	//
	TH2D *hninocorr_tray[122];
	TH1D *hninocorr_tray_diff[122];
	TH1D *hninocorr_tray_diffnoax[122];
	TH1D *htraytotmult_tof = new TH1D("htraytotmult_tof","htraytotmult_tof",200,0,200);
	TH1D *htraytotmult_trg = new TH1D("htraytotmult_trg","htraytotmult_trg",200,0,200);
	TH2D *htraytotmult_trgvstof = new TH2D("htraytotmult_trgvstof","htraytotmult_trgvstof",200,0,200,200,0,200);
	TH1D *htraytotmult_tofminustrg = new TH1D("htraytotmult_tofminustrg","htraytotmult_tofminustrg",200,-100,100);
	for (int i=0;i<122;i++){
		sprintf(buf,"hninocorr_tray%d",i);
		sprintf(tbuf,"L0 vs Nnino, tray=%d",i);
		hninocorr_tray[i]			= new TH2D(buf,tbuf,25,-0.5,24.5,25,-0.5,24.5);
		sprintf(buf,"hninocorr_tray_diff%d",i);
		sprintf(tbuf,"Nnino-L0, tray=%d",i);
		hninocorr_tray_diff[i]		= new TH1D(buf,tbuf,49,-24.5,24.5);
		sprintf(buf,"hninocorr_tray_diffnoax%d",i);
		sprintf(tbuf,"Nnino-L0, tray=%d",i);
		hninocorr_tray_diffnoax[i]	= new TH1D(buf,tbuf,49,-24.5,24.5);
	}
	//
	TH2D *hninocorrtight_tray[122];
	TH1D *hninocorrtight_tray_diff[122];
	TH1D *hninocorrtight_tray_diffnoax[122];
	for (int i=0;i<122;i++){
		sprintf(buf,"hninocorrtight_tray%d",i);
		sprintf(tbuf,"L0 vs Nnino TIGHT, tray=%d",i);
		hninocorrtight_tray[i]			= new TH2D(buf,tbuf,25,-0.5,24.5,25,-0.5,24.5);
		sprintf(buf,"hninocorrtight_tray_diff%d",i);
		sprintf(tbuf,"Nnino-L0 TIGHT, tray=%d",i);
		hninocorrtight_tray_diff[i]		= new TH1D(buf,tbuf,49,-24.5,24.5);
		sprintf(buf,"hninocorrtight_tray_diffnoax%d",i);
		sprintf(tbuf,"Nnino-L0 TIGHT, tray=%d",i);
		hninocorrtight_tray_diffnoax[i]	= new TH1D(buf,tbuf,49,-24.5,24.5);
	}

 	TH1D *htrgdtime_mtd[2];
 	TH1D *htrgdtimekeep_mtd[2];
 	for (int i=0;i<2;i++){
		sprintf(buf,"htrgdtime_mtd%d",i);
		htrgdtime_mtd[i]		= new TH1D(buf,buf,5201,-0.5,5200.5);
		sprintf(buf,"htrgdtimekeep_mtd%d",i);
		htrgdtimekeep_mtd[i]	= new TH1D(buf,buf,5201,-0.5,5200.5);
	}
 	TH1D *htrgdtime_mtd_tray[30];
 	TH1D *htrgdtimekeep_mtd_tray[30];
 	for (int i=0;i<30;i++){
		sprintf(buf,"htrgdtime_mtd_tray%d",i);
		htrgdtime_mtd_tray[i] = new TH1D(buf,buf,5201,-0.5,5200.5);
		sprintf(buf,"htrgdtimekeep_mtd_tray%d",i);
		htrgdtimekeep_mtd_tray[i] = new TH1D(buf,buf,5201,-0.5,5200.5);
	}

	TH1D *hntofhits_crossing[48];
	for (int i=0;i<48;i++){
		sprintf(buf,"hntofhits_crossing%d",i);
		sprintf(tbuf,"TofMult, Crossing=%d",i-28);
		hntofhits_crossing[i]	= new TH1D(buf,tbuf,200,0.0,TOFMULTMAX);
	}

	TH1D *hnhitstof_tof		= new TH1D("hnhitstof_tof","hnhitstof_tof",700,0.0,3500.);
	TH1D *hnhitstof_trg		= new TH1D("hnhitstof_trg","hnhitstof_trg",400,0.0,2000.);
	TH2D *hnhitstof_trgtof	= new TH2D("hnhitstof_trgtof","hnhitstof_trgtof",140,0.0,3500.,80,0.0,2000.);
	TH1D *hnhitstof_tof_low		= new TH1D("hnhitstof_tof_low","hnhitstof_tof_low",200,0.,TOFMULTMAX);
	TH1D *hnhitstof_trg_low		= new TH1D("hnhitstof_trg_low","hnhitstof_trg_low",200,0.,TOFMULTMAX);
	TH2D *hnhitstof_trgtof_low	= new TH2D("hnhitstof_trgtof_low","hnhitstof_trgtof_low",200,0.,TOFMULTMAX,200,0.,TOFMULTMAX);

	TH1D *hnhitstof_xingall		= new TH1D("hnhitstof_xingall","hnhitstof_xingall",200,0.,TOFMULTMAX);
	TH1D *hnhitstof_xingtrg		= new TH1D("hnhitstof_xingtrg","hnhitstof_xingtrg",200,0.,TOFMULTMAX);
	TH1D *hnhitstof_xingm2		= new TH1D("hnhitstof_xingm2","hnhitstof_xingm2",200,0.,TOFMULTMAX);
	TH1D *hnhitstof_xingp2		= new TH1D("hnhitstof_xingp2","hnhitstof_xingp2",200,0.,TOFMULTMAX);
	TH1D *hnhitstof_xingm7		= new TH1D("hnhitstof_xingm7","hnhitstof_xingm7",200,0.,TOFMULTMAX);
	TH1D *hnhitstof_xingp7		= new TH1D("hnhitstof_xingp7","hnhitstof_xingp7",200,0.,TOFMULTMAX);
	TH1D *hnhitstof_xingtrg_vpd	= new TH1D("hnhitstof_xingtrg_vpd","hnhitstof_xingtrg_vpd",200,0.,TOFMULTMAX);
	TH1D *hnhitstof_xingm2_vpd	= new TH1D("hnhitstof_xingm2_vpd","hnhitstof_xingm2_vpd",200,0.,TOFMULTMAX);
	TH1D *hnhitstof_xingp2_vpd	= new TH1D("hnhitstof_xingp2_vpd","hnhitstof_xingp2_vpd",200,0.,TOFMULTMAX);
	TH1D *hnhitstof_xingm7_vpd	= new TH1D("hnhitstof_xingm7_vpd","hnhitstof_xingm7_vpd",200,0.,TOFMULTMAX);
	TH1D *hnhitstof_xingp7_vpd	= new TH1D("hnhitstof_xingp7_vpd","hnhitstof_xingp7_vpd",200,0.,TOFMULTMAX);

	TH1D *hnhitstof_time 		= 0;
	TH1D *hnhitsmtd_time[30]	= {0};
	if (itime_min_use>0 && itime_max_use>itime_min_use){
		int nsec	= itime_max_use - itime_min_use + 60;
		cout<<" TimeMin use = "<<itime_min_use<<endl;
		cout<<" TimeMax use = "<<itime_max_use<<endl;
		cout<<"        nsec = "<<itime_max_use-itime_min_use<<endl;
		hnhitstof_time		= new TH1D("hnhitstof_time","hnhitstof_time",nsec,itime_min_use-30,itime_max_use+30);
		for (int i=0;i<30;i++){
			sprintf(buf,"hnhitsmtd_time%d",i);
			hnhitsmtd_time[i]	= new TH1D(buf,buf,nsec,itime_min_use-30,itime_max_use+30);
		}
	}

	TTree *tp;
	Int_t	vpd_cco[38];
	Float_t	vpd_tle[38];
	Float_t	vpd_tte[38];
	Float_t	vpd_tot[38];
	Float_t vpd_tac[38];
	Float_t vpd_adc[38];
	Float_t vpd_tachi[38];
	Float_t vpd_adchi[38];
	Int_t	p2p_cco[16];
	Float_t	p2p_tle[16];
	Float_t	p2p_tte[16];
	Float_t	p2p_tot[16];
	Float_t p2p_tac[16];
	Float_t p2p_adc[16];
	Float_t bbc_tac[48];
	Float_t bbc_adc[48];
	Float_t zdc_tac[6];
	Float_t zdc_adc[6];
	Float_t mtd_tac[4];
	Float_t mtd_adc[4];
	if (!SkipTrgDetTree){
		tp = new TTree("tp","VPD,BBC,ZDC,P2P,MTD tree");
		Int_t		kseqchain;
		Int_t		p2p_sin[32];		// 4*Sequence + Chain
		Float_t	tmptot;
		tp->Branch("time",&evttime,"time/i");
		tp->Branch("run",&runnum,"run/I");
		tp->Branch("day",&daynum,"day/I");
		tp->Branch("evp", &evp_data,"evtnum/i:token:trigcmd:daqcmd:trgword:phyword:daqbits:daqbitsl1:daqbitsl2:evttime:evtseq");
		tp->Branch("vpd_cco"  ,&vpd_cco[0],  "vpd_cco[38]/I");
		tp->Branch("vpd_tle"  ,&vpd_tle[0],  "vpd_tle[38]/F");
		tp->Branch("vpd_tot"  ,&vpd_tot[0],  "vpd_tot[38]/F");
		tp->Branch("vpd_tac"  ,&vpd_tac[0],  "vpd_tac[38]/F");
		tp->Branch("vpd_adc"  ,&vpd_adc[0],  "vpd_adc[38]/F");
		tp->Branch("vpd_tachi",&vpd_tachi[0],"vpd_tachi[38]/F");
		tp->Branch("vpd_adchi",&vpd_adchi[0],"vpd_adchi[38]/F");
		tp->Branch("p2p_cco"  ,&p2p_cco[0],  "p2p_cco[16]/I");
		tp->Branch("p2p_tle"  ,&p2p_tle[0],  "p2p_tle[16]/F");
		tp->Branch("p2p_tot"  ,&p2p_tot[0],  "p2p_tot[16]/F");
		tp->Branch("p2p_tac"  ,&p2p_tac[0],  "p2p_tac[16]/F");
		tp->Branch("p2p_adc"  ,&p2p_adc[0],  "p2p_adc[16]/F");
		tp->Branch("bbc_tac"  ,&bbc_tac[0],  "bbc_tac[48]/F");
		tp->Branch("bbc_adc"  ,&bbc_adc[0],  "bbc_adc[48]/F");
		tp->Branch("zdc_tac"  ,&zdc_tac[0],  "zdc_tac[6]/F");
		tp->Branch("zdc_adc"  ,&zdc_adc[0],  "zdc_adc[6]/F");
		tp->Branch("mtd_tac"  ,&mtd_tac[0],  "mtd_tac[4]/F");
		tp->Branch("mtd_adc"  ,&mtd_adc[0],  "mtd_adc[4]/F");
		//
		//tp->Branch("p2p_sin"  ,&p2p_sin[0],  "p2p_sin[32]/I");	// k = 4*seq + chain
	}

	int nevt_wTofHit = 0;
	int nevt_wMtdHit = 0;
	int nhitsevt_tofintime[122] = {0};
	int nhitsevt_tofintimetight[122] = {0};
	int nhits_crossing[48] = {0};
	int nhits_crossing_vpd[48] = {0};
	int nhits_crossing_trays[48] = {0};
	int nhits_prec_bytrig[64] = {0};
	int nhits_tric_bytrig[64] = {0};
	int nhits_posc_bytrig[64] = {0};
	int nevts_prec_bytrig[64] = {0};
	int nevts_tric_bytrig[64] = {0};
	int nevts_posc_bytrig[64] = {0};
	int nhitstof_xingall	= 0;
	int nhitstof_xingtrg	= 0;
	int nhitstof_xingm2		= 0;
	int nhitstof_xingp2		= 0;
	int nhitstof_xingm7		= 0;
	int nhitstof_xingp7		= 0;
	int nhitstof_xingtrg_vpd	= 0;
	int nhitstof_xingm2_vpd		= 0;
	int nhitstof_xingp2_vpd		= 0;
	int nhitstof_xingm7_vpd		= 0;
	int nhitstof_xingp7_vpd		= 0;

	TTree *ts;
	int 	nTofHits=0;
	int 	nTofHitsLE=0;
	int 	nTofHitsTE=0;
	int		*tray = new int[10000];
	int		*module = new int[10000];
	int		*cell = new int[10000];
	float	*coco = new float[10000];
	float	*bunchid = new float[10000];
	float	*trgdtime = new float[10000];
	float	*tle = new float[10000];
	float	*tte = new float[10000];
	float	*tot = new float[10000];
	if (!SkipTofStopTree){
		ts = new TTree("ts","TOF stop-side tree");
		//ts->SetMaxTreeSize(100000000000LL);
		ts->Branch("time",&evttime,"time/i");
		ts->Branch("day",&daynum,"day/I");
		ts->Branch("run",&runnum,"run/I");
		ts->Branch("evp", &evp_data,"evtnum/i:token:trigcmd:daqcmd:trgword:phyword:daqbits:daqbitsl1:daqbitsl2:evttime:evtseq");
		ts->Branch("nTofHits"   ,&nTofHits  ,"nTofHits/I");
		ts->Branch("nTofHitsLE" ,&nTofHitsLE,"nTofHitsLE/I");
		ts->Branch("nTofHitsTE" ,&nTofHitsTE,"nTofHitsTE/I");
		ts->Branch("tray"       ,tray       ,"tray[nTofHits]/I");
		ts->Branch("module"     ,module     ,"module[nTofHits]/I");
		ts->Branch("cell"       ,cell       ,"cell[nTofHits]/I");
		ts->Branch("coco"       ,coco       ,"coco[nTofHits]/F");
		ts->Branch("bunchid"    ,bunchid    ,"bunchid[nTofHits]/F");
		ts->Branch("trgdtime"   ,trgdtime   ,"trgdtime[nTofHits]/F");
		ts->Branch("tle"        ,tle        ,"tle[nTofHits]/F");
		ts->Branch("tte"        ,tte        ,"tte[nTofHits]/F");
		ts->Branch("tot"        ,tot        ,"tot[nTofHits]/F");
	}

	TTree *tm;
	int 	nMtdHits=0;
	int 	nMtdHitsLE=0;
	int 	nMtdHitsTE=0;
	const int nnnn = 1000;
	int		mtray[nnnn];
	int		mmodch[nnnn];
	int		mmodule[nnnn];
	int		mcell[nnnn];
	int		mstrip[nnnn];
	int		mstrlr[nnnn];
	float	mcoco[nnnn];
	float	mtrgdtime[nnnn];
	float	mtle[nnnn];
	float	mtte[nnnn];
	float	mtot[nnnn];
	if (!SkipMtdStopTree){
		tm = new TTree("tm","MTD stop-side tree");
		//tm->SetMaxTreeSize(100000000000LL);
		tm->Branch("time",&evttime,"time/i");
		tm->Branch("day",&daynum,"day/I");
		tm->Branch("run",&runnum,"run/I");
		tm->Branch("evp", &evp_data,"evtnum/i:token:trigcmd:daqcmd:trgword:phyword:daqbitm:daqbitml1:daqbitml2:evttime:evtmeq");
		tm->Branch("nMtdHits"   ,&nMtdHits    ,"nMtdHits/I");
		tm->Branch("nMtdHitsLE" ,&nMtdHitsLE  ,"nMtdHitsLE/I");
		tm->Branch("nMtdHitsTE" ,&nMtdHitsTE  ,"nMtdHitsTE/I");
		tm->Branch("tray"       ,mtray        ,"tray[nMtdHits]/I");
		tm->Branch("module"     ,mmodule      ,"module[nMtdHits]/I");
		tm->Branch("modch"      ,mmodch       ,"modch[nMtdHits]/I");
		tm->Branch("cell"       ,mcell        ,"cell[nMtdHits]/I");
		tm->Branch("strip"      ,mstrip       ,"strip[nMtdHits]/I");
		tm->Branch("strlr"      ,mstrlr       ,"strlr[nMtdHits]/I");
		tm->Branch("coco"       ,mcoco        ,"coco[nMtdHits]/F");
		tm->Branch("trgdtime"   ,mtrgdtime    ,"trgdtime[nMtdHits]/F");
		tm->Branch("tle"        ,mtle         ,"tle[nMtdHits]/F");
		tm->Branch("tte"        ,mtte         ,"tte[nMtdHits]/F");
		tm->Branch("tot"        ,mtot         ,"tot[nMtdHits]/F");
	}


//-------------------- file loop -----------------------------------------------------------
  cout<<"argc="<<argc<<endl;
  for (int i = 1; i < argc; ++i) {	// file loop

  	++filenum;
  	cout<<i<<" "<<argv[i]<<endl;
    evp = new daqReader(argv[i]);	// created with the filename as an argument..
    //if(mountpoint) evp->setEvpDisk(mountpoint);

	cout<<i<<"\t "<<argv[i]<<endl;
	if (strstr(evp->file_name,"st_monitor")!=0){
		sscanf(evp->file_name,"/star/scratch/geurts/st_monitor_%d_raw",&runnum);
	} else if (strstr(evp->file_name,"st_physics")!=0){
		if (strstr(evp->file_name,"/star/data03")!=0){
			sscanf(evp->file_name,"/star/data03/daq/geurts/%d/st_physics_",&runnum);
		} else if (strstr(evp->file_name,"/gpfs01/star/subsysg/TOF/BTOF-online")!=0){
	 		sscanf(evp->file_name,"/gpfs01/star/subsysg/TOF/BTOF-online/daq/%d/st_physics_",&runnum);
		}
	} else if (strstr(evp->file_name,"st_tofcosmic")!=0){
		if (strstr(evp->file_name,"/star/data03")!=0){
			sscanf(evp->file_name,"/star/data03/daq/geurts/%d/st_",&runnum);
		} else if (strstr(evp->file_name,"/gpfs01/star/subsysg/TOF/BTOF-online")!=0){
	 		sscanf(evp->file_name,"/gpfs01/star/subsysg/TOF/BTOF-online/daq/%d/st_",&runnum);
		}
	} else if (strstr(evp->file_name,"st_ftp_")!=0){
		if (strstr(evp->file_name,"/star/data03")!=0){
			sscanf(evp->file_name,"/star/data03/daq/tof/geurts/%d/st_",&runnum);
		} else if (strstr(evp->file_name,"/gpfs01/star/subsysg/TOF/BTOF-online")!=0){
	 		sscanf(evp->file_name,"/gpfs01/star/subsysg/TOF/BTOF-online/daq/%d/st_",&runnum);
		}
	} else if (strstr(evp->file_name,"st_epd_")!=0){
		if (strstr(evp->file_name,"/star/data03")!=0){
			sscanf(evp->file_name,"/star/data03/daq/tof/geurts/%d/st_",&runnum);
		} else if (strstr(evp->file_name,"/gpfs01/star/subsysg/TOF/BTOF-online")!=0){
	 		sscanf(evp->file_name,"/gpfs01/star/subsysg/TOF/BTOF-online/daq/%d/st_",&runnum);
		}
	} else if (strstr(evp->file_name,"st_ht_")!=0){
		if (strstr(evp->file_name,"/star/data03")!=0){
			sscanf(evp->file_name,"/star/data03/daq/tof/geurts/%d/st_",&runnum);
		} else if (strstr(evp->file_name,"/gpfs01/star/subsysg/TOF/BTOF-online")!=0){
	 		sscanf(evp->file_name,"/gpfs01/star/subsysg/TOF/BTOF-online/daq/%d/st_",&runnum);
		}
	} else if (strstr(evp->file_name,"st_zerobias_")!=0){
		if (strstr(evp->file_name,"/star/data03")!=0){
			sscanf(evp->file_name,"/star/data03/daq/tof/geurts/%d/st_",&runnum);
		} else if (strstr(evp->file_name,"/gpfs01/star/subsysg/TOF/BTOF-online")!=0){
	 		sscanf(evp->file_name,"/gpfs01/star/subsysg/TOF/BTOF-online/daq/%d/st_",&runnum);
		}
	} else if (strstr(evp->file_name,"st_upc_")!=0){
		if (strstr(evp->file_name,"/star/data03")!=0){
			sscanf(evp->file_name,"/star/data03/daq/geurts/%d/st_",&runnum);
		} else if (strstr(evp->file_name,"/gpfs01/star/subsysg/TOF/BTOF-online")!=0){
	 		sscanf(evp->file_name,"/gpfs01/star/subsysg/TOF/BTOF-online/daq/%d/st_",&runnum);
		}
	} else if (strstr(evp->file_name,"st_mtd_")!=0){
		if (strstr(evp->file_name,"/star/data03")!=0){
			sscanf(evp->file_name,"/star/data03/daq/geurts/%d/st_",&runnum);
		} else if (strstr(evp->file_name,"/gpfs01/star/subsysg/TOF/BTOF-online")!=0){
	 		sscanf(evp->file_name,"/gpfs01/star/subsysg/TOF/BTOF-online/daq/%d/st_",&runnum);
		}
	} else if (strstr(evp->file_name,"st_hlt_")!=0){
		if (strstr(evp->file_name,"/star/data03")!=0){
			sscanf(evp->file_name,"/star/data03/daq/tof/geurts/%d/st_",&runnum);
		} else if (strstr(evp->file_name,"/gpfs01/star/subsysg/TOF/BTOF-online")!=0){
	 		sscanf(evp->file_name,"/gpfs01/star/subsysg/TOF/BTOF-online/daq/%d/st_",&runnum);
		}
	} else if (strstr(evp->file_name,"st_gamma_")!=0){
		if (strstr(evp->file_name,"/star/data03")!=0){
			sscanf(evp->file_name,"/star/data03/daq/tof/geurts/%d/st_",&runnum);
		} else if (strstr(evp->file_name,"/gpfs01/star/subsysg/TOF/BTOF-online")!=0){
	 		sscanf(evp->file_name,"/gpfs01/star/subsysg/TOF/BTOF-online/daq/%d/st_",&runnum);
		}
	} else {
		if (strstr(evp->file_name,"/star/data03")!=0){
			sscanf(evp->file_name,"/star/data03/daq/geurts/%d/",&runnum);
		} else if (strstr(evp->file_name,"/gpfs01/star/subsysg/TOF/BTOF-online")!=0){
	 		sscanf(evp->file_name,"/gpfs01/star/subsysg/TOF/BTOF-online/daq/%d/",&runnum);
		}
	}

	int kyear = runnum/1000000;
	daynum	= (runnum - kyear*1000000)/1000;
	int krunonday = (runnum - kyear*1000000 - daynum*1000);
	cout<<"fileno="<<filenum<<"  Year="<<kyear<<"  Day="<<daynum<<"  RunOnDay="<<krunonday<<"  Run="<<runnum;
	//if (daynum==95)cout<<" ..... SWAPPING MTD TRAYS !!!";
	cout<<endl;

	int	icount=0;

    //---- event loop..........
    for(;;) {

//cout<<"main ----------------------------------------------------------"<<endl;
    	LOG(INFO, "event=%d", icount);
		icount++;
		int	ninomult[24][122] = {0};
		int	ninomulttight[24][122] = {0};
		nhitstof_tof	= 0;
		nhitstof_trg	= 0;

		char *ret = evp->get(0, EVP_TYPE_ANY);		// Get an event
		if(ret) {
			if(evp->status) {
				LOG(ERR, "evp status is non-null [0x%8X, %d dec]", evp->status, evp->status);
				continue;
			}
		} else {    // something other than an event, check what.
			switch(evp->status) {
				case EVP_STAT_OK:
					continue;
				case EVP_STAT_EOR:
					LOG(DBG, "End of Run/File");
					if(evp->IsEvp()) {  			 // but event pool, keep trying...
						LOG(DBG, "Wait a second...");
						sleep(1);
						continue;
					}
					break;    					    // file, we're done...
				case EVP_STAT_EVT:
					LOG(WARN, "Problem getting event - skipping");
					sleep(1);
					continue;
				case EVP_STAT_CRIT:
					LOG(CRIT,"evp->status CRITICAL (?)") ;
					return -1;
			}
		}
		if(evp->status == EVP_STAT_EOR) {
			//LOG(INFO,"End of File reached...") ;
			break ;	// of the for(;;) loop...
		}

		++ievtnum;

		//---- exclude events based on unix time.......
		//
		unsigned int thistime	= evp->evt_time;
		if (itime_exclude_min>0 && itime_exclude_max>itime_exclude_min){
			if (thistime>=itime_exclude_min && thistime<=itime_exclude_max){
				cout<<"skip event .... "<<ievtnum<<" "<<thistime<<endl;
				++nskipped;
				continue;			// skip event......
			}
		}

//TESTTESTTEST
		if (NEVTMAX>0){
			if (ievtnum>NEVTMAX){ break; }
		}
//		if (icount<5291){ continue; }
//TESTTESTTEST
		//	cout<<evp->event_number<<" "<<evp->evt_time<<endl;

		evp_data.event_number	= evp->event_number;
		evp_data.token 			= evp->token;
		evp_data.trgcmd 		= evp->trgcmd;
		evp_data.daqcmd 		= evp->daqcmd;
		evp_data.trgword 		= evp->trgword;
		evp_data.phyword 		= evp->phyword;
		evp_data.daqbits 		= evp->daqbits;
		evp_data.daqbits_l1 	= evp->daqbits_l1;
		evp_data.daqbits_l2 	= evp->daqbits_l2;
		evp_data.daqbits64 		= evp->daqbits64;
		evp_data.evt_time 		= evp->evt_time;
		evp_data.seq 			= evp->seq;
		daqbits					= evp->daqbits;
		//
		evttime					= evp->evt_time;
		if (evttime_start>0){
			evttime			   -= evttime_start;
		}

		//cout<<evp->trgcmd<<" "<<evp->daqcmd<<" "<<evp->trgword<<" "
		//	<<evp->phyword<<" "<<evp->daqbits<<" "<<evp->daqbits64<<" "
		//	<<endl;

		//---- trigger selection.....
		//
		unsigned long long idata	= evp->daqbits64;
		//cout<<idata<<"\t";
		int  trgbits[64];
		bool mb         = false;
		bool zdc        = false;
		bool vpd        = false;
		bool bbcsmall   = false;
		bool bbclarge   = false;
		bool bbcmbtof0  = false;
		bool tof200fast = false;
		bool hlttracks  = false;
		bool tofmult4	= false;
		//
		for (int ibit=63;ibit>=0;ibit--){
			if ((idata>>ibit) & 1){
				//cout<<1;
				trgbits[ibit]	= 1;
				++nevt_bytrig[ibit];
				//if (ibit== 0){ mb         = true; }	// mb                 daq 1
				//if (ibit== 4){ hlttracks  = true; }	// hlttracks          daq 10
				//if (ibit== 7){ zdc        = true; }	// zdcmontac          daq 80
				//if (ibit== 9){ vpd        = true; }	// vpdmontac          daq 200
				//if (ibit==11){ bbcsmall   = true; }	// bbcsmallmonnarrow  daq 800
				//if (ibit==14){ bbclarge   = true; }	// bbclargemonnarrow  daq 4000
				//if (ibit==23){ tof200fast = true; }	// tof200fast         daq 800000
				//
				//if (ibit==10){ bbcsmall    = true; }	// bbcmb              daq 400
				//if (ibit==11){ vpd         = true; }	// vpdmb              daq 800
				//if (ibit==13){ zdc         = true; }	// zdcmb-trgonly      daq 2000
				//if (ibit==22){ tofmult4    = true; } 	// tofmult4           daq 400000
				//if (ibit==26){ bbcmbtof0   = true; }	// bbcmbtof0          daq 4000000
				//
			} else {
				trgbits[ibit]	= 0;
				//cout<<0;
			}
		}		//cout<<"\t"<<(int)tof200fast<<endl;
		//if (!tof200fast) continue;
		//if (!mb) continue;
		//if (!vpd) continue;
		//if (!bbcsmall) continue;
		//if (!bbclarge) continue;
		//if (!hlttracks) continue;
		//
		//if (!tofmult4){ continue; }
		//
		//if (!bbcmbtof0){ continue; }
		//
		for (int ibit=63;ibit>=0;ibit--){ htrgbits->Fill(ibit,trgbits[ibit]); }
		++nevt_seen;
		if (zdc)     { ++ntrg_zdc;      }
		if (vpd)     { ++ntrg_vpd;      }
		if (bbcsmall){ ++ntrg_bbcsmall; }

		//---- Get Trigger Detector Data..................................................
		if (!SkipTrgDetTree){
			//istat = trg_doer(evp, &xing, &P2P, &DSM, &VPD, &TRG);
			istat = trg_doer(evp, &xing, &P2P, VPD, TRG);
			if (istat < 0) {
				//cout<<"....No TRG Data.... "<<ievtnum<<endl;
				//continue;
			} else {
				++nev_det[0];
				// int thistofmult = 0;
				// for (int it=0;it<120;it++){
				// 	thistofmult += TRG->tofmult[i];
				// }
				// if (thistofmult>0){ cout<<nevt_seen<<" "<<thistofmult<<endl; }
			}
		}

		//---- Get TOF Detector Data......................................................
		//cout<<"calling tof_doer.... "<<evp<<endl;
		//
		//===========================================================
		//===========================================================
// 		unsigned int words[10000] = {0};
// 		daq_dta *dd = evp->det("tof")->get("legacy");
// 		if (dd){
// 			while (dd->iterate()){
// 				tof_t *tof = (tof_t*)dd->Void;
// 				if (tof){
// 					for (int r=0;r<4;r++) {					// loop over fibers....
// 						int nwords	= tof->ddl_words[r];
// 						for (int i=0;i<nwords;i++){			// loop over words....
// 							words[i]	 = tof->ddl[r][i];
// 						}
// 						istat	= tof_err_check(words,4*nwords,r+1);
// 					}
// 				}
// 			}
// 		}
		//===========================================================
		//===========================================================
		//
		istat = tof_doer(evp, &P2P, TOF, daynum, kDataChoice, STOP, MYINFO);
		if (istat < 0) {
			//cout<<"....No TOF Data.... "<<ievtnum<<endl;
			//continue;
		} else {
			++nev_det[1];
//?!?!?!?			if (nTofHits){++nevt_wTofHit;}
			//cout<<STOP->nTofHitsLE<<" "<<STOP->nTofHitsTE<<" "<<STOP->nTofHits<<endl;
			//
			for (int i=0;i<4;i++){
				hnwordsfiber[i]->Fill(MYINFO->nwordsfiber[i]);
			}
			//
		}
		if (MYINFO->nhits_intime>5&&MYINFO->nhits_early<5){
			for (int i=63;i>=0;i--){ htrgbits_tofintime->Fill(i,trgbits[i]); }
			++nevt_tofintime;
		}
		if (MYINFO->nhits_intime<5&&MYINFO->nhits_early>5){
			for (int i=63;i>=0;i--){ htrgbits_tofearly->Fill(i,trgbits[i]); }
			++nevt_tofearly;
		}

		//---- Get MTD Detector Data......................................................
 		// istat = mtd_doer(evp, daynum, kDataChoice, MTD, MYINFO);
		if (istat < 0) {
			//cout<<"....No MTD Data.... "<<ievtnum<<endl;
			//continue;
		} else {
			++nev_det[2];
			for (int i=0;i<30;i++){
				nMtdHitsBL[i]		 = MYINFO->nrawhits_mtd[i];
				nMtdHitsBL_total[i]	+= MYINFO->nrawhits_mtd[i];
				if (hnhitsmtd_time[0]){
					hnhitsmtd_time[i]->Fill(evp->evt_time,nMtdHitsBL[i]);
				}
			}

//?!?!?!?			if (nMtdHits){++nevt_wMtdHit;}
//			if (MTD->nMtdHits){
//				cout<<MTD->nMtdHitsLE<<" "<<MTD->nMtdHitsTE<<" "<<MTD->nMtdHits<<endl;
//			}
			//
		}

		//---- Get P2P Detector Data......................................................
		// istat = pp2pp_doer(evp, SiRawFlag, &SVX, &Hits, &sili);
		// if (istat < 0) {
		// 	//cout<<"....No P2P Data.... "<<ievtnum<<endl;
		// 	//continue;
		// } else {
		// 	++nev_det[2];
		// }

		//---- Process & Write Data.......................................................

		int  vpdtrg_ne=0,vpdtrg_nw=0;
		bool liveP2Ptof = false;
		bool liveP2Ptrg = false;
		bool liveVPDtof = false;
		bool liveVPDtrg = false;
		bool liveBBCtrg	= false;
		bool liveZDCtrg	= false;
		bool liveMTDtrg	= false;
		int nh_bbcl 			= 0;
		int nh_bbcl_tofintime	= 0;
		int nh_bbcl_tofearly	= 0;
		if (!SkipTrgDetTree){
			//---- TRG information ....................................
			int k;
			for (int i=0;i<16;i++){
				k	= map_vpd_trg2tof_west[i];
				if ((!TakeAllData && VPD->vpd_trgtac[i]>100.0)
				 || ( TakeAllData)) {
				   if (VPD->vpd_trgadc[i] >= 0.0){
						vpd_tac[k]		= VPD->vpd_trgtac[i];
						vpd_adc[k]		= VPD->vpd_trgadc[i];
						vpd_tachi[k]	= VPD->vpd_trgtac_hi[i];
						vpd_adchi[k]	= VPD->vpd_trgadc_hi[i];
						if (vpd_tac[k]>100.0&&vpd_tac[k]<3800.0){ liveVPDtrg = true; vpdtrg_nw++; }
						if (vpd_adc[k]>10){ hp_vpd_trg->Fill(k,1.); }
				  } else {
					vpd_tac[k]		= 0.0;
					vpd_adc[k]		= 0.0;
					vpd_tachi[k]	= 0.0;
					vpd_adchi[k]	= 0.0;
				  }
				} else {
					vpd_tac[k]		= 0.0;
					vpd_adc[k]		= 0.0;
					vpd_tachi[k]	= 0.0;
					vpd_adchi[k]	= 0.0;
				}
				k	= map_vpd_trg2tof_east[i];
				if ((!TakeAllData && VPD->vpd_trgtac[i+16]>100.0)
				 || ( TakeAllData)) {
				   if (VPD->vpd_trgadc[i+16] >= 0.0){
						vpd_tac[k+19]	= VPD->vpd_trgtac[i+16];
						vpd_adc[k+19]	= VPD->vpd_trgadc[i+16];
						vpd_tachi[k+19]	= VPD->vpd_trgtac_hi[i+16];
						vpd_adchi[k+19]	= VPD->vpd_trgadc_hi[i+16];
						if (vpd_tac[k+19]>100.0&&vpd_tac[k+19]<3800.0){ liveVPDtrg = true; vpdtrg_ne++; }
						if (vpd_adc[k+19]>10){ hp_vpd_trg->Fill(k+19,1.); }
				   } else {
					vpd_tac[k+19]	= 0.0;
					vpd_adc[k+19]	= 0.0;
					vpd_tachi[k+19]	= 0.0;
					vpd_adchi[k+19]	= 0.0;
				   }
				} else {
					vpd_tac[k+19]	= 0.0;
					vpd_adc[k+19]	= 0.0;
					vpd_tachi[k+19]	= 0.0;
					vpd_adchi[k+19]	= 0.0;
				}
			}
			//
// 			if (vpdtrg_nw<3||vpdtrg_ne<3){
// 			    for (int i=0;i<16;i++){
// 					if (VPD->vpd_trgtac[i]>200&&VPD->vpd_trgtac[i]<3500){ cout<<"doer_trg ... BBQ WEST trgch="<<i<<" adc="<<VPD->vpd_trgadc[i]<<" tac="<<VPD->vpd_trgtac[i]<<endl; }
// 					if (VPD->vpd_trgtac[i+16]>200&&VPD->vpd_trgtac[i+16]<3500){ cout<<"doer_trg ... BBQ EAST trgch="<<i<<" adc="<<VPD->vpd_trgadc[i+16]<<" tac="<<VPD->vpd_trgtac[i+16]<<endl; }
// 					if (VPD->vpd_trgtac_hi[i]>200&&VPD->vpd_trgtac_hi[i]<3500){ cout<<"doer_trg ... MXQ WEST trgch="<<i<<" adc="<<VPD->vpd_trgadc_hi[i]<<" tac="<<VPD->vpd_trgtac_hi[i]<<endl; }
// 					if (VPD->vpd_trgtac_hi[i+16]>200&&VPD->vpd_trgtac_hi[i+16]<3500){ cout<<"doer_trg ... MXQ EAST trgch="<<i<<" adc="<<VPD->vpd_trgadc_hi[i+16]<<" tac="<<VPD->vpd_trgtac_hi[i+16]<<endl; }
// 				}
// 			}
			//
			//if (vpdtrg_nw||vpdtrg_ne){ cout<<"VPD TRG N = "<<vpdtrg_nw<<" "<<vpdtrg_ne<<endl;}
			p2p_tac[0]			= P2P.RPWHI1_TAC;
			p2p_tac[1]			= P2P.RPWHI2_TAC;
			p2p_tac[2]			= P2P.RPWHO1_TAC;
			p2p_tac[3]			= P2P.RPWHO2_TAC;
			p2p_tac[4]			= P2P.RPWVU1_TAC;
			p2p_tac[5]			= P2P.RPWVU2_TAC;
			p2p_tac[6]			= P2P.RPWVD1_TAC;
			p2p_tac[7]			= P2P.RPWVD2_TAC;
			p2p_tac[0+8]		= P2P.RPEHO1_TAC;	// order!
			p2p_tac[1+8]		= P2P.RPEHO2_TAC;	// order!
			p2p_tac[2+8]		= P2P.RPEHI1_TAC;	// order!
			p2p_tac[3+8]		= P2P.RPEHI2_TAC;	// order!
			p2p_tac[4+8]		= P2P.RPEVU1_TAC;
			p2p_tac[5+8]		= P2P.RPEVU2_TAC;
			p2p_tac[6+8]		= P2P.RPEVD1_TAC;
			p2p_tac[7+8]		= P2P.RPEVD2_TAC;
			p2p_adc[0]			= P2P.RPWHI1_ADC;
			p2p_adc[1]			= P2P.RPWHI2_ADC;
			p2p_adc[2]			= P2P.RPWHO1_ADC;
			p2p_adc[3]			= P2P.RPWHO2_ADC;
			p2p_adc[4]			= P2P.RPWVU1_ADC;
			p2p_adc[5]			= P2P.RPWVU2_ADC;
			p2p_adc[6]			= P2P.RPWVD1_ADC;
			p2p_adc[7]			= P2P.RPWVD2_ADC;
			p2p_adc[0+8]		= P2P.RPEHO1_ADC;	// order!
			p2p_adc[1+8]		= P2P.RPEHO2_ADC;	// order!
			p2p_adc[2+8]		= P2P.RPEHI1_ADC;	// order!
			p2p_adc[3+8]		= P2P.RPEHI2_ADC;	// order!
			p2p_adc[4+8]		= P2P.RPEVU1_ADC;
			p2p_adc[5+8]		= P2P.RPEVU2_ADC;
			p2p_adc[6+8]		= P2P.RPEVD1_ADC;
			p2p_adc[7+8]		= P2P.RPEVD2_ADC;
			liveP2Ptrg			= false;
			for (int i=0;i<16;i++){
				if ((!TakeAllData && p2p_tac[i]>100.0)
				 || ( TakeAllData)) {
					hp_p2p_trg->Fill(i,1.);
					if (p2p_tac[i]>100.){
						liveP2Ptrg	= true;
					}
				}
			}
			//
	// 		cout<<" ------------------------------------------------- "<<endl;
	// 		for (int iseq=0;iseq<8;iseq++){
	// 			for (int ichain=0;ichain<4;ichain++){
	// 				kseqchain	= 4*iseq + ichain;
	// 				//ictr		= map_seq_cntr[iseq];
	// 				p2p_sin[kseqchain]	= Hits.N[iseq][ichain];
	// 				if (Hits.N[iseq][ichain]>0){
	// 					for (int ihit=0;ihit<Hits.N[iseq][ichain];ihit++){
	// 							cout<<iseq<<" "<<ichain<<" "
	// 								<<Hits.N[iseq][ichain]<<" "
	// 								<<Hits.X[iseq][ichain][ihit]<<" "
	// 								<<Hits.Q[iseq][ichain][ihit]<<endl;
	// 					}
	// 				}
	// 			}
	// 		}
			//
			//---- BBC information ....................................
//cout<<MYINFO->nhits_intime<<" "<<MYINFO->nhits_early<<endl;
			bool is_bbcl;
			for (int i=0;i<48;i++){
				is_bbcl	= false;
				if (i%24>=16) is_bbcl = true;
				if ((!TakeAllData && TRG->bbc_tac[i]>100.0)
				 || ( TakeAllData)) {
					bbc_tac[i]		= TRG->bbc_tac[i];
					bbc_adc[i]		= TRG->bbc_adc[i];
					if (TRG->bbc_tac[i]>25.0&&TRG->bbc_tac[i]<3000.0){
						liveBBCtrg	= true;
						if (is_bbcl) ++nh_bbcl;
						hp_bbc->Fill(i);
						if (MYINFO->nhits_intime>5&&MYINFO->nhits_early<5){
							if (is_bbcl) ++nh_bbcl_tofintime;
							hp_bbc_tofintime->Fill(i);
						}
						if (MYINFO->nhits_intime<5&&MYINFO->nhits_early>5){
							if (is_bbcl) ++nh_bbcl_tofearly;
							hp_bbc_tofearly->Fill(i);
						}
					}
				} else {
					bbc_tac[i]		= 0.0;
					bbc_adc[i]		= 0.0;
				}
			}
			hnh_bbcl->Fill(nh_bbcl);
			hnh_bbcl_tofintime->Fill(nh_bbcl_tofintime);
			hnh_bbcl_tofearly->Fill(nh_bbcl_tofearly);
			//
			//---- ZDC information ....................................
			for (int i=0;i<6;i++){
				if ((!TakeAllData && TRG->zdc_tac[i]>200.0)
				 || ( TakeAllData)) {
					zdc_tac[i]		= TRG->zdc_tac[i];
					zdc_adc[i]		= TRG->zdc_adc[i];
					if (TRG->zdc_tac[i]>100.0){
						liveZDCtrg	= true;
					}
				} else {
					zdc_tac[i]		= 0.0;
					zdc_adc[i]		= 0.0;
				}
			}
			//
			//---- MTD information ....................................
			for (int i=0;i<1;i++){
				if ((!TakeAllData && TRG->mtd_tac[i]>200.0)
				 || ( TakeAllData)) {
					mtd_tac[i]		= TRG->mtd_tac[i];
					mtd_adc[i]		= TRG->mtd_adc[i];
					if (TRG->mtd_tac[i]>100.0){
						liveMTDtrg	= true;
					}
				} else {
					mtd_tac[i]		= 0.0;
					mtd_adc[i]		= 0.0;
				}
			}
			//
			//---- TOF information ....................................
			for (int i=0;i<38;i++){
				vpd_cco[i] 		= TOF->vpd_cco[i];
				vpd_tle[i] 		= TOF->vpd_tle[i];
				vpd_tte[i] 		= TOF->vpd_tte[i];
				vpd_tot[i] 		= TOF->vpd_tot[i];
				if (vpd_tle[i]>0.){
					liveVPDtof	= true;
					hp_vpd_tof->Fill(i,1.);
				}
			}
			for (int i=0;i<16;i++){
				p2p_cco[i] 		= TOF->p2p_cco[i];
				p2p_tle[i] 		= TOF->p2p_tle[i];
				p2p_tte[i] 		= TOF->p2p_tte[i];
				p2p_tot[i] 		= TOF->p2p_tot[i];
				if (p2p_tle[i]>0.){
					liveP2Ptof	= true;
					hp_p2p_tof->Fill(i,1.);
				}
			}
			//
			//---- Fill Tree ..........................................
			// if (liveVPDtrg || liveVPDtof || liveBBCtrg || liveZDCtrg){
				tp->Fill();
			// }
		}
		//---- end SkipTrgDetTree check....

// 		for (int iseq=0;iseq<8;iseq++){
// 			for (int ichain=0;ichain<4;ichain++){
// 				kseqchain		= 4*iseq + ichain;
// 				if (p2p_sin[kseqchain]>0){
// 					ictr		= map_seq_cntr[iseq];
// 					for (int id=0;id<16;id++){
// 						ilr		= id%2;
// 						idet	= 2*ictr + ilr;
// 						if (p2p_sin[kseqchain]>0 && p2p_tac[id]>100 && p2p_tac[id]<2000 ){
// 							hpsipmt->Fill(id,idet);
// 						}
// 					}
// 				}
// 			}
// 		}
//			if (liveP2Ptrg && p2p_tle[i]>0){
//				cout<<i<<" "<<p2p_cco[i]<<" ";
//			}

		if (!SkipTofStopTree){
			nTofHits		= STOP->nTofHits;
			nTofHitsLE		= STOP->nTofHitsLE;
			nTofHitsTE		= STOP->nTofHitsTE;
			nhitstof_tof	= 0;
			for (int it=0;it<122;it++){
				nhitsevt_tofintime[it] = 0;
				nhitsevt_tofintimetight[it] = 0;
			}
			for (int ic=0;ic<48;ic++){
				nhits_crossing[ic]	= 0;
				nhits_crossing_vpd[ic]	= 0;
				nhits_crossing_trays[ic]	= 0;
			}
			nhitstof_xingall	= 0;
			nhitstof_xingtrg	= 0;
			nhitstof_xingm2		= 0;
			nhitstof_xingp2		= 0;
			nhitstof_xingm7		= 0;
			nhitstof_xingp7		= 0;
			nhitstof_xingtrg_vpd	= 0;
			nhitstof_xingm2_vpd		= 0;
			nhitstof_xingp2_vpd		= 0;
			nhitstof_xingm7_vpd		= 0;
			nhitstof_xingp7_vpd		= 0;
			for (int itr=0;itr<64;itr++){
				nhits_prec_bytrig[itr] = 0;
				nhits_tric_bytrig[itr] = 0;
				nhits_posc_bytrig[itr] = 0;
			}
			for (int ih=0;ih<nTofHits;ih++){
				tray[ih]	= STOP->tray[ih];
				module[ih]	= STOP->module[ih];
				cell[ih]	= STOP->cell[ih];
				coco[ih]	= STOP->coco[ih];
				bunchid[ih]	= STOP->bunchid[ih];
				trgdtime[ih]= STOP->trgdtime[ih];
				tle[ih]		= STOP->tle[ih];
				tte[ih]		= STOP->tte[ih];
				tot[ih]		= STOP->tot[ih];
				//
				int ithub		= GetTHUB(tray[ih]+1);
				int itrayuse	= tray[ih];
				int ch			= module[ih]*6 + cell[ih];
				int inino		= GetNINO((ch/8)%3,(ch/24)%8);
				int iglnino		= inino + 3*(module[ih]/4);
				//
				//cout<<tray[ih]<<" "<<ithub<<"\t"<<trgdtime[ih]<<" "<<tle[ih]<<" "<<tot[ih]<<endl;
//				if (ithub>=0&&ithub<=3){
					//
					float offset	= TrayTrigTimingPeaks[itrayuse];
					float acrossing	= (((trgdtime[ih]-offset)-crossingorigin)/crossingperiod) - 28.0;
					int icrossing,kcrossing;
					if (acrossing>0){ icrossing = acrossing+0.5; } else {icrossing = acrossing-0.5; }
					float dcrossing	= acrossing-(float)icrossing;
					kcrossing		= icrossing + 28;
					//cout<<acrossing<<" "<<icrossing<<" "<<dcrossing<<endl;
					htrgdtime_traycorrected->Fill(trgdtime[ih]-offset);
					htrgdtime_traycorrected_tray->Fill(itrayuse+1,trgdtime[ih]-offset);
					hcrossing_traycorrected->Fill(acrossing);
					//
					hbunchid_tray->Fill(tray[ih]+1,bunchid[ih]);
					//
					if (dcrossing>-0.3&&dcrossing<0.25){
						hcrossingkeep_traycorrected->Fill(acrossing);
						if (kcrossing>=0&&kcrossing<48){
							++nhits_crossing[kcrossing];
							if (itrayuse+1<=120){
								++nhits_crossing_trays[kcrossing];
							} else if (itrayuse+1==121||itrayuse+1==122){
								++nhits_crossing_vpd[kcrossing];
							}
						} else {
							//cout<<trgdtime[ih]-offset<<" "<<acrossing<<" "<<icrossing<<" "<<dcrossing<<" "<<kcrossing<<endl;
						}
					}
					for (int itr=0;itr<64;itr++){
						if (trgbits[itr]){
							htrgdtime_traycorrected_bytrig[itr]->Fill(trgdtime[ih]-offset);
						}
					}
					//
					aliml	= trigwindowLow[ithub];
					alimu	= trigwindowHigh[ithub];
					if (trgdtime[ih]>=aliml&&trgdtime[ih]<=alimu){
						++nhitsevt_tofintime[itrayuse];
						if (iglnino>=0&&iglnino<=23){
							++ninomult[iglnino][itrayuse];
						}else{
							cout<<"unknown NINO... "<<itrayuse<<" "<<ch<<" "
								<<(ch/8)%3<<" "<<(ch/24)%8<<" "<<inino<<" "
								<<module[ih]<<" "<<iglnino<<endl;
						}
					}
					aliml	= TrayTrigTimingPeaks[itrayuse]-25.;
					alimu	= TrayTrigTimingPeaks[itrayuse]+25.;
					if (trgdtime[ih]>=aliml&&trgdtime[ih]<=alimu){
						++nhitstof_tof;								// TOF hit mult with tray based crossing cut....
						++nhitsevt_tofintimetight[itrayuse];
						if (iglnino>=0&&iglnino<=23){
							++ninomulttight[iglnino][itrayuse];
						}else{
							cout<<"unknown NINO... "<<itrayuse<<" "<<ch<<" "
								<<(ch/8)%3<<" "<<(ch/24)%8<<" "<<inino<<" "
								<<module[ih]<<" "<<iglnino<<endl;
						}
					}
//				}
				//
				for (int itr=0;itr<64;itr++){
					if (trgbits[itr]){
						if (ithub==0){
							htrgdtime0_bytrig[itr]->Fill(trgdtime[ih]);
							aliml	= trigwindowLow[ithub];
							alimu	= trigwindowHigh[ithub];
							if (trgdtime[ih]>=aliml&&trgdtime[ih]<=alimu){
								htrgdtime0keep_bytrig[itr]->Fill(trgdtime[ih]);
							}
						}
						htrgdtime_bytrig[ithub][itr]->Fill(trgdtime[ih]);
						aliml	= trigwindowLow[ithub];
						alimu	= trigwindowHigh[ithub];
						if (trgdtime[ih]>=aliml&&trgdtime[ih]<=alimu){
							htrgdtimekeep_bytrig[ithub][itr]->Fill(trgdtime[ih]);
						}
					}
				}
				//
				//if (nh_bbcl>0 && ithub>=0&&ithub<=3){
				//	htrgdtime_special[ithub]->Fill(trgdtime[ih]);
				//	float aliml	= trigwindowLow[ithub];
				//	float alimu	= trigwindowHigh[ithub];
				//	if (trgdtime[ih]>=aliml&&trgdtime[ih]<=alimu){
				//		htrgdtimekeep_special[ithub]->Fill(trgdtime[ih]);
				//	}
				//}
				//
			}
			if (!SkipTofStopTreeFill){
				ts->Fill();
			}
			//
			for (int i=0;i<38;i++){
				vpd_cco[i] 		= TOF->vpd_cco[i];
				vpd_tle[i] 		= TOF->vpd_tle[i];
				vpd_tte[i] 		= TOF->vpd_tte[i];
				vpd_tot[i] 		= TOF->vpd_tot[i];
				if (vpd_tle[i]>0.){
					liveVPDtof	= true;
					hp_vpd_tof->Fill(i,1.);
				}
			}
			for (int i=0;i<16;i++){
				p2p_cco[i] 		= TOF->p2p_cco[i];
				p2p_tle[i] 		= TOF->p2p_tle[i];
				p2p_tte[i] 		= TOF->p2p_tte[i];
				p2p_tot[i] 		= TOF->p2p_tot[i];
				if (p2p_tle[i]>0.){
					liveP2Ptof	= true;
					hp_p2p_tof->Fill(i,1.);
				}
			}
			//
			//---- fill tofmult correlation plots....
			float totmult_tof	= 0;
			float totmult_trg	= 0;
			for (int itr=0;itr<120;itr++){
				int trgmult		 = TRG->tofmult[itr];
				nhitstof_trg	+= trgmult;
				//
				int nnino		= 0;
				int nninotight	= 0;
				for (int in=0;in<24;in++){
					if (ninomult[in][itr]     >0){ ++nnino;      }
					if (ninomulttight[in][itr]>0){ ++nninotight; }
				}
				//
				int tofmult = nhitsevt_tofintime[itr];
				int diff	= tofmult - trgmult;
				if (tofmult>0||trgmult>0){
					hmultcorr_tray[itr]->Fill(tofmult,trgmult);
					hmultcorr_tray_diff[itr]->Fill(diff);
				}
				tofmult = nhitsevt_tofintimetight[itr];
				diff	= tofmult - trgmult;
				if (tofmult>0||trgmult>0){
					hmultcorrtight_tray[itr]->Fill(tofmult,trgmult);
					hmultcorrtight_tray_diff[itr]->Fill(diff);
				}
				//
				tofmult = nnino;
				diff	= tofmult - trgmult;
				if (tofmult>0||trgmult>0){
					totmult_tof	+= tofmult;
					totmult_trg	+= trgmult;
					hninocorr_tray[itr]->Fill(tofmult,trgmult);
					hninocorr_tray_diff[itr]->Fill(diff);
					if (tofmult>0&&trgmult>0){
						hninocorr_tray_diffnoax[itr]->Fill(diff);
					}
				}
				tofmult = nninotight;
				diff	= tofmult - trgmult;
				if (tofmult>0||trgmult>0){
					hninocorrtight_tray[itr]->Fill(tofmult,trgmult);
					hninocorrtight_tray_diff[itr]->Fill(diff);
					if (tofmult>0&&trgmult>0){
						hninocorrtight_tray_diffnoax[itr]->Fill(diff);
					}
				}
			}
			htraytotmult_tof->Fill(totmult_tof);
			htraytotmult_trg->Fill(totmult_trg);
			htraytotmult_trgvstof->Fill(totmult_tof,totmult_trg);
			if (totmult_tof>0&&totmult_trg>0){
				htraytotmult_tofminustrg->Fill(totmult_tof-totmult_trg);
			}
			//cout<<totmult_tof<<" "<<totmult_trg<<endl;
			//
			for (int ic=0;ic<48;ic++){
				if (nhits_crossing[ic]>0){
					//cout<<ic<<" "<<nhits_crossing[ic]<<endl;
					hntofhits_crossing[ic]->Fill(nhits_crossing[ic]);
					for (int itr=0;itr<64;itr++){
						if (trgbits[itr]){
							if (ic <28){ nhits_prec_bytrig[itr] += nhits_crossing[ic]; }
							if (ic==28){ nhits_tric_bytrig[itr] += nhits_crossing[ic]; }
							if (ic >28){ nhits_posc_bytrig[itr] += nhits_crossing[ic]; }
						}
					}
					                     nhitstof_xingall += nhits_crossing[ic];
				}
				if (nhits_crossing_trays[ic]>0){
					if (ic==28        ){ nhitstof_xingtrg += nhits_crossing_trays[ic]; }
					if (ic>=26&&ic<=27){ nhitstof_xingm2  += nhits_crossing_trays[ic]; }
					if (ic>=21&&ic<=27){ nhitstof_xingp2  += nhits_crossing_trays[ic]; }
					if (ic>=29&&ic<=30){ nhitstof_xingm7  += nhits_crossing_trays[ic]; }
					if (ic>=29&&ic<=35){ nhitstof_xingp7  += nhits_crossing_trays[ic]; }
				}
				if (nhits_crossing_vpd[ic]>0){
					if (ic==28        ){ nhitstof_xingtrg_vpd += nhits_crossing_vpd[ic]; }
					if (ic>=26&&ic<=27){ nhitstof_xingm2_vpd  += nhits_crossing_vpd[ic]; }
					if (ic>=21&&ic<=27){ nhitstof_xingp2_vpd  += nhits_crossing_vpd[ic]; }
					if (ic>=29&&ic<=30){ nhitstof_xingm7_vpd  += nhits_crossing_vpd[ic]; }
					if (ic>=29&&ic<=35){ nhitstof_xingp7_vpd  += nhits_crossing_vpd[ic]; }
				}
			}
		}
		//---- end SkipTofStopTree check...

		hnhitstof_xingall->Fill(nhitstof_xingall);
		hnhitstof_xingtrg->Fill(nhitstof_xingtrg);
		hnhitstof_xingm2->Fill(nhitstof_xingm2);
		hnhitstof_xingp2->Fill(nhitstof_xingp2);
		hnhitstof_xingm7->Fill(nhitstof_xingm7);
		hnhitstof_xingp7->Fill(nhitstof_xingp7);
		hnhitstof_xingtrg_vpd->Fill(nhitstof_xingtrg_vpd);
		hnhitstof_xingm2_vpd->Fill(nhitstof_xingm2_vpd);
		hnhitstof_xingp2_vpd->Fill(nhitstof_xingp2_vpd);
		hnhitstof_xingm7_vpd->Fill(nhitstof_xingm7_vpd);
		hnhitstof_xingp7_vpd->Fill(nhitstof_xingp7_vpd);

 		if (!SkipMtdStopTree){
 			nMtdHits		= MTD->nMtdHits;
 			nMtdHitsLE		= MTD->nMtdHitsLE;
 			nMtdHitsTE		= MTD->nMtdHitsTE;
 			int kIndex	= 0;
			for (int ih=0;ih<nMtdHits;ih++){
				mtray[ih]	= MTD->tray[ih];
				mmodule[ih]	= MTD->module[ih];
				mmodch[ih]	= MTD->modch[ih];
				mcell[ih]	= MTD->cell[ih];
				mstrip[ih]	= MTD->strip[ih];
				mstrlr[ih]	= MTD->strlr[ih];
				mcoco[ih]	= MTD->coco[ih];
				mtrgdtime[ih]= MTD->trgdtime[ih];
				mtle[ih]	= MTD->tle[ih];
				mtte[ih]	= MTD->tte[ih];
				mtot[ih]	= MTD->tot[ih];
				//
				kIndex		= MTD->tray[ih]-1;
				int iRDO	= 1;
				if (MTD->tray[ih]>=1 && MTD->tray[ih]<=15){		//!!!!!!!!!!!!!!! MTD tray to RDO mapping !!!!!!!!!!!!!!
					iRDO	= 2;
				}
				//
				float dtime	= mtrgdtime[ih];
				htrgdtime_mtd[iRDO-1]->Fill(dtime);
				htrgdtime_mtd_tray[kIndex]->Fill(dtime);
				//cout<<MTD->tray[ih]<<" "<<MTD->module[ih]<<" "
				//	<<MTD->modch[ih]<<" "<<MTD->cell[ih]<<" "
				//	<<MTD->strip[ih]<<endl;
				float aliml	= trigwindowLow_mtd[iRDO-1];
				float alimu	= trigwindowHigh_mtd[iRDO-1];
				if (dtime>=aliml&&dtime<=alimu){
					htrgdtimekeep_mtd[iRDO-1]->Fill(dtime);
					htrgdtimekeep_mtd_tray[kIndex]->Fill(dtime);
				}
			}
			if (!SkipMtdStopTreeFill){
				tm->Fill();
			}
		}
		//---- end SkipMtdStopTree check...

		if (thistime < 0)        { itime_min = thistime; }
		if (thistime < itime_min){ itime_min = thistime; }
		if (thistime > itime_max){ itime_max = thistime; }
		if (ievtnum%1000==0){
			cout<<"Processed "<<ievtnum<<" ("<<NEVTMAX<<")"
				<<"\t time="<<evp->evt_time
				<<"\t Ntof="<<nTofHits
				<<"\t Nmtd=";
				for (int ibl=0;ibl<30;ibl++){ cout<<nMtdHitsBL[ibl]; }
				cout<<endl;
		}

		for (int itr=0;itr<64;itr++){
			hnhits_prec_bytrig[itr]->Fill(nhits_prec_bytrig[itr]);
			hnhits_tric_bytrig[itr]->Fill(nhits_tric_bytrig[itr]);
			hnhits_posc_bytrig[itr]->Fill(nhits_posc_bytrig[itr]);
			if (nhits_prec_bytrig[itr]>100){ ++nevts_prec_bytrig[itr]; }
			if (nhits_tric_bytrig[itr]>100){ ++nevts_tric_bytrig[itr]; }
			if (nhits_posc_bytrig[itr]>100){ ++nevts_posc_bytrig[itr]; }
		}

		//cout<<nhitstof_tof<<" "<<nhitstof_trg<<endl;
		hnhitstof_tof->Fill(nhitstof_tof);
		hnhitstof_trg->Fill(nhitstof_trg);
		hnhitstof_trgtof->Fill(nhitstof_tof,nhitstof_trg);
		hnhitstof_tof_low->Fill(nhitstof_tof);
		hnhitstof_trg_low->Fill(nhitstof_trg);
		hnhitstof_trgtof_low->Fill(nhitstof_tof,nhitstof_trg);

		if (hnhitstof_time){
			hnhitstof_time->Fill(evp->evt_time,nhitstof_tof);
		}

    }	//---- end event in file loop....
    delete evp;

  }		//---- end file loop....

  	//---- Finish
	cout<<" No. evts seen    = "<<ievtnum<<endl;
	cout<<" No. evts skipped = "<<nskipped<<endl;
	cout<<" No. evts w/ TRG  = "<<nev_det[0]<<endl;
	cout<<" No. evts w/ TOF  = "<<nev_det[1]<<endl;
	cout<<" No. evts w/ MTD  = "<<nev_det[2]<<endl;
//	cout<<endl;
//	cout<<"  No. evts w/ TofHit = "<<nevt_wTofHit<<endl;
//	cout<<endl;
	cout<<"  time min/max    = "<<itime_min<<" "<<itime_max<<endl;
	//
	cout<<" nevt_seen        = "<<nevt_seen<<endl;
	cout<<" nevt_tofintime   = "<<nevt_tofintime<<"\t"<<(float)nevt_tofintime/(float)nevt_seen<<endl;
	cout<<" nevt_tofearly    = "<<nevt_tofearly<<"\t"<<(float)nevt_tofearly/(float)nevt_seen<<endl;
	//
	cout<<" nevt_seen        = "<<nevt_seen<<endl;
	cout<<" ntrg_zdc         = "<<ntrg_zdc<<"\t"<<(float)ntrg_zdc/(float)nevt_seen<<endl;
	cout<<" ntrg_vpd         = "<<ntrg_vpd<<"\t"<<(float)ntrg_vpd/(float)nevt_seen<<endl;
	cout<<" ntrg_bbcsmall    = "<<ntrg_bbcsmall<<"\t"<<(float)ntrg_bbcsmall/(float)nevt_seen<<endl;
	//
	for (int itr=0;itr<64;itr++){
		if (nevts_tric_bytrig[itr]){
			cout<<itr<<"\t"<<nevts_prec_bytrig[itr]<<"\t"<<nevts_tric_bytrig[itr]<<"\t"<<nevts_posc_bytrig[itr]<<endl;
		}
	}
	//
//	cout<<" Nrawhits MTD26   = "<<nMtdHits26_total<<endl;
//	cout<<" Nrawhits MTD27   = "<<nMtdHits27_total<<endl;
//	cout<<" Nrawhits MTD28   = "<<nMtdHits28_total<<endl;
	//
	if(!SkipTofStopTree){
		f = ts->GetCurrentFile();
	}
    f->Write();
    f->Close();


    delete[] tray;
	delete[] module;
	delete[] cell;
	delete[] coco;
	delete[] bunchid;
	delete[] trgdtime;
	delete[] tle;
	delete[] tte;
	delete[] tot;


	delete VPD;
	delete TRG;
	delete TOF;
	delete STOP;
	delete MTD;
	delete MYINFO;

    //delete f;
    //delete evp;
    return 0;
}
