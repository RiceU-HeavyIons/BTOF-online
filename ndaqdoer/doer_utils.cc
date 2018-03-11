#include <stdio.h>
#include <unistd.h>
#include <getopt.h>
#include <sys/types.h>
#include <stdlib.h>
#include <iostream>
using namespace std;

#include "doer.h"
#include "TsPacket.h"

unsigned int GetLETime(TsPacket *p, int bunch) {
	int trigoffset = 0;     					// trigger offset
	int trigtime = bunch%2048 + trigoffset; 	// 2048 is a max coarse time
	UInt_t hittime = p->GetEdgeTime() >>10;
	UInt_t coco = (hittime - trigtime + 2048)%2048;
	return coco; 								// bins of 25ns
}

unsigned int GetTrgDTime(TsPacket *p, int bunch) {
	float trgt	= 25.*((float)bunch);
	float time	= p->GetEdgeTime()*(25./1024.);
	float tdiff	= time - trgt;
	if (tdiff<0){ tdiff += 51200.; }
	if (tdiff<0){ tdiff += 51200.; }		//WJL not sure why i need this!
	return tdiff; 
}

int GetTHUB(int trayid){
  //
  // THUB NW
  int trayinTHUB1[30]={21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50};
  // THUB NE 
  int trayinTHUB2[30]={66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95};
  // THUB SW
  int trayinTHUB3[30]={1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,51,52,53,54,55,56,57,58,59,60};
  // THUB SE
  int trayinTHUB4[30]={61,62,63,64,65,96,97,98,99,100,101,102,103,104,105,106,107,108,109,110,111,112,113,114,115,116,117,118,119,120};
  //
  //int trayinTHUB1[30]={1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,51,52,53,54,55,56,57,58,59,60};
  //int trayinTHUB2[30]={21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50};
  //int trayinTHUB3[30]={61,62,63,64,65,96,97,98,99,100,101,102,103,104,105,106,107,108,109,110,111,112,113,114,115,116,117,118,119,120};
  //int trayinTHUB4[30]={66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95};
  //
  if(trayid<1 || trayid>122) return -1;
  int nthub = -1;
  for(int itray=0;itray<30;itray++){
    if(trayid == trayinTHUB1[itray]) nthub=0;
    if(trayid == trayinTHUB2[itray]) nthub=1;
    if(trayid == trayinTHUB3[itray]) nthub=2;
    if(trayid == trayinTHUB4[itray]) nthub=3;
  }
  //
  if(trayid == 121) nthub = 4;
  if(trayid == 122) nthub = 5;
  //
  return nthub;  
}

int GetNINO(int tdc,int chan){
  int tdcid[24]  ={0,1,0,1,0,1,2,0,2,0,1,0,1,2,0,2,0,2,2,1,2,1,2,1};
  int tdcchan[24]={7,7,0,2,5,6,7,4,4,2,3,6,0,2,3,3,1,6,0,5,1,4,5,1};
  int tinoid[24] ={2,2,2,2,2,2,1,1,1,1,2,2,0,0,1,1,1,1,0,0,0,0,0,0};
  int index=-1;
  for(int i=0;i<24;i++){
    if(tdcid[i]==tdc && tdcchan[i]==chan) {index=i;break;}
  }
  if(index<0) cout<<" tdc="<<tdc<<" chan="<<chan<<" not valid!!!"<<endl;
  return tinoid[index];
}
