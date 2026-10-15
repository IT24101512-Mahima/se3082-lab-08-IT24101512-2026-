/* Exercise 3: Monte Carlo Pi with MPI (point-to-point Send/Recv)
 * Based on the Dartmouth serial example, 10,000,000 total samples.
 * Compile: mpicc -O2 -o ex3_pi ex3_pi.c
 * Run:     mpirun -np 4 ./ex3_pi
 */
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define NITER 10000000LL

int main(int argc, char *argv[])
{
    int rank, size;
    MPI_Status status;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    MPI_Barrier(MPI_COMM_WORLD);
    double start = MPI_Wtime();

    /* each rank does its share of the samples */
    long long my_iter = NITER / size;
    if (rank == size - 1) my_iter += NITER % size;

    unsigned int seed = 12345u + 1000u * rank;   /* different random stream per rank */
    long long count = 0;
    for (long long i = 0; i < my_iter; i++) {
        double x = (double)rand_r(&seed) / RAND_MAX;
        double y = (double)rand_r(&seed) / RAND_MAX;
        if (x * x + y * y <= 1.0) count++;
    }

    if (rank != 0) {
        /* workers send their hit count to rank 0, tag 0 */
        MPI_Send(&count, 1, MPI_LONG_LONG, 0, 0, MPI_COMM_WORLD);
    } else {
        long long total = count, recv;
        /* receive in fixed order: 1, 2, 3, ... */
        for (int src = 1; src < size; src++) {
            MPI_Recv(&recv, 1, MPI_LONG_LONG, src, 0, MPI_COMM_WORLD, &status);
            printf("Received %lld from rank %d\n", recv, status.MPI_SOURCE);
            total += recv;
        }
        double pi = 4.0 * (double)total / (double)NITER;
        double elapsed = MPI_Wtime() - start;
        printf("Processes : %d\n", size);
        printf("Samples   : %lld\n", NITER);
        printf("Pi approx : %.8f\n", pi);
        printf("Time      : %f s\n", elapsed);
        printf("CSV,%d,%f\n", size, elapsed);
    }

    MPI_Finalize();
    return 0;
}
