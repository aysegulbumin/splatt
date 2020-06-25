// The following code is adopted from the README file shared in splatt github. 
// Running the given sample API, I had many issues so created the following version which is up and running with the following commands. 
// The following file can be run using the following commands in ubuntu command line
// >> g++ -fopenmp api_splatt.cpp -lsplatt -llapack -lblas -o api_splatt
// >> ./api_splatt
// It is important to have lapack and blas libraries, and the linking libraries is also highly essential.

#include <splatt.h>
#include <iostream>

using namespace std;

/* allocate default options */

splatt_idx_t nmodes;
splatt_csf * tt;
double const * cpd_opts = splatt_default_opts();
int a = splatt_csf_load("mytensor.tns", &nmodes, &tt, cpd_opts);
/* do the factorization! */
splatt_kruskal factored;
int ret = splatt_cpd_als(tt, 10, cpd_opts, &factored);




int main()
{
	/* do some processing */
	for(splatt_idx_t m = 0; m < nmodes; ++m) {
	  /* access factored.lambda and factored.factors[m] */
		cout<<"Mode:"<<m<<endl;
	}

	cout<<"Splatt Success: "<< a <<endl;
	cout<<"Factorization Success: "<< ret <<endl;
	/* cleanup */
	splatt_free_csf(tt, cpd_opts);
	splatt_free_kruskal(&factored);
	return 0;
}
