#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<mpi.h>

#define ROOT 0
#define NOT_PRIME 1
#define PRIME 0
#define MAX_PRIME 3571



//calloc(number_of_elements_to_allocate, sizeof each element)

void mark_multiples_of(int, int, int, int*);
void mark_root(int,int,int*);
void print_primes(int, int, int*);
void do_local_check(int, int , int , int* );

int main(int argc, char *argv[]){
	const int n = (argc ==2)? atoi(argv[1]) : MAX_PRIME;
	int sqrt_n = ceil(sqrt(n)) ;//based on the prime factor test
	//msg[0] = last number checked, msg[1] = increment
	int i, j, comm_sz, my_rank, loc_n, msg[2];
	double start_t, end_t;
	
	
	/*startup MPI */
	MPI_Init(NULL,NULL);
	
	/*get my rank among all the processes */
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	
	/*get number of processes */
	MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
	
	/*range of n per process */
	loc_n = n/comm_sz;
	int *local_primes = (int*) calloc(loc_n, sizeof(int));
	if(my_rank == ROOT){
		printf("local_n is %d \n", loc_n);
		start_t = MPI_Wtime();
		local_primes[0] = NOT_PRIME; //1 is not prime
		for(i = 1; (i+1) <= sqrt_n; i++){

			if(!local_primes[i]){//not_marked
				j = i;
				do{
					j = j+(i+1);
					local_primes[j] = 1;
				}while(j+(i+1) < loc_n-1);

				msg[0] = j;//next number to mark
				msg[1] = i;//current increment
				if(comm_sz-1)//more than 1 node 
					MPI_Send(msg, 2, MPI_INT, my_rank+1, 0, MPI_COMM_WORLD);
				
			}
			
		}
		msg[0] = j+(i);//next number to mark
		msg[1] = i;//current increment 
		if(comm_sz-1)//more than 1 node 
					MPI_Send(msg, 2, MPI_INT, my_rank+1, 0, MPI_COMM_WORLD);
		
	}else if(my_rank < (comm_sz-1)){//not root && not last node
		printf("node %d",my_rank);
		do{
			MPI_Recv(msg, 2, MPI_INT, my_rank-1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			i = msg[1];//current_increment
			j = msg[0];//next_number_to_mark

			for( j+=i+1; (j)<loc_n*(my_rank+1) ; j += (i+1)){//mark multiples of loc_j

				local_primes[(j+1)-(loc_n*my_rank)] = NOT_PRIME;
			}
			msg[0] = j-(i+1);
			msg[1] = i;
			MPI_Send(msg, 2, MPI_INT, my_rank+1, 0, MPI_COMM_WORLD); 

		}while((i+1) <= sqrt_n);
	
	}else{//if i am the last node
		printf("last node\n");
		do{
			MPI_Recv(msg, 2, MPI_INT, my_rank-1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			i = msg[1];//current_increment
			j = msg[0];//next_number_to_mark
			//printf("increment = %d, prev_to_marked = %d\n", i, j);

			for(j+=i+1 ;  (j) <= loc_n*(my_rank+1) ; j += (i+1)){//mark multiples of j
				//printf("next_to_mark = %d\n", j);
				local_primes[(j+1)-(loc_n*my_rank)] = NOT_PRIME;
			}
			
		}while((i+1) <= sqrt_n);
	}
	//printf("my_rank= %d, range[0]= %d, range[1]= %d \n",my_rank
		//,range[0], range[1]);
	
	//after this all primes are marked for this range.
	MPI_Barrier(MPI_COMM_WORLD);
	end_t = MPI_Wtime();
	if(my_rank != ROOT){//send array of marked primes
		MPI_Send(local_primes,loc_n, MPI_INT, 0, 0, MPI_COMM_WORLD);
		
	}else{//if i am the root
		//print_primes(1, loc_n-1, local_primes);
		putchar('\n');
		for(i=1 ; i < comm_sz; i++){
			MPI_Recv(local_primes, loc_n, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			//print_primes(i*(loc_n), loc_n, local_primes); 
			putchar('\n');
		}
		//end_t = MPI_Wtime();
		printf("using %d procs, input was %d, time was  %lf \n",comm_sz,n, end_t-start_t);

	
	}
	
	//print my local ints. 
	//print_primes(my_rank*local_n,local_n, local_primes);
	//free(local_primes);

	MPI_Finalize();
	
	return 0;
}//end main

void mark_multiples_of(int p, int from, int to, int *primes){
	int i ;
	for( i = 0; from < to; i++, from++){
		//printf(" %d%%%d=%d \t ", from, p, from%p);
		//printf("from = %d", from);
		if( !(from%p) )
			primes[i] = 1;
	} 
}
void mark_root(int p,int n, int *primes){
	int j = p;
	do{
		j = j+(p+1);
		primes[j] = 1;
	}while(j < n && j+(p+1) < n);
}

void print_primes(int from, int n, int* primes){
	int prime_counter=0,i;
	for(i = 1 ;i < n ; i++)
		if(!primes[i]){//not marked
			printf("%d ,", from+i);
			//if(!(++prime_counter%20))
				//putint('\n');
			//++prime_counter;
		}
	//printf("# of primes in (1,%d) = %d", n,prime_counter);
}

void do_local_check(int from, int to, int sqrt_n, int* primes){
	int i,j;
	for(i=2, j=0; (i) <= sqrt_n ; i++, j++)
		//if(!primes[j])//if not marked
			mark_multiples_of(i, from, to, primes); 
	
}
