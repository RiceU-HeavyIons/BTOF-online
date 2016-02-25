#include "cal_class.h"

//----------------------------------------------------------------------------------
//
cal::cal(Int_t irhicyear, Int_t iinput, Int_t ichoi, Int_t ialgo, Int_t iener, Int_t itrgcr, TTree *tree){

	iRhicRun	= irhicyear;
	iEnergy		= iener;
	iInputFlag	= iinput;
	iChoice		= ichoi;
	iAlgorithm	= ialgo;
	iTrgCrate	= itrgcr;
	TChain *chain;	
	Long_t	ifd=0,size=0,flags=0,mt=0;
	char 	buf[120];

	kChanKeyOffsets	= 0;

	const char *crates[2]	= {"bbq","mxq"};
	char ofb[200];
 	if (iRhicRun==14 && iEnergy==203){ sprintf(ofb,"cal_run14auhe200_ic%d_ia%d",iChoice,iAlgorithm); } else
 	if (iRhicRun==14 && iEnergy== 15){ sprintf(ofb,"cal_run14auau015_ic%d_ia%d",iChoice,iAlgorithm); } else
 	if (iRhicRun==14 && iEnergy==200){ sprintf(ofb,"cal_run14auau200_ic%d_ia%d",iChoice,iAlgorithm); } else
 	if (iRhicRun==13 && iEnergy==500){ sprintf(ofb,"cal_run13pp500_ic%d_ia%d"  ,iChoice,iAlgorithm); } else
 	if (iRhicRun==10 && iEnergy==200){ sprintf(ofb,"cal_run10auau200_ic%d_ia%d",iChoice,iAlgorithm); } else
 	if (iRhicRun==10 && iEnergy== 62){ sprintf(ofb,"cal_run10auau062_ic%d_ia%d",iChoice,iAlgorithm); } else
 	if (iRhicRun==10 && iEnergy== 39){ sprintf(ofb,"cal_run10auau039_ic%d_ia%d",iChoice,iAlgorithm); } else
 	if (iRhicRun==10 && iEnergy==  7){ sprintf(ofb,"cal_run10auau007_ic%d_ia%d",iChoice,iAlgorithm); } else
 	                                 { cout<<"cannot set outfilebase... run="<<iRhicRun<<" energy="<<iEnergy<<endl; exit(0); }
	TString ofbstr	 = TString(ofb);
	if (iChoice!=0){
		ofbstr		+= TString(Form("_%s",crates[iTrgCrate]));
	}
	cout<<"iChoice = "<<iChoice<<"   ofbstr      = "<<ofbstr.Data()<<endl;
	sprintf(outfilebase,"%s",ofbstr.Data());
	cout<<"iChoice = "<<iChoice<<"   outfilebase = "<<outfilebase<<endl;

	if (tree == 0) {	
		//
		chain = new TChain("tree","muapp chain");
		//
		//---- AuHe3 200 run-14
		if (iRhicRun==14 && iEnergy==203){
			cout <<"..... Using muapp input......... run-14 AuHe3 200"<<endl;
			iTreeStyle		=   1;
			iDayMin			= 171;
			iDayMax			= 172;
			refmultmax		= 200;
			itimemin		= 1403281802;
			chain->Add("/star/institutions/rice/geurts/muapp/run14/out_171.root/tree");
		//
		//---- AuAu 14.5 run-14
		} else if (iRhicRun==14 && iEnergy==15){
			cout <<"..... Using muapp input......... run-14 AuAu 14.5"<<endl;
			iTreeStyle		=   1;
			iDayMin			=  50;
			iDayMax			=  54;
			refmultmax		= 500;
			itimemin		= 1392794887;
			chain->Add("/star/institutions/rice/geurts/muapp/run14/out_050.root/tree");
			chain->Add("/star/institutions/rice/geurts/muapp/run14/out_052.root/tree");
			chain->Add("/star/institutions/rice/geurts/muapp/run14/out_053.root/tree");
			chain->Add("/star/institutions/rice/geurts/muapp/run14/out_054.root/tree");
		//
		} else if (iRhicRun==14 && iEnergy==200){
			cout <<"..... Using muapp input......... run-14 AuAu 200"<<endl;
			iTreeStyle		=   1;
			iDayMin			=  73;		//!!!!!
			iDayMax			=  74;		//!!!!!
			refmultmax		= 650;
			itimemin		= 1394831436;
			chain->Add("/star/institutions/rice/geurts/muapp/run14/out_073.root/tree");
		//
		//---- pp 500 run-13
		} else if (iRhicRun==13 && iEnergy==500){
			cout <<"..... Using muapp input......... run-13 pp 500"<<endl;
			iTreeStyle		=   1;
			iDayMin			=  83;
			iDayMax			=  85;
			refmultmax		=  40;
			itimemin		= 1364130027;		// day 83
			chain->Add("/star/institutions/rice/geurts/muapp/run13/out_083.root/tree");
 			chain->Add("/star/institutions/rice/geurts/muapp/run13/out_084.root/tree");
 			chain->Add("/star/institutions/rice/geurts/muapp/run13/out_085.root/tree");
		//
 		//
 		//----------- BELOW HERE HAS NO MXQ DATA (mvpd_adc, mvpd_tac)........................................
 		//
		//---- AuAu 200 run-10
		} else if (iRhicRun==10 && iEnergy==200){
			cout <<"..... Using muapp input......... run-10 AuAu 200"<<endl;
			iTreeStyle	=   1;
			iDayMin		=   7;
			iDayMax		=  25;
			refmultmax	= 650;
			itimemin	= 1262859556;
			kChanKeyOffsets	= 0;
//----------
// 			chain->Add("/star/institutions/rice/geurts/muapp/run10/out_6.root/tree");
//----------
			chain->Add("/star/institutions/rice/geurts/muapp/run10/out_7.root/tree");
			chain->Add("/star/institutions/rice/geurts/muapp/run10/out_8.root/tree");
			chain->Add("/star/institutions/rice/geurts/muapp/run10/out_9.root/tree");
			chain->Add("/star/institutions/rice/geurts/muapp/run10/out_10.root/tree");
			chain->Add("/star/institutions/rice/geurts/muapp/run10/out_11.root/tree");
			chain->Add("/star/institutions/rice/geurts/muapp/run10/out_12.root/tree");
			chain->Add("/star/institutions/rice/geurts/muapp/run10/out_14.root/tree");
			chain->Add("/star/institutions/rice/geurts/muapp/run10/out_18.root/tree");
			chain->Add("/star/institutions/rice/geurts/muapp/run10/out_19.root/tree");
			chain->Add("/star/institutions/rice/geurts/muapp/run10/out_20.root/tree");
			chain->Add("/star/institutions/rice/geurts/muapp/run10/out_23.root/tree");
			chain->Add("/star/institutions/rice/geurts/muapp/run10/out_25.root/tree");
//----------
//			chain->Add("/star/institutions/rice/geurts/muapp/run10/out_28.root/tree");
//			chain->Add("/star/institutions/rice/geurts/muapp/run10/out_37.root/tree");
//			chain->Add("/star/institutions/rice/geurts/muapp/run10/out_40.root/tree");
// 			chain->Add("/star/institutions/rice/geurts/muapp/run10/out_45.root/tree");
// 			chain->Add("/star/institutions/rice/geurts/muapp/run10/out_50.root/tree");
// 			chain->Add("/star/institutions/rice/geurts/muapp/run10/out_64.root/tree");
// 			chain->Add("/star/institutions/rice/geurts/muapp/run10/out_74.root/tree");
		//
		//---- auau 62 gev run-10
 		} else if (iRhicRun==10 && iEnergy==62){
 			cout <<"..... Using muapp input......... run-10 auau 62 GeV"<<endl;
 			iTreeStyle	=   1;
 			iDayMin		=  80;
 			iDayMax		=  91;
 			refmultmax	= 500;
 			itimemin	= 1269144285;
 			chain->Add("/star/institutions/rice/geurts/muapp/run10/out_80.root/tree");		
 			chain->Add("/star/institutions/rice/geurts/muapp/run10/out_81.root/tree");		
 			chain->Add("/star/institutions/rice/geurts/muapp/run10/out_82.root/tree");		
 			chain->Add("/star/institutions/rice/geurts/muapp/run10/out_84.root/tree");		
 			chain->Add("/star/institutions/rice/geurts/muapp/run10/out_85.root/tree");		
 			chain->Add("/star/institutions/rice/geurts/muapp/run10/out_86.root/tree");		
 			chain->Add("/star/institutions/rice/geurts/muapp/run10/out_87.root/tree");		
 			chain->Add("/star/institutions/rice/geurts/muapp/run10/out_88.root/tree");		
 			chain->Add("/star/institutions/rice/geurts/muapp/run10/out_89.root/tree");		
 			chain->Add("/star/institutions/rice/geurts/muapp/run10/out_91.root/tree");		
		//
		//---- auau 39 gev run-10
 		} else if (iRhicRun==10 && iEnergy==39){
 			cout <<"..... Using muapp input......... run-10 auau 39 GeV"<<endl;
 			iTreeStyle	=   1;
 			iDayMin		=  98;
 			iDayMax		= 112;
 			refmultmax	= 350;
 			itimemin	= 1270700413;
 			//chain->Add("/star/institutions/rice/geurts/muapp/run10/out_98.root/tree");		
 			chain->Add("/star/institutions/rice/geurts/muapp/run10/out_99.root/tree");		
 			chain->Add("/star/institutions/rice/geurts/muapp/run10/out_100.root/tree");		
 			chain->Add("/star/institutions/rice/geurts/muapp/run10/out_101.root/tree");		
 			chain->Add("/star/institutions/rice/geurts/muapp/run10/out_102.root/tree");		
 			chain->Add("/star/institutions/rice/geurts/muapp/run10/out_103.root/tree");		
 			chain->Add("/star/institutions/rice/geurts/muapp/run10/out_104.root/tree");		
 			chain->Add("/star/institutions/rice/geurts/muapp/run10/out_105.root/tree");		
 			chain->Add("/star/institutions/rice/geurts/muapp/run10/out_106.root/tree");		
 			chain->Add("/star/institutions/rice/geurts/muapp/run10/out_107.root/tree");		
 			chain->Add("/star/institutions/rice/geurts/muapp/run10/out_108.root/tree");		
 			chain->Add("/star/institutions/rice/geurts/muapp/run10/out_109.root/tree");		
 			chain->Add("/star/institutions/rice/geurts/muapp/run10/out_110.root/tree");		
 			chain->Add("/star/institutions/rice/geurts/muapp/run10/out_111.root/tree");		
 			chain->Add("/star/institutions/rice/geurts/muapp/run10/out_112.root/tree");		
		//
 		} else if (iRhicRun==10 && iEnergy==7){
 			cout <<"..... Using muapp input......... run-10 auau 7 GeV"<<endl;
 			iTreeStyle		=   1;
 			iDayMin			= 114;
 			iDayMax			= 132;
 			refmultmax		= 300;
 			itimemin		= 1272144119;
 			chain->Add("/star/institutions/rice/geurts/muapp/run10/out_114.root/tree");		
 			chain->Add("/star/institutions/rice/geurts/muapp/run10/out_115.root/tree");		
 			chain->Add("/star/institutions/rice/geurts/muapp/run10/out_116.root/tree");		
 			chain->Add("/star/institutions/rice/geurts/muapp/run10/out_117.root/tree");		
 			chain->Add("/star/institutions/rice/geurts/muapp/run10/out_118.root/tree");		
 			chain->Add("/star/institutions/rice/geurts/muapp/run10/out_119.root/tree");		
 			chain->Add("/star/institutions/rice/geurts/muapp/run10/out_120.root/tree");		
 			chain->Add("/star/institutions/rice/geurts/muapp/run10/out_121.root/tree");		
 			chain->Add("/star/institutions/rice/geurts/muapp/run10/out_122.root/tree");		
 			chain->Add("/star/institutions/rice/geurts/muapp/run10/out_123.root/tree");		
 			chain->Add("/star/institutions/rice/geurts/muapp/run10/out_124.root/tree");		
 			chain->Add("/star/institutions/rice/geurts/muapp/run10/out_125.root/tree");		
 			chain->Add("/star/institutions/rice/geurts/muapp/run10/out_126.root/tree");		
 			chain->Add("/star/institutions/rice/geurts/muapp/run10/out_127.root/tree");		
 			chain->Add("/star/institutions/rice/geurts/muapp/run10/out_128.root/tree");		
 			chain->Add("/star/institutions/rice/geurts/muapp/run10/out_129.root/tree");		
 			chain->Add("/star/institutions/rice/geurts/muapp/run10/out_130.root/tree");		
 			chain->Add("/star/institutions/rice/geurts/muapp/run10/out_131.root/tree");		
 			chain->Add("/star/institutions/rice/geurts/muapp/run10/out_132.root/tree");	
 		}	
 		//
		//
		tree = chain;
		//cout<<chain<<" "<<tree<<endl;
		//tree->Print();
		//
	}	// end check tree=0
	Init(tree);

	nentriestree = fChain->GetEntries();	
	cout<<"..... Nevents    = "<<nentriestree<<endl;
	cout<<"..... iRhicRun   = "<<iRhicRun<<endl;
	cout<<"..... iEnergy    = "<<iEnergy<<endl;
	cout<<"..... iInputFlag = "<<iInputFlag<<endl;
	cout<<"..... iTreeStyle = "<<iTreeStyle<<endl;
	cout<<"..... iDayMin    = "<<iDayMin<<endl;
	cout<<"..... iDayMax    = "<<iDayMax<<endl;
	cout<<"..... itimemin   = "<<itimemin<<endl;
	cout<<"..... iTrgCrate  = "<<iTrgCrate<<endl;
	

}

