#include "noise_class.h"

noise::noise(TTree *treet, TTree *treem, Int_t krun, Int_t ntd)
{
	char	buf[120];
	kRunUse = krun;
	nToDo	= ntd;
	cout<<"..... noise::noise kRunUse = "<<kRunUse<<endl;
	cout<<"..... noise::noise nToDo   = "<<nToDo<<endl;

	sprintf(buf,"/gpfs01/star/subsysg/TOF/BTOF-online/dd/daqdoer_%d.root",kRunUse);

//	if (kRunUse != 14112036){
		finput	= new TFile(buf);
		cout<<"..... noise::noise Input = "<<buf<<endl;
		treet	= (TTree*)gDirectory->Get("ts");
		treem	= (TTree*)gDirectory->Get("tm");
//	} else {
//		TChain *chaint	= new TChain("ts","tof chain");
//			chaint->Add(Form("/gpfs01/star/subsysg/TOF/BTOF-online/dd/daqdoer_%d.root"  ,kRunUse));
// 			chaint->Add(Form("/gpfs01/star/subsysg/TOF/BTOF-online/dd/daqdoer_%d_1.root",kRunUse));
// 			chaint->Add(Form("/gpfs01/star/subsysg/TOF/BTOF-online/dd/daqdoer_%d_2.root",kRunUse));
// 			chaint->Add(Form("/gpfs01/star/subsysg/TOF/BTOF-online/dd/daqdoer_%d_3.root",kRunUse));
// 			chaint->Add(Form("/gpfs01/star/subsysg/TOF/BTOF-online/dd/daqdoer_%d_4.root",kRunUse));
//		TChain *chainm	= new TChain("tm","mtd chain");
//			chainm->Add(Form("/gpfs01/star/subsysg/TOF/BTOF-online/dd/daqdoer_%d.root"  ,kRunUse));
// 			chainm->Add(Form("/gpfs01/star/subsysg/TOF/BTOF-online/dd/daqdoer_%d_1.root",kRunUse));
// 			chainm->Add(Form("/gpfs01/star/subsysg/TOF/BTOF-online/dd/daqdoer_%d_2.root",kRunUse));
// 			chainm->Add(Form("/gpfs01/star/subsysg/TOF/BTOF-online/dd/daqdoer_%d_3.root",kRunUse));
// 			chainm->Add(Form("/gpfs01/star/subsysg/TOF/BTOF-online/dd/daqdoer_%d_4.root",kRunUse));
//		treet	= chaint;
//		treem	= chainm;
//	}
	Init(treet,treem);

	hnhitstof_time = (TH1D*)finput->Get("hnhitstof_time");
	if (hnhitstof_time){ hnhitstof_time->SetDirectory(0); }
	hnhitsmtd26_time = (TH1D*)finput->Get("hnhitsmtd26_time");
	if (hnhitsmtd26_time){ hnhitsmtd26_time->SetDirectory(0); }
	hnhitsmtd27_time = (TH1D*)finput->Get("hnhitsmtd27_time");
	if (hnhitsmtd27_time){ hnhitsmtd27_time->SetDirectory(0); }
	hnhitsmtd28_time = (TH1D*)finput->Get("hnhitsmtd28_time");
	if (hnhitsmtd28_time){ hnhitsmtd28_time->SetDirectory(0); }

}

noise::~noise()
{
	if (!fChaint) return;
	delete fChaint->GetCurrentFile();
}

Int_t noise::GetEntry(Long64_t entry)
{
	// Read contents of entry.
	if (!fChaint) return 0;
	return fChaint->GetEntry(entry);
}
Long64_t noise::LoadTree(Long64_t entry)
{
	if (!fChaint) return -5;
	if (!fChainm) return -5;

	Long64_t centryt = fChaint->LoadTree(entry);
	Long64_t centrym = fChainm->LoadTree(entry);

	if (centryt<0){ return centryt; }
	if (centrym<0){ return centryt; }

	if (!fChaint->InheritsFrom(TChain::Class())){ return centryt; }
	if (!fChainm->InheritsFrom(TChain::Class())){ return centrym; }

	TChain *chaint = (TChain*)fChaint;
	TChain *chainm = (TChain*)fChainm;
	if (chaint->GetTreeNumber() != fCurrentt) {
		fCurrentt = chaint->GetTreeNumber();
		Notify();
	}
	if (chainm->GetTreeNumber() != fCurrentm) {
		fCurrentm = chainm->GetTreeNumber();
		Notify();
	}
	return centryt;
}

