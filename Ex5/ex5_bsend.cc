// Exercise 5.2: messaage2.cc rewritten with Buffered Send (MPI_Bsend)
// Compile: mpicxx -o ex5_bsend ex5_bsend.cc
// Run:     mpirun -np 4 ./ex5_bsend
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
    int x[10], y[10];
    if (rank == 1) {
        for (int r = 0; r < 10; r++)
            x[r] = 10 * r;

        // 1. Work out how much buffer space one message of 10 ints needs
        int msg_size;
        MPI_Pack_size(10, MPI_INT, MPI_COMM_WORLD, &msg_size);
        int buf_size = msg_size + MPI_BSEND_OVERHEAD;

        // 2. Allocate a SEPARATE buffer and attach it to MPI.
        //    (We do not reuse x or y as the buffer - x stays untouched.)
        char *bsend_buf = (char *)malloc(buf_size);
        MPI_Buffer_attach(bsend_buf, buf_size);

        printf("Sending message to computer 3 from computer 1 (Bsend)\n");
        MPI_Bsend(x, 10, MPI_INT, 3, 0, MPI_COMM_WORLD);   // returns as soon as data is copied

        // x is still intact because MPI copied it into bsend_buf
        printf("Rank 1: x after Bsend:");
        for (int r = 0; r < 10; r++) printf(" %d", x[r]);
        printf("\n");

        // 3. Detach (waits until buffered messages are delivered), then free
        MPI_Buffer_detach(&bsend_buf, &buf_size);
        free(bsend_buf);
    }
    else if (rank == 3) {
        MPI_Recv(y, 10, MPI_INT, 1, 0, MPI_COMM_WORLD, &status);
        printf("in computer 3 the value of y is printed\n");
        for (int r = 0; r < 10; r++)
            printf(" %d ", y[r]);
        printf("\n");
    }
    else
        printf("Just a normal process From rank %d machine %s\n", rank, name);
    MPI_Finalize();
}
