#include <iostream>
#include <iomanip>
using namespace std;
int temp()
{
//     cout << "Normal  : " << ival << endl;
//     cout << "Right 10: " << setw(10) << ival << endl;
//     cout << "Base   8: " << setbase(8) << ival << endl;
//     cout << "Base  16: " << setbase(16) << ival << endl;
	for (int i=0;i<32;i++){
		unsigned int ival = pow(2,i);
	    cout<<setbase(10)<<i<<" "
	    	<<setprecision(12)<<ival<<" "
	    	<<setbase(16)<<ival<<endl;
	}

    return(0);
}
