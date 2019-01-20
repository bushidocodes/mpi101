#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[])
{
    int processId, i, flag = 0;
    int primary[1] = {0};
    MPI_Group worldGroup, newGroup;
    MPI_Comm secondary;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &processId);

    MPI_Comm_group(MPI_COMM_WORLD, &worldGroup);       /* Extract the group from the default communicator */
    MPI_Group_excl(worldGroup, 1, primary, &newGroup); /* Create a new group for secondaries by excluding primary process*/
    MPI_Comm_create(MPI_COMM_WORLD, newGroup, &secondary);

    /* Process 1 broadcasts the value of flag to the other secondaries */
    /* Note that the value of the primary node doesn't change*/
    fprintf(stderr, "Before: Process: %d   Flag: %d\n", processId, flag);
    if (processId == 1)
        flag = 1;

    /* We have to exclude 0 from the broadcast because it is not in the secondary group. If we haven't excluded it 
    explicitly, this would have resulted in a null communicator error*/
    if (processId != 0)
        /* processId 1 has a rank of 0 in the secondary communicator. The values are compacted... */
        MPI_Bcast(&flag, 1, MPI_INT, 0, secondary);
    fprintf(stderr, "After process: %d   Flag: %d\n", processId, flag);

    /* It is good housekeeping to clean up your custom groups and communicators */
    if (processId != 0)
    {
        MPI_Comm_free(&secondary);
        MPI_Group_free(&newGroup);
    }

    MPI_Finalize();
    return 0;
}