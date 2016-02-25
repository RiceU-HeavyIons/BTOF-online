//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Wed Apr  6 09:46:28 2011 by ROOT version 5.22/00
// from TTree tm/MTD stop-side tree
// found on file: daqdoer_12076029.root
//////////////////////////////////////////////////////////

#ifndef mtdview_h
#define mtdview_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

class mtdview {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   // Declaration of leaf types
   Int_t           day;
   Int_t           run;
   UInt_t          evp_evtnum;
   UInt_t          evp_token;
   UInt_t          evp_trigcmd;
   UInt_t          evp_daqcmd;
   UInt_t          evp_trgword;
   UInt_t          evp_phyword;
   UInt_t          evp_daqbitm;
   UInt_t          evp_daqbitml1;
   UInt_t          evp_daqbitml2;
   UInt_t          evp_evttime;
   UInt_t          evp_evtmeq;
   Int_t           nMtdHits;
   Int_t           nMtdHitsLE;
   Int_t           nMtdHitsTE;
   Int_t           tray[1000];   //[nMtdHits]
   Int_t           module[1000];   //[nMtdHits]
   Int_t           modch[1000];   //[nMtdHits]
   Int_t           cell[1000];   //[nMtdHits]
   Int_t           strip[1000];   //[nMtdHits]
   Int_t           strlr[1000];   //[nMtdHits]
   Float_t         coco[1000];   //[nMtdHits]
   Float_t         tle[1000];   //[nMtdHits]
   Float_t         tte[1000];   //[nMtdHits]
   Float_t         tot[1000];   //[nMtdHits]

   // List of branches
   TBranch        *b_day;   //!
   TBranch        *b_run;   //!
   TBranch        *b_evp;   //!
   TBranch        *b_nMtdHits;   //!
   TBranch        *b_nMtdHitsLE;   //!
   TBranch        *b_nMtdHitsTE;   //!
   TBranch        *b_tray;   //!
   TBranch        *b_module;   //!
   TBranch        *b_modch;   //!
   TBranch        *b_cell;   //!
   TBranch        *b_strip;   //!
   TBranch        *b_strlr;   //!
   TBranch        *b_coco;   //!
   TBranch        *b_tle;   //!
   TBranch        *b_tte;   //!
   TBranch        *b_tot;   //!

   mtdview(TTree *tree=0);
   virtual ~mtdview();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef mtdview_cxx
mtdview::mtdview(TTree *tree)
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
//   if (tree == 0) {
//      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("daqdoer_12076029.root");
//      if (!f) {
//         f = new TFile("daqdoer_12076029.root");
//      }
//      tree = (TTree*)gDirectory->Get("tm");
//   }

	TChain *chain = new TChain("tm","mtd hits chain");

// 	chain->Add("daqdoer_12076003.root");
// 	chain->Add("daqdoer_12076007.root");
// 	chain->Add("daqdoer_12076008.root");
// 	chain->Add("daqdoer_12076029.root");
// 	chain->Add("daqdoer_12075019.root");
// 	chain->Add("daqdoer_12075021.root");
// 	chain->Add("daqdoer_12076009.root");
// 	chain->Add("daqdoer_12075040.root");
// 	chain->Add("daqdoer_12075041.root");

	chain->Add("/star/institutions/rice/llope/dd/daqdoer_12127004_mtd.root");
	
	tree	= chain;

	Init(tree);
}

mtdview::~mtdview()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t mtdview::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t mtdview::LoadTree(Long64_t entry)
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

void mtdview::Init(TTree *tree)
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

   fChain->SetBranchAddress("day", &day, &b_day);
   fChain->SetBranchAddress("run", &run, &b_run);
   fChain->SetBranchAddress("evp", &evp_evtnum, &b_evp);
   fChain->SetBranchAddress("nMtdHits", &nMtdHits, &b_nMtdHits);
   fChain->SetBranchAddress("nMtdHitsLE", &nMtdHitsLE, &b_nMtdHitsLE);
   fChain->SetBranchAddress("nMtdHitsTE", &nMtdHitsTE, &b_nMtdHitsTE);
   fChain->SetBranchAddress("tray", tray, &b_tray);
   fChain->SetBranchAddress("module", module, &b_module);
   fChain->SetBranchAddress("modch", modch, &b_modch);
   fChain->SetBranchAddress("cell", cell, &b_cell);
   fChain->SetBranchAddress("strip", strip, &b_strip);
   fChain->SetBranchAddress("strlr", strlr, &b_strlr);
   fChain->SetBranchAddress("coco", coco, &b_coco);
   fChain->SetBranchAddress("tle", tle, &b_tle);
   fChain->SetBranchAddress("tte", tte, &b_tte);
   fChain->SetBranchAddress("tot", tot, &b_tot);
   Notify();
}

Bool_t mtdview::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void mtdview::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t mtdview::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef mtdview_cxx
