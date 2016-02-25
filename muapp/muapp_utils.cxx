
#include "muapp.h"

muapp::muapp(TTree *tree, Int_t mrun, Int_t myear, Int_t mday)
{
	//int n	= kBigNumber;
	int nf	= 0;

	char buf[200];
	kRhicRun	= mrun; 
	kYear		= myear;
	kDay		= mday;
	if (kDay==0){ kDay=2; }

	if (tree == 0) {

		char infilename[120];
		char thisdir[120];
		TString path,dir,file,fullpath;
		
		TChain *chain = new TChain("MuDst","chain");
 		
 		iChoice	= -1;
 		int NDISK	= 1;
 		char dirstring1[100];
 		char dirstring2[100];
// 		cout<<" Input? "<<endl;
// 		cout<<"\t  = 0 for data9,10,12 fastoffline...."<<endl;
//		cout<<"\t  = 1 for rice disk fastoffline... (mudst dir)"<<endl; 
//		cout<<"\t  = 2 for starscatch fastoffline (mudst/run10/039/).."<<endl; 
//		cout<<"\t  = 3 for rice disk fastoffline st_physics... (pdst dir, run10 7.7GeV only)"<<endl; 
//		cout<<"\t  = 4 for rice disk fastoffline st_monitor... (pdst dir, run10 7.7GeV only)"<<endl; 
//		cout<<"\t  = 5 for data01/pwg disk fastoffline... (mudst dir)"<<endl; 
//		cout<<"\t  = 6 for data02/pwg disk fastoffline... (mudst dir)"<<endl; 
//		cout<<"\t  = 7 for data05 fastoffline (mudst/run10/039/).."<<endl; 
// 		cout<<"\t  = 8 for star/data11/reco/AuAu62_production/ReversedFullField/P10ik_calib/2010...."<<endl;
// 		cout<<"\t  = 9 for rice disk pdst run-11...."<<endl;
// 		cout<<"\t  =10 for rice disk pdst run-10...."<<endl;
// 		cout<<"\t  =11 for rice disk MuDst run-11/auau200...."<<endl;
// 		cout<<"\t  =12 for rice disk pdst run-12...."<<endl;
// 		cout<<"\t  =13 for rice disk pdst run-13...."<<endl;
// 		cout<<"\t  =14 for rice disk pdst run-14...."<<endl;
// 		cin>>iChoice;

		iChoice	= kRhicRun;								// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		TString rconfig	= TString("billSlewingTest");	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

		if (kRhicRun>15000000){
			nf		= 1;
			//sprintf(buf,"/star/u/geurts/run14/mudst/st_*_%d_raw_*.MuDst.root",kRhicRun);
			//sprintf(buf,"/star/institutions/rice/geurts/pdst/run14/mudst/st_*_%d_raw_*.MuDst.root",kRhicRun);
			//cout<<buf<<endl;
			//
			int kPeriodTemp	= kRhicRun/1000000;
			int kDayTemp	= (kRhicRun - kPeriodTemp*1000000)/1000;
			kDay			= kDayTemp;
			//
//			sprintf(buf,"/star/data09/reco/AuAu_200_production_2014/ReversedFullField/dev/2014/075/15075094/st_*_%d_*.MuDst.root",kRhicRun);
//			cout<<"...Adding "<<buf<<endl;
//			chain->Add(buf);	
//			sprintf(buf,"/star/data10/reco/AuAu_200_production_2014/ReversedFullField/dev/2014/075/15075094/st_*_%d_*.MuDst.root",kRhicRun);
//			cout<<"...Adding "<<buf<<endl;
//			chain->Add(buf);	
//			sprintf(buf,"/star/data11/reco/AuAu_200_production_2014/ReversedFullField/dev/2014/075/15075094/st_*_%d_*.MuDst.root",kRhicRun);
//			cout<<"...Adding "<<buf<<endl;
//			chain->Add(buf);	
//			sprintf(buf,"/star/data12/reco/AuAu_200_production_2014/ReversedFullField/dev/2014/075/15075094/st_*_%d_*.MuDst.root",kRhicRun);
//			cout<<"...Adding "<<buf<<endl;
//			chain->Add(buf);	
			//chain->Print();
			//
			sprintf(buf,"/star/data09/reco/%s/ReversedFullField/dev/2014/%d/%d/st_*_%d_*.MuDst.root",rconfig.Data(),kDayTemp,kRhicRun,kRhicRun);
			cout<<"...Adding "<<buf<<endl;
			chain->Add(buf);	
			sprintf(buf,"/star/data10/reco/%s/ReversedFullField/dev/2014/%d/%d/st_*_%d_*.MuDst.root",rconfig.Data(),kDayTemp,kRhicRun,kRhicRun);
			cout<<"...Adding "<<buf<<endl;
			chain->Add(buf);	
			sprintf(buf,"/star/data11/reco/%s/ReversedFullField/dev/2014/%d/%d/st_*_%d_*.MuDst.root",rconfig.Data(),kDayTemp,kRhicRun,kRhicRun);
			cout<<"...Adding "<<buf<<endl;
			chain->Add(buf);	
			sprintf(buf,"/star/data12/reco/%s/ReversedFullField/dev/2014/%d/%d/st_*_%d_*.MuDst.root",rconfig.Data(),kDayTemp,kRhicRun,kRhicRun);
			cout<<"...Adding "<<buf<<endl;
			chain->Add(buf);	
			//chain->Print();
			//
			kRhicRun		= kPeriodTemp - 1;
			//
		} else if (iChoice==14){	
			if (kDay<10){	
				sprintf(thisdir,"/star/institutions/rice/geurts/pdst/run14/mudst/00%d/",kDay);
			} else if (kDay<100){	
				sprintf(thisdir,"/star/institutions/rice/geurts/pdst/run14/mudst/0%d/",kDay);
			} else if (kDay<1000){	
				sprintf(thisdir,"/star/institutions/rice/geurts/pdst/run14/mudst/%d/",kDay);
			}
			fullpath	= TString(thisdir) + TString("*.MuDst.root");
			chain->Add(fullpath);
			nf = 1;
		}

		cout<<"\t kRhicRun = "<<kRhicRun<<endl;
		cout<<"\t iChoice  = "<<iChoice<<endl;
		cout<<"\t kDay     = "<<kDay<<endl;

// 		if (iChoice==0){
// 			NDISK	= 3;	
// 		} else if (iChoice== 1){
// 			NDISK	= 1;	
// 		} else if (iChoice== 2){
// 			NDISK	= 1;			
// 		} else if (iChoice== 3){
// 			NDISK	= 1;			
// 		} else if (iChoice== 4){
// 			NDISK	= 1;			
// 		} else if (iChoice== 5){
// 			NDISK	= 1;			
// 		} else if (iChoice== 6){
// 			NDISK	= 1;			
// 		} else if (iChoice== 7){
// 			NDISK	= 1;			
// 		} else if (iChoice== 8){
// 			NDISK	= 1;			
// 		} else if (iChoice== 9){
// 			NDISK	= 1;			
// 		} else if (iChoice==10){
// 			NDISK	= 1;			
// 		} else if (iChoice==11){
// 			NDISK	= 1;			
// 		} else if (iChoice==12){
// 			NDISK	= 1;			
// 		} else if (iChoice==13){
// 			NDISK	= 1;			
// 		} else if (iChoice==14){
// 			NDISK	= 1;			
// 		}
//		cout<<"\t NDISK    = "<<NDISK<<endl;
 		
//		int nAdded	= 0;
//  		if (kDay>=3){
//  			for (int idisk=0;idisk<NDISK;idisk++){
//   				if (kDay<10){
// //xx	 				if (idisk==0){ sprintf(thisdir,"/star/data09/reco/AuAu200_production/FullField/dev/2010/00%d/",kDay); }else
// //xx	 				if (idisk==1){ sprintf(thisdir,"/star/data10/reco/AuAu200_production/FullField/dev/2010/00%d/",kDay); } 	
// 					if (iChoice==0){
// //		 				if (idisk==0){ sprintf(thisdir,"/star/data09/reco/AuAu39_production/ReversedFullField/dev/2010/00%d/",kDay); }else
// //		 				if (idisk==1){ sprintf(thisdir,"/star/data10/reco/AuAu39_production/ReversedFullField/dev/2010/00%d/",kDay); } 
// //		 				if (idisk==0){ sprintf(thisdir,"/star/data09/reco/AuAu7_production/ReversedFullField/dev/2010/00%d/",kDay); }else
// //		 				if (idisk==1){ sprintf(thisdir,"/star/data10/reco/AuAu7_production/ReversedFullField/dev/2010/00%d/",kDay); }else
// //		 				if (idisk==2){ sprintf(thisdir,"/star/data12/reco/AuAu7_production/ReversedFullField/dev/2010/00%d/",kDay); } 
// 		 				if (idisk==0){ sprintf(thisdir,"/star/data09/reco/pp500_production_2011/ReversedFullField/dev/2011/00%d/",kDay); }else
// 		 				if (idisk==1){ sprintf(thisdir,"/star/data10/reco/pp500_production_2011/ReversedFullField/dev/2011/00%d/",kDay); }else
// 		 				if (idisk==2){ sprintf(thisdir,"/star/data12/reco/pp500_production_2011/ReversedFullField/dev/2011/00%d/",kDay); } 
// 		 			} else if (iChoice==1){
// 		 				if (idisk==0){ sprintf(thisdir,"/star/institutions/rice/geurts/mudst/run10/200/00%d/",kDay); }
// 		 			} else if (iChoice==2){
// 		 				if (idisk==0){ sprintf(thisdir,"/star/scratch/geurts/mudst/run10/039/00%d/",kDay); }
// 		 			} else if (iChoice==3||iChoice==4){
// 		 				if (idisk==0){ sprintf(thisdir,"/star/institutions/rice/geurts/pdst/00%d/",kDay); }
// 		 			} else if (iChoice==5){
// 		 				if (idisk==0){ sprintf(thisdir,"/star/data01/pwg/geurts/mudst/run10/039/00%d/",kDay); }
// 		 			} else if (iChoice==6){
// 		 				if (idisk==0){ sprintf(thisdir,"/star/data02/pwg/geurts/mudst/run10/039/00%d/",kDay); }
// 		 			} else if (iChoice==7){
// 		 				if (idisk==0){ sprintf(thisdir,"/star/data05/scratch/geurts/mudst/run10/039/00%d/",kDay); }
// 		 			} else if (iChoice==8){
// 		 				if (idisk==0){ sprintf(thisdir,"/star/data11/reco/AuAu62_production/ReversedFullField/P10ik_calib/2010/00%d/",kDay); }
// 		 			} else if (iChoice==9){
// 		 				if (idisk==0){ sprintf(thisdir,"/star/institutions/rice/geurts/pdst/run11/00%d/",kDay); }
// 		 			} else if (iChoice==10){
// 		 				if (idisk==0){ sprintf(thisdir,"/star/institutions/rice/geurts/pdst/run10/00%d/",kDay); }
// 		 			} else if (iChoice==11){
// 		 				if (idisk==0){ sprintf(thisdir,"/star/institutions/rice/geurts/muapp/run11/auau200/00%d/",kDay); }
// 		 			} else if (iChoice==12){
// 		 				if (idisk==0){ sprintf(thisdir,"/star/institutions/rice/geurts/pdst/run12/00%d/",kDay); }
// 		 			} else if (iChoice==13){
// 		 				if (idisk==0){ sprintf(thisdir,"/star/institutions/rice/geurts/pdst/run13/00%d/",kDay); }
// 		 			} else if (iChoice==14){
// 		 				if (idisk==0){ sprintf(thisdir,"/star/institutions/rice/geurts/pdst/run14/00%d/",kDay); }
// 		 			}
// 	 			} else 
//  				if (kDay<100){
// //xx	 				if (idisk==0){ sprintf(thisdir,"/star/data09/reco/AuAu200_production/FullField/dev/2010/0%d/",kDay); }else
// //xx	 				if (idisk==1){ sprintf(thisdir,"/star/data10/reco/AuAu200_production/FullField/dev/2010/0%d/",kDay); } 				
// 					if (iChoice==0){
// //		 				if (idisk==0){ sprintf(thisdir,"/star/data09/reco/AuAu39_production/ReversedFullField/dev/2010/0%d/",kDay); }else
// //		 				if (idisk==1){ sprintf(thisdir,"/star/data10/reco/AuAu39_production/ReversedFullField/dev/2010/0%d/",kDay); } 
// // 		 				if (idisk==0){ sprintf(thisdir,"/star/data09/reco/AuAu7_production/ReversedFullField/dev/2010/0%d/",kDay); }else
// // 		 				if (idisk==1){ sprintf(thisdir,"/star/data10/reco/AuAu7_production/ReversedFullField/dev/2010/0%d/",kDay); }else
// // 		 				if (idisk==2){ sprintf(thisdir,"/star/data12/reco/AuAu7_production/ReversedFullField/dev/2010/0%d/",kDay); } 
// 		 				if (idisk==0){ sprintf(thisdir,"/star/data09/reco/pp500_production_2011/ReversedFullField/dev/2011/0%d/",kDay); }else
// 		 				if (idisk==1){ sprintf(thisdir,"/star/data10/reco/pp500_production_2011/ReversedFullField/dev/2011/0%d/",kDay); }else
// 		 				if (idisk==2){ sprintf(thisdir,"/star/data12/reco/pp500_production_2011/ReversedFullField/dev/2011/0%d/",kDay); } 
// 		 			} else if (iChoice==1){
// 		 				if (idisk==0){ sprintf(thisdir,"/star/institutions/rice/geurts/mudst/run10/200/0%d/",kDay); }
// 		 			} else if (iChoice==2){
// 		 				if (idisk==0){ sprintf(thisdir,"/star/scratch/geurts/mudst/run10/039/0%d/",kDay); }
// 		 			} else if (iChoice==3||iChoice==4){
// 		 				if (idisk==0){ sprintf(thisdir,"/star/institutions/rice/geurts/pdst/0%d/",kDay); }
// 		 			} else if (iChoice==5){
// 		 				if (idisk==0){ sprintf(thisdir,"/star/data01/pwg/geurts/mudst/run10/039/0%d/",kDay); }
// 		 			} else if (iChoice==6){
// 		 				if (idisk==0){ sprintf(thisdir,"/star/data02/pwg/geurts/mudst/run10/039/0%d/",kDay); }
// 		 			} else if (iChoice==7){
// 		 				if (idisk==0){ sprintf(thisdir,"/star/data05/scratch/geurts/mudst/run10/039/0%d/",kDay); }
// 		 			} else if (iChoice==8){
// 		 				if (idisk==0){ sprintf(thisdir,"/star/data11/reco/AuAu62_production/ReversedFullField/P10ik_calib/2010/0%d/",kDay); }
// 		 			} else if (iChoice==9){
// 		 				if (idisk==0){ sprintf(thisdir,"/star/institutions/rice/geurts/pdst/run11/0%d/",kDay); }
// 		 			} else if (iChoice==10){
// 		 				if (idisk==0){ sprintf(thisdir,"/star/institutions/rice/geurts/pdst/run10/0%d/",kDay); }
// 		 			} else if (iChoice==11){
// 		 				if (idisk==0){ sprintf(thisdir,"/star/institutions/rice/geurts/muapp/run11/auau200/0%d/",kDay); }
// 		 			} else if (iChoice==12){
// 		 				if (idisk==0){ sprintf(thisdir,"/star/institutions/rice/geurts/pdst/run12/0%d/",kDay); }
// 		 			} else if (iChoice==13){
// 		 				if (idisk==0){ sprintf(thisdir,"/star/institutions/rice/geurts/pdst/run13/0%d/",kDay); }
// 		 			} else if (iChoice==14){
// 		 				if (idisk==0){ sprintf(thisdir,"/star/institutions/rice/geurts/pdst/run14/0%d/",kDay); }
// 		 			}
// 	 			} else 
//  				if (kDay<1000){
// //xx	 				if (idisk==0){ sprintf(thisdir,"/star/data09/reco/AuAu200_production/FullField/dev/2010/%d/",kDay); }else
// //xx	 				if (idisk==1){ sprintf(thisdir,"/star/data10/reco/AuAu200_production/FullField/dev/2010/%d/",kDay); } 				
// 					if (iChoice==0){
// //		 				if (idisk==0){ sprintf(thisdir,"/star/data09/reco/AuAu39_production/ReversedFullField/dev/2010/%d/",kDay); }else
// //		 				if (idisk==1){ sprintf(thisdir,"/star/data10/reco/AuAu39_production/ReversedFullField/dev/2010/%d/",kDay); } 
// // 		 				if (idisk==0){ sprintf(thisdir,"/star/data09/reco/AuAu7_production/ReversedFullField/dev/2010/%d/",kDay); }else
// // 		 				if (idisk==1){ sprintf(thisdir,"/star/data10/reco/AuAu7_production/ReversedFullField/dev/2010/%d/",kDay); }else
// // 		 				if (idisk==2){ sprintf(thisdir,"/star/data12/reco/AuAu7_production/ReversedFullField/dev/2010/%d/",kDay); } 
// 		 				if (idisk==0){ sprintf(thisdir,"/star/data09/reco/pp500_production_2011/ReversedFullField/dev/2011/%d/",kDay); }else
// 		 				if (idisk==1){ sprintf(thisdir,"/star/data10/reco/pp500_production_2011/ReversedFullField/dev/2011/%d/",kDay); }else
// 		 				if (idisk==2){ sprintf(thisdir,"/star/data12/reco/pp500_production_2011/ReversedFullField/dev/2011/%d/",kDay); } 
// 		 			} else if (iChoice==1){
// 		 				if (idisk==0){ sprintf(thisdir,"/star/institutions/rice/geurts/mudst/run10/200/%d/",kDay); }
// 		 			} else if (iChoice==2){
// 		 				if (idisk==0){ sprintf(thisdir,"/star/scratch/geurts/mudst/run10/039/%d/",kDay); }
// 		 			} else if (iChoice==3||iChoice==4){
// 		 				if (idisk==0){ sprintf(thisdir,"/star/institutions/rice/geurts/pdst/%d/",kDay); }
// 		 			} else if (iChoice==5){
// 		 				if (idisk==0){ sprintf(thisdir,"/star/data01/pwg/geurts/mudst/run10/039/%d/",kDay); }
// 		 			} else if (iChoice==6){
// 		 				if (idisk==0){ sprintf(thisdir,"/star/data02/pwg/geurts/mudst/run10/039/%d/",kDay); }
// 		 			} else if (iChoice==7){
// 		 				if (idisk==0){ sprintf(thisdir,"/star/data05/scratch/geurts/mudst/run10/039/%d/",kDay); }
// 		 			} else if (iChoice==8){
// 		 				if (idisk==0){ sprintf(thisdir,"/star/data11/reco/AuAu62_production/ReversedFullField/P10ik_calib/2010/%d/",kDay); }
// 		 			} else if (iChoice==9){
// 		 				if (idisk==0){ sprintf(thisdir,"/star/institutions/rice/geurts/pdst/run11/%d/",kDay); }
// 		 			} else if (iChoice==10){
// 		 				if (idisk==0){ sprintf(thisdir,"/star/institutions/rice/geurts/pdst/run10/%d/",kDay); }
// 		 			} else if (iChoice==11){
// 		 				if (idisk==0){ sprintf(thisdir,"/star/institutions/rice/geurts/muapp/run11/auau200/%d/",kDay); }
// 		 			} else if (iChoice==12){
// 		 				if (idisk==0){ sprintf(thisdir,"/star/institutions/rice/geurts/pdst/run12/%d/",kDay); }
// 		 			} else if (iChoice==13){
// 		 				if (idisk==0){ sprintf(thisdir,"/star/institutions/rice/geurts/pdst/run13/%d/",kDay); }
// 		 			} else if (iChoice==14){
// 		 				if (idisk==0){ sprintf(thisdir,"/star/institutions/rice/geurts/pdst/run14/%d/",kDay); }
// 		 			}
// 	 			} 
// 	 			cout<<idisk<<"  thisdir = "<<thisdir<<endl;
// 				path	 = TString(thisdir);
// 	//   			path	+= TString("/");
// 				gSystem->ChangeDirectory(thisdir);
// 				void *daydir	= gSystem->OpenDirectory(thisdir); 			
// 				const char *namedir;
// 				const char *namefile;
// 				while ((namedir = gSystem->GetDirEntry(daydir))) {
// 					cout << "namedir = " << namedir << endl;
// 					dir		 = TString(namedir);
// 					dir		+= TString("/");
// 					void *rundir	= gSystem->OpenDirectory(namedir);
// //					cout << "rundir = " << rundir << endl;
// 					while ((namefile = gSystem->GetDirEntry(rundir))) {
// 						cout << "namefile = " << namefile << endl;
// 						file		= TString(namefile);
// 						fullpath	= path+dir+file;
// 						if (iChoice<=2 || iChoice==5 || iChoice==6 || iChoice==8 
// 						 || iChoice==9 || iChoice==10 || iChoice==11 || iChoice==12
// 						 || iChoice==13|| iChoice==14){
// // 							if (strstr(fullpath.Data(),"st_physics_")==NULL){
// // 								//cout<<"Skipping.... "<<fullpath<<endl;			
// // 							}else{
// // 								if (strstr(fullpath.Data(),"MuDst.root")==NULL){
// // 									//cout<<"Skipping.... "<<fullpath<<endl;
// // 								} else {
// // 									++nAdded;
// // 									nf += chain->Add(fullpath);
// // 									cout<<"Adding...... "<<nAdded<<"\t "<<fullpath<<endl;
// // 								}
// // 							}
// 							if (strstr(fullpath.Data(),"MuDst.root")!=NULL){
// 								cout<<"fullpath = "<<fullpath<<endl;
// 								nf += chain->Add(fullpath);
// 							}
// 						} else if (iChoice==3){
// 							if (strstr(fullpath.Data(),"pDst_phy_")==NULL){
// 								//cout<<"Skipping.... "<<fullpath<<endl;			
// 							}else{
// 								if (strstr(fullpath.Data(),"MuDst.root")==NULL){
// 									//cout<<"Skipping.... "<<fullpath<<endl;
// 								} else {
// 									++nAdded;
// 									nf += chain->Add(fullpath);
// 									cout<<"Adding...... "<<nAdded<<"\t "<<fullpath<<endl;
// 								}
// 							}							
// 						} else if (iChoice==4){
// 							if (strstr(fullpath.Data(),"pDst_mon_")==NULL){
// 								//cout<<"Skipping.... "<<fullpath<<endl;			
// 							}else{
// 								if (strstr(fullpath.Data(),"MuDst.root")==NULL){
// 									//cout<<"Skipping.... "<<fullpath<<endl;
// 								} else {
// 									++nAdded;
// 									nf += chain->Add(fullpath);
// 									cout<<"Adding...... "<<nAdded<<"\t "<<fullpath<<endl;
// 								}
// 							}							
// 						}
// 					 }
// 				}
//  			}
//  		}	//---- end kDay>=3
//		gSystem->ChangeDirectory("/star/u/geurts/run14/muapp");
		
		tree = chain;
	}	//---- end tree=0
	nFiles	= nf;
	cout <<"Nfiles = "<<nFiles<<endl;
	Init(tree);
}

muapp::~muapp()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t muapp::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}

Long64_t muapp::LoadTree(Long64_t entry)
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

