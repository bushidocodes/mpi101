#include <stdio.h>
#include <mpi.h>

/* problem parameters */
/* We are using these to maintain readibility but maximize use of inlining for perf reasons */
#define f(x) ((x) * (x))
// #define numberRects 50
// #define lowerLimit 2.0
// #define upperLimit 5.0

int main(int argc, char *argv[])
{
  /* MPI Vars */
  int dest, noProcesses, processId, src, tag;
  MPI_Status status;

  /* Problem variables */
  int i, numberRects;
  double area, at, height, lower, width, total, range, upperLimit, lowerLimit;

  /* MPI Setup */
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &noProcesses);
  MPI_Comm_rank(MPI_COMM_WORLD, &processId);

  tag = 0;
  if (processId == 0) /* if rank is 0, collect parameters */
  {
    fprintf(stderr, "Enter number of steps:\n");
    scanf("%d", &numberRects);
    fprintf(stderr, "Enter low end of interval:\n");
    scanf("%lf", &lowerLimit);
    fprintf(stderr, "Enter high end of interval:\n");
    scanf("%lf", &upperLimit);
  }
  MPI_Bcast(&numberRects, 1, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Bcast(&lowerLimit, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  MPI_Bcast(&upperLimit, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);

  /* Adjust problem size for subproblem */
  range = (upperLimit - lowerLimit) / noProcesses;
  width = range / numberRects;
  lower = lowerLimit + range * processId;

  /* Calculate area for subproblem */
  area = 0.0;

  for (i = 0; i < numberRects; i++)
  {
    at = lower + i * width + width / 2.0;
    height = f(at);
    area = area + width * height;
  }

  MPI_Reduce(&area, &total, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

  /* collect information and print results */
  if (processId == 0)
  {
    fprintf(stderr, "The area from %f to %f is: %f\n", lowerLimit, upperLimit, total);
  }

  /* Finish and Clean up */
  MPI_Finalize();
  return 0;
}