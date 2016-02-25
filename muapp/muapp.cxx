#include "muapp.h"
#include "TMath.h"
//#include "TsPacket.h"

const bool 	DOQTCORRCHECK		= true;
const char*	QtCorrFileDescrip	= "run14auhe200_20140623";

const Float_t TACtoNS		= 0.01773;

Short_t upvpd_map(UChar_t half, UChar_t tdc, UChar_t ch) {
	Short_t	result;
	UShort_t	tg;
	tg = 12*half + tdc; // three TDIGs for half = 0 and two TDIG for half = 1;

//.....................................................................
//..... RUNS 8 and 9
//.....................................................................
//  const Short_t vmap[19][8] = {
// /* ch:  0   1   2   3   4   5   6   7 */
//       {24,  0,  8, 19,  0, 26,  0,  0}, 	// 85-0 	tg=0
//       { 0, -7,  0, -8,-19,-18,-26,-24}, 	// 85-1 	tg=1
//       {18,  0,  0,  0,  0,  0,  7,  0}, 	// 85-2 	tg=2
//       { 0,  0,  0,  0,  0,  0,  0,  0},		//      	tg=3
// 
//       {11,  0,  6, 17,  0, 25,  0,  0}, 	// 79-0 	tg=4
//       { 0, -5,  0, -6,-17,-16,-25,-11}, 	// 79-1 	tg=5
//       {16,  0,  0,  0,  0,  0,  5,  0}, 	// 79-2 	tg=6
//       { 0,  0,  0,  0,  0,  0,  0,  0},		//      	tg=7
// 
//       {20,  0, 22,  0,  0,  0,  0,  0}, 	// 1101-0 	tg=8
//       { 0,-23,  0,-22,  0,-21,  0,-20}, 	// 1101-1 	tg=9
//       {21,  0,  0,  0,  0,  0, 23,  0},		// 1101-2 	tg=10
//       { 0,  0,  0,  0,  0,  0,  0,  0},		//        	tg=11
// 
//       {27,  0,  4, 15,  0, 10,  0,  0}, 	// 80-0 	tg=12
//       { 0, -3,  0, -4,-15, -9,-10,-27}, 	// 80-1 	tg=13
//       { 9,  0,  0,  0,  0,  0,  3,  0}, 	// 80-2 	tg=14
//       { 0,  0,  0,  0,  0,  0,  0,  0},		//      	tg=15
// 
//       {14,  0,  2, 13,  0,  0,  0,  0}, 	// 83-0 	tg=16
//       { 0, -1,  0, -2,-13,-12,  0,-14}, 	// 83-1 	tg=17
//       {12,  0,  0,  0,  0,  0,  1,  0}  	// 83-2 	tg=18

//.....................................................................
//..... RUN 10
//.....................................................................
 const Short_t vmap[19][8] = {					//east and west are symmetric.
/*      ch:  0   1   2   3   4   5   6   7 */     
           {11,  0,  0,  0,  0,  1,  0,  0},    // 85/89-0		tg= 0
	       { 0,  0,  0,  0,  3,  0,  0,  0},    // 85/89-1		tg= 1
	       { 0,  0,  0,  0,  0,  0,  2,  0},    // 85/89-2		tg= 2
	       { 0,  0,  0,  0,  0,  0,  0,  0},	//				tg= 3
	       {12,  0,  0,  0,  0,  4,  0,  0},    // 79/91-0		tg= 4
	       { 0,  0,  0,  0,  6,  0,  0,  0},    // 79/91-1		tg= 5
	       { 0,  0,  0,  0,  0,  0,  5,  0},    // 79/91-2		tg= 6
	       { 0,  0,  0,  0,  0,  0,  0,  0},	//				tg= 7
	       {18,  0,  0,  0,  0,  7,  0,  0},    // 1101/1102-0	tg= 8
	       { 0,  0,  0,  0,  9,  0,  0,  0},    // 1101/1102-1	tg= 9
	       { 0,  0,  0,  0,  0,  0,  8,  0},    // 1101/1102-2	tg=10
	       { 0,  0,  0,  0,  0,  0,  0,  0},	//				tg=11
	       {19,  0,  0,  0,  0, 10,  0,  0},    // 80/885-0		tg=12
	       { 0,  0,  0,  0, 14,  0,  0,  0},    // 80/885-1		tg=13
	       { 0,  0,  0,  0,  0,  0, 13,  0},    // 80/885-2		tg=14
	       { 0,  0,  0,  0,  0,  0,  0,  0},	//				tg=15
	       { 0,  0,  0,  0,  0, 15,  0,  0},    // 83/82-0		tg=16
	       { 0,  0,  0,  0,  0, 17,  0,  0},    // 83/82-1		tg=17
	       { 0,  0,  0, 16,  0,  0,  0,  0}     // 83/82-2		tg=18
 };

	result		= 0;
	if (tg<19&&ch<8){
		result	= vmap[tg][ch];
	} else {
		//cout<<" RANGE?? ... "<<(int)half<<"  "<<(int)tdc<<"   tg="<<(int)tg<<"   ch="<<(int)ch<<"   "
		//	<<result<<"     "<<endl;
	}
	// cout<<(int)half<<" "<<(int)tg<<" "<<(int)ch<<" "<<result<<endl;
	if (result!=0) {
		//cout<<" KNOWN   ... "<<(int)half<<"  "<<(int)tdc<<"   tg="<<(int)tg<<"   ch="<<(int)ch<<"   "
		//	 <<result<<"     "<<endl;
	} else {
		//cout<<" UNKNOWN ... "<<(int)half<<"  "<<(int)tdc<<"   tg="<<(int)tg<<"   ch="<<(int)ch<<"   "
		//	<<result<<"     "<<endl;
	}
	return result;
}