void muapp::Init(TTree *tree)
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

	fChain->SetBranchStatus("*",0);
	TIter 	iter(fChain->GetListOfBranches());
	char 	bname[40];
	while(TBranch *br = (TBranch *)iter.Next()) {
		strcpy(bname,br->GetName());
		//if (strcmp(bname,"Event")==0){ 			fChain->SetBranchStatus(br->GetName(),1); } 
		if (strcmp(bname,"MuEvent")==0){ 			fChain->SetBranchStatus(br->GetName(),1); } 
		//if (strcmp(bname,"PrimaryVertices")==0){	fChain->SetBranchStatus(br->GetName(),1); } 
		//if (strcmp(bname,"PrimaryTracks")==0){ 	fChain->SetBranchStatus(br->GetName(),1); } 
		//if (strcmp(bname,"GlobalTracks")==0){ 	fChain->SetBranchStatus(br->GetName(),1); } 
		if (strcmp(bname,"BTofHeader")==0){ 		fChain->SetBranchStatus(br->GetName(),1); } 
		if (strcmp(bname,"BTofRawHit")==0){ 		fChain->SetBranchStatus(br->GetName(),1); } 
		if (strcmp(bname,"BTofHit")   ==0){ 		fChain->SetBranchStatus(br->GetName(),1); } 
		if (fChain->GetBranchStatus(br->GetName())){
			cout<<"ACTIVATING "<<bname<<" "<<(int)fChain->GetBranchStatus(br->GetName())<<endl;
		}
			//cout<<bname<<" "<<(int)fChain->GetBranchStatus(br->GetName())<<endl;
	}

   fChain->SetBranchAddress("MuEvent", &MuEvent_, &b_MuEvent_);
   fChain->SetBranchAddress("MuEvent.mRunInfo.mRunId", MuEvent_mRunInfo_mRunId, &b_MuEvent_mRunInfo_mRunId);
   fChain->SetBranchAddress("MuEvent.mRunInfo.mProductionTime", MuEvent_mRunInfo_mProductionTime, &b_MuEvent_mRunInfo_mProductionTime);
   fChain->SetBranchAddress("MuEvent.mRunInfo.mProductionVersion", MuEvent_mRunInfo_mProductionVersion, &b_MuEvent_mRunInfo_mProductionVersion);
   fChain->SetBranchAddress("MuEvent.mRunInfo.mCenterOfMassEnergy", MuEvent_mRunInfo_mCenterOfMassEnergy, &b_MuEvent_mRunInfo_mCenterOfMassEnergy);
   fChain->SetBranchAddress("MuEvent.mRunInfo.mBeamMassNumber[2]", MuEvent_mRunInfo_mBeamMassNumber, &b_MuEvent_mRunInfo_mBeamMassNumber);
   fChain->SetBranchAddress("MuEvent.mRunInfo.mMagneticFieldZ", MuEvent_mRunInfo_mMagneticFieldZ, &b_MuEvent_mRunInfo_mMagneticFieldZ);
   fChain->SetBranchAddress("MuEvent.mRunInfo.mTpcDriftVelocity[2]", MuEvent_mRunInfo_mTpcDriftVelocity, &b_MuEvent_mRunInfo_mTpcDriftVelocity);
   fChain->SetBranchAddress("MuEvent.mRunInfo.mSvtDriftVelocityScaler", MuEvent_mRunInfo_mSvtDriftVelocityScaler, &b_MuEvent_mRunInfo_mSvtDriftVelocityScaler);
   fChain->SetBranchAddress("MuEvent.mRunInfo.mZdcEastRate", MuEvent_mRunInfo_mZdcEastRate, &b_MuEvent_mRunInfo_mZdcEastRate);
   fChain->SetBranchAddress("MuEvent.mRunInfo.mZdcWestRate", MuEvent_mRunInfo_mZdcWestRate, &b_MuEvent_mRunInfo_mZdcWestRate);
   fChain->SetBranchAddress("MuEvent.mRunInfo.mZdcCoincidenceRate", MuEvent_mRunInfo_mZdcCoincidenceRate, &b_MuEvent_mRunInfo_mZdcCoincidenceRate);
   fChain->SetBranchAddress("MuEvent.mRunInfo.mBackgroundRate", MuEvent_mRunInfo_mBackgroundRate, &b_MuEvent_mRunInfo_mBackgroundRate);
   fChain->SetBranchAddress("MuEvent.mRunInfo.mL0RateToRich", MuEvent_mRunInfo_mL0RateToRich, &b_MuEvent_mRunInfo_mL0RateToRich);
   fChain->SetBranchAddress("MuEvent.mRunInfo.mBbcCoincidenceRate", MuEvent_mRunInfo_mBbcCoincidenceRate, &b_MuEvent_mRunInfo_mBbcCoincidenceRate);
   fChain->SetBranchAddress("MuEvent.mRunInfo.mBeamEnergy[2]", MuEvent_mRunInfo_mBeamEnergy, &b_MuEvent_mRunInfo_mBeamEnergy);
   fChain->SetBranchAddress("MuEvent.mRunInfo.mInitialBeamIntensity[2]", MuEvent_mRunInfo_mInitialBeamIntensity, &b_MuEvent_mRunInfo_mInitialBeamIntensity);
   fChain->SetBranchAddress("MuEvent.mRunInfo.mBeamLifeTime[2]", MuEvent_mRunInfo_mBeamLifeTime, &b_MuEvent_mRunInfo_mBeamLifeTime);
   fChain->SetBranchAddress("MuEvent.mRunInfo.mBeamFillNumber[2]", MuEvent_mRunInfo_mBeamFillNumber, &b_MuEvent_mRunInfo_mBeamFillNumber);
   fChain->SetBranchAddress("MuEvent.mRunInfo.mBbcEastRate", MuEvent_mRunInfo_mBbcEastRate, &b_MuEvent_mRunInfo_mBbcEastRate);
   fChain->SetBranchAddress("MuEvent.mRunInfo.mBbcWestRate", MuEvent_mRunInfo_mBbcWestRate, &b_MuEvent_mRunInfo_mBbcWestRate);
   fChain->SetBranchAddress("MuEvent.mRunInfo.mBbcBlueBackgroundRate", MuEvent_mRunInfo_mBbcBlueBackgroundRate, &b_MuEvent_mRunInfo_mBbcBlueBackgroundRate);
   fChain->SetBranchAddress("MuEvent.mRunInfo.mBbcYellowBackgroundRate", MuEvent_mRunInfo_mBbcYellowBackgroundRate, &b_MuEvent_mRunInfo_mBbcYellowBackgroundRate);
   fChain->SetBranchAddress("MuEvent.mRunInfo.mSpaceChargeCorrectionMode", MuEvent_mRunInfo_mSpaceChargeCorrectionMode, &b_MuEvent_mRunInfo_mSpaceChargeCorrectionMode);
   fChain->SetBranchAddress("MuEvent.mRunInfo.mSpaceCharge", MuEvent_mRunInfo_mSpaceCharge, &b_MuEvent_mRunInfo_mSpaceCharge);
   fChain->SetBranchAddress("MuEvent.mEventInfo.mType", MuEvent_mEventInfo_mType, &b_MuEvent_mEventInfo_mType);
   fChain->SetBranchAddress("MuEvent.mEventInfo.mRunId", MuEvent_mEventInfo_mRunId, &b_MuEvent_mEventInfo_mRunId);
   fChain->SetBranchAddress("MuEvent.mEventInfo.mId", MuEvent_mEventInfo_mId, &b_MuEvent_mEventInfo_mId);
   fChain->SetBranchAddress("MuEvent.mEventInfo.mTime", MuEvent_mEventInfo_mTime, &b_MuEvent_mEventInfo_mTime);
   fChain->SetBranchAddress("MuEvent.mEventInfo.mTriggerMask", MuEvent_mEventInfo_mTriggerMask, &b_MuEvent_mEventInfo_mTriggerMask);
   fChain->SetBranchAddress("MuEvent.mEventInfo.mBunchCrossingNumber[2]", MuEvent_mEventInfo_mBunchCrossingNumber, &b_MuEvent_mEventInfo_mBunchCrossingNumber);
   fChain->SetBranchAddress("MuEvent.mEventInfo.mEventSize", MuEvent_mEventInfo_mEventSize, &b_MuEvent_mEventInfo_mEventSize);
   fChain->SetBranchAddress("MuEvent.mEventSummary.mNumberOfTracks", MuEvent_mEventSummary_mNumberOfTracks, &b_MuEvent_mEventSummary_mNumberOfTracks);
   fChain->SetBranchAddress("MuEvent.mEventSummary.mNumberOfGoodTracks", MuEvent_mEventSummary_mNumberOfGoodTracks, &b_MuEvent_mEventSummary_mNumberOfGoodTracks);
   fChain->SetBranchAddress("MuEvent.mEventSummary.mNumberOfGoodPrimaryTracks", MuEvent_mEventSummary_mNumberOfGoodPrimaryTracks, &b_MuEvent_mEventSummary_mNumberOfGoodPrimaryTracks);
   fChain->SetBranchAddress("MuEvent.mEventSummary.mNumberOfPositiveTracks", MuEvent_mEventSummary_mNumberOfPositiveTracks, &b_MuEvent_mEventSummary_mNumberOfPositiveTracks);
   fChain->SetBranchAddress("MuEvent.mEventSummary.mNumberOfNegativeTracks", MuEvent_mEventSummary_mNumberOfNegativeTracks, &b_MuEvent_mEventSummary_mNumberOfNegativeTracks);
   fChain->SetBranchAddress("MuEvent.mEventSummary.mNumberOfExoticTracks", MuEvent_mEventSummary_mNumberOfExoticTracks, &b_MuEvent_mEventSummary_mNumberOfExoticTracks);
   fChain->SetBranchAddress("MuEvent.mEventSummary.mNumberOfVertices", MuEvent_mEventSummary_mNumberOfVertices, &b_MuEvent_mEventSummary_mNumberOfVertices);
   fChain->SetBranchAddress("MuEvent.mEventSummary.mNumberOfVertexTypes", MuEvent_mEventSummary_mNumberOfVertexTypes, &b_MuEvent_mEventSummary_mNumberOfVertexTypes);
   fChain->SetBranchAddress("MuEvent.mEventSummary.mNumberOfPileupVertices", MuEvent_mEventSummary_mNumberOfPileupVertices, &b_MuEvent_mEventSummary_mNumberOfPileupVertices);
   fChain->SetBranchAddress("MuEvent.mEventSummary.mMeanPt", MuEvent_mEventSummary_mMeanPt, &b_MuEvent_mEventSummary_mMeanPt);
   fChain->SetBranchAddress("MuEvent.mEventSummary.mMeanPt2", MuEvent_mEventSummary_mMeanPt2, &b_MuEvent_mEventSummary_mMeanPt2);
   fChain->SetBranchAddress("MuEvent.mEventSummary.mMeanEta", MuEvent_mEventSummary_mMeanEta, &b_MuEvent_mEventSummary_mMeanEta);
   fChain->SetBranchAddress("MuEvent.mEventSummary.mRmsEta", MuEvent_mEventSummary_mRmsEta, &b_MuEvent_mEventSummary_mRmsEta);
   fChain->SetBranchAddress("MuEvent.mEventSummary.mPrimaryVertexPos.mX1", MuEvent_mEventSummary_mPrimaryVertexPos_mX1, &b_MuEvent_mEventSummary_mPrimaryVertexPos_mX1);
   fChain->SetBranchAddress("MuEvent.mEventSummary.mPrimaryVertexPos.mX2", MuEvent_mEventSummary_mPrimaryVertexPos_mX2, &b_MuEvent_mEventSummary_mPrimaryVertexPos_mX2);
   fChain->SetBranchAddress("MuEvent.mEventSummary.mPrimaryVertexPos.mX3", MuEvent_mEventSummary_mPrimaryVertexPos_mX3, &b_MuEvent_mEventSummary_mPrimaryVertexPos_mX3);
   fChain->SetBranchAddress("MuEvent.mEventSummary.mVertexTypeArraySize", MuEvent_mEventSummary_mVertexTypeArraySize, &b_MuEvent_mEventSummary_mVertexTypeArraySize);
   fChain->SetBranchAddress("MuEvent.mEventSummary.mPhiBinsSize", MuEvent_mEventSummary_mPhiBinsSize, &b_MuEvent_mEventSummary_mPhiBinsSize);
   fChain->SetBranchAddress("MuEvent.mEventSummary.mPtAndEtaBinsSize", MuEvent_mEventSummary_mPtAndEtaBinsSize, &b_MuEvent_mEventSummary_mPtAndEtaBinsSize);
   fChain->SetBranchAddress("MuEvent.mEventSummary.mHistogramSize", MuEvent_mEventSummary_mHistogramSize, &b_MuEvent_mEventSummary_mHistogramSize);
   fChain->SetBranchAddress("MuEvent.mEventSummary.mEtaBins", MuEvent_mEventSummary_mEtaBins, &b_MuEvent_mEventSummary_mEtaBins);
   fChain->SetBranchAddress("MuEvent.mEventSummary.mPtBins", MuEvent_mEventSummary_mPtBins, &b_MuEvent_mEventSummary_mPtBins);
   fChain->SetBranchAddress("MuEvent.mEventSummary.mPhiBins", MuEvent_mEventSummary_mPhiBins, &b_MuEvent_mEventSummary_mPhiBins);
   fChain->SetBranchAddress("MuEvent.mEventSummary.mEtaOfTracksHisto", MuEvent_mEventSummary_mEtaOfTracksHisto, &b_MuEvent_mEventSummary_mEtaOfTracksHisto);
   fChain->SetBranchAddress("MuEvent.mEventSummary.mPtOfTracksHisto", MuEvent_mEventSummary_mPtOfTracksHisto, &b_MuEvent_mEventSummary_mPtOfTracksHisto);
   fChain->SetBranchAddress("MuEvent.mEventSummary.mPhiOfTracksHisto", MuEvent_mEventSummary_mPhiOfTracksHisto, &b_MuEvent_mEventSummary_mPhiOfTracksHisto);
   fChain->SetBranchAddress("MuEvent.mEventSummary.mEneryVsEtaHisto", MuEvent_mEventSummary_mEneryVsEtaHisto, &b_MuEvent_mEventSummary_mEneryVsEtaHisto);
   fChain->SetBranchAddress("MuEvent.mEventSummary.mEnergyVsPhiHisto", MuEvent_mEventSummary_mEnergyVsPhiHisto, &b_MuEvent_mEventSummary_mEnergyVsPhiHisto);
   fChain->SetBranchAddress("MuEvent.mEventSummary.mMagneticFieldZ", MuEvent_mEventSummary_mMagneticFieldZ, &b_MuEvent_mEventSummary_mMagneticFieldZ);
   fChain->SetBranchAddress("MuEvent.mVpdTriggerDetector.mADC[2][16]", MuEvent_mVpdTriggerDetector_mADC, &b_MuEvent_mVpdTriggerDetector_mADC);
   fChain->SetBranchAddress("MuEvent.mVpdTriggerDetector.mTDC[2][16]", MuEvent_mVpdTriggerDetector_mTDC, &b_MuEvent_mVpdTriggerDetector_mTDC);
   fChain->SetBranchAddress("MuEvent.mVpdTriggerDetector.mADCmxq[2][16]", MuEvent_mVpdTriggerDetector_mADCmxq, &b_MuEvent_mVpdTriggerDetector_mADCmxq);
   fChain->SetBranchAddress("MuEvent.mVpdTriggerDetector.mTDCmxq[2][16]", MuEvent_mVpdTriggerDetector_mTDCmxq, &b_MuEvent_mVpdTriggerDetector_mTDCmxq);
   fChain->SetBranchAddress("MuEvent.mVpdTriggerDetector.mEarliestTDC[2]", MuEvent_mVpdTriggerDetector_mEarliestTDC, &b_MuEvent_mVpdTriggerDetector_mEarliestTDC);
   fChain->SetBranchAddress("MuEvent.mVpdTriggerDetector.mTimeDifference", MuEvent_mVpdTriggerDetector_mTimeDifference, &b_MuEvent_mVpdTriggerDetector_mTimeDifference);
   fChain->SetBranchAddress("MuEvent.mVpdTriggerDetector.mYear", MuEvent_mVpdTriggerDetector_mYear, &b_MuEvent_mVpdTriggerDetector_mYear);
   fChain->SetBranchAddress("MuEvent.mMtdTriggerDetector.mADC[2][8]", MuEvent_mMtdTriggerDetector_mADC, &b_MuEvent_mMtdTriggerDetector_mADC);
   fChain->SetBranchAddress("MuEvent.mMtdTriggerDetector.mTDC[2][8]", MuEvent_mMtdTriggerDetector_mTDC, &b_MuEvent_mMtdTriggerDetector_mTDC);
   fChain->SetBranchAddress("MuEvent.mCtbTriggerDetector.mMips[120][2][11]", MuEvent_mCtbTriggerDetector_mMips, &b_MuEvent_mCtbTriggerDetector_mMips);
   fChain->SetBranchAddress("MuEvent.mCtbTriggerDetector.mTime[120][2][11]", MuEvent_mCtbTriggerDetector_mTime, &b_MuEvent_mCtbTriggerDetector_mTime);
   fChain->SetBranchAddress("MuEvent.mCtbTriggerDetector.mAux[16][11]", MuEvent_mCtbTriggerDetector_mAux, &b_MuEvent_mCtbTriggerDetector_mAux);
   fChain->SetBranchAddress("MuEvent.mCtbTriggerDetector.mNumberOfPreSamples", MuEvent_mCtbTriggerDetector_mNumberOfPreSamples, &b_MuEvent_mCtbTriggerDetector_mNumberOfPreSamples);
   fChain->SetBranchAddress("MuEvent.mCtbTriggerDetector.mNumberOfPostSamples", MuEvent_mCtbTriggerDetector_mNumberOfPostSamples, &b_MuEvent_mCtbTriggerDetector_mNumberOfPostSamples);
   fChain->SetBranchAddress("MuEvent.mZdcTriggerDetector.mAdc[16]", MuEvent_mZdcTriggerDetector_mAdc, &b_MuEvent_mZdcTriggerDetector_mAdc);
   fChain->SetBranchAddress("MuEvent.mZdcTriggerDetector.mTdc[16]", MuEvent_mZdcTriggerDetector_mTdc, &b_MuEvent_mZdcTriggerDetector_mTdc);
   fChain->SetBranchAddress("MuEvent.mZdcTriggerDetector.mSumAdc[2]", MuEvent_mZdcTriggerDetector_mSumAdc, &b_MuEvent_mZdcTriggerDetector_mSumAdc);
   fChain->SetBranchAddress("MuEvent.mZdcTriggerDetector.mSum", MuEvent_mZdcTriggerDetector_mSum, &b_MuEvent_mZdcTriggerDetector_mSum);
   fChain->SetBranchAddress("MuEvent.mZdcTriggerDetector.mVertexZ", MuEvent_mZdcTriggerDetector_mVertexZ, &b_MuEvent_mZdcTriggerDetector_mVertexZ);
   fChain->SetBranchAddress("MuEvent.mZdcTriggerDetector.mZdcSmdEast[16]", MuEvent_mZdcTriggerDetector_mZdcSmdEast, &b_MuEvent_mZdcTriggerDetector_mZdcSmdEast);
   fChain->SetBranchAddress("MuEvent.mZdcTriggerDetector.mZdcSmdWest[16]", MuEvent_mZdcTriggerDetector_mZdcSmdWest, &b_MuEvent_mZdcTriggerDetector_mZdcSmdWest);
   fChain->SetBranchAddress("MuEvent.mBbcTriggerDetector.mAdc[48]", MuEvent_mBbcTriggerDetector_mAdc, &b_MuEvent_mBbcTriggerDetector_mAdc);
   fChain->SetBranchAddress("MuEvent.mBbcTriggerDetector.mTdc[48]", MuEvent_mBbcTriggerDetector_mTdc, &b_MuEvent_mBbcTriggerDetector_mTdc);
   fChain->SetBranchAddress("MuEvent.mBbcTriggerDetector.mReg[2]", MuEvent_mBbcTriggerDetector_mReg, &b_MuEvent_mBbcTriggerDetector_mReg);
   fChain->SetBranchAddress("MuEvent.mBbcTriggerDetector.mPed[128]", MuEvent_mBbcTriggerDetector_mPed, &b_MuEvent_mBbcTriggerDetector_mPed);
   fChain->SetBranchAddress("MuEvent.mBbcTriggerDetector.mScl[32]", MuEvent_mBbcTriggerDetector_mScl, &b_MuEvent_mBbcTriggerDetector_mScl);
   fChain->SetBranchAddress("MuEvent.mBbcTriggerDetector.mYear", MuEvent_mBbcTriggerDetector_mYear, &b_MuEvent_mBbcTriggerDetector_mYear);
   fChain->SetBranchAddress("MuEvent.mBbcTriggerDetector.mDSMVTX", MuEvent_mBbcTriggerDetector_mDSMVTX, &b_MuEvent_mBbcTriggerDetector_mDSMVTX);
   fChain->SetBranchAddress("MuEvent.mEmcTriggerDetector.mHighTower[300]", MuEvent_mEmcTriggerDetector_mHighTower, &b_MuEvent_mEmcTriggerDetector_mHighTower);
   fChain->SetBranchAddress("MuEvent.mEmcTriggerDetector.mPatch[300]", MuEvent_mEmcTriggerDetector_mPatch, &b_MuEvent_mEmcTriggerDetector_mPatch);
   fChain->SetBranchAddress("MuEvent.mEmcTriggerDetector.mEHighTower[90]", MuEvent_mEmcTriggerDetector_mEHighTower, &b_MuEvent_mEmcTriggerDetector_mEHighTower);
   fChain->SetBranchAddress("MuEvent.mEmcTriggerDetector.mEPatch[90]", MuEvent_mEmcTriggerDetector_mEPatch, &b_MuEvent_mEmcTriggerDetector_mEPatch);
   fChain->SetBranchAddress("MuEvent.mEmcTriggerDetector.mBemcLayer1[48]", MuEvent_mEmcTriggerDetector_mBemcLayer1, &b_MuEvent_mEmcTriggerDetector_mBemcLayer1);
   fChain->SetBranchAddress("MuEvent.mEmcTriggerDetector.mEemcLayer1[16]", MuEvent_mEmcTriggerDetector_mEemcLayer1, &b_MuEvent_mEmcTriggerDetector_mEemcLayer1);
   fChain->SetBranchAddress("MuEvent.mEmcTriggerDetector.mEmcLayer2[8]", MuEvent_mEmcTriggerDetector_mEmcLayer2, &b_MuEvent_mEmcTriggerDetector_mEmcLayer2);
   fChain->SetBranchAddress("MuEvent.mFpdTriggerDetector.mEN[49]", MuEvent_mFpdTriggerDetector_mEN, &b_MuEvent_mFpdTriggerDetector_mEN);
   fChain->SetBranchAddress("MuEvent.mFpdTriggerDetector.mES[49]", MuEvent_mFpdTriggerDetector_mES, &b_MuEvent_mFpdTriggerDetector_mES);
   fChain->SetBranchAddress("MuEvent.mFpdTriggerDetector.mET[25]", MuEvent_mFpdTriggerDetector_mET, &b_MuEvent_mFpdTriggerDetector_mET);
   fChain->SetBranchAddress("MuEvent.mFpdTriggerDetector.mEB[25]", MuEvent_mFpdTriggerDetector_mEB, &b_MuEvent_mFpdTriggerDetector_mEB);
   fChain->SetBranchAddress("MuEvent.mFpdTriggerDetector.mEPN[7]", MuEvent_mFpdTriggerDetector_mEPN, &b_MuEvent_mFpdTriggerDetector_mEPN);
   fChain->SetBranchAddress("MuEvent.mFpdTriggerDetector.mEPS[7]", MuEvent_mFpdTriggerDetector_mEPS, &b_MuEvent_mFpdTriggerDetector_mEPS);
   fChain->SetBranchAddress("MuEvent.mFpdTriggerDetector.mWN[49]", MuEvent_mFpdTriggerDetector_mWN, &b_MuEvent_mFpdTriggerDetector_mWN);
   fChain->SetBranchAddress("MuEvent.mFpdTriggerDetector.mWS[49]", MuEvent_mFpdTriggerDetector_mWS, &b_MuEvent_mFpdTriggerDetector_mWS);
   fChain->SetBranchAddress("MuEvent.mFpdTriggerDetector.mWT[25]", MuEvent_mFpdTriggerDetector_mWT, &b_MuEvent_mFpdTriggerDetector_mWT);
   fChain->SetBranchAddress("MuEvent.mFpdTriggerDetector.mWB[25]", MuEvent_mFpdTriggerDetector_mWB, &b_MuEvent_mFpdTriggerDetector_mWB);
   fChain->SetBranchAddress("MuEvent.mFpdTriggerDetector.mWPN[7]", MuEvent_mFpdTriggerDetector_mWPN, &b_MuEvent_mFpdTriggerDetector_mWPN);
   fChain->SetBranchAddress("MuEvent.mFpdTriggerDetector.mWPS[7]", MuEvent_mFpdTriggerDetector_mWPS, &b_MuEvent_mFpdTriggerDetector_mWPS);
   fChain->SetBranchAddress("MuEvent.mFpdTriggerDetector.mLayer1[2][6][4]", MuEvent_mFpdTriggerDetector_mLayer1, &b_MuEvent_mFpdTriggerDetector_mLayer1);
   fChain->SetBranchAddress("MuEvent.mFpdTriggerDetector.mLayer2[2][6]", MuEvent_mFpdTriggerDetector_mLayer2, &b_MuEvent_mFpdTriggerDetector_mLayer2);
   fChain->SetBranchAddress("MuEvent.mFmsTriggerDetector.mNumQTdata", MuEvent_mFmsTriggerDetector_mNumQTdata, &b_MuEvent_mFmsTriggerDetector_mNumQTdata);
   fChain->SetBranchAddress("MuEvent.mFmsTriggerDetector.mQTdata[1600]", MuEvent_mFmsTriggerDetector_mQTdata, &b_MuEvent_mFmsTriggerDetector_mQTdata);
   fChain->SetBranchAddress("MuEvent.mFmsTriggerDetector.mDSM[256]", MuEvent_mFmsTriggerDetector_mDSM, &b_MuEvent_mFmsTriggerDetector_mDSM);
   fChain->SetBranchAddress("MuEvent.mFmsTriggerDetector.mDSM01[112]", MuEvent_mFmsTriggerDetector_mDSM01, &b_MuEvent_mFmsTriggerDetector_mDSM01);
   fChain->SetBranchAddress("MuEvent.mFmsTriggerDetector.mDSM02[16]", MuEvent_mFmsTriggerDetector_mDSM02, &b_MuEvent_mFmsTriggerDetector_mDSM02);
   fChain->SetBranchAddress("MuEvent.mFmsTriggerDetector.mDSM1[16]", MuEvent_mFmsTriggerDetector_mDSM1, &b_MuEvent_mFmsTriggerDetector_mDSM1);
   fChain->SetBranchAddress("MuEvent.mFmsTriggerDetector.mDSM2[8]", MuEvent_mFmsTriggerDetector_mDSM2, &b_MuEvent_mFmsTriggerDetector_mDSM2);
   fChain->SetBranchAddress("MuEvent.mFpdCollection.mAdc[256]", MuEvent_mFpdCollection_mAdc, &b_MuEvent_mFpdCollection_mAdc);
   fChain->SetBranchAddress("MuEvent.mFpdCollection.mTdc[8]", MuEvent_mFpdCollection_mTdc, &b_MuEvent_mFpdCollection_mTdc);
   fChain->SetBranchAddress("MuEvent.mFpdCollection.mReg[3]", MuEvent_mFpdCollection_mReg, &b_MuEvent_mFpdCollection_mReg);
   fChain->SetBranchAddress("MuEvent.mFpdCollection.mPed[256]", MuEvent_mFpdCollection_mPed, &b_MuEvent_mFpdCollection_mPed);
   fChain->SetBranchAddress("MuEvent.mFpdCollection.mScl[128]", MuEvent_mFpdCollection_mScl, &b_MuEvent_mFpdCollection_mScl);
   fChain->SetBranchAddress("MuEvent.mFpdCollection.mToken", MuEvent_mFpdCollection_mToken, &b_MuEvent_mFpdCollection_mToken);
   fChain->SetBranchAddress("MuEvent.mL0Trigger.mTriggerActionWord", MuEvent_mL0Trigger_mTriggerActionWord, &b_MuEvent_mL0Trigger_mTriggerActionWord);
   fChain->SetBranchAddress("MuEvent.mL0Trigger.mTriggerWord", MuEvent_mL0Trigger_mTriggerWord, &b_MuEvent_mL0Trigger_mTriggerWord);
   fChain->SetBranchAddress("MuEvent.mL0Trigger.mCoarsePixelArray[32]", MuEvent_mL0Trigger_mCoarsePixelArray, &b_MuEvent_mL0Trigger_mCoarsePixelArray);
   fChain->SetBranchAddress("MuEvent.mL0Trigger.mMwcCtbMultiplicity", MuEvent_mL0Trigger_mMwcCtbMultiplicity, &b_MuEvent_mL0Trigger_mMwcCtbMultiplicity);
   fChain->SetBranchAddress("MuEvent.mL0Trigger.mMwcCtbDipole", MuEvent_mL0Trigger_mMwcCtbDipole, &b_MuEvent_mL0Trigger_mMwcCtbDipole);
   fChain->SetBranchAddress("MuEvent.mL0Trigger.mMwcCtbTopology", MuEvent_mL0Trigger_mMwcCtbTopology, &b_MuEvent_mL0Trigger_mMwcCtbTopology);
   fChain->SetBranchAddress("MuEvent.mL0Trigger.mMwcCtbMoment", MuEvent_mL0Trigger_mMwcCtbMoment, &b_MuEvent_mL0Trigger_mMwcCtbMoment);
   fChain->SetBranchAddress("MuEvent.mL0Trigger.mDsmInput", MuEvent_mL0Trigger_mDsmInput, &b_MuEvent_mL0Trigger_mDsmInput);
   fChain->SetBranchAddress("MuEvent.mL0Trigger.mDetectorBusy", MuEvent_mL0Trigger_mDetectorBusy, &b_MuEvent_mL0Trigger_mDetectorBusy);
   fChain->SetBranchAddress("MuEvent.mL0Trigger.mTriggerToken", MuEvent_mL0Trigger_mTriggerToken, &b_MuEvent_mL0Trigger_mTriggerToken);
   fChain->SetBranchAddress("MuEvent.mL0Trigger.mDsmAddress", MuEvent_mL0Trigger_mDsmAddress, &b_MuEvent_mL0Trigger_mDsmAddress);
   fChain->SetBranchAddress("MuEvent.mL0Trigger.mAddBits", MuEvent_mL0Trigger_mAddBits, &b_MuEvent_mL0Trigger_mAddBits);
   fChain->SetBranchAddress("MuEvent.mL0Trigger.mLastDsmArray[8]", MuEvent_mL0Trigger_mLastDsmArray, &b_MuEvent_mL0Trigger_mLastDsmArray);
   fChain->SetBranchAddress("MuEvent.mL0Trigger.mBcDataArray[16]", MuEvent_mL0Trigger_mBcDataArray, &b_MuEvent_mL0Trigger_mBcDataArray);
   fChain->SetBranchAddress("MuEvent.mL3EventSummary.mNumberOfProcessedEvents", MuEvent_mL3EventSummary_mNumberOfProcessedEvents, &b_MuEvent_mL3EventSummary_mNumberOfProcessedEvents);
   fChain->SetBranchAddress("MuEvent.mL3EventSummary.mNumberReconstructedEvents", MuEvent_mL3EventSummary_mNumberReconstructedEvents, &b_MuEvent_mL3EventSummary_mNumberReconstructedEvents);
   fChain->SetBranchAddress("MuEvent.mL3EventSummary.mNumberOfTracks", MuEvent_mL3EventSummary_mNumberOfTracks, &b_MuEvent_mL3EventSummary_mNumberOfTracks);
   fChain->SetBranchAddress("MuEvent.mL3EventSummary.mNumberOfAlgorithms", MuEvent_mL3EventSummary_mNumberOfAlgorithms, &b_MuEvent_mL3EventSummary_mNumberOfAlgorithms);
   fChain->SetBranchAddress("MuEvent.mL3EventSummary.mFlags", MuEvent_mL3EventSummary_mFlags, &b_MuEvent_mL3EventSummary_mFlags);
   fChain->SetBranchAddress("MuEvent.mL3EventSummary.mL0TriggerWord", MuEvent_mL3EventSummary_mL0TriggerWord, &b_MuEvent_mL3EventSummary_mL0TriggerWord);
   fChain->SetBranchAddress("MuEvent.mL3EventSummary.mUnbiasedPreScale", MuEvent_mL3EventSummary_mUnbiasedPreScale, &b_MuEvent_mL3EventSummary_mUnbiasedPreScale);
   fChain->SetBranchAddress("MuEvent.mL3EventSummary.mPrimaryVertex.mX1", MuEvent_mL3EventSummary_mPrimaryVertex_mX1, &b_MuEvent_mL3EventSummary_mPrimaryVertex_mX1);
   fChain->SetBranchAddress("MuEvent.mL3EventSummary.mPrimaryVertex.mX2", MuEvent_mL3EventSummary_mPrimaryVertex_mX2, &b_MuEvent_mL3EventSummary_mPrimaryVertex_mX2);
   fChain->SetBranchAddress("MuEvent.mL3EventSummary.mPrimaryVertex.mX3", MuEvent_mL3EventSummary_mPrimaryVertex_mX3, &b_MuEvent_mL3EventSummary_mPrimaryVertex_mX3);
   fChain->SetBranchAddress("MuEvent.mTriggerIdCollection.mL1TriggerId.mMask", MuEvent_mTriggerIdCollection_mL1TriggerId_mMask, &b_MuEvent_mTriggerIdCollection_mL1TriggerId_mMask);
   fChain->SetBranchAddress("MuEvent.mTriggerIdCollection.mL1TriggerId.mId[64]", MuEvent_mTriggerIdCollection_mL1TriggerId_mId, &b_MuEvent_mTriggerIdCollection_mL1TriggerId_mId);
   fChain->SetBranchAddress("MuEvent.mTriggerIdCollection.mL1TriggerId.mVersion[64]", MuEvent_mTriggerIdCollection_mL1TriggerId_mVersion, &b_MuEvent_mTriggerIdCollection_mL1TriggerId_mVersion);
   fChain->SetBranchAddress("MuEvent.mTriggerIdCollection.mL1TriggerId.mNameVersion[64]", MuEvent_mTriggerIdCollection_mL1TriggerId_mNameVersion, &b_MuEvent_mTriggerIdCollection_mL1TriggerId_mNameVersion);
   fChain->SetBranchAddress("MuEvent.mTriggerIdCollection.mL1TriggerId.mThresholdVersion[64]", MuEvent_mTriggerIdCollection_mL1TriggerId_mThresholdVersion, &b_MuEvent_mTriggerIdCollection_mL1TriggerId_mThresholdVersion);
   fChain->SetBranchAddress("MuEvent.mTriggerIdCollection.mL1TriggerId.mPrescaleVersion[64]", MuEvent_mTriggerIdCollection_mL1TriggerId_mPrescaleVersion, &b_MuEvent_mTriggerIdCollection_mL1TriggerId_mPrescaleVersion);
   fChain->SetBranchAddress("MuEvent.mTriggerIdCollection.mL2TriggerId.mMask", MuEvent_mTriggerIdCollection_mL2TriggerId_mMask, &b_MuEvent_mTriggerIdCollection_mL2TriggerId_mMask);
   fChain->SetBranchAddress("MuEvent.mTriggerIdCollection.mL2TriggerId.mId[64]", MuEvent_mTriggerIdCollection_mL2TriggerId_mId, &b_MuEvent_mTriggerIdCollection_mL2TriggerId_mId);
   fChain->SetBranchAddress("MuEvent.mTriggerIdCollection.mL2TriggerId.mVersion[64]", MuEvent_mTriggerIdCollection_mL2TriggerId_mVersion, &b_MuEvent_mTriggerIdCollection_mL2TriggerId_mVersion);
   fChain->SetBranchAddress("MuEvent.mTriggerIdCollection.mL2TriggerId.mNameVersion[64]", MuEvent_mTriggerIdCollection_mL2TriggerId_mNameVersion, &b_MuEvent_mTriggerIdCollection_mL2TriggerId_mNameVersion);
   fChain->SetBranchAddress("MuEvent.mTriggerIdCollection.mL2TriggerId.mThresholdVersion[64]", MuEvent_mTriggerIdCollection_mL2TriggerId_mThresholdVersion, &b_MuEvent_mTriggerIdCollection_mL2TriggerId_mThresholdVersion);
   fChain->SetBranchAddress("MuEvent.mTriggerIdCollection.mL2TriggerId.mPrescaleVersion[64]", MuEvent_mTriggerIdCollection_mL2TriggerId_mPrescaleVersion, &b_MuEvent_mTriggerIdCollection_mL2TriggerId_mPrescaleVersion);
   fChain->SetBranchAddress("MuEvent.mTriggerIdCollection.mL3TriggerId.mMask", MuEvent_mTriggerIdCollection_mL3TriggerId_mMask, &b_MuEvent_mTriggerIdCollection_mL3TriggerId_mMask);
   fChain->SetBranchAddress("MuEvent.mTriggerIdCollection.mL3TriggerId.mId[64]", MuEvent_mTriggerIdCollection_mL3TriggerId_mId, &b_MuEvent_mTriggerIdCollection_mL3TriggerId_mId);
   fChain->SetBranchAddress("MuEvent.mTriggerIdCollection.mL3TriggerId.mVersion[64]", MuEvent_mTriggerIdCollection_mL3TriggerId_mVersion, &b_MuEvent_mTriggerIdCollection_mL3TriggerId_mVersion);
   fChain->SetBranchAddress("MuEvent.mTriggerIdCollection.mL3TriggerId.mNameVersion[64]", MuEvent_mTriggerIdCollection_mL3TriggerId_mNameVersion, &b_MuEvent_mTriggerIdCollection_mL3TriggerId_mNameVersion);
   fChain->SetBranchAddress("MuEvent.mTriggerIdCollection.mL3TriggerId.mThresholdVersion[64]", MuEvent_mTriggerIdCollection_mL3TriggerId_mThresholdVersion, &b_MuEvent_mTriggerIdCollection_mL3TriggerId_mThresholdVersion);
   fChain->SetBranchAddress("MuEvent.mTriggerIdCollection.mL3TriggerId.mPrescaleVersion[64]", MuEvent_mTriggerIdCollection_mL3TriggerId_mPrescaleVersion, &b_MuEvent_mTriggerIdCollection_mL3TriggerId_mPrescaleVersion);
   fChain->SetBranchAddress("MuEvent.mTriggerIdCollection.mNTriggerId.mMask", MuEvent_mTriggerIdCollection_mNTriggerId_mMask, &b_MuEvent_mTriggerIdCollection_mNTriggerId_mMask);
   fChain->SetBranchAddress("MuEvent.mTriggerIdCollection.mNTriggerId.mId[64]", MuEvent_mTriggerIdCollection_mNTriggerId_mId, &b_MuEvent_mTriggerIdCollection_mNTriggerId_mId);
   fChain->SetBranchAddress("MuEvent.mTriggerIdCollection.mNTriggerId.mVersion[64]", MuEvent_mTriggerIdCollection_mNTriggerId_mVersion, &b_MuEvent_mTriggerIdCollection_mNTriggerId_mVersion);
   fChain->SetBranchAddress("MuEvent.mTriggerIdCollection.mNTriggerId.mNameVersion[64]", MuEvent_mTriggerIdCollection_mNTriggerId_mNameVersion, &b_MuEvent_mTriggerIdCollection_mNTriggerId_mNameVersion);
   fChain->SetBranchAddress("MuEvent.mTriggerIdCollection.mNTriggerId.mThresholdVersion[64]", MuEvent_mTriggerIdCollection_mNTriggerId_mThresholdVersion, &b_MuEvent_mTriggerIdCollection_mNTriggerId_mThresholdVersion);
   fChain->SetBranchAddress("MuEvent.mTriggerIdCollection.mNTriggerId.mPrescaleVersion[64]", MuEvent_mTriggerIdCollection_mNTriggerId_mPrescaleVersion, &b_MuEvent_mTriggerIdCollection_mNTriggerId_mPrescaleVersion);
   fChain->SetBranchAddress("MuEvent.mTriggerIdCollection.mLETriggerId.mMask", MuEvent_mTriggerIdCollection_mLETriggerId_mMask, &b_MuEvent_mTriggerIdCollection_mLETriggerId_mMask);
   fChain->SetBranchAddress("MuEvent.mTriggerIdCollection.mLETriggerId.mId[64]", MuEvent_mTriggerIdCollection_mLETriggerId_mId, &b_MuEvent_mTriggerIdCollection_mLETriggerId_mId);
   fChain->SetBranchAddress("MuEvent.mTriggerIdCollection.mLETriggerId.mVersion[64]", MuEvent_mTriggerIdCollection_mLETriggerId_mVersion, &b_MuEvent_mTriggerIdCollection_mLETriggerId_mVersion);
   fChain->SetBranchAddress("MuEvent.mTriggerIdCollection.mLETriggerId.mNameVersion[64]", MuEvent_mTriggerIdCollection_mLETriggerId_mNameVersion, &b_MuEvent_mTriggerIdCollection_mLETriggerId_mNameVersion);
   fChain->SetBranchAddress("MuEvent.mTriggerIdCollection.mLETriggerId.mThresholdVersion[64]", MuEvent_mTriggerIdCollection_mLETriggerId_mThresholdVersion, &b_MuEvent_mTriggerIdCollection_mLETriggerId_mThresholdVersion);
   fChain->SetBranchAddress("MuEvent.mTriggerIdCollection.mLETriggerId.mPrescaleVersion[64]", MuEvent_mTriggerIdCollection_mLETriggerId_mPrescaleVersion, &b_MuEvent_mTriggerIdCollection_mLETriggerId_mPrescaleVersion);
   fChain->SetBranchAddress("MuEvent.mRefMultPos", MuEvent_mRefMultPos, &b_MuEvent_mRefMultPos);
   fChain->SetBranchAddress("MuEvent.mRefMultNeg", MuEvent_mRefMultNeg, &b_MuEvent_mRefMultNeg);
   fChain->SetBranchAddress("MuEvent.mRefMultFtpcEast", MuEvent_mRefMultFtpcEast, &b_MuEvent_mRefMultFtpcEast);
   fChain->SetBranchAddress("MuEvent.mRefMultFtpcWest", MuEvent_mRefMultFtpcWest, &b_MuEvent_mRefMultFtpcWest);
   fChain->SetBranchAddress("MuEvent.mReactionPlane[2]", MuEvent_mReactionPlane, &b_MuEvent_mReactionPlane);
   fChain->SetBranchAddress("MuEvent.mReactionPlanePtWgt[2]", MuEvent_mReactionPlanePtWgt, &b_MuEvent_mReactionPlanePtWgt);
   fChain->SetBranchAddress("MuEvent.mPrimaryVertexError.mX1", MuEvent_mPrimaryVertexError_mX1, &b_MuEvent_mPrimaryVertexError_mX1);
   fChain->SetBranchAddress("MuEvent.mPrimaryVertexError.mX2", MuEvent_mPrimaryVertexError_mX2, &b_MuEvent_mPrimaryVertexError_mX2);
   fChain->SetBranchAddress("MuEvent.mPrimaryVertexError.mX3", MuEvent_mPrimaryVertexError_mX3, &b_MuEvent_mPrimaryVertexError_mX3);
   fChain->SetBranchAddress("MuEvent.mL2Result", MuEvent_mL2Result, &b_MuEvent_mL2Result);
   fChain->SetBranchAddress("MuEvent.mVpdEast", MuEvent_mVpdEast, &b_MuEvent_mVpdEast);
   fChain->SetBranchAddress("MuEvent.mVpdWest", MuEvent_mVpdWest, &b_MuEvent_mVpdWest);
   fChain->SetBranchAddress("MuEvent.mVpdTstart", MuEvent_mVpdTstart, &b_MuEvent_mVpdTstart);
   fChain->SetBranchAddress("MuEvent.mVpdTdiff", MuEvent_mVpdTdiff, &b_MuEvent_mVpdTdiff);
   fChain->SetBranchAddress("MuEvent.mVpdVz", MuEvent_mVpdVz, &b_MuEvent_mVpdVz);
