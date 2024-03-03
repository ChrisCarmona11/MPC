/*
 * main implementation: use this 'C' sample to create your own application
 *
 */





#include "derivative.h" /* include peripheral declarations */



int main(void)
{
	int counter = 0;
	int a, b, c = 0;
	long int long_a, long_b, long_c;
	float f_a, f_b, f_c;
	double d_a, d_b, d_c;
	
	
	b = 2; c = 3;
	a = b + c;
	long_a = long_b + long_c;
	f_a = f_b + f_c;
	d_a = d_b + d_c;
	
	printf("%i \n",a);
	
	return 0;
}
