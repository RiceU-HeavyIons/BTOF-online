#include "/star/u/syang/Macro/headers.h"
#include "/star/u/syang/Macro/function.C"
#include "/star/u/syang/run18/mtdTrig/common/mtdTrigFun.C"

#include "trgEvent.h"

const int prepost=0; // you can select which bunchcrossing you want to display or to analysis; 0-current bunchcrossing ; negative Value -> pre bunchcrossing; positive Value -> post bunchcrossing

const int tacRef = 0;

Bool_t vpdslewingcorrFlag = kFALSE;

Bool_t alignEWVPD = kTRUE;
//Bool_t alignEWVPD = kFALSE;

const unsigned short bbq_vpdcutLo[2]  = {300,  300};
const unsigned short bbq_vpdcutHi[2]  = {3000, 3000};
const unsigned short bbq_vpdadccut[2] = {10,   10};

const unsigned short mxq_vpdcutLo[2]  = {300,  300};
const unsigned short mxq_vpdcutHi[2]  = {3500, 3500};
const unsigned short mxq_vpdadccut[2] = {10,   10};

char buf[500];
//********************** function ***********************
void initialHistograms();
void writeHistograms(char *outFile);

//************************* initial histogram **************************
TH1D *hBBQVPD_RawADC[2][16];
TH1D *hBBQVPD_RawTAC[2][16];
TH1D *hBBQVPD_ADC[2][16];
TH1D *hBBQVPD_TAC[2][16];
TH1D *hBBQVPD_TACDiff[2][16];
TH1D *hMXQVPD_RawADC[2][16];
TH1D *hMXQVPD_RawTAC[2][16];
TH1D *hMXQVPD_ADC[2][16];
TH1D *hMXQVPD_TAC[2][16];
TH1D *hMXQVPD_TACDiff[2][16];

