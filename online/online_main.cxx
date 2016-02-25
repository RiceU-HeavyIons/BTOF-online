#include "online_class.h"

int main(int argc, char **argv) { 

	TTree	*tree		= 0;
	Int_t	krun		= 11077027;
	
	for (int i=1; i<argc; i++) {
		if (argc>1){
			if (argv[i][0] == '-') { 
				switch (argv[i][1]) {
					case 'r': krun  	= atoi(argv[++i]);
						break;
				}
			} 
		}
	}

	cout<<"..... Main ... krun = "<<krun<<endl;
	
	online *instance = new online(tree,krun); 
	instance->Loop();
	
	cout<<"..... Main ... Program done.........."<<endl;
	
	//return 0;
} 

