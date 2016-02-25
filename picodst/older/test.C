#include <iostream>
#include <stdlib.h>
using namespace std;

const int NSETS = 4;

void cp_myfo(char dirIn[200], char dirOut[200]){

char buf[100];
cout<<dirIn<<endl;
cout<<dirOut<<endl;
cout<<"..................................................................."<<endl;

TString sdirIn	= TString(dirIn);
TString sdirOut	= TString(dirOut);
TString subdirs[NSETS];
//TString thissubdir;
const char* nextFileName;
TString thisFile;
int nfiles_subdir[NSETS] = {0};
int runnums_subdir[100][NSETS];
int fileids_subdir[100][NSETS];
int thisrunnum,thisfileid;
Long_t pathid, pathsize, pathflag, pathmodtime; 

//---- loop over entire directory tree and collect runnum and fileid info from subdirs...
//
for (int iset=0;iset<NSETS;iset++){
	sprintf(buf,"/2k0%d/",iset+1);
	subdirs[iset]		= TString(buf);
	TString thissubdir	= sdirIn+subdirs[iset];
	cout<<thissubdir.Data()<<endl;	
	//
	void *dirp = gSystem->OpenDirectory(thissubdir.Data());
	while(nextFileName=gSystem->GetDirEntry(dirp)){ 
		thisFile = nextFileName;
		if (thisFile.EndsWith("root") ){
			//
			int k = nfiles_subdir[iset];
			sscanf(nextFileName,"st_physics_%d_raw_%d.",&thisrunnum,&thisfileid);
			runnums_subdir[k][iset]	= thisrunnum;
			fileids_subdir[k][iset]	= thisfileid;
			//cout<<nextFileName<<" "<<runnums_subdir[k][iset]<<" "<<fileids_subdir[k][iset]<<endl;
			++nfiles_subdir[iset];
			//
		}
	}
	gSystem->FreeDirectory(dirp);
	//
	cout<<"....nfiles = "<<nfiles_subdir[iset]<<endl;
}
//
//---- build list of runnum&fileid pairs seen....
//
int npairs	= 0;
int runnums_use[100] = {0};
int fileids_use[100] = {0};
int kpair	= 0;
//
for (int iset=0;iset<NSETS;iset++){
	int nf		= nfiles_subdir[iset];
	for (int if=0;if<nf;if++){
		int ir	= runnums_subdir[if][iset];
		int id	= fileids_subdir[if][iset];
		if (npairs==0){						// first entry
				kpair	= npairs;
				runnums_use[kpair]	= ir;
				fileids_use[kpair]	= id;
				++npairs;
		} else if (npairs>0){				// loop over seen pairs...
			bool seen = false;
			for (int ip=0;ip<npairs;ip++){
				int irt	= runnums_use[ip];				
				int idt	= fileids_use[ip];				
				if (irt==ir && idt==id){
					seen = true;
				}
			}
			if (!seen){
				kpair	= npairs;
				runnums_use[kpair]	= ir;
				fileids_use[kpair]	= id;
				++npairs;
			}
		}
	}
}
//
//---- calculate mask for each pair....
//
int dirmask_use[100]	= {0};
for (int ip=0;ip<npairs;ip++){
	//cout<<ip<<" "<<runnums_use[ip]<<" "<<fileids_use[ip]<<endl;
	//
	for (int iset=0;iset<NSETS;iset++){
	//
		sprintf(buf,"/2k0%d/",iset+1);
		//subdirs[iset]	= TString(buf);
		TString thissubdir	= sdirIn+subdirs[iset];
		//cout<<thissubdir.Data()<<endl;	
		//
		if (fileids_use[ip]>=1000000){
 			sprintf(buf,"st_physics_%d_raw_%d.2k0%d.MuDst.root",runnums_use[ip],fileids_use[ip],iset+1);
		} else if (fileids_use[ip]>=100000){
 			sprintf(buf,"st_physics_%d_raw_0%d.2k0%d.MuDst.root",runnums_use[ip],fileids_use[ip],iset+1);
		} else if (fileids_use[ip]>=10000){
 			sprintf(buf,"st_physics_%d_raw_00%d.2k0%d.MuDst.root",runnums_use[ip],fileids_use[ip],iset+1);
		}	
 		thisFile	= thissubdir+TString(buf);
 		//
 		//cout<<thisFile.Data()<<" ";	
		pathsize	= 0;
		int info = gSystem->GetPathInfo(thisFile.Data(), &pathid, &pathsize, &pathflag, &pathmodtime);
		if (pathsize > 0){		
			//cout<<"exists... "<<pathsize<<endl;
			dirmask_use[ip]	+= TMath::Power(2,iset);
		} else {
			//cout<<"does not exist... "<<pathsize<<endl;
		}
	}	//---- end sets loop
}	//---- end pairs loop

for (int ip=0;ip<npairs;ip++){
	cout<<ip<<" "<<runnums_use[ip]<<" "<<fileids_use[ip]<<" "<<dirmask_use[ip]<<endl;
}

//---- O.K.................................................................................
//---- O.K.................................................................................
//---- O.K.................................................................................

if (gClassTable->GetID("TTable") < 0) {
	gSystem->Load("libStar");
	gSystem->Load("libPhysics");
} 
gROOT->LoadMacro("$STAR/StRoot/StMuDSTMaker/COMMON/macros/loadSharedLibraries.C");
loadSharedLibraries();

for (int ip=0;ip<npairs;ip++){
	//
	if (fileids_use[ip]>=1000000){
		sprintf(buf,"st_physics_%d_raw_%d.MuDst.root",runnums_use[ip],fileids_use[ip]);
	} else if (fileids_use[ip]>=100000){
		sprintf(buf,"st_physics_%d_raw_0%d.MuDst.root",runnums_use[ip],fileids_use[ip]);
	} else if (fileids_use[ip]>=10000){
		sprintf(buf,"st_physics_%d_raw_00%d.MuDst.root",runnums_use[ip],fileids_use[ip]);
	}	
	TString fileOut	= TString(dirOut)+TString(buf);
	cout<<ip<<" "<<runnums_use[ip]<<" "<<fileids_use[ip]<<" "<<dirmask_use[ip]<<" "<<fileOut.Data()<<endl;
	//
	TChain *chain	= new TChain("MuDst","my chain");
	TFile *f2 		= new TFile(fileOut,"RECREATE");
	TTree *t1;
	TTree *t2;
	//
	for (iset=0;iset<NSETS;iset++){
		if(dirmask_use[ip]&(1<<iset)){
			//
			TString fileIn;
			sprintf(buf,"/2k0%d/",iset+1);
			TString thissubdir	= sdirIn+subdirs[iset];
			//
			if (fileids_use[ip]>=1000000){
				sprintf(buf,"st_physics_%d_raw_%d.2k0%d.MuDst.root",runnums_use[ip],fileids_use[ip],iset+1);
			} else if (fileids_use[ip]>=100000){
				sprintf(buf,"st_physics_%d_raw_0%d.2k0%d.MuDst.root",runnums_use[ip],fileids_use[ip],iset+1);
			} else if (fileids_use[ip]>=10000){
				sprintf(buf,"st_physics_%d_raw_00%d.2k0%d.MuDst.root",runnums_use[ip],fileids_use[ip],iset+1);
			}	
			fileIn	= thissubdir+TString(buf);
			cout<<"..... adding file "<<fileIn.Data()<<endl;
			//
			chain->Add(fileIn.Data());
		}
	}
	//
	t1	= chain;
	cout<<"..... chain Nentries = "<<t1->GetEntries()<<endl;
	//
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
			//	if (touched){
			//		cout<<i<<" "<<b->GetName()<<" "<<bbuf<<"\t "<<t1->GetBranchStatus(bbuf)<<" "<<"touched"<<endl;
			//	} else {
			//		cout<<i<<" "<<b->GetName()<<" "<<bbuf<<"\t "<<t1->GetBranchStatus(bbuf)<<endl;
			//	}
			//
		}
		//
		//TTree *t2 = t1->CopyTree("MuEvent.mEventSummary.mNumberOfGoodPrimaryTracks[0]>0&&abs(Event.mPrimaryVertexZ[0])<70.&&BTofHeader.mVpdHitPattern[0]>0&&BTofHeader.mVpdHitPattern[1]>0");
		//TTree *t2 = t1->CopyTree("MuEvent.mEventSummary.mNumberOfGoodPrimaryTracks[0]>0&&abs(Event.mPrimaryVertexZ[0])<50.&&BTofHeader.mVpdHitPattern[0][0]>0&&BTofHeader.mVpdHitPattern[0][1]>0");
		t2 = (TTree*)t1->CopyTree("MuEvent.mEventSummary.mNumberOfGoodPrimaryTracks[0]>1&&abs(Event.mPrimaryVertexZ[0])<50.&&sqrt(Event.mPrimaryVertexX[0]*Event.mPrimaryVertexX[0]+Event.mPrimaryVertexY[0]*Event.mPrimaryVertexY[0])<2.0");
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
	//
	t2->Write();
	f2->Close();
	//
}	//---- end loop over pairs

}
//---- end of macro....
