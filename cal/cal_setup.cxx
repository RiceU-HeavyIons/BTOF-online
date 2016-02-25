#include "cal_class.h"

//	cal_utils and cal_setup temp hacked for 200 pp2pp VPD_MB data

void cal::Setup() {

	cout<<"..... Setup ..................................... "<<endl;
	NDETSIDE	= 19;
	NITERATIONS	= 8;			// max=10
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

	Zdiffcut_l	= -40.00; 
	Zdiffcutb_l	= -15.00; 
	Zdiffcutc_l	=  -8.00; 
	Zdiffcut_u	=  40.00; 
	Zdiffcutb_u	=  15.00; 
	Zdiffcutc_u	=   8.00; 
	Nprimarycut	= 2;
	refmultcut	= 1;
	vtx_spot_x	= 0.0;
	vtx_spot_x	= 0.0;
	
	if (iRhicRun==14){
		vtx_spot_x	=  0.0;
		vtx_spot_y	= -0.9;
		if (iEnergy==200){refmultcut = 50;}
	}

	//----- VPD TOF-ToT and TOF-LE
	if (iChoice==0){
		cout<<"..... Setup    Using VPD TOF-ToT and TOF-LE"<<endl;
		A 			= &vpd_tot[0];
		T 			= &vpd_tle[0];
		if (iTrgCrate==0){
			Aother 	= &vpd_adc[0];
		} else if (iTrgCrate==1){
			Aother 	= &mvpd_adc[0];
		}
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
	//
	//----- VPD TRG-ADC and TRG-TAC
	} else if (iChoice==1){
		if (iTrgCrate==0){
			cout<<"..... Setup    Using VPD BBQ TRG-ADC and TRG-TAC"<<endl;
			A 			= &vpd_adc[0];
			T 			= &vpd_tac[0];
		} else if (iTrgCrate==1){
			cout<<"..... Setup    Using VPD MXQ TRG-ADC and TRG-TAC"<<endl;
			A 			= &mvpd_adc[0];
			T 			= &mvpd_tac[0];
		}
		Aother 		= &vpd_tot[0];
		A_nbin		=   64;
		A_min		=    0.0;
		A_max		= 3584.0;
		T_min		= 100.0;
		T_max		= 3950.0;
		TACtoNS		= 0.01773;
		//
	//
	//----- VPD TRG-ADC and TOF-LE
	} else if (iChoice==2){
		if (iTrgCrate==0){
			cout<<"..... Setup    Using VPD BBQ TRG-ADC and TOF-LE"<<endl;
			A 			= &vpd_adc[0];
		} else if (iTrgCrate==1){
			cout<<"..... Setup    Using VPD MXQ TRG-ADC and TOF-LE"<<endl;
			A 			= &mvpd_adc[0];
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
	//
	//----- VPD TOF-ToT and TRG-TAC
	} else if (iChoice==3){
		cout<<"..... Setup    Using VPD TOF-ToT and TRG-TAC"<<endl;
		A 			= &vpd_tot[0];
		if (iTrgCrate==0){
			cout<<"..... Setup    Using VPD TOF-ToT and TRG-TAC BBQ"<<endl;
			T 		= &vpd_tac[0];
		} else if (iTrgCrate==1){
			cout<<"..... Setup    Using VPD TOF-ToT and TRG-TAC MXQ"<<endl;
			T 		= &mvpd_tac[0];
		}
		Aother 		= &vpd_adc[0];
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
		TACtoNS		= 0.01773;
		//
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
		Zdiffcut_l	= -40.00; 
		Zdiffcutb_l	= -20.00; 
		Zdiffcutc_l	= -10.00; 
		Zdiffcut_u	=  40.00; 
		Zdiffcutb_u	=  20.00; 
		Zdiffcutc_u	=  10.00; 
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
	
	if (iRhicRun==14&&iEnergy==203){	// 3He+Au needs special windows!
		Zdiffcut_l	= -40.00; 
		Zdiffcut_u	=  40.00; 
		Zdiffcutb_l	=  -9.00; 
		Zdiffcutb_u	=  20.00; 
		Zdiffcutc_l	=  -8.00; 
		Zdiffcutc_u	=  16.00; 
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
	cout<<"..... Setup Zdiff  cutl= "<<Zdiffcut_l<<endl;
	cout<<"..... Setup Zdiff  cutu= "<<Zdiffcut_u<<endl;
	cout<<"..... Setup Zdiffb cutl= "<<Zdiffcutb_l<<endl;
	cout<<"..... Setup Zdiffb cutu= "<<Zdiffcutb_u<<endl;
	cout<<"..... Setup Zdiffc cutl= "<<Zdiffcutc_l<<endl;
	cout<<"..... Setup Zdiffc cutu= "<<Zdiffcutc_u<<endl;
	cout<<"..... Setup vtx_spot_x = "<<vtx_spot_x<<endl;
	cout<<"..... Setup vtx_spot_y = "<<vtx_spot_y<<endl;

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
	cout<<"..... Setup   out base = "<<outfilebase<<endl; 
	sprintf(buf,"root/%s.root",outfilebase);
	cout<<"..... Setup  root file = "<<buf<<endl; 
	cout<<"..... Setup refmultmax = "<<refmultmax<<endl; 
	cout<<"..... Setup timeliml   = "<<timeliml<<endl;
	cout<<"..... Setup timelimu   = "<<timelimu<<endl;
	cout<<"..... Setup dtime      = "<<dtime<<endl;
	cout<<"..... Setup dtimenbins = "<<dtimenbins<<endl;
	if (dtimenbins>500){
		cout<<"too many dtime bins... exiting"<<endl;
		exit(0);
	}

	fout = new TFile(buf,"RECREATE");
	fout->cd();
		hcoco_vpd_w	= new TH1D("hcoco_vpd_w","hcoco_vpd_w",256,-25.5,230.5);
		hcoco_vpd_e	= new TH1D("hcoco_vpd_e","hcoco_vpd_e",256,-25.5,230.5);
		hcoco_p2p_w	= new TH1D("hcoco_p2p_w","hcoco_p2p_w",256,-25.5,230.5);
		hcoco_p2p_e	= new TH1D("hcoco_p2p_e","hcoco_p2p_e",256,-25.5,230.5);
		if (iChoice==0||iChoice==2){
			hinitialoffsets  	= new TH2D("hinitialoffsets","hinitialoffsets",
								2*NDETSIDE,-0.5,((Float_t)(2*NDETSIDE))-0.5,1001,-250.25,250.25);
			hinitialoffsetvals  = new TH1D("hinitialoffsetvals","hinitialoffsetvals",
								2*NDETSIDE,-0.5,((Float_t)(2*NDETSIDE))-0.5); }
		else if (iChoice==1||iChoice==3||iChoice==5){
			hinitialoffsets  	= new TH2D("hinitialoffsets","hinitialoffsets",
								2*NDETSIDE,-0.5,((Float_t)(2*NDETSIDE))-0.5,501,-50.1,50.1);
			hinitialoffsetvals  = new TH1D("hinitialoffsetvals","hinitialoffsetvals",
								2*NDETSIDE,-0.5,((Float_t)(2*NDETSIDE))-0.5); 
		}
		for (int is=0;is<2;is++){
			sprintf(buf,"hinitialoffsetsEW%d",is);
			hinitialoffsetsEW[is]	= new TH1D(buf,buf,1604,-200.5,200.5);
		}
// 		hnenw		 = new TH2D("hnenw","hnenw",9,-0.5,8.5,9,-0.5,8.5);	
// 		htavCtrRel   = new TH2D("htavCtrRel","htavCtrRel",8,-0.5,7.5,400,-100,100);	
// 		htavCtrRelsh = new TH2D("htavCtrRelsh","htavCtrRelsh",8,-0.5,7.5,400,-100,100);	
// 		htavPmtRel   = new TH2D("htavPmtRel","htavPmtRel",16,-0.5,15.5,400,-100,100);	
// 		htavPmtRelsh = new TH2D("htavPmtRelsh","htavPmtRelsh",16,-0.5,15.5,400,-100,100);	

		hdtzv_11			= new TH1D("hdtzv_11","hdtzv_11",50,-100,100);
		hdtzv_11w	= new TH2D("hdtzv_11w","hdtzv_11w",NDETSIDE,-0.5,((Float_t)NDETSIDE)-0.5,25,-100,100);
		hdtzv_11e	= new TH2D("hdtzv_11e","hdtzv_11e",NDETSIDE,-0.5,((Float_t)NDETSIDE)-0.5,25,-100,100);

		hdzd		= new TProfile2D("hdzd","hdzd",NDETSIDE,-0.5,((Float_t)NDETSIDE)-0.5,
												   NDETSIDE,-0.5,((Float_t)NDETSIDE)-0.5," ");
		hdzd3D		= new TH3D("hdzd3D","hdzd3D",  NDETSIDE,-0.5,((Float_t)NDETSIDE)-0.5,
												   NDETSIDE,-0.5,((Float_t)NDETSIDE)-0.5,
												   100,-200.,200.);
		hdzd3Dvals	= new TH2D("hdzd3Dvals","hdzd3Dvals",NDETSIDE,-0.5,((Float_t)NDETSIDE)-0.5,
												        NDETSIDE,-0.5,((Float_t)NDETSIDE)-0.5 );

		hddzvzt_A_all	= new TH1D("hddzvzt_A_all" ,"hddzvzt_A_all" ,200,-200,200);
		hddzvzt_B_all	= new TH1D("hddzvzt_B_all" ,"hddzvzt_B_all" ,200,-200,200);
		hddzvzt_C_all	= new TH1D("hddzvzt_C_all" ,"hddzvzt_C_all" ,200,-200,200);
		hddzvzt_A_keep	= new TH1D("hddzvzt_A_keep","hddzvzt_A_keep",200,-200,200);
		hddzvzt_B_keep	= new TH1D("hddzvzt_B_keep","hddzvzt_B_keep",200,-200,200);
		hddzvzt_C_keep	= new TH1D("hddzvzt_C_keep","hddzvzt_C_keep",200,-200,200);

		int ndaybins	= iDayMax - iDayMin + 1;
		hddzvzt_A_evt	= new TH2D("hddzvzt_A_evt" ,"hddzvzt_A_evt" ,100,0.,nentriestree,100,-200,200);
		hddzvzt_B_evt	= new TH2D("hddzvzt_B_evt" ,"hddzvzt_B_evt" ,100,0.,nentriestree,100,-200,200);
		hddzvzt_C_evt	= new TH2D("hddzvzt_C_evt" ,"hddzvzt_C_evt" ,100,0.,nentriestree,100,-200,200);
		hddzvzt_A_day	= new TH2D("hddzvzt_A_day" ,"hddzvzt_A_day" ,ndaybins,iDayMin,iDayMax+1,100,-200,200);
		hddzvzt_B_day	= new TH2D("hddzvzt_B_day" ,"hddzvzt_B_day" ,ndaybins,iDayMin,iDayMax+1,100,-200,200);
		hddzvzt_C_day	= new TH2D("hddzvzt_C_day" ,"hddzvzt_C_day" ,ndaybins,iDayMin,iDayMax+1,100,-200,200);

		hdtzv_nn_00			= new TH2D("hdtzv_nn_00","hdtzv_nn_00",80,-200,200,100,-200,200);
		hdtzv_nn_11			= new TH2D("hdtzv_nn_11","hdtzv_nn_11",80,-200,200,100,-200,200);
		hdtzv_nn_22			= new TH2D("hdtzv_nn_22","hdtzv_nn_22",80,-200,200,100,-200,200);
		hdtzv_nn_33			= new TH2D("hdtzv_nn_33","hdtzv_nn_33",80,-200,200,100,-200,200);
		hdtzv_nn_00_keep	= new TH2D("hdtzv_nn_00_keep","hdtzv_nn_00_keep",80,-200,200,100,-200,200);
		hdtzv_nn_11_keep	= new TH2D("hdtzv_nn_11_keep","hdtzv_nn_11_keep",80,-200,200,100,-200,200);
		hdtzv_nn_22_keep	= new TH2D("hdtzv_nn_22_keep","hdtzv_nn_22_keep",80,-200,200,100,-200,200);
		hdtzv_nn_33_keep	= new TH2D("hdtzv_nn_33_keep","hdtzv_nn_33_keep",80,-200,200,100,-200,200);
		hnvalidpairs= new TH2D("hnvalidpairs","hnvalidpairs",250,-0.5,249.5,250,-0.5,249.5);

		hdtzvb_nn_00		= new TH2D("hdtzvb_nn_00","hdtzvb_nn_00",80,-200,200,100,-200,200);
		hdtzvb_nn_11		= new TH2D("hdtzvb_nn_11","hdtzvb_nn_11",80,-200,200,100,-200,200);
		hdtzvb_nn_22		= new TH2D("hdtzvb_nn_22","hdtzvb_nn_22",80,-200,200,100,-200,200);
		hdtzvb_nn_33		= new TH2D("hdtzvb_nn_33","hdtzvb_nn_33",80,-200,200,100,-200,200);
		hdtzvb_nn_00_keep	= new TH2D("hdtzvb_nn_00_keep","hdtzvb_nn_00_keep",80,-200,200,100,-200,200);
		hdtzvb_nn_11_keep	= new TH2D("hdtzvb_nn_11_keep","hdtzvb_nn_11_keep",80,-200,200,100,-200,200);
		hdtzvb_nn_22_keep	= new TH2D("hdtzvb_nn_22_keep","hdtzvb_nn_22_keep",80,-200,200,100,-200,200);
		hdtzvb_nn_33_keep	= new TH2D("hdtzvb_nn_33_keep","hdtzvb_nn_33_keep",80,-200,200,100,-200,200);
		hnvalidpairsb		= new TH2D("hnvalidpairsb","hnvalidpairsb",250,-0.5,249.5,250,-0.5,249.5);

		hdtzvc_nn_00		= new TH2D("hdtzvc_nn_00","hdtzvc_nn_00",80,-200,200,100,-200,200);
		hdtzvc_nn_11		= new TH2D("hdtzvc_nn_11","hdtzvc_nn_11",80,-200,200,100,-200,200);
		hdtzvc_nn_22		= new TH2D("hdtzvc_nn_22","hdtzvc_nn_22",80,-200,200,100,-200,200);
		hdtzvc_nn_33		= new TH2D("hdtzvc_nn_33","hdtzvc_nn_33",80,-200,200,100,-200,200);
		hdtzvc_nn_00_keep	= new TH2D("hdtzvc_nn_00_keep","hdtzvc_nn_00_keep",80,-200,200,100,-200,200);
		hdtzvc_nn_11_keep	= new TH2D("hdtzvc_nn_11_keep","hdtzvc_nn_11_keep",80,-200,200,100,-200,200);
		hdtzvc_nn_22_keep	= new TH2D("hdtzvc_nn_22_keep","hdtzvc_nn_22_keep",80,-200,200,100,-200,200);
		hdtzvc_nn_33_keep	= new TH2D("hdtzvc_nn_33_keep","hdtzvc_nn_33_keep",80,-200,200,100,-200,200);
		hnvalidpairsc		= new TH2D("hnvalidpairsc","hnvalidpairsc",250,-0.5,249.5,250,-0.5,249.5);

		hdtzv1D_nn_00		= new TH1D("hdtzv1D_nn_00","hdtzv1D_nn_00",200,-100,100);
		hdtzv1D_nn_11		= new TH1D("hdtzv1D_nn_11","hdtzv1D_nn_11",200,-100,100);
		hdtzv1D_nn_22		= new TH1D("hdtzv1D_nn_22","hdtzv1D_nn_22",200,-100,100);
		hdtzv1D_nn_33		= new TH1D("hdtzv1D_nn_33","hdtzv1D_nn_33",200,-100,100);
		hdtzv1D_nn_00_keep	= new TH1D("hdtzv1D_nn_00_keep","hdtzv1D_nn_00_keep",200,-100,100);
		hdtzv1D_nn_11_keep	= new TH1D("hdtzv1D_nn_11_keep","hdtzv1D_nn_11_keep",200,-100,100);
		hdtzv1D_nn_22_keep	= new TH1D("hdtzv1D_nn_22_keep","hdtzv1D_nn_22_keep",200,-100,100);
		hdtzv1D_nn_33_keep	= new TH1D("hdtzv1D_nn_33_keep","hdtzv1D_nn_33_keep",200,-100,100);
		hdtzvb1D_nn_00		= new TH1D("hdtzvb1D_nn_00","hdtzvb1D_nn_00",200,-100,100);
		hdtzvb1D_nn_11		= new TH1D("hdtzvb1D_nn_11","hdtzvb1D_nn_11",200,-100,100);
		hdtzvb1D_nn_22		= new TH1D("hdtzvb1D_nn_22","hdtzvb1D_nn_22",200,-100,100);
		hdtzvb1D_nn_33		= new TH1D("hdtzvb1D_nn_33","hdtzvb1D_nn_33",200,-100,100);
		hdtzvb1D_nn_00_keep	= new TH1D("hdtzvb1D_nn_00_keep","hdtzvb1D_nn_00_keep",200,-100,100);
		hdtzvb1D_nn_11_keep	= new TH1D("hdtzvb1D_nn_11_keep","hdtzvb1D_nn_11_keep",200,-100,100);
		hdtzvb1D_nn_22_keep	= new TH1D("hdtzvb1D_nn_22_keep","hdtzvb1D_nn_22_keep",200,-100,100);
		hdtzvb1D_nn_33_keep	= new TH1D("hdtzvb1D_nn_33_keep","hdtzvb1D_nn_33_keep",200,-100,100);
		hdtzvc1D_nn_00		= new TH1D("hdtzvc1D_nn_00","hdtzvc1D_nn_00",200,-100,100);
		hdtzvc1D_nn_11		= new TH1D("hdtzvc1D_nn_11","hdtzvc1D_nn_11",200,-100,100);
		hdtzvc1D_nn_22		= new TH1D("hdtzvc1D_nn_22","hdtzvc1D_nn_22",200,-100,100);
		hdtzvc1D_nn_33		= new TH1D("hdtzvc1D_nn_33","hdtzvc1D_nn_33",200,-100,100);
		hdtzvc1D_nn_00_keep	= new TH1D("hdtzvc1D_nn_00_keep","hdtzvc1D_nn_00_keep",200,-100,100);
		hdtzvc1D_nn_11_keep	= new TH1D("hdtzvc1D_nn_11_keep","hdtzvc1D_nn_11_keep",200,-100,100);
		hdtzvc1D_nn_22_keep	= new TH1D("hdtzvc1D_nn_22_keep","hdtzvc1D_nn_22_keep",200,-100,100);
		hdtzvc1D_nn_33_keep	= new TH1D("hdtzvc1D_nn_33_keep","hdtzvc1D_nn_33_keep",200,-100,100);

 		hAiter[0]	= new TH2D("hAiter0","hAiter0",
							NITERATIONS,-0.5,((Float_t)NITERATIONS)-0.5,A_nbin,A_min,A_max);
 		hAiter[1]	= new TH2D("hAiter1","hAiter1",
							NITERATIONS,-0.5,((Float_t)NITERATIONS)-0.5,A_nbin,A_min,A_max);
		hnev_day	= new TH1D("hnev_day","hnev_day",366,-0.5,365.5);
		hnev_daqbits= new TH1D("hnev_daqbits","hnev_daqbits",64,-0.5,63.5);

		for (int i=0;i<40;i++){
			sprintf(buf,"hcnts%d",i);
				hcnts[i]	= new TH1D(buf,buf,NITERATIONS,-0.5,((float)NITERATIONS)-0.5);
				hcnts[i]->SetStats(kFALSE);
		}


	dir_fits		= fout->mkdir("fits");
	dir_fitslices	= fout->mkdir("fitslices");
	dir_res			= fout->mkdir("res");
	dir_A			= fout->mkdir("A");
	dir_zvtx		= fout->mkdir("zvtx");
	dir_day			= fout->mkdir("day");
	dir_time		= fout->mkdir("time");
	dir_ffun		= fout->mkdir("ffun");
	dir_1mn			= fout->mkdir("1mn");
	dir_nvpd		= fout->mkdir("nvpd");

	const Int_t nbinsx =  120;
	Double_t xmin    =   0.1;
	Double_t xmax    =  60.1;
	Double_t logxmin = TMath::Log10(xmin);
	Double_t logxmax = TMath::Log10(xmax);
	Double_t binwidthx = (logxmax-logxmin)/nbinsx;
	Double_t xbins[nbinsx+1];
	xbins[0] = xmin;
	for (Int_t i=1;i<=nbinsx;i++) {
	  xbins[i] = xmin + TMath::Power(10,logxmin+i*binwidthx);
	}
	const Int_t nbinsy =   128;
	Double_t ymin    =     0.5;
	Double_t ymay    =  4096.5;
	Double_t logymin = TMath::Log10(ymin);
	Double_t logymay = TMath::Log10(ymay);
	Double_t binwidthy = (logymay-logymin)/nbinsy;
	Double_t ybins[nbinsy+1];
	ybins[0] = ymin;
	for (Int_t i=1;i<=nbinsy;i++) {
	  ybins[i] = ymin + TMath::Power(10,logymin+i*binwidthy);
	}
	//for (int i=0;i<20;i++){
	//	cout<<i<<" "<<xbins[i]<<" "<<ybins[i]<<endl;
	//}
	
	for (int is=0;is<2;is++){

		dir_A->cd();
		for (int idet=0;idet<NDETSIDE;idet++){
			sprintf(buf,"adctot%d%d",idet,is);
				adctot[idet][is]	= new TH2D(buf,buf,nbinsx,xbins,nbinsy,ybins);
				adctot[idet][is]->SetStats(kFALSE);
			sprintf(buf,"htot%d%d",idet,is);
				htot[idet][is]	= new TH1D(buf,buf,128,0.0,  64.0);
			sprintf(buf,"hadc%d%d",idet,is);
				hadc[idet][is]	= new TH1D(buf,buf,128,0.0,4096.0);
		}
		
		dir_nvpd->cd();
		for (int iter=0;iter<NITERATIONS;iter++){
				sprintf(buf,"hnvpdrm%d%d",iter,is);
				hnvpdrm[iter][is]	= new TH2D(buf,buf,int(refmultmax),0,refmultmax,20,-0.5,19.5);			
		}
				
		dir_fits->cd();
		for (int i=0;i<NITERATIONS*NDETSIDE;i++){
			ipass			= i/NDETSIDE;
			if (ipass==0){
				sprintf(buf,"hdt_A%d%d",i,is);
				hdt_A[i][is]	= new TH2D(buf,buf,A_nbin,A_min,A_max,dT_nbin,dT_min,dT_max);
				sprintf(buf,"hdt_A_1D%d%d",i,is);
				hdt_A_1D[i][is]	= new TH1D(buf,buf,dT_nbin,dT_min,dT_max);
			} else {
				sprintf(buf,"hdt_A%d%d",i,is);
				hdt_A[i][is]	= new TH2D(buf,buf,A_nbin,A_min,A_max,dT_nbin,dT_min2,dT_max2);
				sprintf(buf,"hdt_A_1D%d%d",i,is);
				hdt_A_1D[i][is]	= new TH1D(buf,buf,dT_nbin,dT_min2,dT_max2);
			}
		}
		dir_fitslices->cd();
		for (int i=0;i<NITERATIONS*NDETSIDE;i++){
			sprintf(buf,"hdt_A_cons%d%d",i,is);
				hdt_A_cons[i][is]	= new TH1D(buf,buf,A_nbin,A_min,A_max);
			sprintf(buf,"hdt_A_mean%d%d",i,is);
				hdt_A_mean[i][is]	= new TH1D(buf,buf,A_nbin,A_min,A_max);
			sprintf(buf,"hdt_A_sigm%d%d",i,is);
				hdt_A_sigm[i][is]	= new TH1D(buf,buf,A_nbin,A_min,A_max);
		}
		dir_res->cd();
		for (int idet=0;idet<NDETSIDE;idet++){
			sprintf(buf,"hdt_beg%d%d",idet,is);
			hdt_beg[idet][is]	= new TH1D(buf,buf,500,-rangeDt_0,rangeDt_0);
			sprintf(buf,"hdt_end%d%d",idet,is);
			hdt_end[idet][is]	= new TH1D(buf,buf,501,-5.01,5.01);
			//
			sprintf(buf,"hdt_beg_n%d%d",idet,is);
			hdt_beg_n[idet][is]	= new TH2D(buf,buf,
							NDETSIDE,-0.5,((Float_t)(NDETSIDE))-0.5,500,-rangeDt_0,rangeDt_0);
			sprintf(buf,"hdt_end_n%d%d",idet,is);
			hdt_end_n[idet][is]	= new TH2D(buf,buf,
							NDETSIDE,-0.5,((Float_t)(NDETSIDE))-0.5,501,-5.01,5.01);
			//
			sprintf(buf,"hdt_end_n_cons%d%d",idet,is);
				hdt_end_n_cons[idet][is]	= new TH1D(buf,buf,NDETSIDE,-0.5,((Float_t)(NDETSIDE))-0.5);
			sprintf(buf,"hdt_end_n_mean%d%d",idet,is);
				hdt_end_n_mean[idet][is]	= new TH1D(buf,buf,NDETSIDE,-0.5,((Float_t)(NDETSIDE))-0.5);
			sprintf(buf,"hdt_end_n_sigm%d%d",idet,is);
				hdt_end_n_sigm[idet][is]	= new TH1D(buf,buf,NDETSIDE,-0.5,((Float_t)(NDETSIDE))-0.5);
		}
		//
		dir_day->cd();
		int idt;
		for (int id=iDayMin;id<=iDayMax;id++){
			idt	= id - iDayMin;
			sprintf(buf,"hdt_end_n_det0_day%d%d",idt,is);
			hdt_end_n_det0_day[idt][is]	= new TH2D(buf,buf,
						NDETSIDE,-0.5,((Float_t)(NDETSIDE))-0.5,500,-rangeDt_0,rangeDt_0);
			sprintf(buf,"hdt_end_n_det1_day%d%d",idt,is);
			hdt_end_n_det1_day[idt][is]	= new TH2D(buf,buf,
						NDETSIDE,-0.5,((Float_t)(NDETSIDE))-0.5,500,-rangeDt_0,rangeDt_0);
			sprintf(buf,"hdt_end_n_det2_day%d%d",idt,is);
			hdt_end_n_det2_day[idt][is]	= new TH2D(buf,buf,
						NDETSIDE,-0.5,((Float_t)(NDETSIDE))-0.5,500,-rangeDt_0,rangeDt_0);
			sprintf(buf,"hdt_end_n_det3_day%d%d",idt,is);
			hdt_end_n_det3_day[idt][is]	= new TH2D(buf,buf,
						NDETSIDE,-0.5,((Float_t)(NDETSIDE))-0.5,500,-rangeDt_0,rangeDt_0);
		}
		dir_time->cd();
		for (int idt=0;idt<=dtimenbins;idt++){
			sprintf(buf,"hdt_end_n_det0_time%d%d",idt,is);
			hdt_end_n_det0_time[idt][is]	= new TH2D(buf,buf,
						NDETSIDE,-0.5,((Float_t)(NDETSIDE))-0.5,500,-rangeDt_0,rangeDt_0);
			sprintf(buf,"hdt_end_n_det1_time%d%d",idt,is);
			hdt_end_n_det1_time[idt][is]	= new TH2D(buf,buf,
						NDETSIDE,-0.5,((Float_t)(NDETSIDE))-0.5,500,-rangeDt_0,rangeDt_0);
			sprintf(buf,"hdt_end_n_det2_time%d%d",idt,is);
			hdt_end_n_det2_time[idt][is]	= new TH2D(buf,buf,
						NDETSIDE,-0.5,((Float_t)(NDETSIDE))-0.5,500,-rangeDt_0,rangeDt_0);
			sprintf(buf,"hdt_end_n_det3_time%d%d",idt,is);
			hdt_end_n_det3_time[idt][is]	= new TH2D(buf,buf,
						NDETSIDE,-0.5,((Float_t)(NDETSIDE))-0.5,500,-rangeDt_0,rangeDt_0);
		}

		//
// 		sprintf(buf,"hdt_ch5_day%d",is);
// 			hdt_ch5_day[is]	= new TH3D(buf,buf,
// 							NDETSIDE,-0.5,((Float_t)(NDETSIDE))-0.5,500,-rangeDt_0,rangeDt_0,
// 							10,124.5,134.5);
// 		sprintf(buf,"hdt_ch5_daqbits%d",is);
// 			hdt_ch5_daqbits[is]	= new TH3D(buf,buf,
// 							NDETSIDE,-0.5,((Float_t)(NDETSIDE))-0.5,500,-rangeDt_0,rangeDt_0,
// 							16,-0.5,15.5);
		//

		fout->cd();
		sprintf(buf,"hdt_end_det0_zvtx%d",is);
		hdt_end_det0_zvtx[is]	= new TH2D(buf,buf,100,-100,100,500,-rangeDt_0,rangeDt_0);
		sprintf(buf,"hdt_end_det0_zvtx_cons%d",is);
		hdt_end_det0_zvtx_cons[is]	= new TH1D(buf,buf,100,-100,100);
		sprintf(buf,"hdt_end_det0_zvtx_mean%d",is);
		hdt_end_det0_zvtx_mean[is]	= new TH1D(buf,buf,100,-100,100);
		sprintf(buf,"hdt_end_det0_zvtx_sigm%d",is);
		hdt_end_det0_zvtx_sigm[is]	= new TH1D(buf,buf,100,-100,100);

		sprintf(buf,"hdt_end_det0_grefmult%d",is);
		hdt_end_det0_grefmult[is]	= new TH2D(buf,buf,int(refmultmax),0,refmultmax,500,-rangeDt_0,rangeDt_0);
		sprintf(buf,"hdt_end_det0_grefmult_cons%d",is);
		hdt_end_det0_grefmult_cons[is]	= new TH1D(buf,buf,int(refmultmax),0,refmultmax);
		sprintf(buf,"hdt_end_det0_grefmult_mean%d",is);
		hdt_end_det0_grefmult_mean[is]	= new TH1D(buf,buf,int(refmultmax),0,refmultmax);
		sprintf(buf,"hdt_end_det0_grefmult_sigm%d",is);
		hdt_end_det0_grefmult_sigm[is]	= new TH1D(buf,buf,int(refmultmax),0,refmultmax);

		dir_1mn->cd();
		for (int i=0;i<NITERATIONS;i++){
			for (int idet=0;idet<NDETSIDE;idet++){
				sprintf(buf,"h1mn_n%d%d%d",i,idet,is);
				h1mn_n[i][idet][is]	= new TH2D(buf,buf,NDETSIDE,0.5,((Float_t)(NDETSIDE))+0.5,200,-5,5);
			}
		}
		
	}	//---- end is


	hdt_end_chan_nLLa = new TH2D("hdt_end_chan_nLLa","hdt_end_chan_nLLa",
					2*NDETSIDE,-0.5,((Float_t)(2*NDETSIDE))-0.5,400,-1.,1.);
	hdt_end_chan_nLLa_cons = new TH1D("hdt_end_chan_nLLa_cons","hdt_end_chan_nLLa_cons",
					2*NDETSIDE,-0.5,((Float_t)(2*NDETSIDE))-0.5);
	hdt_end_chan_nLLa_mean = new TH1D("hdt_end_chan_nLLa_mean","hdt_end_chan_nLLa_mean",
					2*NDETSIDE,-0.5,((Float_t)(2*NDETSIDE))-0.5);
	hdt_end_chan_nLLa_sigm = new TH1D("hdt_end_chan_nLLa_sigm","hdt_end_chan_nLLa_sigm",
					2*NDETSIDE,-0.5,((Float_t)(2*NDETSIDE))-0.5);
	hdt_end_chan_nLLb = new TH2D("hdt_end_chan_nLLb","hdt_end_chan_nLLb",
					2*NDETSIDE,-0.5,((Float_t)(2*NDETSIDE))-0.5,400,-1.,1.);
	hdt_end_chan_nLLb_cons = new TH1D("hdt_end_chan_nLLb_cons","hdt_end_chan_nLLb_cons",
					2*NDETSIDE,-0.5,((Float_t)(2*NDETSIDE))-0.5);
	hdt_end_chan_nLLb_mean = new TH1D("hdt_end_chan_nLLb_mean","hdt_end_chan_nLLb_mean",
					2*NDETSIDE,-0.5,((Float_t)(2*NDETSIDE))-0.5);
	hdt_end_chan_nLLb_sigm = new TH1D("hdt_end_chan_nLLb_sigm","hdt_end_chan_nLLb_sigm",
					2*NDETSIDE,-0.5,((Float_t)(2*NDETSIDE))-0.5);
	
	dir_zvtx->cd();
	htemp1				= new TH2D("htemp1","htemp1",200,-200,200,100,-25,25);
	htemp2				= new TH2D("htemp2","htemp2",19,-0.5,18.5,100,-25,25);
	htemp3				= new TH2D("htemp3","htemp3",19,-0.5,18.5,100,-25,25);
	hthist_zvtx			= new TH2D("hthist_zvtx","hthist_zvtx",200,-200,200,100,-25,25);
	hzvt_end_zvtx_all	= new TH2D("hzvt_end_zvtx_all","hzvt_end_zvtx_all",200,-100,100,200,-200.0,200.0);
	hzvt_end_zvtx_cut	= new TH2D("hzvt_end_zvtx_cut","hzvt_end_zvtx_cut",200,-100,100,200,-200.0,200.0);
	hdzvtx_11			= new TH1D("hdzvtx_11","hdzvtx_11",500,-50,50);
	hdzvtx_11_zvtx		= new TH2D("hdzvtx_11_zvtx","hdzvtx_11_zvtx",100,-50,50,500,-50,50);
	
// 	TDirectory *dir_xp	= fout->mkdir("xpos");
// 		TH2D *hdt_end_xp[8];		//---- west only...
// 		for (int ip=0;ip<8;ip++){
// 			sprintf(buf,"hdt_end_xp%d",ip);
// 			hdt_end_xp[ip]	= new TH2D(buf,buf,32,-0.8,0.8,60,-3.0,3.0);
// 		}
	fout->cd();

	out_ffun_nb	= 128; 
	out_ffun_x1	= 0.0; 
	if (iChoice==0){ out_ffun_x2	=	  64.0; } else
	if (iChoice==1){ out_ffun_x2	=	4096.0; } else
	if (iChoice==2){ out_ffun_x2	=	4096.0; } else
	if (iChoice==3){ out_ffun_x2	=	  64.0; } else
	if (iChoice==5){ out_ffun_x2	=	4096.0; }
	dir_ffun->cd();
	for (int is=0;is<2;is++){
		for (int i=0;i<NDETSIDE*NITERATIONS;i++){
			//sprintf(buf,"hadc_iter%d%d",iter,is);
			//hadc_iter[iter][is]	= new TH1D(buf,buf,out_ffun_nb,out_ffun_x1,out_ffun_x2);
			sprintf(buf,"ffun_iter%d%d",i,is);
			ffun_iter[i][is]	= new TH1D(buf,buf,out_ffun_nb,out_ffun_x1,out_ffun_x2);
			//
		}
		for (int idet=0;idet<NDETSIDE;idet++){
			sprintf(buf,"hcorrfinal%d%d",idet,is);
			hcorrfinal[idet][is]	= new TH1D(buf,buf,out_ffun_nb,out_ffun_x1,out_ffun_x2);
		}
	}

	cout <<"..... Setup Done ................."<<endl;
}