void muapp::Loop(Int_t kToDo){

	char 		buf[200],bufb[200];
	Int_t 		ican=-1;
	TCanvas*	ccan[100];

	//---- setup
	TString rootfilename;
	TString plotfilebase;
	TString plotfileO,plotfile,plotfileC,pdffile;
	Long64_t nentrieschain,nentries,ientry=0,jentry=-1;
	//
	cout<<"iChoice  = "<<iChoice<<endl;
	cout<<"kRhicRun = "<<kRhicRun<<endl;
	if (kDay<10){
		sprintf(buf ,"/star/institutions/rice/geurts/muapp/run%d/out_00%d.root",kRhicRun,kDay);
		sprintf(bufb,"./pdf/muapp_run%d_day00%d",kRhicRun,kDay);
//		sprintf(buf ,"/star/institutions/rice/geurts/muapp/run14/out_%d.root",kRhicRun);
//		sprintf(bufb,"./muapp_run%d",kRhicRun);
	} else if (kDay<100){
		sprintf(buf ,"/star/institutions/rice/geurts/muapp/run%d/out_0%d.root",kRhicRun,kDay);
		sprintf(bufb,"./pdf/muapp_run%d_day0%d",kRhicRun,kDay);
//		sprintf(buf ,"/star/institutions/rice/geurts/muapp/run14/out_0%d.root",kRhicRun);
//		sprintf(bufb,"./muapp_run%d",kRhicRun);
	} else if (kDay<1000){
		sprintf(buf ,"/star/institutions/rice/geurts/muapp/run%d/out_%d.root",kRhicRun,kDay);
		sprintf(bufb,"./pdf/muapp_run%d_day%d",kRhicRun,kDay);
//		sprintf(buf ,"/star/institutions/rice/geurts/muapp/run14/out_%d.root",kRhicRun);
//		sprintf(bufb,"./muapp_run%d",kRhicRun);
		//		if (iChoice==4){
		//			sprintf(rootfilename,"outm_%d.root",kDay);
		//		}
	}
	rootfilename	= TString(buf );
	plotfilebase	= TString(bufb);
	plotfileO		= plotfilebase + TString(".ps(");
	plotfile		= plotfilebase + TString(".ps");
	plotfileC		= plotfilebase + TString(".ps]");
	pdffile			= plotfilebase + TString(".pdf");
	cout<<"rootfilename = "<<rootfilename.Data()<<endl;
	cout<<"plotfilebase = "<<plotfilebase.Data()<<endl;
	
	const int anb	= 100;
	float ax1		= 4.5;
	float ax2		= 4095.5;
	float logax1	= TMath::Log10(ax1);
	float logax2	= TMath::Log10(ax2);
	float abw		= (logax2-logax1)/((float)anb);
	double xb[anb+1];
	xb[0]			= ax1;
	for (int i=1;i<=anb;i++){
		xb[i]		= TMath::Power(10,logax1+i*abw);
	}
	
	mFile = new TFile(rootfilename,"RECREATE");
	mFile->cd();
	//hnev_trig	= new TH1D("hnev_trig","hnev_trig",25,-0.5,24.5);
	//mGlobalPt  = new TH1D("globalPt","globalPt",100,0.,3.);
	//mPrimaryPt = new TH1D("primaryPt","primaryPt",100,0.,3.);
	//mL3Pt      = new TH1D("l3Pt","l3Pt",100,0.,3.);
	//mRefMult   = new TH1D("refMult","refMult",100,0.,100.);
	
	TH1D *hcoco_vpd_w = new TH1D("hcoco_vpd_w","hcoco_vpd_w",256,-25.5,230.5);
	TH1D *hcoco_vpd_e = new TH1D("hcoco_vpd_e","hcoco_vpd_e",256,-25.5,230.5);
	TH1D *hcocokeep_vpd_w = new TH1D("hcocokeep_vpd_w","hcoco_vpd_w",256,-25.5,230.5);
	TH1D *hcocokeep_vpd_e = new TH1D("hcocokeep_vpd_e","hcoco_vpd_e",256,-25.5,230.5);
		hcocokeep_vpd_w->SetFillStyle(1001);
		hcocokeep_vpd_w->SetFillColor(3);
		hcocokeep_vpd_e->SetFillStyle(1001);
		hcocokeep_vpd_e->SetFillColor(3);
	TH1D *hcoco_p2p_w = new TH1D("hcoco_p2p_w","hcoco_p2p_w",256,-25.5,230.5);
	TH1D *hcoco_p2p_e = new TH1D("hcoco_p2p_e","hcoco_p2p_e",256,-25.5,230.5);
	//
	TH1D *hmaperror_w	= new TH1D("hmaperror_w","hmaperror_w",200,-0.5,199.5);
	TH1D *hmaperror_e	= new TH1D("hmaperror_e","hmaperror_e",200,-0.5,199.5);
	//
	TH2D *hvpdmultcompare[2];
	TH2D *hmvpdmultcompare[2];
	TH2D *hvpdmapcheck[2];
	TH2D *hmvpdmapcheck[2];
	TH2D *htrgadccompare[16][2];
	TH2D *htrgtaccompare[16][2];
	TH1D *htrgadcdiff[16][2];
	TH1D *htrgtacdiff[16][2];
	TH1D *hvpdadc[16][2];
	TH1D *hvpdtac[16][2];
	TH1D *hmvpdadc[16][2];
	TH1D *hmvpdtac[16][2];
	TH2D *htacadc[16][2];
	TH2D *hdtacadc[16][2];
	TH2D *hearliestcompare[2];
	TH1D *hearliestcomparediff[2];
	for (int is=0;is<2;is++){
		//
		sprintf(buf,"hvpdmultcompare%d",is);
		hvpdmultcompare[is]	= new TH2D(buf,buf,19,-0.5,18.5,19,-0.5,18.5);
		sprintf(buf,"hvpdmapcheck%d",is);
		hvpdmapcheck[is]	= new TH2D(buf,buf,19,-0.5,18.5,19,-0.5,18.5);
		sprintf(buf,"hmvpdmultcompare%d",is);
		hmvpdmultcompare[is]	= new TH2D(buf,buf,19,-0.5,18.5,19,-0.5,18.5);
		sprintf(buf,"hmvpdmapcheck%d",is);
		hmvpdmapcheck[is]	= new TH2D(buf,buf,19,-0.5,18.5,19,-0.5,18.5);
		//
		for (int idet=0;idet<16;idet++){
			sprintf(buf,"htrgadccompare%d%d",idet,is);
			htrgadccompare[idet][is]	= new TH2D(buf,buf,256,0.0,4096,256,0.0,4096);
			sprintf(buf,"htrgtaccompare%d%d",idet,is);
			htrgtaccompare[idet][is]	= new TH2D(buf,buf,256,0.0,4096,256,0.0,4096);
			sprintf(buf,"htrgadcdiff%d%d",idet,is);
			htrgadcdiff[idet][is]	= new TH1D(buf,buf,2048,-1024,1024);
			sprintf(buf,"htrgtacdiff%d%d",idet,is);
			htrgtacdiff[idet][is]	= new TH1D(buf,buf,2048,-1024,1024);
			sprintf(buf,"hvpdadc%d%d",idet,is);
			hvpdadc[idet][is]	= new TH1D(buf,buf,1024,0,4096);
			hvpdadc[idet][is]->SetLineColor(is+1);
			hvpdadc[idet][is]->SetLineWidth(1);
			sprintf(buf,"hvpdtac%d%d",idet,is);
			hvpdtac[idet][is]	= new TH1D(buf,buf,1024,0,4096);
			hvpdtac[idet][is]->SetLineColor(is+1);
			hvpdtac[idet][is]->SetLineWidth(1);
			sprintf(buf,"hmvpdadc%d%d",idet,is);
			hmvpdadc[idet][is]	= new TH1D(buf,buf,1024,0,4096);
			hmvpdadc[idet][is]->SetLineColor(is+3);
			hmvpdadc[idet][is]->SetLineWidth(1);
			sprintf(buf,"hmvpdtac%d%d",idet,is);
			hmvpdtac[idet][is]	= new TH1D(buf,buf,1024,0,4096);
			hmvpdtac[idet][is]->SetLineColor(is+3);
			hmvpdtac[idet][is]->SetLineWidth(1);
			//
			sprintf(buf,"htacadc%d%d",idet,is);
			htacadc[idet][is]	= new TH2D(buf,buf,anb,xb,512,0.0,4096.0);
			sprintf(buf,"hdtacadc%d%d",idet,is);
			hdtacadc[idet][is]	= new TH2D(buf,buf,anb,xb,128,-250,250);
		}
		//
		sprintf(buf,"hearliestcompare%d",is);
		hearliestcompare[is]	= new TH2D(buf,buf,256,0.0,4096,256,0.0,4096.);
		sprintf(buf,"hearliestcomparediff%d",is);
		hearliestcomparediff[is]= new TH1D(buf,buf,400,-200,200);
		//
	}
	TH2D *hdtacadcnbych	= new TH2D("hdtacadcnbych","hdtacadcnbych",32,-0.5,31.5,16,-0.5,15.5);
	TH2D *hdzvtxtrg		= new TH2D("hdzvtxtrg","hdzvtxtrg",100,-100,100,100,-100,100);
	TH2D *hdzvtxraw		= new TH2D("hdzvtxraw","hdzvtxraw",100,-100,100,100,-100,100);
	TH2D *hdzvtxcal		= new TH2D("hdzvtxcal","hdzvtxcal",100,-100,100,100,-100,100);
	TH1D *hdzvtxtrg1D	= new TH1D("hdzvtxtrg1D","hdzvtxtrg1D",100,-100,100);
	TH1D *hdzvtxraw1D	= new TH1D("hdzvtxraw1D","hdzvtxraw1D",100,-100,100);
	TH1D *hdzvtxcal1D	= new TH1D("hdzvtxcal1D","hdzvtxcal1D",100,-100,100);
	//
	//---- output tree...
	tree       = new TTree("tree","my data tree");
		//tree->Branch("istream",&istream,"istream/I");
		tree->Branch("day",&iday,"day/I");
		tree->Branch("itime",&itime,"itime/i");
		tree->Branch("run",&runid,"run/I");
		tree->Branch("evtid",&evtid,"evtid/I");
		tree->Branch("entry",&jentry,"entry/I");
		tree->Branch("grefmult",&grefmult,"grefmult/I");
		tree->Branch("nvert",&nvert,"nvert/I");
		tree->Branch("nprimary",&nprimary,"nprimary/I");
		tree->Branch("nglobal",&nglobal,"nglobal/I");
		tree->Branch("vtx",&vtx,"vtx[3]/F");
		tree->Branch("vtxe",&vtxe,"vtxe[3]/F");
		//tree->Branch("ispinbits",&ispinbits,"ispinbits/I");
		tree->Branch("fill",&fill,"fill[2]/F");
		tree->Branch("field",&field,"field/F");
		tree->Branch("zdcrate",&zdcrate,"zdcrate[2]/F");
		tree->Branch("bbcrate",&bbcrate,"bbcrate[2]/F");
		tree->Branch("bbcbg",&bbcbg,"bbcbg[2]/F");
		tree->Branch("zdccoinrate",&zdccoinrate,"zdccoinrate/F");
		tree->Branch("bbccoinrate",&bbccoinrate,"bbccoinrate/F");
		tree->Branch("daqbits",&trigindex,"daqbits/l");			// now 64 bits!
		//
		//---- TRG data....
		tree->Branch("vpd_adc",&vpd_adc,"vpd_adc[38]/F");
		tree->Branch("vpd_tac",&vpd_tac,"vpd_tac[38]/F");
		tree->Branch("mvpd_adc",&mvpd_adc,"mvpd_adc[38]/F");
		tree->Branch("mvpd_tac",&mvpd_tac,"mvpd_tac[38]/F");
		//tree->Branch("p2p_adc",&p2p_adc,"p2p_adc[16]/F");
		//tree->Branch("p2p_tac",&p2p_tac,"p2p_tac[16]/F");
		tree->Branch("bbc_adc",&bbc_adc,"bbc_adc[48]/F");
		tree->Branch("bbc_tac",&bbc_tac,"bbc_tac[48]/F");
		tree->Branch("zdc_adc",&zdc_adc,"zdc_adc[16]/F");
		tree->Branch("zdc_tac",&zdc_tac,"zdc_tac[16]/F");
		//
		//---- TOF data...
		tree->Branch("vpd_cco",&vpd_cco,"vpd_cco[38]/F");
		tree->Branch("vpd_tle",&vpd_tle,"vpd_tle[38]/F");
		tree->Branch("vpd_tot",&vpd_tot,"vpd_tot[38]/F");
		//tree->Branch("p2p_cco",&p2p_cco,"p2p_cco[16]/F");
		//tree->Branch("p2p_tle",&p2p_tle,"p2p_tle[16]/F");
		//tree->Branch("p2p_tot",&p2p_tot,"p2p_tot[16]/F");
		//
	//---- end output definition...
		
//---- map of TRG digitised VPD channel to TOF digitised VPD channel
// index is TRG ID, result is TOF ID.
//
//---- RUN-9 
//Int_t map_vpd_trg2tof[16] = 
//	{  0,  1,  2,  3,
//	   6,  7,  8,  9,
//	  10, 11, 12,  4,
//	  15, 16, 17, 18 };
//
//---- BBQ
Int_t map_vpd_trg2tof_west[16] = 
	{  0,  1,  2,  3,
	   6,  7,  8, 15,
	  10,  9, 12,  4,
	  11, 16, 17, 18 };
Int_t map_vpd_trg2tof_east[16] = 
	{  0,  1, 16,  3,
	   6,  7,  8,  9,
	  10, 12, 11,  4,			// 11 and 12 swapped here run 14
	  15,  2, 17, 18 };
//---- MXQ
Int_t map_mvpd_trg2tof_west[16] = 
	{  0,  1,  2,  3,
	   6,  7,  8, 15,
	  10,  9, 12,  4,
	  11, 16, 17, 18 };
Int_t map_mvpd_trg2tof_east[16] = 
	{  0,  1, 16,  3,
	   6,  7,  8,  9,
	  10, 12, 11,  4,			// 11 and 12 swapped here run 14
	  15,  2, 17, 18 };
		  
	int		telist_n[54];
	Float_t	telist_t[50][54];

	int		idetew,kdetew,k;
	int		nhvpdtof[2];
	int		vpd_ch,absvpd_ch;
	int		iDetCh;
	bool	isVPDhit,isP2Phit;
	bool	isLE,isTE;
	float	tval;
	int		itray,mflag,irawch,irawmod,ihalf,itdig,ichip,ichs,hitti;
	int		tword,ev_id,bu_id,trti,coco,cocouse;
	float	adc,tac,madc,mtac;
	bool	cocogood;
// 	float	n_bbcl			= 0;
// 	float	n_bbcl_vpd		= 0;
// 	float	n_bbcl_zdc		= 0;
// 	float	n_bbcl_bbcs		= 0;
// 	float	n_bbcl_vpdbbcs	= 0;
// 	float	n_bbcl_zdcbbcs	= 0;
// 	float	n_bbcl_vpdzdc	= 0;
// 	float 	prsc_bbcl 		= 250;
// 	float 	prsc_vpd  		=  40;
// 	float 	prsc_bbcs 		=  60;
// 	float 	prsc_zdc  		=   1;
	itimemin	= 0;
	int	cococut_lower;
	int	cococut_upper;
	int cocoeastshift;
	if (kYear==10){
		cococut_lower	= 123;
		cococut_upper	= 127;
		cocoeastshift	=  11;
	} else if (kYear==11 & kDay<113){
		cococut_lower	= 128;
		cococut_upper	= 132;
		cocoeastshift	=  15;
	} else if (kYear==11 & kDay>=113){
		cococut_lower	= 128;
		cococut_upper	= 131;
		cocoeastshift	=  15;
	} else if (kYear==12){			// 5us everywhere....
		cococut_lower	= 128;
		cococut_upper	= 131;
		cocoeastshift	=  12;
	//} else if (kYear==12&&kDay>=120){			// 5us stop, 500ns start
	//	cococut_lower	=  9;
	//	cococut_upper	= 11;
	//	cocoeastshift	=  0;
	} else if (kYear==13){			// 5us stop, 500ns start
		cococut_lower	= 113;
		cococut_upper	= 115;
		cocoeastshift	=   0;
	} else if (kYear==14){			// 5us stop, 500ns start
//		cococut_lower	= 113;
//		cococut_upper	= 115;
//		cocoeastshift	=   0;
		cococut_lower	= 116;
		cococut_upper	= 120;
		cocoeastshift	=   0;
	}
	cout<<"	   kYear         = "<<kYear<<endl;
	cout<<"	   cococut_lower = "<<cococut_lower<<endl;
	cout<<"	   cococut_upper = "<<cococut_upper<<endl;
	cout<<"	   cocoeastshift = "<<cocoeastshift<<endl;

	if (fChain == 0) return;
	nentrieschain	= fChain->GetEntries();
	if (kToDo>0){ nentries	= kToDo; } else { nentries = nentrieschain; }
	bool debug		= false;
	//cout<<".....    Nfiles    = "<<fChain->GetNtrees()<<endl;
	cout<<".....    Nev chain = "<<nentrieschain<<endl;
	cout<<".....    Nev to do = "<<nentries<<endl;
	Long64_t nb=0;
	
	if (DOQTCORRCHECK){
		ReadQTcorr(0);
		ReadQTcorr(1);
	}
	
	int nevtseen = 0;
	int nevtkept = 0;
	int nevttree = 0;
	//
	for (Long64_t jentry=0; jentry<nentries;jentry++) {
	//while (ientry>=0) {
	//	jentry++;
	//	if (jentry>nentries){ break; }	
		ientry = LoadTree(jentry);
		if (ientry < 0) {
			cout<<" ientry<0 "<<ientry<<" "<<fCurrent<<endl;
			break;
		}
		//nb = fChain->GetEntry(jentry);
		nb = GetEntry(jentry);
		++nevtseen;
		//cout<<nb<<endl;
		
//		if(BTofHeader_mVpdHitPattern[0][0]                     ==   0  ){ continue; }  
//		if(BTofHeader_mVpdHitPattern[0][1]                     ==   0  ){ continue; }  
		if(MuEvent_mEventSummary_mNumberOfGoodPrimaryTracks[0] <    2  ){ continue; }
		if(MuEvent_mEventSummary_mPrimaryVertexPos_mX3[0]      < - 70.0){ continue; }  
		if(MuEvent_mEventSummary_mPrimaryVertexPos_mX3[0]      >   70.0){ continue; }  
		if(fabs(MuEvent_mEventSummary_mPrimaryVertexPos_mX3[0])<  0.001){ continue; }  
		++nevtkept;

		for (int i=0;i<38;i++){ vpd_adc[i] = vpd_tac[i] = mvpd_adc[i] = mvpd_tac[i] = vpd_cco[i] = vpd_tle[i] = vpd_tte[i] = 0.0; }
		for (int i=0;i<16;i++){ p2p_adc[i] = p2p_tac[i] = p2p_cco[i] = p2p_tle[i] = p2p_tte[i] = 0.0; }
		for (int i=0;i<54;i++){ telist_n[i] = 0; }

		runid         = MuEvent_mRunInfo_mRunId[0];
		if (runid==0){ cout<<" huh ?!? "<<endl; exit(0); }
		evtid         = MuEvent_mEventInfo_mId[0];
		int kPeriod	  = runid/1000000;
		iday          = (runid%(kPeriod*1000000))/1000;
		itime         = MuEvent_mEventInfo_mTime[0];
		if (itimemin==0       ){ itimemin = itime; } else
		if (itime   < itimemin){ itimemin = itime; } 

		//if (iday==114&&runid<11114074){ continue; }
				
		//---- Trigger selection.................
		//
		//cout<<MuEvent_mTriggerIdCollection_mNTriggerId_mMask[0]<<endl;		
// 		unsigned int idata	= MuEvent_mTriggerIdCollection_mNTriggerId_mMask[0];
// 		int trgbits[32];
// 		bool mb         = false;
// 		bool zdc        = false;
// 		bool vpd        = false;
// 		bool bbcsmall   = false;
// 		bool bbclarge   = false;
// 		bool tof200fast = false;
// 		bool hlttracks  = false;
// 		//cout<<idata<<"\t";
// 		for (int i=31;i>=0;i--){
// 			if ((idata>>i) & 1){
// 				//cout<<1;
// 				trgbits[i]	= 1;
// 				if (i== 0){ mb         = true; }	// mb                 daq 1
// 				if (i== 4){ hlttracks  = true; }	// hlttracks          daq 10
// 				if (i== 7){ zdc        = true; }	// zdcmontac          daq 80
// 				if (i== 9){ vpd        = true; }	// vpdmontac          daq 200
// 				if (i==11){ bbcsmall   = true; }	// bbcsmallmonnarrow  daq 800
// 				if (i==14){ bbclarge   = true; }	// bbclargemonnarrow  daq 4000
// 				if (i==23){ tof200fast = true; }	// tof200fast         daq 800000
// 			} else {
// 				trgbits[i]	= 0;
// 				//cout<<0;
// 			}
// 		}		//cout<<"\t"<<(int)tof200fast<<endl;
// 		//if (!tof200fast) continue;
// 		//if (!mb) continue;
// 		//if (!vpd) continue;
// 		//if (!bbcsmall) continue;
// 		//if (!bbclarge) continue;
// 		//if (!hlttracks) continue;
// 		//
// 		if (bbclarge){
// 			//if(MuEvent_mEventSummary_mNumberOfGoodPrimaryTracks[0] >=    7  ){ 
// 			//if(MuEvent_mEventSummary_mPrimaryVertexPos_mX3[0]      >=  -30.0){
// 			//if(MuEvent_mEventSummary_mPrimaryVertexPos_mX3[0]      <=   30.0){	
// 								   n_bbcl 			+=                     prsc_bbcl;
// 				if (zdc) 		   n_bbcl_zdc 		+=            prsc_zdc*prsc_bbcl;
// 				if (vpd) 		   n_bbcl_vpd		+=            prsc_vpd*prsc_bbcl;
// 				if (bbcsmall) 	   n_bbcl_bbcs		+=           prsc_bbcs*prsc_bbcl;
// 				if (bbcsmall&&vpd) n_bbcl_vpdbbcs	+=  prsc_bbcs*prsc_vpd*prsc_bbcl;
// 				if (bbcsmall&&zdc) n_bbcl_zdcbbcs	+=  prsc_bbcs*prsc_zdc*prsc_bbcl;
// 				if (vpd&&zdc)      n_bbcl_vpdzdc	+=  prsc_vpd* prsc_zdc*prsc_bbcl;
// 			//} } }
// 		}
// 		//.......................................
//  		trigindex	= 0;
//  		for (int ib=0;ib<32;ib++){
// 				if (kDay>=114){ 		
// 				//cout<<MuEvent_mTriggerIdCollection_mNTriggerId_mId[0][ib]<<" ";
// 				   if (MuEvent_mTriggerIdCollection_mNTriggerId_mId[0][ib]==      2
// 					|| MuEvent_mTriggerIdCollection_mNTriggerId_mId[0][ib]== 290070){ trigindex	+=  8388608; } else	// vpd-tof-mon-tac or tof-mon-fast
// 				   if (MuEvent_mTriggerIdCollection_mNTriggerId_mId[0][ib]==      3
// 				    || MuEvent_mTriggerIdCollection_mNTriggerId_mId[0][ib]== 290003){ trigindex	+=        2; } else	// mbslow & mbslow-bbc
// 				   if (MuEvent_mTriggerIdCollection_mNTriggerId_mId[0][ib]==     19
// 				    || MuEvent_mTriggerIdCollection_mNTriggerId_mId[0][ib]== 290005){ trigindex	+=   262144; } else	// hlt
// 				   if (MuEvent_mTriggerIdCollection_mNTriggerId_mId[0][ib]==     23
// 					|| MuEvent_mTriggerIdCollection_mNTriggerId_mId[0][ib]== 290080){ trigindex	+=  4194304; } else	// mb-mon
// 				   if (MuEvent_mTriggerIdCollection_mNTriggerId_mId[0][ib]==   9300){ trigindex	+=   131072; } else	// zb
// 				   if (MuEvent_mTriggerIdCollection_mNTriggerId_mId[0][ib]== 290001
// 				    || MuEvent_mTriggerIdCollection_mNTriggerId_mId[0][ib]== 290004){ trigindex	+=        1; } else	// mb
// 				   if (MuEvent_mTriggerIdCollection_mNTriggerId_mId[0][ib]== 290002){ trigindex	+= 16777216; } else	// vpd-tac-slow
// 				   if (MuEvent_mTriggerIdCollection_mNTriggerId_mId[0][ib]== 290052){ trigindex	+=      256; } else	// vpd-mon
// 				   if (MuEvent_mTriggerIdCollection_mNTriggerId_mId[0][ib]== 290053
// 					|| MuEvent_mTriggerIdCollection_mNTriggerId_mId[0][ib]== 290054){ trigindex	+=      512; } else	// vpd-mon-tac & vpd-tac
// 				   if (MuEvent_mTriggerIdCollection_mNTriggerId_mId[0][ib]== 290060){ trigindex	+=   524288; } else	// tof-mon & tof-150-slow
// 				   if (MuEvent_mTriggerIdCollection_mNTriggerId_mId[0][ib]== 290070){ trigindex	+=  8388608; } else	// tof-150-fast
// 				   //
// 				   if (MuEvent_mTriggerIdCollection_mNTriggerId_mId[0][ib]==      7
// 					|| MuEvent_mTriggerIdCollection_mNTriggerId_mId[0][ib]== 290071){ trigindex	+=       64; } else	// zdc-mon 
// 				   if (MuEvent_mTriggerIdCollection_mNTriggerId_mId[0][ib]==      8
// 					|| MuEvent_mTriggerIdCollection_mNTriggerId_mId[0][ib]== 290072){ trigindex	+=      128; } else	// zdc-mon-tac
// 				   if (MuEvent_mTriggerIdCollection_mNTriggerId_mId[0][ib]==     11
// 					|| MuEvent_mTriggerIdCollection_mNTriggerId_mId[0][ib]== 290073){ trigindex	+=     1024; } else	// bbc-small-mon
// 				   if (MuEvent_mTriggerIdCollection_mNTriggerId_mId[0][ib]==     12
// 					|| MuEvent_mTriggerIdCollection_mNTriggerId_mId[0][ib]== 290074){ trigindex	+=     2048; } else	// bbc-small-mon-narrow
// 				   if (MuEvent_mTriggerIdCollection_mNTriggerId_mId[0][ib]==     13
// 					|| MuEvent_mTriggerIdCollection_mNTriggerId_mId[0][ib]== 290075
// 					|| MuEvent_mTriggerIdCollection_mNTriggerId_mId[0][ib]== 290085){ trigindex	+=     4096; } else	// bbc-small-mon-wide
// 				   if (MuEvent_mTriggerIdCollection_mNTriggerId_mId[0][ib]==     14
// 					|| MuEvent_mTriggerIdCollection_mNTriggerId_mId[0][ib]== 290076){ trigindex	+=     8192; } else	// bbc-large-mon
// 				   if (MuEvent_mTriggerIdCollection_mNTriggerId_mId[0][ib]==     15
// 					|| MuEvent_mTriggerIdCollection_mNTriggerId_mId[0][ib]== 290077){ trigindex	+=    16384; } else	// bbc-large-mon-narrow
// 				   if (MuEvent_mTriggerIdCollection_mNTriggerId_mId[0][ib]==     16
// 					|| MuEvent_mTriggerIdCollection_mNTriggerId_mId[0][ib]== 290078
// 					|| MuEvent_mTriggerIdCollection_mNTriggerId_mId[0][ib]== 290088){ trigindex	+=    32768; } else	// bbc-large-mon-wide
// 				   if (MuEvent_mTriggerIdCollection_mNTriggerId_mId[0][ib]==     21
// 					|| MuEvent_mTriggerIdCollection_mNTriggerId_mId[0][ib]== 290081){ trigindex	+=  1048576; } else	// bbc-small-tof-mon
// 				   if (MuEvent_mTriggerIdCollection_mNTriggerId_mId[0][ib]==     22
// 					|| MuEvent_mTriggerIdCollection_mNTriggerId_mId[0][ib]== 290079
// 					|| MuEvent_mTriggerIdCollection_mNTriggerId_mId[0][ib]== 290089){ trigindex	+=  2097152; } else	// bbc-small-tof-mon-wide
// 				   //
// 				   if (MuEvent_mTriggerIdCollection_mNTriggerId_mId[0][ib]== 290501){ trigindex	+=        8; } else	// ht-ll
// 				   if (MuEvent_mTriggerIdCollection_mNTriggerId_mId[0][ib]== 290701){ trigindex	+=       16; } else	// mtd
// 				   if (MuEvent_mTriggerIdCollection_mNTriggerId_mId[0][ib]== 290802){ trigindex	+= 33554432; } else // cosmic
// 				   if (MuEvent_mTriggerIdCollection_mNTriggerId_mId[0][ib] >      0){
// 							cout<<"Unknown trigger !!!!! "
// 								<<MuEvent_mTriggerIdCollection_mNTriggerId_mId[0][ib]<<endl;
// 				   }
// 				}
// 			//	
// //	 			if (MuEvent_mTriggerIdCollection_mNTriggerId_mId[0][ib]== 2){ trigindex	+=       1; is_MB=true;} else
// //  			if (MuEvent_mTriggerIdCollection_mNTriggerId_mId[0][ib]== 4){ trigindex	+=       8; } else
// //  			if (MuEvent_mTriggerIdCollection_mNTriggerId_mId[0][ib]== 5){ trigindex	+=      16; } else
// //  			if (MuEvent_mTriggerIdCollection_mNTriggerId_mId[0][ib]== 7){ trigindex	+=      64; } else
// //  			if (MuEvent_mTriggerIdCollection_mNTriggerId_mId[0][ib]== 8){ trigindex	+=     128; } else
// //  			if (MuEvent_mTriggerIdCollection_mNTriggerId_mId[0][ib]== 9){ trigindex	+=     256; is_VPD=true; } else
// //  			if (MuEvent_mTriggerIdCollection_mNTriggerId_mId[0][ib]==10){ trigindex	+=     512; is_VPDtac=true; } else
// //  			if (MuEvent_mTriggerIdCollection_mNTriggerId_mId[0][ib]==11){ trigindex	+=    1024; } else
// //  			if (MuEvent_mTriggerIdCollection_mNTriggerId_mId[0][ib]==12){ trigindex	+=    2048; } else
// //  			if (MuEvent_mTriggerIdCollection_mNTriggerId_mId[0][ib]==13){ trigindex	+=    4096; } else
// //  			if (MuEvent_mTriggerIdCollection_mNTriggerId_mId[0][ib]==14){ trigindex	+=    8192; } else
// //  			if (MuEvent_mTriggerIdCollection_mNTriggerId_mId[0][ib]==15){ trigindex	+=   16384; } else
// //  			if (MuEvent_mTriggerIdCollection_mNTriggerId_mId[0][ib]==16){ trigindex	+=   32768; } else
// //  			if (MuEvent_mTriggerIdCollection_mNTriggerId_mId[0][ib]==20){ trigindex	+=  524288; } else
// //  			if (MuEvent_mTriggerIdCollection_mNTriggerId_mId[0][ib]==21){ trigindex	+= 1048576; } else
// //  			if (MuEvent_mTriggerIdCollection_mNTriggerId_mId[0][ib]==22){ trigindex	+= 2097152; } else
// //  			if (MuEvent_mTriggerIdCollection_mNTriggerId_mId[0][ib]==23){ trigindex	+= 4194304; } 
//  		} //cout<<endl;

 		//
		if (jentry%1000==0){ cout<<"processing..... "<<jentry<<" ..... year="<<kPeriod<<" ..... day="<<iday<<endl; }
		//
		trigindex     = MuEvent_mTriggerIdCollection_mNTriggerId_mMask[0];
		grefmult      = MuEvent_mRefMultPos[0] + MuEvent_mRefMultNeg[0];
		nvert         = MuEvent_mEventSummary_mNumberOfVertices[0];
		nprimary      = MuEvent_mEventSummary_mNumberOfGoodPrimaryTracks[0];
		nglobal       = MuEvent_mEventSummary_mNumberOfGoodTracks[0];
		vtx[0]        = MuEvent_mEventSummary_mPrimaryVertexPos_mX1[0];
		vtx[1]        = MuEvent_mEventSummary_mPrimaryVertexPos_mX2[0];
		vtx[2]        = MuEvent_mEventSummary_mPrimaryVertexPos_mX3[0];
		vtxe[0]       = MuEvent_mPrimaryVertexError_mX1[0];
		vtxe[1]       = MuEvent_mPrimaryVertexError_mX2[0];
		vtxe[2]       = MuEvent_mPrimaryVertexError_mX3[0];
		for (int i=0;i<3;i++){
			if (vtxe[i] <0.0){ vtxe[i]=   0.0; }
			if ( vtx[i]==0.0){ vtxe[i]=-250.0; }
		}
		//
//		ispinbits     = muevent->l0Trigger().spinBits(runid);
 		field         = MuEvent_mRunInfo_mMagneticFieldZ[0];
 		fill[0]       = MuEvent_mRunInfo_mBeamFillNumber[0][0];
 		fill[1]       = MuEvent_mRunInfo_mBeamFillNumber[0][1];
		zdcrate[0]    = MuEvent_mRunInfo_mZdcEastRate[0];
		zdcrate[1]    = MuEvent_mRunInfo_mZdcWestRate[0];
		bbcrate[0]    = MuEvent_mRunInfo_mBbcEastRate[0];
		bbcrate[1]    = MuEvent_mRunInfo_mBbcEastRate[0];
 		bbcbg[0]      = MuEvent_mRunInfo_mBbcYellowBackgroundRate[0];
 		bbcbg[1]      = MuEvent_mRunInfo_mBbcBlueBackgroundRate[0];
 		zdccoinrate   = MuEvent_mRunInfo_mZdcCoincidenceRate[0];
 		bbccoinrate   = MuEvent_mRunInfo_mBbcCoincidenceRate[0];
 
//XXXXXXXXXXXXXXXXXXX 
 		//MTD?!?!?!?
//  		if ((int)mtd_mMtdRawHits.size()){
// 	 		cout<<"mtd_ = "<<mtd_<<" "<<(int)mtd_mMtdRawHits.size()<<endl;
// 	 	}
// //		for (int i=0;i<mtd_mMtdRawHits->size();i++){
// //			cout<<i<<endl;
// //		}
//XXXXXXXXXXXXXXXXXXX 
 		
//cout <<"---------------"<<endl;
		bool vpdlit[16][2]	= {0};
		bool mvpdlit[16][2]	= {0};
		int	 nhvpdtrg[2]	= {0};
		int	 nhmvpdtrg[2]	= {0};
		for (int is=0;is<2;is++){
			int kbbq,kmxq;
			int kdetewbbq,kdetewmxq;
			for (int id=0;id<16;id++){
				adc			= (Float_t)MuEvent_mVpdTriggerDetector_mADC[0][is][id];
				tac			= (Float_t)MuEvent_mVpdTriggerDetector_mTDC[0][is][id];
				madc		= (Float_t)MuEvent_mVpdTriggerDetector_mADCmxq[0][is][id];
				mtac		= (Float_t)MuEvent_mVpdTriggerDetector_mTDCmxq[0][is][id];
//if (adc>20||madc>20){ 
//	cout<<"TRG ..."<<is<<" "<<id<<" "<<adc<<" "<<tac<<endl; 
//}
				htrgadccompare[id][is]->Fill(adc,madc);
				htrgtaccompare[id][is]->Fill(tac,mtac);
				if (tac>200&&mtac>200){
					htrgadcdiff[id][is]->Fill(adc-madc);
					htrgtacdiff[id][is]->Fill(tac-mtac);
				}
				if (is==0){ kbbq		= map_vpd_trg2tof_east[id]; } else	// EAST
				if (is==1){ kbbq		= map_vpd_trg2tof_west[id]; }		// WEST
				if (is==0){ kdetewbbq	= kbbq + 19; } else					// EAST
				if (is==1){ kdetewbbq	= kbbq     ; } 						// WEST
				if (is==0){ kmxq		= map_mvpd_trg2tof_east[id]; } else	// EAST
				if (is==1){ kmxq		= map_mvpd_trg2tof_west[id]; }		// WEST
				if (is==0){ kdetewmxq	= kmxq + 19; } else					// EAST
				if (is==1){ kdetewmxq	= kmxq     ; } 						// WEST
				//cout<<is<<" "<<id<<" "<<k<<" "<<kdetew<<" "<<adc<<" "<<tac<<endl;
				if (tac>0 && tac<4000 && adc>0 && adc<4000 ){
					vpd_adc[kdetewbbq]	= adc;
					vpd_tac[kdetewbbq]	= tac;
					if (tac>200){ vpdlit[id][is] = true; }
				} else {
					vpd_adc[kdetewbbq]	= 0;
					vpd_tac[kdetewbbq]	= 0;
				}				
				if (mtac>0 && mtac<4000 && madc>0 && madc<4000 ){
					mvpd_adc[kdetewmxq]	= madc;
					mvpd_tac[kdetewmxq]	= mtac;
					if (mtac>200){ mvpdlit[id][is] = true; }
				} else {
					mvpd_adc[kdetewmxq]	= 0;
					mvpd_tac[kdetewmxq]	= 0;
				}
				int ksidetmp	= 1-is;
				if (vpd_tac[kdetewbbq] >200&&vpd_tac[kdetewbbq] <4000){ nhvpdtrg[ksidetmp]  += 1; }
//if (tac>200 && tac<4000 && adc>0 && adc<4000 ){
//	cout<<is<<" "<<id<<" "<<kbbq<<" "<<kdetewbbq<<" "<<adc
//		<<" "<<tac<<" "<<ksidetmp<<" "<<nhvpdtrg[ksidetmp]<<endl;
//}
				if (mvpd_tac[kdetewmxq]>200&&mvpd_tac[kdetewmxq]<4000){ nhmvpdtrg[ksidetmp] += 1; }
				hvpdadc[id][is]->Fill(adc);
				hvpdtac[id][is]->Fill(tac);
				hmvpdadc[id][is]->Fill(madc);
				hmvpdtac[id][is]->Fill(mtac);
			}
		}
		//
		if (DOQTCORRCHECK){
			//
			float tacprime;
			float earliesttacs[2]		= {0};
			float largesttacs[2]		= {0};
			float largesttacprimes[2]	= {0};
			for (int is=0;is<2;is++){
				float largesttac      = 0;
				float largesttacprime = 0;
				for (int id=0;id<16;id++){
					if (vpdlit[id][is]){
						adc			= (Float_t)MuEvent_mVpdTriggerDetector_mADC[0][is][id];
						tac			= (Float_t)MuEvent_mVpdTriggerDetector_mTDC[0][is][id];
						tacprime	= tac + qtcorr_bbq[id][is]->Eval(adc);
						if (tac     >largesttac     ) largesttac      = tac     ;
						if (tacprime>largesttacprime) largesttacprime = tacprime;
						float nta	= 0.0;
						float ta 	= 0.0;
						for (int kd=0;kd<16;kd++){
							if (vpdlit[kd][is] && kd!=id){
								nta	+= 1.;
								ta	+= (Float_t)MuEvent_mVpdTriggerDetector_mTDC[0][is][kd];
							}
						}
						if (nta>1.){
							ta		/= nta;
							htacadc[id][is]->Fill(adc,tac);
							hdtacadc[id][is]->Fill(adc,tac-ta);
						}
						hdtacadcnbych->Fill(16*is+id,nta);
					}
				}
				//
				float earliesttac		= MuEvent_mVpdTriggerDetector_mEarliestTDC[0][is];
				earliesttacs[is]		= earliesttac;
				largesttacs[is]			= largesttac;
				largesttacprimes[is]	= largesttacprime;
				hearliestcompare[is]->Fill(earliesttac,largesttacprime);
				hearliestcomparediff[is]->Fill(earliesttac-largesttacprime);
				//cout<<is<<" "<<earliesttac<<" "<<largesttacprime<<endl;
				//
			}
			//
			float zvtxtrg	= -999;
			float zvtxraw	= -999;
			float zvtxcal	= -999;
			if (earliesttacs[0]>200&&earliesttacs[1]>200){
				zvtxtrg	= (earliesttacs[1]-earliesttacs[0])*TACtoNS*15.;
				hdzvtxtrg->Fill(vtx[2],zvtxtrg);
				hdzvtxtrg1D->Fill(vtx[2]-zvtxtrg);
			}
			if (largesttacs[0]>200&&largesttacs[1]>200){
				zvtxraw	= (largesttacs[1]-largesttacs[0])*TACtoNS*15.;
				hdzvtxraw->Fill(vtx[2],zvtxraw);
				hdzvtxraw1D->Fill(vtx[2]-zvtxraw);
			}
			if (largesttacprimes[0]>200&&largesttacprimes[1]>200){
				zvtxcal	= (largesttacprimes[1]-largesttacprimes[0])*TACtoNS*15.;
				hdzvtxcal->Fill(vtx[2],zvtxcal);
				hdzvtxcal1D->Fill(vtx[2]-zvtxcal);
			}
			//
		}	// end DOQTCORRCHECK

		for (int id=0;id<48;id++){
				adc			= (Float_t)MuEvent_mBbcTriggerDetector_mAdc[0][id];
				tac			= (Float_t)MuEvent_mBbcTriggerDetector_mTdc[0][id];
				if (id< 24){ kdetew	= id + 24; } else		// EAST	is=0!!!
				if (id>=24){ kdetew	= id - 24; } 			// WEST	is=1!!!
				if (tac>100 && tac<4000){
					bbc_adc[kdetew]	= adc;
					bbc_tac[kdetew]	= tac;
				}
		}

		for (int id=0;id<16;id++){
				adc			= (Float_t)MuEvent_mZdcTriggerDetector_mAdc[0][id];
				tac			= (Float_t)MuEvent_mZdcTriggerDetector_mTdc[0][id];
				if (id< 8){ kdetew	= id + 8; } else		// EAST	is=0!!!
				if (id>=8){ kdetew	= id - 8; } 			// WEST	is=1!!!
				//if (tac>0 && tac<4000){
					zdc_adc[kdetew]	= adc;
					zdc_tac[kdetew]	= tac;
				//}
		}

		if (BTofRawHit_ > kMaxBTofRawHit){
			cout<<BTofRawHit_<<" !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"<<endl;
			for (int i=0;i<10;i++){ printf("\a"); }
		}

		//---- upVPD INFO FROM BTofHit..................................
// 		for (int ih=0;ih<BTofHit_;ih++){
// 			itray		= BTofHit_mTray[ih];
// 			if ( (itray==121 || itray==122) ){
// 				irawmod		= BTofHit_mModule[ih];			
// 				irawch		= BTofHit_mCell[ih];
// 				cout<<"BTofHit    .... "<<itray<<" "<<irawmod<<" "<<irawch<<endl;
// 			}
// 		}

		//---- upVPD INFO FROM BTofRawHit..................................
		for (int ih=0;ih<BTofRawHit_;ih++){
			//
			isVPDhit	= isP2Phit	= false;
			isLE		= isTE    	= false;
			mflag		= BTofRawHit_mFlag[ih];
			itray		= BTofRawHit_mTray[ih];
			irawch		= BTofRawHit_mChannel[ih];			
			//
			if ( (irawch<192) && (itray==121 || itray==122) ){
				//
				tval	= (25./1024.)*BTofRawHit_mTdc[ih];
				//
				ihalf	= (BTofRawHit_mChannel[ih])/96;
				itdig	= (BTofRawHit_mChannel[ih] - ihalf*96)/24;
				ichip	= (BTofRawHit_mChannel[ih] - ihalf*96 - itdig*24)/8;
				ichs	= (BTofRawHit_mChannel[ih] - ihalf*96 - itdig*24 - ichip*8);
				//
				hitti	= BTofRawHit_mTdc[ih]>>10;
				if (itray==121){
					tword	= BTofHeader_mTriggerTime[0][1];
				}else{
					tword	= BTofHeader_mTriggerTime[0][2];
				}
				ev_id	= (tword & 0x000FFF000) >> 12;
				bu_id	= (tword & 0x000000FFF);
				trti	= bu_id%2048;
				coco 	= (hitti - trti + 2048)%2048;

				vpd_ch 		= upvpd_map(ihalf, itdig*4+ichip, ichs);

//cout<<"TOF tray="<<itray<<" irawch="<<irawch
//<<" itdig="<<BTofRawHit_mChannel[ih]/24<<" ichip="<<ichip<<" ichs="<<ichs
//<<" vpd_ch="<<vpd_ch
//<<endl;
					
				//cout<<"BTofRawHit .... "<<itray<<" "<<irawch<<" "<<itdig<<" "<<ichs<<" "<<vpd_ch<<endl;
				if (vpd_ch==0){
					if (itray==121){ hmaperror_w->Fill(irawch,1.); } else
					if (itray==122){ hmaperror_e->Fill(irawch,1.); }
					cout<<ih<<" "<<itray<<" "<<irawch<<" "<<ihalf<<" "<<itdig<<" "<<ichip
							<<" ][ "<<itdig*4+ichip<<" "<<ichs
							<<endl;
				}
				absvpd_ch	= TMath::Abs(vpd_ch);
				if (mflag>0){ isLE = true; } else
				if (mflag<0){ isTE = true; } 

				//
				//==== Coarse Counter Shift ============================================
				//
				if (absvpd_ch>0 && absvpd_ch<=19){
					isVPDhit	= true;
					if (itray==121){ 			//.............. WEST
						iDetCh 	= absvpd_ch - 1;
						cocouse	= coco;
						hcoco_vpd_w->Fill(cocouse,1.); 
					} else if (itray==122){ 	//.............. EAST
						iDetCh	= absvpd_ch - 1 + 19; 
						cocouse	= coco + cocoeastshift;
						hcoco_vpd_e->Fill(cocouse,1.);
					} 
// 				} else if (absvpd_ch>=20){
// 					isP2Phit	= true;
// 					if (itray==121){ 
// 						iDetCh = absvpd_ch - 20; 
// 						if (kDay<125){
// 							cocouse	= coco - 24; 
// 						} else {
// 							cocouse	= coco - 22;			//GUESS!!!!!!!!!!!!!!!!!! 
// 						}
// 						hcoco_p2p_w->Fill(cocouse,1.); 
// 					} else if (itray==122){ 
// 						iDetCh = absvpd_ch - 20 + 8; 
// 						if (kDay<125){
// 							cocouse	= coco - 12; 
// 						} else {
// 							cocouse	= coco - 11;			//GUESS!!!!!!!!!!!!!!!!!! 
// 						}
// 						hcoco_p2p_e->Fill(cocouse,1.);
// 					} 
				}
				//
				//==== Coarse Counter Cut ==============================================
				//
				cocogood	= false;
				if (isVPDhit){
					if (cocouse>=cococut_lower&&cocouse<=cococut_upper){ 
						cocogood = true; 
						if (itray==121){ hcocokeep_vpd_w->Fill(cocouse,1.); }else 
						if (itray==122){ hcocokeep_vpd_e->Fill(cocouse,1.); }
					}
//				} else if (isP2Phit) {
//					if (cocouse>=95&&cocouse<=98){ 		//--- THIS IS IFFY !!!!!!!!!!!!!
//						cocogood = true; 	
//					}
				}
				//
				//======================================================================
				// 
				
				if (isLE){
					if (isVPDhit && cocogood){
						if ( (vpd_tle[iDetCh]== 0   )
						  || (vpd_tle[iDetCh] > tval) ){
							vpd_cco[iDetCh]	= (Float_t)cocouse;
							vpd_tle[iDetCh]	= tval;
						}
					}
//					if (isP2Phit && cocogood){
//						if ( (p2p_tle[iDetCh]== 0   )
//						  || (p2p_tle[iDetCh] > tval) ){
//							p2p_cco[iDetCh]	= (Float_t)cocouse;
//							p2p_tle[iDetCh]	= tval;
//						}					
//					}
				} else if (isTE) {
					if (isVPDhit){
						k						 = telist_n[iDetCh];
						telist_t[k][iDetCh]		 = tval;
						telist_n[iDetCh]		+= 1;
						if (k>49){ 
							cout<<" TOO MANY TE VALS.... "<<iDetCh<<endl;
							for (int i=0;i<10;i++){ printf("\a"); }
						}
					}
//					if (isP2Phit){
//						k					 	 = telist_n[iDetCh+38];
//						telist_t[k][iDetCh+38]	 = tval;
//						telist_n[iDetCh+38	 ]	+= 1;
//					}
				}
								
			}	//----- end check tray=121,122
		}	//----- end tof hit loop...

		//---- find TE hits for each LE hit...
		bool litmap[38]	= {0};
		float	tle,tte,diffmin;
		for (int ic=0;ic<38;ic++){
			if (vpd_tle[ic]>0.0 && telist_n[ic]>0){
				tle		= vpd_tle[ic];
				diffmin	= 999999.;
				for (int ih=0;ih<telist_n[ic];ih++){
					tte	= telist_t[ih][ic]; 
					if ((tte-tle)>0.0 && (tte-tle)<diffmin){
						diffmin		= tte-tle;
						vpd_tte[ic]	= tte;
						if (tle>0 && diffmin>5){ 
							litmap[ic]	= true;
						}
					}
				}
			}
		}
		for (int is=0;is<2;is++){ nhvpdtof[is] = 0; }
		for (int ic=0;ic<38;ic++){
			int is		= ic/19;
			int idet	= ic%19;
			if (litmap[ic]){
				nhvpdtof[is]++;
			}
		}

// 		for (int ic=0;ic<16;ic++){
// 			if (p2p_tle[ic]>0.0 && telist_n[ic+38]>0){
// 				tle		= p2p_tle[ic];
// 				diffmin	= 999999.;
// 				for (int ih=0;ih<telist_n[ic+38];ih++){
// 					tte	= telist_t[ih][ic+38]; 
// 					if ((tte-tle)>0.0 && (tte-tle)<diffmin){
// 						diffmin		= tte-tle;
// 						p2p_tte[ic]	= tte;
// 					}
// 				}
// 			}
// 		}

		float 	tot;
		for (int ic=0;ic<38;ic++){
			if (vpd_tle[ic]>0 && vpd_tte[ic]>0 ){
				tot				= vpd_tte[ic] - vpd_tle[ic];
				if (tot<60.){
					vpd_tot[ic]	= tot;
				} else {
					vpd_tle[ic]	= 0.0;
				}
			}
		}		
// 		for (int ic=0;ic<16;ic++){
// 			if (p2p_tle[ic]>0 && p2p_tte[ic]>0){
// 				tot				= p2p_tte[ic] - p2p_tle[ic];
// 				if (tot<60.){
// 					p2p_tot[ic]	= tot;
// 				} else {
// 					p2p_tle[ic]	= 0.0;
// 				}
// 			}
// 		}		

		// ----- VPD map check....
		//
		for (int is=0;is<2;is++){
			hvpdmultcompare[is]->Fill(nhvpdtof[is],nhvpdtrg[is]);
			hmvpdmultcompare[is]->Fill(nhvpdtof[is],nhmvpdtrg[is]);
			//
			if (nhvpdtrg[is]==1 && nhvpdtof[is]<=2){
				for (int idet=0;idet<19;idet++){
					int idetew	= is*19 + idet;
					if (vpd_tac[idetew]>200 && vpd_tac[idetew]<4000){
						for (int kdet=0;kdet<19;kdet++){			
							int kdetew	= is*19 + kdet;
							if (vpd_tle[kdetew]>0 && vpd_tot[kdetew]>5){
								hvpdmapcheck[is]->Fill(kdet,idet);
							}
						}
					}
				}
			}
			//
			if (nhmvpdtrg[is]==1 && nhvpdtof[is]<=2){
				for (int idet=0;idet<19;idet++){
					int idetew	= is*19 + idet;
					if (mvpd_tac[idetew]>200 && mvpd_tac[idetew]<4000){
						for (int kdet=0;kdet<19;kdet++){			
							int kdetew	= is*19 + kdet;
							if (vpd_tle[kdetew]>0 && vpd_tot[kdetew]>5){
								hmvpdmapcheck[is]->Fill(kdet,idet);
							}
						}
					}
				}
			}
		}
		
		++nevttree;
		tree->Fill();
	}
	cout<<"done with event loop..."<<endl;
	//
	//---------------------------------------------------
	
	//---- Paint...
	//
	gROOT->SetStyle("Plain");
	gStyle->SetPaperSize(TStyle::kUSLetter);
	gStyle->SetOptStat(0);
	gStyle->SetPadRightMargin(0.01);
	gStyle->SetPadTopMargin(0.065);
	gStyle->SetPadBottomMargin(0.04);
	gStyle->SetPadLeftMargin(0.06);
	gStyle->SetPadGridY(0);	
	//gStyle->SetTitleOffset(0.90,"X");
	//gStyle->SetTitleOffset(0.90,"Y");
	
 	++ican; sprintf(buf,"ccan%d",ican);
 	ccan[ican] = new TCanvas(buf,buf,0,0,750.,750.*(8.5/11.));
 	ccan[ican]->SetFillColor(10);
 	ccan[ican]->cd(); ccan[ican]->Divide(1,2,0.0001,0.0001);
		ccan[ican]->cd(1);
			gPad->SetLogy(1);
			hcoco_vpd_w->Draw();
			hcocokeep_vpd_w->Draw("same");
		ccan[ican]->cd(2);
			gPad->SetLogy(1);
			hcoco_vpd_e->Draw();
			hcocokeep_vpd_e->Draw("same");
	ccan[ican]->cd(); ccan[ican]->Update();
	ccan[ican]->Print(plotfileO.Data());

 	++ican; sprintf(buf,"ccan%d",ican);
 	ccan[ican] = new TCanvas(buf,buf,0,0,750.,750.*(8.5/11.));
 	ccan[ican]->SetFillColor(10);
 	ccan[ican]->cd(); ccan[ican]->Divide(2,1,0.0001,0.0001);
		ccan[ican]->cd(1);
			gPad->SetLogz(1);
			hvpdmultcompare[0]->Draw("col");
		ccan[ican]->cd(2);
			gPad->SetLogz(1);
			hvpdmultcompare[1]->Draw("col");
	ccan[ican]->cd(); ccan[ican]->Update();
	ccan[ican]->Print(plotfile.Data());
	//
 	++ican; sprintf(buf,"ccan%d",ican);
 	ccan[ican] = new TCanvas(buf,buf,0,0,750.,750.*(8.5/11.));
 	ccan[ican]->SetFillColor(10);
 	ccan[ican]->cd(); ccan[ican]->Divide(2,1,0.0001,0.0001);
		ccan[ican]->cd(1);
			gPad->SetLogz(1);
			hmvpdmultcompare[0]->Draw("col");
		ccan[ican]->cd(2);
			gPad->SetLogz(1);
			hmvpdmultcompare[1]->Draw("col");
	ccan[ican]->cd(); ccan[ican]->Update();
	ccan[ican]->Print(plotfile.Data());

 	++ican; sprintf(buf,"ccan%d",ican);
 	ccan[ican] = new TCanvas(buf,buf,0,0,750.,750.*(8.5/11.));
 	ccan[ican]->SetFillColor(10);
 	ccan[ican]->cd(); ccan[ican]->Divide(2,1,0.0001,0.0001);
		ccan[ican]->cd(1);
			hvpdmapcheck[0]->Draw("col");
		ccan[ican]->cd(2);
			hvpdmapcheck[1]->Draw("col");
	ccan[ican]->cd(); ccan[ican]->Update();
	ccan[ican]->Print(plotfile.Data());
	//
 	++ican; sprintf(buf,"ccan%d",ican);
 	ccan[ican] = new TCanvas(buf,buf,0,0,750.,750.*(8.5/11.));
 	ccan[ican]->SetFillColor(10);
 	ccan[ican]->cd(); ccan[ican]->Divide(2,1,0.0001,0.0001);
		ccan[ican]->cd(1);
			hmvpdmapcheck[0]->Draw("col");
		ccan[ican]->cd(2);
			hmvpdmapcheck[1]->Draw("col");
	ccan[ican]->cd(); ccan[ican]->Update();
	ccan[ican]->Print(plotfile.Data());

	++ican; sprintf(buf,"ccan%d",ican);
	ccan[ican] = new TCanvas(buf,buf,0,0,750.,750.*(8.5/11.));
	ccan[ican]->SetFillColor(10);
	ccan[ican]->cd(); ccan[ican]->Divide(4,4,0.0001,0.0001);
	for (int id=0;id<16;id++){
		ccan[ican]->cd(1+id);
			gPad->SetLogy(1);
			hvpdtac[id][0]->Draw();
			hvpdtac[id][1]->Draw("same");
	}
	ccan[ican]->cd(); ccan[ican]->Update();
	ccan[ican]->Print(plotfile.Data());
	//
	++ican; sprintf(buf,"ccan%d",ican);
	ccan[ican] = new TCanvas(buf,buf,0,0,750.,750.*(8.5/11.));
	ccan[ican]->SetFillColor(10);
	ccan[ican]->cd(); ccan[ican]->Divide(4,4,0.0001,0.0001);
	for (int id=0;id<16;id++){
		ccan[ican]->cd(1+id);
			gPad->SetLogy(1);
			hmvpdtac[id][0]->Draw();
			hmvpdtac[id][1]->Draw("same");
	}
	ccan[ican]->cd(); ccan[ican]->Update();
	ccan[ican]->Print(plotfile.Data());

	//---- MXQ vs BBQ comparison
	for (int is=0;is<2;is++){
		++ican; sprintf(buf,"ccan%d",ican);
		ccan[ican] = new TCanvas(buf,buf,0,0,750.,750.*(8.5/11.));
		ccan[ican]->SetFillColor(10);
		ccan[ican]->cd(); ccan[ican]->Divide(4,4,0.0001,0.0001);
		for (int id=0;id<16;id++){
			ccan[ican]->cd(1+id);
				gPad->SetLogz(1);
				htrgadccompare[id][is]->Draw("col");
		}
		ccan[ican]->cd(); ccan[ican]->Update();
		ccan[ican]->Print(plotfile.Data());
		//
		++ican; sprintf(buf,"ccan%d",ican);
		ccan[ican] = new TCanvas(buf,buf,0,0,750.,750.*(8.5/11.));
		ccan[ican]->SetFillColor(10);
		ccan[ican]->cd(); ccan[ican]->Divide(4,4,0.0001,0.0001);
		for (int id=0;id<16;id++){
			ccan[ican]->cd(1+id);
				gPad->SetLogz(1);
				htrgtaccompare[id][is]->Draw("col");
		}
		ccan[ican]->cd(); ccan[ican]->Update();
		ccan[ican]->Print(plotfile.Data());
		//
		++ican; sprintf(buf,"ccan%d",ican);
		ccan[ican] = new TCanvas(buf,buf,0,0,750.,750.*(8.5/11.));
		ccan[ican]->SetFillColor(10);
		ccan[ican]->cd(); ccan[ican]->Divide(4,4,0.0001,0.0001);
		for (int id=0;id<16;id++){
			ccan[ican]->cd(1+id);
				gPad->SetLogy(1);
				htrgadcdiff[id][is]->SetFillColor(7);
				htrgadcdiff[id][is]->Draw();
		}
		ccan[ican]->cd(); ccan[ican]->Update();
		ccan[ican]->Print(plotfile.Data());
		//
		++ican; sprintf(buf,"ccan%d",ican);
		ccan[ican] = new TCanvas(buf,buf,0,0,750.,750.*(8.5/11.));
		ccan[ican]->SetFillColor(10);
		ccan[ican]->cd(); ccan[ican]->Divide(4,4,0.0001,0.0001);
		for (int id=0;id<16;id++){
			ccan[ican]->cd(1+id);
				gPad->SetLogy(1);
				htrgtacdiff[id][is]->SetFillColor(5);
				htrgtacdiff[id][is]->Draw();
		}
		ccan[ican]->cd(); ccan[ican]->Update();
		ccan[ican]->Print(plotfile.Data());
	}

	//---- show TAC slewing
	//
	if (DOQTCORRCHECK){
		//
		++ican; sprintf(buf,"ccan%d",ican);
		ccan[ican] = new TCanvas(buf,buf,0,0,750.,750.*(8.5/11.));
		ccan[ican]->SetFillColor(10);
		ccan[ican]->cd(); //ccan[ican]->Divide(4,4,0.0001,0.0001);
			ccan[ican]->cd(1);
				gPad->SetLogz(1);
				hdtacadcnbych->Draw("col");
		ccan[ican]->cd(); ccan[ican]->Update();
		ccan[ican]->Print(plotfile.Data());
		//
	// 	gStyle->SetOptStat(1100011);
	// 	for (int is=0;is<2;is++){
	// 		++ican; sprintf(buf,"ccan%d",ican);
	// 		ccan[ican] = new TCanvas(buf,buf,0,0,750.,750.*(8.5/11.));
	// 		ccan[ican]->SetFillColor(10);
	// 		ccan[ican]->cd(); ccan[ican]->Divide(4,4,0.0001,0.0001);
	// 		for (int id=0;id<16;id++){
	// 			ccan[ican]->cd(1+id);
	// 			gPad->SetLogx(1);
	// 			gPad->SetLogz(1);
	// 			//hdtacadc[id][is]->Draw("col");
	// 			htacadc[id][is]->Draw("col");
	// 			qtcorr_bbq[id][is]->Draw("L");
	// 		}
	// 		ccan[ican]->cd(); ccan[ican]->Update();
	// 		ccan[ican]->Print(plotfile.Data());
	// 	}
		gStyle->SetPadRightMargin(0.01);
		gStyle->SetPadTopMargin(0.065);
		gStyle->SetPadBottomMargin(0.04);
		gStyle->SetPadLeftMargin(0.10);
		++ican; sprintf(buf,"ccan%d",ican);
		ccan[ican] = new TCanvas(buf,buf,0,0,750.,750.*(8.5/11.));
		ccan[ican]->SetFillColor(10);
		ccan[ican]->cd(); ccan[ican]->Divide(2,2,0.0001,0.0001);
		ccan[ican]->cd(1);
			gPad->SetLogz(1);
			hearliestcompare[0]->Draw("col");
		ccan[ican]->cd(2);
			gPad->SetLogz(1);
			hearliestcompare[1]->Draw("col");
		ccan[ican]->cd(3);
			gPad->SetLogy(1);
			hearliestcomparediff[0]->Draw();
		ccan[ican]->cd(4);
			gPad->SetLogy(1);
			hearliestcomparediff[1]->Draw();
		ccan[ican]->cd(); ccan[ican]->Update();
		ccan[ican]->Print(plotfile.Data());
		//
		gStyle->SetOptFit(1111);
		gStyle->SetStatY(0.9);
		gStyle->SetStatW(0.35);
		gStyle->SetStatStyle(0);
		++ican; sprintf(buf,"ccan%d",ican);
		ccan[ican] = new TCanvas(buf,buf,0,0,750.,750.*(8.5/11.));
		ccan[ican]->SetFillColor(10);
		ccan[ican]->cd(); ccan[ican]->Divide(3,2,0.0001,0.0001);
		ccan[ican]->cd(1);
			gPad->SetLogz(1);
			hdzvtxtrg->Draw("col");
		ccan[ican]->cd(2);
			gPad->SetLogz(1);
			hdzvtxraw->Draw("col");
		ccan[ican]->cd(3);
			gPad->SetLogz(1);
			hdzvtxcal->Draw("col");
		ccan[ican]->cd(4);
			gPad->SetLogz(1);
			//hdzvtxtrg1D->Draw();
			TF1 *gtrg	= new TF1("gtrg","gaus",-100,100);	
				gtrg->SetLineWidth(1); gtrg->SetLineColor(2);
			hdzvtxtrg1D->Fit("gtrg");
		ccan[ican]->cd(5);
			gPad->SetLogz(1);
			//hdzvtxraw1D->Draw();
			TF1 *graw	= new TF1("graw","gaus",-100,100);	
				graw->SetLineWidth(1); graw->SetLineColor(2);
			hdzvtxraw1D->Fit("graw");
		ccan[ican]->cd(6);
			gPad->SetLogz(1);
			//hdzvtxcal1D->Draw();
			TF1 *gcal	= new TF1("gcal","gaus",-100,100);	
				gcal->SetLineWidth(1); gcal->SetLineColor(2);
			hdzvtxcal1D->Fit("gcal");
		ccan[ican]->cd(); ccan[ican]->Update();
		ccan[ican]->Print(plotfile.Data());
		//
	}	// end DOQTCORRCHECK
	
	cout<<"itimemin = "<<itimemin<<endl;
	mFile->Write();
	mFile->Close();
	ccan[ican]->Print(plotfileC);
	sprintf(buf,"ps2pdf %s %s",plotfile.Data(),pdffile.Data());
	gSystem->Exec(buf);
	sprintf(buf,"cp -v %s /star/u/geurts/WWW/files/",pdffile.Data());
	gSystem->Exec(buf);

	cout<<endl;
	cout<<"nevtseen = "<<nevtseen<<endl;
	cout<<"nevtkept = "<<nevtkept<<endl;
	cout<<"nevttree = "<<nevttree<<endl;
	cout<<endl;
	

// 	cout<<endl;
// 	cout<<".....counts incremented by prescales: zdc=1,vpd=40,bbcs=60,bbcl=250"<<endl; 
// 	cout<<".....(actual event counts in parens)..............................."<<endl;
// // 								   n_bbcl 			+=                     prsc_bbcl;
// // 				if (zdc) 		   n_bbcl_zdc 		+=            prsc_zdc*prsc_bbcl;
// // 				if (vpd) 		   n_bbcl_vpd		+=            prsc_vpd*prsc_bbcl;
// // 				if (bbcsmall) 	   n_bbcl_bbcs		+=           prsc_bbcs*prsc_bbcl;
// // 				if (bbcsmall&&vpd) n_bbcl_vpdbbcs	+=  prsc_bbcs*prsc_vpd*prsc_bbcl;
// // 				if (bbcsmall&&zdc) n_bbcl_zdcbbcs	+=  prsc_bbcs*prsc_zdc*prsc_bbcl;
// // 				if (vpd&&zdc)      n_bbcl_vpdzdc	+=  prsc_vpd* prsc_zdc*prsc_bbcl;
// 	cout<<" bbcl            = "<<n_bbcl        <<"\t ("<<n_bbcl/prsc_bbcl<<")"<<endl;
// 	cout<<" bbcl&&vpd       = "<<n_bbcl_vpd    <<"\t ("<<n_bbcl_vpd/prsc_bbcl/prsc_vpd<<")"<<endl;
// 	cout<<" bbcl&&bbcs      = "<<n_bbcl_bbcs   <<"\t ("<<n_bbcl_bbcs/prsc_bbcl/prsc_bbcs<<")"<<endl;
// 	cout<<" bbcl&&zdc       = "<<n_bbcl_zdc    <<"\t ("<<n_bbcl_zdc/prsc_bbcl/prsc_zdc<<")"<<endl;
// 	cout<<" bbcl&&vpd&&bbcs = "<<n_bbcl_vpdbbcs<<"\t ("<<n_bbcl_vpdbbcs/prsc_bbcl/prsc_vpd/prsc_bbcs<<")"<<endl;
// 	cout<<" bbcl&&zdc&&bbcs = "<<n_bbcl_zdcbbcs<<"\t ("<<n_bbcl_zdcbbcs/prsc_bbcl/prsc_zdc/prsc_bbcs<<")"<<endl;
// 	cout<<" bbcl&&vpd&&zdc  = "<<n_bbcl_vpdzdc <<"\t ("<<n_bbcl_vpdzdc/prsc_bbcl/prsc_vpd/prsc_zdc<<")"<<endl;
// 	cout<<".....ratios........................................................"<<endl;
// 	if (n_bbcl>0){
// 		cout<<" (bbcl&&vpd)/bbcl               = "<<n_bbcl_vpd    /n_bbcl<<endl;
// 		cout<<" (bbcl&&bbcs)/bbcl              = "<<n_bbcl_bbcs   /n_bbcl<<endl;
// 		cout<<" (bbcl&&zdc)/bbcl               = "<<n_bbcl_zdc    /n_bbcl<<endl;
// 		cout<<" (bbcl&&vpd&&bbcs)/bbcl         = "<<n_bbcl_vpdbbcs/n_bbcl<<endl;
// 		cout<<" (bbcl&&zdc&&bbcs)/bbcl         = "<<n_bbcl_zdcbbcs/n_bbcl<<endl;
// 		cout<<" (bbcl&&vpd&&zdc)/bbcl          = "<<n_bbcl_vpdzdc /n_bbcl<<endl;
// 	}
// 	if (n_bbcl_vpd>0){
// 		cout<<" (bbcl&&zdc&&bbcs)/(bbcl&&vpd)  = "<<n_bbcl_zdcbbcs/n_bbcl_vpd<<endl;
// 		cout<<" (bbcl&&vpd&&zdc)/(bbcl&&vpd)   = "<<n_bbcl_vpdzdc/n_bbcl_vpd<<endl;
// 	} else {
// 		cout<<" (bbcl&&zdc&&bbcs)/(bbcl&&vpd)  = "<<"N/A"<<endl;
// 		cout<<" (bbcl&&vpd&&zdc)/(bbcl&&vpd)   = "<<"N/A"<<endl;
// 	}
// 	if (n_bbcl_bbcs>0){
// 		cout<<" (bbcl&&zdc&&bbcs)/(bbcl&&bbcs) = "<<n_bbcl_zdcbbcs/n_bbcl_bbcs<<endl;	
// 		cout<<" (bbcl&&vpd&&zdc)/(bbcl&&bbcs)  = "<<n_bbcl_vpdzdc/n_bbcl_bbcs<<endl;	
// 	} else {
// 		cout<<" (bbcl&&zdc&&bbcs)/(bbcl&&bbcs) = "<<"N/A"<<endl;	
// 		cout<<" (bbcl&&vpd&&zdc)/(bbcl&&bbcs)  = "<<"N/A"<<endl;
// 	}
// 	if (n_bbcl_zdc>0){
// 		cout<<" (bbcl&&zdc&&bbcs)/(bbcl&&zdc)  = "<<n_bbcl_zdcbbcs/n_bbcl_zdc<<endl;
// 		cout<<" (bbcl&&vpd&&zdc)/(bbcl&&zdc)   = "<<n_bbcl_vpdzdc/n_bbcl_zdc<<endl;
// 	} else {
// 		cout<<" (bbcl&&zdc&&bbcs)/(bbcl&&zdc)  = "<<"N/A"<<endl;
// 		cout<<" (bbcl&&vpd&&zdc)/(bbcl&&zdc)   = "<<"N/A"<<endl;
// 	}
//	cout<<endl;

}
//----- end of routine....

