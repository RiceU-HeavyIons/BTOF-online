#include "cal_class.h"

int main(int argc, char **argv) { 

	Int_t	ntodo   	=   0;
	Int_t	ichoi		=   0;
	Int_t	ialgo		=   2;
	TTree	*tree		=   0;
	Int_t	iener		= 500;
	Int_t	irhicyear	=  10;
	Int_t	iinput		=   0;
	Int_t	itrgcr		=   0;		// 0=BBQ (vpd), 1=MXQ (mtd)
	
	for (int i=1; i<argc; i++) {
		if (argc>1){
			if (argv[i][0] == '-') { 
				switch (argv[i][1]) {
					case 'n': ntodo  	= atoi(argv[++i]);
						break;
					case 'c': ichoi		= atoi(argv[++i]);
						break;
					case 'a': ialgo		= atoi(argv[++i]);
						break;
					case 'r': irhicyear	= atoi(argv[++i]);
						break;
					case 'e': iener		= atoi(argv[++i]);
						break;
					case 'i': iinput  	= atoi(argv[++i]);
						break;
					case 'd': itrgcr	= atoi(argv[++i]);
				}
			} 
		}
	}

	cout <<"..... Main rhic year = "<<irhicyear<<endl;
	cout <<"..... Main    energy = "<<iener<<endl;
	cout <<"..... Main    input  = "<<iinput<<endl;
	cout <<"..... Main    ntodo  = "<<ntodo  <<endl;
	cout <<"..... Main    choice = "<<ichoi<<endl;
	cout <<"..... Main    algo   = "<<ialgo<<endl;
	cout <<"..... Main    itrgcr = "<<itrgcr<<endl;

	cal *calinstance = new cal(irhicyear, iinput, ichoi, ialgo, iener, itrgcr, tree); 

	calinstance->Setup();
	calinstance->Loop(ntodo);

	cout<<"..... Main ... Program done ......................... "<<endl;
} 

