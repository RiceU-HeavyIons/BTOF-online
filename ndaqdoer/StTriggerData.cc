/***************************************************************************
 *
 * $Id: StTriggerData.cc,v 1.1 2016/02/25 16:02:51 geurts Exp $
 *
 * Author: Akio Ogawa, Feb 2003
 ***************************************************************************
 *
 * Description:
 *
 ***************************************************************************
 *
 * $Log: StTriggerData.cc,v $
 * Revision 1.1  2016/02/25 16:02:51  geurts
 * First version (used for Run 15, based on Bill Llope's code for Run-14)
 *
 * Revision 2.21  2013/02/12 19:40:32  ullrich
 * Add two new methods: mxqAtSlotAddress and mtd3AtAddress (Llope).
 *
 * Revision 2.20  2012/04/30 15:19:11  ullrich
 * Added access function for l2sum (Akio)
 *
 * Revision 2.19  2012/02/01 17:00:07  ullrich
 * Fixed bug concerning seg failt when MIX DSM not in run and added new arg to MtdVpdTacDiff()
 *
 * Revision 2.18  2011/11/04 19:20:38  ullrich
 * Added tpcMaskDSM() - Akio.
 *
 * Revision 2.17  2011/02/15 18:56:08  ullrich
 * New access fct for ZDCSMD, new ZDCSMD map, spinBit() modified.
 *
 * Revision 2.16  2011/01/18 23:06:07  ullrich
 * New function mtdgemAtAddress added. vpdADC, vpdTDC, vpdADCHighThr, vpdTDCHighThr, vpdEarliestTDC, and vpdEarliestTDCHighThr updated.
 *
 * Revision 2.15  2010/06/01 22:18:44  ullrich
 * Change member debug to mDebug.
 *
 * Revision 2.14  2010/04/07 14:43:19  ullrich
 * Added new access function for BBC large tile earliest TAC and difference
 *
 * Revision 2.13  2010/01/08 22:44:37  ullrich
 * Updates needed to add StFmsCollection and related classes.
 *
 * Revision 2.12  2009/08/24 22:38:28  ullrich
 * New data member mErrorFlag and referring access fct.
 *
 * Revision 2.11  2009/06/16 15:44:26  ullrich
 * Added fmsADC() method.
 *
 * Revision 2.10  2009/05/15 18:16:15  ullrich
 * Updates for pp2pp and ToF.
 *
 * Revision 2.9  2009/05/05 20:53:16  ullrich
 * Updates for MTD.
 *
 * Revision 2.8  2009/03/19 02:46:01  ullrich
 * Add 2nd argument (pre/post) to vpdEarliestTDC().
 *
 * Revision 2.7  2009/03/04 02:01:30  ullrich
 * New access functions for ZDC DSM layer-1 and layer-2 data.
 *
 * Revision 2.6  2009/02/27 02:56:52  ullrich
 * Fixed bug in reading pre/post data.
 *
 * Revision 2.5  2009/01/14 17:54:45  ullrich
 * Modified to cope with necessary changes for 2009.
 *
 * Revision 2.4  2006/09/13 23:59:55  ullrich
 * Added new data member mRun. Removed arg run from ctb(), ctbTraySlat(), zdcSMD()
 *
 * Revision 2.3  2004/08/03 17:22:16  ullrich
 * Major update by Akio and Marco.
 *
 * Revision 2.2  2004/02/11 01:39:49  ullrich
 * Use enumeration StBeamDirector for east/west. Add member for ZDC vertex.
 *
 * Revision 2.1  2003/04/16 17:47:41  ullrich
 * Initial Revision.
 *
 **************************************************************************/
#include <stdio.h>			//WJL
#include "StTriggerData.h"
#include <rtsLog.h>			//WJL

static const char rcsid[] = "$Id: StTriggerData.cc,v 1.1 2016/02/25 16:02:51 geurts Exp $";

//WJL ClassImp(StTriggerData)

StTriggerData::StTriggerData() : mYear(0), mZdcVertexZ(-999), mRun(0), mErrorFlag(0) 
{ 
    mDebug = 0; 
}

//StTriggerData::~StTriggerData() { /* noop */}

void StTriggerData::setDebug(unsigned int val) { mDebug=val; }  

int StTriggerData::prepostAddress(int prepost) const
{ 
    if (prepost == 0) return 0;
    int npre = numberOfPreXing();
    if (prepost < 0 && -prepost <= npre) return 1+npre+prepost;
    int npost = numberOfPostXing();
    if (prepost > 0 &&  prepost <= npost) return npre+prepost;
    cout << "Wrong prepost " << prepost << " (pre=" << numberOfPreXing() << ", post=" << numberOfPostXing() << ")" << endl;
    return -1;
}