//----------------------------------------------------------------------------------
//
cal::~cal()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

//----------------------------------------------------------------------------------
//
Int_t cal::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t cal::LoadTree(Long64_t entry)
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

void cal::Init(TTree *tree)
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

#ifdef muapp
	cout <<"..... Mapping muapp branches (fastoffline files)...."<<endl;
	fChain->SetBranchAddress("day", &day, &b_day);
	fChain->SetBranchAddress("run", &run, &b_run);
	fChain->SetBranchAddress("daqbits", &daqbits, &b_daqbits);
	fChain->SetBranchAddress("vpd_adc", vpd_adc, &b_vpd_adc);
	fChain->SetBranchAddress("vpd_tac", vpd_tac, &b_vpd_tac);
	fChain->SetBranchAddress("mvpd_adc",mvpd_adc,&b_mvpd_adc);
	fChain->SetBranchAddress("mvpd_tac",mvpd_tac,&b_mvpd_tac);
//	fChain->SetBranchAddress("p2p_adc", p2p_adc, &b_p2p_adc);
//	fChain->SetBranchAddress("p2p_tac", p2p_tac, &b_p2p_tac);
	fChain->SetBranchAddress("bbc_adc", bbc_adc, &b_bbc_adc);
	fChain->SetBranchAddress("bbc_tac", bbc_tac, &b_bbc_tac);
	fChain->SetBranchAddress("vpd_cco", vpd_cco, &b_vpd_cco);
	fChain->SetBranchAddress("vpd_tle", vpd_tle, &b_vpd_tle);
	fChain->SetBranchAddress("vpd_tot", vpd_tot, &b_vpd_tot);
