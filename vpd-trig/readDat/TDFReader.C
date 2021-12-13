#include <vector.h>

void TDFReader(const char *file="trgdata/run18160030.1.dat", TString outFilename="test.root" ,int nEvents=99999999)
{
	cout << "Reading "<<file<<" up to "<<nEvents<<" events\n";

    gROOT->Macro("Load.C");
	gSystem->Load("StIOMaker");
	gSystem->Load("StTriggerDataMaker");
	gSystem->Load("StMTDTrgMaker");
	cout << "Setting up chain" << endl;

	StChain* chain = new StChain;

	StIOMaker* iomaker = new StIOMaker("IO","r", file);
	StTriggerDataMaker* trgmaker = new StTriggerDataMaker();
	trgmaker->setDebug(0);

	vector<ULong64_t> triggerids; 
	triggerids.clear();
	//triggerids.push_back(0x8);
	//triggerids.push_back(0x200000);

	StMTDTrgMaker* mtdtrgmaker = new StMTDTrgMaker();
	mtdtrgmaker->setOutFileName(outFilename);
	mtdtrgmaker->setTriggerIDs(triggerids);

	cout << "Init Chain" << endl;
	chain->Init();

	cout << "Event Loop  nEvents=" << nEvents << endl;
	//chain->EventLoop(0,nEvents);
	for(Int_t i=0; i<nEvents; i++){
		chain->Clear();
		Int_t iret = chain->Make(i);
		if(iret){
			cout<<"Bad return code!"<<endl;
			break;
		}
	}

	cout << "Finish Chain" << endl;
	chain->Finish();
	delete chain;
}
