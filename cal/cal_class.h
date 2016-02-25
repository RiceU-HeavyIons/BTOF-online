#include <iostream>
#include <bitset>

using namespace std;

#include <TROOT.h>
#include <TSystem.h>
#include <TFile.h>
#include <TChain.h>
#include <TStyle.h>
#include <TDirectory.h>
#include <TTree.h>
#include <TPostScript.h>
#include <TCanvas.h>
#include <TLatex.h>
#include <TH1.h>
#include <TH2.h>
#include <TH3.h>
#include <TF1.h>
#include <TProfile.h>
#include <TProfile2D.h>
#include <TMultiGraph.h>
#include <TMath.h>
#include <TBits.h>

#include "/star/u/geurts/lit/SplineFit/TSplineFit.h"

const float TACtoNS_ch_BBQ[38] = {
			0.0178478,0.0178836,0.0179766,0.0177158,0.0177167,0.01773,
			0.0173541,0.0178161,0.0180473,0.0173594,0.0177976,0.0178202,
			0.0175173,0.01773,0.01773,0.0176563,0.0176634,0.0175941,
			0.0177309,0.0176309,0.0176897,0.0178395,0.0178902,0.0175098,
			0.01773,0.0177172,0.0178123,0.0175419,0.01774,0.0179449,
			0.0177135,0.0178428,0.01773,0.01773,0.0177906,0.0174997,
			0.0176159,0.0178622};

class cal {
public :

	TTree          *fChain;   //!pointer to the analyzed TTree or TChain
	Int_t           fCurrent; //!current Tree number in a TChain
	
	cal(Int_t irhicyear, Int_t iinput, Int_t ichoi, Int_t ialgo, Int_t iener, int itrgcr, TTree *tree=0);
	virtual ~cal();	
	virtual Int_t    Cut(Long64_t entry);
	virtual Int_t    GetEntry(Long64_t entry);
	virtual Long64_t LoadTree(Long64_t entry);
	virtual void     Init(TTree *tree);
	virtual Bool_t   Notify();
	virtual void     Show(Long64_t entry = -1);
	//
	virtual void 	 Setup();
	virtual void     Loop(Int_t);
	virtual Bool_t   AdcTotCut(Float_t tot, Float_t adc);

	char	outfilebase[70];	//
	Float_t	refmultmax;
	Int_t 	kChanKeyOffsets;
	Long64_t nentriestree;

	Int_t           day;
	Int_t           run;
	Float_t         vpd_adc[38];
	Float_t         vpd_tac[38];
	Float_t         mvpd_adc[38];
	Float_t         mvpd_tac[38];
//	Float_t         p2p_adc[16];
//	Float_t         p2p_tac[16];
	Float_t         bbc_adc[48];
	Float_t         bbc_tac[48];
	Float_t         vpd_cco[38];
	Float_t         vpd_tle[38];
	Float_t         vpd_tot[38];
//	Float_t         p2p_cco[16];
//	Float_t         p2p_tle[16];
//	Float_t         p2p_tot[16];

	ULong64_t       daqbits;
	UInt_t          itime;
	UInt_t          itimemin;
	Int_t           evtid;
	Int_t           grefmult;
	Int_t           nvert;
	Int_t           nprimary;
	Int_t           nglobal;
	Float_t         vtx[3];
	Float_t         vtxe[3];
	Float_t         fill[2];
	Float_t         field;
	Float_t         zdcrate[2];
	Float_t         bbcrate[2];
	Float_t         bbcbg[2];
	Float_t         zdccoinrate;
	Float_t         bbccoinrate;

	TBranch        *b_day;       //!
	TBranch        *b_run;       //!
	TBranch        *b_daqbits;   //!
	TBranch        *b_vpd_adc;   //!
	TBranch        *b_vpd_tac;   //!
	TBranch        *b_mvpd_adc;   //!
	TBranch        *b_mvpd_tac;   //!
	TBranch        *b_p2p_adc;   //!
	TBranch        *b_p2p_tac;   //!
	TBranch        *b_bbc_adc;   //!
	TBranch        *b_bbc_tac;   //!
	TBranch        *b_vpd_cco;   //!
	TBranch        *b_vpd_tle;   //!
	TBranch        *b_vpd_tot;   //!
	TBranch        *b_p2p_cco;   //!
	TBranch        *b_p2p_tle;   //!
	TBranch        *b_p2p_tot;   //!