//	fChain->SetBranchAddress("p2p_cco", p2p_cco, &b_p2p_cco);
//	fChain->SetBranchAddress("p2p_tle", p2p_tle, &b_p2p_tle);
//	fChain->SetBranchAddress("p2p_tot", p2p_tot, &b_p2p_tot);
	fChain->SetBranchAddress("itime", &itime, &b_itime);
	fChain->SetBranchAddress("evtid", &evtid, &b_evtid);
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
#else
	cout <<"..... Mapping daqdoer branches (daq files)........."<<endl;
	fChain->SetBranchAddress("day", &day, &b_day);
	fChain->SetBranchAddress("run", &run, &b_run);
	fChain->SetBranchAddress("daqbits", &daqbits, &b_daqbits);
	fChain->SetBranchAddress("vpd_cco", vpd_cco, &b_vpd_cco);
	fChain->SetBranchAddress("vpd_tle", vpd_tle, &b_vpd_tle);
	fChain->SetBranchAddress("vpd_tot", vpd_tot, &b_vpd_tot);
	fChain->SetBranchAddress("vpd_tac", vpd_tac, &b_vpd_tac);
	fChain->SetBranchAddress("vpd_adc", vpd_adc, &b_vpd_adc);
//	fChain->SetBranchAddress("p2p_cco", p2p_cco, &b_p2p_cco);
//	fChain->SetBranchAddress("p2p_tle", p2p_tle, &b_p2p_tle);
//	fChain->SetBranchAddress("p2p_tot", p2p_tot, &b_p2p_tot);
//	fChain->SetBranchAddress("p2p_tac", p2p_tac, &b_p2p_tac);
//	fChain->SetBranchAddress("p2p_adc", p2p_adc, &b_p2p_adc);
#endif

