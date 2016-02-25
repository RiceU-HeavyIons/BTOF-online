#include <iostream>
#include <stdlib.h>
using namespace std;

void cp(){

if (gClassTable->GetID("TTable") < 0) {
	gSystem->Load("libStar");
	gSystem->Load("libPhysics");
}  
gROOT->LoadMacro("$STAR/StRoot/StMuDSTMaker/COMMON/macros/loadSharedLibraries.C");
loadSharedLibraries();

TFile *f1 = new TFile("/star/data09/reco/AuAu7_production/ReversedFullField/dev/2010/116/11116072/st_physics_11116072_raw_1010001.MuDst.root","READ");
f1->ls();
TTree *t1 = (TTree*)f1->Get("MuDst");

TObjArray *lb1 = t1->GetListOfBranches(); 
Int_t nb = lb1->GetEntries(); 
bool touched;
char bbuf[30],bbuf2[30];
int kStatus = 0;
for (Int_t i=0;i<nb;i++) { 
	TBranch *b = (TBranch*)lb1->At(i);
	sprintf(bbuf,"%s",b->GetName());
	sprintf(bbuf2,"%s*",b->GetName());
	touched = false;
	if (strcmp(bbuf,"OtherTracks" )==0){ t1->SetBranchStatus(bbuf2,kStatus); touched=true; } else
	if (strcmp(bbuf,"L3Tracks"    )==0){ t1->SetBranchStatus(bbuf2,kStatus); touched=true; } else
	if (strcmp(bbuf,"L3AlgoAccept")==0){ t1->SetBranchStatus(bbuf2,kStatus); touched=true; } else
	if (strcmp(bbuf,"L3AlgoReject")==0){ t1->SetBranchStatus(bbuf2,kStatus); touched=true; } else
	if (strcmp(bbuf,"RichSpectra" )==0){ t1->SetBranchStatus(bbuf2,kStatus); touched=true; } else
	if (strcmp(bbuf,"McEvent"     )==0){ t1->SetBranchStatus(bbuf2,kStatus); touched=true; } else
	if (strcmp(bbuf,"pp2pp"       )==0){ t1->SetBranchStatus(bbuf2,kStatus); touched=true; } else 
	if (strcmp(bbuf,"McV0"        )==0){ t1->SetBranchStatus(bbuf2,kStatus); touched=true; } else 
	if (strcmp(bbuf,"V0Assoc"     )==0){ t1->SetBranchStatus(bbuf2,kStatus); touched=true; } else 
	if (strcmp(bbuf,"McXi"        )==0){ t1->SetBranchStatus(bbuf2,kStatus); touched=true; } else 
	if (strcmp(bbuf,"XiAssoc"     )==0){ t1->SetBranchStatus(bbuf2,kStatus); touched=true; } else 
	if (strcmp(bbuf,"Kink"        )==0){ t1->SetBranchStatus(bbuf2,kStatus); touched=true; } else 
	if (strcmp(bbuf,"McKink"      )==0){ t1->SetBranchStatus(bbuf2,kStatus); touched=true; } else 
	if (strcmp(bbuf,"KinkAssoc"   )==0){ t1->SetBranchStatus(bbuf2,kStatus); touched=true; } else 
	if (strcmp(bbuf,"FmsHit"      )==0){ t1->SetBranchStatus(bbuf2,kStatus); touched=true; } else 
	if (strcmp(bbuf,"PmdHit"      )==0){ t1->SetBranchStatus(bbuf2,kStatus); touched=true; } else 
	if (strcmp(bbuf,"PmdCluster"  )==0){ t1->SetBranchStatus(bbuf2,kStatus); touched=true; } else 
	if (strcmp(bbuf,"CpvHit"      )==0){ t1->SetBranchStatus(bbuf2,kStatus); touched=true; } else
	if (strcmp(bbuf,"CpvCluster"  )==0){ t1->SetBranchStatus(bbuf2,kStatus); touched=true; } else
	if (strcmp(bbuf,"EEmcPrs"     )==0){ t1->SetBranchStatus(bbuf2,kStatus); touched=true; } else
	if (strcmp(bbuf,"EEmcSmdu"    )==0){ t1->SetBranchStatus(bbuf2,kStatus); touched=true; } else
	if (strcmp(bbuf,"EEmcSmdv"    )==0){ t1->SetBranchStatus(bbuf2,kStatus); touched=true; } 
	//
	if (touched){
		cout<<i<<" "<<b->GetName()<<" "<<bbuf<<"\t "<<t1->GetBranchStatus(bbuf)<<" "<<"touched"<<endl;
	} else {
		cout<<i<<" "<<b->GetName()<<" "<<bbuf<<"\t "<<t1->GetBranchStatus(bbuf)<<endl;
	}
}

TFile *f2 = new TFile("./picodst_11116072_raw_1010001.MuDst.root","RECREATE");
TTree *t2 = t1->CopyTree("MuEvent.mEventSummary.mNumberOfGoodPrimaryTracks[0]>0&&abs(Event.mPrimaryVertexZ[0])<100.");
t2->Write();

}