// t1 = (l-x)/c
// t2 = (l+x)/c
// t2-t1 = 2x/c
// dt = 2*x/c
// dt = slope*x where slope=(2/c)

// time*cal = tac -> tac/cal = time

// dtac/cal = slope*x
// dtac     = slopetac*x where slopetac = cal*2/c

// cal = slopetac*c/2

// x = (c/2)*dt
// x = slope*dt where slope=(c/2)*cal

//x=50cm
//dtac=200
//-> slopetac = 4 bins/cm = cal*2/c
//-> (4bins/cm)*(30cm/ns)/2
//-> 60bins/ns
//-> (1/60)ns/bin

// 	char buf[50];
// 	gStyle->SetOptStat(0);
// 
// 	Double_t	lpars[2];
// 	TF1 *flin = new TF1("flin","pol1",-30,30);
// 	//TProfile *htdvpd_pfx = (TProfile*)htdvpd->ProfileX("htdvpd_pfx",1,-1,"");
// 	htdvpd->FitSlicesY();
// 	TH1D *htdvpd_pfx = (TH1D*)gDirectory->Get("htdvpd_1");
// 			htdvpd_pfx->SetMarkerStyle(20);
// 			htdvpd_pfx->SetMarkerColor(2);
// 	htdvpd_pfx->Fit("flin","NR");
// 		flin->GetParameters(lpars);
// 		Double_t cal = 30.0*lpars[1]/2.0;
// 		cout<<lpars[0]<<" "<<lpars[1]<<" >>--> "<<1000./cal<<" ps/bin"<<endl;
// 
// 	TCanvas *muapp_1 = new TCanvas("muapp_1","muapp_1",360,360,640,460);
// 	muapp_1->cd(1);
// 		htdvpd->Draw("colz");
// 		htdvpd_pfx->Draw("same");
// 		flin->Draw("same");
// 	muapp_1->cd(); muapp_1->Update();
//     TLatex *tt = new TLatex();
//     	tt->SetTextSize(0.05);
//     	tt->SetNDC(kTRUE);
// 		sprintf(buf,"slope=%4.2f tacbins/cm  #Rightarrow  %4.2f ps/tacbin",lpars[1],1000./cal);
// 		cout<<buf<<endl;
// 	    	tt->DrawLatex(0.18,0.91,buf);
// 	muapp_1->Print("muapp_1.eps");
	
