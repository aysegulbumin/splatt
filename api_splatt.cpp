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
	cout<<"Factorization Splatt Success: "<< ret <<endl;
	/* cleanup */
	splatt_free_csf(tt, cpd_opts);
	splatt_free_kruskal(&factored);
	return 0;
}