int main(int argc, char **argv)
{
	//************************ read data files ********************
	if(argc!=3 && argc!=1) {
		cout<<"Oops! The amount of Parameter is wrong!"<<endl;
		return 0;
	}

	TString FileInput;
	char FileOutput[1000];

	if(argc==1){
		FileInput  = "test.list";
		sprintf(FileOutput,"test.root");
	}

	if(argc==3){
		FileInput = argv[1];
		sprintf(FileOutput,"%s.root",argv[2]);
	} 

	int fileNumber = 0;

	char FileList[512];

	//----------------------------------
	// Open files and add to the chain
	//----------------------------------
	TChain *chain = new TChain("trgEvent");

	ifstream* inputStream = new ifstream;
	inputStream->open(FileInput.Data());
	if (!(inputStream)) {
		printf("can not open list file\n");
		return 0;
	}
	for (;inputStream->good();) {
		inputStream->getline(FileList,512);
		if  ( inputStream->good() ) {
			TFile *ftmp = new TFile(FileList);
			if(!ftmp||!(ftmp->IsOpen())||!(ftmp->GetNkeys())) {
				printf(" file %s error in opening!!!\n",FileList);
			} else {
				printf(" read in file %s\n",FileList);
				chain->Add(FileList);
				fileNumber++;
			}
			delete ftmp;
		}
	}

	printf("%d files read in\n",fileNumber);
	delete inputStream;

	// book histograms
	initialHistograms();

	//load bbq vpd slewing parametes
	Short_t bbqvpdadc[2][32][9]; //0-east;1-west
	memset(bbqvpdadc,0,sizeof(bbqvpdadc));
	Short_t bbqvpdtac[2][32][9];
	memset(bbqvpdtac,0,sizeof(bbqvpdtac));
	if(vpdslewingcorrFlag){
		bool loadStatus=kTRUE;
		loadStatus = loadSlewPars("bbqvpd","/star/u/syang/run18/mtdTrig/common/vpd_slew_corr.txt",bbqvpdadc,bbqvpdtac);
		if(!loadStatus){
			cout<<"CAN NOT open the BBQ VPD slewing parameters file!!!"<<endl;
			return -1;
		}
	}

	//load mxq vpd slewing parametes
	Short_t mxqvpdadc[2][32][9]; //0-east;1-west
	memset(mxqvpdadc,0,sizeof(mxqvpdadc));
	Short_t mxqvpdtac[2][32][9];
	memset(mxqvpdtac,0,sizeof(mxqvpdtac));
	if(vpdslewingcorrFlag){
		bool loadStatus=kTRUE;
		loadStatus = loadSlewPars("mxqvpd","/star/u/syang/run18/mtdTrig/common/mvpd_slew_corr.txt",mxqvpdadc,mxqvpdtac);
		if(!loadStatus){
			cout<<"CAN NOT open the MXQ VPD slewing parameters file!!!"<<endl;
			return -1;
		}
	}

	// variables 
	UShort_t bbq_vpd[2][32]; // vpd information read from BBQ crate.  0-east, 1-west
	memset(bbq_vpd,0,sizeof(bbq_vpd));

	UShort_t mxq_vpd[2][32]; // vpd information read from MXQ crate.  0-east, 1-west
	memset(mxq_vpd,0,sizeof(mxq_vpd));

	//--------------------
	// loop the chain
	//--------------------
	int nevents = (int)chain->GetEntries();
	cout << "== total entries : " << nevents << endl;

	trgEvent *t = new trgEvent(chain);

	for (int evt=0; evt<nevents; evt++) {

		if(evt%(nevents/10)==0) cout<<"Proceeding "<<evt<<"-th event"<<endl;

		t->GetEntry(evt);
		//cout<<"YY"<<endl;

		Int_t npre = t->npre;
		//Int_t npost = t->npost;
		//cout<<"npre:"<<npre<<endl;
		//cout<<"npost:"<<npost<<endl;

		memset(bbq_vpd,0,sizeof(bbq_vpd));
		memset(mxq_vpd,0,sizeof(mxq_vpd));
		for (int i=0; i<2; i++) {
			for (int j=0; j<32; j++) {
				if (i==0) {
					bbq_vpd[i][j] = t->bbqEastVpd[prepost+npre][j];
					mxq_vpd[i][j] = t->mxqEastVpd[prepost+npre][j];
				}
				else if (i==1) {
					bbq_vpd[i][j] = t->bbqWestVpd[prepost+npre][j];
					mxq_vpd[i][j] = t->mxqWestVpd[prepost+npre][j];
				}

				Int_t idx = j%4 + j/8*4;

				if (j/4%2==0) {
					hBBQVPD_RawADC[i][idx]->Fill(bbq_vpd[i][j]);
					hMXQVPD_RawADC[i][idx]->Fill(mxq_vpd[i][j]);
				}
				else {
					hBBQVPD_RawTAC[i][idx]->Fill(bbq_vpd[i][j]);
					hMXQVPD_RawTAC[i][idx]->Fill(mxq_vpd[i][j]);
				}
			}
		} 
		if (vpdslewingcorrFlag) {
			implementSlewPars(2,bbq_vpd,bbqvpdadc,bbqvpdtac);
			implementSlewPars(2,mxq_vpd,mxqvpdadc,mxqvpdtac);
		}

		Int_t adcRefIdx = tacRef + tacRef/4*4;
		Int_t tacRefIdx = tacRef + (tacRef/4+1)*4;
		for (Int_t i=0; i<2; i++) {
			for (Int_t j=0; j<16; j++) {
				Int_t adcIdx = j + j/4*4;
				Int_t tacIdx = j + (j/4+1)*4;

				if(
						bbq_vpd[i][adcRefIdx]>bbq_vpdadccut[i] &&
						bbq_vpd[i][tacRefIdx]>bbq_vpdcutLo[i]  &&
						bbq_vpd[i][tacRefIdx]<bbq_vpdcutHi[i]  &&
						bbq_vpd[i][adcIdx]>bbq_vpdadccut[i]    &&
						bbq_vpd[i][tacIdx]>bbq_vpdcutLo[i]     &&
						bbq_vpd[i][tacIdx]<bbq_vpdcutHi[i]
				  ) {
					hBBQVPD_ADC[i][j]->Fill(bbq_vpd[i][adcIdx]);
					hBBQVPD_TAC[i][j]->Fill(bbq_vpd[i][tacIdx]);
					hBBQVPD_TACDiff[i][j]->Fill(bbq_vpd[i][tacIdx]-bbq_vpd[i][tacRefIdx]);
				}

				if(
						mxq_vpd[i][adcRefIdx]>mxq_vpdadccut[i] &&
						mxq_vpd[i][tacRefIdx]>mxq_vpdcutLo[i]  &&
						mxq_vpd[i][tacRefIdx]<mxq_vpdcutHi[i]  &&
						//( mxq_vpd[i][adcIdx]>mxq_vpdadccut[i]  && !(i==1 && j==6) )  &&
						mxq_vpd[i][adcIdx]>mxq_vpdadccut[i]    &&
						mxq_vpd[i][tacIdx]>mxq_vpdcutLo[i]     &&
						mxq_vpd[i][tacIdx]<mxq_vpdcutHi[i]
				  ) {
					hMXQVPD_ADC[i][j]->Fill(mxq_vpd[i][adcIdx]);
					hMXQVPD_TAC[i][j]->Fill(mxq_vpd[i][tacIdx]);
					hMXQVPD_TACDiff[i][j]->Fill(mxq_vpd[i][tacIdx]-mxq_vpd[i][tacRefIdx]);
				}

			}
		}
	}

	Int_t bbqPeak[2][16];
	Int_t bbqOffset[2][16];
	memset(bbqPeak, 0, sizeof(bbqPeak));
	memset(bbqOffset, 0, sizeof(bbqOffset));
	Int_t bbqOffsetMin[2];
	Int_t bbqOffsetMinIdx[2];

	Int_t mxqPeak[2][16];
	Int_t mxqOffset[2][16];
	memset(mxqPeak, 0, sizeof(mxqPeak));
	memset(mxqOffset, 0, sizeof(mxqOffset));
	Int_t mxqOffsetMin[2];
	Int_t mxqOffsetMinIdx[2];

	gStyle->SetOptStat(1110);
	//gStyle->SetOptStat(0);
	gStyle->SetOptFit(1111);
	//gStyle->SetOptDate(0);
	//gStyle->SetPalette(1,0);
	//gStyle->SetTitleOffset(1.6,"Y");

	TF1 *g = new TF1("g", "gaus", -2000, 2000);
	g->SetLineColor(2);
	g->SetLineWidth(2);

	TCanvas *c1 = new TCanvas("c1","c1",1000,1000);

	TPDF *ps1;
	TPDF *ps2;
	if(argc==1) {
		ps1 = new TPDF("bbq_test.pdf", 111);
		ps2 = new TPDF("mxq_test.pdf", 111);
	}
	else if (argc==3) {
		ps1 = new TPDF(Form("bbq_%s.pdf", argv[2]), 111);
		ps2 = new TPDF(Form("mxq_%s.pdf", argv[2]), 111);
	}
	ps1->Off();
	ps2->Off();

	int nColumns = 4;
	int nRows = 4;
	int nPads = nColumns*nRows;
	c1->Divide(nColumns,nRows);

	for(int i=0;i<nPads;i++){
		c1->cd(i+1);
		gPad->SetLogy(1);
		setPad(0.14,0.06,0.08,0.12);
	}

	Int_t rebX = 4;

	// bbq vpd calibarion
	Int_t psIdx=0;
	for (Int_t i=0; i<2; i++) {
		for (Int_t j=0; j<16; j++) {
			c1->cd(psIdx%nPads+1);
			hBBQVPD_RawADC[i][j]->RebinX(rebX);
			hBBQVPD_RawADC[i][j]->Draw();
			psIdx++;

			c1->cd(psIdx%nPads+1);
			hBBQVPD_RawTAC[i][j]->RebinX(rebX);
			hBBQVPD_RawTAC[i][j]->Draw();
			psIdx++;

			if(psIdx%nPads==0) pdfAction(c1, ps1);
		}
		if(psIdx%nPads!=0) pdfAction(c1, ps1);
	}

	psIdx=0;
	for (Int_t i=0; i<2; i++) {
		for (Int_t j=0; j<16; j++) {
			c1->cd(psIdx%nPads+1);
			hBBQVPD_ADC[i][j]->RebinX(rebX);
			hBBQVPD_ADC[i][j]->Draw();
			psIdx++;

			c1->cd(psIdx%nPads+1);
			if (hBBQVPD_TAC[i][j]->GetEntries()==0) { // dead channel
				hBBQVPD_TAC[i][j]->Draw();
				bbqPeak[i][j] = 4096;
			}
			else {
				Double_t mean  = hBBQVPD_TAC[i][j]->GetMean();
				Double_t sigma = hBBQVPD_TAC[i][j]->GetRMS();
				hBBQVPD_TAC[i][j]->RebinX(rebX);
				hBBQVPD_TAC[i][j]->Fit("g", "RQ", "", mean-5*sigma, mean+5*sigma);
				hBBQVPD_TAC[i][j]->Draw("same");
				bbqPeak[i][j] = g->GetParameter(1);
			}
			psIdx++;

			if(psIdx%nPads==0) pdfAction(c1, ps1);
		}
		if(psIdx%nPads!=0) pdfAction(c1, ps1);
	}

	psIdx=0;
	for (Int_t i=0; i<2; i++) {
		for (Int_t j=0; j<16; j++) {
			c1->cd(psIdx%nPads+1);
			hBBQVPD_TACDiff[i][j]->RebinX(rebX);
			hBBQVPD_TACDiff[i][j]->Draw();
			if (hBBQVPD_TACDiff[i][j]->GetEntries()==0) { // dead channel
				bbqOffset[i][j] = 4096;
			}
			else if (hBBQVPD_TACDiff[i][j]->GetRMS()==0) { // reference channel
				//cout<<"********** RMS=0 **********"<<endl;
				bbqOffset[i][j] = 0;
			}
			else {
				Double_t mean  = hBBQVPD_TACDiff[i][j]->GetMean();
				Double_t sigma = hBBQVPD_TACDiff[i][j]->GetRMS();
				hBBQVPD_TACDiff[i][j]->Fit(g, "RQ", "", mean-5*sigma, mean+5*sigma);
				mean  = g->GetParameter(1);
				sigma = g->GetParameter(2);
				hBBQVPD_TACDiff[i][j]->Fit(g, "RQ", "", mean-3*sigma, mean+3*sigma);
				hBBQVPD_TACDiff[i][j]->Draw("same");
				bbqOffset[i][j] = TMath::Nint(g->GetParameter(1));
			}

			psIdx++;

			if(psIdx%nPads==0) pdfAction(c1, ps1);
		}
		if(psIdx%nPads!=0) pdfAction(c1, ps1);
	}

	for (Int_t i=0; i<2; i++) {
		Int_t min = 99999;
		Int_t idx = 0;
		for (Int_t j=0; j<16; j++) {
			if(min>bbqOffset[i][j]) {
				min=bbqOffset[i][j];
				idx = j;
			}
		}
		bbqOffsetMin[i] = min;
		bbqOffsetMinIdx[i] = idx;

		for (Int_t j=0; j<16; j++) {
			if (bbqOffset[i][j]==4096) {
				//bbqOffset[i][j] = 0;
			}
			else {
				bbqOffset[i][j] += TMath::Abs(bbqOffsetMin[i]);
			}
		}
	}

	if (alignEWVPD) {
		if (bbqPeak[0][bbqOffsetMinIdx[0]] > bbqPeak[1][bbqOffsetMinIdx[1]]) {
			for (Int_t j=0; j<16; j++) {
				if(bbqOffset[0][j]==4096) {
					//bbqOffset[0][j] = 0;
				}
				else {
					bbqOffset[0][j] += (bbqPeak[0][bbqOffsetMinIdx[0]] - bbqPeak[1][bbqOffsetMinIdx[1]]);
				}
			}
		}
		else {
			for (Int_t j=0; j<16; j++) {
				if(bbqOffset[1][j]==4096) {
					//bbqOffset[1][j] = 0;
				}
				else {
					bbqOffset[1][j] += (bbqPeak[1][bbqOffsetMinIdx[1]] - bbqPeak[0][bbqOffsetMinIdx[0]]);
				}
			}
		}
	}
	// bbq vpd calibarion done

	// mxq vpd calibration
	psIdx=0;
	for (Int_t i=0; i<2; i++) {
		for (Int_t j=0; j<16; j++) {
			c1->cd(psIdx%nPads+1);
			hMXQVPD_RawADC[i][j]->RebinX(rebX);
			hMXQVPD_RawADC[i][j]->Draw();
			psIdx++;

			c1->cd(psIdx%nPads+1);
			hMXQVPD_RawTAC[i][j]->RebinX(rebX);
			hMXQVPD_RawTAC[i][j]->Draw();
			psIdx++;

			if(psIdx%nPads==0) pdfAction(c1, ps2);
		}
		if(psIdx%nPads!=0) pdfAction(c1, ps2);
	}

	psIdx=0;
	for (Int_t i=0; i<2; i++) {
		for (Int_t j=0; j<16; j++) {
			c1->cd(psIdx%nPads+1);
			hMXQVPD_ADC[i][j]->RebinX(rebX);
			hMXQVPD_ADC[i][j]->Draw();
			psIdx++;

			c1->cd(psIdx%nPads+1);
			if (hMXQVPD_TAC[i][j]->GetEntries()==0) { // dead channel
				hMXQVPD_TAC[i][j]->Draw();
				mxqPeak[i][j] = 4096;
			}
			else {
				Double_t mean  = hMXQVPD_TAC[i][j]->GetMean();
				Double_t sigma = hMXQVPD_TAC[i][j]->GetRMS();
				hMXQVPD_TAC[i][j]->RebinX(rebX);
				hMXQVPD_TAC[i][j]->Fit("g", "RQ", "", mean-5*sigma, mean+5*sigma);
				hMXQVPD_TAC[i][j]->Draw("same");
				mxqPeak[i][j] = g->GetParameter(1);
			}
			psIdx++;

			if(psIdx%nPads==0) pdfAction(c1, ps2);
		}
		if(psIdx%nPads!=0) pdfAction(c1, ps2);
	}

	psIdx=0;
	for (Int_t i=0; i<2; i++) {
		for (Int_t j=0; j<16; j++) {
			c1->cd(psIdx%nPads+1);
			hMXQVPD_TACDiff[i][j]->RebinX(rebX);
			hMXQVPD_TACDiff[i][j]->Draw();
			if (hMXQVPD_TACDiff[i][j]->GetEntries()==0) { // dead channel
				mxqOffset[i][j] = 4096;
			}
			else if (hMXQVPD_TACDiff[i][j]->GetRMS()==0) { // reference channel
				//cout<<"********** RMS=0 **********"<<endl;
				mxqOffset[i][j] = 0;
			}
			else {
				Double_t mean  = hMXQVPD_TACDiff[i][j]->GetMean();
				Double_t sigma = hMXQVPD_TACDiff[i][j]->GetRMS();
				hMXQVPD_TACDiff[i][j]->Fit(g, "RQ", "", mean-5*sigma, mean+5*sigma);
				mean  = g->GetParameter(1);
				sigma = g->GetParameter(2);
				hMXQVPD_TACDiff[i][j]->Fit(g, "RQ", "", mean-3*sigma, mean+3*sigma);
				hMXQVPD_TACDiff[i][j]->Draw("same");
				mxqOffset[i][j] = TMath::Nint(g->GetParameter(1));
			}

			psIdx++;

			if(psIdx%nPads==0) pdfAction(c1, ps2);
		}
		if(psIdx%nPads!=0) pdfAction(c1, ps2);
	}

	for (Int_t i=0; i<2; i++) {
		Int_t min = 99999;
		Int_t idx = 0;
		for (Int_t j=0; j<16; j++) {
			if(min>mxqOffset[i][j]) {
				min=mxqOffset[i][j];
				idx = j;
			}
		}
		mxqOffsetMin[i] = min;
		mxqOffsetMinIdx[i] = idx;

		for (Int_t j=0; j<16; j++) {
			if (mxqOffset[i][j]==4096) {
				//mxqOffset[i][j] = 0;
			}
			else {
				mxqOffset[i][j] += TMath::Abs(mxqOffsetMin[i]);
			}
		}
	}

	if (alignEWVPD) {
		if (mxqPeak[0][mxqOffsetMinIdx[0]] > mxqPeak[1][mxqOffsetMinIdx[1]]) {
			for (Int_t j=0; j<16; j++) {
				if(mxqOffset[0][j]==4096) {
					//mxqOffset[0][j] = 0;
				}
				else {
					mxqOffset[0][j] += (mxqPeak[0][mxqOffsetMinIdx[0]] - mxqPeak[1][mxqOffsetMinIdx[1]]);
				}
			}
		}
		else {
			for (Int_t j=0; j<16; j++) {
				if(mxqOffset[1][j]==4096) {
					//mxqOffset[1][j] = 0;
				}
				else {
					mxqOffset[1][j] += (mxqPeak[1][mxqOffsetMinIdx[1]] - mxqPeak[0][mxqOffsetMinIdx[0]]);
				}
			}
		}
	}
	// mxq calibration done

	ps1->On();
	ps1->Close();

	ps2->On();
	ps2->Close();

	Int_t width = 10;
	ofstream outData;

	if (argc==1) {
		outData.open("vpd_tac_test.dat");
	}
	else if (argc==3) {
		outData.open(Form("vpd_tac_%s.dat", argv[2]));
	}
	outData<<"#A line beginning with the # character is a comment"<<endl;
	outData<<"#Board#, Channel#  where Board# is 0-16 and Channel# is 0-31"<<endl;
	outData<<"#If Pedestal=-1, Pedestal is ignored and Gain value is valid"<<endl;
	outData<<"#If Pedestal>=0, Pedestal is valid and Gain value is ignored"<<endl;
	outData<<"#Board #  Channel#  #Pedestal #Gain"<<endl;
	for(Int_t i=0; i<2; i++){
		if (i==0) outData<<"#VP001 east TAC offset"<<endl;
		else if (i==1)  outData<<"#VP002 west TAC offset"<<endl;

		for ( int j=0; j<16 ; j++ ) {
			outData<< setiosflags(ios::left) << setw(width) << i+3 << setw(width) << j+(j/4+1)*4 << setw(width) <<bbqOffset[i][j] << setw(width) << 0 <<endl;
		}
	}
	outData.close();

	if (argc==1) {
		outData.open("mvpd_tac_test.dat");
	}
	else if (argc==3) {
		outData.open(Form("mvpd_tac_%s.dat", argv[2]));
	}
	outData<<"#A line beginning with the # character is a comment"<<endl;
	outData<<"#Board#, Channel#  where Board# is 0-16 and Channel# is 0-31"<<endl;
	outData<<"#If Pedestal=-1, Pedestal is ignored and Gain value is valid"<<endl;
	outData<<"#If Pedestal>=0, Pedestal is valid and Gain value is ignored"<<endl;
	outData<<"#Board #  Channel#  #Pedestal #Gain"<<endl;
	for(Int_t i=0; i<2; i++){
		if (i==0) outData<<"#VP001 east TAC offset"<<endl;
		else if (i==1)  outData<<"#VP002 west TAC offset"<<endl;

		for ( int j=0; j<16 ; j++ ) {
			outData<< setiosflags(ios::left) << setw(width) << i+3 << setw(width) << j+(j/4+1)*4 << setw(width) <<mxqOffset[i][j] << setw(width) << 0 <<endl;
		}
	}
	outData.close();



	cout<<"VPD and MVPD offset calibration done !"<<endl;

}

