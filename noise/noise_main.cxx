#include "noise_class.h"

int main(int argc, char **argv) {

	TTree	*treet		= 0;
	TTree	*treem		= 0;
	Int_t	krun		= 99999999;
	Int_t	ntd			= 0;

/*
// set working environment
char const* tmp = getenv("BTOF_NOISE_DIR");
tmp = getenv("BTOF_DD_DIR");
if (tmp)  btof_dd_dir=tmp;
 else	btof_dd_dir=".";
 cout << "data   : " << btof_dd_dir << endl;

tmp = getenv("BTOF_NOISE_DIR");
if (tmp)  btof_noise_dir=tmp;
else	btof_noise_dir=".";
cout << "results: " << btof_noise_dir << endl;

tmp = getenv("BTOF_PUB_DIR");
if (tmp)  btof_pub_dir=tmp;
 else	btof_pub_dir=".";
cout << "pub    : " << btof_pub_dir << endl;
*/

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
