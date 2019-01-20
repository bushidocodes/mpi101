#include <stdio.h>
#include <mpi.h>

/* problem parameters */

int main(int argc, char *argv[])
{
    /* MPI Vars */
    int noProcesses, processId, src, tag;
    MPI_Status status;
    MPI_Request handle;

    /* Problem variables */
    int datum1, datum2;

    /* MPI Setup */
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &noProcesses);
    MPI_Comm_rank(MPI_COMM_WORLD, &processId);

    tag = 0;
    if (processId == 0) /* if rank is 0, collect parameters */
    {
        datum1 = 1985;
        MPI_Bcast(&datum1, 1, MPI_INT, 0, MPI_COMM_WORLD);
    }
    else
    {
        int dest = 0;

        MPI_Irecv(&datum2, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, &handle);
        MPI_Wait(&handle, &status);
        printf("Process %d datum2: %d", processId, datum2);
    }

    /* Finish and Clean up */
    MPI_Finalize();
    return 0;
}