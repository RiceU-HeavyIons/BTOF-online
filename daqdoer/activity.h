//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Thu Apr  4 13:44:36 2013 by ROOT version 5.22/00
// from TTree ts/TOF stop-side tree
// found on file: /star/institutions/rice/llope/dd/daqdoer_activity.root
//////////////////////////////////////////////////////////

#ifndef activity_h
#define activity_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

class activity {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

	Int_t			Detector;

   // Declaration of leaf types
   UInt_t          time;
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
   Int_t           nHits;
   Int_t           nHitsLE;
   Int_t           nHitsTE;
//    Int_t           tray[20000];   //[nTofHits]
//    Int_t           module[20000];   //[nTofHits]
//    Int_t           cell[20000];   //[nTofHits]
//    Float_t         coco[20000];   //[nTofHits]
//    Float_t         bunchid[20000];   //[nTofHits]
//    Float_t         trgdtime[20000];   //[nTofHits]
//    Float_t         tle[20000];   //[nTofHits]
//    Float_t         tte[20000];   //[nTofHits]
//    Float_t         tot[20000];   //[nTofHits]

   // List of branches
   TBranch        *b_time;   //!
   TBranch        *b_day;   //!
   TBranch        *b_run;   //!
   TBranch        *b_evp;   //!
   TBranch        *b_nHits;   //!
   TBranch        *b_nHitsLE;   //!
   TBranch        *b_nHitsTE;   //!
//    TBranch        *b_tray;   //!
//    TBranch        *b_module;   //!
//    TBranch        *b_cell;   //!
//    TBranch        *b_coco;   //!
//    TBranch        *b_bunchid;   //!
//    TBranch        *b_trgdtime;   //!
//    TBranch        *b_tle;   //!
//    TBranch        *b_tte;   //!
//    TBranch        *b_tot;   //!

   activity(TTree *tree=0);
   virtual ~activity();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef activity_cxx
activity::activity(TTree *tree)
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
// 	if (tree == 0) {
// 		TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("/star/institutions/rice/llope/dd/daqdoer_activity.root");
// 		if (!f) {
// 			f = new TFile("/star/institutions/rice/llope/dd/daqdoer_activity.root");
// 		}
// 		tree = (TTree*)gDirectory->Get("ts");
// 	}

	Detector = 1;	// =0 TOF, =1 MTD

	TChain *chain;
	if (Detector==0){
		chain = new TChain("ts","chain");
	} else if (Detector==1){
		chain = new TChain("tm","chain");
	}
	chain->Add("/star/institutions/rice/llope/dd/daqdoer_activity.root");
	chain->Add("/star/institutions/rice/llope/dd/daqdoer_activity_1.root");
	tree = chain;
	Init(tree);
}

activity::~activity()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t activity::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t activity::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (!fChain->InheritsFrom(TChain::Class()))  return centry;
   TChain *chain = (TChain*)fChain;
   if (chain->GetTreeNumber() != fCurrent) {
      fCurrent = chain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void activity::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

	if (Detector==0){
	   fChain->SetBranchAddress("time", &time, &b_time);
	   fChain->SetBranchAddress("day", &day, &b_day);
	   fChain->SetBranchAddress("run", &run, &b_run);
	   fChain->SetBranchAddress("evp", &evp_evtnum, &b_evp);
	   fChain->SetBranchAddress("nTofHits", &nHits, &b_nHits);
	   fChain->SetBranchAddress("nTofHitsLE", &nHitsLE, &b_nHitsLE);
	   fChain->SetBranchAddress("nTofHitsTE", &nHitsTE, &b_nHitsTE);
// 	   fChain->SetBranchAddress("tray", tray, &b_tray);
// 	   fChain->SetBranchAddress("module", module, &b_module);
// 	   fChain->SetBranchAddress("cell", cell, &b_cell);
// 	   fChain->SetBranchAddress("coco", coco, &b_coco);
// 	   fChain->SetBranchAddress("bunchid", bunchid, &b_bunchid);
// 	   fChain->SetBranchAddress("trgdtime", trgdtime, &b_trgdtime);
// 	   fChain->SetBranchAddress("tle", tle, &b_tle);
// 	   fChain->SetBranchAddress("tte", tte, &b_tte);
// 	   fChain->SetBranchAddress("tot", tot, &b_tot);
	} else if (Detector==1){
	   fChain->SetBranchAddress("time", &time, &b_time);
	   fChain->SetBranchAddress("day", &day, &b_day);
	   fChain->SetBranchAddress("run", &run, &b_run);
	   fChain->SetBranchAddress("evp", &evp_evtnum, &b_evp);
	   fChain->SetBranchAddress("nMtdHits", &nHits, &b_nHits);
	   fChain->SetBranchAddress("nMtdHitsLE", &nHitsLE, &b_nHitsLE);
	   fChain->SetBranchAddress("nMtdHitsTE", &nHitsTE, &b_nHitsTE);
// 	   fChain->SetBranchAddress("tray", tray, &b_tray);
// 	   fChain->SetBranchAddress("module", module, &b_module);
// 	   fChain->SetBranchAddress("modch", modch, &b_modch);
// 	   fChain->SetBranchAddress("cell", cell, &b_cell);
// 	   fChain->SetBranchAddress("strip", strip, &b_strip);
// 	   fChain->SetBranchAddress("strlr", strlr, &b_strlr);
// 	   fChain->SetBranchAddress("coco", coco, &b_coco);
// 	   fChain->SetBranchAddress("trgdtime", trgdtime, &b_trgdtime);
// 	   fChain->SetBranchAddress("tle", tle, &b_tle);
// 	   fChain->SetBranchAddress("tte", tte, &b_tte);
// 	   fChain->SetBranchAddress("tot", tot, &b_tot);
	}

	Notify();
}

Bool_t activity::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void activity::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t activity::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef activity_cxx
