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

class online {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   Int_t	kRunUse;
   TCanvas *ccan[100];

   online(TTree *tree=0, Int_t kkk=0);
   virtual ~online();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);

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
   Int_t           vpd_cco[38];
   Float_t         vpd_tle[38];
   Float_t         vpd_tot[38];
   Float_t         vpd_tac[38];
   Float_t         vpd_adc[38];
   Float_t         vpd_tachi[38];
   Float_t         vpd_adchi[38];
   Int_t           p2p_cco[16];
   Float_t         p2p_tle[16];
   Float_t         p2p_tot[16];
   Float_t         p2p_tac[16];
   Float_t         p2p_adc[16];
   Float_t         bbc_tac[48];
   Float_t         bbc_adc[48];
   Float_t         zdc_tac[6];
   Float_t         zdc_adc[6];
   Int_t           p2p_sin[32];

   // List of branches
   TBranch        *b_day;   //!
   TBranch        *b_run;   //!
   TBranch        *b_evp;   //!
   TBranch        *b_vpd_cco;   //!
   TBranch        *b_vpd_tle;   //!
   TBranch        *b_vpd_tot;   //!
   TBranch        *b_vpd_tac;   //!
   TBranch        *b_vpd_adc;   //!
   TBranch        *b_vpd_tachi;   //!
   TBranch        *b_vpd_adchi;   //!
   TBranch        *b_p2p_cco;   //!
   TBranch        *b_p2p_tle;   //!
   TBranch        *b_p2p_tot;   //!
   TBranch        *b_p2p_tac;   //!
   TBranch        *b_p2p_adc;   //!
   TBranch        *b_bbc_tac;   //!
   TBranch        *b_bbc_adc;   //!
   TBranch        *b_zdc_tac;   //!
   TBranch        *b_zdc_adc;   //!
   TBranch        *b_p2p_sin;   //!

};

