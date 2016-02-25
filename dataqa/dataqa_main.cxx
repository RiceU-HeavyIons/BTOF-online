#include "dataqa_class.h"

int main(int argc, char **argv) { 

	TTree	*tree 	= 0;
 	Int_t	ntodo	= 0;
 	Int_t	mrun	= 14;
 	Int_t	mstream	= 0;	// =0 st_physics, =1 st_hltgood
// 
 	for (int i=1; i<argc; i++) {
 		if (argc>1){
 			if (argv[i][0] == '-') { 
 				switch (argv[i][1]) {
 					case 'n': ntodo  	= atoi(argv[++i]);
 						break;
 					case 'r': mrun		= atoi(argv[++i]);
 						break;
 					case 's': mstream	= atoi(argv[++i]);
 						break;
 				}
 			} 
 		}
 	}
// 
 	cout<<"main   ntodo   = "<<ntodo<<endl;
 	cout<<"main   mrun    = "<<mrun<<endl;
 	cout<<"main   mstream = "<<mstream<<endl;
// 	
	dataqa* qa 	= new dataqa(tree,mrun,ntodo,mstream); 
	qa->Loop();
	
	return 0;
} 
