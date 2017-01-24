// Kohei's Decoder
// modified by Llope for pp2pp and other improvements....

// $Id: TsPacket.cxx,v 1.3 2009/02/06 00:47:27 tofp Exp $
// Author: Kohei Kajimoto 2006/08/04

/*************************************************************************
 * Copyright (C) 2006-2008, Kohei Kajimoto.                              *
 * All rights reserved.                                                  *
 *************************************************************************/

#include "TsPacket.h"

//TsConfig *TsPacket::fConfig = NULL;

/**
 * public functions
 */

TsPacket::TsPacket(UInt_t pack)
{
  decode(pack);
}

TsPacket::~TsPacket()
{
  // do nothing
}

UInt_t TsPacket::GetEdgeTime() {

  UInt_t r = 0;

  if (id == ID_LEADING_MEAS) {
    r = (edge_time << 2) + inter;
    return r;
  }

  if (id == ID_TRAILING_MEAS) {
    r = (edge_time << 2) + inter;		// TRAYS ONLY
//	r = edge_time;
    return r;
  }

  return r;
}

void TsPacket::Print() {
  cout << hex;
  cout << "TSPacket: ";
  cout << fTSPacket;
  cout << " id: ";
  cout.fill('0');
  cout.width(2);
  cout << (short)id;

  if (id == ID_GROUP_HEADER) {
  } else if (id == ID_GROUP_TRAILER) {
  } else if (id == ID_TDC_HEADER) {
  } else if (id == ID_TDC_TRAILER) {
  } else if (id == ID_LEADING_MEAS) {
    cout << " tdc: ";
    cout.width(2);
    cout << (short)tdc;
    cout << " ch: ";
    cout.width(2);
    cout << (short)channel;
    cout << " val: ";
    cout.width(6);
    cout << edge_time;
    cout << "(";
    cout.fill('0');
    cout.width(2);
    cout << (short)inter;
    cout << ")";

  } else if (id == ID_TRAILING_MEAS) {
    cout << " tdc: " << setw(2) << (short)tdc;
    cout << " ch: " << setw(2) << (short)channel;
    cout << " val: " << setw(6) << edge_time;

  } else if (id == ID_ERRORS) {
    cout << " error flags: " << error_flags;

  } else if (id == ID_HEADER_TRIGGER) {
    cout << " trigger: " << (int)trigger;
    cout << " daq: " << (int)daq;
    cout << " token: " << (int)token;

  } else if (id == ID_HEADER_DEBUG) {
    cout << " header debug data: ";
    cout << fTSPacket;

  } else if (id == ID_GEOGRAPHICAL) {
    cout << " tray_id: " << (short) tray_id;
    cout << " half: " << (short) half;

  } else if (id == ID_TDIG_SEPARATOR) {
    cout << " board id: " << (short)board_id;
    cout << " number of items: " << (short)num_items;
    cout << " trigger counter: " << (short)trigger_counter;
  } else {
    cout << " unknown id";
  }
    cout << endl;
}

/*
 * private functions
 */