	TBranch        *b_itime;   //!
	TBranch        *b_evtid;   //!
	TBranch        *b_grefmult;   //!
	TBranch        *b_nvert;   //!
	TBranch        *b_nprimary;   //!
	TBranch        *b_nglobal;   //!
	TBranch        *b_vtx;   //!
	TBranch        *b_vtxe;   //!
	TBranch        *b_fill;   //!
	TBranch        *b_field;   //!
	TBranch        *b_zdcrate;   //!
	TBranch        *b_bbcrate;   //!
	TBranch        *b_bbcbg;   //!
	TBranch        *b_zdccoinrate;   //!
	TBranch        *b_bbccoinrate;   //!
	
	Int_t		iTrgCrate;
	Int_t		iRhicRun;
	Int_t		iEnergy;
	Int_t		iInputFlag;
	Int_t		iTreeStyle;
	Int_t		iChoice;
	Int_t		iAlgorithm;
	Int_t		iDayMin,iDayMax;
	Int_t		dtimenbins;
	Float_t		*A;
	Float_t		*Aother;
	Float_t		*T;
	Int_t		A_nbin;	
	Float_t		A_min, A_max;
	Float_t		T_min, T_max;
	Int_t		dT_nbin;	
	Float_t		dT_min, dT_max;
	Float_t		dT_min2, dT_max2;
	Float_t		rangeDt_0;
	Float_t		rangeDt_1;

	Float_t 	Zerrorcut;
	Float_t 	Zdiffcut_l; 
	Float_t 	Zdiffcutb_l; 
	Float_t 	Zdiffcutc_l; 
	Float_t 	Zdiffcut_u; 
	Float_t 	Zdiffcutb_u; 
	Float_t 	Zdiffcutc_u; 
	Int_t		Nprimarycut;
	Int_t		refmultcut;
	Float_t		vtx_spot_x;
	Float_t		vtx_spot_y;
	
	Int_t		NDETSIDE;
	Int_t 		NITERATIONS;
	Int_t 		NPASSES;
	Int_t		NREQUIRE;
	Float_t		ADCminimum;
	Float_t		TACtoNS;
	
