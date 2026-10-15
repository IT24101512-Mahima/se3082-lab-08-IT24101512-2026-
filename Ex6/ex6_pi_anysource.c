/* Exercise 6: Exercise 3 with MPI_ANY_SOURCE on the receive side
 * Based on the Dartmouth serial example, 10,000,000 total samples.
 * Compile: mpicc -O2 -o ex6_pi_anysource ex6_pi_anysource.c
 * Run:     mpirun -np 4 ./ex6_pi_anysource
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
        /* receive in whatever order messages ARRIVE */
        for (int i = 1; i < size; i++) {
            MPI_Recv(&recv, 1, MPI_LONG_LONG, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
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
