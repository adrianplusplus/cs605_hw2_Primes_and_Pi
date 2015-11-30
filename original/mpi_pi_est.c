#include<stdio.h>
#include<mpi.h>

#define NAREA 10E6
#define ROOT 0

void  compute_local_sum(int*, int*, double*, double*, double*);

int main(void){
  int i, comm_sz, my_rank, local_n, range[2];
  double step, x, sum, pi, local_sum, start_t , end_t;
  step = 1.0/NAREA;
  
  /*Start up MPI */
  MPI_Init(NULL,NULL);

  /* Get my rank among all the processors */
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);


  /* get number of processes */
  MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);

  /* number of areas per processor */
  local_n = NAREA/(comm_sz);
  if(my_rank == ROOT){
      printf("local_n is %d \n", local_n);
      start_t = MPI_Wtime();
	}
	range[0] = (my_rank)*local_n;
	range[1] = (my_rank+1)*local_n;
	
	compute_local_sum( range ,range+1,&step,&x, &local_sum);

  /* Add up the sums calculated by each process */
  if(my_rank != ROOT){
    MPI_Send(&local_sum, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
  
  }else{
      sum = local_sum;
      for(i = 1; i < comm_sz; i++){
      
        /*receive their sums */
        MPI_Recv(&local_sum, 1, MPI_DOUBLE, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        
        /*summ their sums with the main sum*/
        sum += local_sum;
        
        //printf("local sum received from procs %d is  %lf \n", i, local_sum);
        //printf("cummulative sum is %lf \n", sum);
      }   
}
  /* print the result */
  if(my_rank == ROOT){
    
    printf(" sum after loop is %lf\n", sum);
    pi = sum*step;
    end_t = MPI_Wtime();
    printf(" pi = %lf  \n", pi);
    printf("time is %lf \n" , end_t - start_t);
  }

  MPI_Finalize();
  return 0;
}

void compute_local_sum(int *start, int *end, double *step, double *x, double *sum){
  *sum=0.0;
  for(*start; *start < *end ; (*start)++){
    *x = (*start + 0.5) * *step;
    *sum += 4.0/(1.0 + *x**x);
  }

}

