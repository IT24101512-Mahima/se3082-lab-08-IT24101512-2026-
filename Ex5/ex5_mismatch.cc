// Exercise 5.1: source/destination mismatch (modified messages1.cc)
// Rank 1 sends to rank 3, but rank 3 waits for a message from rank 2.
// Compile: mpicxx -o ex5_mismatch ex5_mismatch.cc
// Run:     mpirun -np 4 ./ex5_mismatch      (needs at least 4 processes)
// Expected: the program HANGS (deadlock) - press Ctrl+C to stop it.
#include <cstdio>
#include <cstdlib>
#include <mpi.h>
int main(void)
{
    int rank;
    MPI_Status status;
    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    char name[MPI_MAX_PROCESSOR_NAME];
    int len;
    MPI_Get_processor_name(name, &len);
    int x, y;
    x = 30;
    if (rank == 1) {
        printf("Sending message to computer 3 from computer 1\n");
        fflush(stdout);
        MPI_Ssend(&x, 1, MPI_INT, 3, 0, MPI_COMM_WORLD);   // goes to 3
        printf("Rank 1: send completed\n");                // never printed
    }
    else if (rank == 3) {
        printf("Rank 3: waiting for a message from rank 2...\n");
        fflush(stdout);
        MPI_Recv(&y, 1, MPI_INT, 2, 0, MPI_COMM_WORLD, &status); // expects 2 -> mismatch
        printf("in computer 3 the value of y is %d\n", y); // never printed
    }
    else
        printf("Just a normal process From rank %d machine %s\n", rank, name);
    MPI_Finalize();
}
