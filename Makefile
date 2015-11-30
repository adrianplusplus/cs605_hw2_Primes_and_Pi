MCC= mpicc
CC = gcc
CCFLAGS = -lm 

all:  mpi_primes mpi_pi_est serial_primes serial_pi_est

mpi_primes: mpi_primes.c
	${MCC} -o mpi_primes  mpi_primes.c ${CCFLAGS}

mpi_pi_est: mpi_pi_est.c
	${MCC} -o mpi_pi_est mpi_pi_est.c  ${CCFLAGS}

serial_primes: serial_primes.c
	${CC} -o serial_primes serial_primes.c ${CCFLAGS}

serial_pi_est: serial_pi_est.c
	${CC} -o serial_pi_est serial_pi_est.c ${CCFLAGS}

clean:
	rm mpi_primes mpi_pi_est serial_primes serial_pi_est *.o

