#include <iostream>
#include <stdlib.h>
using namespace std;

void cp(char fileIn[200], char fileOut[200]){

cout<<"INPUT  ... "<<fileIn<<endl;
cout<<"OUTPUT ... "<<fileOut<<endl;

if (gClassTable->GetID("TTable") < 0) {
	gSystem->Load("libStar");
	gSystem->Load("libPhysics");
} 
//gROOT->LoadMacro("/afs/rhic.bnl.gov/star/packages/new/StRoot/StMuDSTMaker/COMMON/macros/loadSharedLibraries.C");
gROOT->LoadMacro("./loadSharedLibraries.C");
loadSharedLibraries();

TFile *f1 = new TFile(fileIn,"READ");
TTree *t1 = (TTree*)f1->Get("MuDst");
if (t1){

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
		if (strcmp(bbuf,"CovGlobTrack")==0){ t1->SetBranchStatus(bbuf2,kStatus); touched=true; } else
		if (strcmp(bbuf,"CovPrimTrack")==0){ t1->SetBranchStatus(bbuf2,kStatus); touched=true; } else
		if (strcmp(bbuf,"RichSpectra" )==0){ t1->SetBranchStatus(bbuf2,kStatus); touched=true; } else
		if (strcmp(bbuf,"McEvent"     )==0){ t1->SetBranchStatus(bbuf2,kStatus); touched=true; } else
		if (strcmp(bbuf,"pp2pp"       )==0){ t1->SetBranchStatus(bbuf2,kStatus); touched=true; } else 
		if (strcmp(bbuf,"StrangeCuts" )==0){ t1->SetBranchStatus(bbuf2,kStatus); touched=true; } else 
		if (strcmp(bbuf,"V0"          )==0){ t1->SetBranchStatus(bbuf2,kStatus); touched=true; } else 
		if (strcmp(bbuf,"McV0"        )==0){ t1->SetBranchStatus(bbuf2,kStatus); touched=true; } else 
		if (strcmp(bbuf,"V0Assoc"     )==0){ t1->SetBranchStatus(bbuf2,kStatus); touched=true; } else 
		if (strcmp(bbuf,"Xi"          )==0){ t1->SetBranchStatus(bbuf2,kStatus); touched=true; } else 
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
		if (strcmp(bbuf,"EmcTow"      )==0){ t1->SetBranchStatus(bbuf2,kStatus); touched=true; } else
		if (strcmp(bbuf,"EmcPrs"      )==0){ t1->SetBranchStatus(bbuf2,kStatus); touched=true; } else
		if (strcmp(bbuf,"EmcSmde"     )==0){ t1->SetBranchStatus(bbuf2,kStatus); touched=true; } else
		if (strcmp(bbuf,"EmcSmdp"     )==0){ t1->SetBranchStatus(bbuf2,kStatus); touched=true; } else
		if (strcmp(bbuf,"EEmcPrs"     )==0){ t1->SetBranchStatus(bbuf2,kStatus); touched=true; } else
		if (strcmp(bbuf,"EEmcSmdu"    )==0){ t1->SetBranchStatus(bbuf2,kStatus); touched=true; } else
		if (strcmp(bbuf,"EEmcSmdv"    )==0){ t1->SetBranchStatus(bbuf2,kStatus); touched=true; } 
		//
		//if (touched){
		//	cout<<i<<" \t "<<b->GetName()<<" \t "<<bbuf<<" \t "<<t1->GetBranchStatus(bbuf)<<" "<<"touched"<<endl;
		//} else {
		//	cout<<i<<" \t "<<b->GetName()<<" \t "<<bbuf<<" \t "<<t1->GetBranchStatus(bbuf)<<endl;
		//}
	}
	
	TFile *f2 = new TFile(fileOut,"RECREATE");
	//TTree *t2 = t1->CopyTree("MuEvent.mEventSummary.mNumberOfGoodPrimaryTracks[0]>0&&abs(Event.mPrimaryVertexZ[0])<50.&&BTofHeader.mVpdHitPattern[0][0]>0&&BTofHeader.mVpdHitPattern[0][1]>0");
	//TTree *t2 = t1->CopyTree("MuEvent.mEventSummary.mNumberOfGoodPrimaryTracks[0]>1&&abs(Event.mPrimaryVertexZ[0])<50.&&sqrt(Event.mPrimaryVertexX[0]*Event.mPrimaryVertexX[0]+Event.mPrimaryVertexY[0]*Event.mPrimaryVertexY[0])<2.0");
	//
	TTree *t2 = t1->CopyTree("MuEvent.mEventSummary.mNumberOfVertices[0]==1&&PrimaryVertices.mRanking[0]>-2&&MuEvent.mEventSummary.mNumberOfGoodPrimaryTracks[0]>1&&abs(Event.mPrimaryVertexZ[0])<50.&&(BTofHeader.mVpdHitPattern[0][0]>0&&BTofHeader.mVpdHitPattern[0][1]>0)");
	//TTree *t2 = t1->CopyTree("MuEvent.mEventSummary.mNumberOfGoodPrimaryTracks[0]>1&&abs(Event.mPrimaryVertexZ[0])<50.&&(BTofHeader.mVpdHitPattern[0][0]>0&&BTofHeader.mVpdHitPattern[0][1]>0)");
	t2->Write();
	//
} else {
	cout << endl;
	cout << endl;
	cout << "+--------------------------------+" << endl;
	cout << "|      NO KEYS IN THIS FILE      |" << endl;
	cout << "+--------------------------------+" << endl;
	cout << endl;
	cout << endl;
}

}
//---- end of macro....
