#include <mpi.h>
#include <stdio.h>

/* problem parameters */
#define f(x) ((x) * (x))

int main(int argc, char *argv[])
{ /* MPI variables */
    int dest, noProcesses, processId;
    MPI_Status status; /* problem variables */
    int i, chunk, numberChunks, numberRects;
    double area, at, height, lower, width, total, range;
    double lowerLimit, upperLimit; /* MPI setup */
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &noProcesses);
    MPI_Comm_rank(MPI_COMM_WORLD, &processId);
    if (processId == 0) /* if rank is 0, collect parameters */
    {
        fprintf(stderr, "Enter number of chunk to divide problem into:\n");
        scanf("%d", &numberChunks);
        fprintf(stderr, "Enter number of steps per chunk:\n");
        scanf("%d", &numberRects);
        fprintf(stderr, "Enter low end of interval:\n");
        scanf("%lf", &lowerLimit);
        fprintf(stderr, "Enter high end of interval:\n");
        scanf("%lf", &upperLimit);
    }
    MPI_Bcast(&numberChunks, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&numberRects, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&lowerLimit, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(&upperLimit, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    /* collect information and print results */
    /* if rank is 0, assign chunk, collect results, print results */
    if (processId == 0)
    {
        total = 0.0;
        if (noProcesses - 1 < numberChunks)
            chunk = noProcesses - 1;
        else
            chunk = 0;
        for (i = 1; i <= numberChunks; i++)
        {
            MPI_Recv(&area, 1, MPI_DOUBLE, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            fprintf(stderr, "Area for process %d, is: %f\n", status.MPI_TAG, area);
            total = total + area;
            if (chunk != 0 && chunk < numberChunks)
                chunk++;
            else
                chunk = 0;
            MPI_Send(&chunk, 1, MPI_INT, status.MPI_TAG, chunk, MPI_COMM_WORLD);
        }
        fprintf(stderr, "The area from %f to %f is: %f\n", lowerLimit, upperLimit, total);
    }
    else
    /* all other processes, calculate area for chunk and send results */
    {
        if (processId > numberChunks)
            chunk = 0; /* too many processes */
        else
            chunk = processId;
        while (chunk != 0)
        {
            /* adjust problem size for subproblem */
            range = (upperLimit - lowerLimit) / numberChunks;
            width = range / numberRects;
            lower = lowerLimit + range * (chunk - 1); /* calculate area for this chunk */
            area = 0.0;
            for (i = 0; i < numberRects; i++)
            {
                at = lower + i * width + width / 2.0;
                height = f(at);
                area = area + width * height;
            }
            /* send results and get next chunk */
            dest = 0;
            MPI_Send(&area, 1, MPI_DOUBLE, dest, processId, MPI_COMM_WORLD);
            MPI_Recv(&chunk, 1, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        }
    } /* finish */
    MPI_Finalize();
    return 0;
}