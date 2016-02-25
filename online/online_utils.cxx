#include "online_class.h"

online::online(TTree *tree, Int_t krun)
{

	char	buf[120];
	kRunUse = krun;
	cout<<"..... online::online kRunUse = "<<kRunUse<<endl;

// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
//    if (tree == 0) {
//       TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("daqdoer_362.root");
//       if (!f) {
//          f = new TFile("daqdoer_362.root");
//       }
//       tree = (TTree*)gDirectory->Get("tp");
//    }

//   TFile *f = new TFile("/star/institutions/rice/geurts/dd/daqdoer_10362early.root");
//   TFile *f = new TFile("/star/institutions/rice/geurts/dd/daqdoer_10362064.root");
//   TFile *f = new TFile("/star/institutions/rice/geurts/dd/daqdoer_10362068.root");
//   TFile *f = new TFile("/star/institutions/rice/geurts/dd/daqdoer_10363010.root");
//   TFile *f = new TFile("/star/institutions/rice/geurts/dd/daqdoer_10364002.root");
//   TFile *f = new TFile("/star/institutions/rice/geurts/dd/daqdoer_10364003.root");
//   TFile *f = new TFile("/star/institutions/rice/geurts/dd/daqdoer_10364004.root");
//   TFile *f = new TFile("/star/institutions/rice/geurts/dd/daqdoer_10365002.root");
//
//   TFile *f = new TFile("/star/institutions/rice/geurts/dd/daqdoer_11001035.root");
//   TFile *f = new TFile("/star/institutions/rice/geurts/dd/daqdoer_11001037.root");
//   TFile *f = new TFile("/star/institutions/rice/geurts/dd/daqdoer_11001038.root");
//   TFile *f = new TFile("/star/institutions/rice/geurts/dd/daqdoer_11001053.root");

//   TFile *f = new TFile("/star/institutions/rice/geurts/dd/daqdoer_11004068.root");

//   TFile *f = new TFile("/star/institutions/rice/geurts/dd/daqdoer_11004067.root");
//   TFile *f = new TFile("/star/institutions/rice/geurts/dd/daqdoer_11004068.root");

//   TFile *f = new TFile("/star/institutions/rice/geurts/dd/daqdoer_11005010.root");
//   TFile *f = new TFile("/star/institutions/rice/geurts/dd/daqdoer_11005014.root");

//   sprintf(buf,"/star/u/geurts/run11/daqdoer/daqdoer_%d.root",kRunUse);
   sprintf(buf,"/star/institutions/rice/geurts/dd/daqdoer_%d.root",kRunUse);
   TFile *f = new TFile(buf);
   tree	= (TTree*)gDirectory->Get("tp");

   Init(tree);

}

online::~online()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t online::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t online::LoadTree(Long64_t entry)
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

void online::Init(TTree *tree)
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
   fChain->SetBranchAddress("vpd_cco", vpd_cco, &b_vpd_cco);
   fChain->SetBranchAddress("vpd_tle", vpd_tle, &b_vpd_tle);
   fChain->SetBranchAddress("vpd_tot", vpd_tot, &b_vpd_tot);
   fChain->SetBranchAddress("vpd_tac", vpd_tac, &b_vpd_tac);
   fChain->SetBranchAddress("vpd_adc", vpd_adc, &b_vpd_adc);
   fChain->SetBranchAddress("vpd_tachi", vpd_tachi, &b_vpd_tachi);
   fChain->SetBranchAddress("vpd_adchi", vpd_adchi, &b_vpd_adchi);
   fChain->SetBranchAddress("p2p_cco", p2p_cco, &b_p2p_cco);
   fChain->SetBranchAddress("p2p_tle", p2p_tle, &b_p2p_tle);
   fChain->SetBranchAddress("p2p_tot", p2p_tot, &b_p2p_tot);
   fChain->SetBranchAddress("p2p_tac", p2p_tac, &b_p2p_tac);
   fChain->SetBranchAddress("p2p_adc", p2p_adc, &b_p2p_adc);
   fChain->SetBranchAddress("bbc_tac", bbc_tac, &b_bbc_tac);
   fChain->SetBranchAddress("bbc_adc", bbc_adc, &b_bbc_adc);
   fChain->SetBranchAddress("zdc_tac", zdc_tac, &b_zdc_tac);
   fChain->SetBranchAddress("zdc_adc", zdc_adc, &b_zdc_adc);
   fChain->SetBranchAddress("p2p_sin", p2p_sin, &b_p2p_sin);
   Notify();
}

Bool_t online::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void online::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t online::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