//    fChain->SetBranchAddress("PrimaryVertices", &PrimaryVertices_, &b_PrimaryVertices_);
//    fChain->SetBranchAddress("PrimaryVertices.mFlag", &PrimaryVertices_mFlag, &b_PrimaryVertices_mFlag);
//    fChain->SetBranchAddress("PrimaryVertices.mPosition.mX1", &PrimaryVertices_mPosition_mX1, &b_PrimaryVertices_mPosition_mX1);
//    fChain->SetBranchAddress("PrimaryVertices.mPosition.mX2", &PrimaryVertices_mPosition_mX2, &b_PrimaryVertices_mPosition_mX2);
//    fChain->SetBranchAddress("PrimaryVertices.mPosition.mX3", &PrimaryVertices_mPosition_mX3, &b_PrimaryVertices_mPosition_mX3);
//    fChain->SetBranchAddress("PrimaryVertices.mPosError.mX1", &PrimaryVertices_mPosError_mX1, &b_PrimaryVertices_mPosError_mX1);
//    fChain->SetBranchAddress("PrimaryVertices.mPosError.mX2", &PrimaryVertices_mPosError_mX2, &b_PrimaryVertices_mPosError_mX2);
//    fChain->SetBranchAddress("PrimaryVertices.mPosError.mX3", &PrimaryVertices_mPosError_mX3, &b_PrimaryVertices_mPosError_mX3);
//    fChain->SetBranchAddress("PrimaryVertices.mVertexFinderId", &PrimaryVertices_mVertexFinderId, &b_PrimaryVertices_mVertexFinderId);
//    fChain->SetBranchAddress("PrimaryVertices.mRanking", &PrimaryVertices_mRanking, &b_PrimaryVertices_mRanking);
//    fChain->SetBranchAddress("PrimaryVertices.mNTracksUsed", &PrimaryVertices_mNTracksUsed, &b_PrimaryVertices_mNTracksUsed);
//    fChain->SetBranchAddress("PrimaryVertices.mNBTOFMatch", &PrimaryVertices_mNBTOFMatch, &b_PrimaryVertices_mNBTOFMatch);
//    fChain->SetBranchAddress("PrimaryVertices.mNCTBMatch", &PrimaryVertices_mNCTBMatch, &b_PrimaryVertices_mNCTBMatch);
//    fChain->SetBranchAddress("PrimaryVertices.mNBEMCMatch", &PrimaryVertices_mNBEMCMatch, &b_PrimaryVertices_mNBEMCMatch);
//    fChain->SetBranchAddress("PrimaryVertices.mNEEMCMatch", &PrimaryVertices_mNEEMCMatch, &b_PrimaryVertices_mNEEMCMatch);
//    fChain->SetBranchAddress("PrimaryVertices.mNBTOFNotMatch", &PrimaryVertices_mNBTOFNotMatch, &b_PrimaryVertices_mNBTOFNotMatch);
//    fChain->SetBranchAddress("PrimaryVertices.mNCTBNotMatch", &PrimaryVertices_mNCTBNotMatch, &b_PrimaryVertices_mNCTBNotMatch);
//    fChain->SetBranchAddress("PrimaryVertices.mNBEMCNotMatch", &PrimaryVertices_mNBEMCNotMatch, &b_PrimaryVertices_mNBEMCNotMatch);
//    fChain->SetBranchAddress("PrimaryVertices.mNEEMCNotMatch", &PrimaryVertices_mNEEMCNotMatch, &b_PrimaryVertices_mNEEMCNotMatch);
//    fChain->SetBranchAddress("PrimaryVertices.mNCrossCentralMembrane", &PrimaryVertices_mNCrossCentralMembrane, &b_PrimaryVertices_mNCrossCentralMembrane);
//    fChain->SetBranchAddress("PrimaryVertices.mNTracksWithPromptHit", &PrimaryVertices_mNTracksWithPromptHit, &b_PrimaryVertices_mNTracksWithPromptHit);
//    fChain->SetBranchAddress("PrimaryVertices.mNPostXTracks", &PrimaryVertices_mNPostXTracks, &b_PrimaryVertices_mNPostXTracks);
//    fChain->SetBranchAddress("PrimaryVertices.mSumTrackPt", &PrimaryVertices_mSumTrackPt, &b_PrimaryVertices_mSumTrackPt);
//    fChain->SetBranchAddress("PrimaryVertices.mMeanDip", &PrimaryVertices_mMeanDip, &b_PrimaryVertices_mMeanDip);
//    fChain->SetBranchAddress("PrimaryVertices.mChiSquared", &PrimaryVertices_mChiSquared, &b_PrimaryVertices_mChiSquared);
//    fChain->SetBranchAddress("PrimaryVertices.mNTracks", &PrimaryVertices_mNTracks, &b_PrimaryVertices_mNTracks);
//    fChain->SetBranchAddress("PrimaryVertices.mNTpcWestOnly", &PrimaryVertices_mNTpcWestOnly, &b_PrimaryVertices_mNTpcWestOnly);
//    fChain->SetBranchAddress("PrimaryVertices.mNTpcEastOnly", &PrimaryVertices_mNTpcEastOnly, &b_PrimaryVertices_mNTpcEastOnly);
//    fChain->SetBranchAddress("PrimaryVertices.mRefMultNeg", &PrimaryVertices_mRefMultNeg, &b_PrimaryVertices_mRefMultNeg);
//    fChain->SetBranchAddress("PrimaryVertices.mRefMultPos", &PrimaryVertices_mRefMultPos, &b_PrimaryVertices_mRefMultPos);
//    fChain->SetBranchAddress("PrimaryVertices.mRefMultFtpcWest", &PrimaryVertices_mRefMultFtpcWest, &b_PrimaryVertices_mRefMultFtpcWest);
//    fChain->SetBranchAddress("PrimaryVertices.mRefMultFtpcEast", &PrimaryVertices_mRefMultFtpcEast, &b_PrimaryVertices_mRefMultFtpcEast);
//    fChain->SetBranchAddress("PrimaryVertices.mIdTruth", &PrimaryVertices_mIdTruth, &b_PrimaryVertices_mIdTruth);
//    fChain->SetBranchAddress("PrimaryVertices.mQuality", &PrimaryVertices_mQuality, &b_PrimaryVertices_mQuality);
//    fChain->SetBranchAddress("PrimaryVertices.mIdParent", &PrimaryVertices_mIdParent, &b_PrimaryVertices_mIdParent);
//    fChain->SetBranchAddress("PrimaryTracks", &PrimaryTracks_, &b_PrimaryTracks_);
//    fChain->SetBranchAddress("PrimaryTracks.mId", &PrimaryTracks_mId, &b_PrimaryTracks_mId);
//    fChain->SetBranchAddress("PrimaryTracks.mType", &PrimaryTracks_mType, &b_PrimaryTracks_mType);
//    fChain->SetBranchAddress("PrimaryTracks.mFlag", &PrimaryTracks_mFlag, &b_PrimaryTracks_mFlag);
//    fChain->SetBranchAddress("PrimaryTracks.mFlagExtension", &PrimaryTracks_mFlagExtension, &b_PrimaryTracks_mFlagExtension);
//    fChain->SetBranchAddress("PrimaryTracks.mIndex2Global", &PrimaryTracks_mIndex2Global, &b_PrimaryTracks_mIndex2Global);
//    fChain->SetBranchAddress("PrimaryTracks.mIndex2RichSpectra", &PrimaryTracks_mIndex2RichSpectra, &b_PrimaryTracks_mIndex2RichSpectra);
//    fChain->SetBranchAddress("PrimaryTracks.mIndex2BTofHit", &PrimaryTracks_mIndex2BTofHit, &b_PrimaryTracks_mIndex2BTofHit);
//    fChain->SetBranchAddress("PrimaryTracks.mIndex2MtdHit", &PrimaryTracks_mIndex2MtdHit, &b_PrimaryTracks_mIndex2MtdHit);
//    fChain->SetBranchAddress("PrimaryTracks.mVertexIndex", &PrimaryTracks_mVertexIndex, &b_PrimaryTracks_mVertexIndex);
//    fChain->SetBranchAddress("PrimaryTracks.mNHits", &PrimaryTracks_mNHits, &b_PrimaryTracks_mNHits);
//    fChain->SetBranchAddress("PrimaryTracks.mNHitsPoss", &PrimaryTracks_mNHitsPoss, &b_PrimaryTracks_mNHitsPoss);
//    fChain->SetBranchAddress("PrimaryTracks.mNHitsDedx", &PrimaryTracks_mNHitsDedx, &b_PrimaryTracks_mNHitsDedx);
//    fChain->SetBranchAddress("PrimaryTracks.mNHitsFit", &PrimaryTracks_mNHitsFit, &b_PrimaryTracks_mNHitsFit);
//    fChain->SetBranchAddress("PrimaryTracks.mNHitsPossInner", &PrimaryTracks_mNHitsPossInner, &b_PrimaryTracks_mNHitsPossInner);
//    fChain->SetBranchAddress("PrimaryTracks.mNHitsFitInner", &PrimaryTracks_mNHitsFitInner, &b_PrimaryTracks_mNHitsFitInner);
//    fChain->SetBranchAddress("PrimaryTracks.mNHitsPossTpc", &PrimaryTracks_mNHitsPossTpc, &b_PrimaryTracks_mNHitsPossTpc);
//    fChain->SetBranchAddress("PrimaryTracks.mNHitsFitTpc", &PrimaryTracks_mNHitsFitTpc, &b_PrimaryTracks_mNHitsFitTpc);
//    fChain->SetBranchAddress("PrimaryTracks.mPidProbElectron", &PrimaryTracks_mPidProbElectron, &b_PrimaryTracks_mPidProbElectron);
//    fChain->SetBranchAddress("PrimaryTracks.mPidProbPion", &PrimaryTracks_mPidProbPion, &b_PrimaryTracks_mPidProbPion);
//    fChain->SetBranchAddress("PrimaryTracks.mPidProbKaon", &PrimaryTracks_mPidProbKaon, &b_PrimaryTracks_mPidProbKaon);
//    fChain->SetBranchAddress("PrimaryTracks.mPidProbProton", &PrimaryTracks_mPidProbProton, &b_PrimaryTracks_mPidProbProton);
//    fChain->SetBranchAddress("PrimaryTracks.mNSigmaElectron", &PrimaryTracks_mNSigmaElectron, &b_PrimaryTracks_mNSigmaElectron);
//    fChain->SetBranchAddress("PrimaryTracks.mNSigmaPion", &PrimaryTracks_mNSigmaPion, &b_PrimaryTracks_mNSigmaPion);
//    fChain->SetBranchAddress("PrimaryTracks.mNSigmaKaon", &PrimaryTracks_mNSigmaKaon, &b_PrimaryTracks_mNSigmaKaon);
//    fChain->SetBranchAddress("PrimaryTracks.mNSigmaProton", &PrimaryTracks_mNSigmaProton, &b_PrimaryTracks_mNSigmaProton);
//    fChain->SetBranchAddress("PrimaryTracks.mdEdx", &PrimaryTracks_mdEdx, &b_PrimaryTracks_mdEdx);
//    fChain->SetBranchAddress("PrimaryTracks.mChiSqXY", &PrimaryTracks_mChiSqXY, &b_PrimaryTracks_mChiSqXY);
//    fChain->SetBranchAddress("PrimaryTracks.mChiSqZ", &PrimaryTracks_mChiSqZ, &b_PrimaryTracks_mChiSqZ);
//    fChain->SetBranchAddress("PrimaryTracks.mPt", &PrimaryTracks_mPt, &b_PrimaryTracks_mPt);
//    fChain->SetBranchAddress("PrimaryTracks.mEta", &PrimaryTracks_mEta, &b_PrimaryTracks_mEta);
//    fChain->SetBranchAddress("PrimaryTracks.mPhi", &PrimaryTracks_mPhi, &b_PrimaryTracks_mPhi);
//    fChain->SetBranchAddress("PrimaryTracks.mTopologyMap.mMap0", &PrimaryTracks_mTopologyMap_mMap0, &b_PrimaryTracks_mTopologyMap_mMap0);
//    fChain->SetBranchAddress("PrimaryTracks.mTopologyMap.mMap1", &PrimaryTracks_mTopologyMap_mMap1, &b_PrimaryTracks_mTopologyMap_mMap1);
//    fChain->SetBranchAddress("PrimaryTracks.mP.mX1", &PrimaryTracks_mP_mX1, &b_PrimaryTracks_mP_mX1);
//    fChain->SetBranchAddress("PrimaryTracks.mP.mX2", &PrimaryTracks_mP_mX2, &b_PrimaryTracks_mP_mX2);
//    fChain->SetBranchAddress("PrimaryTracks.mP.mX3", &PrimaryTracks_mP_mX3, &b_PrimaryTracks_mP_mX3);
//    fChain->SetBranchAddress("PrimaryTracks.mDCA.mX1", &PrimaryTracks_mDCA_mX1, &b_PrimaryTracks_mDCA_mX1);
//    fChain->SetBranchAddress("PrimaryTracks.mDCA.mX2", &PrimaryTracks_mDCA_mX2, &b_PrimaryTracks_mDCA_mX2);
//    fChain->SetBranchAddress("PrimaryTracks.mDCA.mX3", &PrimaryTracks_mDCA_mX3, &b_PrimaryTracks_mDCA_mX3);
//    fChain->SetBranchAddress("PrimaryTracks.mDCAGlobal.mX1", &PrimaryTracks_mDCAGlobal_mX1, &b_PrimaryTracks_mDCAGlobal_mX1);
//    fChain->SetBranchAddress("PrimaryTracks.mDCAGlobal.mX2", &PrimaryTracks_mDCAGlobal_mX2, &b_PrimaryTracks_mDCAGlobal_mX2);
//    fChain->SetBranchAddress("PrimaryTracks.mDCAGlobal.mX3", &PrimaryTracks_mDCAGlobal_mX3, &b_PrimaryTracks_mDCAGlobal_mX3);
//    fChain->SetBranchAddress("PrimaryTracks.mFirstPoint.mX1", &PrimaryTracks_mFirstPoint_mX1, &b_PrimaryTracks_mFirstPoint_mX1);
//    fChain->SetBranchAddress("PrimaryTracks.mFirstPoint.mX2", &PrimaryTracks_mFirstPoint_mX2, &b_PrimaryTracks_mFirstPoint_mX2);
//    fChain->SetBranchAddress("PrimaryTracks.mFirstPoint.mX3", &PrimaryTracks_mFirstPoint_mX3, &b_PrimaryTracks_mFirstPoint_mX3);
//    fChain->SetBranchAddress("PrimaryTracks.mLastPoint.mX1", &PrimaryTracks_mLastPoint_mX1, &b_PrimaryTracks_mLastPoint_mX1);
//    fChain->SetBranchAddress("PrimaryTracks.mLastPoint.mX2", &PrimaryTracks_mLastPoint_mX2, &b_PrimaryTracks_mLastPoint_mX2);
//    fChain->SetBranchAddress("PrimaryTracks.mLastPoint.mX3", &PrimaryTracks_mLastPoint_mX3, &b_PrimaryTracks_mLastPoint_mX3);
//    fChain->SetBranchAddress("PrimaryTracks.mHelix.mP.mX1", &PrimaryTracks_mHelix_mP_mX1, &b_PrimaryTracks_mHelix_mP_mX1);
//    fChain->SetBranchAddress("PrimaryTracks.mHelix.mP.mX2", &PrimaryTracks_mHelix_mP_mX2, &b_PrimaryTracks_mHelix_mP_mX2);
//    fChain->SetBranchAddress("PrimaryTracks.mHelix.mP.mX3", &PrimaryTracks_mHelix_mP_mX3, &b_PrimaryTracks_mHelix_mP_mX3);
//    fChain->SetBranchAddress("PrimaryTracks.mHelix.mOrigin.mX1", &PrimaryTracks_mHelix_mOrigin_mX1, &b_PrimaryTracks_mHelix_mOrigin_mX1);
//    fChain->SetBranchAddress("PrimaryTracks.mHelix.mOrigin.mX2", &PrimaryTracks_mHelix_mOrigin_mX2, &b_PrimaryTracks_mHelix_mOrigin_mX2);
//    fChain->SetBranchAddress("PrimaryTracks.mHelix.mOrigin.mX3", &PrimaryTracks_mHelix_mOrigin_mX3, &b_PrimaryTracks_mHelix_mOrigin_mX3);
//    fChain->SetBranchAddress("PrimaryTracks.mHelix.mQ", &PrimaryTracks_mHelix_mQ, &b_PrimaryTracks_mHelix_mQ);
//    fChain->SetBranchAddress("PrimaryTracks.mHelix.mB", &PrimaryTracks_mHelix_mB, &b_PrimaryTracks_mHelix_mB);
//    fChain->SetBranchAddress("PrimaryTracks.mOuterHelix.mP.mX1", &PrimaryTracks_mOuterHelix_mP_mX1, &b_PrimaryTracks_mOuterHelix_mP_mX1);
//    fChain->SetBranchAddress("PrimaryTracks.mOuterHelix.mP.mX2", &PrimaryTracks_mOuterHelix_mP_mX2, &b_PrimaryTracks_mOuterHelix_mP_mX2);
//    fChain->SetBranchAddress("PrimaryTracks.mOuterHelix.mP.mX3", &PrimaryTracks_mOuterHelix_mP_mX3, &b_PrimaryTracks_mOuterHelix_mP_mX3);
//    fChain->SetBranchAddress("PrimaryTracks.mOuterHelix.mOrigin.mX1", &PrimaryTracks_mOuterHelix_mOrigin_mX1, &b_PrimaryTracks_mOuterHelix_mOrigin_mX1);
//    fChain->SetBranchAddress("PrimaryTracks.mOuterHelix.mOrigin.mX2", &PrimaryTracks_mOuterHelix_mOrigin_mX2, &b_PrimaryTracks_mOuterHelix_mOrigin_mX2);
//    fChain->SetBranchAddress("PrimaryTracks.mOuterHelix.mOrigin.mX3", &PrimaryTracks_mOuterHelix_mOrigin_mX3, &b_PrimaryTracks_mOuterHelix_mOrigin_mX3);
//    fChain->SetBranchAddress("PrimaryTracks.mOuterHelix.mQ", &PrimaryTracks_mOuterHelix_mQ, &b_PrimaryTracks_mOuterHelix_mQ);
//    fChain->SetBranchAddress("PrimaryTracks.mOuterHelix.mB", &PrimaryTracks_mOuterHelix_mB, &b_PrimaryTracks_mOuterHelix_mB);
//    fChain->SetBranchAddress("PrimaryTracks.mProbPidTraits.mNDF", &PrimaryTracks_mProbPidTraits_mNDF, &b_PrimaryTracks_mProbPidTraits_mNDF);
//    fChain->SetBranchAddress("PrimaryTracks.mProbPidTraits.mdEdxFit", &PrimaryTracks_mProbPidTraits_mdEdxFit, &b_PrimaryTracks_mProbPidTraits_mdEdxFit);
//    fChain->SetBranchAddress("PrimaryTracks.mProbPidTraits.mdEdxErrorFit", &PrimaryTracks_mProbPidTraits_mdEdxErrorFit, &b_PrimaryTracks_mProbPidTraits_mdEdxErrorFit);
//    fChain->SetBranchAddress("PrimaryTracks.mProbPidTraits.mdEdxTruncated", &PrimaryTracks_mProbPidTraits_mdEdxTruncated, &b_PrimaryTracks_mProbPidTraits_mdEdxTruncated);
//    fChain->SetBranchAddress("PrimaryTracks.mProbPidTraits.mdEdxErrorTruncated", &PrimaryTracks_mProbPidTraits_mdEdxErrorTruncated, &b_PrimaryTracks_mProbPidTraits_mdEdxErrorTruncated);
//    fChain->SetBranchAddress("PrimaryTracks.mProbPidTraits.mdEdxTrackLength", &PrimaryTracks_mProbPidTraits_mdEdxTrackLength, &b_PrimaryTracks_mProbPidTraits_mdEdxTrackLength);
//    fChain->SetBranchAddress("PrimaryTracks.mProbPidTraits.mLog2dX", &PrimaryTracks_mProbPidTraits_mLog2dX, &b_PrimaryTracks_mProbPidTraits_mLog2dX);
//    fChain->SetBranchAddress("PrimaryTracks.mProbPidTraits.mProbabilities[9]", &PrimaryTracks_mProbPidTraits_mProbabilities, &b_PrimaryTracks_mProbPidTraits_mProbabilities);
//    fChain->SetBranchAddress("PrimaryTracks.mBTofPidTraits.mMatchFlag", &PrimaryTracks_mBTofPidTraits_mMatchFlag, &b_PrimaryTracks_mBTofPidTraits_mMatchFlag);
//    fChain->SetBranchAddress("PrimaryTracks.mBTofPidTraits.mYLocal", &PrimaryTracks_mBTofPidTraits_mYLocal, &b_PrimaryTracks_mBTofPidTraits_mYLocal);
//    fChain->SetBranchAddress("PrimaryTracks.mBTofPidTraits.mZLocal", &PrimaryTracks_mBTofPidTraits_mZLocal, &b_PrimaryTracks_mBTofPidTraits_mZLocal);
//    fChain->SetBranchAddress("PrimaryTracks.mBTofPidTraits.mThetaLocal", &PrimaryTracks_mBTofPidTraits_mThetaLocal, &b_PrimaryTracks_mBTofPidTraits_mThetaLocal);
//    fChain->SetBranchAddress("PrimaryTracks.mBTofPidTraits.mPosition.mX1", &PrimaryTracks_mBTofPidTraits_mPosition_mX1, &b_PrimaryTracks_mBTofPidTraits_mPosition_mX1);
//    fChain->SetBranchAddress("PrimaryTracks.mBTofPidTraits.mPosition.mX2", &PrimaryTracks_mBTofPidTraits_mPosition_mX2, &b_PrimaryTracks_mBTofPidTraits_mPosition_mX2);
//    fChain->SetBranchAddress("PrimaryTracks.mBTofPidTraits.mPosition.mX3", &PrimaryTracks_mBTofPidTraits_mPosition_mX3, &b_PrimaryTracks_mBTofPidTraits_mPosition_mX3);
//    fChain->SetBranchAddress("PrimaryTracks.mBTofPidTraits.mTimeOfFlight", &PrimaryTracks_mBTofPidTraits_mTimeOfFlight, &b_PrimaryTracks_mBTofPidTraits_mTimeOfFlight);
//    fChain->SetBranchAddress("PrimaryTracks.mBTofPidTraits.mPathLength", &PrimaryTracks_mBTofPidTraits_mPathLength, &b_PrimaryTracks_mBTofPidTraits_mPathLength);
//    fChain->SetBranchAddress("PrimaryTracks.mBTofPidTraits.mBeta", &PrimaryTracks_mBTofPidTraits_mBeta, &b_PrimaryTracks_mBTofPidTraits_mBeta);
//    fChain->SetBranchAddress("PrimaryTracks.mBTofPidTraits.mSigmaElectron", &PrimaryTracks_mBTofPidTraits_mSigmaElectron, &b_PrimaryTracks_mBTofPidTraits_mSigmaElectron);
//    fChain->SetBranchAddress("PrimaryTracks.mBTofPidTraits.mSigmaPion", &PrimaryTracks_mBTofPidTraits_mSigmaPion, &b_PrimaryTracks_mBTofPidTraits_mSigmaPion);
//    fChain->SetBranchAddress("PrimaryTracks.mBTofPidTraits.mSigmaKaon", &PrimaryTracks_mBTofPidTraits_mSigmaKaon, &b_PrimaryTracks_mBTofPidTraits_mSigmaKaon);
//    fChain->SetBranchAddress("PrimaryTracks.mBTofPidTraits.mSigmaProton", &PrimaryTracks_mBTofPidTraits_mSigmaProton, &b_PrimaryTracks_mBTofPidTraits_mSigmaProton);
//    fChain->SetBranchAddress("PrimaryTracks.mBTofPidTraits.mProbElectron", &PrimaryTracks_mBTofPidTraits_mProbElectron, &b_PrimaryTracks_mBTofPidTraits_mProbElectron);
//    fChain->SetBranchAddress("PrimaryTracks.mBTofPidTraits.mProbPion", &PrimaryTracks_mBTofPidTraits_mProbPion, &b_PrimaryTracks_mBTofPidTraits_mProbPion);
//    fChain->SetBranchAddress("PrimaryTracks.mBTofPidTraits.mProbKaon", &PrimaryTracks_mBTofPidTraits_mProbKaon, &b_PrimaryTracks_mBTofPidTraits_mProbKaon);
//    fChain->SetBranchAddress("PrimaryTracks.mBTofPidTraits.mProbProton", &PrimaryTracks_mBTofPidTraits_mProbProton, &b_PrimaryTracks_mBTofPidTraits_mProbProton);
//    fChain->SetBranchAddress("PrimaryTracks.mMtdPidTraits.mMatchFlag", &PrimaryTracks_mMtdPidTraits_mMatchFlag, &b_PrimaryTracks_mMtdPidTraits_mMatchFlag);
//    fChain->SetBranchAddress("PrimaryTracks.mMtdPidTraits.mYLocal", &PrimaryTracks_mMtdPidTraits_mYLocal, &b_PrimaryTracks_mMtdPidTraits_mYLocal);
//    fChain->SetBranchAddress("PrimaryTracks.mMtdPidTraits.mZLocal", &PrimaryTracks_mMtdPidTraits_mZLocal, &b_PrimaryTracks_mMtdPidTraits_mZLocal);
//    fChain->SetBranchAddress("PrimaryTracks.mMtdPidTraits.mThetaLocal", &PrimaryTracks_mMtdPidTraits_mThetaLocal, &b_PrimaryTracks_mMtdPidTraits_mThetaLocal);
//    fChain->SetBranchAddress("PrimaryTracks.mMtdPidTraits.mPosition.mX1", &PrimaryTracks_mMtdPidTraits_mPosition_mX1, &b_PrimaryTracks_mMtdPidTraits_mPosition_mX1);
//    fChain->SetBranchAddress("PrimaryTracks.mMtdPidTraits.mPosition.mX2", &PrimaryTracks_mMtdPidTraits_mPosition_mX2, &b_PrimaryTracks_mMtdPidTraits_mPosition_mX2);
//    fChain->SetBranchAddress("PrimaryTracks.mMtdPidTraits.mPosition.mX3", &PrimaryTracks_mMtdPidTraits_mPosition_mX3, &b_PrimaryTracks_mMtdPidTraits_mPosition_mX3);
//    fChain->SetBranchAddress("PrimaryTracks.mMtdPidTraits.mTimeOfFlight", &PrimaryTracks_mMtdPidTraits_mTimeOfFlight, &b_PrimaryTracks_mMtdPidTraits_mTimeOfFlight);
//    fChain->SetBranchAddress("PrimaryTracks.mMtdPidTraits.mPathLength", &PrimaryTracks_mMtdPidTraits_mPathLength, &b_PrimaryTracks_mMtdPidTraits_mPathLength);
//    fChain->SetBranchAddress("PrimaryTracks.mMtdPidTraits.mBeta", &PrimaryTracks_mMtdPidTraits_mBeta, &b_PrimaryTracks_mMtdPidTraits_mBeta);
//    fChain->SetBranchAddress("PrimaryTracks.mMtdPidTraits.mSigmaMuon", &PrimaryTracks_mMtdPidTraits_mSigmaMuon, &b_PrimaryTracks_mMtdPidTraits_mSigmaMuon);
//    fChain->SetBranchAddress("PrimaryTracks.mMtdPidTraits.mProbMuon", &PrimaryTracks_mMtdPidTraits_mProbMuon, &b_PrimaryTracks_mMtdPidTraits_mProbMuon);
//    fChain->SetBranchAddress("PrimaryTracks.mIndex2Cov", &PrimaryTracks_mIndex2Cov, &b_PrimaryTracks_mIndex2Cov);
//    fChain->SetBranchAddress("PrimaryTracks.mIdTruth", &PrimaryTracks_mIdTruth, &b_PrimaryTracks_mIdTruth);
//    fChain->SetBranchAddress("PrimaryTracks.mQuality", &PrimaryTracks_mQuality, &b_PrimaryTracks_mQuality);
//    fChain->SetBranchAddress("PrimaryTracks.mIdParentVx", &PrimaryTracks_mIdParentVx, &b_PrimaryTracks_mIdParentVx);
//    fChain->SetBranchAddress("GlobalTracks", &GlobalTracks_, &b_GlobalTracks_);
//    fChain->SetBranchAddress("GlobalTracks.mId", &GlobalTracks_mId, &b_GlobalTracks_mId);
//    fChain->SetBranchAddress("GlobalTracks.mType", &GlobalTracks_mType, &b_GlobalTracks_mType);
//    fChain->SetBranchAddress("GlobalTracks.mFlag", &GlobalTracks_mFlag, &b_GlobalTracks_mFlag);
//    fChain->SetBranchAddress("GlobalTracks.mFlagExtension", &GlobalTracks_mFlagExtension, &b_GlobalTracks_mFlagExtension);
//    fChain->SetBranchAddress("GlobalTracks.mIndex2Global", &GlobalTracks_mIndex2Global, &b_GlobalTracks_mIndex2Global);
//    fChain->SetBranchAddress("GlobalTracks.mIndex2RichSpectra", &GlobalTracks_mIndex2RichSpectra, &b_GlobalTracks_mIndex2RichSpectra);
//    fChain->SetBranchAddress("GlobalTracks.mIndex2BTofHit", &GlobalTracks_mIndex2BTofHit, &b_GlobalTracks_mIndex2BTofHit);
//    fChain->SetBranchAddress("GlobalTracks.mIndex2MtdHit", &GlobalTracks_mIndex2MtdHit, &b_GlobalTracks_mIndex2MtdHit);
//    fChain->SetBranchAddress("GlobalTracks.mVertexIndex", &GlobalTracks_mVertexIndex, &b_GlobalTracks_mVertexIndex);
//    fChain->SetBranchAddress("GlobalTracks.mNHits", &GlobalTracks_mNHits, &b_GlobalTracks_mNHits);
//    fChain->SetBranchAddress("GlobalTracks.mNHitsPoss", &GlobalTracks_mNHitsPoss, &b_GlobalTracks_mNHitsPoss);
//    fChain->SetBranchAddress("GlobalTracks.mNHitsDedx", &GlobalTracks_mNHitsDedx, &b_GlobalTracks_mNHitsDedx);
//    fChain->SetBranchAddress("GlobalTracks.mNHitsFit", &GlobalTracks_mNHitsFit, &b_GlobalTracks_mNHitsFit);
//    fChain->SetBranchAddress("GlobalTracks.mNHitsPossInner", &GlobalTracks_mNHitsPossInner, &b_GlobalTracks_mNHitsPossInner);
//    fChain->SetBranchAddress("GlobalTracks.mNHitsFitInner", &GlobalTracks_mNHitsFitInner, &b_GlobalTracks_mNHitsFitInner);
//    fChain->SetBranchAddress("GlobalTracks.mNHitsPossTpc", &GlobalTracks_mNHitsPossTpc, &b_GlobalTracks_mNHitsPossTpc);
//    fChain->SetBranchAddress("GlobalTracks.mNHitsFitTpc", &GlobalTracks_mNHitsFitTpc, &b_GlobalTracks_mNHitsFitTpc);
//    fChain->SetBranchAddress("GlobalTracks.mPidProbElectron", &GlobalTracks_mPidProbElectron, &b_GlobalTracks_mPidProbElectron);
//    fChain->SetBranchAddress("GlobalTracks.mPidProbPion", &GlobalTracks_mPidProbPion, &b_GlobalTracks_mPidProbPion);
//    fChain->SetBranchAddress("GlobalTracks.mPidProbKaon", &GlobalTracks_mPidProbKaon, &b_GlobalTracks_mPidProbKaon);
//    fChain->SetBranchAddress("GlobalTracks.mPidProbProton", &GlobalTracks_mPidProbProton, &b_GlobalTracks_mPidProbProton);
//    fChain->SetBranchAddress("GlobalTracks.mNSigmaElectron", &GlobalTracks_mNSigmaElectron, &b_GlobalTracks_mNSigmaElectron);
//    fChain->SetBranchAddress("GlobalTracks.mNSigmaPion", &GlobalTracks_mNSigmaPion, &b_GlobalTracks_mNSigmaPion);
//    fChain->SetBranchAddress("GlobalTracks.mNSigmaKaon", &GlobalTracks_mNSigmaKaon, &b_GlobalTracks_mNSigmaKaon);
//    fChain->SetBranchAddress("GlobalTracks.mNSigmaProton", &GlobalTracks_mNSigmaProton, &b_GlobalTracks_mNSigmaProton);
//    fChain->SetBranchAddress("GlobalTracks.mdEdx", &GlobalTracks_mdEdx, &b_GlobalTracks_mdEdx);
//    fChain->SetBranchAddress("GlobalTracks.mChiSqXY", &GlobalTracks_mChiSqXY, &b_GlobalTracks_mChiSqXY);
//    fChain->SetBranchAddress("GlobalTracks.mChiSqZ", &GlobalTracks_mChiSqZ, &b_GlobalTracks_mChiSqZ);
//    fChain->SetBranchAddress("GlobalTracks.mPt", &GlobalTracks_mPt, &b_GlobalTracks_mPt);
//    fChain->SetBranchAddress("GlobalTracks.mEta", &GlobalTracks_mEta, &b_GlobalTracks_mEta);
//    fChain->SetBranchAddress("GlobalTracks.mPhi", &GlobalTracks_mPhi, &b_GlobalTracks_mPhi);
//    fChain->SetBranchAddress("GlobalTracks.mTopologyMap.mMap0", &GlobalTracks_mTopologyMap_mMap0, &b_GlobalTracks_mTopologyMap_mMap0);
//    fChain->SetBranchAddress("GlobalTracks.mTopologyMap.mMap1", &GlobalTracks_mTopologyMap_mMap1, &b_GlobalTracks_mTopologyMap_mMap1);
//    fChain->SetBranchAddress("GlobalTracks.mP.mX1", &GlobalTracks_mP_mX1, &b_GlobalTracks_mP_mX1);
//    fChain->SetBranchAddress("GlobalTracks.mP.mX2", &GlobalTracks_mP_mX2, &b_GlobalTracks_mP_mX2);
//    fChain->SetBranchAddress("GlobalTracks.mP.mX3", &GlobalTracks_mP_mX3, &b_GlobalTracks_mP_mX3);
//    fChain->SetBranchAddress("GlobalTracks.mDCA.mX1", &GlobalTracks_mDCA_mX1, &b_GlobalTracks_mDCA_mX1);
//    fChain->SetBranchAddress("GlobalTracks.mDCA.mX2", &GlobalTracks_mDCA_mX2, &b_GlobalTracks_mDCA_mX2);
//    fChain->SetBranchAddress("GlobalTracks.mDCA.mX3", &GlobalTracks_mDCA_mX3, &b_GlobalTracks_mDCA_mX3);
//    fChain->SetBranchAddress("GlobalTracks.mDCAGlobal.mX1", &GlobalTracks_mDCAGlobal_mX1, &b_GlobalTracks_mDCAGlobal_mX1);
//    fChain->SetBranchAddress("GlobalTracks.mDCAGlobal.mX2", &GlobalTracks_mDCAGlobal_mX2, &b_GlobalTracks_mDCAGlobal_mX2);
//    fChain->SetBranchAddress("GlobalTracks.mDCAGlobal.mX3", &GlobalTracks_mDCAGlobal_mX3, &b_GlobalTracks_mDCAGlobal_mX3);
//    fChain->SetBranchAddress("GlobalTracks.mFirstPoint.mX1", &GlobalTracks_mFirstPoint_mX1, &b_GlobalTracks_mFirstPoint_mX1);
//    fChain->SetBranchAddress("GlobalTracks.mFirstPoint.mX2", &GlobalTracks_mFirstPoint_mX2, &b_GlobalTracks_mFirstPoint_mX2);
//    fChain->SetBranchAddress("GlobalTracks.mFirstPoint.mX3", &GlobalTracks_mFirstPoint_mX3, &b_GlobalTracks_mFirstPoint_mX3);
//    fChain->SetBranchAddress("GlobalTracks.mLastPoint.mX1", &GlobalTracks_mLastPoint_mX1, &b_GlobalTracks_mLastPoint_mX1);
//    fChain->SetBranchAddress("GlobalTracks.mLastPoint.mX2", &GlobalTracks_mLastPoint_mX2, &b_GlobalTracks_mLastPoint_mX2);
//    fChain->SetBranchAddress("GlobalTracks.mLastPoint.mX3", &GlobalTracks_mLastPoint_mX3, &b_GlobalTracks_mLastPoint_mX3);
//    fChain->SetBranchAddress("GlobalTracks.mHelix.mP.mX1", &GlobalTracks_mHelix_mP_mX1, &b_GlobalTracks_mHelix_mP_mX1);
//    fChain->SetBranchAddress("GlobalTracks.mHelix.mP.mX2", &GlobalTracks_mHelix_mP_mX2, &b_GlobalTracks_mHelix_mP_mX2);
//    fChain->SetBranchAddress("GlobalTracks.mHelix.mP.mX3", &GlobalTracks_mHelix_mP_mX3, &b_GlobalTracks_mHelix_mP_mX3);
//    fChain->SetBranchAddress("GlobalTracks.mHelix.mOrigin.mX1", &GlobalTracks_mHelix_mOrigin_mX1, &b_GlobalTracks_mHelix_mOrigin_mX1);
//    fChain->SetBranchAddress("GlobalTracks.mHelix.mOrigin.mX2", &GlobalTracks_mHelix_mOrigin_mX2, &b_GlobalTracks_mHelix_mOrigin_mX2);
//    fChain->SetBranchAddress("GlobalTracks.mHelix.mOrigin.mX3", &GlobalTracks_mHelix_mOrigin_mX3, &b_GlobalTracks_mHelix_mOrigin_mX3);
//    fChain->SetBranchAddress("GlobalTracks.mHelix.mQ", &GlobalTracks_mHelix_mQ, &b_GlobalTracks_mHelix_mQ);
//    fChain->SetBranchAddress("GlobalTracks.mHelix.mB", &GlobalTracks_mHelix_mB, &b_GlobalTracks_mHelix_mB);
//    fChain->SetBranchAddress("GlobalTracks.mOuterHelix.mP.mX1", &GlobalTracks_mOuterHelix_mP_mX1, &b_GlobalTracks_mOuterHelix_mP_mX1);
//    fChain->SetBranchAddress("GlobalTracks.mOuterHelix.mP.mX2", &GlobalTracks_mOuterHelix_mP_mX2, &b_GlobalTracks_mOuterHelix_mP_mX2);
//    fChain->SetBranchAddress("GlobalTracks.mOuterHelix.mP.mX3", &GlobalTracks_mOuterHelix_mP_mX3, &b_GlobalTracks_mOuterHelix_mP_mX3);
//    fChain->SetBranchAddress("GlobalTracks.mOuterHelix.mOrigin.mX1", &GlobalTracks_mOuterHelix_mOrigin_mX1, &b_GlobalTracks_mOuterHelix_mOrigin_mX1);
//    fChain->SetBranchAddress("GlobalTracks.mOuterHelix.mOrigin.mX2", &GlobalTracks_mOuterHelix_mOrigin_mX2, &b_GlobalTracks_mOuterHelix_mOrigin_mX2);
//    fChain->SetBranchAddress("GlobalTracks.mOuterHelix.mOrigin.mX3", &GlobalTracks_mOuterHelix_mOrigin_mX3, &b_GlobalTracks_mOuterHelix_mOrigin_mX3);
//    fChain->SetBranchAddress("GlobalTracks.mOuterHelix.mQ", &GlobalTracks_mOuterHelix_mQ, &b_GlobalTracks_mOuterHelix_mQ);
//    fChain->SetBranchAddress("GlobalTracks.mOuterHelix.mB", &GlobalTracks_mOuterHelix_mB, &b_GlobalTracks_mOuterHelix_mB);
//    fChain->SetBranchAddress("GlobalTracks.mProbPidTraits.mNDF", &GlobalTracks_mProbPidTraits_mNDF, &b_GlobalTracks_mProbPidTraits_mNDF);
//    fChain->SetBranchAddress("GlobalTracks.mProbPidTraits.mdEdxFit", &GlobalTracks_mProbPidTraits_mdEdxFit, &b_GlobalTracks_mProbPidTraits_mdEdxFit);
//    fChain->SetBranchAddress("GlobalTracks.mProbPidTraits.mdEdxErrorFit", &GlobalTracks_mProbPidTraits_mdEdxErrorFit, &b_GlobalTracks_mProbPidTraits_mdEdxErrorFit);
//    fChain->SetBranchAddress("GlobalTracks.mProbPidTraits.mdEdxTruncated", &GlobalTracks_mProbPidTraits_mdEdxTruncated, &b_GlobalTracks_mProbPidTraits_mdEdxTruncated);
//    fChain->SetBranchAddress("GlobalTracks.mProbPidTraits.mdEdxErrorTruncated", &GlobalTracks_mProbPidTraits_mdEdxErrorTruncated, &b_GlobalTracks_mProbPidTraits_mdEdxErrorTruncated);
//    fChain->SetBranchAddress("GlobalTracks.mProbPidTraits.mdEdxTrackLength", &GlobalTracks_mProbPidTraits_mdEdxTrackLength, &b_GlobalTracks_mProbPidTraits_mdEdxTrackLength);
//    fChain->SetBranchAddress("GlobalTracks.mProbPidTraits.mLog2dX", &GlobalTracks_mProbPidTraits_mLog2dX, &b_GlobalTracks_mProbPidTraits_mLog2dX);
//    fChain->SetBranchAddress("GlobalTracks.mProbPidTraits.mProbabilities[9]", &GlobalTracks_mProbPidTraits_mProbabilities, &b_GlobalTracks_mProbPidTraits_mProbabilities);
//    fChain->SetBranchAddress("GlobalTracks.mBTofPidTraits.mMatchFlag", &GlobalTracks_mBTofPidTraits_mMatchFlag, &b_GlobalTracks_mBTofPidTraits_mMatchFlag);
//    fChain->SetBranchAddress("GlobalTracks.mBTofPidTraits.mYLocal", &GlobalTracks_mBTofPidTraits_mYLocal, &b_GlobalTracks_mBTofPidTraits_mYLocal);
//    fChain->SetBranchAddress("GlobalTracks.mBTofPidTraits.mZLocal", &GlobalTracks_mBTofPidTraits_mZLocal, &b_GlobalTracks_mBTofPidTraits_mZLocal);
//    fChain->SetBranchAddress("GlobalTracks.mBTofPidTraits.mThetaLocal", &GlobalTracks_mBTofPidTraits_mThetaLocal, &b_GlobalTracks_mBTofPidTraits_mThetaLocal);
//    fChain->SetBranchAddress("GlobalTracks.mBTofPidTraits.mPosition.mX1", &GlobalTracks_mBTofPidTraits_mPosition_mX1, &b_GlobalTracks_mBTofPidTraits_mPosition_mX1);
//    fChain->SetBranchAddress("GlobalTracks.mBTofPidTraits.mPosition.mX2", &GlobalTracks_mBTofPidTraits_mPosition_mX2, &b_GlobalTracks_mBTofPidTraits_mPosition_mX2);
//    fChain->SetBranchAddress("GlobalTracks.mBTofPidTraits.mPosition.mX3", &GlobalTracks_mBTofPidTraits_mPosition_mX3, &b_GlobalTracks_mBTofPidTraits_mPosition_mX3);
//    fChain->SetBranchAddress("GlobalTracks.mBTofPidTraits.mTimeOfFlight", &GlobalTracks_mBTofPidTraits_mTimeOfFlight, &b_GlobalTracks_mBTofPidTraits_mTimeOfFlight);
//    fChain->SetBranchAddress("GlobalTracks.mBTofPidTraits.mPathLength", &GlobalTracks_mBTofPidTraits_mPathLength, &b_GlobalTracks_mBTofPidTraits_mPathLength);
//    fChain->SetBranchAddress("GlobalTracks.mBTofPidTraits.mBeta", &GlobalTracks_mBTofPidTraits_mBeta, &b_GlobalTracks_mBTofPidTraits_mBeta);
//    fChain->SetBranchAddress("GlobalTracks.mBTofPidTraits.mSigmaElectron", &GlobalTracks_mBTofPidTraits_mSigmaElectron, &b_GlobalTracks_mBTofPidTraits_mSigmaElectron);
//    fChain->SetBranchAddress("GlobalTracks.mBTofPidTraits.mSigmaPion", &GlobalTracks_mBTofPidTraits_mSigmaPion, &b_GlobalTracks_mBTofPidTraits_mSigmaPion);
//    fChain->SetBranchAddress("GlobalTracks.mBTofPidTraits.mSigmaKaon", &GlobalTracks_mBTofPidTraits_mSigmaKaon, &b_GlobalTracks_mBTofPidTraits_mSigmaKaon);
//    fChain->SetBranchAddress("GlobalTracks.mBTofPidTraits.mSigmaProton", &GlobalTracks_mBTofPidTraits_mSigmaProton, &b_GlobalTracks_mBTofPidTraits_mSigmaProton);
//    fChain->SetBranchAddress("GlobalTracks.mBTofPidTraits.mProbElectron", &GlobalTracks_mBTofPidTraits_mProbElectron, &b_GlobalTracks_mBTofPidTraits_mProbElectron);
//    fChain->SetBranchAddress("GlobalTracks.mBTofPidTraits.mProbPion", &GlobalTracks_mBTofPidTraits_mProbPion, &b_GlobalTracks_mBTofPidTraits_mProbPion);
//    fChain->SetBranchAddress("GlobalTracks.mBTofPidTraits.mProbKaon", &GlobalTracks_mBTofPidTraits_mProbKaon, &b_GlobalTracks_mBTofPidTraits_mProbKaon);
//    fChain->SetBranchAddress("GlobalTracks.mBTofPidTraits.mProbProton", &GlobalTracks_mBTofPidTraits_mProbProton, &b_GlobalTracks_mBTofPidTraits_mProbProton);
//    fChain->SetBranchAddress("GlobalTracks.mMtdPidTraits.mMatchFlag", &GlobalTracks_mMtdPidTraits_mMatchFlag, &b_GlobalTracks_mMtdPidTraits_mMatchFlag);
//    fChain->SetBranchAddress("GlobalTracks.mMtdPidTraits.mYLocal", &GlobalTracks_mMtdPidTraits_mYLocal, &b_GlobalTracks_mMtdPidTraits_mYLocal);
//    fChain->SetBranchAddress("GlobalTracks.mMtdPidTraits.mZLocal", &GlobalTracks_mMtdPidTraits_mZLocal, &b_GlobalTracks_mMtdPidTraits_mZLocal);
//    fChain->SetBranchAddress("GlobalTracks.mMtdPidTraits.mThetaLocal", &GlobalTracks_mMtdPidTraits_mThetaLocal, &b_GlobalTracks_mMtdPidTraits_mThetaLocal);
//    fChain->SetBranchAddress("GlobalTracks.mMtdPidTraits.mPosition.mX1", &GlobalTracks_mMtdPidTraits_mPosition_mX1, &b_GlobalTracks_mMtdPidTraits_mPosition_mX1);
//    fChain->SetBranchAddress("GlobalTracks.mMtdPidTraits.mPosition.mX2", &GlobalTracks_mMtdPidTraits_mPosition_mX2, &b_GlobalTracks_mMtdPidTraits_mPosition_mX2);
//    fChain->SetBranchAddress("GlobalTracks.mMtdPidTraits.mPosition.mX3", &GlobalTracks_mMtdPidTraits_mPosition_mX3, &b_GlobalTracks_mMtdPidTraits_mPosition_mX3);
//    fChain->SetBranchAddress("GlobalTracks.mMtdPidTraits.mTimeOfFlight", &GlobalTracks_mMtdPidTraits_mTimeOfFlight, &b_GlobalTracks_mMtdPidTraits_mTimeOfFlight);
//    fChain->SetBranchAddress("GlobalTracks.mMtdPidTraits.mPathLength", &GlobalTracks_mMtdPidTraits_mPathLength, &b_GlobalTracks_mMtdPidTraits_mPathLength);
//    fChain->SetBranchAddress("GlobalTracks.mMtdPidTraits.mBeta", &GlobalTracks_mMtdPidTraits_mBeta, &b_GlobalTracks_mMtdPidTraits_mBeta);
//    fChain->SetBranchAddress("GlobalTracks.mMtdPidTraits.mSigmaMuon", &GlobalTracks_mMtdPidTraits_mSigmaMuon, &b_GlobalTracks_mMtdPidTraits_mSigmaMuon);
//    fChain->SetBranchAddress("GlobalTracks.mMtdPidTraits.mProbMuon", &GlobalTracks_mMtdPidTraits_mProbMuon, &b_GlobalTracks_mMtdPidTraits_mProbMuon);
//    fChain->SetBranchAddress("GlobalTracks.mIndex2Cov", &GlobalTracks_mIndex2Cov, &b_GlobalTracks_mIndex2Cov);
//    fChain->SetBranchAddress("GlobalTracks.mIdTruth", &GlobalTracks_mIdTruth, &b_GlobalTracks_mIdTruth);
//    fChain->SetBranchAddress("GlobalTracks.mQuality", &GlobalTracks_mQuality, &b_GlobalTracks_mQuality);
//    fChain->SetBranchAddress("GlobalTracks.mIdParentVx", &GlobalTracks_mIdParentVx, &b_GlobalTracks_mIdParentVx);
//    fChain->SetBranchAddress("OtherTracks", &OtherTracks_, &b_OtherTracks_);
//    fChain->SetBranchAddress("OtherTracks.mId", &OtherTracks_mId, &b_OtherTracks_mId);
//    fChain->SetBranchAddress("OtherTracks.mType", &OtherTracks_mType, &b_OtherTracks_mType);
//    fChain->SetBranchAddress("OtherTracks.mFlag", &OtherTracks_mFlag, &b_OtherTracks_mFlag);
//    fChain->SetBranchAddress("OtherTracks.mFlagExtension", &OtherTracks_mFlagExtension, &b_OtherTracks_mFlagExtension);
//    fChain->SetBranchAddress("OtherTracks.mIndex2Global", &OtherTracks_mIndex2Global, &b_OtherTracks_mIndex2Global);
//    fChain->SetBranchAddress("OtherTracks.mIndex2RichSpectra", &OtherTracks_mIndex2RichSpectra, &b_OtherTracks_mIndex2RichSpectra);
//    fChain->SetBranchAddress("OtherTracks.mIndex2BTofHit", &OtherTracks_mIndex2BTofHit, &b_OtherTracks_mIndex2BTofHit);
//    fChain->SetBranchAddress("OtherTracks.mIndex2MtdHit", &OtherTracks_mIndex2MtdHit, &b_OtherTracks_mIndex2MtdHit);
//    fChain->SetBranchAddress("OtherTracks.mVertexIndex", &OtherTracks_mVertexIndex, &b_OtherTracks_mVertexIndex);
//    fChain->SetBranchAddress("OtherTracks.mNHits", &OtherTracks_mNHits, &b_OtherTracks_mNHits);
//    fChain->SetBranchAddress("OtherTracks.mNHitsPoss", &OtherTracks_mNHitsPoss, &b_OtherTracks_mNHitsPoss);
//    fChain->SetBranchAddress("OtherTracks.mNHitsDedx", &OtherTracks_mNHitsDedx, &b_OtherTracks_mNHitsDedx);
//    fChain->SetBranchAddress("OtherTracks.mNHitsFit", &OtherTracks_mNHitsFit, &b_OtherTracks_mNHitsFit);
//    fChain->SetBranchAddress("OtherTracks.mNHitsPossInner", &OtherTracks_mNHitsPossInner, &b_OtherTracks_mNHitsPossInner);
//    fChain->SetBranchAddress("OtherTracks.mNHitsFitInner", &OtherTracks_mNHitsFitInner, &b_OtherTracks_mNHitsFitInner);
//    fChain->SetBranchAddress("OtherTracks.mNHitsPossTpc", &OtherTracks_mNHitsPossTpc, &b_OtherTracks_mNHitsPossTpc);
//    fChain->SetBranchAddress("OtherTracks.mNHitsFitTpc", &OtherTracks_mNHitsFitTpc, &b_OtherTracks_mNHitsFitTpc);
//    fChain->SetBranchAddress("OtherTracks.mPidProbElectron", &OtherTracks_mPidProbElectron, &b_OtherTracks_mPidProbElectron);
//    fChain->SetBranchAddress("OtherTracks.mPidProbPion", &OtherTracks_mPidProbPion, &b_OtherTracks_mPidProbPion);
//    fChain->SetBranchAddress("OtherTracks.mPidProbKaon", &OtherTracks_mPidProbKaon, &b_OtherTracks_mPidProbKaon);
//    fChain->SetBranchAddress("OtherTracks.mPidProbProton", &OtherTracks_mPidProbProton, &b_OtherTracks_mPidProbProton);
//    fChain->SetBranchAddress("OtherTracks.mNSigmaElectron", &OtherTracks_mNSigmaElectron, &b_OtherTracks_mNSigmaElectron);
//    fChain->SetBranchAddress("OtherTracks.mNSigmaPion", &OtherTracks_mNSigmaPion, &b_OtherTracks_mNSigmaPion);
//    fChain->SetBranchAddress("OtherTracks.mNSigmaKaon", &OtherTracks_mNSigmaKaon, &b_OtherTracks_mNSigmaKaon);
//    fChain->SetBranchAddress("OtherTracks.mNSigmaProton", &OtherTracks_mNSigmaProton, &b_OtherTracks_mNSigmaProton);
//    fChain->SetBranchAddress("OtherTracks.mdEdx", &OtherTracks_mdEdx, &b_OtherTracks_mdEdx);
//    fChain->SetBranchAddress("OtherTracks.mChiSqXY", &OtherTracks_mChiSqXY, &b_OtherTracks_mChiSqXY);
//    fChain->SetBranchAddress("OtherTracks.mChiSqZ", &OtherTracks_mChiSqZ, &b_OtherTracks_mChiSqZ);
//    fChain->SetBranchAddress("OtherTracks.mPt", &OtherTracks_mPt, &b_OtherTracks_mPt);
//    fChain->SetBranchAddress("OtherTracks.mEta", &OtherTracks_mEta, &b_OtherTracks_mEta);
//    fChain->SetBranchAddress("OtherTracks.mPhi", &OtherTracks_mPhi, &b_OtherTracks_mPhi);
//    fChain->SetBranchAddress("OtherTracks.mTopologyMap.mMap0", &OtherTracks_mTopologyMap_mMap0, &b_OtherTracks_mTopologyMap_mMap0);
//    fChain->SetBranchAddress("OtherTracks.mTopologyMap.mMap1", &OtherTracks_mTopologyMap_mMap1, &b_OtherTracks_mTopologyMap_mMap1);
//    fChain->SetBranchAddress("OtherTracks.mP.mX1", &OtherTracks_mP_mX1, &b_OtherTracks_mP_mX1);
//    fChain->SetBranchAddress("OtherTracks.mP.mX2", &OtherTracks_mP_mX2, &b_OtherTracks_mP_mX2);
//    fChain->SetBranchAddress("OtherTracks.mP.mX3", &OtherTracks_mP_mX3, &b_OtherTracks_mP_mX3);
//    fChain->SetBranchAddress("OtherTracks.mDCA.mX1", &OtherTracks_mDCA_mX1, &b_OtherTracks_mDCA_mX1);
//    fChain->SetBranchAddress("OtherTracks.mDCA.mX2", &OtherTracks_mDCA_mX2, &b_OtherTracks_mDCA_mX2);
//    fChain->SetBranchAddress("OtherTracks.mDCA.mX3", &OtherTracks_mDCA_mX3, &b_OtherTracks_mDCA_mX3);
//    fChain->SetBranchAddress("OtherTracks.mDCAGlobal.mX1", &OtherTracks_mDCAGlobal_mX1, &b_OtherTracks_mDCAGlobal_mX1);
//    fChain->SetBranchAddress("OtherTracks.mDCAGlobal.mX2", &OtherTracks_mDCAGlobal_mX2, &b_OtherTracks_mDCAGlobal_mX2);
//    fChain->SetBranchAddress("OtherTracks.mDCAGlobal.mX3", &OtherTracks_mDCAGlobal_mX3, &b_OtherTracks_mDCAGlobal_mX3);
//    fChain->SetBranchAddress("OtherTracks.mFirstPoint.mX1", &OtherTracks_mFirstPoint_mX1, &b_OtherTracks_mFirstPoint_mX1);
//    fChain->SetBranchAddress("OtherTracks.mFirstPoint.mX2", &OtherTracks_mFirstPoint_mX2, &b_OtherTracks_mFirstPoint_mX2);
//    fChain->SetBranchAddress("OtherTracks.mFirstPoint.mX3", &OtherTracks_mFirstPoint_mX3, &b_OtherTracks_mFirstPoint_mX3);
//    fChain->SetBranchAddress("OtherTracks.mLastPoint.mX1", &OtherTracks_mLastPoint_mX1, &b_OtherTracks_mLastPoint_mX1);
//    fChain->SetBranchAddress("OtherTracks.mLastPoint.mX2", &OtherTracks_mLastPoint_mX2, &b_OtherTracks_mLastPoint_mX2);
//    fChain->SetBranchAddress("OtherTracks.mLastPoint.mX3", &OtherTracks_mLastPoint_mX3, &b_OtherTracks_mLastPoint_mX3);
//    fChain->SetBranchAddress("OtherTracks.mHelix.mP.mX1", &OtherTracks_mHelix_mP_mX1, &b_OtherTracks_mHelix_mP_mX1);
//    fChain->SetBranchAddress("OtherTracks.mHelix.mP.mX2", &OtherTracks_mHelix_mP_mX2, &b_OtherTracks_mHelix_mP_mX2);
//    fChain->SetBranchAddress("OtherTracks.mHelix.mP.mX3", &OtherTracks_mHelix_mP_mX3, &b_OtherTracks_mHelix_mP_mX3);
//    fChain->SetBranchAddress("OtherTracks.mHelix.mOrigin.mX1", &OtherTracks_mHelix_mOrigin_mX1, &b_OtherTracks_mHelix_mOrigin_mX1);
//    fChain->SetBranchAddress("OtherTracks.mHelix.mOrigin.mX2", &OtherTracks_mHelix_mOrigin_mX2, &b_OtherTracks_mHelix_mOrigin_mX2);
//    fChain->SetBranchAddress("OtherTracks.mHelix.mOrigin.mX3", &OtherTracks_mHelix_mOrigin_mX3, &b_OtherTracks_mHelix_mOrigin_mX3);
//    fChain->SetBranchAddress("OtherTracks.mHelix.mQ", &OtherTracks_mHelix_mQ, &b_OtherTracks_mHelix_mQ);
//    fChain->SetBranchAddress("OtherTracks.mHelix.mB", &OtherTracks_mHelix_mB, &b_OtherTracks_mHelix_mB);
//    fChain->SetBranchAddress("OtherTracks.mOuterHelix.mP.mX1", &OtherTracks_mOuterHelix_mP_mX1, &b_OtherTracks_mOuterHelix_mP_mX1);
//    fChain->SetBranchAddress("OtherTracks.mOuterHelix.mP.mX2", &OtherTracks_mOuterHelix_mP_mX2, &b_OtherTracks_mOuterHelix_mP_mX2);
//    fChain->SetBranchAddress("OtherTracks.mOuterHelix.mP.mX3", &OtherTracks_mOuterHelix_mP_mX3, &b_OtherTracks_mOuterHelix_mP_mX3);
//    fChain->SetBranchAddress("OtherTracks.mOuterHelix.mOrigin.mX1", &OtherTracks_mOuterHelix_mOrigin_mX1, &b_OtherTracks_mOuterHelix_mOrigin_mX1);
//    fChain->SetBranchAddress("OtherTracks.mOuterHelix.mOrigin.mX2", &OtherTracks_mOuterHelix_mOrigin_mX2, &b_OtherTracks_mOuterHelix_mOrigin_mX2);
//    fChain->SetBranchAddress("OtherTracks.mOuterHelix.mOrigin.mX3", &OtherTracks_mOuterHelix_mOrigin_mX3, &b_OtherTracks_mOuterHelix_mOrigin_mX3);
//    fChain->SetBranchAddress("OtherTracks.mOuterHelix.mQ", &OtherTracks_mOuterHelix_mQ, &b_OtherTracks_mOuterHelix_mQ);
//    fChain->SetBranchAddress("OtherTracks.mOuterHelix.mB", &OtherTracks_mOuterHelix_mB, &b_OtherTracks_mOuterHelix_mB);
//    fChain->SetBranchAddress("OtherTracks.mProbPidTraits.mNDF", &OtherTracks_mProbPidTraits_mNDF, &b_OtherTracks_mProbPidTraits_mNDF);
//    fChain->SetBranchAddress("OtherTracks.mProbPidTraits.mdEdxFit", &OtherTracks_mProbPidTraits_mdEdxFit, &b_OtherTracks_mProbPidTraits_mdEdxFit);
//    fChain->SetBranchAddress("OtherTracks.mProbPidTraits.mdEdxErrorFit", &OtherTracks_mProbPidTraits_mdEdxErrorFit, &b_OtherTracks_mProbPidTraits_mdEdxErrorFit);
//    fChain->SetBranchAddress("OtherTracks.mProbPidTraits.mdEdxTruncated", &OtherTracks_mProbPidTraits_mdEdxTruncated, &b_OtherTracks_mProbPidTraits_mdEdxTruncated);
//    fChain->SetBranchAddress("OtherTracks.mProbPidTraits.mdEdxErrorTruncated", &OtherTracks_mProbPidTraits_mdEdxErrorTruncated, &b_OtherTracks_mProbPidTraits_mdEdxErrorTruncated);
//    fChain->SetBranchAddress("OtherTracks.mProbPidTraits.mdEdxTrackLength", &OtherTracks_mProbPidTraits_mdEdxTrackLength, &b_OtherTracks_mProbPidTraits_mdEdxTrackLength);
//    fChain->SetBranchAddress("OtherTracks.mProbPidTraits.mLog2dX", &OtherTracks_mProbPidTraits_mLog2dX, &b_OtherTracks_mProbPidTraits_mLog2dX);
//    fChain->SetBranchAddress("OtherTracks.mProbPidTraits.mProbabilities[9]", &OtherTracks_mProbPidTraits_mProbabilities, &b_OtherTracks_mProbPidTraits_mProbabilities);
//    fChain->SetBranchAddress("OtherTracks.mBTofPidTraits.mMatchFlag", &OtherTracks_mBTofPidTraits_mMatchFlag, &b_OtherTracks_mBTofPidTraits_mMatchFlag);
//    fChain->SetBranchAddress("OtherTracks.mBTofPidTraits.mYLocal", &OtherTracks_mBTofPidTraits_mYLocal, &b_OtherTracks_mBTofPidTraits_mYLocal);
//    fChain->SetBranchAddress("OtherTracks.mBTofPidTraits.mZLocal", &OtherTracks_mBTofPidTraits_mZLocal, &b_OtherTracks_mBTofPidTraits_mZLocal);
//    fChain->SetBranchAddress("OtherTracks.mBTofPidTraits.mThetaLocal", &OtherTracks_mBTofPidTraits_mThetaLocal, &b_OtherTracks_mBTofPidTraits_mThetaLocal);
//    fChain->SetBranchAddress("OtherTracks.mBTofPidTraits.mPosition.mX1", &OtherTracks_mBTofPidTraits_mPosition_mX1, &b_OtherTracks_mBTofPidTraits_mPosition_mX1);
//    fChain->SetBranchAddress("OtherTracks.mBTofPidTraits.mPosition.mX2", &OtherTracks_mBTofPidTraits_mPosition_mX2, &b_OtherTracks_mBTofPidTraits_mPosition_mX2);
//    fChain->SetBranchAddress("OtherTracks.mBTofPidTraits.mPosition.mX3", &OtherTracks_mBTofPidTraits_mPosition_mX3, &b_OtherTracks_mBTofPidTraits_mPosition_mX3);
//    fChain->SetBranchAddress("OtherTracks.mBTofPidTraits.mTimeOfFlight", &OtherTracks_mBTofPidTraits_mTimeOfFlight, &b_OtherTracks_mBTofPidTraits_mTimeOfFlight);
//    fChain->SetBranchAddress("OtherTracks.mBTofPidTraits.mPathLength", &OtherTracks_mBTofPidTraits_mPathLength, &b_OtherTracks_mBTofPidTraits_mPathLength);
//    fChain->SetBranchAddress("OtherTracks.mBTofPidTraits.mBeta", &OtherTracks_mBTofPidTraits_mBeta, &b_OtherTracks_mBTofPidTraits_mBeta);
//    fChain->SetBranchAddress("OtherTracks.mBTofPidTraits.mSigmaElectron", &OtherTracks_mBTofPidTraits_mSigmaElectron, &b_OtherTracks_mBTofPidTraits_mSigmaElectron);
//    fChain->SetBranchAddress("OtherTracks.mBTofPidTraits.mSigmaPion", &OtherTracks_mBTofPidTraits_mSigmaPion, &b_OtherTracks_mBTofPidTraits_mSigmaPion);
//    fChain->SetBranchAddress("OtherTracks.mBTofPidTraits.mSigmaKaon", &OtherTracks_mBTofPidTraits_mSigmaKaon, &b_OtherTracks_mBTofPidTraits_mSigmaKaon);
//    fChain->SetBranchAddress("OtherTracks.mBTofPidTraits.mSigmaProton", &OtherTracks_mBTofPidTraits_mSigmaProton, &b_OtherTracks_mBTofPidTraits_mSigmaProton);
//    fChain->SetBranchAddress("OtherTracks.mBTofPidTraits.mProbElectron", &OtherTracks_mBTofPidTraits_mProbElectron, &b_OtherTracks_mBTofPidTraits_mProbElectron);
//    fChain->SetBranchAddress("OtherTracks.mBTofPidTraits.mProbPion", &OtherTracks_mBTofPidTraits_mProbPion, &b_OtherTracks_mBTofPidTraits_mProbPion);
//    fChain->SetBranchAddress("OtherTracks.mBTofPidTraits.mProbKaon", &OtherTracks_mBTofPidTraits_mProbKaon, &b_OtherTracks_mBTofPidTraits_mProbKaon);
//    fChain->SetBranchAddress("OtherTracks.mBTofPidTraits.mProbProton", &OtherTracks_mBTofPidTraits_mProbProton, &b_OtherTracks_mBTofPidTraits_mProbProton);
//    fChain->SetBranchAddress("OtherTracks.mMtdPidTraits.mMatchFlag", &OtherTracks_mMtdPidTraits_mMatchFlag, &b_OtherTracks_mMtdPidTraits_mMatchFlag);
//    fChain->SetBranchAddress("OtherTracks.mMtdPidTraits.mYLocal", &OtherTracks_mMtdPidTraits_mYLocal, &b_OtherTracks_mMtdPidTraits_mYLocal);
//    fChain->SetBranchAddress("OtherTracks.mMtdPidTraits.mZLocal", &OtherTracks_mMtdPidTraits_mZLocal, &b_OtherTracks_mMtdPidTraits_mZLocal);
//    fChain->SetBranchAddress("OtherTracks.mMtdPidTraits.mThetaLocal", &OtherTracks_mMtdPidTraits_mThetaLocal, &b_OtherTracks_mMtdPidTraits_mThetaLocal);
//    fChain->SetBranchAddress("OtherTracks.mMtdPidTraits.mPosition.mX1", &OtherTracks_mMtdPidTraits_mPosition_mX1, &b_OtherTracks_mMtdPidTraits_mPosition_mX1);
//    fChain->SetBranchAddress("OtherTracks.mMtdPidTraits.mPosition.mX2", &OtherTracks_mMtdPidTraits_mPosition_mX2, &b_OtherTracks_mMtdPidTraits_mPosition_mX2);
//    fChain->SetBranchAddress("OtherTracks.mMtdPidTraits.mPosition.mX3", &OtherTracks_mMtdPidTraits_mPosition_mX3, &b_OtherTracks_mMtdPidTraits_mPosition_mX3);
//    fChain->SetBranchAddress("OtherTracks.mMtdPidTraits.mTimeOfFlight", &OtherTracks_mMtdPidTraits_mTimeOfFlight, &b_OtherTracks_mMtdPidTraits_mTimeOfFlight);
//    fChain->SetBranchAddress("OtherTracks.mMtdPidTraits.mPathLength", &OtherTracks_mMtdPidTraits_mPathLength, &b_OtherTracks_mMtdPidTraits_mPathLength);
//    fChain->SetBranchAddress("OtherTracks.mMtdPidTraits.mBeta", &OtherTracks_mMtdPidTraits_mBeta, &b_OtherTracks_mMtdPidTraits_mBeta);
//    fChain->SetBranchAddress("OtherTracks.mMtdPidTraits.mSigmaMuon", &OtherTracks_mMtdPidTraits_mSigmaMuon, &b_OtherTracks_mMtdPidTraits_mSigmaMuon);
//    fChain->SetBranchAddress("OtherTracks.mMtdPidTraits.mProbMuon", &OtherTracks_mMtdPidTraits_mProbMuon, &b_OtherTracks_mMtdPidTraits_mProbMuon);
//    fChain->SetBranchAddress("OtherTracks.mIndex2Cov", &OtherTracks_mIndex2Cov, &b_OtherTracks_mIndex2Cov);
//    fChain->SetBranchAddress("OtherTracks.mIdTruth", &OtherTracks_mIdTruth, &b_OtherTracks_mIdTruth);
//    fChain->SetBranchAddress("OtherTracks.mQuality", &OtherTracks_mQuality, &b_OtherTracks_mQuality);
//    fChain->SetBranchAddress("OtherTracks.mIdParentVx", &OtherTracks_mIdParentVx, &b_OtherTracks_mIdParentVx);
//    fChain->SetBranchAddress("L3Tracks", &L3Tracks_, &b_L3Tracks_);
//    fChain->SetBranchAddress("L3Tracks.mId", &L3Tracks_mId, &b_L3Tracks_mId);
//    fChain->SetBranchAddress("L3Tracks.mType", &L3Tracks_mType, &b_L3Tracks_mType);
//    fChain->SetBranchAddress("L3Tracks.mFlag", &L3Tracks_mFlag, &b_L3Tracks_mFlag);
//    fChain->SetBranchAddress("L3Tracks.mFlagExtension", &L3Tracks_mFlagExtension, &b_L3Tracks_mFlagExtension);
//    fChain->SetBranchAddress("L3Tracks.mIndex2Global", &L3Tracks_mIndex2Global, &b_L3Tracks_mIndex2Global);
//    fChain->SetBranchAddress("L3Tracks.mIndex2RichSpectra", &L3Tracks_mIndex2RichSpectra, &b_L3Tracks_mIndex2RichSpectra);
//    fChain->SetBranchAddress("L3Tracks.mIndex2BTofHit", &L3Tracks_mIndex2BTofHit, &b_L3Tracks_mIndex2BTofHit);
//    fChain->SetBranchAddress("L3Tracks.mIndex2MtdHit", &L3Tracks_mIndex2MtdHit, &b_L3Tracks_mIndex2MtdHit);
//    fChain->SetBranchAddress("L3Tracks.mVertexIndex", &L3Tracks_mVertexIndex, &b_L3Tracks_mVertexIndex);
//    fChain->SetBranchAddress("L3Tracks.mNHits", &L3Tracks_mNHits, &b_L3Tracks_mNHits);
//    fChain->SetBranchAddress("L3Tracks.mNHitsPoss", &L3Tracks_mNHitsPoss, &b_L3Tracks_mNHitsPoss);
//    fChain->SetBranchAddress("L3Tracks.mNHitsDedx", &L3Tracks_mNHitsDedx, &b_L3Tracks_mNHitsDedx);
//    fChain->SetBranchAddress("L3Tracks.mNHitsFit", &L3Tracks_mNHitsFit, &b_L3Tracks_mNHitsFit);
//    fChain->SetBranchAddress("L3Tracks.mNHitsPossInner", &L3Tracks_mNHitsPossInner, &b_L3Tracks_mNHitsPossInner);
//    fChain->SetBranchAddress("L3Tracks.mNHitsFitInner", &L3Tracks_mNHitsFitInner, &b_L3Tracks_mNHitsFitInner);
//    fChain->SetBranchAddress("L3Tracks.mNHitsPossTpc", &L3Tracks_mNHitsPossTpc, &b_L3Tracks_mNHitsPossTpc);
//    fChain->SetBranchAddress("L3Tracks.mNHitsFitTpc", &L3Tracks_mNHitsFitTpc, &b_L3Tracks_mNHitsFitTpc);
//    fChain->SetBranchAddress("L3Tracks.mPidProbElectron", &L3Tracks_mPidProbElectron, &b_L3Tracks_mPidProbElectron);
//    fChain->SetBranchAddress("L3Tracks.mPidProbPion", &L3Tracks_mPidProbPion, &b_L3Tracks_mPidProbPion);
//    fChain->SetBranchAddress("L3Tracks.mPidProbKaon", &L3Tracks_mPidProbKaon, &b_L3Tracks_mPidProbKaon);
//    fChain->SetBranchAddress("L3Tracks.mPidProbProton", &L3Tracks_mPidProbProton, &b_L3Tracks_mPidProbProton);
//    fChain->SetBranchAddress("L3Tracks.mNSigmaElectron", &L3Tracks_mNSigmaElectron, &b_L3Tracks_mNSigmaElectron);
//    fChain->SetBranchAddress("L3Tracks.mNSigmaPion", &L3Tracks_mNSigmaPion, &b_L3Tracks_mNSigmaPion);
//    fChain->SetBranchAddress("L3Tracks.mNSigmaKaon", &L3Tracks_mNSigmaKaon, &b_L3Tracks_mNSigmaKaon);
//    fChain->SetBranchAddress("L3Tracks.mNSigmaProton", &L3Tracks_mNSigmaProton, &b_L3Tracks_mNSigmaProton);
//    fChain->SetBranchAddress("L3Tracks.mdEdx", &L3Tracks_mdEdx, &b_L3Tracks_mdEdx);
//    fChain->SetBranchAddress("L3Tracks.mChiSqXY", &L3Tracks_mChiSqXY, &b_L3Tracks_mChiSqXY);
//    fChain->SetBranchAddress("L3Tracks.mChiSqZ", &L3Tracks_mChiSqZ, &b_L3Tracks_mChiSqZ);
//    fChain->SetBranchAddress("L3Tracks.mPt", &L3Tracks_mPt, &b_L3Tracks_mPt);
//    fChain->SetBranchAddress("L3Tracks.mEta", &L3Tracks_mEta, &b_L3Tracks_mEta);
//    fChain->SetBranchAddress("L3Tracks.mPhi", &L3Tracks_mPhi, &b_L3Tracks_mPhi);
//    fChain->SetBranchAddress("L3Tracks.mTopologyMap.mMap0", &L3Tracks_mTopologyMap_mMap0, &b_L3Tracks_mTopologyMap_mMap0);
//    fChain->SetBranchAddress("L3Tracks.mTopologyMap.mMap1", &L3Tracks_mTopologyMap_mMap1, &b_L3Tracks_mTopologyMap_mMap1);
//    fChain->SetBranchAddress("L3Tracks.mP.mX1", &L3Tracks_mP_mX1, &b_L3Tracks_mP_mX1);
//    fChain->SetBranchAddress("L3Tracks.mP.mX2", &L3Tracks_mP_mX2, &b_L3Tracks_mP_mX2);
//    fChain->SetBranchAddress("L3Tracks.mP.mX3", &L3Tracks_mP_mX3, &b_L3Tracks_mP_mX3);
//    fChain->SetBranchAddress("L3Tracks.mDCA.mX1", &L3Tracks_mDCA_mX1, &b_L3Tracks_mDCA_mX1);
//    fChain->SetBranchAddress("L3Tracks.mDCA.mX2", &L3Tracks_mDCA_mX2, &b_L3Tracks_mDCA_mX2);
//    fChain->SetBranchAddress("L3Tracks.mDCA.mX3", &L3Tracks_mDCA_mX3, &b_L3Tracks_mDCA_mX3);
//    fChain->SetBranchAddress("L3Tracks.mDCAGlobal.mX1", &L3Tracks_mDCAGlobal_mX1, &b_L3Tracks_mDCAGlobal_mX1);
//    fChain->SetBranchAddress("L3Tracks.mDCAGlobal.mX2", &L3Tracks_mDCAGlobal_mX2, &b_L3Tracks_mDCAGlobal_mX2);
//    fChain->SetBranchAddress("L3Tracks.mDCAGlobal.mX3", &L3Tracks_mDCAGlobal_mX3, &b_L3Tracks_mDCAGlobal_mX3);
//    fChain->SetBranchAddress("L3Tracks.mFirstPoint.mX1", &L3Tracks_mFirstPoint_mX1, &b_L3Tracks_mFirstPoint_mX1);
//    fChain->SetBranchAddress("L3Tracks.mFirstPoint.mX2", &L3Tracks_mFirstPoint_mX2, &b_L3Tracks_mFirstPoint_mX2);
//    fChain->SetBranchAddress("L3Tracks.mFirstPoint.mX3", &L3Tracks_mFirstPoint_mX3, &b_L3Tracks_mFirstPoint_mX3);
//    fChain->SetBranchAddress("L3Tracks.mLastPoint.mX1", &L3Tracks_mLastPoint_mX1, &b_L3Tracks_mLastPoint_mX1);
//    fChain->SetBranchAddress("L3Tracks.mLastPoint.mX2", &L3Tracks_mLastPoint_mX2, &b_L3Tracks_mLastPoint_mX2);
//    fChain->SetBranchAddress("L3Tracks.mLastPoint.mX3", &L3Tracks_mLastPoint_mX3, &b_L3Tracks_mLastPoint_mX3);
//    fChain->SetBranchAddress("L3Tracks.mHelix.mP.mX1", &L3Tracks_mHelix_mP_mX1, &b_L3Tracks_mHelix_mP_mX1);
//    fChain->SetBranchAddress("L3Tracks.mHelix.mP.mX2", &L3Tracks_mHelix_mP_mX2, &b_L3Tracks_mHelix_mP_mX2);
//    fChain->SetBranchAddress("L3Tracks.mHelix.mP.mX3", &L3Tracks_mHelix_mP_mX3, &b_L3Tracks_mHelix_mP_mX3);
//    fChain->SetBranchAddress("L3Tracks.mHelix.mOrigin.mX1", &L3Tracks_mHelix_mOrigin_mX1, &b_L3Tracks_mHelix_mOrigin_mX1);
//    fChain->SetBranchAddress("L3Tracks.mHelix.mOrigin.mX2", &L3Tracks_mHelix_mOrigin_mX2, &b_L3Tracks_mHelix_mOrigin_mX2);
//    fChain->SetBranchAddress("L3Tracks.mHelix.mOrigin.mX3", &L3Tracks_mHelix_mOrigin_mX3, &b_L3Tracks_mHelix_mOrigin_mX3);
//    fChain->SetBranchAddress("L3Tracks.mHelix.mQ", &L3Tracks_mHelix_mQ, &b_L3Tracks_mHelix_mQ);
//    fChain->SetBranchAddress("L3Tracks.mHelix.mB", &L3Tracks_mHelix_mB, &b_L3Tracks_mHelix_mB);
//    fChain->SetBranchAddress("L3Tracks.mOuterHelix.mP.mX1", &L3Tracks_mOuterHelix_mP_mX1, &b_L3Tracks_mOuterHelix_mP_mX1);
//    fChain->SetBranchAddress("L3Tracks.mOuterHelix.mP.mX2", &L3Tracks_mOuterHelix_mP_mX2, &b_L3Tracks_mOuterHelix_mP_mX2);
//    fChain->SetBranchAddress("L3Tracks.mOuterHelix.mP.mX3", &L3Tracks_mOuterHelix_mP_mX3, &b_L3Tracks_mOuterHelix_mP_mX3);
//    fChain->SetBranchAddress("L3Tracks.mOuterHelix.mOrigin.mX1", &L3Tracks_mOuterHelix_mOrigin_mX1, &b_L3Tracks_mOuterHelix_mOrigin_mX1);
//    fChain->SetBranchAddress("L3Tracks.mOuterHelix.mOrigin.mX2", &L3Tracks_mOuterHelix_mOrigin_mX2, &b_L3Tracks_mOuterHelix_mOrigin_mX2);
//    fChain->SetBranchAddress("L3Tracks.mOuterHelix.mOrigin.mX3", &L3Tracks_mOuterHelix_mOrigin_mX3, &b_L3Tracks_mOuterHelix_mOrigin_mX3);
//    fChain->SetBranchAddress("L3Tracks.mOuterHelix.mQ", &L3Tracks_mOuterHelix_mQ, &b_L3Tracks_mOuterHelix_mQ);
//    fChain->SetBranchAddress("L3Tracks.mOuterHelix.mB", &L3Tracks_mOuterHelix_mB, &b_L3Tracks_mOuterHelix_mB);
//    fChain->SetBranchAddress("L3Tracks.mProbPidTraits.mNDF", &L3Tracks_mProbPidTraits_mNDF, &b_L3Tracks_mProbPidTraits_mNDF);
//    fChain->SetBranchAddress("L3Tracks.mProbPidTraits.mdEdxFit", &L3Tracks_mProbPidTraits_mdEdxFit, &b_L3Tracks_mProbPidTraits_mdEdxFit);
//    fChain->SetBranchAddress("L3Tracks.mProbPidTraits.mdEdxErrorFit", &L3Tracks_mProbPidTraits_mdEdxErrorFit, &b_L3Tracks_mProbPidTraits_mdEdxErrorFit);
//    fChain->SetBranchAddress("L3Tracks.mProbPidTraits.mdEdxTruncated", &L3Tracks_mProbPidTraits_mdEdxTruncated, &b_L3Tracks_mProbPidTraits_mdEdxTruncated);
//    fChain->SetBranchAddress("L3Tracks.mProbPidTraits.mdEdxErrorTruncated", &L3Tracks_mProbPidTraits_mdEdxErrorTruncated, &b_L3Tracks_mProbPidTraits_mdEdxErrorTruncated);
//    fChain->SetBranchAddress("L3Tracks.mProbPidTraits.mdEdxTrackLength", &L3Tracks_mProbPidTraits_mdEdxTrackLength, &b_L3Tracks_mProbPidTraits_mdEdxTrackLength);
//    fChain->SetBranchAddress("L3Tracks.mProbPidTraits.mLog2dX", &L3Tracks_mProbPidTraits_mLog2dX, &b_L3Tracks_mProbPidTraits_mLog2dX);
//    fChain->SetBranchAddress("L3Tracks.mProbPidTraits.mProbabilities[9]", &L3Tracks_mProbPidTraits_mProbabilities, &b_L3Tracks_mProbPidTraits_mProbabilities);
//    fChain->SetBranchAddress("L3Tracks.mBTofPidTraits.mMatchFlag", &L3Tracks_mBTofPidTraits_mMatchFlag, &b_L3Tracks_mBTofPidTraits_mMatchFlag);
//    fChain->SetBranchAddress("L3Tracks.mBTofPidTraits.mYLocal", &L3Tracks_mBTofPidTraits_mYLocal, &b_L3Tracks_mBTofPidTraits_mYLocal);
//    fChain->SetBranchAddress("L3Tracks.mBTofPidTraits.mZLocal", &L3Tracks_mBTofPidTraits_mZLocal, &b_L3Tracks_mBTofPidTraits_mZLocal);
//    fChain->SetBranchAddress("L3Tracks.mBTofPidTraits.mThetaLocal", &L3Tracks_mBTofPidTraits_mThetaLocal, &b_L3Tracks_mBTofPidTraits_mThetaLocal);
//    fChain->SetBranchAddress("L3Tracks.mBTofPidTraits.mPosition.mX1", &L3Tracks_mBTofPidTraits_mPosition_mX1, &b_L3Tracks_mBTofPidTraits_mPosition_mX1);
//    fChain->SetBranchAddress("L3Tracks.mBTofPidTraits.mPosition.mX2", &L3Tracks_mBTofPidTraits_mPosition_mX2, &b_L3Tracks_mBTofPidTraits_mPosition_mX2);
//    fChain->SetBranchAddress("L3Tracks.mBTofPidTraits.mPosition.mX3", &L3Tracks_mBTofPidTraits_mPosition_mX3, &b_L3Tracks_mBTofPidTraits_mPosition_mX3);
//    fChain->SetBranchAddress("L3Tracks.mBTofPidTraits.mTimeOfFlight", &L3Tracks_mBTofPidTraits_mTimeOfFlight, &b_L3Tracks_mBTofPidTraits_mTimeOfFlight);
//    fChain->SetBranchAddress("L3Tracks.mBTofPidTraits.mPathLength", &L3Tracks_mBTofPidTraits_mPathLength, &b_L3Tracks_mBTofPidTraits_mPathLength);
//    fChain->SetBranchAddress("L3Tracks.mBTofPidTraits.mBeta", &L3Tracks_mBTofPidTraits_mBeta, &b_L3Tracks_mBTofPidTraits_mBeta);
//    fChain->SetBranchAddress("L3Tracks.mBTofPidTraits.mSigmaElectron", &L3Tracks_mBTofPidTraits_mSigmaElectron, &b_L3Tracks_mBTofPidTraits_mSigmaElectron);
//    fChain->SetBranchAddress("L3Tracks.mBTofPidTraits.mSigmaPion", &L3Tracks_mBTofPidTraits_mSigmaPion, &b_L3Tracks_mBTofPidTraits_mSigmaPion);
//    fChain->SetBranchAddress("L3Tracks.mBTofPidTraits.mSigmaKaon", &L3Tracks_mBTofPidTraits_mSigmaKaon, &b_L3Tracks_mBTofPidTraits_mSigmaKaon);
//    fChain->SetBranchAddress("L3Tracks.mBTofPidTraits.mSigmaProton", &L3Tracks_mBTofPidTraits_mSigmaProton, &b_L3Tracks_mBTofPidTraits_mSigmaProton);
//    fChain->SetBranchAddress("L3Tracks.mBTofPidTraits.mProbElectron", &L3Tracks_mBTofPidTraits_mProbElectron, &b_L3Tracks_mBTofPidTraits_mProbElectron);
//    fChain->SetBranchAddress("L3Tracks.mBTofPidTraits.mProbPion", &L3Tracks_mBTofPidTraits_mProbPion, &b_L3Tracks_mBTofPidTraits_mProbPion);
//    fChain->SetBranchAddress("L3Tracks.mBTofPidTraits.mProbKaon", &L3Tracks_mBTofPidTraits_mProbKaon, &b_L3Tracks_mBTofPidTraits_mProbKaon);
//    fChain->SetBranchAddress("L3Tracks.mBTofPidTraits.mProbProton", &L3Tracks_mBTofPidTraits_mProbProton, &b_L3Tracks_mBTofPidTraits_mProbProton);
//    fChain->SetBranchAddress("L3Tracks.mMtdPidTraits.mMatchFlag", &L3Tracks_mMtdPidTraits_mMatchFlag, &b_L3Tracks_mMtdPidTraits_mMatchFlag);
//    fChain->SetBranchAddress("L3Tracks.mMtdPidTraits.mYLocal", &L3Tracks_mMtdPidTraits_mYLocal, &b_L3Tracks_mMtdPidTraits_mYLocal);
//    fChain->SetBranchAddress("L3Tracks.mMtdPidTraits.mZLocal", &L3Tracks_mMtdPidTraits_mZLocal, &b_L3Tracks_mMtdPidTraits_mZLocal);
//    fChain->SetBranchAddress("L3Tracks.mMtdPidTraits.mThetaLocal", &L3Tracks_mMtdPidTraits_mThetaLocal, &b_L3Tracks_mMtdPidTraits_mThetaLocal);
//    fChain->SetBranchAddress("L3Tracks.mMtdPidTraits.mPosition.mX1", &L3Tracks_mMtdPidTraits_mPosition_mX1, &b_L3Tracks_mMtdPidTraits_mPosition_mX1);
//    fChain->SetBranchAddress("L3Tracks.mMtdPidTraits.mPosition.mX2", &L3Tracks_mMtdPidTraits_mPosition_mX2, &b_L3Tracks_mMtdPidTraits_mPosition_mX2);
//    fChain->SetBranchAddress("L3Tracks.mMtdPidTraits.mPosition.mX3", &L3Tracks_mMtdPidTraits_mPosition_mX3, &b_L3Tracks_mMtdPidTraits_mPosition_mX3);
//    fChain->SetBranchAddress("L3Tracks.mMtdPidTraits.mTimeOfFlight", &L3Tracks_mMtdPidTraits_mTimeOfFlight, &b_L3Tracks_mMtdPidTraits_mTimeOfFlight);
//    fChain->SetBranchAddress("L3Tracks.mMtdPidTraits.mPathLength", &L3Tracks_mMtdPidTraits_mPathLength, &b_L3Tracks_mMtdPidTraits_mPathLength);
//    fChain->SetBranchAddress("L3Tracks.mMtdPidTraits.mBeta", &L3Tracks_mMtdPidTraits_mBeta, &b_L3Tracks_mMtdPidTraits_mBeta);
//    fChain->SetBranchAddress("L3Tracks.mMtdPidTraits.mSigmaMuon", &L3Tracks_mMtdPidTraits_mSigmaMuon, &b_L3Tracks_mMtdPidTraits_mSigmaMuon);
//    fChain->SetBranchAddress("L3Tracks.mMtdPidTraits.mProbMuon", &L3Tracks_mMtdPidTraits_mProbMuon, &b_L3Tracks_mMtdPidTraits_mProbMuon);
//    fChain->SetBranchAddress("L3Tracks.mIndex2Cov", &L3Tracks_mIndex2Cov, &b_L3Tracks_mIndex2Cov);
//    fChain->SetBranchAddress("L3Tracks.mIdTruth", &L3Tracks_mIdTruth, &b_L3Tracks_mIdTruth);
//    fChain->SetBranchAddress("L3Tracks.mQuality", &L3Tracks_mQuality, &b_L3Tracks_mQuality);
//    fChain->SetBranchAddress("L3Tracks.mIdParentVx", &L3Tracks_mIdParentVx, &b_L3Tracks_mIdParentVx);
//    fChain->SetBranchAddress("RichSpectra", &RichSpectra_, &b_RichSpectra_);
//    fChain->SetBranchAddress("RichSpectra.mExtrapolatedX", &RichSpectra_mExtrapolatedX, &b_RichSpectra_mExtrapolatedX);
//    fChain->SetBranchAddress("RichSpectra.mExtrapolatedY", &RichSpectra_mExtrapolatedY, &b_RichSpectra_mExtrapolatedY);
//    fChain->SetBranchAddress("RichSpectra.mDx", &RichSpectra_mDx, &b_RichSpectra_mDx);
//    fChain->SetBranchAddress("RichSpectra.mDy", &RichSpectra_mDy, &b_RichSpectra_mDy);
//    fChain->SetBranchAddress("RichSpectra.mCdx", &RichSpectra_mCdx, &b_RichSpectra_mCdx);
//    fChain->SetBranchAddress("RichSpectra.mCdy", &RichSpectra_mCdy, &b_RichSpectra_mCdy);
//    fChain->SetBranchAddress("RichSpectra.mCherenkovAngle", &RichSpectra_mCherenkovAngle, &b_RichSpectra_mCherenkovAngle);
//    fChain->SetBranchAddress("RichSpectra.mCherenkovAngleSigma", &RichSpectra_mCherenkovAngleSigma, &b_RichSpectra_mCherenkovAngleSigma);
//    fChain->SetBranchAddress("RichSpectra.mNumberOfPhotons", &RichSpectra_mNumberOfPhotons, &b_RichSpectra_mNumberOfPhotons);
//    fChain->SetBranchAddress("RichSpectra.mPeakAngle", &RichSpectra_mPeakAngle, &b_RichSpectra_mPeakAngle);
//    fChain->SetBranchAddress("RichSpectra.mPeakPhotons", &RichSpectra_mPeakPhotons, &b_RichSpectra_mPeakPhotons);
//    fChain->SetBranchAddress("RichSpectra.mTotalPhotons", &RichSpectra_mTotalPhotons, &b_RichSpectra_mTotalPhotons);
//    fChain->SetBranchAddress("RichSpectra.mMassSquared", &RichSpectra_mMassSquared, &b_RichSpectra_mMassSquared);
//    fChain->SetBranchAddress("RichSpectra.mLineIntegralRatio", &RichSpectra_mLineIntegralRatio, &b_RichSpectra_mLineIntegralRatio);
//    fChain->SetBranchAddress("RichSpectra.mLineIntegral", &RichSpectra_mLineIntegral, &b_RichSpectra_mLineIntegral);
//    fChain->SetBranchAddress("RichSpectra.mAlpha", &RichSpectra_mAlpha, &b_RichSpectra_mAlpha);
//    fChain->SetBranchAddress("RichSpectra.mFlag", &RichSpectra_mFlag, &b_RichSpectra_mFlag);
//    fChain->SetBranchAddress("RichSpectra.mReserved", &RichSpectra_mReserved, &b_RichSpectra_mReserved);
//    fChain->SetBranchAddress("RichSpectra.mDpi", &RichSpectra_mDpi, &b_RichSpectra_mDpi);
//    fChain->SetBranchAddress("RichSpectra.mDk", &RichSpectra_mDk, &b_RichSpectra_mDk);
//    fChain->SetBranchAddress("RichSpectra.mDp", &RichSpectra_mDp, &b_RichSpectra_mDp);
//    fChain->SetBranchAddress("RichSpectra.mNDpi", &RichSpectra_mNDpi, &b_RichSpectra_mNDpi);
//    fChain->SetBranchAddress("RichSpectra.mNDk", &RichSpectra_mNDk, &b_RichSpectra_mNDk);
//    fChain->SetBranchAddress("RichSpectra.mNDp", &RichSpectra_mNDp, &b_RichSpectra_mNDp);
//    fChain->SetBranchAddress("RichSpectra.mVersion", &RichSpectra_mVersion, &b_RichSpectra_mVersion);
//    fChain->SetBranchAddress("DetectorStates", &DetectorStates_, &b_DetectorStates_);
//    fChain->SetBranchAddress("DetectorStates.mDetectorId", DetectorStates_mDetectorId, &b_DetectorStates_mDetectorId);
//    fChain->SetBranchAddress("DetectorStates.mIsGood", DetectorStates_mIsGood, &b_DetectorStates_mIsGood);
//    fChain->SetBranchAddress("L3AlgoAccept", &L3AlgoAccept_, &b_L3AlgoAccept_);
//    fChain->SetBranchAddress("L3AlgoAccept.mId", &L3AlgoAccept_mId, &b_L3AlgoAccept_mId);
//    fChain->SetBranchAddress("L3AlgoAccept.mOn", &L3AlgoAccept_mOn, &b_L3AlgoAccept_mOn);
//    fChain->SetBranchAddress("L3AlgoAccept.mAccept", &L3AlgoAccept_mAccept, &b_L3AlgoAccept_mAccept);
//    fChain->SetBranchAddress("L3AlgoAccept.mBuild", &L3AlgoAccept_mBuild, &b_L3AlgoAccept_mBuild);
//    fChain->SetBranchAddress("L3AlgoAccept.mNumberOfProcessedEvents", &L3AlgoAccept_mNumberOfProcessedEvents, &b_L3AlgoAccept_mNumberOfProcessedEvents);
//    fChain->SetBranchAddress("L3AlgoAccept.mNumberOfAcceptedEvents", &L3AlgoAccept_mNumberOfAcceptedEvents, &b_L3AlgoAccept_mNumberOfAcceptedEvents);
//    fChain->SetBranchAddress("L3AlgoAccept.mNumberOfBuildEvents", &L3AlgoAccept_mNumberOfBuildEvents, &b_L3AlgoAccept_mNumberOfBuildEvents);
//    fChain->SetBranchAddress("L3AlgoAccept.mDataSize", &L3AlgoAccept_mDataSize, &b_L3AlgoAccept_mDataSize);
//    fChain->SetBranchAddress("L3AlgoAccept.mDataArray", &L3AlgoAccept_mDataArray, &b_L3AlgoAccept_mDataArray);
//    fChain->SetBranchAddress("L3AlgoAccept.mPreScale", &L3AlgoAccept_mPreScale, &b_L3AlgoAccept_mPreScale);
//    fChain->SetBranchAddress("L3AlgoAccept.mPostScale", &L3AlgoAccept_mPostScale, &b_L3AlgoAccept_mPostScale);
//    fChain->SetBranchAddress("L3AlgoAccept.mIntParameterSize", &L3AlgoAccept_mIntParameterSize, &b_L3AlgoAccept_mIntParameterSize);
//    fChain->SetBranchAddress("L3AlgoAccept.mIntParameterArray", &L3AlgoAccept_mIntParameterArray, &b_L3AlgoAccept_mIntParameterArray);
//    fChain->SetBranchAddress("L3AlgoAccept.mFloatParameterSize", &L3AlgoAccept_mFloatParameterSize, &b_L3AlgoAccept_mFloatParameterSize);
//    fChain->SetBranchAddress("L3AlgoAccept.mFloatParameterArray", &L3AlgoAccept_mFloatParameterArray, &b_L3AlgoAccept_mFloatParameterArray);
//    fChain->SetBranchAddress("L3AlgoReject", &L3AlgoReject_, &b_L3AlgoReject_);
//    fChain->SetBranchAddress("L3AlgoReject.mId", &L3AlgoReject_mId, &b_L3AlgoReject_mId);
//    fChain->SetBranchAddress("L3AlgoReject.mOn", &L3AlgoReject_mOn, &b_L3AlgoReject_mOn);
//    fChain->SetBranchAddress("L3AlgoReject.mAccept", &L3AlgoReject_mAccept, &b_L3AlgoReject_mAccept);
//    fChain->SetBranchAddress("L3AlgoReject.mBuild", &L3AlgoReject_mBuild, &b_L3AlgoReject_mBuild);
//    fChain->SetBranchAddress("L3AlgoReject.mNumberOfProcessedEvents", &L3AlgoReject_mNumberOfProcessedEvents, &b_L3AlgoReject_mNumberOfProcessedEvents);
//    fChain->SetBranchAddress("L3AlgoReject.mNumberOfAcceptedEvents", &L3AlgoReject_mNumberOfAcceptedEvents, &b_L3AlgoReject_mNumberOfAcceptedEvents);
//    fChain->SetBranchAddress("L3AlgoReject.mNumberOfBuildEvents", &L3AlgoReject_mNumberOfBuildEvents, &b_L3AlgoReject_mNumberOfBuildEvents);
//    fChain->SetBranchAddress("L3AlgoReject.mDataSize", &L3AlgoReject_mDataSize, &b_L3AlgoReject_mDataSize);
//    fChain->SetBranchAddress("L3AlgoReject.mDataArray", &L3AlgoReject_mDataArray, &b_L3AlgoReject_mDataArray);
//    fChain->SetBranchAddress("L3AlgoReject.mPreScale", &L3AlgoReject_mPreScale, &b_L3AlgoReject_mPreScale);
//    fChain->SetBranchAddress("L3AlgoReject.mPostScale", &L3AlgoReject_mPostScale, &b_L3AlgoReject_mPostScale);
//    fChain->SetBranchAddress("L3AlgoReject.mIntParameterSize", &L3AlgoReject_mIntParameterSize, &b_L3AlgoReject_mIntParameterSize);
//    fChain->SetBranchAddress("L3AlgoReject.mIntParameterArray", &L3AlgoReject_mIntParameterArray, &b_L3AlgoReject_mIntParameterArray);
//    fChain->SetBranchAddress("L3AlgoReject.mFloatParameterSize", &L3AlgoReject_mFloatParameterSize, &b_L3AlgoReject_mFloatParameterSize);
//    fChain->SetBranchAddress("L3AlgoReject.mFloatParameterArray", &L3AlgoReject_mFloatParameterArray, &b_L3AlgoReject_mFloatParameterArray);
//    fChain->SetBranchAddress("CovGlobTrack", &CovGlobTrack_, &b_CovGlobTrack_);
//    fChain->SetBranchAddress("CovGlobTrack.mImp", &CovGlobTrack_mImp, &b_CovGlobTrack_mImp);
//    fChain->SetBranchAddress("CovGlobTrack.mZ", &CovGlobTrack_mZ, &b_CovGlobTrack_mZ);
//    fChain->SetBranchAddress("CovGlobTrack.mPsi", &CovGlobTrack_mPsi, &b_CovGlobTrack_mPsi);
//    fChain->SetBranchAddress("CovGlobTrack.mPti", &CovGlobTrack_mPti, &b_CovGlobTrack_mPti);
//    fChain->SetBranchAddress("CovGlobTrack.mTan", &CovGlobTrack_mTan, &b_CovGlobTrack_mTan);
//    fChain->SetBranchAddress("CovGlobTrack.mCurv", &CovGlobTrack_mCurv, &b_CovGlobTrack_mCurv);
//    fChain->SetBranchAddress("CovGlobTrack.mImpImp", &CovGlobTrack_mImpImp, &b_CovGlobTrack_mImpImp);
//    fChain->SetBranchAddress("CovGlobTrack.mZImp", &CovGlobTrack_mZImp, &b_CovGlobTrack_mZImp);
//    fChain->SetBranchAddress("CovGlobTrack.mZZ", &CovGlobTrack_mZZ, &b_CovGlobTrack_mZZ);
//    fChain->SetBranchAddress("CovGlobTrack.mPsiImp", &CovGlobTrack_mPsiImp, &b_CovGlobTrack_mPsiImp);
//    fChain->SetBranchAddress("CovGlobTrack.mPsiZ", &CovGlobTrack_mPsiZ, &b_CovGlobTrack_mPsiZ);
//    fChain->SetBranchAddress("CovGlobTrack.mPsiPsi", &CovGlobTrack_mPsiPsi, &b_CovGlobTrack_mPsiPsi);
//    fChain->SetBranchAddress("CovGlobTrack.mPtiImp", &CovGlobTrack_mPtiImp, &b_CovGlobTrack_mPtiImp);
//    fChain->SetBranchAddress("CovGlobTrack.mPtiZ", &CovGlobTrack_mPtiZ, &b_CovGlobTrack_mPtiZ);
//    fChain->SetBranchAddress("CovGlobTrack.mPtiPsi", &CovGlobTrack_mPtiPsi, &b_CovGlobTrack_mPtiPsi);
//    fChain->SetBranchAddress("CovGlobTrack.mPtiPti", &CovGlobTrack_mPtiPti, &b_CovGlobTrack_mPtiPti);
//    fChain->SetBranchAddress("CovGlobTrack.mTanImp", &CovGlobTrack_mTanImp, &b_CovGlobTrack_mTanImp);
//    fChain->SetBranchAddress("CovGlobTrack.mTanZ", &CovGlobTrack_mTanZ, &b_CovGlobTrack_mTanZ);
//    fChain->SetBranchAddress("CovGlobTrack.mTanPsi", &CovGlobTrack_mTanPsi, &b_CovGlobTrack_mTanPsi);
//    fChain->SetBranchAddress("CovGlobTrack.mTanPti", &CovGlobTrack_mTanPti, &b_CovGlobTrack_mTanPti);
//    fChain->SetBranchAddress("CovGlobTrack.mTanTan", &CovGlobTrack_mTanTan, &b_CovGlobTrack_mTanTan);
//    fChain->SetBranchAddress("CovPrimTrack", &CovPrimTrack_, &b_CovPrimTrack_);
//    fChain->SetBranchAddress("CovPrimTrack.mTanTan", &CovPrimTrack_mTanTan, &b_CovPrimTrack_mTanTan);
//    fChain->SetBranchAddress("CovPrimTrack.mPsiTan", &CovPrimTrack_mPsiTan, &b_CovPrimTrack_mPsiTan);
//    fChain->SetBranchAddress("CovPrimTrack.mPsiPsi", &CovPrimTrack_mPsiPsi, &b_CovPrimTrack_mPsiPsi);
//    fChain->SetBranchAddress("CovPrimTrack.mPtiTan", &CovPrimTrack_mPtiTan, &b_CovPrimTrack_mPtiTan);
//    fChain->SetBranchAddress("CovPrimTrack.mPtiPsi", &CovPrimTrack_mPtiPsi, &b_CovPrimTrack_mPtiPsi);
//    fChain->SetBranchAddress("CovPrimTrack.mPtiPti", &CovPrimTrack_mPtiPti, &b_CovPrimTrack_mPtiPti);
//    fChain->SetBranchAddress("pp2pp", &pp2pp_, &b_pp2pp_);
//    fChain->SetBranchAddress("pp2pp.fUniqueID", &pp2pp_fUniqueID, &b_pp2pp_fUniqueID);
//    fChain->SetBranchAddress("pp2pp.fBits", &pp2pp_fBits, &b_pp2pp_fBits);
//    fChain->SetBranchAddress("pp2pp.mSiliconBunch", &pp2pp_mSiliconBunch, &b_pp2pp_mSiliconBunch);
//    fChain->SetBranchAddress("pp2pp.mNumberPlanes[8]", &pp2pp_mNumberPlanes, &b_pp2pp_mNumberPlanes);
//    fChain->SetBranchAddress("pp2pp.mNumberPlanesWithClusters[8]", &pp2pp_mNumberPlanesWithClusters, &b_pp2pp_mNumberPlanesWithClusters);
//    fChain->SetBranchAddress("pp2pp.mStatusRomanPot[8]", &pp2pp_mStatusRomanPot, &b_pp2pp_mStatusRomanPot);
//    fChain->SetBranchAddress("pp2pp.mADC[8][2]", &pp2pp_mADC, &b_pp2pp_mADC);
//    fChain->SetBranchAddress("pp2pp.mTAC[8][2]", &pp2pp_mTAC, &b_pp2pp_mTAC);
//    fChain->SetBranchAddress("pp2pp.mOffsetPlane[8][4]", &pp2pp_mOffsetPlane, &b_pp2pp_mOffsetPlane);
//    fChain->SetBranchAddress("pp2pp.mzPlane[8][4]", &pp2pp_mzPlane, &b_pp2pp_mzPlane);
//    fChain->SetBranchAddress("pp2pp.mAnglePlane[8][4]", &pp2pp_mAnglePlane, &b_pp2pp_mAnglePlane);
//    fChain->SetBranchAddress("pp2pp.mOrientationPlane[8][4]", &pp2pp_mOrientationPlane, &b_pp2pp_mOrientationPlane);
//    fChain->SetBranchAddress("pp2pp.mStatusPlane[8][4]", &pp2pp_mStatusPlane, &b_pp2pp_mStatusPlane);
//    fChain->SetBranchAddress("pp2pp.mNumberOfClusters[8][4]", &pp2pp_mNumberOfClusters, &b_pp2pp_mNumberOfClusters);
//    fChain->SetBranchAddress("pp2pp.mPositionCluster[8][4]", &pp2pp_mPositionCluster, &b_pp2pp_mPositionCluster);
//    fChain->SetBranchAddress("pp2pp.mLengthCluster[8][4]", &pp2pp_mLengthCluster, &b_pp2pp_mLengthCluster);
//    fChain->SetBranchAddress("pp2pp.mEnergyCluster[8][4]", &pp2pp_mEnergyCluster, &b_pp2pp_mEnergyCluster);
//    fChain->SetBranchAddress("pp2pp.mXYCluster[8][4]", &pp2pp_mXYCluster, &b_pp2pp_mXYCluster);
//    fChain->SetBranchAddress("pp2pp.mQualityCluster[8][4]", &pp2pp_mQualityCluster, &b_pp2pp_mQualityCluster);
//    fChain->SetBranchAddress("mtd", &mtd_, &b_mtd_);
//    fChain->SetBranchAddress("mtd.fUniqueID", &mtd_fUniqueID, &b_mtd_fUniqueID);
//    fChain->SetBranchAddress("mtd.fBits", &mtd_fBits, &b_mtd_fBits);
//    fChain->SetBranchAddress("Event", &Event_, &b_Event_);
//    fChain->SetBranchAddress("Event.mRun", Event_mRun, &b_Event_mRun);
//    fChain->SetBranchAddress("Event.mEvent", Event_mEvent, &b_Event_mEvent);
//    fChain->SetBranchAddress("Event.mPrimaryVertexX", Event_mPrimaryVertexX, &b_Event_mPrimaryVertexX);
//    fChain->SetBranchAddress("Event.mPrimaryVertexY", Event_mPrimaryVertexY, &b_Event_mPrimaryVertexY);
//    fChain->SetBranchAddress("Event.mPrimaryVertexZ", Event_mPrimaryVertexZ, &b_Event_mPrimaryVertexZ);
//    fChain->SetBranchAddress("Event.mGlobalTracks", Event_mGlobalTracks, &b_Event_mGlobalTracks);
//    fChain->SetBranchAddress("Event.mPrimaryTracks", Event_mPrimaryTracks, &b_Event_mPrimaryTracks);
//    fChain->SetBranchAddress("Event.mPrimaryNegTracks", Event_mPrimaryNegTracks, &b_Event_mPrimaryNegTracks);
//    fChain->SetBranchAddress("Event.mMagneticField", Event_mMagneticField, &b_Event_mMagneticField);
//    fChain->SetBranchAddress("Event.mL0TriggerWord", Event_mL0TriggerWord, &b_Event_mL0TriggerWord);
//    fChain->SetBranchAddress("McEvent", &McEvent_, &b_McEvent_);
//    fChain->SetBranchAddress("McEvent.mRun", &McEvent_mRun, &b_McEvent_mRun);
//    fChain->SetBranchAddress("McEvent.mEvent", &McEvent_mEvent, &b_McEvent_mEvent);
//    fChain->SetBranchAddress("McEvent.mPrimaryVertexX", &McEvent_mPrimaryVertexX, &b_McEvent_mPrimaryVertexX);
//    fChain->SetBranchAddress("McEvent.mPrimaryVertexY", &McEvent_mPrimaryVertexY, &b_McEvent_mPrimaryVertexY);
//    fChain->SetBranchAddress("McEvent.mPrimaryVertexZ", &McEvent_mPrimaryVertexZ, &b_McEvent_mPrimaryVertexZ);
//    fChain->SetBranchAddress("McEvent.mGlobalTracks", &McEvent_mGlobalTracks, &b_McEvent_mGlobalTracks);
//    fChain->SetBranchAddress("McEvent.mPrimaryTracks", &McEvent_mPrimaryTracks, &b_McEvent_mPrimaryTracks);
//    fChain->SetBranchAddress("McEvent.mPrimaryNegTracks", &McEvent_mPrimaryNegTracks, &b_McEvent_mPrimaryNegTracks);
//    fChain->SetBranchAddress("McEvent.mMagneticField", &McEvent_mMagneticField, &b_McEvent_mMagneticField);
//    fChain->SetBranchAddress("McEvent.mL0TriggerWord", &McEvent_mL0TriggerWord, &b_McEvent_mL0TriggerWord);
//    fChain->SetBranchAddress("V0", &V0_, &b_V0_);
//    fChain->SetBranchAddress("V0.mDecayVertexV0X", &V0_mDecayVertexV0X, &b_V0_mDecayVertexV0X);
//    fChain->SetBranchAddress("V0.mDecayVertexV0Y", &V0_mDecayVertexV0Y, &b_V0_mDecayVertexV0Y);
//    fChain->SetBranchAddress("V0.mDecayVertexV0Z", &V0_mDecayVertexV0Z, &b_V0_mDecayVertexV0Z);
//    fChain->SetBranchAddress("V0.mDcaV0Daughters", &V0_mDcaV0Daughters, &b_V0_mDcaV0Daughters);
//    fChain->SetBranchAddress("V0.mDcaV0ToPrimVertex", &V0_mDcaV0ToPrimVertex, &b_V0_mDcaV0ToPrimVertex);
//    fChain->SetBranchAddress("V0.mDcaPosToPrimVertex", &V0_mDcaPosToPrimVertex, &b_V0_mDcaPosToPrimVertex);
//    fChain->SetBranchAddress("V0.mDcaNegToPrimVertex", &V0_mDcaNegToPrimVertex, &b_V0_mDcaNegToPrimVertex);
//    fChain->SetBranchAddress("V0.mMomPosX", &V0_mMomPosX, &b_V0_mMomPosX);
//    fChain->SetBranchAddress("V0.mMomPosY", &V0_mMomPosY, &b_V0_mMomPosY);
//    fChain->SetBranchAddress("V0.mMomPosZ", &V0_mMomPosZ, &b_V0_mMomPosZ);
//    fChain->SetBranchAddress("V0.mMomNegX", &V0_mMomNegX, &b_V0_mMomNegX);
//    fChain->SetBranchAddress("V0.mMomNegY", &V0_mMomNegY, &b_V0_mMomNegY);
//    fChain->SetBranchAddress("V0.mMomNegZ", &V0_mMomNegZ, &b_V0_mMomNegZ);
//    fChain->SetBranchAddress("V0.mKeyPos", &V0_mKeyPos, &b_V0_mKeyPos);
//    fChain->SetBranchAddress("V0.mKeyNeg", &V0_mKeyNeg, &b_V0_mKeyNeg);
//    fChain->SetBranchAddress("V0.mTopologyMapPos.mMap0", &V0_mTopologyMapPos_mMap0, &b_V0_mTopologyMapPos_mMap0);
//    fChain->SetBranchAddress("V0.mTopologyMapPos.mMap1", &V0_mTopologyMapPos_mMap1, &b_V0_mTopologyMapPos_mMap1);
//    fChain->SetBranchAddress("V0.mTopologyMapNeg.mMap0", &V0_mTopologyMapNeg_mMap0, &b_V0_mTopologyMapNeg_mMap0);
//    fChain->SetBranchAddress("V0.mTopologyMapNeg.mMap1", &V0_mTopologyMapNeg_mMap1, &b_V0_mTopologyMapNeg_mMap1);
//    fChain->SetBranchAddress("V0.mChi2V0", &V0_mChi2V0, &b_V0_mChi2V0);
//    fChain->SetBranchAddress("V0.mClV0", &V0_mClV0, &b_V0_mClV0);
//    fChain->SetBranchAddress("V0.mChi2Pos", &V0_mChi2Pos, &b_V0_mChi2Pos);
//    fChain->SetBranchAddress("V0.mClPos", &V0_mClPos, &b_V0_mClPos);
//    fChain->SetBranchAddress("V0.mChi2Neg", &V0_mChi2Neg, &b_V0_mChi2Neg);
//    fChain->SetBranchAddress("V0.mClNeg", &V0_mClNeg, &b_V0_mClNeg);
//    fChain->SetBranchAddress("V0.mDedxPos", &V0_mDedxPos, &b_V0_mDedxPos);
//    fChain->SetBranchAddress("V0.mDedxNeg", &V0_mDedxNeg, &b_V0_mDedxNeg);
//    fChain->SetBranchAddress("V0.mErrDedxPos", &V0_mErrDedxPos, &b_V0_mErrDedxPos);
//    fChain->SetBranchAddress("V0.mErrDedxNeg", &V0_mErrDedxNeg, &b_V0_mErrDedxNeg);
//    fChain->SetBranchAddress("V0.mNumDedxPos", &V0_mNumDedxPos, &b_V0_mNumDedxPos);
//    fChain->SetBranchAddress("V0.mNumDedxNeg", &V0_mNumDedxNeg, &b_V0_mNumDedxNeg);
//    fChain->SetBranchAddress("McV0", &McV0_, &b_McV0_);
//    fChain->SetBranchAddress("McV0.mPositiveSimTpcHits", &McV0_mPositiveSimTpcHits, &b_McV0_mPositiveSimTpcHits);
//    fChain->SetBranchAddress("McV0.mPositiveCommonTpcHits", &McV0_mPositiveCommonTpcHits, &b_McV0_mPositiveCommonTpcHits);
//    fChain->SetBranchAddress("McV0.mNegativeSimTpcHits", &McV0_mNegativeSimTpcHits, &b_McV0_mNegativeSimTpcHits);
//    fChain->SetBranchAddress("McV0.mNegativeCommonTpcHits", &McV0_mNegativeCommonTpcHits, &b_McV0_mNegativeCommonTpcHits);
//    fChain->SetBranchAddress("McV0.mDecayMode", &McV0_mDecayMode, &b_McV0_mDecayMode);
//    fChain->SetBranchAddress("McV0.mParentGeantId", &McV0_mParentGeantId, &b_McV0_mParentGeantId);
//    fChain->SetBranchAddress("McV0.mPositiveGeantId", &McV0_mPositiveGeantId, &b_McV0_mPositiveGeantId);
//    fChain->SetBranchAddress("McV0.mNegativeGeantId", &McV0_mNegativeGeantId, &b_McV0_mNegativeGeantId);
//    fChain->SetBranchAddress("McV0.mParentMomentumX", &McV0_mParentMomentumX, &b_McV0_mParentMomentumX);
//    fChain->SetBranchAddress("McV0.mParentMomentumY", &McV0_mParentMomentumY, &b_McV0_mParentMomentumY);
//    fChain->SetBranchAddress("McV0.mParentMomentumZ", &McV0_mParentMomentumZ, &b_McV0_mParentMomentumZ);
//    fChain->SetBranchAddress("McV0.mPositiveMomentumX", &McV0_mPositiveMomentumX, &b_McV0_mPositiveMomentumX);
//    fChain->SetBranchAddress("McV0.mPositiveMomentumY", &McV0_mPositiveMomentumY, &b_McV0_mPositiveMomentumY);
//    fChain->SetBranchAddress("McV0.mPositiveMomentumZ", &McV0_mPositiveMomentumZ, &b_McV0_mPositiveMomentumZ);
//    fChain->SetBranchAddress("McV0.mNegativeMomentumX", &McV0_mNegativeMomentumX, &b_McV0_mNegativeMomentumX);
//    fChain->SetBranchAddress("McV0.mNegativeMomentumY", &McV0_mNegativeMomentumY, &b_McV0_mNegativeMomentumY);
//    fChain->SetBranchAddress("McV0.mNegativeMomentumZ", &McV0_mNegativeMomentumZ, &b_McV0_mNegativeMomentumZ);
//    fChain->SetBranchAddress("McV0.mPositionX", &McV0_mPositionX, &b_McV0_mPositionX);
//    fChain->SetBranchAddress("McV0.mPositionY", &McV0_mPositionY, &b_McV0_mPositionY);
//    fChain->SetBranchAddress("McV0.mPositionZ", &McV0_mPositionZ, &b_McV0_mPositionZ);
//    fChain->SetBranchAddress("V0Assoc", &V0Assoc_, &b_V0Assoc_);
//    fChain->SetBranchAddress("V0Assoc.mIndexRecoArray", &V0Assoc_mIndexRecoArray, &b_V0Assoc_mIndexRecoArray);
//    fChain->SetBranchAddress("V0Assoc.mIndexMcArray", &V0Assoc_mIndexMcArray, &b_V0Assoc_mIndexMcArray);
//    fChain->SetBranchAddress("Xi", &Xi_, &b_Xi_);
//    fChain->SetBranchAddress("Xi.mDecayVertexV0X", &Xi_mDecayVertexV0X, &b_Xi_mDecayVertexV0X);
//    fChain->SetBranchAddress("Xi.mDecayVertexV0Y", &Xi_mDecayVertexV0Y, &b_Xi_mDecayVertexV0Y);
//    fChain->SetBranchAddress("Xi.mDecayVertexV0Z", &Xi_mDecayVertexV0Z, &b_Xi_mDecayVertexV0Z);
//    fChain->SetBranchAddress("Xi.mDcaV0Daughters", &Xi_mDcaV0Daughters, &b_Xi_mDcaV0Daughters);
//    fChain->SetBranchAddress("Xi.mDcaV0ToPrimVertex", &Xi_mDcaV0ToPrimVertex, &b_Xi_mDcaV0ToPrimVertex);
//    fChain->SetBranchAddress("Xi.mDcaPosToPrimVertex", &Xi_mDcaPosToPrimVertex, &b_Xi_mDcaPosToPrimVertex);
//    fChain->SetBranchAddress("Xi.mDcaNegToPrimVertex", &Xi_mDcaNegToPrimVertex, &b_Xi_mDcaNegToPrimVertex);
//    fChain->SetBranchAddress("Xi.mMomPosX", &Xi_mMomPosX, &b_Xi_mMomPosX);
//    fChain->SetBranchAddress("Xi.mMomPosY", &Xi_mMomPosY, &b_Xi_mMomPosY);
//    fChain->SetBranchAddress("Xi.mMomPosZ", &Xi_mMomPosZ, &b_Xi_mMomPosZ);
//    fChain->SetBranchAddress("Xi.mMomNegX", &Xi_mMomNegX, &b_Xi_mMomNegX);
//    fChain->SetBranchAddress("Xi.mMomNegY", &Xi_mMomNegY, &b_Xi_mMomNegY);
//    fChain->SetBranchAddress("Xi.mMomNegZ", &Xi_mMomNegZ, &b_Xi_mMomNegZ);
//    fChain->SetBranchAddress("Xi.mKeyPos", &Xi_mKeyPos, &b_Xi_mKeyPos);
//    fChain->SetBranchAddress("Xi.mKeyNeg", &Xi_mKeyNeg, &b_Xi_mKeyNeg);
//    fChain->SetBranchAddress("Xi.mTopologyMapPos.mMap0", &Xi_mTopologyMapPos_mMap0, &b_Xi_mTopologyMapPos_mMap0);
//    fChain->SetBranchAddress("Xi.mTopologyMapPos.mMap1", &Xi_mTopologyMapPos_mMap1, &b_Xi_mTopologyMapPos_mMap1);
//    fChain->SetBranchAddress("Xi.mTopologyMapNeg.mMap0", &Xi_mTopologyMapNeg_mMap0, &b_Xi_mTopologyMapNeg_mMap0);
//    fChain->SetBranchAddress("Xi.mTopologyMapNeg.mMap1", &Xi_mTopologyMapNeg_mMap1, &b_Xi_mTopologyMapNeg_mMap1);
//    fChain->SetBranchAddress("Xi.mChi2V0", &Xi_mChi2V0, &b_Xi_mChi2V0);
//    fChain->SetBranchAddress("Xi.mClV0", &Xi_mClV0, &b_Xi_mClV0);
//    fChain->SetBranchAddress("Xi.mChi2Pos", &Xi_mChi2Pos, &b_Xi_mChi2Pos);
//    fChain->SetBranchAddress("Xi.mClPos", &Xi_mClPos, &b_Xi_mClPos);
//    fChain->SetBranchAddress("Xi.mChi2Neg", &Xi_mChi2Neg, &b_Xi_mChi2Neg);
//    fChain->SetBranchAddress("Xi.mClNeg", &Xi_mClNeg, &b_Xi_mClNeg);
//    fChain->SetBranchAddress("Xi.mDedxPos", &Xi_mDedxPos, &b_Xi_mDedxPos);
//    fChain->SetBranchAddress("Xi.mDedxNeg", &Xi_mDedxNeg, &b_Xi_mDedxNeg);
//    fChain->SetBranchAddress("Xi.mErrDedxPos", &Xi_mErrDedxPos, &b_Xi_mErrDedxPos);
//    fChain->SetBranchAddress("Xi.mErrDedxNeg", &Xi_mErrDedxNeg, &b_Xi_mErrDedxNeg);
//    fChain->SetBranchAddress("Xi.mNumDedxPos", &Xi_mNumDedxPos, &b_Xi_mNumDedxPos);
//    fChain->SetBranchAddress("Xi.mNumDedxNeg", &Xi_mNumDedxNeg, &b_Xi_mNumDedxNeg);
//    fChain->SetBranchAddress("Xi.mCharge", &Xi_mCharge, &b_Xi_mCharge);
//    fChain->SetBranchAddress("Xi.mDecayVertexXiX", &Xi_mDecayVertexXiX, &b_Xi_mDecayVertexXiX);
//    fChain->SetBranchAddress("Xi.mDecayVertexXiY", &Xi_mDecayVertexXiY, &b_Xi_mDecayVertexXiY);
//    fChain->SetBranchAddress("Xi.mDecayVertexXiZ", &Xi_mDecayVertexXiZ, &b_Xi_mDecayVertexXiZ);
//    fChain->SetBranchAddress("Xi.mDcaXiDaughters", &Xi_mDcaXiDaughters, &b_Xi_mDcaXiDaughters);
//    fChain->SetBranchAddress("Xi.mDcaBachelorToPrimVertex", &Xi_mDcaBachelorToPrimVertex, &b_Xi_mDcaBachelorToPrimVertex);
//    fChain->SetBranchAddress("Xi.mDcaXiToPrimVertex", &Xi_mDcaXiToPrimVertex, &b_Xi_mDcaXiToPrimVertex);
//    fChain->SetBranchAddress("Xi.mMomBachelorX", &Xi_mMomBachelorX, &b_Xi_mMomBachelorX);
//    fChain->SetBranchAddress("Xi.mMomBachelorY", &Xi_mMomBachelorY, &b_Xi_mMomBachelorY);
//    fChain->SetBranchAddress("Xi.mMomBachelorZ", &Xi_mMomBachelorZ, &b_Xi_mMomBachelorZ);
//    fChain->SetBranchAddress("Xi.mKeyBachelor", &Xi_mKeyBachelor, &b_Xi_mKeyBachelor);
//    fChain->SetBranchAddress("Xi.mTopologyMapBachelor.mMap0", &Xi_mTopologyMapBachelor_mMap0, &b_Xi_mTopologyMapBachelor_mMap0);
//    fChain->SetBranchAddress("Xi.mTopologyMapBachelor.mMap1", &Xi_mTopologyMapBachelor_mMap1, &b_Xi_mTopologyMapBachelor_mMap1);
//    fChain->SetBranchAddress("Xi.mChi2Xi", &Xi_mChi2Xi, &b_Xi_mChi2Xi);
//    fChain->SetBranchAddress("Xi.mClXi", &Xi_mClXi, &b_Xi_mClXi);
//    fChain->SetBranchAddress("Xi.mChi2Bachelor", &Xi_mChi2Bachelor, &b_Xi_mChi2Bachelor);
//    fChain->SetBranchAddress("Xi.mClBachelor", &Xi_mClBachelor, &b_Xi_mClBachelor);
//    fChain->SetBranchAddress("Xi.mDedxBachelor", &Xi_mDedxBachelor, &b_Xi_mDedxBachelor);
//    fChain->SetBranchAddress("Xi.mErrDedxBachelor", &Xi_mErrDedxBachelor, &b_Xi_mErrDedxBachelor);
//    fChain->SetBranchAddress("Xi.mNumDedxBachelor", &Xi_mNumDedxBachelor, &b_Xi_mNumDedxBachelor);
//    fChain->SetBranchAddress("McXi", &McXi_, &b_McXi_);
//    fChain->SetBranchAddress("McXi.mParentGeantId", &McXi_mParentGeantId, &b_McXi_mParentGeantId);
//    fChain->SetBranchAddress("McXi.mDaughterGeantId", &McXi_mDaughterGeantId, &b_McXi_mDaughterGeantId);
//    fChain->SetBranchAddress("McXi.mParentMomentumX", &McXi_mParentMomentumX, &b_McXi_mParentMomentumX);
//    fChain->SetBranchAddress("McXi.mParentMomentumY", &McXi_mParentMomentumY, &b_McXi_mParentMomentumY);
//    fChain->SetBranchAddress("McXi.mParentMomentumZ", &McXi_mParentMomentumZ, &b_McXi_mParentMomentumZ);
//    fChain->SetBranchAddress("McXi.mParentPrimMomentumX", &McXi_mParentPrimMomentumX, &b_McXi_mParentPrimMomentumX);
//    fChain->SetBranchAddress("McXi.mParentPrimMomentumY", &McXi_mParentPrimMomentumY, &b_McXi_mParentPrimMomentumY);
//    fChain->SetBranchAddress("McXi.mParentPrimMomentumZ", &McXi_mParentPrimMomentumZ, &b_McXi_mParentPrimMomentumZ);
//    fChain->SetBranchAddress("McXi.mDaughterMomentumX", &McXi_mDaughterMomentumX, &b_McXi_mDaughterMomentumX);
//    fChain->SetBranchAddress("McXi.mDaughterMomentumY", &McXi_mDaughterMomentumY, &b_McXi_mDaughterMomentumY);
//    fChain->SetBranchAddress("McXi.mDaughterMomentumZ", &McXi_mDaughterMomentumZ, &b_McXi_mDaughterMomentumZ);
//    fChain->SetBranchAddress("McXi.mPositionX", &McXi_mPositionX, &b_McXi_mPositionX);
//    fChain->SetBranchAddress("McXi.mPositionY", &McXi_mPositionY, &b_McXi_mPositionY);
//    fChain->SetBranchAddress("McXi.mPositionZ", &McXi_mPositionZ, &b_McXi_mPositionZ);
//    fChain->SetBranchAddress("McXi.mSimTpcHits", &McXi_mSimTpcHits, &b_McXi_mSimTpcHits);
//    fChain->SetBranchAddress("McXi.mCommonTpcHits", &McXi_mCommonTpcHits, &b_McXi_mCommonTpcHits);
//    fChain->SetBranchAddress("McXi.mDecayMode", &McXi_mDecayMode, &b_McXi_mDecayMode);
//    fChain->SetBranchAddress("McXi.v0", &McXi_v0, &b_McXi_v0);
//    fChain->SetBranchAddress("XiAssoc", &XiAssoc_, &b_XiAssoc_);
//    fChain->SetBranchAddress("XiAssoc.mIndexRecoArray", &XiAssoc_mIndexRecoArray, &b_XiAssoc_mIndexRecoArray);
//    fChain->SetBranchAddress("XiAssoc.mIndexMcArray", &XiAssoc_mIndexMcArray, &b_XiAssoc_mIndexMcArray);
//    fChain->SetBranchAddress("Kink", &Kink_, &b_Kink_);
//    fChain->SetBranchAddress("Kink.mParentGeantId", &Kink_mParentGeantId, &b_Kink_mParentGeantId);
//    fChain->SetBranchAddress("Kink.mDaughterGeantId", &Kink_mDaughterGeantId, &b_Kink_mDaughterGeantId);
//    fChain->SetBranchAddress("Kink.mParentMomentumX", &Kink_mParentMomentumX, &b_Kink_mParentMomentumX);
//    fChain->SetBranchAddress("Kink.mParentMomentumY", &Kink_mParentMomentumY, &b_Kink_mParentMomentumY);
//    fChain->SetBranchAddress("Kink.mParentMomentumZ", &Kink_mParentMomentumZ, &b_Kink_mParentMomentumZ);
//    fChain->SetBranchAddress("Kink.mParentPrimMomentumX", &Kink_mParentPrimMomentumX, &b_Kink_mParentPrimMomentumX);
//    fChain->SetBranchAddress("Kink.mParentPrimMomentumY", &Kink_mParentPrimMomentumY, &b_Kink_mParentPrimMomentumY);
//    fChain->SetBranchAddress("Kink.mParentPrimMomentumZ", &Kink_mParentPrimMomentumZ, &b_Kink_mParentPrimMomentumZ);
//    fChain->SetBranchAddress("Kink.mDaughterMomentumX", &Kink_mDaughterMomentumX, &b_Kink_mDaughterMomentumX);
//    fChain->SetBranchAddress("Kink.mDaughterMomentumY", &Kink_mDaughterMomentumY, &b_Kink_mDaughterMomentumY);
//    fChain->SetBranchAddress("Kink.mDaughterMomentumZ", &Kink_mDaughterMomentumZ, &b_Kink_mDaughterMomentumZ);
//    fChain->SetBranchAddress("Kink.mPositionX", &Kink_mPositionX, &b_Kink_mPositionX);
//    fChain->SetBranchAddress("Kink.mPositionY", &Kink_mPositionY, &b_Kink_mPositionY);
//    fChain->SetBranchAddress("Kink.mPositionZ", &Kink_mPositionZ, &b_Kink_mPositionZ);
//    fChain->SetBranchAddress("Kink.mDcaParentDaughter", &Kink_mDcaParentDaughter, &b_Kink_mDcaParentDaughter);
//    fChain->SetBranchAddress("Kink.mDcaDaughterPrimaryVertex", &Kink_mDcaDaughterPrimaryVertex, &b_Kink_mDcaDaughterPrimaryVertex);
//    fChain->SetBranchAddress("Kink.mDcaParentPrimaryVertex", &Kink_mDcaParentPrimaryVertex, &b_Kink_mDcaParentPrimaryVertex);
//    fChain->SetBranchAddress("Kink.mHitDistanceParentDaughter", &Kink_mHitDistanceParentDaughter, &b_Kink_mHitDistanceParentDaughter);
//    fChain->SetBranchAddress("Kink.mHitDistanceParentVertex", &Kink_mHitDistanceParentVertex, &b_Kink_mHitDistanceParentVertex);
//    fChain->SetBranchAddress("Kink.mMinDeltaEnergy", &Kink_mMinDeltaEnergy, &b_Kink_mMinDeltaEnergy);
//    fChain->SetBranchAddress("Kink.mDecayAngle", &Kink_mDecayAngle, &b_Kink_mDecayAngle);
//    fChain->SetBranchAddress("Kink.mParentMomentum", &Kink_mParentMomentum, &b_Kink_mParentMomentum);
//    fChain->SetBranchAddress("Kink.mParentPrimMomentum", &Kink_mParentPrimMomentum, &b_Kink_mParentPrimMomentum);
//    fChain->SetBranchAddress("Kink.mParentCharge", &Kink_mParentCharge, &b_Kink_mParentCharge);
//    fChain->SetBranchAddress("Kink.mDaughterMomentum", &Kink_mDaughterMomentum, &b_Kink_mDaughterMomentum);
//    fChain->SetBranchAddress("Kink.mDaughterCharge", &Kink_mDaughterCharge, &b_Kink_mDaughterCharge);
//    fChain->SetBranchAddress("Kink.mDecayLength", &Kink_mDecayLength, &b_Kink_mDecayLength);
//    fChain->SetBranchAddress("Kink.mTransverseMomentum", &Kink_mTransverseMomentum, &b_Kink_mTransverseMomentum);
//    fChain->SetBranchAddress("Kink.mTransverseMassKaon", &Kink_mTransverseMassKaon, &b_Kink_mTransverseMassKaon);
//    fChain->SetBranchAddress("Kink.mTransverseMassPion", &Kink_mTransverseMassPion, &b_Kink_mTransverseMassPion);
//    fChain->SetBranchAddress("Kink.mRapidityKaon", &Kink_mRapidityKaon, &b_Kink_mRapidityKaon);
//    fChain->SetBranchAddress("Kink.mRapidityPion", &Kink_mRapidityPion, &b_Kink_mRapidityPion);
//    fChain->SetBranchAddress("Kink.mChi2Kink", &Kink_mChi2Kink, &b_Kink_mChi2Kink);
//    fChain->SetBranchAddress("Kink.mClKink", &Kink_mClKink, &b_Kink_mClKink);
//    fChain->SetBranchAddress("Kink.mChi2Parent", &Kink_mChi2Parent, &b_Kink_mChi2Parent);
//    fChain->SetBranchAddress("Kink.mClParent", &Kink_mClParent, &b_Kink_mClParent);
//    fChain->SetBranchAddress("Kink.mChi2Daughter", &Kink_mChi2Daughter, &b_Kink_mChi2Daughter);
//    fChain->SetBranchAddress("Kink.mClDaughter", &Kink_mClDaughter, &b_Kink_mClDaughter);
//    fChain->SetBranchAddress("Kink.mDedxParent", &Kink_mDedxParent, &b_Kink_mDedxParent);
//    fChain->SetBranchAddress("Kink.mDedxDaughter", &Kink_mDedxDaughter, &b_Kink_mDedxDaughter);
//    fChain->SetBranchAddress("Kink.mErrDedxParent", &Kink_mErrDedxParent, &b_Kink_mErrDedxParent);
//    fChain->SetBranchAddress("Kink.mErrDedxDaughter", &Kink_mErrDedxDaughter, &b_Kink_mErrDedxDaughter);
//    fChain->SetBranchAddress("Kink.mNumDedxParent", &Kink_mNumDedxParent, &b_Kink_mNumDedxParent);
//    fChain->SetBranchAddress("Kink.mNumDedxDaughter", &Kink_mNumDedxDaughter, &b_Kink_mNumDedxDaughter);
//    fChain->SetBranchAddress("Kink.mKeyParent", &Kink_mKeyParent, &b_Kink_mKeyParent);
//    fChain->SetBranchAddress("Kink.mKeyDaughter", &Kink_mKeyDaughter, &b_Kink_mKeyDaughter);
//    fChain->SetBranchAddress("McKink", &McKink_, &b_McKink_);
//    fChain->SetBranchAddress("McKink.mParentGeantId", &McKink_mParentGeantId, &b_McKink_mParentGeantId);
//    fChain->SetBranchAddress("McKink.mDaughterGeantId", &McKink_mDaughterGeantId, &b_McKink_mDaughterGeantId);
//    fChain->SetBranchAddress("McKink.mParentMomentumX", &McKink_mParentMomentumX, &b_McKink_mParentMomentumX);
//    fChain->SetBranchAddress("McKink.mParentMomentumY", &McKink_mParentMomentumY, &b_McKink_mParentMomentumY);
//    fChain->SetBranchAddress("McKink.mParentMomentumZ", &McKink_mParentMomentumZ, &b_McKink_mParentMomentumZ);
//    fChain->SetBranchAddress("McKink.mParentPrimMomentumX", &McKink_mParentPrimMomentumX, &b_McKink_mParentPrimMomentumX);
//    fChain->SetBranchAddress("McKink.mParentPrimMomentumY", &McKink_mParentPrimMomentumY, &b_McKink_mParentPrimMomentumY);
//    fChain->SetBranchAddress("McKink.mParentPrimMomentumZ", &McKink_mParentPrimMomentumZ, &b_McKink_mParentPrimMomentumZ);
//    fChain->SetBranchAddress("McKink.mDaughterMomentumX", &McKink_mDaughterMomentumX, &b_McKink_mDaughterMomentumX);
//    fChain->SetBranchAddress("McKink.mDaughterMomentumY", &McKink_mDaughterMomentumY, &b_McKink_mDaughterMomentumY);
//    fChain->SetBranchAddress("McKink.mDaughterMomentumZ", &McKink_mDaughterMomentumZ, &b_McKink_mDaughterMomentumZ);
//    fChain->SetBranchAddress("McKink.mPositionX", &McKink_mPositionX, &b_McKink_mPositionX);
//    fChain->SetBranchAddress("McKink.mPositionY", &McKink_mPositionY, &b_McKink_mPositionY);
//    fChain->SetBranchAddress("McKink.mPositionZ", &McKink_mPositionZ, &b_McKink_mPositionZ);
//    fChain->SetBranchAddress("McKink.mSimTpcHits", &McKink_mSimTpcHits, &b_McKink_mSimTpcHits);
//    fChain->SetBranchAddress("McKink.mCommonTpcHits", &McKink_mCommonTpcHits, &b_McKink_mCommonTpcHits);
//    fChain->SetBranchAddress("McKink.mDecayMode", &McKink_mDecayMode, &b_McKink_mDecayMode);
//    fChain->SetBranchAddress("KinkAssoc", &KinkAssoc_, &b_KinkAssoc_);
//    fChain->SetBranchAddress("KinkAssoc.mIndexRecoArray", &KinkAssoc_mIndexRecoArray, &b_KinkAssoc_mIndexRecoArray);
//    fChain->SetBranchAddress("KinkAssoc.mIndexMcArray", &KinkAssoc_mIndexMcArray, &b_KinkAssoc_mIndexMcArray);
//    fChain->SetBranchAddress("StrangeCuts", &StrangeCuts_, &b_StrangeCuts_);
//    fChain->SetBranchAddress("StrangeCuts.fName", StrangeCuts_fName, &b_StrangeCuts_fName);
//    fChain->SetBranchAddress("StrangeCuts.fTitle", StrangeCuts_fTitle, &b_StrangeCuts_fTitle);
//    fChain->SetBranchAddress("StMuMcVertex", &StMuMcVertex_, &b_StMuMcVertex_);
//    fChain->SetBranchAddress("StMuMcVertex.mId", &StMuMcVertex_mId, &b_StMuMcVertex_mId);
//    fChain->SetBranchAddress("StMuMcVertex.mNoDaughters", &StMuMcVertex_mNoDaughters, &b_StMuMcVertex_mNoDaughters);
//    fChain->SetBranchAddress("StMuMcVertex.mIdParTrk", &StMuMcVertex_mIdParTrk, &b_StMuMcVertex_mIdParTrk);
//    fChain->SetBranchAddress("StMuMcVertex.mIsInterm", &StMuMcVertex_mIsInterm, &b_StMuMcVertex_mIsInterm);
//    fChain->SetBranchAddress("StMuMcVertex.mTime", &StMuMcVertex_mTime, &b_StMuMcVertex_mTime);
//    fChain->SetBranchAddress("StMuMcVertex.mXyzV.mX1", &StMuMcVertex_mXyzV_mX1, &b_StMuMcVertex_mXyzV_mX1);
//    fChain->SetBranchAddress("StMuMcVertex.mXyzV.mX2", &StMuMcVertex_mXyzV_mX2, &b_StMuMcVertex_mXyzV_mX2);
//    fChain->SetBranchAddress("StMuMcVertex.mXyzV.mX3", &StMuMcVertex_mXyzV_mX3, &b_StMuMcVertex_mXyzV_mX3);
//    fChain->SetBranchAddress("StMuMcTrack", &StMuMcTrack_, &b_StMuMcTrack_);
//    fChain->SetBranchAddress("StMuMcTrack.mGePid", &StMuMcTrack_mGePid, &b_StMuMcTrack_mGePid);
//    fChain->SetBranchAddress("StMuMcTrack.mId", &StMuMcTrack_mId, &b_StMuMcTrack_mId);
//    fChain->SetBranchAddress("StMuMcTrack.mIsShower", &StMuMcTrack_mIsShower, &b_StMuMcTrack_mIsShower);
//    fChain->SetBranchAddress("StMuMcTrack.mHits[22]", &StMuMcTrack_mHits, &b_StMuMcTrack_mHits);
//    fChain->SetBranchAddress("StMuMcTrack.mItrmdVertex", &StMuMcTrack_mItrmdVertex, &b_StMuMcTrack_mItrmdVertex);
//    fChain->SetBranchAddress("StMuMcTrack.mIdVx", &StMuMcTrack_mIdVx, &b_StMuMcTrack_mIdVx);
//    fChain->SetBranchAddress("StMuMcTrack.mIdVxEnd", &StMuMcTrack_mIdVxEnd, &b_StMuMcTrack_mIdVxEnd);
//    fChain->SetBranchAddress("StMuMcTrack.mCharge", &StMuMcTrack_mCharge, &b_StMuMcTrack_mCharge);
//    fChain->SetBranchAddress("StMuMcTrack.mE", &StMuMcTrack_mE, &b_StMuMcTrack_mE);
//    fChain->SetBranchAddress("StMuMcTrack.mEta", &StMuMcTrack_mEta, &b_StMuMcTrack_mEta);
//    fChain->SetBranchAddress("StMuMcTrack.mPxyz.mX1", &StMuMcTrack_mPxyz_mX1, &b_StMuMcTrack_mPxyz_mX1);
//    fChain->SetBranchAddress("StMuMcTrack.mPxyz.mX2", &StMuMcTrack_mPxyz_mX2, &b_StMuMcTrack_mPxyz_mX2);
//    fChain->SetBranchAddress("StMuMcTrack.mPxyz.mX3", &StMuMcTrack_mPxyz_mX3, &b_StMuMcTrack_mPxyz_mX3);
//    fChain->SetBranchAddress("StMuMcTrack.mpT", &StMuMcTrack_mpT, &b_StMuMcTrack_mpT);
//    fChain->SetBranchAddress("StMuMcTrack.mPtot", &StMuMcTrack_mPtot, &b_StMuMcTrack_mPtot);
//    fChain->SetBranchAddress("StMuMcTrack.mRapidity", &StMuMcTrack_mRapidity, &b_StMuMcTrack_mRapidity);
//    fChain->SetBranchAddress("EmcTow", &EmcTow_, &b_EmcTow_);
//    fChain->SetBranchAddress("EmcTow.mTowerADC[4800]", EmcTow_mTowerADC, &b_EmcTow_mTowerADC);
//    fChain->SetBranchAddress("EmcTow.mEndcapTowerADC[720]", EmcTow_mEndcapTowerADC, &b_EmcTow_mEndcapTowerADC);
//    fChain->SetBranchAddress("EmcTow.mBTowCrateFlags[30]", EmcTow_mBTowCrateFlags, &b_EmcTow_mBTowCrateFlags);
//    fChain->SetBranchAddress("EmcTow.mBSmdCrateFlags[8]", EmcTow_mBSmdCrateFlags, &b_EmcTow_mBSmdCrateFlags);
//    fChain->SetBranchAddress("EmcTow.mBPrsCrateFlags[4]", EmcTow_mBPrsCrateFlags, &b_EmcTow_mBPrsCrateFlags);
//    fChain->SetBranchAddress("EmcTow.mETowCrateFlags[6]", EmcTow_mETowCrateFlags, &b_EmcTow_mETowCrateFlags);
//    fChain->SetBranchAddress("EmcTow.mESmdCrateFlags[36]", EmcTow_mESmdCrateFlags, &b_EmcTow_mESmdCrateFlags);
//    fChain->SetBranchAddress("EmcTow.mEPrsCrateFlags[12]", EmcTow_mEPrsCrateFlags, &b_EmcTow_mEPrsCrateFlags);
//    fChain->SetBranchAddress("EmcPrs", &EmcPrs_, &b_EmcPrs_);
//    fChain->SetBranchAddress("EmcPrs.mEnergy", &EmcPrs_mEnergy, &b_EmcPrs_mEnergy);
//    fChain->SetBranchAddress("EmcPrs.mId", &EmcPrs_mId, &b_EmcPrs_mId);
//    fChain->SetBranchAddress("EmcPrs.mAdc", &EmcPrs_mAdc, &b_EmcPrs_mAdc);
//    fChain->SetBranchAddress("EmcPrs.mCalType", &EmcPrs_mCalType, &b_EmcPrs_mCalType);
//    fChain->SetBranchAddress("EmcSmde", &EmcSmde_, &b_EmcSmde_);
//    fChain->SetBranchAddress("EmcSmde.mEnergy", &EmcSmde_mEnergy, &b_EmcSmde_mEnergy);
//    fChain->SetBranchAddress("EmcSmde.mId", &EmcSmde_mId, &b_EmcSmde_mId);
//    fChain->SetBranchAddress("EmcSmde.mAdc", &EmcSmde_mAdc, &b_EmcSmde_mAdc);
//    fChain->SetBranchAddress("EmcSmde.mCalType", &EmcSmde_mCalType, &b_EmcSmde_mCalType);
//    fChain->SetBranchAddress("EmcSmdp", &EmcSmdp_, &b_EmcSmdp_);
//    fChain->SetBranchAddress("EmcSmdp.mEnergy", &EmcSmdp_mEnergy, &b_EmcSmdp_mEnergy);
//    fChain->SetBranchAddress("EmcSmdp.mId", &EmcSmdp_mId, &b_EmcSmdp_mId);
//    fChain->SetBranchAddress("EmcSmdp.mAdc", &EmcSmdp_mAdc, &b_EmcSmdp_mAdc);
//    fChain->SetBranchAddress("EmcSmdp.mCalType", &EmcSmdp_mCalType, &b_EmcSmdp_mCalType);
//    fChain->SetBranchAddress("EEmcPrs", &EEmcPrs_, &b_EEmcPrs_);
//    fChain->SetBranchAddress("EEmcPrs.mEnergy", &EEmcPrs_mEnergy, &b_EEmcPrs_mEnergy);
//    fChain->SetBranchAddress("EEmcPrs.mId", &EEmcPrs_mId, &b_EEmcPrs_mId);
//    fChain->SetBranchAddress("EEmcPrs.mAdc", &EEmcPrs_mAdc, &b_EEmcPrs_mAdc);
//    fChain->SetBranchAddress("EEmcPrs.mCalType", &EEmcPrs_mCalType, &b_EEmcPrs_mCalType);
//    fChain->SetBranchAddress("EEmcSmdu", &EEmcSmdu_, &b_EEmcSmdu_);
//    fChain->SetBranchAddress("EEmcSmdu.mEnergy", &EEmcSmdu_mEnergy, &b_EEmcSmdu_mEnergy);
//    fChain->SetBranchAddress("EEmcSmdu.mId", &EEmcSmdu_mId, &b_EEmcSmdu_mId);
//    fChain->SetBranchAddress("EEmcSmdu.mAdc", &EEmcSmdu_mAdc, &b_EEmcSmdu_mAdc);
//    fChain->SetBranchAddress("EEmcSmdu.mCalType", &EEmcSmdu_mCalType, &b_EEmcSmdu_mCalType);
//    fChain->SetBranchAddress("EEmcSmdv", &EEmcSmdv_, &b_EEmcSmdv_);
//    fChain->SetBranchAddress("EEmcSmdv.mEnergy", &EEmcSmdv_mEnergy, &b_EEmcSmdv_mEnergy);
//    fChain->SetBranchAddress("EEmcSmdv.mId", &EEmcSmdv_mId, &b_EEmcSmdv_mId);
//    fChain->SetBranchAddress("EEmcSmdv.mAdc", &EEmcSmdv_mAdc, &b_EEmcSmdv_mAdc);
//    fChain->SetBranchAddress("EEmcSmdv.mCalType", &EEmcSmdv_mCalType, &b_EEmcSmdv_mCalType);
//    fChain->SetBranchAddress("PmdHit", &PmdHit_, &b_PmdHit_);
//    fChain->SetBranchAddress("PmdHit.mSuperModule", &PmdHit_mSuperModule, &b_PmdHit_mSuperModule);
//    fChain->SetBranchAddress("PmdHit.mSubDetector", &PmdHit_mSubDetector, &b_PmdHit_mSubDetector);
//    fChain->SetBranchAddress("PmdHit.mRow", &PmdHit_mRow, &b_PmdHit_mRow);
//    fChain->SetBranchAddress("PmdHit.mCol", &PmdHit_mCol, &b_PmdHit_mCol);
//    fChain->SetBranchAddress("PmdHit.mEnergy", &PmdHit_mEnergy, &b_PmdHit_mEnergy);
//    fChain->SetBranchAddress("PmdHit.mADC", &PmdHit_mADC, &b_PmdHit_mADC);
//    fChain->SetBranchAddress("CpvHit", &CpvHit_, &b_CpvHit_);
//    fChain->SetBranchAddress("CpvHit.mSuperModule", &CpvHit_mSuperModule, &b_CpvHit_mSuperModule);
//    fChain->SetBranchAddress("CpvHit.mSubDetector", &CpvHit_mSubDetector, &b_CpvHit_mSubDetector);
//    fChain->SetBranchAddress("CpvHit.mRow", &CpvHit_mRow, &b_CpvHit_mRow);
//    fChain->SetBranchAddress("CpvHit.mCol", &CpvHit_mCol, &b_CpvHit_mCol);
//    fChain->SetBranchAddress("CpvHit.mEnergy", &CpvHit_mEnergy, &b_CpvHit_mEnergy);
//    fChain->SetBranchAddress("CpvHit.mADC", &CpvHit_mADC, &b_CpvHit_mADC);
//    fChain->SetBranchAddress("PmdCluster", &PmdCluster_, &b_PmdCluster_);
//    fChain->SetBranchAddress("PmdCluster.mSuperModule", &PmdCluster_mSuperModule, &b_PmdCluster_mSuperModule);
//    fChain->SetBranchAddress("PmdCluster.mNcell", &PmdCluster_mNcell, &b_PmdCluster_mNcell);
//    fChain->SetBranchAddress("PmdCluster.mEta", &PmdCluster_mEta, &b_PmdCluster_mEta);
//    fChain->SetBranchAddress("PmdCluster.mPhi", &PmdCluster_mPhi, &b_PmdCluster_mPhi);
//    fChain->SetBranchAddress("PmdCluster.mSigma", &PmdCluster_mSigma, &b_PmdCluster_mSigma);
//    fChain->SetBranchAddress("PmdCluster.mEnergy", &PmdCluster_mEnergy, &b_PmdCluster_mEnergy);
//    fChain->SetBranchAddress("PmdCluster.mEnergyPID", &PmdCluster_mEnergyPID, &b_PmdCluster_mEnergyPID);
//    fChain->SetBranchAddress("PmdCluster.mPID", &PmdCluster_mPID, &b_PmdCluster_mPID);
//    fChain->SetBranchAddress("PmdCluster.mMcPID", &PmdCluster_mMcPID, &b_PmdCluster_mMcPID);
//    fChain->SetBranchAddress("CpvCluster", &CpvCluster_, &b_CpvCluster_);
//    fChain->SetBranchAddress("CpvCluster.mSuperModule", &CpvCluster_mSuperModule, &b_CpvCluster_mSuperModule);
//    fChain->SetBranchAddress("CpvCluster.mNcell", &CpvCluster_mNcell, &b_CpvCluster_mNcell);
//    fChain->SetBranchAddress("CpvCluster.mEta", &CpvCluster_mEta, &b_CpvCluster_mEta);
//    fChain->SetBranchAddress("CpvCluster.mPhi", &CpvCluster_mPhi, &b_CpvCluster_mPhi);
//    fChain->SetBranchAddress("CpvCluster.mSigma", &CpvCluster_mSigma, &b_CpvCluster_mSigma);
//    fChain->SetBranchAddress("CpvCluster.mEnergy", &CpvCluster_mEnergy, &b_CpvCluster_mEnergy);
//    fChain->SetBranchAddress("CpvCluster.mEnergyPID", &CpvCluster_mEnergyPID, &b_CpvCluster_mEnergyPID);
//    fChain->SetBranchAddress("CpvCluster.mPID", &CpvCluster_mPID, &b_CpvCluster_mPID);
//    fChain->SetBranchAddress("CpvCluster.mMcPID", &CpvCluster_mMcPID, &b_CpvCluster_mMcPID);
//    fChain->SetBranchAddress("FmsHit", &FmsHit_, &b_FmsHit_);
//    fChain->SetBranchAddress("FmsHit.mDetectorId", &FmsHit_mDetectorId, &b_FmsHit_mDetectorId);
//    fChain->SetBranchAddress("FmsHit.mChannel", &FmsHit_mChannel, &b_FmsHit_mChannel);
//    fChain->SetBranchAddress("FmsHit.mQTCrtSlotCh", &FmsHit_mQTCrtSlotCh, &b_FmsHit_mQTCrtSlotCh);
//    fChain->SetBranchAddress("FmsHit.mAdc", &FmsHit_mAdc, &b_FmsHit_mAdc);
//    fChain->SetBranchAddress("FmsHit.mTdc", &FmsHit_mTdc, &b_FmsHit_mTdc);
//    fChain->SetBranchAddress("FmsHit.mEnergy", &FmsHit_mEnergy, &b_FmsHit_mEnergy);
//    fChain->SetBranchAddress("TofHit", &TofHit_, &b_TofHit_);
//    fChain->SetBranchAddress("TofHit.mIconf", &TofHit_mIconf, &b_TofHit_mIconf);
//    fChain->SetBranchAddress("TofHit.mTrayIndex", &TofHit_mTrayIndex, &b_TofHit_mTrayIndex);
//    fChain->SetBranchAddress("TofHit.mModuleIndex", &TofHit_mModuleIndex, &b_TofHit_mModuleIndex);
//    fChain->SetBranchAddress("TofHit.mCellIndex", &TofHit_mCellIndex, &b_TofHit_mCellIndex);
//    fChain->SetBranchAddress("TofHit.mDaqIndex", &TofHit_mDaqIndex, &b_TofHit_mDaqIndex);
//    fChain->SetBranchAddress("TofHit.mADC", &TofHit_mADC, &b_TofHit_mADC);
//    fChain->SetBranchAddress("TofHit.mTimeOfFlight", &TofHit_mTimeOfFlight, &b_TofHit_mTimeOfFlight);
//    fChain->SetBranchAddress("TofHit.mPathLength", &TofHit_mPathLength, &b_TofHit_mPathLength);
//    fChain->SetBranchAddress("TofHit.mBeta", &TofHit_mBeta, &b_TofHit_mBeta);
//    fChain->SetBranchAddress("TofHit.mAssociatedTrackId", &TofHit_mAssociatedTrackId, &b_TofHit_mAssociatedTrackId);
//    fChain->SetBranchAddress("TofHit.mProjectedPoint.mX1", &TofHit_mProjectedPoint_mX1, &b_TofHit_mProjectedPoint_mX1);
//    fChain->SetBranchAddress("TofHit.mProjectedPoint.mX2", &TofHit_mProjectedPoint_mX2, &b_TofHit_mProjectedPoint_mX2);
//    fChain->SetBranchAddress("TofHit.mProjectedPoint.mX3", &TofHit_mProjectedPoint_mX3, &b_TofHit_mProjectedPoint_mX3);
//    fChain->SetBranchAddress("TofHit.mTOFExpectedAsElectron", &TofHit_mTOFExpectedAsElectron, &b_TofHit_mTOFExpectedAsElectron);
//    fChain->SetBranchAddress("TofHit.mTOFExpectedAsPion", &TofHit_mTOFExpectedAsPion, &b_TofHit_mTOFExpectedAsPion);
//    fChain->SetBranchAddress("TofHit.mTOFExpectedAsKaon", &TofHit_mTOFExpectedAsKaon, &b_TofHit_mTOFExpectedAsKaon);
//    fChain->SetBranchAddress("TofHit.mTOFExpectedAsProton", &TofHit_mTOFExpectedAsProton, &b_TofHit_mTOFExpectedAsProton);
//    fChain->SetBranchAddress("TofHit.mSigmaElectron", &TofHit_mSigmaElectron, &b_TofHit_mSigmaElectron);
//    fChain->SetBranchAddress("TofHit.mSigmaPion", &TofHit_mSigmaPion, &b_TofHit_mSigmaPion);
//    fChain->SetBranchAddress("TofHit.mSigmaKaon", &TofHit_mSigmaKaon, &b_TofHit_mSigmaKaon);
//    fChain->SetBranchAddress("TofHit.mSigmaProton", &TofHit_mSigmaProton, &b_TofHit_mSigmaProton);
//    fChain->SetBranchAddress("TofHit.mParticleHypothesis", &TofHit_mParticleHypothesis, &b_TofHit_mParticleHypothesis);
//    fChain->SetBranchAddress("TofData", &TofData_, &b_TofData_);
//    fChain->SetBranchAddress("TofData.mDataIndex", &TofData_mDataIndex, &b_TofData_mDataIndex);
//    fChain->SetBranchAddress("TofData.mAdc", &TofData_mAdc, &b_TofData_mAdc);
//    fChain->SetBranchAddress("TofData.mTdc", &TofData_mTdc, &b_TofData_mTdc);
//    fChain->SetBranchAddress("TofData.mTc", &TofData_mTc, &b_TofData_mTc);
//    fChain->SetBranchAddress("TofData.mSc", &TofData_mSc, &b_TofData_mSc);
//    fChain->SetBranchAddress("TofData.mLeadingTdc", &TofData_mLeadingTdc, &b_TofData_mLeadingTdc);
//    fChain->SetBranchAddress("TofData.mTrailingTdc", &TofData_mTrailingTdc, &b_TofData_mTrailingTdc);
//    fChain->SetBranchAddress("TofRawData", &TofRawData_, &b_TofRawData_);
//    fChain->SetBranchAddress("TofRawData.mLeTeFlag", &TofRawData_mLeTeFlag, &b_TofRawData_mLeTeFlag);
//    fChain->SetBranchAddress("TofRawData.mTray", &TofRawData_mTray, &b_TofRawData_mTray);
//    fChain->SetBranchAddress("TofRawData.mChannel", &TofRawData_mChannel, &b_TofRawData_mChannel);
//    fChain->SetBranchAddress("TofRawData.mTdc", &TofRawData_mTdc, &b_TofRawData_mTdc);
//    fChain->SetBranchAddress("TofRawData.mTriggertime", &TofRawData_mTriggertime, &b_TofRawData_mTriggertime);
//    fChain->SetBranchAddress("TofRawData.mQuality", &TofRawData_mQuality, &b_TofRawData_mQuality);
   fChain->SetBranchAddress("BTofHit", &BTofHit_, &b_BTofHit_);
   fChain->SetBranchAddress("BTofHit.mTray", BTofHit_mTray, &b_BTofHit_mTray);
   fChain->SetBranchAddress("BTofHit.mModule", BTofHit_mModule, &b_BTofHit_mModule);
   fChain->SetBranchAddress("BTofHit.mCell", BTofHit_mCell, &b_BTofHit_mCell);
   fChain->SetBranchAddress("BTofHit.mLeadingEdgeTime", BTofHit_mLeadingEdgeTime, &b_BTofHit_mLeadingEdgeTime);
   fChain->SetBranchAddress("BTofHit.mTrailingEdgeTime", BTofHit_mTrailingEdgeTime, &b_BTofHit_mTrailingEdgeTime);
   fChain->SetBranchAddress("BTofHit.mAssociatedTrackId", BTofHit_mAssociatedTrackId, &b_BTofHit_mAssociatedTrackId);
   fChain->SetBranchAddress("BTofHit.mIndex2Primary", BTofHit_mIndex2Primary, &b_BTofHit_mIndex2Primary);
   fChain->SetBranchAddress("BTofHit.mIndex2Global", BTofHit_mIndex2Global, &b_BTofHit_mIndex2Global);
   fChain->SetBranchAddress("BTofHit.mIdTruth", BTofHit_mIdTruth, &b_BTofHit_mIdTruth);
   fChain->SetBranchAddress("BTofHit.mQuality", BTofHit_mQuality, &b_BTofHit_mQuality);
   fChain->SetBranchAddress("BTofRawHit", &BTofRawHit_, &b_BTofRawHit_);
   fChain->SetBranchAddress("BTofRawHit.mFlag", BTofRawHit_mFlag, &b_BTofRawHit_mFlag);
   fChain->SetBranchAddress("BTofRawHit.mTray", BTofRawHit_mTray, &b_BTofRawHit_mTray);
   fChain->SetBranchAddress("BTofRawHit.mChannel", BTofRawHit_mChannel, &b_BTofRawHit_mChannel);
   fChain->SetBranchAddress("BTofRawHit.mTdc", BTofRawHit_mTdc, &b_BTofRawHit_mTdc);
   fChain->SetBranchAddress("BTofHeader", &BTofHeader_, &b_BTofHeader_);
   fChain->SetBranchAddress("BTofHeader.mFiberHeader[4]", BTofHeader_mFiberHeader, &b_BTofHeader_mFiberHeader);
   fChain->SetBranchAddress("BTofHeader.mFiberTriggerWord[4]", BTofHeader_mFiberTriggerWord, &b_BTofHeader_mFiberTriggerWord);
   fChain->SetBranchAddress("BTofHeader.mVpdHitPattern[2]", BTofHeader_mVpdHitPattern, &b_BTofHeader_mVpdHitPattern);
   fChain->SetBranchAddress("BTofHeader.mVpdVz[20]", BTofHeader_mVpdVz, &b_BTofHeader_mVpdVz);
   fChain->SetBranchAddress("BTofHeader.mTStart", BTofHeader_mTStart, &b_BTofHeader_mTStart);
   fChain->SetBranchAddress("BTofHeader.mTStartErr", BTofHeader_mTStartErr, &b_BTofHeader_mTStartErr);
   fChain->SetBranchAddress("BTofHeader.mTDiff", BTofHeader_mTDiff, &b_BTofHeader_mTDiff);
   fChain->SetBranchAddress("BTofHeader.mVpdTime[2][19]", BTofHeader_mVpdTime, &b_BTofHeader_mVpdTime);
   fChain->SetBranchAddress("BTofHeader.mTriggerTime[4]", BTofHeader_mTriggerTime, &b_BTofHeader_mTriggerTime);
   fChain->SetBranchAddress("BTofHeader.mNTzero", BTofHeader_mNTzero, &b_BTofHeader_mNTzero);
