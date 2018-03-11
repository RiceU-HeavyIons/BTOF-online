
#include <stdio.h>
#include <unistd.h>
#include <getopt.h>
#include <sys/types.h>
#include <stdlib.h>
#include <iostream>

using namespace std;

#include <rtsLog.h>

#include "doer.h"

// int trg_doer(daqReader *rdr,
// 				struct bunch_crossing_st *xing,
// 				struct P2P_st *P2P,
// 				struct DSM_st *DSM,
// 				struct VPD_st *VPD,
// 				struct TRG_st *TRG)  {
int trg_doer(daqReader *rdr,
				struct bunch_crossing_st *xing,
				struct P2P_st *P2P,
				struct VPD_st *VPD,
				struct TRG_st *TRG)  {

    int ii,k;
    daq_dta *dd;
    dd = rdr->det("trg")->get("raw") ;
    if (!(dd && dd->iterate())) return -1;

    StTriggerData2018* trg_p;
    trg_p = new StTriggerData2018((TriggerDataBlk2018*)dd->Void, rdr->run);

    xing->xing_lo 		= trg_p->bunchCounterLow();
    xing->xing_hi 		= trg_p->bunchCounterHigh();
    xing->bunch_number 	= trg_p->bunchId7Bit() ;
    xing->b120 			= trg_p->bunchId48Bit() ;
    //printf(" BunchId 7bit=%d  48bit=%d\n",trg_p->bunchId7Bit(), trg_p->bunchId48Bit());
    //printf("    xing 7bit=%d  48bit=%d\n",xing->xing_lo, xing->xing_hi);
    // East     : 0, West       : 1;
    // Vertical : 0, Horizontal : 1;
    // Up/Outer : 0, Down/Inner : 1;
    // channel1 : 0, channel2   : 1;
    P2P->RPEVU1_ADC = trg_p->pp2ppADC(east,0,0,0);
    P2P->RPEVU2_ADC = trg_p->pp2ppADC(east,0,0,1);
    P2P->RPEVD1_ADC = trg_p->pp2ppADC(east,0,1,0);
    P2P->RPEVD2_ADC = trg_p->pp2ppADC(east,0,1,1);
    P2P->RPEVU1_TAC = trg_p->pp2ppTAC(east,0,0,0);
    P2P->RPEVU2_TAC = trg_p->pp2ppTAC(east,0,0,1);
    P2P->RPEVD1_TAC = trg_p->pp2ppTAC(east,0,1,0);
    P2P->RPEVD2_TAC = trg_p->pp2ppTAC(east,0,1,1);
    P2P->RPWVU1_ADC = trg_p->pp2ppADC(west,0,0,0);
    P2P->RPWVU2_ADC = trg_p->pp2ppADC(west,0,0,1);
    P2P->RPWVD1_ADC = trg_p->pp2ppADC(west,0,1,0);
    P2P->RPWVD2_ADC = trg_p->pp2ppADC(west,0,1,1);
    P2P->RPWVU1_TAC = trg_p->pp2ppTAC(west,0,0,0);
    P2P->RPWVU2_TAC = trg_p->pp2ppTAC(west,0,0,1);
    P2P->RPWVD1_TAC = trg_p->pp2ppTAC(west,0,1,0);
    P2P->RPWVD2_TAC = trg_p->pp2ppTAC(west,0,1,1);
    P2P->RPEHO1_ADC = trg_p->pp2ppADC(east,1,0,0);
    P2P->RPEHO2_ADC = trg_p->pp2ppADC(east,1,0,1);
    P2P->RPEHI1_ADC = trg_p->pp2ppADC(east,1,1,0);
    P2P->RPEHI2_ADC = trg_p->pp2ppADC(east,1,1,1);
    P2P->RPEHO1_TAC = trg_p->pp2ppTAC(east,1,0,0);
    P2P->RPEHO2_TAC = trg_p->pp2ppTAC(east,1,0,1);
    P2P->RPEHI1_TAC = trg_p->pp2ppTAC(east,1,1,0);
    P2P->RPEHI2_TAC = trg_p->pp2ppTAC(east,1,1,1);
    P2P->RPWHO1_ADC = trg_p->pp2ppADC(west,1,0,0);
    P2P->RPWHO2_ADC = trg_p->pp2ppADC(west,1,0,1);
    P2P->RPWHI1_ADC = trg_p->pp2ppADC(west,1,1,0);
    P2P->RPWHI2_ADC = trg_p->pp2ppADC(west,1,1,1);
    P2P->RPWHO1_TAC = trg_p->pp2ppTAC(west,1,0,0);
    P2P->RPWHO2_TAC = trg_p->pp2ppTAC(west,1,0,1);
    P2P->RPWHI1_TAC = trg_p->pp2ppTAC(west,1,1,0);
    P2P->RPWHI2_TAC = trg_p->pp2ppTAC(west,1,1,1);
    //for(ii=0; ii<8; ii++) DSM->lastDSM[ii] = trg_p->lastDSM(ii);
    //for(ii=0; ii<8; ii++) DSM->lastTOF[ii] = trg_p->lastTOF(ii);
    //DSM->pp2ppDSM = trg_p->pp2ppDSM();
    //DSM->tcuBits = trg_p->tcuBits();

    int tempn[2] = {0};
    for (int i=0;i<16;i++){
    	k	= i+1;													// TRG channels are [1,16] in StEvent
	    VPD->vpd_trgadc[i]			= trg_p->vpdADC(west,k,0);
	    VPD->vpd_trgadc[i+16]		= trg_p->vpdADC(east,k,0);
	    VPD->vpd_trgtac[i]			= trg_p->vpdTDC(west,k,0);			// west
	    VPD->vpd_trgtac[i+16]		= trg_p->vpdTDC(east,k,0);			// east
	    VPD->vpd_trgadc_hi[i]		= trg_p->vpdADCHighThr(west,k,0);
	    VPD->vpd_trgadc_hi[i+16]	= trg_p->vpdADCHighThr(east,k,0);
	    VPD->vpd_trgtac_hi[i]		= trg_p->vpdTDCHighThr(west,k,0);
	    VPD->vpd_trgtac_hi[i+16]	= trg_p->vpdTDCHighThr(east,k,0);
	    if (VPD->vpd_trgadc[i]		< 0.){ VPD->vpd_trgadc[i]		= 0.; }
		if (VPD->vpd_trgadc[i+16]	< 0.){ VPD->vpd_trgadc[i+16]	= 0.; }
		if (VPD->vpd_trgtac[i]		< 0.){ VPD->vpd_trgtac[i]		= 0.; }
		if (VPD->vpd_trgtac[i+16]	< 0.){ VPD->vpd_trgtac[i+16]	= 0.; }
		if (VPD->vpd_trgadc_hi[i]	< 0.){ VPD->vpd_trgadc_hi[i]	= 0.; }
		if (VPD->vpd_trgadc_hi[i+16]< 0.){ VPD->vpd_trgadc_hi[i+16]	= 0.; }
		if (VPD->vpd_trgtac_hi[i]	< 0.){ VPD->vpd_trgtac_hi[i]	= 0.; }
		if (VPD->vpd_trgtac_hi[i+16]< 0.){ VPD->vpd_trgtac_hi[i+16]	= 0.; }
		//
	    //if (trg_p->vpdADC(west,k,0)>0 || trg_p->vpdADC(east,k,0)>0){
	    //	cout<<i<<" "<<trg_p->vpdADC(west,k,0)<<" "<<trg_p->vpdADC(east,k,0)<<endl;
	    //}
		//
		if (VPD->vpd_trgtac[i]>200&&VPD->vpd_trgtac[i]<3500){ ++tempn[0]; }		// west
		if (VPD->vpd_trgtac[i+16]>200&&VPD->vpd_trgtac[i+16]<3500){ ++tempn[1]; }		// west
		//
	}

//	if (tempn[0]<3||tempn[1]<3){
//	    for (int i=0;i<16;i++){
//			if (VPD->vpd_trgtac[i]>200&&VPD->vpd_trgtac[i]<3500){ cout<<"doer_trg ... BBQ WEST trgch="<<i<<" adc="<<VPD->vpd_trgadc[i]<<" tac="<<VPD->vpd_trgtac[i]<<endl; }
//			if (VPD->vpd_trgtac[i+16]>200&&VPD->vpd_trgtac[i+16]<3500){ cout<<"doer_trg ... BBQ EAST trgch="<<i<<" adc="<<VPD->vpd_trgadc[i+16]<<" tac="<<VPD->vpd_trgtac[i+16]<<endl; }
//			if (VPD->vpd_trgtac_hi[i]>200&&VPD->vpd_trgtac_hi[i]<3500){ cout<<"doer_trg ... MXQ WEST trgch="<<i<<" adc="<<VPD->vpd_trgadc_hi[i]<<" tac="<<VPD->vpd_trgtac_hi[i]<<endl; }
//			if (VPD->vpd_trgtac_hi[i+16]>200&&VPD->vpd_trgtac_hi[i+16]<3500){ cout<<"doer_trg ... MXQ EAST trgch="<<i<<" adc="<<VPD->vpd_trgadc_hi[i+16]<<" tac="<<VPD->vpd_trgtac_hi[i+16]<<endl; }
//		}
//	}

    for (int i=0;i<24;i++){
    	k	= i+1;
    	TRG->bbc_adc[i]		= trg_p->bbcADC(west,k,0);
    	TRG->bbc_tac[i]		= trg_p->bbcTDC(west,k,0);
    	TRG->bbc_adc[i+24]	= trg_p->bbcADC(east,k,0);
    	TRG->bbc_tac[i+24]	= trg_p->bbcTDC(east,k,0);
    	if (TRG->bbc_adc[i]		< 0.){ TRG->bbc_adc[i]		= 0.;}
		if (TRG->bbc_tac[i]		< 0.){ TRG->bbc_tac[i]		= 0.;}
		if (TRG->bbc_adc[i+24]	< 0.){ TRG->bbc_adc[i+24]	= 0.;}
		if (TRG->bbc_tac[i+24]	< 0.){ TRG->bbc_tac[i+24]	= 0.;}

	}

    for (int i=0;i<3;i++){
    	k	= i+1;
    	TRG->zdc_adc[i]		= trg_p->zdcADC(west,k,0);
    	TRG->zdc_tac[i]		= trg_p->zdcPmtTDC(west,k,0);
    	TRG->zdc_adc[i+3]	= trg_p->zdcADC(east,k,0);
    	TRG->zdc_tac[i+3]	= trg_p->zdcPmtTDC(east,k,0);
		//if (TRG->zdc_adc[i]>0 || TRG->zdc_adc[i+3]>0 ){
		//	cout<<TRG->zdc_adc[i]<<" "<<TRG->zdc_adc[i+3]<<endl;
		//}
	}

    for (int i=0;i<2;i++){
    	k	= i+1;
    	TRG->mtd_adc[i]		= trg_p->mtdAdc(west,k,0);
    	TRG->mtd_tac[i]		= trg_p->mtdTdc(west,k,0);
    	TRG->mtd_adc[i+2]	= trg_p->mtdAdc(east,k,0);
    	TRG->mtd_tac[i+2]	= trg_p->mtdTdc(east,k,0);

		//if (TRG->mtd_adc[i]>0 || TRG->mtd_adc[i+2]>0 ){
		//	cout<<" MTD ... "<<i<<" "<<TRG->mtd_adc[i]<<" "<<TRG->mtd_adc[i+2]<<endl;
		//}
	}

	for (int i=0;i<120;i++){
		int itray		= i+1;
		int traymult	= trg_p->tofTrayMultiplicity(itray,0);
		TRG->tofmult[i]	= traymult;
	}

    delete 	trg_p;
    trg_p	= 0;
    return 0;
}