unsigned short StTriggerData::decodeEmc12bit(const int dsm, const int channel, const unsigned char *raw) const
{
    static const int dsmmap[16]={7,6,5,4,3,2,1,0,15,14,13,12,11,10,9,8};
    const unsigned char *crate_p=raw+dsm*16; // 16 bytes=128 bits per dsm
    
    if (channel%2) {  
        // channel is odd, take highest four bits + next byte
        int k=(channel-1)/2*3+1; // position of low byte
        return (crate_p[dsmmap[k+1]]<<4) + (crate_p[dsmmap[k]]>>4);
    }
    else {
        // channel is even, take lower byte + lowest 4 bits of next 
        int k=channel/2*3; // position of low byte
        return ((crate_p[dsmmap[k+1]]&0xF) << 8) + crate_p[dsmmap[k]];
    }
}

void StTriggerData::decodeQT(unsigned int ndata, unsigned int* data, unsigned short adc[16][32], unsigned char tdc[16][32])
{
    if (ndata==0) return;
    if (ndata>MaxQTData)         { printf("QT data length %d is too long (max is %d)\n",ndata,MaxQTData); return;}
    if (data[ndata-1] != 0xac10) { printf("Wrong QT data last word %x (should be 0xAC10)\n",data[ndata-1]); return;}
    int header=1;
    unsigned int crate,addr,ch,nline,oldch;
    for (unsigned int i=0; i<ndata-1; i++){
        unsigned int d = data[i];
        if (header==1){
            crate =  (d & 0xff000000) >> 24;
            addr  = ((d & 0x00ff0000) >> 16) - 0x10;
            nline =  (d & 0x000000ff);
            oldch = 0;
            if(nline>0) header=0;
            //printf("i=%3d  crate=%3d  addr=%3d  nline=%3d\n",i,crate,addr,nline);
        }
        else {
            ch = (d & 0xf8000000) >> 27;
            adc[addr][ch] = (unsigned short)  (d & 0x00000fff);
            tdc[addr][ch] = (unsigned char)  ((d & 0x0001f000) >> 12);
            //printf("i=%3d  crate=%3d  addr=%3d  nline=%3d  ch=%3d  adc=%5d  tdc=%5d\n",i,crate,addr,nline,ch,adc[addr][ch],tdc[addr][ch]);
            //if(adc[addr][ch]==0) printf("ADC = 0  problem : i=%3d  crate=%3d  addr=%3d  nline=%3d  ch=%3d  adc=%5d  tdc=%5d\n",i,crate,addr,nline,ch,adc[addr][ch],tdc[addr][ch]);
            //if(ch<=oldch)      printf("Ch Order problem : i=%3d  crate=%3d  addr=%3d  nline=%3d  ch=%3d  adc=%5d  tdc=%5d\n",i,crate,addr,nline,ch,adc[addr][ch],tdc[addr][ch]);
            oldch=ch;
            nline--;
            if (nline==0) header=1;
        }    
    }
}

//
//  Default functions. Most of them return a default value (zero). Not all 
//  of them will be overwritten by classes inheriting from StTriggerData.
//