//    fChain->SetBranchAddress("MTDHit", &MTDHit_, &b_MTDHit_);
//    fChain->SetBranchAddress("MTDHit.fUniqueID", &MTDHit_fUniqueID, &b_MTDHit_fUniqueID);
//    fChain->SetBranchAddress("MTDHit.fBits", &MTDHit_fBits, &b_MTDHit_fBits);
//    fChain->SetBranchAddress("MTDHit.mBackLeg", &MTDHit_mBackLeg, &b_MTDHit_mBackLeg);
//    fChain->SetBranchAddress("MTDHit.mModule", &MTDHit_mModule, &b_MTDHit_mModule);
//    fChain->SetBranchAddress("MTDHit.mCell", &MTDHit_mCell, &b_MTDHit_mCell);
//    fChain->SetBranchAddress("MTDHit.mLeadingEdgeTime.first", &MTDHit_mLeadingEdgeTime_first, &b_MTDHit_mLeadingEdgeTime_first);
//    fChain->SetBranchAddress("MTDHit.mLeadingEdgeTime.second", &MTDHit_mLeadingEdgeTime_second, &b_MTDHit_mLeadingEdgeTime_second);
//    fChain->SetBranchAddress("MTDHit.mTrailingEdgeTime.first", &MTDHit_mTrailingEdgeTime_first, &b_MTDHit_mTrailingEdgeTime_first);
//    fChain->SetBranchAddress("MTDHit.mTrailingEdgeTime.second", &MTDHit_mTrailingEdgeTime_second, &b_MTDHit_mTrailingEdgeTime_second);
//    fChain->SetBranchAddress("MTDHit.mIdTruth", &MTDHit_mIdTruth, &b_MTDHit_mIdTruth);
//    fChain->SetBranchAddress("MTDHit.mQuality", &MTDHit_mQuality, &b_MTDHit_mQuality);
//    fChain->SetBranchAddress("MTDHit.mTrackKey", &MTDHit_mTrackKey, &b_MTDHit_mTrackKey);
//    fChain->SetBranchAddress("MTDRawHit", &MTDRawHit_, &b_MTDRawHit_);
//    fChain->SetBranchAddress("MTDRawHit.fUniqueID", &MTDRawHit_fUniqueID, &b_MTDRawHit_fUniqueID);
//    fChain->SetBranchAddress("MTDRawHit.fBits", &MTDRawHit_fBits, &b_MTDRawHit_fBits);
//    fChain->SetBranchAddress("MTDRawHit.mFlag", &MTDRawHit_mFlag, &b_MTDRawHit_mFlag);
//    fChain->SetBranchAddress("MTDRawHit.mBackLeg", &MTDRawHit_mBackLeg, &b_MTDRawHit_mBackLeg);
//    fChain->SetBranchAddress("MTDRawHit.mChannel", &MTDRawHit_mChannel, &b_MTDRawHit_mChannel);
//    fChain->SetBranchAddress("MTDRawHit.mTdc", &MTDRawHit_mTdc, &b_MTDRawHit_mTdc);
//    fChain->SetBranchAddress("MTDHeader", &MTDHeader_, &b_MTDHeader_);
//    fChain->SetBranchAddress("MTDHeader.fUniqueID", MTDHeader_fUniqueID, &b_MTDHeader_fUniqueID);
//    fChain->SetBranchAddress("MTDHeader.fBits", MTDHeader_fBits, &b_MTDHeader_fBits);
//    fChain->SetBranchAddress("MTDHeader.mFiberHeader1", MTDHeader_mFiberHeader1, &b_MTDHeader_mFiberHeader1);
//    fChain->SetBranchAddress("MTDHeader.mFiberHeader2", MTDHeader_mFiberHeader2, &b_MTDHeader_mFiberHeader2);
//    fChain->SetBranchAddress("MTDHeader.mFiberTriggerWord1", MTDHeader_mFiberTriggerWord1, &b_MTDHeader_mFiberTriggerWord1);
//    fChain->SetBranchAddress("MTDHeader.mFiberTriggerWord2", MTDHeader_mFiberTriggerWord2, &b_MTDHeader_mFiberTriggerWord2);
//    fChain->SetBranchAddress("MTDHeader.mTriggerTime1", MTDHeader_mTriggerTime1, &b_MTDHeader_mTriggerTime1);
//    fChain->SetBranchAddress("MTDHeader.mTriggerTime2", MTDHeader_mTriggerTime2, &b_MTDHeader_mTriggerTime2);
//    fChain->SetBranchAddress("FgtStrip", &FgtStrip_, &b_FgtStrip_);
//    fChain->SetBranchAddress("FgtStrip.mGeoId", &FgtStrip_mGeoId, &b_FgtStrip_mGeoId);
//    fChain->SetBranchAddress("FgtStrip.mAdcStartIdx", &FgtStrip_mAdcStartIdx, &b_FgtStrip_mAdcStartIdx);
//    fChain->SetBranchAddress("FgtStrip.mNumSavedTimeBins", &FgtStrip_mNumSavedTimeBins, &b_FgtStrip_mNumSavedTimeBins);
//    fChain->SetBranchAddress("FgtStrip.mClusterSeedType", &FgtStrip_mClusterSeedType, &b_FgtStrip_mClusterSeedType);
//    fChain->SetBranchAddress("FgtStrip.mCharge", &FgtStrip_mCharge, &b_FgtStrip_mCharge);
//    fChain->SetBranchAddress("FgtStrip.mChargeUncert", &FgtStrip_mChargeUncert, &b_FgtStrip_mChargeUncert);
//    fChain->SetBranchAddress("FgtCluster", &FgtCluster_, &b_FgtCluster_);
//    fChain->SetBranchAddress("FgtCluster.mCentralStripGeoId", &FgtCluster_mCentralStripGeoId, &b_FgtCluster_mCentralStripGeoId);
//    fChain->SetBranchAddress("FgtCluster.mFirstStripAssociationIdx", &FgtCluster_mFirstStripAssociationIdx, &b_FgtCluster_mFirstStripAssociationIdx);
//    fChain->SetBranchAddress("FgtCluster.mMaxTimeBin", &FgtCluster_mMaxTimeBin, &b_FgtCluster_mMaxTimeBin);
//    fChain->SetBranchAddress("FgtCluster.mMaxAdc", &FgtCluster_mMaxAdc, &b_FgtCluster_mMaxAdc);
//    fChain->SetBranchAddress("FgtCluster.mNumStrips", &FgtCluster_mNumStrips, &b_FgtCluster_mNumStrips);
//    fChain->SetBranchAddress("FgtCluster.mCharge", &FgtCluster_mCharge, &b_FgtCluster_mCharge);
//    fChain->SetBranchAddress("FgtCluster.mChargeUncert", &FgtCluster_mChargeUncert, &b_FgtCluster_mChargeUncert);
//    fChain->SetBranchAddress("FgtCluster.mR", &FgtCluster_mR, &b_FgtCluster_mR);
//    fChain->SetBranchAddress("FgtCluster.mErrR", &FgtCluster_mErrR, &b_FgtCluster_mErrR);
//    fChain->SetBranchAddress("FgtCluster.mPhi", &FgtCluster_mPhi, &b_FgtCluster_mPhi);
//    fChain->SetBranchAddress("FgtCluster.mErrPhi", &FgtCluster_mErrPhi, &b_FgtCluster_mErrPhi);
//    fChain->SetBranchAddress("FgtCluster.mEvenOddChargeAsy", &FgtCluster_mEvenOddChargeAsy, &b_FgtCluster_mEvenOddChargeAsy);
//    fChain->SetBranchAddress("FgtStripAssociation", &FgtStripAssociation_, &b_FgtStripAssociation_);
//    fChain->SetBranchAddress("FgtStripAssociation.mClusIdx", &FgtStripAssociation_mClusIdx, &b_FgtStripAssociation_mClusIdx);
//    fChain->SetBranchAddress("FgtStripAssociation.mStripIdx", &FgtStripAssociation_mStripIdx, &b_FgtStripAssociation_mStripIdx);
//    fChain->SetBranchAddress("FgtStripAssociation.mWeight", &FgtStripAssociation_mWeight, &b_FgtStripAssociation_mWeight);
//    fChain->SetBranchAddress("FgtAdc", &FgtAdc_, &b_FgtAdc_);
//    fChain->SetBranchAddress("FgtAdc.mAdc", &FgtAdc_mAdc, &b_FgtAdc_mAdc);
//    fChain->SetBranchAddress("FgtAdc.mTimeBin", &FgtAdc_mTimeBin, &b_FgtAdc_mTimeBin);
   Notify();
   
}

Bool_t muapp::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

	if (fChain&&fCurrent>=0){
		cout<<"loading... "<<fCurrent+1<<" of "<<nFiles<<"\t "<<fChain->GetCurrentFile()->GetName()<<endl;
	}
	return kTRUE;
}

void muapp::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t muapp::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
