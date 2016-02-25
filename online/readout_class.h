#include <iostream>
#include <fstream>
#include <bitset>

using namespace std;

#include <TROOT.h>
#include <TSystem.h>
#include <TStyle.h>
#include <TFile.h>
#include <TChain.h>
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
#include <TError.h>
#include <TLegend.h>

#include "/star/u/geurts/lit/SplineFit/TSplineFit.h"

class readout {
public :

	readout(TTree *tree=0, Int_t mmm=6);
	virtual ~readout();
	virtual Int_t    Cut(Long64_t entry);
	virtual Int_t    GetEntry(Long64_t entry);
	virtual Long64_t LoadTree(Long64_t entry);
	virtual void     Init(TTree *tree);
	virtual void     Loop(Int_t);
	virtual Bool_t   Notify();
	virtual void     Show(Long64_t entry = -1);
	
	Int_t	kDayUse;
	
	TTree          *fChain;   //!pointer to the analyzed TTree or TChain
	Int_t           fCurrent; //!current Tree number in a TChain	
	// Declaration of leaf types
	Int_t           run;
	Int_t           day;
	Int_t           daqbits;
	Int_t           itime;
	Int_t           evtid;
	Int_t           entry;
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
	Float_t         vpd_adc[38];
	Float_t         vpd_tac[38];
	Float_t         p2p_adc[16];
	Float_t         p2p_tac[16];
	Float_t         bbc_adc[48];
	Float_t         bbc_tac[48];
	Float_t         zdc_adc[16];
	Float_t         zdc_tac[16];
	Float_t         vpd_cco[38];
	Float_t         vpd_tle[38];
	Float_t         vpd_tot[38];
	Float_t         p2p_cco[16];
	Float_t         p2p_tle[16];
	Float_t         p2p_tot[16];
	
	// List of branches
	TBranch        *b_run;   //!
	TBranch        *b_day;   //!
	TBranch        *b_itime;   //!
	TBranch        *b_evtid;   //!
	TBranch        *b_entry;   //!
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
	TBranch        *b_daqbits;   //!
	TBranch        *b_vpd_adc;   //!
	TBranch        *b_vpd_tac;   //!
	TBranch        *b_p2p_adc;   //!
	TBranch        *b_p2p_tac;   //!
	TBranch        *b_bbc_adc;   //!
	TBranch        *b_bbc_tac;   //!
	TBranch        *b_zdc_adc;   //!
	TBranch        *b_zdc_tac;   //!
	TBranch        *b_vpd_cco;   //!
	TBranch        *b_vpd_tle;   //!
	TBranch        *b_vpd_tot;   //!
	TBranch        *b_p2p_cco;   //!
	TBranch        *b_p2p_tle;   //!
	TBranch        *b_p2p_tot;   //!
	
};