void TsPacket::decode(UInt_t pack) {

  fTSPacket = pack;                               // save raw packet
  id = (fTSPacket & 0xF0000000) >> 28;            // extract id

  if ( id == ID_GROUP_HEADER) {

  } else if ( id == ID_GROUP_TRAILER) {

  } else if ( id == ID_TDC_HEADER) {
    event_id = (fTSPacket & 0x000FFF000) >> 12;
    bunch_id = (fTSPacket & 0x000000FFF);

  } else if ( id == ID_TDC_TRAILER) {
    event_id = (fTSPacket & 0x000FFF000) >> 12;
    bunch_id = (fTSPacket & 0x000000FFF);
    
  } else if ( id == ID_LEADING_MEAS ) {            //  leading edge
    tdc       = (fTSPacket & 0x0F000000) >> 24;
    channel   = (fTSPacket & 0x00E00000) >> 21;
    inter     = (fTSPacket & 0x00180000) >> 19;
    edge_time = (fTSPacket & 0x0007FFFF);
    tdc_ch = tdc_map( leading(tdc, channel) );

  } else if ( id == ID_TRAILING_MEAS ) {           // trailing edge
//      tdc       = (fTSPacket & 0x0F000000) >> 24;
//      channel   = (fTSPacket & 0x00F80000) >> 19;
//      edge_time = (fTSPacket & 0x0007FFFF);
//      inter     = 0;
//      tdc_ch = tdc_map( trailing(tdc, channel) );
    tdc       = (fTSPacket & 0x0F000000) >> 24;
    channel   = (fTSPacket & 0x00E00000) >> 21;
    inter     = (fTSPacket & 0x00180000) >> 19;
    edge_time = (fTSPacket & 0x0007FFFF);
    tdc_ch = tdc_map( leading(tdc, channel) );

  } else if ( id == ID_ERRORS ) {
    tdc         = (fTSPacket & 0x0F000000) >> 24;
    error_flags = (fTSPacket & 0x00007FFF);

  } else if ( id == ID_HEADER_TRIGGER ) {           // header trigger data
    trigger  = (fTSPacket & 0x000F0000) >> 16;
    daq      = (fTSPacket & 0x0000F000) >> 12;
    token    = (fTSPacket & 0x00000FFF);

    tdc_ch = -1;

  } else if ( id == ID_HEADER_DEBUG ) {           // header debug data

  } else if ( id == ID_TDIG_SEPARATOR ) {         // TDIG separator word
    board_id        = (fTSPacket & 0x0F000000) >> 24;
    num_items       = (fTSPacket & 0x0000FF00) >> 8;
    trigger_counter = (fTSPacket & 0x000000FF);
    
  } else if (id == ID_GEOGRAPHICAL) {
    half    = (fTSPacket & 0x00000001);
    tray_id = (fTSPacket & 0x000000FE) >> 1;

  } else {                        // unknown TSPacket
    tdc_ch = -1;
  }
}

Short_t TsPacket::tdc_map(Short_t tdc_ch) {
/*
  int mode = TsPacket::fConfig->fMap;
  if(mode == TsConfig::MAP_TINO) {
    return tino_map(tdc_ch);
  }

  if(mode == TsConfig::MAP_TAMP) {
    return tamp_map(tdc_ch);
  }

  if(mode == TsConfig::MAP_TAM2) {
    return tamp2_map(tdc_ch);
  }

//  if(mode == TsConfig::MAP_TRAY4) {
//      return tray_map_v4(tdc, ch);
//  }
*/
  return tdc_ch;
}

Short_t TsPacket::submarine_map(UChar_t tdc, UChar_t ch) {
  UShort_t tg = tdc >> 2;
  UShort_t ux = (tdc & 0x03);

  const UShort_t vmap[3][8] = {
/* ch:  0   1   2   3   4   5   6   7 */
      { 3, 13,  8, 15, 10,  1,  6,  5}, /* U2 */
      {17, 18,  2,  7, 20, 22,  0,  4}, /* U3 */
      {23, 21, 16, 14,  9, 19, 12, 11}  /* U4 */
  };

  return (24*tg + vmap[ux][ch]);
}


