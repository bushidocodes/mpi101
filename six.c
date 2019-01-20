#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[])
{
    int processId;
    int a[4] = {0, 0, 0, 0};
    int b[4] = {0, 0, 0, 0};

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &processId);

    if (processId == 0)
        a[0] = 1;
    if (processId == 1)
        a[1] = 2;
    if (processId == 2)
        a[2] = 3;
    if (processId == 3)
        a[3] = 4;

    if (processId == 0)
        fprintf(stderr, "Before: b[ ] = [%d, %d, %d, %d]\n", b[0], b[1], b[2], b[3]);

    MPI_Gather(&a[processId], 1, MPI_INT, b, 1, MPI_INT, 0, MPI_COMM_WORLD);

    if (processId == 0)
        fprintf(stderr, "After: b[ ] = [%d, %d, %d, %d]\n", b[0], b[1], b[2], b[3]);

    MPI_Finalize();
    return 0;
}