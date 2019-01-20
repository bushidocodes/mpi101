#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[])
{
    int processId, b;
    int a[4] = {0, 0, 0, 0};

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &processId);

    if (processId == 0)
    {
        a[0] = 1;
        a[1] = 2;
        a[2] = 3;
        a[3] = 4;
    }

    MPI_Scatter(a, 1, MPI_INT, &b, 1, MPI_INT, 0, MPI_COMM_WORLD);

    fprintf(stderr, "Process %d: b = %d\n", processId, b);

    MPI_Finalize();
    return 0;
}