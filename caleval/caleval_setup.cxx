#include "caleval_class.h"

void caleval::Setup() 
{

	cout<<"..... Setup ..................................... "<<endl;

	NDETSIDE	= 19;
	NITERATIONS	= 4*19 + 1;		// 20, 39, 58, 77
//	NITERATIONS	= 19;			// for testing!
	//NPASSES	=  1 + (NITERATIONS/NDETSIDE);
	
	NREQUIRE	= 2;	// MONEY.  <0 for ==, >0 for >=

	dT_nbin		=   100;
	dT_min		= -8.00;	// -8.00
	dT_max		=  8.00;	//  8.00
	dT_min2		= -1.25;
	dT_max2		=  1.25;
	rangeDt_0	= 12.50;
	rangeDt_1	=  1.25;	

	Zerrorcut	=  0.25;

	zvtoffset	=  0.00; 
	zvtoffsetb	=  0.00; 
	Zdiffcut	= 40.00; 
	Zdiffcutb	= 12.00; 
	Nprimarycut	= 10;

	//----- VPD TOF-ToT and TOF-LE
	if (iChoice==0){
		cout<<"..... Setup    Using VPD TOF-ToT and TOF-LE, iTrgCrate="<<iTrgCrate<<endl;
		A 			= &vpd_tot[0];
		if (iTrgCrate==0){
			Aother 	= &vpd_adc[0];
		} else if (iTrgCrate==1){
			Aother 	= &mvpd_adc[0];
		}
		T 			= &vpd_tle[0];
		A_nbin		= 60;
		if (iRhicRun==9){
			A_min		= 0.0;			// RUN-9
			A_max		= 60.0;			//
		} else if (iRhicRun>=10){
			A_min		= 9.0;			// RUN-10
			A_max		= 69.0;			//
		}
		T_min		= 0.0;
		T_max		= 999999.0;
		TACtoNS		= 1.0;
		//
//		zvtoffset	=  0.0;
// 		if (iEnergy==200){
// 			zvtoffset	=  6.00; 		// only used in later passes in choice=2 runs
// 		} else if (iEnergy==500){
// 			zvtoffset	=  4.00; 		// only used in later passes in choice=2 runs
// 		}
// 		if (iEnergy==200){
// 			Zdiffcut	= 40.00; 
// 			Zdiffcutb	= 12.00; 		// only used in later passes in choice=2 runs 
// 		} else if (iEnergy==39){
// 			Zdiffcut	= 40.00; 
// 			Zdiffcutb	= 12.00; 		// only used in later passes in choice=2 runs 
// 			//Zdiffcut	= 20.00; 
// 			//Zdiffcutb	= 06.00; 		// only used in later passes in choice=2 runs 
// 		}
	//
	//----- VPD TRG-ADC and TRG-TAC
	} else if (iChoice==1){
		cout<<"..... Setup    Using VPD TRG-ADC and TRG-TAC, iTrgCrate="<<iTrgCrate<<endl;
		if (iTrgCrate==0){
			A 			= &vpd_adc[0];
			T 			= &vpd_tac[0];
		} else if (iTrgCrate==1){
			A 			= &mvpd_adc[0];
			T 			= &mvpd_tac[0];
		}
		Aother 		= &vpd_tot[0];
		A_nbin		=   64;
		A_min		=    0.0;
		A_max		= 3584.0;
		T_min		= 100.0;
		T_max		= 3950.0;
		TACtoNS		= 0.01773;		//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		//
// 		zvtoffset	=  40.0;
// 		zvtoffsetb	=  36.0;
// 		Zdiffcutb	= 15.00;  		// only used in later passes in choice=2 runs
	//
	//----- VPD TRG-ADC and TOF-LE
	} else if (iChoice==2){
		cout<<"..... Setup    Using VPD TRG-ADC and TOF-LE, iTrgCrate="<<iTrgCrate<<endl;
		if (iTrgCrate==0){
			A 		= &vpd_adc[0];
		} else if (iTrgCrate==1){
			A 		= &mvpd_adc[0];
		}
		T 			= &vpd_tle[0];
		Aother 		= &vpd_tot[0];
		A_nbin		=   64;
		A_min		=    0.0;
		A_max		= 3584.0;
		T_min		= 0.0;
		T_max		= 999999.0;
		TACtoNS		= 1.0;
		//
// 		zvtoffset	=  0.0;
// //		zvtoffset	=  8.00; 		// only used in later passes in choice=2 runs
// 		Zdiffcutb	= 12.00;  		// only used in later passes in choice=2 runs
	//
	//----- VPD TOF-ToT and TRG-TAC
	} else if (iChoice==3){
		cout<<"..... Setup    Using VPD TOF-ToT and TRG-TAC, iTrgCrate="<<iTrgCrate<<endl;
		A 			= &vpd_tot[0];
		if (iTrgCrate==0){
			Aother 	= &vpd_adc[0];
			T 		= &vpd_tac[0];
		} else if (iTrgCrate==1){
			Aother 	= &mvpd_adc[0];
			T 		= &mvpd_tac[0];
		}
		A_nbin		= 60;
		if (iRhicRun==9){
			A_min		= 0.0;			// RUN-9
			A_max		= 60.0;			//
		} else if (iRhicRun>=10){
			A_min		= 0.0;			// RUN-10
			A_max		= 60.0;			//
		}
		T_min		= 100.0;
		T_max		= 3950.0;
		TACtoNS		= 0.01773;		//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		//
// 		zvtoffset	=  40.0;
// 		zvtoffsetb	=  36.0;
// 		Zdiffcutb	= 15.00; 
	//
	//----- BBC TRG-ADC and TRG-TAC
	} else if (iChoice==5){
		NDETSIDE	= 24;
		NITERATIONS	= 1;
		cout<<"..... Setup    Using BBC TRG-ADC and TRG-TAC"<<endl;
		A 			= &bbc_adc[0];
		Aother 		= 0;
		T 			= &bbc_tac[0];
		A_nbin		=   60;
		A_min		=   15.0;
		A_max		= 3915.0;
		T_min		=  800.0;
		T_max		= 3000.0;
		TACtoNS		= 0.017;
		//
		Zerrorcut	=  0.15;
		zvtoffset	=  0.00; 
		Zdiffcut	= 40.00; 
		Zdiffcutb	= 20.00; 
		//
		dT_min		= -12.00;
		dT_max		=  12.00;
		dT_min2		= -2.5;
		dT_max2		=  2.5;
		rangeDt_0	= 20.0;
		rangeDt_1	=  2.5;	
	//
	} else {
		cout<<"UNKNOWN DATA CHOICE ..... Stopping Program. "<<endl;
		exit(EXIT_FAILURE);
	}
	
	cout<<"..... Setup      iAlgo = "<<iAlgorithm<<endl;
	cout<<"..... Setup    iChoice = "<<iChoice<<endl;
	cout<<"..... Setup      Niter = "<<NITERATIONS<<endl;
	cout<<"..... Setup    iDayMin = "<<iDayMin<<endl;
	cout<<"..... Setup    iDayMax = "<<iDayMax<<endl;
	cout<<"..... Setup   NREQUIRE = "<<NREQUIRE<<endl;
	cout<<"..... Setup    *A      = "<<A<<endl;
	cout<<"..... Setup     A_min  = "<<A_min<<endl;
	cout<<"..... Setup     A_max  = "<<A_max<<endl;
	cout<<"..... Setup    *Aother = "<<Aother<<endl;
	cout<<"..... Setup    *T      = "<<T<<endl;
	cout<<"..... Setup     T_min  = "<<T_min<<endl;
	cout<<"..... Setup     T_max  = "<<T_max<<endl;
	cout<<"..... Setup    dT_min  = "<<dT_min<<endl;
	cout<<"..... Setup    dT_max  = "<<dT_max<<endl;
	cout<<"..... Setup    dT_min2 = "<<dT_min2<<endl;
	cout<<"..... Setup    dT_max2 = "<<dT_max2<<endl;
	cout<<"..... Setup    TAC->ns = "<<TACtoNS<<endl;
	cout<<"..... Setup Nprim cut  = "<<Nprimarycut<<endl;
	cout<<"..... Setup Zerror cut = "<<Zerrorcut<<endl;
	cout<<"..... Setup Zdiff cut  = "<<Zdiffcut<<endl;
	cout<<"..... Setup Zdiffb cut = "<<Zdiffcutb<<endl;
	cout<<"..... Setup zvt Offset = "<<zvtoffset<<endl;
	cout<<"..... Setup zvtbOffset = "<<zvtoffsetb<<endl;

	ADCminimum	= 0.0;
	if (iAlgorithm==1){
		if (iChoice==0 || iChoice==3 ){
			ADCminimum	=   0.0;				// ToT is pulse size			
		} else if (iChoice==1 || iChoice==2){
			ADCminimum	= 200.0;				// ADC is pulse size			
		}
	}
	cout<<"..... Setup    ADC LL = "<<ADCminimum<<endl;

	char 			buf[50];
	Int_t			ipass;

	unsigned int timeliml	= itimemin;
	unsigned int timelimu	= itimemin + ((iDayMax-iDayMin+1)*24*60*60);
	unsigned int dtime		= timelimu - timeliml;
	dtimenbins				= dtime/4/60/60;		// 4hrs per timebin....
	//
	cout<<"..... Setup  RhicRun   = "<<iRhicRun<<endl; 
	cout<<"..... Setup  InputFlag = "<<iInputFlag<<endl; 
	cout<<"..... Setup  TreeStyle = "<<iTreeStyle<<endl; 
//	cout<<"..... Setup   out base = "<<outfilebase<<endl; 
//	sprintf(rootfile,"root/%s.root",outfilebase);
//	cout<<"..... Setup  root file = "<<rootfile<<endl; 
	cout<<"..... Setup refmultmax = "<<refmultmax<<endl; 
	cout<<"..... Setup timeliml   = "<<timeliml<<endl;
	cout<<"..... Setup timelimu   = "<<timelimu<<endl;
	cout<<"..... Setup dtime      = "<<dtime<<endl;
	cout<<"..... Setup dtimenbins = "<<dtimenbins<<endl;
	if (dtimenbins>500){
		cout<<"too many dtime bins... exiting"<<endl;
		exit(0);
	}


	cout <<"..... Setup Done ................."<<endl;
}
