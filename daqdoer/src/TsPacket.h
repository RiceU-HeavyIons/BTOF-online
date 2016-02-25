// $Id: TsPacket.h,v 1.1 2016/02/25 16:02:52 geurts Exp $
// Author: Kohei Kajimoto 2006/08/04

/*************************************************************************
 * Copyright (C) 2006-2008, Kohei Kajimoto.                              *
 * All rights reserved.                                                  *
 *************************************************************************/

#ifndef TSPACKET_H_
#define TSPACKET_H_

#include <cstdio>

using namespace std;

#include <Riostream.h>
#include <Rtypes.h>
#include <TROOT.h>
#include <TStyle.h>

//#include "TsConfig.h"
#define COARSEBIN2NS 25.0   // Coarse bin 25 nano-second per bin
#define VHRBIN2PS 24.414    // Very High resolution mode, pico-second per bin
							// 1000*25/1024 (ps/chn)
#define HRBIN2PS 97.656     // High resolution mode, pico-second per bin
							// 97.65625= 1000*100/1024  (ps/chn)

class TsPacket {

private:
  UInt_t fTSPacket;         // raw data

  // primary data
  UChar_t id;               // Packet ID
  UChar_t tdc;              // Programmed ID of TDC
  UShort_t event_id;        // Event ID
  UShort_t bunch_id;        // Bunch ID
  UShort_t word_count;      // Word count
  UChar_t  channel;         // TDC channel number LE(0-7), TE(0-31)
							// or LE(0-7), TE(0-7) for new TDIG
  UChar_t  inter;           // Interpolation factor for LE (and TE)
  UInt_t   edge_time;       // Time measurments in 100ps bins
  UShort_t error_flags;     // Error Flags

  // header trigger data (0xa)
  UChar_t  error_code;
  UChar_t  trigger;
  UChar_t  daq;
  UShort_t token;

  // header debug data (0xb)

  // geographical data (0xc)
  UChar_t tray_id;
  UChar_t half;

  // header tag word (0xdeadface)

  // separator word
  UChar_t  board_id;
  UShort_t num_items;
  UChar_t  trigger_counter;

// secondary data
  Short_t tdc_ch;

/*
 * private functions
 */
  // mapping for the big test stand with TAMP board.
  static Short_t tamp_map(Short_t tdc_ch) {
//  static Short_t cmap[] = {2, 4, 5, 1, 3, 0};
	static Short_t cmap[] = {5, 3, 0, 4, 1, 2};
	static Short_t mmap[] = {1, 0, 2, 3};
	static Short_t offset = 48;
	tdc_ch -= offset;
	if ( tdc_ch >= 0 && tdc_ch < 24) {
	  tdc_ch = 6*mmap[tdc_ch/6] + cmap[tdc_ch%6];
	}
	return (tdc_ch + offset);
  }

  // mapping for the big test stand with TINO board.
  static Short_t tino_map(Short_t tdc_ch) {
	static Short_t amap[] = {
	   0,  1,  2, 99, 99, 99,
	  99, 99, 99, 99, 99, 99,
	   3,  4,  5, 99, 99, 99,
	   6,  7,  8, 99, 99, 99
    };
	static Short_t offset = 48;
	Short_t ret = tdc_ch;
	tdc_ch -= offset;
	if ( tdc_ch >= 0 && tdc_ch < 24) {
	  ret =  amap[tdc_ch] + offset;
	}
	return ret;
  }

  // mapping for the big test stand with TAMP board.
  static Short_t tamp2_map(Short_t tdc_ch) {
//  	static Short_t offset = 48;
//  	static Short_t amap[] = {
//  		11,  7,  6,  9,  8, 10,
//  		 5,  1,  0,  3,  2,  4,
//  		17, 13, 12, 15, 14, 16,
//  		23, 19, 18, 21, 20, 22 };
//  	if (tdc_ch >= offset && tdc_ch < offset + 24)
//  		tdc_ch = amap[tdc_ch - offset] + offset;
//  	return tdc_ch;
	static Short_t cmap[] = {5, 1, 0, 3, 2, 4};
	static Short_t mmap[] = {1, 0, 2, 3};
	static Short_t offset = 48;
	tdc_ch -= offset;
	if ( tdc_ch >= 0 && tdc_ch < 24) {
	  tdc_ch = 6*mmap[tdc_ch/6] + cmap[tdc_ch%6];
	}
	return (tdc_ch + offset);
  }

  static Short_t leading(UChar_t tdc, UChar_t ch) {
	UChar_t tdig_no = tdc/4;
	UChar_t tdc_no = tdc - tdig_no;
	//cout<<(int)tdig_no<<" "<<(int)tdc_no<<" "<<(int)(8*tdc_no + ch)<<endl;
	return (8*tdc_no + ch);
  }

  static Short_t trailing(UChar_t tdc, UChar_t ch) {
	UChar_t tdig_no = tdc/4;
	UChar_t tdc_no = tdig_no;
	return (24*tdc_no + ch);
  }

  Short_t tdc_map(Short_t tdc_ch);

  void decode(UInt_t pack);

/**
 * public functions
 */
public:

  TsPacket(UInt_t pack);
  ~TsPacket();

// static TsConfig *fConfig;

//  static void SetConfig(TsConfig *conf) { fConfig = conf; };

//  static const int MODE_NON  = 0;
//  static const int MODE_TAMP = 1;
//  static const int MODE_TINO = 2;
//  static const int MODE_TAMP2 = 3;

