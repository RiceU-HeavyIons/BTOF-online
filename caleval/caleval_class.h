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
#include <TGraph.h>
#include <TMath.h>

//#include "/star/u/geurts/lit/SplineFit/TSplineFit.h"

class caleval {
public :

	TTree          *fChain;   //!pointer to the analyzed TTree or TChain
	Int_t           fCurrent; //!current Tree number in a TChain
	
	caleval(Int_t irhicyear, Int_t iinput, Int_t ichoi, Int_t ialgo, Int_t iener, Int_t itrgcr, TTree *tree=0);
	virtual ~caleval();	
	virtual Int_t    Cut(Long64_t entry);
	virtual Int_t    GetEntry(Long64_t entry);
	virtual Long64_t LoadTree(Long64_t entry);
	virtual void     Init(TTree *tree);
	virtual Bool_t   Notify();
	virtual void     Show(Long64_t entry = -1);
	//
	virtual void 	 Setup();
	virtual void     Loop(Int_t);
	
	virtual float	Get_rawch0ch0();
	virtual float	Get_raw11();
	virtual float	Get_cal11();
	virtual float	Get_calavg();
	virtual float	Get_cal11X(int);
	virtual float	Get_cal11Xa(int);

	virtual void 	FormCorrA(const int);
	virtual void 	FormCorrB(const int);
	virtual void 	FormCorrC(const int);
	virtual void	WriteCorr(const int);
	virtual float	Calibrate(int, float, float, int);

	virtual int		mapTRGtoTOF(int, int);
	virtual int		mapTOFtoTRG(int, int);
	bool			invertmap;
	
	TString DataDirectory;
	TString WebDirectory;
	
	bool 	lit[19][2];
	int		idetearliest_raw[2];
	int		idetearliest_cal[2];
	float 	tsign;
	float 	tfactor;
	float	binxlow[128][38];
	float	bincorr[128][38];
	TGraph	*fcorr[38];
	TGraph	*fcorrA[38];
	TGraph	*fcorrB[38];
	TGraph	*fcorrC[38];
	Int_t	kChanKeyOffsets;
	Int_t	nentriestree;
	char	runstring[120];	//
//	char	outfilebase[120];	//
//	char	rootfile[120];	//
	Float_t	refmultmax;
	Float_t	CrudeEWoffset;
	
	TH1D*	hA[38];
	TH1D*	hA_even[38];
	TH1D*	hA_odd[38];
	
	Int_t           day;
	Int_t           run;
	Float_t         vpd_adc[38];
	Float_t         vpd_tac[38];
	Float_t         mvpd_adc[38];
	Float_t         mvpd_tac[38];
	Float_t         p2p_adc[16];
	Float_t         p2p_tac[16];
	Float_t         bbc_adc[48];
	Float_t         bbc_tac[48];
	Float_t         vpd_cco[38];
	Float_t         vpd_tle[38];
	Float_t         vpd_tot[38];
	Float_t         p2p_cco[16];
	Float_t         p2p_tle[16];
	Float_t         p2p_tot[16];

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
	
	Int_t		iRhicRun;
	Int_t		iEnergy;
	Int_t		iInputFlag;
	Int_t		iTreeStyle;
	Int_t		iChoice;
	Int_t		iAlgorithm;
	Int_t		iDayMin,iDayMax;
	Int_t		iTrgCrate;
	Int_t		dtimenbins;
	Float_t		*A;
	Float_t		*Aother;
	Float_t		*T;
	Float_t		Toff[38];
	Int_t		A_nbin;	
	Float_t		A_min, A_max;
	Float_t		T_min, T_max;
	Int_t		dT_nbin;	
	Float_t		dT_min, dT_max;
	Float_t		dT_min2, dT_max2;
	Float_t		rangeDt_0;
	Float_t		rangeDt_1;

	Float_t 	Zerrorcut;
	Float_t 	zvtoffset; 
	Float_t 	zvtoffsetb; 
	Float_t 	Zdiffcut; 
	Float_t 	Zdiffcutb; 
	Int_t		Nprimarycut;
	
	Int_t		NDETSIDE;
	Int_t 		NITERATIONS;
	Int_t 		NPASSES;
	Int_t		NREQUIRE;
	Float_t		TACtoNS;
	Float_t		ADCminimum;
	
	TPostScript *psout;
	TFile		*fout;
	//
	//
};


