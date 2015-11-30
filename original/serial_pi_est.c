#include <stdio.h>

#define NAREA 100
void main() {
	int i; 
	 double step,x,sum=0.0,pi;
	step = 1.0/NAREA;
	for (i=0; i<NAREA; i++) {
		x = (i+0.5)*step;
		sum += 4.0/(1.0+x*x);
	}
	printf("sum after the loop is %lf \n", sum); 
	pi = sum*step;
	printf("PI = %lf\n",pi);
}
