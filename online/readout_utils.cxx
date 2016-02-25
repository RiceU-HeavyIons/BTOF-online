#include "readout_class.h"

readout::readout(TTree *tree, Int_t mday)
{

	kDayUse	= mday;
	char buf[100];
	sprintf(buf,"/star/u/geurts/run9/muapp/out_%d.root",kDayUse);
	
	if (tree == 0) {
		TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject(buf);
		if (!f) {
			f = new TFile(buf);
		}
		tree = (TTree*)gDirectory->Get("tree");	
	}
	Init(tree);

}

readout::~readout()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t readout::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t readout::LoadTree(Long64_t entry)
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

void readout::Init(TTree *tree)
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

   fChain->SetBranchAddress("run", &run, &b_run);
   fChain->SetBranchAddress("day", &day, &b_day);
   fChain->SetBranchAddress("itime", &itime, &b_itime);
   fChain->SetBranchAddress("evtid", &evtid, &b_evtid);
   fChain->SetBranchAddress("entry", &entry, &b_entry);
   fChain->SetBranchAddress("grefmult", &grefmult, &b_grefmult);
   fChain->SetBranchAddress("nvert", &nvert, &b_nvert);
   fChain->SetBranchAddress("nprimary", &nprimary, &b_nprimary);
   fChain->SetBranchAddress("nglobal", &nglobal, &b_nglobal);
   fChain->SetBranchAddress("vtx", vtx, &b_vtx);
   fChain->SetBranchAddress("vtxe", vtxe, &b_vtxe);
   fChain->SetBranchAddress("fill", fill, &b_fill);
   fChain->SetBranchAddress("field", &field, &b_field);
   fChain->SetBranchAddress("zdcrate", zdcrate, &b_zdcrate);
   fChain->SetBranchAddress("bbcrate", bbcrate, &b_bbcrate);
   fChain->SetBranchAddress("bbcbg", bbcbg, &b_bbcbg);
   fChain->SetBranchAddress("zdccoinrate", &zdccoinrate, &b_zdccoinrate);
   fChain->SetBranchAddress("bbccoinrate", &bbccoinrate, &b_bbccoinrate);
   fChain->SetBranchAddress("daqbits", &daqbits, &b_daqbits);
   fChain->SetBranchAddress("vpd_adc", vpd_adc, &b_vpd_adc);
   fChain->SetBranchAddress("vpd_tac", vpd_tac, &b_vpd_tac);
   fChain->SetBranchAddress("p2p_adc", p2p_adc, &b_p2p_adc);
   fChain->SetBranchAddress("p2p_tac", p2p_tac, &b_p2p_tac);
   fChain->SetBranchAddress("bbc_adc", bbc_adc, &b_bbc_adc);
   fChain->SetBranchAddress("bbc_tac", bbc_tac, &b_bbc_tac);
   fChain->SetBranchAddress("zdc_adc", zdc_adc, &b_zdc_adc);
   fChain->SetBranchAddress("zdc_tac", zdc_tac, &b_zdc_tac);
   fChain->SetBranchAddress("vpd_cco", vpd_cco, &b_vpd_cco);
   fChain->SetBranchAddress("vpd_tle", vpd_tle, &b_vpd_tle);
   fChain->SetBranchAddress("vpd_tot", vpd_tot, &b_vpd_tot);
   fChain->SetBranchAddress("p2p_cco", p2p_cco, &b_p2p_cco);
   fChain->SetBranchAddress("p2p_tle", p2p_tle, &b_p2p_tle);
   fChain->SetBranchAddress("p2p_tot", p2p_tot, &b_p2p_tot);
   Notify();
}

Bool_t readout::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void readout::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t readout::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}