Short_t TsPacket::upvpd_map(int trayid, UChar_t half, UChar_t tdc, UChar_t ch) {
  Short_t	result;
  UShort_t	tg;

//   tg 	= 8*half + tdc;
//   const Short_t vmap[15][8] = {
// /* ch:  0   1   2   3   4   5   6   7 */
//       {24,  0,  8, 19,  0, 26,  0,  0}, // 85-0
//       { 0, -7,  0, -8,-19,-18,-26,-24}, // 85-1
//       {18,  0,  0,  0,  0,  0,  7,  0}, // 85-2
// 
//       { 0,  0,  0,  0,  0,  0,  0,  0},
// 
//       {11,  0,  6, 17,  0, 25,  0,  0}, // 79-0
//       { 0, -5,  0, -6,-17,-16,-25,-11}, // 79-1
//       {16,  0,  0,  0,  0,  0,  5,  0}, // 79-2
// 
//       { 0,  0,  0,  0,  0,  0,  0,  0},
// 
//       {27,  0,  4, 15,  0, 10,  0,  0}, // 80-0
//       { 0, -3,  0, -4,-15, -9,-10,-27}, // 80-1
//       { 9,  0,  0,  0,  0,  0,  3,  0}, // 80-2
// 
//       { 0,  0,  0,  0,  0,  0,  0,  0},
// 
//       {14,  0,  2, 13,  0,  0,  0,  0}, // 83-0
//       { 0, -1,  0, -2,-13,-12,  0,-14}, // 83-1
//       {12,  0,  0,  0,  0,  0,  1,  0}  // 83-2
//   };

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
//  const Short_t vmap[19][8] = {					//east and west are symmetric.
// /*      ch:  0   1   2   3   4   5   6   7 */     
//            {11,  0,  0,  0,  0,  1,  0,  0},    // 85/89-0		tg= 0
// 	       { 0,  0,  0,  0,  3,  0,  0,  0},    // 85/89-1		tg= 1
// 	       { 0,  0,  0,  0,  0,  0,  2,  0},    // 85/89-2		tg= 2
// 	       { 0,  0,  0,  0,  0,  0,  0,  0},	//				tg= 3
// 	       {12,  0,  0,  0,  0,  4,  0,  0},    // 79/91-0		tg= 4
// 	       { 0,  0,  0,  0,  6,  0,  0,  0},    // 79/91-1		tg= 5
// 	       { 0,  0,  0,  0,  0,  0,  5,  0},    // 79/91-2		tg= 6
// 	       { 0,  0,  0,  0,  0,  0,  0,  0},	//				tg= 7
// 	       {18,  0,  0,  0,  0,  7,  0,  0},    // 1101/1102-0	tg= 8
// 	       { 0,  0,  0,  0,  9,  0,  0,  0},    // 1101/1102-1	tg= 9
// 	       { 0,  0,  0,  0,  0,  0,  8,  0},    // 1101/1102-2	tg=10
// 	       { 0,  0,  0,  0,  0,  0,  0,  0},	//				tg=11
// 	       {19,  0,  0,  0,  0, 10,  0,  0},    // 80/885-0		tg=12
// 	       { 0,  0,  0,  0, 14,  0,  0,  0},    // 80/885-1		tg=13
// 	       { 0,  0,  0,  0,  0,  0, 13,  0},    // 80/885-2		tg=14
// 	       { 0,  0,  0,  0,  0,  0,  0,  0},	//				tg=15
// 	       { 0,  0,  0,  0,  0, 15,  0,  0},    // 83/82-0		tg=16
// 	       { 0,  0,  0,  0, 17,  0,  0,  0},    // 83/82-1		tg=17
// 	       { 0,  0,  0,  0,  0,  0, 16,  0}     // 83/82-2		tg=18
//  };

//.....................................................................
//..... RUN 14 (after changing two bad TPMD channels)
//.....................................................................
// const Short_t vmap[19][8] = {					//east and west are symmetric.
///*      ch:  0   1   2   3   4   5   6   7 */     
//           {11,  0,  0,  0,  0,  1,  0,  0},    // 85/89-0		tg= 0
//	       { 0,  0,  0,  0,  3,  0,  0,  0},    // 85/89-1		tg= 1
//	       { 0,  0,  0,  0,  0,  0,  2,  0},    // 85/89-2		tg= 2
//	       { 0,  0,  0,  0,  0,  0,  0,  0},	//				tg= 3
//	       {12,  0,  0,  0,  0,  4,  0,  0},    // 79/91-0		tg= 4
//	       { 0,  0,  0,  0,  6,  0,  0,  0},    // 79/91-1		tg= 5
//	       { 0,  0,  0,  0,  0,  0,  5,  0},    // 79/91-2		tg= 6
//	       { 0,  0,  0,  0,  0,  0,  0,  0},	//				tg= 7
//	       {18,  0,  0,  0,  0,  7,  0,  0},    // 1101/1102-0	tg= 8
//	       { 0,  0,  0,  0,  9,  0,  0,  0},    // 1101/1102-1	tg= 9
//	       { 0,  0,  0,  0,  0,  0,  8,  0},    // 1101/1102-2	tg=10
//	       { 0,  0,  0,  0,  0,  0,  0,  0},	//				tg=11
//	       {19,  0,  0,  0,  0, 10,  0,  0},    // 80/885-0		tg=12
//	       { 0,  0,  0,  0, 14,  0,  0,  0},    // 80/885-1		tg=13
//	       { 0,  0,  0,  0,  0,  0, 13,  0},    // 80/885-2		tg=14
//	       { 0,  0,  0,  0,  0,  0,  0,  0},	//				tg=15
//	       { 0,  0,  0,  0,  0, 15,  0,  0},    // 83/82-0		tg=16
//	       { 0,  0,  0,  0,  0, 17,  0,  0},    // 83/82-1		tg=17	changed J-25 (ch4) to J-27 (ch5)
//	       { 0,  0,  0, 16,  0,  0,  0,  0}     // 83/82-2		tg=18	changed J-17 (ch6) to J-19 (ch3)
// ..... RUN 15: changed back again
//	       { 0,  0,  0,  0, 17, 0,  0,  0},    // 83/82-1		tg=17	changed J-25 (ch4) to J-27 (ch5)
//	       { 0,  0,  0, 0,  0,  0,  16,  0}     // 83/82-2		tg=18	changed J-17 (ch6) to J-19 (ch3)
// };
 const Short_t vmapEast[19][8] = {					//east and west are symmetric.
/*      ch:  0   1   2   3   4   5   6   7 */     
           {11,  0,  0,  0,  0,  1,  0,  0},    // 85/89-0		tg= 0
	       { 0,  0,  0,  0,  3,  0,  0,  0},    // 85/89-1		tg= 1
	       { 0,  0,  0,  0,  0,  0,  2,  0},    // 85/89-2		tg= 2
	       { 0,  0,  0,  0,  0,  0,  0,  0},	//				tg= 3
	       {12,  0,  0,  0,  0,  4,  0,  0},    // 79/91-0		tg= 4
	       { 0,  0,  0,  0,  6,  0,  0,  0},    // 79/91-1		tg= 5
	       { 0,  0,  0,  0,  0,  0,  5,  0},    // 79/91-2		tg= 6
	       { 0,  0,  0,  0,  0,  0,  0,  0},	//				tg= 7
	       { 0,  0,  0,  0,  0,  7,  0,  18},    // 1101/1102-0	tg= 8
	       { 0,  0,  0,  0,  9,  0,  0,  0},    // 1101/1102-1	tg= 9
	       { 0,  0,  0,  0,  0,  0,  8,  0},    // 1101/1102-2	tg=10
	       { 0,  0,  0,  0,  0,  0,  0,  0},	//				tg=11
	       {19,  0,  0,  0,  0, 10,  0,  0},    // 80/885-0		tg=12
	       { 0,  0,  0,  0, 14,  0,  0,  0},    // 80/885-1		tg=13
	       { 0,  0,  0,  0,  0,  0, 13,  0},    // 80/885-2		tg=14
	       { 0,  0,  0,  0,  0,  0,  0,  0},	//				tg=15
	       { 0,  0,  0,  0,  0, 15,  0,  0},    // 83/82-0		tg=16
	       { 0,  0,  0,  0, 17, 0,  0,  0},    // 83/82-1		tg=17	changed J-25 (ch4) to J-27 (ch5)
	       { 0,  0,  0, 0,  0,  0,  16,  0}     // 83/82-2		tg=18	changed J-17 (ch6) to J-19 (ch3)
 };
 const Short_t vmapWest[19][8] = {					//east and west are symmetric.
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
	       { 0,  0,  0,  0, 17, 0,  0,  0},    // 83/82-1		tg=17	changed J-25 (ch4) to J-27 (ch5)
	       { 0,  0,  0, 0,  0,  0,  16,  0}     // 83/82-2		tg=18	changed J-17 (ch6) to J-19 (ch3)
 };



if (trayid == 121) {
  result	= vmapWest[tg][ch];
	
} else if (trayid == 122) {
  result	= vmapEast[tg][ch];
	
} else{
	cout << "UNKNOWN vpd " << trayid << endl;
}
  
//  if (result==0){
//  	cout<<"XXXXXXXXXXXXXXXXXXXXXXXXXX \a "<<half<<" "<<tdc<<" "<<tg<<" "<<ch<<endl;
//  }

//  cout<<(int)half<<" "<<(int)tg<<" "<<(int)ch<<" "<<result<<endl;

  if (result!=0) {
     //cout<<" KNOWN   ... "<<(int)half<<"  "<<(int)tdc<<"   tg="<<(int)tg<<"   ch="<<(int)ch<<"   "
     //	 <<result<<"     "<<endl;
  } else {
     cout<<" UNKNOWN ... "<<(int)half<<"  "<<(int)tdc<<"   tg="<<(int)tg<<"   ch="<<(int)ch<<"   "
     	 <<result<<"     "<<endl;
  }
  return result;

}






