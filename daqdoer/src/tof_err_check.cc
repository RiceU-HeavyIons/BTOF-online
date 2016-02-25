#include <iostream>
using namespace std;

//---- prototypes...
bool ValidDataword(int pid);
int FindTray(int itr, int ith);

//---- main routine...
int tof_err_check(unsigned int data[], int nbytes, int rdo1){

	int ierror	= 0;
	int thub	= rdo1 - 1;
	int nwords	= nbytes/4;
	//
	int packetid=0,bunch=0,half=0,tray=0;
	int bunchids_tof[2][30];
	int bunchids_vpd[2];
	for (int ih=0;ih<2;ih++){
		bunchids_vpd[ih] = -9999; 
		for (int it=0;it<30;it++){ bunchids_tof[ih][it] = -9999; }
	}
	int errormask_tof[30]	= {0};
	int errormask_vpd		= {0};
	
	const int nmasked			= 2;
	int masked_thub[nmasked]	= { 1, 3};	// mask trays 95 and 102....
	int masked_index[nmasked]	= {29,11};
	
	//---- loop over datawords....
	//
	for (int i=0;i<nwords;i++){
		//
		packetid = (data[i]&0xF0000000)>>28;
		if ( !ValidDataword(packetid) ){ 
			return 1;
		}
		//
		if ( packetid == 0xD ) continue;  
		if ( packetid == 0xE ) continue;  
		if ( packetid == 0xA ) continue;	// trigger header
		if ( packetid == 0x4 ) continue;	// LE
		if ( packetid == 0x5 ) continue;	// TE
		if ( packetid == 0x6 ) continue;
		//
		if ( packetid == 0xC ){				// geographical
			half	=  data[i]&0x01;    				// [0,1]
			tray	= (data[i]&0x0FE)>>1;				// [1,122]
			continue;
		}
		//
		if ( packetid == 0x2 ){
			bunch 	= data[i]&0xFFF;
			if (tray<=120){ 
				int index	= FindTray(tray,thub);		// [0,29]
				bunchids_tof[half][index]	= bunch; 
			} else { 
				bunchids_vpd[half]			= bunch; 
			} 
		}
		//
		//cout<<hex<<packetid<<dec<<" "<<tray<<" "<<half<<" "<<thub<<" "<<endl;
		//
	}
	
	//---- check bunchids...
	//
	for (int it=0;it<30;it++){
		//
		bool masked		= false;
		for (int im=0;im<nmasked;im++){
			if (thub==masked_thub[im] && it==masked_index[im]){
				masked	= true;
			}
		}
		if (masked){ continue; }
		//
		for (int ih=0;ih<2;ih++){
			if (bunchids_tof[ih][it] < -9990){ 
				ierror = 3; 
			}
		}
 		if (ierror){ 
 			cout<<"\t tof missing   thub="<<thub<<" index="<<it<<" "<<ierror<<endl;
 		}
		if ((bunchids_tof[0][0] != bunchids_tof[0][it])
		 || (bunchids_tof[1][0] != bunchids_tof[1][it])){ 
			cout<<"\t tof mismatch  thub="<<thub<<" index="<<it<<" "<<ierror<<endl;		
			ierror	= 2;
		}
		errormask_tof[it]	= ierror;
	}
	//
	if (thub==0||thub==1){
		for (int ih=0;ih<2;ih++){
			if (bunchids_vpd[ih] < -9990){ 
				ierror = 3; 
			}
		}
 		if (ierror){ 
 			cout<<"\t vpd missing   thub="<<thub<<" "<<ierror<<endl;
 		}
		if ((bunchids_tof[0][0] != bunchids_vpd[0])
		 || (bunchids_tof[1][0] != bunchids_vpd[1])){ 
			ierror	= 2;
		}
		errormask_vpd	= ierror;
	}

	return ierror; 	
}

//----------------------------------------------
bool ValidDataword(int pid){
  if(pid == 0x2) return true;
  if(pid == 0xD) return true;
  if(pid == 0xE) return true;
  if(pid == 0xA) return true;
  if(pid == 0xC) return true;
  if(pid == 0x4) return true;
  if(pid == 0x5) return true;
  return false;
}

//----------------------------------------------
int FindTray(int itr, int ith){
	int trayinTHUB[4][30] ={ {21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50},
       {66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95},
       {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,51,52,53,54,55,56,57,58,59,60},
       {61,62,63,64,65,96,97,98,99,100,101,102,103,104,105,106,107,108,109,110,111,112,113,114,115,116,117,118,119,120} };
	int ilocal	= -1;
	for(int itray=0;itray<30;itray++){
		if( itr == trayinTHUB[ith][itray] ) { ilocal = itray; break; }
	}
	if (ilocal<0 || ilocal>29){ 
		cout<<"\t"<<"tray="<<itr<<" thub="<<ith<<" index="<<ilocal<<endl; 
	}
	return ilocal;
}