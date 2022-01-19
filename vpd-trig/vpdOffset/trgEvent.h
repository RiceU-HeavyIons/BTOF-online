//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Thu Mar  8 13:56:22 2018 by ROOT version 5.34/30
// from TTree trgEvent/trgEvent
// found on file: ../readDat/output/19065018/run19065018.root
//////////////////////////////////////////////////////////

#ifndef trgEvent_h
#define trgEvent_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.

// Fixed size dimensions of array or collections stored in the TTree if any.

class trgEvent {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   // Declaration of leaf types
   UInt_t          eventNumber;
   ULong64_t       triggerMask;
   UInt_t          npre;
   UInt_t          npost;
   UInt_t          nsum;
   UShort_t        bbqEastVpd[11][32];   //[nsum]
   UShort_t        bbqWestVpd[11][32];   //[nsum]
   UShort_t        fastBbqEastVpd[11];   //[nsum]
   UShort_t        fastBbqWestVpd[11];   //[nsum]
   UShort_t        bbcEastVpdNHits[11];   //[nsum]
   UShort_t        bbcEastVpdADCsum[11];   //[nsum]
   UShort_t        bbcEastVpdTACsum[11];   //[nsum]
   UShort_t        bbcWestVpdNHits[11];   //[nsum]
   UShort_t        bbcWestVpdADCsum[11];   //[nsum]
   UShort_t        bbcWestVpdTACsum[11];   //[nsum]
   UShort_t        mxqEastVpd[11][32];   //[nsum]
   UShort_t        mxqWestVpd[11][32];   //[nsum]
   UShort_t        fastMxqEastVpd[11];   //[nsum]
   UShort_t        fastMxqWestVpd[11];   //[nsum]
   UShort_t        tofMult[11];   //[nsum]
   UShort_t        tofTrayMult[11][120];   //[nsum]
   UShort_t        MT001[11][32];   //[nsum]
   UShort_t        MT002[11][32];   //[nsum]
   UShort_t        MT003[11][32];   //[nsum]
   UShort_t        MT004[11][32];   //[nsum]
   UChar_t         MtdDsm[11][32];   //[nsum]
   UShort_t        DsmTF201Ch[8];
   UShort_t        lastDsmBit[8];

   // List of branches
   TBranch        *b_eventNumber;   //!
   TBranch        *b_triggerMask;   //!
   TBranch        *b_npre;   //!
   TBranch        *b_npost;   //!
   TBranch        *b_nsum;   //!
   TBranch        *b_bbqEastVpd;   //!
   TBranch        *b_bbqWestVpd;   //!
   TBranch        *b_fastBbqEastVpd;   //!
   TBranch        *b_fastBbqWestVpd;   //!
   TBranch        *b_bbcEastVpdNHits;   //!
   TBranch        *b_bbcEastVpdADCsum;   //!
   TBranch        *b_bbcEastVpdTACsum;   //!
   TBranch        *b_bbcWestVpdNHits;   //!
   TBranch        *b_bbcWestVpdADCsum;   //!
   TBranch        *b_bbcWestVpdTACsum;   //!
   TBranch        *b_mxqEastVpd;   //!
   TBranch        *b_mxqWestVpd;   //!
   TBranch        *b_fastMxqEastVpd;   //!
   TBranch        *b_fastMxqWestVpd;   //!
   TBranch        *b_tofMult;   //!
   TBranch        *b_tofTrayMult;   //!
   TBranch        *b_MT001;   //!
   TBranch        *b_MT002;   //!
   TBranch        *b_MT003;   //!
   TBranch        *b_MT004;   //!
   TBranch        *b_MtdDsm;   //!
   TBranch        *b_DsmTF201Ch;   //!
   TBranch        *b_lastDsmBit;   //!

   trgEvent(TTree *tree=0);
   virtual ~trgEvent();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef trgEvent_cxx
trgEvent::trgEvent(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("../readDat/output/19065018/run19065018.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("../readDat/output/19065018/run19065018.root");
      }
      f->GetObject("trgEvent",tree);

   }
   Init(tree);
}

trgEvent::~trgEvent()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t trgEvent::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t trgEvent::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void trgEvent::Init(TTree *tree)
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

   fChain->SetBranchAddress("eventNumber", &eventNumber, &b_eventNumber);
   fChain->SetBranchAddress("triggerMask", &triggerMask, &b_triggerMask);
   fChain->SetBranchAddress("npre", &npre, &b_npre);
   fChain->SetBranchAddress("npost", &npost, &b_npost);
   fChain->SetBranchAddress("nsum", &nsum, &b_nsum);
   fChain->SetBranchAddress("bbqEastVpd", bbqEastVpd, &b_bbqEastVpd);
   fChain->SetBranchAddress("bbqWestVpd", bbqWestVpd, &b_bbqWestVpd);
   fChain->SetBranchAddress("fastBbqEastVpd", fastBbqEastVpd, &b_fastBbqEastVpd);
   fChain->SetBranchAddress("fastBbqWestVpd", fastBbqWestVpd, &b_fastBbqWestVpd);
   fChain->SetBranchAddress("bbcEastVpdNHits", bbcEastVpdNHits, &b_bbcEastVpdNHits);
   fChain->SetBranchAddress("bbcEastVpdADCsum", bbcEastVpdADCsum, &b_bbcEastVpdADCsum);
   fChain->SetBranchAddress("bbcEastVpdTACsum", bbcEastVpdTACsum, &b_bbcEastVpdTACsum);
   fChain->SetBranchAddress("bbcWestVpdNHits", bbcWestVpdNHits, &b_bbcWestVpdNHits);
   fChain->SetBranchAddress("bbcWestVpdADCsum", bbcWestVpdADCsum, &b_bbcWestVpdADCsum);
   fChain->SetBranchAddress("bbcWestVpdTACsum", bbcWestVpdTACsum, &b_bbcWestVpdTACsum);
   fChain->SetBranchAddress("mxqEastVpd", mxqEastVpd, &b_mxqEastVpd);
   fChain->SetBranchAddress("mxqWestVpd", mxqWestVpd, &b_mxqWestVpd);
   fChain->SetBranchAddress("fastMxqEastVpd", fastMxqEastVpd, &b_fastMxqEastVpd);
   fChain->SetBranchAddress("fastMxqWestVpd", fastMxqWestVpd, &b_fastMxqWestVpd);
   fChain->SetBranchAddress("tofMult", tofMult, &b_tofMult);
   fChain->SetBranchAddress("tofTrayMult", tofTrayMult, &b_tofTrayMult);
   fChain->SetBranchAddress("MT001", MT001, &b_MT001);
   fChain->SetBranchAddress("MT002", MT002, &b_MT002);
   fChain->SetBranchAddress("MT003", MT003, &b_MT003);
   fChain->SetBranchAddress("MT004", MT004, &b_MT004);
   fChain->SetBranchAddress("MtdDsm", MtdDsm, &b_MtdDsm);
   fChain->SetBranchAddress("DsmTF201Ch", DsmTF201Ch, &b_DsmTF201Ch);
   fChain->SetBranchAddress("lastDsmBit", lastDsmBit, &b_lastDsmBit);
   Notify();
}

Bool_t trgEvent::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void trgEvent::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t trgEvent::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef trgEvent_cxx
