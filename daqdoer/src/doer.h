// $Id: doer.h,v 1.2 2016/02/29 21:41:36 geurts Exp $
 // Author: W.J. Llope 2009/06/01

#include <DAQ_READER/daqReader.h>
#include <DAQ_READER/daq_dta.h>
#include <DAQ_BSMD/daq_bsmd.h>
#include <DAQ_BTOW/daq_btow.h>
#include <DAQ_EMC/daq_emc.h>
#include <DAQ_ESMD/daq_esmd.h>
#include <DAQ_ETOW/daq_etow.h>
#include <DAQ_FPD/daq_fpd.h>
#include <DAQ_FTP/daq_ftp.h>
#include <DAQ_L3/daq_l3.h>
#include <DAQ_PMD/daq_pmd.h>
#include <DAQ_PP2PP/daq_pp2pp.h>
#include <DAQ_RIC/daq_ric.h>
#include <DAQ_SC/daq_sc.h>
#include <DAQ_SSD/daq_ssd.h>
#include <DAQ_SVT/daq_svt.h>
#include <DAQ_TOF/daq_tof.h>
#include <DAQ_MTD/daq_mtd.h>
#include <DAQ_TPC/daq_tpc.h>
#include <DAQ_TPX/daq_tpx.h>
#include <DAQ_TRG/daq_trg.h>

#include "StTriggerData2019.h"

#include <TFile.h>
#include <TTree.h>
#include <TH1D.h>
#include <TH2D.h>

extern float TrayTrigTimingPeaks[122];
extern int   trigwindowLow[6];
extern int   trigwindowHigh[6];

//----- new master data block.......................................
struct doer_st {			// filled once/event - requires coarse counter cut....
	int		vpd_cco[38];	// digitized by TOF
	float	vpd_tle[38];	// digitized by TOF
	float	vpd_tte[38];	// digitized by TOF
	float	vpd_tot[38];	// digitized by TOF
	int		p2p_cco[38];	// digitized by TOF
	float	p2p_tle[16];	// digitized by TOF
	float	p2p_tte[16];	// digitized by TOF
	float	p2p_tot[16];	// digitized by TOF
};
struct stop_st {			// filled once/event, variable length/event
	int 	nTofHits;
	int 	nTofHitsLE;
	int 	nTofHitsTE;
	int		tray[25000];
	int		module[25000];
	int		cell[25000];
	float	coco[25000];
	float	bunchid[25000];
	float	trgdtime[25000];
	float	tle[25000];
	float	tte[25000];
	float	tot[25000];
};
struct mtd_st {				// filled once/event, variable length/event
	int 	nMtdHits;
	int 	nMtdHitsLE;
	int 	nMtdHitsTE;
	int		tray[1000];
	int		cell[1000];
	int		module[1000];
	int		modch[1000];
	int		strip[1000];
	int		strlr[1000];
	float	coco[1000];
	float	trgdtime[1000];
	float	tle[1000];
	float	tte[1000];
	float	tot[1000];
};


//----- run stuff...................................................
struct evp_data_st {
    u_int event_number;
    u_int token ;		// current token
    u_int trgcmd ;		// current trigger command
    u_int daqcmd ;		// current DAQ command
    u_int trgword ;		// the Trigger Word
    u_int phyword ;		// the Physics Word
    u_int daqbits ;		// "offline" bits aka L3 summary...
    u_longlong daqbits64 ;		// "offline" bits aka L3 summary...
    u_int daqbits_l1;		// triggers satisfying l1
    u_int daqbits_l2;		// triggers satisfying l2
    u_int evt_time ;		// time in UNIX seconds
    u_int seq ;			// event sequence from EVB
};