void noise::Init(TTree *treet,TTree *treem)
{
	if (!treet){ cout<<"Init -- no treet"<<endl; return;}
	fChaint = treet;
	fCurrentt = -1;
	fChaint->SetMakeClass(1);

	if (!treem){ cout<<"Init -- no treem"<<endl; return;}
	fChainm = treem;
	fCurrentm = -1;
	fChainm->SetMakeClass(1);

// 	fChain->SetBranchStatus("*",0);
// 	fChain->SetBranchStatus("nTofHits",1);
// 	fChain->SetBranchStatus("nTofHitsLE",1);
// 	fChain->SetBranchStatus("nTofHitsTE",1);
// 	fChain->SetBranchStatus("tray",1);
// 	fChain->SetBranchStatus("module",1);
// 	fChain->SetBranchStatus("cell",1);
// 	fChain->SetBranchStatus("tle",1);
// 	fChain->SetBranchStatus("tte",1);
// //	fChain->SetBranchStatus("tot",1);
// 	//
// 	TIter 	iter(fChain->GetListOfBranches());
// 	char 	bname[40];
// 	while(TBranch *br = (TBranch *)iter.Next()) {
// 		strcpy(bname,br->GetName());
// 		//if (strcmp(bname,"Event")==0){ 			fChain->SetBranchStatus(br->GetName(),1); }
// 		//if (fChain->GetBranchStatus(br->GetName())){
// 			cout<<"Branch="<<bname<<"   Status="<<(int)fChain->GetBranchStatus(br->GetName())<<endl;
// 		//}
// 	}

	fChaint->SetBranchAddress("day", &day, &b_day);
	fChaint->SetBranchAddress("run", &run, &b_run);
	fChaint->SetBranchAddress("evp", &evp_evtnum, &b_evp);
	fChaint->SetBranchAddress("nTofHits", &nTofHits, &b_nTofHits);
	fChaint->SetBranchAddress("nTofHitsLE", &nTofHitsLE, &b_nTofHitsLE);
	fChaint->SetBranchAddress("nTofHitsTE", &nTofHitsTE, &b_nTofHitsTE);
	fChaint->SetBranchAddress("tray", tray, &b_tray);
	fChaint->SetBranchAddress("module", module, &b_module);
	fChaint->SetBranchAddress("cell", cell, &b_cell);
	fChaint->SetBranchAddress("coco", coco, &b_coco);
	fChaint->SetBranchAddress("tle", tle, &b_tle);
	fChaint->SetBranchAddress("tte", tte, &b_tte);
	fChaint->SetBranchAddress("tot", tot, &b_tot);

	fChainm->SetBranchAddress("nMtdHits", &nMtdHits, &b_nMtdHits);
	fChainm->SetBranchAddress("nMtdHitsLE", &nMtdHitsLE, &b_nMtdHitsLE);
	fChainm->SetBranchAddress("nMtdHitsTE", &nMtdHitsTE, &b_nMtdHitsTE);
	fChainm->SetBranchAddress("tray", mtray, &b_mtray);
	fChainm->SetBranchAddress("module", mmodule, &b_mmodule);
	fChainm->SetBranchAddress("modch", mmodch, &b_mmodch);
	fChainm->SetBranchAddress("cell", mcell, &b_mcell);
	fChainm->SetBranchAddress("strip", mstrip, &b_mstrip);
	fChainm->SetBranchAddress("strlr", mstrlr, &b_mstrlr);
	fChainm->SetBranchAddress("coco", mcoco, &b_mcoco);
	fChainm->SetBranchAddress("tle", mtle, &b_mtle);
	fChainm->SetBranchAddress("tte", mtte, &b_mtte);
	fChainm->SetBranchAddress("tot", mtot, &b_mtot);

	Notify();
}

Bool_t noise::Notify()
{
	cout<<fCurrentt<<" "<<fChaint->GetName()<<" "
		<<fCurrentm<<" "<<fChainm->GetName()<<" "
		<<endl;
	return kTRUE;
}

void noise::Show(Long64_t entry)
{
	if (!fChaint) return;
	if (!fChainm) return;
	fChaint->Show(entry);
	fChainm->Show(entry);
}
Int_t noise::Cut(Long64_t entry)
{
	return 1;
}