int StTriggerData::year() const {return mYear;}
unsigned int StTriggerData::errorFlag() const {return mErrorFlag;}
unsigned int StTriggerData::eventNumber() const {return 0;}
float StTriggerData::zdcVertexZ() const {return mZdcVertexZ;}
void StTriggerData::setZdcVertexZ(float val) {mZdcVertexZ = val;}
unsigned short StTriggerData::dsmInput() const {return 0;}
unsigned short StTriggerData::trgToken() const {return 0;}
unsigned short StTriggerData::dsmAddress() const {return 0;}
unsigned short StTriggerData::mAddBits() const {return 0;}
unsigned short StTriggerData::bcData(int address) const {return 0;}
unsigned short StTriggerData::busyStatus() const {return 0;}
unsigned int StTriggerData::bunchCounterHigh() const {return 0;}
unsigned int StTriggerData::bunchCounterLow() const {return 0;}
unsigned int StTriggerData::bunchId48Bit() const {return 0;}
unsigned int StTriggerData::bunchId7Bit() const {return 0;}
unsigned int StTriggerData::spinBit() const {return 0;}
unsigned int StTriggerData::spinBitYellowFilled() const {return 0;}
unsigned int StTriggerData::spinBitYellowUp() const {return 0;}
unsigned int StTriggerData::spinBitYellowDown() const {return 0;}
unsigned int StTriggerData::spinBitYellowUnpol() const {return 0;}
unsigned int StTriggerData::spinBitBlueFilled() const {return 0;}
unsigned int StTriggerData::spinBitBlueUp() const {return 0;}
unsigned int StTriggerData::spinBitBlueDown() const {return 0;}
unsigned int StTriggerData::spinBitBlueUnpol() const {return 0;}
unsigned short StTriggerData::lastDSM(int channel) const {return 0;};
unsigned short StTriggerData::vertexDSM(int channel) const {return 0;}
unsigned short StTriggerData::ctbLayer1DSM(int channel) const {return 0;}
unsigned short StTriggerData::ctbLayer2DSM(int channel) const {return 0;}
unsigned short StTriggerData::bemcLayer1DSM(int channel, int prepost) const {return 0;}
unsigned short StTriggerData::eemcLayer1DSM(int channel, int prepost) const {return 0;}
unsigned short StTriggerData::emcLayer2DSM(int channel) const {return 0;}
unsigned short StTriggerData::tpcMaskDSM(int channel) const {return 0;}
unsigned short StTriggerData::fpdLayer1DSMRaw(StBeamDirection eastwest, int channel, int prepost) const {return 0;}
unsigned short StTriggerData::fpdLayer1DSM(StBeamDirection eastwest, int module, int board, int prepost) const {return 0;}
unsigned short StTriggerData::fpdLayer2DSMRaw(int channel) const {return 0;}
unsigned short StTriggerData::fpdLayer2DSM(StBeamDirection eastwest, int module) const {return 0;}
unsigned short StTriggerData::ctbRaw(int address, int prepost) const {return 0;}
unsigned short StTriggerData::ctb(int pmt, int prepost) const {return 0;}
unsigned short StTriggerData::ctbTraySlat(int tray, int slat, int prepost) const {return 0;}
unsigned short StTriggerData::ctbSum(int prepost) const {return 0;}
unsigned short StTriggerData::mwc(int sector, int prepost) const {return 0;}
bool StTriggerData::zdcPresent(int prepost) const {return 1;}
unsigned short StTriggerData::zdcAtChannel(int channel, int prepost) const {return 0;}
unsigned short StTriggerData::zdcAtAddress(int address, int prepost) const {return 0;}
unsigned short StTriggerData::zdcUnAttenuated(StBeamDirection eastwest, int prepost) const {return 0;}
unsigned short StTriggerData::zdcAttenuated(StBeamDirection eastwest, int prepost) const {return 0;}
unsigned short StTriggerData::zdcADC(StBeamDirection eastwest, int pmt, int prepost) const {return 0;}
unsigned short StTriggerData::zdcTDC(StBeamDirection eastwest, int prepost) const {return 0;}
unsigned short StTriggerData::zdcPmtTDC(StBeamDirection eastwest, int pmt, int prepost) const {return 0;}
unsigned short StTriggerData::zdcHardwareSum(int prepost) const {return 0;}
unsigned short StTriggerData::zdcEarliestTDC(StBeamDirection eastwest, int prepost) const {return 0;}
bool StTriggerData::zdcSumADCaboveThreshold(StBeamDirection eastwest, int prepost) const {return 0;}
bool StTriggerData::zdcFrontADCaboveThreshold(StBeamDirection eastwest, int prepost) const {return 0;}
bool StTriggerData::zdcBackADCaboveThreshold(StBeamDirection eastwest, int prepost) const {return 0;}
bool StTriggerData::zdcSumADCaboveThresholdL2(StBeamDirection eastwest) const {return 0;}
bool StTriggerData::zdcFrontADCaboveThresholdL2(StBeamDirection eastwest) const {return 0;}
bool StTriggerData::zdcBackADCaboveThresholdL2(StBeamDirection eastwest) const {return 0;}
unsigned short StTriggerData::zdcTimeDifference() const {return 0;}
bool StTriggerData::zdcSumADCaboveThresholdL3(StBeamDirection eastwest) const {return 0;}
bool StTriggerData::zdcFrontADCaboveThresholdL3(StBeamDirection eastwest) const {return 0;}
bool StTriggerData::zdcBackADCaboveThresholdL3(StBeamDirection eastwest) const {return 0;}
bool StTriggerData::zdcTimeDifferenceInWindow() const {return 0;}
bool StTriggerData::zdcSMDPresent(int prepost) const {return 1;}
unsigned short StTriggerData::zdcTruncatedSum(StBeamDirection eastwest, int prepost) const {return 0;}
unsigned short StTriggerData::zdcSMDHighestStrip(StBeamDirection eastwest, int verthori, int prepost) const {return 0;}
unsigned short StTriggerData::zdcSMD(StBeamDirection eastwest, int verthori, int strip, int prepost) const {return 0;}
unsigned char  StTriggerData::bemcHighTower(int patch_id, int prepost) const {return 0;}
unsigned char  StTriggerData::bemcJetPatch (int patch_id, int prepost) const {return 0;}
unsigned char  StTriggerData::eemcHighTower(int patch_id, int prepost) const {return 0;}
unsigned char  StTriggerData::eemcJetPatch (int patch_id, int prepost) const {return 0;}
unsigned char  StTriggerData::bemcHighestTowerADC(int prepost) const {return 0;}
unsigned char  StTriggerData::eemcHighestTowerADC(int prepost) const {return 0;}
unsigned short StTriggerData::bbcADC(StBeamDirection eastwest, int pmt, int prepost) const {return 0;}
unsigned short StTriggerData::bbcTDC(StBeamDirection eastwest, int pmt, int prepost) const {return 0;}
unsigned short StTriggerData::bbcADCSum(StBeamDirection eastwest, int prepost) const {return 0;}
unsigned short StTriggerData::bbcADCSumLargeTile(StBeamDirection eastwest, int prepost) const {return 0;}
unsigned short StTriggerData::bbcEarliestTDC(StBeamDirection eastwest, int prepost) const {return 0;}
unsigned short StTriggerData::bbcTimeDifference() const {return 0;}
unsigned short StTriggerData::bbcEarliestTDCLarge(StBeamDirection eastwest, int prepost) const {return 0;}
unsigned short StTriggerData::bbcTimeDifferenceLarge() const {return 0;}
unsigned short StTriggerData::fpd(StBeamDirection eastwest, int module, int pmt, int prepost) const {return 0;} 
unsigned short StTriggerData::fpdSum(StBeamDirection eastwest, int module) const {return 0;}
unsigned short StTriggerData::nQTdata(int prepost) const {return 0;}
unsigned int*  StTriggerData::QTdata(int prepost) const {return 0;}
unsigned short StTriggerData::vpdADC(StBeamDirection eastwest, int pmt, int prepost) const {return 0;}
unsigned short StTriggerData::vpdTDC(StBeamDirection eastwest, int pmt, int prepost) const {return 0;}
unsigned short StTriggerData::vpdADCHighThr(StBeamDirection eastwest, int pmt, int prepost) const {return 0;}
unsigned short StTriggerData::vpdTDCHighThr(StBeamDirection eastwest, int pmt, int prepost) const {return 0;}
unsigned short StTriggerData::vpdEarliestTDC(StBeamDirection eastwest, int prepost) const {return 0;}
unsigned short StTriggerData::vpdEarliestTDCHighThr(StBeamDirection eastwest, int prepost) const {return 0;}
unsigned short StTriggerData::vpdTimeDifference() const {return 0;}
unsigned short StTriggerData::mxqAtSlotAddress(int address, int prepost, int slot) const {return 0;}
unsigned short StTriggerData::mtdAtAddress(int address, int prepost) const {return 0;}
unsigned short StTriggerData::mtdgemAtAddress(int address, int prepost) const {return 0;}
unsigned short StTriggerData::mtd3AtAddress(int address, int prepost) const {return 0;}
unsigned short StTriggerData::mtdAdc(StBeamDirection eastwest, int pmt, int prepost) const {return 0;}
unsigned short StTriggerData::mtdTdc(StBeamDirection eastwest, int pmt, int prepost) const {return 0;}
unsigned char  StTriggerData::mtdDsmAtCh(int ch, int prepost) const {return 0;}
bool           StTriggerData::mtdDsmHit(int pmt, int prepost) const {return 0;}
unsigned short StTriggerData::mtdVpdTacDiff() const {return 0;}
unsigned short StTriggerData::tofAtAddress(int address, int prepost) const {return 0;}
unsigned short StTriggerData::tofTrayMultiplicity(int tray, int prepost) const {return 0;}
unsigned short StTriggerData::tofMultiplicity(int prepost) const {return 0;}
unsigned short StTriggerData::pp2ppADC(StBeamDirection eastwest, int vh, int udio, int ch, int prepost) const {return 0;}
unsigned short StTriggerData::pp2ppTAC(StBeamDirection eastwest, int vh, int udio, int ch, int prepost) const {return 0;}
unsigned long  StTriggerData::pp2ppDSM(int prepost) const {return 0;}
unsigned short StTriggerData::fmsADC(int crt, int adr, int ch, int prepost) const {return 0;}
unsigned short StTriggerData::fmsTDC(int crt, int adr, int ch, int prepost) const {return 0;}
unsigned char* StTriggerData::getDsm_FMS(int prepost) const {return 0;}
unsigned char* StTriggerData::getDsm01_FMS(int prepost) const {return 0;}
unsigned char* StTriggerData::getDsm02_FMS(int prepost) const {return 0;}
unsigned short* StTriggerData::getDsm1_FMS(int prepost) const {return 0;}
unsigned short* StTriggerData::getDsm2_FMS() const {return 0;}
int StTriggerData::L2ResultsOffset(StL2AlgorithmId id) const {return -1;}  
bool StTriggerData::isL2Triggered(StL2TriggerResultType id) const {return false;}  
unsigned long long StTriggerData::l2sum() const {return 0;}  
void StTriggerData::killFMS() {return;}
