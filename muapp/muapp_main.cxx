#include "muapp.h"

int main(int argc, char **argv) { 

	TTree	*tree 	= 0;
	Int_t	mday	= 20;
	Int_t	ntodo	= 0;
	Int_t	myear	= 14;
	Int_t	mrun	= 14;

	for (int i=1; i<argc; i++) {
		if (argc>1){
			if (argv[i][0] == '-') { 
				switch (argv[i][1]) {
					case 'n': ntodo  	= atoi(argv[++i]);
						break;
					case 'd': mday		= atoi(argv[++i]);
						break;
					case 'y': myear		= atoi(argv[++i]);
						break;
					case 'r': mrun		= atoi(argv[++i]);
						break;
				}
			} 
		}
	}

	cout<<"main   ntodo = "<<ntodo<<endl;
	cout<<"main   mrun  = "<<mrun<<endl;
	cout<<"main   myear = "<<myear<<endl;
	cout<<"main   mday  = "<<mday<<endl;
	
	muapp* mu 	= new muapp(tree, mrun, myear, mday); 
	mu->Loop(ntodo);
	
	return 0;
} 