//---- silicon stuff..................................................
//
// #define MAXSEQ		8
// #define MAXCHAIN	4
// #define MAXSVX		6
// #define MAXHIT		10
// #define AVRTIME		20
// #define NSIGMA		3
// struct sili_st {
// 	signed short si[MAXSEQ][MAXCHAIN][MAXSVX][PP2PP_SVX_CH];	// raw silicon data
// };
// struct Hits_st {
//     int   N[MAXSEQ][MAXCHAIN];					// number of hits in this plane
//     float X[MAXSEQ][MAXCHAIN][MAXHIT];			// hit position
//     float Q[MAXSEQ][MAXCHAIN][MAXHIT];			// hit charge
// };
// struct SVX_st {
//     float Thr[MAXSEQ][MAXCHAIN][MAXSVX];		// current threshold per SVX
//     float Ped[MAXSEQ][MAXCHAIN][MAXSVX];		// last pedestal per SVX
//     float Sigma[MAXSEQ][MAXCHAIN][MAXSVX];		// last pedestal sigma per SVX
// };


//---- trg stuff......................................................
//
struct bunch_crossing_st {
    unsigned int xing_lo;
    unsigned int xing_hi;
    unsigned int bunch_number;
    unsigned int b120;
};
struct P2P_st {
    unsigned short RPWVD2_ADC;
    unsigned short RPWVD1_ADC;
    unsigned short RPWVU2_ADC;
    unsigned short RPWVU1_ADC;
    unsigned short RPEVD2_ADC;
    unsigned short RPEVD1_ADC;
    unsigned short RPEVU2_ADC;
    unsigned short RPEVU1_ADC;
    unsigned short RPWVD2_TAC;
    unsigned short RPWVD1_TAC;
    unsigned short RPWVU2_TAC;
    unsigned short RPWVU1_TAC;
    unsigned short RPEVD2_TAC;
    unsigned short RPEVD1_TAC;
    unsigned short RPEVU2_TAC;
    unsigned short RPEVU1_TAC;
    unsigned short RPWHI2_ADC;
    unsigned short RPWHI1_ADC;
    unsigned short RPWHO2_ADC;
    unsigned short RPWHO1_ADC;
    unsigned short RPEHI2_ADC;
    unsigned short RPEHI1_ADC;
    unsigned short RPEHO2_ADC;
    unsigned short RPEHO1_ADC;
    unsigned short RPWHI2_TAC;
    unsigned short RPWHI1_TAC;
    unsigned short RPWHO2_TAC;
    unsigned short RPWHO1_TAC;
    unsigned short RPEHI2_TAC;
    unsigned short RPEHI1_TAC;
    unsigned short RPEHO2_TAC;
    unsigned short RPEHO1_TAC;
};
struct VPD_st {
	unsigned int   vpd_trgadc[36];
	unsigned int   vpd_trgtac[36];
	unsigned int   vpd_trgadc_hi[36];
	unsigned int   vpd_trgtac_hi[36];
};
// struct DSM_st {
//     unsigned short lastDSM[8];		// Layer 3 the last DSM (LD301). Our word is lastDSM[3]. Bits are 1:3 - ET,ITE,ITW
//     unsigned short lastTOF[8];		// Layer 2 DSM. Our DSM output is in lastTOF[3]
// 									// Bits are: 0 - MTD; 1:4 - EA:ED; 5:6 - EOR,WOR; 7:10 - EVF,EHF,WVF,WHF. Other - must be 0.
//     unsigned long  pp2ppDSM;		// Layer 1 DSM inputs from our board (MT101)
// 									// Bits: 0:15 - EO1,EO2,EI1,EI2,WO1,WO2,WI1,WI2,EU1,EU2,ED1,ED2,WU1,WU2,WD1,WD2
//     unsigned short tcuBits;			// TCU bits
// };

struct TRG_st {
	int   bbc_adc[48];
	int   bbc_tac[48];
	int   zdc_adc[6];
	int   zdc_tac[6];
	int   mtd_adc[4];
	int   mtd_tac[4];
	int   tofmult[120];
};

struct INFO_st {
	unsigned int    nwordsfiber[4];
	int nhits_early;
	int nhits_intime;
	int nrawhits_mtd[30];
};



//---- end ...............................................................................
