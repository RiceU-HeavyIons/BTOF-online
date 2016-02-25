//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Fri May 29 12:32:52 2009 by ROOT version 5.12/00h-rc2
// from TTree tp/pp2pp-upvpd tree
// found on file: daqdoer_149.root
//////////////////////////////////////////////////////////

#ifndef view_h
#define view_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

class view {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   // Declaration of leave types
   Int_t           day;
   Int_t           run;
   Int_t           daqbits;
   Int_t           vpd_cco[38];
   Float_t         vpd_tle[38];
   Float_t         vpd_tot[38];
   Float_t         vpd_tac[38];
   Float_t         vpd_adc[38];
   Int_t           p2p_cco[16];
   Float_t         p2p_tle[16];
   Float_t         p2p_tot[16];
   Float_t         p2p_tac[16];
   Float_t         p2p_adc[16];

   // List of branches
   TBranch        *b_day;   //!
   TBranch        *b_run;   //!
   TBranch        *b_daqbits;   //!
   TBranch        *b_vpd_cco;   //!
   TBranch        *b_vpd_tle;   //!
   TBranch        *b_vpd_tot;   //!
   TBranch        *b_vpd_tac;   //!
   TBranch        *b_vpd_adc;   //!
   TBranch        *b_p2p_cco;   //!
   TBranch        *b_p2p_tle;   //!
   TBranch        *b_p2p_tot;   //!
   TBranch        *b_p2p_tac;   //!
   TBranch        *b_p2p_adc;   //!

   view(TTree *tree=0);
   virtual ~view();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef view_cxx
view::view(TTree *tree)
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
//      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("daqdoer_125.root");
//      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("daqdoer_142.root");
//      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("daqdoer_149.root");
//      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("daqdoer_171.root");
//      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("daqdoer_177.root");
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("daqdoer_182.root");

      //
      if (!f) {
//         f = new TFile("daqdoer_125.root");
//         f = new TFile("daqdoer_142.root");
//         f = new TFile("daqdoer_149.root");
//         f = new TFile("daqdoer_171.root");
//         f = new TFile("daqdoer_177.root");
         f = new TFile("daqdoer_182.root");


      }
      tree = (TTree*)gDirectory->Get("tp");
      
      cout<<f->GetName()<<endl;

   }
   Init(tree);
}

view::~view()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t view::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t view::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->IsA() != TChain::Class()) return centry;
   TChain *chain = (TChain*)fChain;
   if (chain->GetTreeNumber() != fCurrent) {
      fCurrent = chain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void view::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normaly not necessary to make changes to the generated
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
   fChain->SetBranchAddress("daqbits", &daqbits, &b_daqbits);
   fChain->SetBranchAddress("vpd_cco", vpd_cco, &b_vpd_cco);
   fChain->SetBranchAddress("vpd_tle", vpd_tle, &b_vpd_tle);
   fChain->SetBranchAddress("vpd_tot", vpd_tot, &b_vpd_tot);
   fChain->SetBranchAddress("vpd_tac", vpd_tac, &b_vpd_tac);
   fChain->SetBranchAddress("vpd_adc", vpd_adc, &b_vpd_adc);
   fChain->SetBranchAddress("p2p_cco", p2p_cco, &b_p2p_cco);
   fChain->SetBranchAddress("p2p_tle", p2p_tle, &b_p2p_tle);
   fChain->SetBranchAddress("p2p_tot", p2p_tot, &b_p2p_tot);
   fChain->SetBranchAddress("p2p_tac", p2p_tac, &b_p2p_tac);
   fChain->SetBranchAddress("p2p_adc", p2p_adc, &b_p2p_adc);
   Notify();
}

Bool_t view::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normaly not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void view::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t view::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef view_cxx