//	fChain->SetBranchStatus("*",0);
	cout<<"..... List of available branches...................."<<endl;
	TIter 	iter(fChain->GetListOfBranches());
	char 	bname[40];
	while(TBranch *br = (TBranch *)iter.Next()) {
		strcpy(bname,br->GetName());
//		//if (strcmp(bname,"Event")==0){ 			fChain->SetBranchStatus(br->GetName(),1); } 
//		if (strcmp(bname,"MuEvent")==0){ 			fChain->SetBranchStatus(br->GetName(),1); } 
//		//if (strcmp(bname,"PrimaryVertices")==0){	fChain->SetBranchStatus(br->GetName(),1); } 
//		//if (strcmp(bname,"PrimaryTracks")==0){ 	fChain->SetBranchStatus(br->GetName(),1); } 
//		//if (strcmp(bname,"GlobalTracks")==0){ 	fChain->SetBranchStatus(br->GetName(),1); } 
//		if (strcmp(bname,"BTofHeader")==0){ 		fChain->SetBranchStatus(br->GetName(),1); } 
//		if (strcmp(bname,"BTofRawHit")==0){ 		fChain->SetBranchStatus(br->GetName(),1); } 
//		if (fChain->GetBranchStatus(br->GetName())){
//			cout<<"ACTIVATING "<<bname<<" "<<(int)fChain->GetBranchStatus(br->GetName())<<endl;
//		}
		cout<<bname<<" ";
	}	cout<<endl;

   Notify();
}

//----------------------------------------------------------------------------------
//
Bool_t cal::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normaly not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

	//cout<<"New File ...."<<fCurrent<<endl;
	if (fCurrent>=0 && fChain){
   		cout<<"... New File    "<<fCurrent<<" ... "
   			<<fChain->GetCurrentFile()->GetName()<<" ..............."<<endl;
   	}
	return kTRUE;
}

//----------------------------------------------------------------------------------
//
void cal::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}

//----------------------------------------------------------------------------------
//
Int_t cal::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