	TPostScript *psout;
	TFile		*fout;
	TDirectory	*dir_A;
	TDirectory	*dir_fits;	
	TDirectory	*dir_fitslices;
	TDirectory	*dir_res;		
	TDirectory	*dir_zvtx;		
	TDirectory	*dir_day;
	TDirectory	*dir_time;
	TDirectory	*dir_ffun;	
	TDirectory	*dir_1mn;	
	TDirectory	*dir_nvpd;	
	//
	TH1D	*hcoco_vpd_w;
	TH1D	*hcoco_vpd_e;
	TH1D	*hcoco_p2p_w;
	TH1D	*hcoco_p2p_e;
	TH2D	*hinitialoffsets;
	TH1D	*hinitialoffsetvals;
	TH1D	*hinitialoffsetsEW[2];
	//TH2D 	*hnenw;		   
	//TH2D 	*htavCtrRel;  
	//TH2D 	*htavCtrRelsh; 
	//TH2D 	*htavPmtRel;   
	//TH2D 	*htavPmtRelsh; 
	TH2D 	*hAiter[2];
	TH1D	*hnev_day;
	TH1D	*hnev_daqbits;
	//
	TH1D	*hcnts[40];
	//
	TH1D	*hddzvzt_A_all, *hddzvzt_A_keep;
	TH1D	*hddzvzt_B_all, *hddzvzt_B_keep;
	TH1D	*hddzvzt_C_all, *hddzvzt_C_keep;
	TH2D	*hddzvzt_A_evt;
	TH2D	*hddzvzt_B_evt;
	TH2D	*hddzvzt_C_evt;
	TH2D	*hddzvzt_A_day;
	TH2D	*hddzvzt_B_day;
	TH2D	*hddzvzt_C_day;
	TH2D 	*h1mn_n[10][19][2];		// iter, idet, is
	//
	TH1D	*hdtzv_11;
	TH2D	*hdtzv_nn_00;
	TH2D	*hdtzv_nn_11;
	TH2D	*hdtzv_nn_22;
	TH2D	*hdtzv_nn_33;
	TH2D	*hdtzv_nn_00_keep;
	TH2D	*hdtzv_nn_11_keep;
	TH2D	*hdtzv_nn_22_keep;
	TH2D	*hdtzv_nn_33_keep;
	TH2D	*hdtzv_11w;
	TH2D	*hdtzv_11e;
	TH2D	*hnvalidpairs;
	TProfile2D	*hdzd;
	TH3D 		*hdzd3D;
	TH2D 		*hdzd3Dvals;
	TH2D	*hdtzvb_nn_00;
	TH2D	*hdtzvb_nn_11;
	TH2D	*hdtzvb_nn_22;
	TH2D	*hdtzvb_nn_33;
	TH2D	*hdtzvb_nn_00_keep;
	TH2D	*hdtzvb_nn_11_keep;
	TH2D	*hdtzvb_nn_22_keep;
	TH2D	*hdtzvb_nn_33_keep;
	TH2D	*hnvalidpairsb;
	TH2D	*hdtzvc_nn_00;
	TH2D	*hdtzvc_nn_11;
	TH2D	*hdtzvc_nn_22;
	TH2D	*hdtzvc_nn_33;
	TH2D	*hdtzvc_nn_00_keep;
	TH2D	*hdtzvc_nn_11_keep;
	TH2D	*hdtzvc_nn_22_keep;
	TH2D	*hdtzvc_nn_33_keep;
	TH2D	*hnvalidpairsc;
	//
	TH1D	*hdtzv1D_nn_00;
	TH1D	*hdtzv1D_nn_11;
	TH1D	*hdtzv1D_nn_22;
	TH1D	*hdtzv1D_nn_33;
	TH1D	*hdtzv1D_nn_00_keep;
	TH1D	*hdtzv1D_nn_11_keep;
	TH1D	*hdtzv1D_nn_22_keep;
	TH1D	*hdtzv1D_nn_33_keep;
	TH1D	*hdtzvb1D_nn_00;
	TH1D	*hdtzvb1D_nn_11;
	TH1D	*hdtzvb1D_nn_22;
	TH1D	*hdtzvb1D_nn_33;
	TH1D	*hdtzvb1D_nn_00_keep;
	TH1D	*hdtzvb1D_nn_11_keep;
	TH1D	*hdtzvb1D_nn_22_keep;
	TH1D	*hdtzvb1D_nn_33_keep;
	TH1D	*hdtzvc1D_nn_00;
	TH1D	*hdtzvc1D_nn_11;
	TH1D	*hdtzvc1D_nn_22;
	TH1D	*hdtzvc1D_nn_33;
	TH1D	*hdtzvc1D_nn_00_keep;
	TH1D	*hdtzvc1D_nn_11_keep;
	TH1D	*hdtzvc1D_nn_22_keep;
	TH1D	*hdtzvc1D_nn_33_keep;
	//
	TH2D 	*hnvpdrm[10][2];		// passes, is
	//
	TH2D	*adctot[200][2];		// dets
	TH1D	*htot[200][2];			// dets
	TH1D	*hadc[200][2];			// dets
	//
	TSplineFit 	*ff[200][2];			// iterations
	TH1D		*hdt_A_1D[200][2];		// iterations
	TH1D		*hdt_A_fit[200][2];		// iterations
	//
	TH1D 	*hcorrfinal[38][2];		// dets side
	//
	TH2D 	*hdt_A[200][2];				// iterations
	TH1D 	*hdt_A_cons[200][2];		// iterations
	TH1D 	*hdt_A_mean[200][2];		// iterations
	TH1D 	*hdt_A_sigm[200][2];		// iterations
	TH1D 	*hdt_beg[200][2];			// dets
	TH1D 	*hdt_end[200][2];			// dets
	TH2D 	*hdt_beg_n[200][2];			// dets
	TH2D 	*hdt_end_n[200][2];			// dets
	TH1D 	*hdt_end_n_cons[200][2];	// dets
	TH1D 	*hdt_end_n_mean[200][2];	// dets
	TH1D 	*hdt_end_n_sigm[200][2];	// dets
	//
	TH2D 	*hdt_end_n_det0_day[200][2];			// days
	TH2D 	*hdt_end_n_det1_day[200][2];			// days
	TH2D 	*hdt_end_n_det2_day[200][2];			// days
	TH2D 	*hdt_end_n_det3_day[200][2];			// days
	TH1D 	*hdt_end_n_det0_day_cons[200][2];	// days
	TH1D 	*hdt_end_n_det1_day_cons[200][2];	// days
	TH1D 	*hdt_end_n_det2_day_cons[200][2];	// days
	TH1D 	*hdt_end_n_det3_day_cons[200][2];	// days
	TH1D 	*hdt_end_n_det0_day_mean[200][2];	// days
	TH1D 	*hdt_end_n_det1_day_mean[200][2];	// days
	TH1D 	*hdt_end_n_det2_day_mean[200][2];	// days
	TH1D 	*hdt_end_n_det3_day_mean[200][2];	// days
	TH1D 	*hdt_end_n_det0_day_sigm[200][2];	// days
	TH1D 	*hdt_end_n_det1_day_sigm[200][2];	// days
	TH1D 	*hdt_end_n_det2_day_sigm[200][2];	// days
	TH1D 	*hdt_end_n_det3_day_sigm[200][2];	// days
	//
	TH2D 	*hdt_end_n_det0_time[500][2];			// days
	TH2D 	*hdt_end_n_det1_time[500][2];			// days
	TH2D 	*hdt_end_n_det2_time[500][2];			// days
	TH2D 	*hdt_end_n_det3_time[500][2];			// days
	TH1D 	*hdt_end_n_det0_time_cons[500][2];	// days
	TH1D 	*hdt_end_n_det1_time_cons[500][2];	// days
	TH1D 	*hdt_end_n_det2_time_cons[500][2];	// days
	TH1D 	*hdt_end_n_det3_time_cons[500][2];	// days
	TH1D 	*hdt_end_n_det0_time_mean[500][2];	// days
	TH1D 	*hdt_end_n_det1_time_mean[500][2];	// days
	TH1D 	*hdt_end_n_det2_time_mean[500][2];	// days
	TH1D 	*hdt_end_n_det3_time_mean[500][2];	// days
	TH1D 	*hdt_end_n_det0_time_sigm[500][2];	// days
	TH1D 	*hdt_end_n_det1_time_sigm[500][2];	// days
	TH1D 	*hdt_end_n_det2_time_sigm[500][2];	// days
	TH1D 	*hdt_end_n_det3_time_sigm[500][2];	// days
	//
	TH2D 	*hdt_end_det0_zvtx[2];	
	TH1D 	*hdt_end_det0_zvtx_cons[2];	
	TH1D 	*hdt_end_det0_zvtx_mean[2];	
	TH1D 	*hdt_end_det0_zvtx_sigm[2];	
	//
	TH2D 	*hdt_end_det0_grefmult[2];	
	TH1D 	*hdt_end_det0_grefmult_cons[2];	
	TH1D 	*hdt_end_det0_grefmult_mean[2];	
	TH1D 	*hdt_end_det0_grefmult_sigm[2];	
	//
	TH2D	*hdt_end_chan_nLLb;
	TH1D	*hdt_end_chan_nLLb_cons;
	TH1D	*hdt_end_chan_nLLb_mean;
	TH1D	*hdt_end_chan_nLLb_sigm;
	TH2D	*hdt_end_chan_nLLa;
	TH1D	*hdt_end_chan_nLLa_cons;
	TH1D	*hdt_end_chan_nLLa_mean;
	TH1D	*hdt_end_chan_nLLa_sigm;
	//
	TH2D	*htemp1;
	TH2D	*htemp2;
	TH2D	*htemp3;
	TH2D	*hthist_zvtx;
	TH2D	*hzvt_end_zvtx_all;
	TH2D	*hzvt_end_zvtx_cut;
	TH1D	*hdzvtx_11;
	TH2D	*hdzvtx_11_zvtx;
	//
	//TH3D	*hdt_ch5_day[2];			//
	//TH3D	*hdt_ch5_daqbits[2];		//
	//
	Int_t	out_ffun_nb;
	Float_t	out_ffun_x1;
	Float_t	out_ffun_x2;
//	TH1D 	*hadc_iter[200][2];		// iterations;
	TH1D 	*ffun_iter[200][2];		// iterations;
	//
};


