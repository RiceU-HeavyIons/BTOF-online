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
#include <TLine.h>
#include <TError.h>
#include <TGraph.h>

//#include "/star/u/llope/lit/SplineFit/TSplineFit.h"

// default directories (BNL)
#define DEFAULT_IN_DIR /gpfs01/star/subsysg/TOF/BTOF-online/dd/
#define DEFAULT_OUT_DIR "/gpfs01/star/subsysg/TOF/BTOF-online/noise/"
#define DEFAULT_PUB_DIR "/star/u/geurts/WWW/files/"
#define PS2PDF "/usr/bin/ps2pdf"

// default directories (Rice)
//#define DEFAULT_IN_DIR "/macstar/star4/btof/dd/"
//#define DEFAULT_OUT_DIR "/macstar/star4/btof/noise/"
//#define DEFAULT_PUB_DIR "/macstar/star4/btof/pub/"
//#define PS2PDF "/usr/local/bin/ps2pdf"

class noise {
public :

	noise(TTree *treet=0, TTree *treem=0, Int_t kkk=0, Int_t ntd=0);
	virtual ~noise();
	virtual Int_t    Cut(Long64_t entry);
	virtual Int_t    GetEntry(Long64_t entry);
	virtual Long64_t LoadTree(Long64_t entry);
	virtual void     Init(TTree *treet,TTree *treem);
	virtual void     Loop();
	virtual Bool_t   Notify();
	virtual void     Show(Long64_t entry = -1);

	TFile		   *finput;
	TTree          *fChaint;   //!pointer to the analyzed TTree or TChain
	Int_t           fCurrentt; //!current Tree number in a TChain
	TTree          *fChainm;   //!pointer to the analyzed TTree or TChain
	Int_t           fCurrentm; //!current Tree number in a TChain

	Int_t			kRunUse;
	Int_t			nToDo;
//	string noise_in_dir, noise_out_dir, noise_pub_dir;

	TH1D *hnhitstof_time;
	TH1D *hnhitsmtd26_time;
	TH1D *hnhitsmtd27_time;
	TH1D *hnhitsmtd28_time;

	// Declaration of leaf types
	Int_t           day;
	Int_t           run;
	UInt_t          evp_evtnum;
	UInt_t          evp_token;
	UInt_t          evp_trigcmd;
	UInt_t          evp_daqcmd;
	UInt_t          evp_trgword;
	UInt_t          evp_phyword;
	UInt_t          evp_daqbits;
	UInt_t          evp_daqbitsl1;
	UInt_t          evp_daqbitsl2;
	UInt_t          evp_evttime;
	UInt_t          evp_evtseq;
	Int_t           nTofHits;
	Int_t           nTofHitsLE;
	Int_t           nTofHitsTE;
	Int_t           tray[100000];   //[nTofHits]
	Int_t           module[100000];   //[nTofHits]
	Int_t           cell[100000];   //[nTofHits]
	Float_t         coco[100000];   //[nTofHits]
	Float_t         tle[100000];   //[nTofHits]
	Float_t         tte[100000];   //[nTofHits]
	Float_t         tot[100000];   //[nTofHits]

	Int_t           nMtdHits;
	Int_t           nMtdHitsLE;
	Int_t           nMtdHitsTE;
	Int_t           mtray[100000];   //[nMtdHits]
	Int_t           mmodule[100000];   //[nMtdHits]
	Int_t           mmodch[100000];   //[nMtdHits]
	Int_t           mcell[100000];   //[nMtdHits]
	Int_t           mstrip[100000];   //[nMtdHits]
	Int_t           mstrlr[100000];   //[nMtdHits]
	Float_t         mcoco[100000];   //[nMtdHits]
	Float_t         mtle[100000];   //[nMtdHits]
	Float_t         mtte[100000];   //[nMtdHits]
	Float_t         mtot[100000];   //[nMtdHits]

	// List of branches
	TBranch        *b_day;   //!
	TBranch        *b_run;   //!
	TBranch        *b_evp;   //!
	TBranch        *b_nTofHits;   //!
	TBranch        *b_nTofHitsLE;   //!
	TBranch        *b_nTofHitsTE;   //!
	TBranch        *b_tray;   //!
	TBranch        *b_module;   //!
	TBranch        *b_cell;   //!
	TBranch        *b_coco;   //!
	TBranch        *b_tle;   //!
	TBranch        *b_tte;   //!
	TBranch        *b_tot;   //!

   TBranch        *b_nMtdHits;   //!
   TBranch        *b_nMtdHitsLE;   //!
   TBranch        *b_nMtdHitsTE;   //!
   TBranch        *b_mtray;   //!
   TBranch        *b_mmodule;   //!
   TBranch        *b_mmodch;   //!
   TBranch        *b_mcell;   //!
   TBranch        *b_mstrip;   //!
   TBranch        *b_mstrlr;   //!
   TBranch        *b_mcoco;   //!
   TBranch        *b_mtle;   //!
   TBranch        *b_mtte;   //!
   TBranch        *b_mtot;   //!

};
