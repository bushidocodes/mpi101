#include <mpi.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
    int processId;       /* rank of process */
    int numberProcesses; /* number of processes */
    int nameSize;        /* length of name */
    char computerName[MPI_MAX_PROCESSOR_NAME];

    MPI_Init(&argc, &argv);                          // Starts up MPI Session
    MPI_Comm_size(MPI_COMM_WORLD, &numberProcesses); // Sets numberProcesses to the number of processes running in the communicator MPI_COMM_WORLD
    MPI_Comm_rank(MPI_COMM_WORLD, &processId);       // Sets processId equal to the ID of this process in the default communicator MPI_COMM_WORLD

    MPI_Get_processor_name(computerName, &nameSize); // Sets nameSize equal to the hostname where the process is running
    fprintf(stderr, "Hello World from process %d on %s\n", processId, computerName);

    MPI_Finalize(); // Shuts down MPI Session
    return 0;
}