  static const unsigned int ID_GROUP_HEADER   = 0x00;
  static const unsigned int ID_GROUP_TRAILER  = 0x01;
  static const unsigned int ID_TDC_HEADER     = 0x02;
  static const unsigned int ID_TDC_TRAILER    = 0x03;
  static const unsigned int ID_LEADING_MEAS   = 0x04;  // leading  mes.
  static const unsigned int ID_TRAILING_MEAS  = 0x05;  // trailing mes.
  static const unsigned int ID_ERRORS         = 0x06;
  static const unsigned int ID_DEBUGGING_DATA = 0x07;

  static const unsigned int ID_MULTIPLICITY   = 0x09;
  static const unsigned int ID_HEADER_TRIGGER = 0x0a;  // 10
  static const unsigned int ID_HEADER_DEBUG   = 0x0b;  // 11
  static const unsigned int ID_GEOGRAPHICAL   = 0x0c;  // 12
  static const unsigned int ID_HEADER_TAG     = 0x0d;  // 13
  static const unsigned int ID_TDIG_SEPARATOR = 0x0e;  // 14
  static const unsigned int ID_RESERVED       = 0x0f;  // 15

  UChar_t GetId()  const { return id; }
  UChar_t GetTdc() const { return (UInt_t)tdc; }

  UShort_t GetEventId() const { return event_id; }
  UShort_t GetBunchId() const { return bunch_id; }

  bool IsLE() const { return (id == ID_LEADING_MEAS); }
  bool IsTE() const { return (id == ID_TRAILING_MEAS); }
  bool IsId(UChar_t id) const { return (id == this->id); }

  static Short_t tray_map_test(UChar_t tdc, UChar_t ch) {
	int tg = tdc / 3;
	int ux = tdc % 3;

	static const int amap[] = {
		/*   0   1   2   3   4   5   6   7 */
			 6,  1,  0, 17,  5,  4, 19,  7,
		/*   8   9  10  11  12  13  14  15 */
			 8, 21, 11, 22,  9, 13, 10, 14,
		/*  16  17  18  19  20  21  22  23 */
			15,  3, 20, 12,  2, 18, 23, 16 };
	//  return (24*tg + amap[gch]);

	static const int bmap[3][8] = {
		/* ch:  0   1   2   3   4   5   6   7 */
			  { 2, 22,  9, 20,  7,  4, 11,  0}, /* U2 */
			  {18, 17,  3, 10, 15, 13,  5,  1}, /* U3 */
			  {12, 14, 19, 21,  8, 16, 23,  6}  /* U4 */
	};
//    return (24*tg + bmap[ux][channel]);

	const short vmap[3][8] = {
		/* ch:  0   1   2   3   4   5   6   7 */
			  { 2, 16,  9, 14,  7,  4, 11,  0}, /* U2 */
			  {12, 23,  3, 10, 21, 19,  5,  1}, /* U3 */
			  {18, 20, 13, 15,  8, 22, 17,  6}  /* U4 */
	};
	return (24*tg + vmap[ux][ch]);
  }

  static Short_t tray_map_v4(UChar_t tdc, UChar_t ch) {

	static const int vch[3][8] = {
	/* ch:  0   1   2   3   4   5   6   7 */
		  {20, 23, 12, 15,  7,  5, 10,  2}, /* U2 */
		  {19, 22, 17, 14,  9,  6,  4,  1}, /* U3 */
		  {21, 18, 13, 16,  8, 11,  3,  0}  /* U4 */
	};
	int tdig = tdc / 3;
	int ux   = tdc % 3;

	return (24*tdig + vch[ux][ch]);
  }

  //===========================================================
  //===========================================================
  static Short_t tray_map_v5(UChar_t tdc, UChar_t ch) {
	int tg = tdc >> 2;
	int ux = (tdc & 0x03);
	static const short vmap[3][8] = {
	  /* ch:  0   1   2   3   4   5   6   7         */
			{ 2, 16,  9, 14,  7,  4, 11,  0}, /* U2 */
			{12, 23,  3, 10, 21, 19,  5,  1}, /* U3 */
			{18, 20, 13, 15,  8, 22, 17,  6}  /* U4 */
	};
	return (24*tg + vmap[ux][ch]);
  }
  //===========================================================
  //===========================================================

  static Short_t submarine_map(UChar_t tdc, UChar_t ch);
  static Short_t upvpd_map(UChar_t half, UChar_t tdc, UChar_t ch);

  Short_t GetRawChannel() const { return channel; }
  Short_t GetChannel() const { return tdc_ch; }

  Short_t GetChannelTray4() const { return tray_map_v4(tdc, channel); }
  Short_t GetChannelTray5() const { return tray_map_v5(tdc, channel); }
  Short_t GetChannelSubmarine() const { return submarine_map(tdc, channel); }
  UInt_t  GetEdgeTime();

// Geographical data
  UChar_t GetTrayId()  const { return tray_id; }
  UChar_t GetHalf()    const { return half; }

// Separator word
  UChar_t GetBoardId()     const { return board_id; }
  UChar_t GetNumItems()    const { return num_items; }
  UChar_t GetTrigCounter() const { return trigger_counter; }

//   Int_t GetLETime(int bunch) {
// //	  int trigoffset = 0;     // trigger offset
// //    int trigoffset = 1009;  // trigger offset
// //    int trigoffset = 111;   // for pp in 2008
// 	  int trigoffset = 208;   // for 2009
// 	  int trigtime = (bunch + trigoffset)%2048;   // 2048 is a max coarse time
// 	  UInt_t hittime = GetEdgeTime() >>10;
// 	  Int_t letime = (hittime - trigtime + 2048)%2048 - 2048;
// 	  return letime; // bins of 25ns
//   }

  void Print();
};
#endif
