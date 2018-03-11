// $Id: doer_tof.cc,v 1.2 2016/02/29 21:41:36 geurts Exp $
// Author: W.J. Llope 2009/06/01

#include <stdio.h>
#include <unistd.h>
#include <getopt.h>
#include <sys/types.h>
#include <stdlib.h>
#include <iostream>

#include <rtsLog.h>
#include <TH1D.h>
#include <TH2D.h>

#include "doer.h"
#include "TsPacket.h"

#include "rocMess_wGet4v1.h"

	
int etof_doer(daqReader *rdr, struct P2P_st *P2P, struct doer_st *TOF, 
			Int_t iday, Int_t kDataChoice,
			struct stop_st *STOP, struct INFO_st *MYINFO){
	
	// rtsLogLevel( "D" );
	daq_dta *dd;
	daq_etof *etof = (daq_etof*)rdr->det("etof");
	dd = etof->get("raw");

	int iInputSizeBytes = etof->out_bytes;

	if(!dd) return -1;

	cout << "size_t = " << dd->get_size_t() << endl;
	cout << "det in bytes=" << etof->in_bytes << " out bytes = " << etof->out_bytes  << endl;

	Int_t iInputSzLg = iInputSizeBytes / sizeof( ULong64_t );
	cout << "iInputSzLg=" << iInputSzLg << endl; 

	size_t slice = 0;
	while(dd->iterate()) {
		cout << "ETOF data: " << slice << endl;
		slice++;

		cout << "void: " << dd->Void << endl;
		cout << "sizeof(void) = " << sizeof( dd->Void ) << endl;

		ULong64_t * pulLongBuff = static_cast< ULong64_t * >(dd->Void);
		ULong64_t ulTrgGdpbFullTs = pulLongBuff[0];
		ULong64_t ulTrgStarFullTs = pulLongBuff[1];
		UInt_t    uStarToken      = (pulLongBuff[2] >> 32) & 0xFFF;
		UInt_t    uStarDaqCmdIn   = (pulLongBuff[2] >> 16) & 0x00F;
		UInt_t    uStarTrigCmdIn  = (pulLongBuff[2]      ) & 0x00F;
		ULong64_t fullEventStatusFlag = pulLongBuff[3];

		cout << "ulTrgGdpbFullTs=" << ulTrgGdpbFullTs << endl;
		cout << "ulTrgStarFullTs=" << ulTrgStarFullTs << endl; 
		cout << "uStarToken=" << uStarToken << endl;
		cout << "uStarDaqCmdIn=" << uStarDaqCmdIn << endl;
		cout << "uStarTrigCmdIn=" << uStarTrigCmdIn << endl;
		cout << "fullEventStatusFlag=" << fullEventStatusFlag << endl;

		UInt_t uMsgsToRead = iInputSzLg - 4;
		for( UInt_t uMsgIdx = 0; uMsgIdx < uMsgsToRead; uMsgIdx++)
		{
			ngdpb::Message mess( pulLongBuff[4 + uMsgIdx] );
			// cout << "getGet4FineTs=" << 0+mess.getGet4FineTs() << endl;
			mess.printData();
			cout << "ADC=" << mess.getNxAdcValue() << endl;
			// fvMsgBuffer.push_back( mess );
		}

	}
	
	// rtsLogLevel( "C" );
	
}	
//---- end tof doer....




