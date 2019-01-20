#include <mpi.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
    int processId, i, flag = 0, color = 0;
    MPI_Comm birdsOfAFeather;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &processId);

    if (processId == 0 || processId == 1)
        color = 1;

    /* Break a single communicator into different communicators where the processes have the same "color" */
    /* Each process refers to this using the same local variable, but these now differ */

    MPI_Comm_split(MPI_COMM_WORLD, color, processId, &birdsOfAFeather);

    fprintf(stderr, "Process: %d   Flag: %d\n", processId, flag);

    if (processId == 0)
        flag = 1;
    if (processId == 2)
        flag = -1;

    /* */
    MPI_Bcast(&flag, 1, MPI_INT, 0, birdsOfAFeather);

    fprintf(stderr, "Process: %d   Flag: %d\n", processId, flag);

    MPI_Comm_free(&birdsOfAFeather);

    MPI_Finalize();
    return 0;
}