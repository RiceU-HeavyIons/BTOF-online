
#include <stdio.h>
#include <unistd.h>
#include <getopt.h>
#include <sys/types.h>
#include <stdlib.h>
#include <iostream>

#include <rtsLog.h>

#include "doer.h"

//int pp2pp_doer(daqReader *rdr, int StoreRaw, SVX_st *SVX, Hits_st *Hits, sili_st *sili)
int pp2pp_doer(daqReader *rdr, int StoreRaw)
{
//     daq_dta *dd;
//     int seq, ch, svx;
//     float sum, sum2; 
//     int sumn;
//     static int PedEvtCnt = 0;
//     int i, j, k;
//     
//     //memset(&SVX->Ped, 0, sizeof(SVX->Ped));
//     //memset(&SVX->Sigma, 0, sizeof(SVX->Sigma));
//     //memset(&sili, -1, sizeof(sili));
//     
// 	for (int iseq=0;iseq<8;iseq++){
// 		for (int ichain=0;ichain<4;ichain++){
// 			Hits->N[iseq][ichain]	= 0;
// 		}
// 	}
// 
//     dd = rdr->det("pp2pp")->get("adc");
//     if (dd == NULL) return -1;
// 
//     while(dd->iterate()) {
// 		pp2pp_t *d = (pp2pp_t *) dd->Void ;
// 		seq = d->seq_id - 1;
// 		ch = d->chain_id;
// 		svx = d->svx_id;
// 		if (seq == 2 && ch == 2 && svx == 7) svx = 3;	//	wrong SVXID - this is a feature
// 		if (seq < 0 || seq >= MAXSEQ || ch < 0 || ch >= MAXCHAIN || svx < 0 || svx >= MAXSVX) {
// 			LOG(WARN, "Strange SVXID:%d/%d/%d", seq, ch, svx);
// 			continue;
// 		}
// 		sum = 0;
// 		sum2 = 0;
// 		sumn = 0;
// 		for(i=0; i<PP2PP_SVX_CH; i++) switch (d->trace[i]) {
// 			case 0:
// 				sili->si[seq][ch][svx][i] = -10;
// 				break;
// 			case 1:
// 				sili->si[seq][ch][svx][i] = d->adc[i];
// 				if (i != 0 && i != 127) {	// skip the first and the last SVX channels - not connected
// 				sum += d->adc[i];
// 				sum2 += d->adc[i] * d->adc[i];
// 				sumn++;
// 				}
// 				break;
// 			default:
// 				sili->si[seq][ch][svx][i] = -10 * d->trace[i];
// 				break;
// 		}
// 		if (sumn == PP2PP_SVX_CH - 2) {
// 			PedEvtCnt++;
// 			sum /= sumn;
// 			sum2 /= sumn;
// 			sum2 = sum2 - sum*sum;
// 			SVX->Ped[seq][ch][svx] = sum;
// 			SVX->Sigma[seq][ch][svx] = sum2;
// 			if (PedEvtCnt > AVRTIME) {
// 				SVX->Thr[seq][ch][svx] = (SVX->Thr[seq][ch][svx]*(AVRTIME - 1.0) 
// 				                       +   sum + NSIGMA*sum2)/AVRTIME;
// 			} else if (PedEvtCnt == AVRTIME) {
// 				SVX->Thr[seq][ch][svx] += sum + NSIGMA*sum2;
// 				SVX->Thr[seq][ch][svx] /= AVRTIME;
// 			} else {
// 				SVX->Thr[seq][ch][svx] += sum + NSIGMA*sum2;
// 			}
// 		}
// 		// simple decoding code. Don't pass SVX boundary yet
// 		if (PedEvtCnt >= AVRTIME) {		
// 			for(i=1; i<PP2PP_SVX_CH-1; i++) {
// 				if (sili->si[seq][ch][svx][i] > SVX->Thr[seq][ch][svx]) {		// we are above threshold ?
// 					for (j=i+1; j<PP2PP_SVX_CH-1; j++) if (sili->si[seq][ch][svx][i] <= SVX->Thr[seq][ch][svx]) break;	// look forward
// 					sum = 0;
// 					sum2 = 0;
// 					for(k=i; k<j; k++) {
// 						sum += sili->si[seq][ch][svx][i] - SVX->Thr[seq][ch][svx];
// 						sum2 += (k - i) * (sili->si[seq][ch][svx][i] - SVX->Thr[seq][ch][svx]);
// 					}
// 					sum2 /= sum;
// 					sum /= k - i;
// 					k = Hits->N[seq][ch];
// 					if (k == MAXHIT) break;
// 					Hits->X[seq][ch][k] = i + sum2;
// 					Hits->Q[seq][ch][k] = sum;
// 					Hits->N[seq][ch]++;
// 					i = j - 1;
// 				}
// 			}
// 		}
//     }		//---- end dd Iterater
    return 0;
}

