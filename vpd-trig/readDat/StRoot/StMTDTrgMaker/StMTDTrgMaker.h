#ifndef STMTDTRGMaker_HH
#define STMTDTRGMaker_HH

#include "StMaker.h"

class TFile;
class TTree;
class TH1D;
class TH2D;
class TString;
class StTriggerData;

#if !defined(ST_NO_TEMPLATE_DEF_ARGS) || defined(__CINT__)
typedef vector<ULong64_t> ULong64Vec;
#else
typedef vector<ULong64_t, ULong64_t> ULong64Vec;
#endif

class StMTDTrgMaker : public StMaker {
	public:
		StMTDTrgMaker(const char *name="MTDTrg");
		~StMTDTrgMaker(){};

		Int_t   Init();
		Int_t   Make();
		Int_t   Finish();

		void    setOutFileName(const TString name) { mOutFileName = name; }
		void    setTriggerIDs(const ULong64Vec triggerids) { mTriggerIDs = triggerids; }

	protected:
		void    BookTreeHist();

	private:
		TFile   *fOutFile;
		TTree   *mEvtTree;

		StTriggerData *td;

		TString       mOutFileName;
		Int_t         mEventCounter;

		TH1D          *hBbqVpdADC[2][16]; // 0 - east; 1-west;
		TH1D          *hBbqVpdTAC[2][16]; // 0 - east; 1-west;
		TH1D          *hBbqFastVpdTAC[2];
		TH1D          *hMxqVpdADC[2][16];
		TH1D          *hMxqVpdTAC[2][16];
		TH1D          *hMxqFastVpdTAC[2];
		TH1D          *hMxqMtdADC[8][16];
		TH1D          *hMxqMtdTAC[8][16];

		ULong64Vec  mTriggerIDs;

		struct EVENT{
			UInt_t    eventNumber;
			ULong64_t triggerMask;
			UInt_t    npre;
			UInt_t    npost;
			UInt_t    nsum;
			// VPD information; BBQ and MXQ
			UShort_t Vpd[11][64]; // 11 - different bunch crossings
			UShort_t VpdHi[11][64];
			UShort_t bbqEastVpd[11][32];
			UShort_t bbqWestVpd[11][32];
			UShort_t fastBbqEastVpd[11];
			UShort_t fastBbqWestVpd[11];
			UShort_t bbcEastVpdNHits[11];
			UShort_t bbcEastVpdADCsum[11];
			UShort_t bbcEastVpdTACsum[11];
			UShort_t bbcWestVpdNHits[11];
			UShort_t bbcWestVpdADCsum[11];
			UShort_t bbcWestVpdTACsum[11];
			UShort_t mxqEastVpd[11][32];
			UShort_t mxqWestVpd[11][32];
			UShort_t fastMxqEastVpd[11];
			UShort_t fastMxqWestVpd[11];
			// TOF information
			UShort_t tofMult[11];
			UShort_t tofTrayMult[11][120];
			// MTD information
			UShort_t MT001[11][32];
			UShort_t MT002[11][32];
			UShort_t MT003[11][32];
			UShort_t MT004[11][32];
			UShort_t MT005[11][32];
			UShort_t MT006[11][32];
			UShort_t MT007[11][32];
			UShort_t MT008[11][32];
			UChar_t  MtdDsm[11][32];
			UShort_t DsmTF201Ch[8];
			UShort_t lastDsmBit[8];
		} event;

		ClassDef(StMTDTrgMaker, 0)   //StAF chain virtual base class for Makers
};
#endif
