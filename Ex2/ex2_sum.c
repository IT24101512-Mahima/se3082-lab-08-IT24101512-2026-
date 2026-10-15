/* Exercise 2: Parallel sum of 1..10,000,000 using MPI
 * Compile: mpicc -O2 -o ex2_sum ex2_sum.c
 * Run:     mpirun -np 4 ./ex2_sum
 */
#include <stdio.h>
#include <mpi.h>

#define N 10000000LL

int main(int argc, char *argv[])
{
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    MPI_Barrier(MPI_COMM_WORLD);          /* start everyone together */
    double start = MPI_Wtime();

    /* Split 1..N into 'size' blocks; last rank takes any remainder */
    long long chunk = N / size;
    long long lo = rank * chunk + 1;
    long long hi = (rank == size - 1) ? N : lo + chunk - 1;

    long long local_sum = 0;
    for (long long i = lo; i <= hi; i++)
        local_sum += i;

    long long total = 0;
    MPI_Reduce(&local_sum, &total, 1, MPI_LONG_LONG, MPI_SUM, 0, MPI_COMM_WORLD);

    double elapsed = MPI_Wtime() - start;

    if (rank == 0) {
        printf("Processes : %d\n", size);
        printf("Sum       : %lld (expected %lld)\n", total, N * (N + 1) / 2);
        printf("Time      : %f s\n", elapsed);
        printf("CSV,%d,%f\n", size, elapsed);   /* line picked up by run_benchmarks.sh */
    }

    MPI_Finalize();
    return 0;
}
