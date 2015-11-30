#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#define NOT_PRIME 1
#define PRIME 0
#define PROCS 12
//calloc(number_of_elements_to_allocate, sizeof each element)
#define MAX_PRIME 100
void mark_multiples_of(int, int, char*);
void print_primes(int, char*);

int main(int argc, char *argv[]){
	const int n = (argc ==2)? atoi(argv[1]) : MAX_PRIME;
	char* primes = (char*) calloc( n,sizeof(char));
	int i,j;

	primes[0] = 1;//not prime.
	int sqrt_n = ceil( sqrt(n) );
	
	for(i = 1; (i+1) <= sqrt_n ; i++){
		if( !primes[i] )//if not marqued
			mark_multiples_of(i,n,primes);
			
	}
	
	//print_primes(n, primes);

}//end main

void mark_multiples_of(int p,int n, char *primes){
	int j = p;
	do{
		j = j+(p+1);
		primes[j] = 1;
	}while(j < n && j+(p+1) < n);
}

void print_primes(int n, char* primes){
	int prime_counter=0,i;
	for(i = 0 ;i < n ; i++)
		if(!primes[i]){//not marked
			printf("%d ,", i+1);
			if(!(++prime_counter%20))
				putchar('\n');
			//++prime_counter;
		}
	printf("\n# of primes in (1,%d) = %d", n,prime_counter);
}