void muapp::ReadQTcorr(Int_t icrate){
	char buf[200];
	TString qtcorrfile;
	if (icrate==0){
		sprintf(buf,"../caleval/qtcorr_bbq_%s.txt",QtCorrFileDescrip);
		qtcorrfile	= TString(buf);
	} else if (icrate==1){
		sprintf(buf,"../caleval/qtcorr_mxq_%s.txt",QtCorrFileDescrip);
		qtcorrfile	= TString(buf);
	}
	cout<<"muapp::ReadQTcorr ... Reading "<<qtcorrfile.Data()<<endl;

	string 		str;
	char*		cstr;
	ifstream    file;
	file.open(qtcorrfile.Data(),ifstream::in);
	int iadctac	= 0;
	int iside	= 0;
	int idetew	= 0;
	int idet	= 0;
	int info[8][2][16][2]	= {0};		// value, adc/tac, idet, iside
	//
	while(getline(file,str)){
		cstr = &str[0];
		if (str.empty()) continue; 
		if (cstr[0]=='#') continue;
		int field,n;
		char *start = cstr+4;
		int kf=0;
		int fields[11]={0};
		while (sscanf(start, "%d%n", &field, &n) == 1) {
			start += n;
			fields[kf]	= field;
			//cout<<fields[kf]<<" ";
			kf++;
		}	//cout<<" \t ";
		//
		iside		= idetew/16;
		iadctac		= fields[2];
		for (int kp=3;kp<11;kp++){
			info[kp-3][iadctac][idet][iside]	= fields[kp];
			//cout<<kp-3<<" "<<iadctac<<" "<<idet<<" "<<iside<<" "<<info[kp-3][iadctac][idet][iside]<<endl;
		}
		//cout<<idet<<" "<<iside<<" "<<iadctac<<endl;
		if (iadctac==1) idetew++;
		idet	= idetew%16;
		//
		//if (iside==1&&idet>=32) break;
		//
	}
	file.close();
	//
 	for (int is=0;is<2;is++){
 		for (int idet=0;idet<16;idet++){
 			if (icrate==0){
				qtcorr_bbq[idet][is]	= new TGraph();				
 			} else if (icrate==1){
				qtcorr_mxq[idet][is]	= new TGraph();
 			}
 		}
 	}
 	for (int is=0;is<2;is++){
 		for (int idet=0;idet<16;idet++){
 			//
 			//cout<<is<<" "<<idet<<" \t ";
 			//for (int kp=0;kp<8;kp++){
 			//	cout<<info[kp][0][idet][is]<<" ";
 			//}	cout<<endl;
 			//cout<<is<<" "<<idet<<" \t ";
 			//for (int kp=0;kp<8;kp++){
 			//	cout<<info[kp][1][idet][is]<<" ";
 			//}	cout<<endl;
			for (int iadc=0;iadc<4096;iadc++){
				//
				int thiskp	= 0;
				for (int kp=1;kp<8;kp++){
					if (iadc>info[kp-1][0][idet][is]&&iadc<=info[kp][0][idet][is]){
						thiskp	= kp;
						break;
					}
				}
				//if (is==0&&idet==0){
				//	cout<<iadc<<" "<<thiskp<<endl;
				//}
				//
				int ng	= 0;
 				if (icrate==0){
 					ng	= qtcorr_bbq[idet][is]->GetN();
						  qtcorr_bbq[idet][is]->SetPoint(ng,iadc,info[thiskp][1][idet][is]);
 				} else if (icrate==1){
 					ng	= qtcorr_mxq[idet][is]->GetN();
						  qtcorr_mxq[idet][is]->SetPoint(ng,iadc,info[thiskp][1][idet][is]);
	 			}
				//
			}
 		}
 	}
	//

}
