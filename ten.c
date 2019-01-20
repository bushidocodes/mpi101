#include <stdio.h>
#include <mpi.h>

/* problem paramesters */
#define f(x) ((x) * (x))

main(int argc, char const *argv[])
{
    /* MPI Variables */
    int noProcesses, processId;

    /* Problem Variables */
    int i;
    double area, at, height, lower, width, total, range;

    struct ParamStruct
    {
        double lowerLimit;
        double upperLimit;
        int numberRects;
    } params;

    /* define type mirroring the native struct, but for MPI */
    MPI_Datatype rectStruct;                                             /* New MPI Datatype */
    int blocklengths[3] = {1, 1, 1};                                     /* Size of each block. If arrays, > 1 */
    MPI_Aint displacements[3] = {0, sizeof(double), 2 * sizeof(double)}; /* Displacements for where the blocks start. The type MPI_Aint is sized to be able to hold all possible addresses on the target architecture*/
    MPI_Datatype types[3] = {MPI_DOUBLE, MPI_DOUBLE, MPI_INT};
    MPI_Type_struct(3, blocklengths, displacements, types, &rectStruct);
    MPI_Type_commit(&rectStruct);

    /* MPI Setup */
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &noProcesses);
    MPI_Comm_rank(MPI_COMM_WORLD, &processId);

    if (processId == 0) /* if rank is 0, collect parameters */
    {
        fprintf(stderr, "Enter number of steps:\n");
        scanf("%d", &params.numberRects);
        fprintf(stderr, "Enter low end of interval:\n");
        scanf("%lf", &params.lowerLimit);
        fprintf(stderr, "Enter high end of interval:\n");
        scanf("%lf", &params.upperLimit);
    }
    MPI_Bcast(&params, 1, rectStruct, 0, MPI_COMM_WORLD);

    /* Adjust problem size for subproblem */
    range = (params.upperLimit - params.lowerLimit) / noProcesses;
    width = range / params.numberRects;
    lower = params.lowerLimit + range * processId;

    /* Calculate area for subproblem */
    area = 0.0;

    for (i = 0; i < params.numberRects; i++)
    {
        at = lower + i * width + width / 2.0;
        height = f(at);
        area = area + width * height;
    }

    MPI_Reduce(&area, &total, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    /* collect information and print results */
    if (processId == 0)
    {
        fprintf(stderr, "The area from %f to %f is: %f\n", params.lowerLimit, params.upperLimit, total);
    }

    /* Finish and Clean up */
    MPI_Finalize();

    return 0;
}
