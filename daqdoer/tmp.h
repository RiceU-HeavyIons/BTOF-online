//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Thu Apr  4 14:32:29 2013 by ROOT version 5.22/00
// from TChain tm/chain
//////////////////////////////////////////////////////////

#ifndef tmp_h
#define tmp_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

class tmp {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

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
   UInt_t          evp_daqbitm;
   UInt_t          evp_daqbitml1;
   UInt_t          evp_daqbitml2;
   UInt_t          evp_evttime;
   UInt_t          evp_evtmeq;
   Int_t           nMtdHits;
   Int_t           nMtdHitsLE;
   Int_t           nMtdHitsTE;
   Int_t           tray[422];   //[nMtdHits]
   Int_t           module[422];   //[nMtdHits]
   Int_t           modch[422];   //[nMtdHits]
   Int_t           cell[422];   //[nMtdHits]
   Int_t           strip[422];   //[nMtdHits]
   Int_t           strlr[422];   //[nMtdHits]
   Float_t         coco[422];   //[nMtdHits]
   Float_t         trgdtime[422];   //[nMtdHits]
   Float_t         tle[422];   //[nMtdHits]
   Float_t         tte[422];   //[nMtdHits]
   Float_t         tot[422];   //[nMtdHits]

   // List of branches
   TBranch        *b_time;   //!
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
   TBranch        *b_trgdtime;   //!
   TBranch        *b_tle;   //!
   TBranch        *b_tte;   //!
   TBranch        *b_tot;   //!

   tmp(TTree *tree=0);
   virtual ~tmp();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef tmp_cxx
tmp::tmp(TTree *tree)
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {

#ifdef SINGLE_TREE
      // The following code should be used if you want this class to access
      // a single tree instead of a chain
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("/star/institutions/rice/llope/dd/daqdoer_activity.root");
      if (!f) {
         f = new TFile("/star/institutions/rice/llope/dd/daqdoer_activity.root");
         f->cd("Rint:/");
      }
      tree = (TTree*)gDirectory->Get("tm");

#else // SINGLE_TREE

      // The following code should be used if you want this class to access a chain
      // of trees.
      TChain * chain = new TChain("tm","chain");
      chain->Add("/star/institutions/rice/llope/dd/daqdoer_activity.root/tm");
      chain->Add("/star/institutions/rice/llope/dd/daqdoer_activity_1.root/tm");
      tree = chain;
#endif // SINGLE_TREE

   }
   Init(tree);
}

tmp::~tmp()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t tmp::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t tmp::LoadTree(Long64_t entry)
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

void tmp::Init(TTree *tree)
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

   fChain->SetBranchAddress("time", &time, &b_time);
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
   fChain->SetBranchAddress("trgdtime", trgdtime, &b_trgdtime);
   fChain->SetBranchAddress("tle", tle, &b_tle);
   fChain->SetBranchAddress("tte", tte, &b_tte);
   fChain->SetBranchAddress("tot", tot, &b_tot);
   Notify();
}

Bool_t tmp::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void tmp::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t tmp::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef tmp_cxx
