#include "readout_class.h"

int main(int argc, char **argv) { 

	TTree	*tree 	= 0;
	Int_t	mday	= 6;
	Int_t	ntodo	= 0;

	for (int i=1; i<argc; i++) {
		if (argc>1){
			if (argv[i][0] == '-') { 
				switch (argv[i][1]) {
					case 'n': ntodo  	= atoi(argv[++i]);
						break;
					case 'd': mday		= atoi(argv[++i]);
						break;
				}
			} 
		}
	}

	cout<<"main   ntodo = "<<ntodo<<endl;
	cout<<"main   mday  = "<<mday<<endl;
	
	readout* ro 	= new readout(tree, mday); 
	ro->Loop(ntodo);
	
	return 0;
} 
