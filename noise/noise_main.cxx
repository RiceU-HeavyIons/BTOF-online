#include "noise_class.h"

int main(int argc, char **argv) {

	TTree	*treet		= 0;
	TTree	*treem		= 0;
	Int_t	krun		= 99999999;
	Int_t	ntd			= 0;

	for (int i=1; i<argc; i++) {
		if (argc>1){
			if (argv[i][0] == '-') {
				switch (argv[i][1]) {
					case 'r': krun  	= atoi(argv[++i]);
						break;
					case 'n': ntd    	= atoi(argv[++i]);
						break;
				}
			}
		}
	}

//	cout<<"..... Main ... krun  = "<<krun<<endl;
//	cout<<"..... Main ... ntodo = "<<ntd<<endl;


	noise *instance = new noise(treet,treem,krun,ntd);
	gErrorIgnoreLevel	= 2000;
	instance->Loop();


//	cout<<"..... Main ... Program done.........."<<endl;

	//return 0;
}
