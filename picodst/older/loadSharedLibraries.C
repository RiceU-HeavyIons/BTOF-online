#include "TSystem.h"

extern TSystem* gSystem;

void loadSharedLibraries() {
  gSystem->Load("libStTableUtilities");
  gSystem->Load("libTable");
  gSystem->Load("libPhysics");
  gSystem->Load("St_base");
  gSystem->Load("StChain");
  gSystem->Load("St_Tables");
  gSystem->Load("StUtilities");        // new addition 22jul99
  gSystem->Load("StTreeMaker");
  gSystem->Load("StIOMaker");
  gSystem->Load("StarClassLibrary");
  gSystem->Load("StEvent");
  gSystem->Load("StTriggerDataMaker"); // new starting from April 2003
//  gSystem->Load("StTriggerData");
//  gSystem->Load("StTriggerData2013");	// WJL
  gSystem->Load("StBichsel");
  gSystem->Load("StEventUtilities");
  gSystem->Load("StDbLib");
  gSystem->Load("StEmcUtil");
  gSystem->Load("StTofUtil");
  gSystem->Load("StPmdUtil");
  gSystem->Load("StPreEclMaker");
  gSystem->Load("StStrangeMuDstMaker");
  gSystem->Load("StMuDSTMaker");  
//
//

  cout << " loading of shared libraries done" << endl;
}