//________________________________________________________________________
void initialHistograms()
{
	gStyle->SetTitleOffset(1.6,"Y");

	TString dir[2] = {"East", "West"};

	for (Int_t i=0; i<2; i++) {
		for (Int_t j=0; j<16; j++) {
			sprintf(buf, "h%sBBQVPD_RawADC_CH%d", dir[i].Data(), j);
			hBBQVPD_RawADC[i][j] = new TH1D(buf, buf, 4096, 0, 4096);

			sprintf(buf, "h%sBBQVPD_RawTAC_CH%d", dir[i].Data(), j);
			hBBQVPD_RawTAC[i][j] = new TH1D(buf, buf, 4096, 0, 4096);

			sprintf(buf, "h%sBBQVPD_ADC_CH%d", dir[i].Data(), j);
			hBBQVPD_ADC[i][j] = new TH1D(buf, buf, 4096, 0, 4096);

			sprintf(buf, "h%sBBQVPD_TAC_CH%d", dir[i].Data(), j);
			hBBQVPD_TAC[i][j] = new TH1D(buf, buf, 4096, 0, 4096);

			sprintf(buf, "h%sBBQVPD_TACDiff_CH%d", dir[i].Data(), j);
			hBBQVPD_TACDiff[i][j] = new TH1D(buf, buf, 3000, -1500, 1500);

			sprintf(buf, "h%sMXQVPD_RawADC_CH%d", dir[i].Data(), j);
			hMXQVPD_RawADC[i][j] = new TH1D(buf, buf, 4096, 0, 4096);

			sprintf(buf, "h%sMXQVPD_RawTAC_CH%d", dir[i].Data(), j);
			hMXQVPD_RawTAC[i][j] = new TH1D(buf, buf, 4096, 0, 4096);

			sprintf(buf, "h%sMXQVPD_ADC_CH%d", dir[i].Data(), j);
			hMXQVPD_ADC[i][j] = new TH1D(buf, buf, 4096, 0, 4096);

			sprintf(buf, "h%sMXQVPD_TAC_CH%d", dir[i].Data(), j);
			hMXQVPD_TAC[i][j] = new TH1D(buf, buf, 4096, 0, 4096);

			sprintf(buf, "h%sMXQVPD_TACDiff_CH%d", dir[i].Data(), j);
			hMXQVPD_TACDiff[i][j] = new TH1D(buf, buf, 3000, -1500, 1500);
		}
	}
}

//________________________________________________________________________
void writeHistograms(char *outFile)
{
	TFile f(outFile,"RECREATE");

	for (Int_t i=0; i<2; i++) {
		for (Int_t j=0; j<16; j++) {
			hBBQVPD_ADC[i][j]->Write();
			hBBQVPD_TAC[i][j]->Write();
			hBBQVPD_TACDiff[i][j]->Write();
		}
	}

	for (Int_t i=0; i<2; i++) {
		for (Int_t j=0; j<16; j++) {
			hMXQVPD_ADC[i][j]->Write();
			hMXQVPD_TAC[i][j]->Write();
			hMXQVPD_TACDiff[i][j]->Write();
		}
	}

	f.Close();
}
