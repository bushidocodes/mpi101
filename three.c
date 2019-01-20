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
    for (dest = 1; dest < noProcesses; dest++) /* distribute parameters */
    {
      MPI_Send(&numberRects, 1, MPI_INT, dest, 0, MPI_COMM_WORLD);
      MPI_Send(&lowerLimit, 1, MPI_DOUBLE, dest, 1, MPI_COMM_WORLD);
      MPI_Send(&upperLimit, 1, MPI_DOUBLE, dest, 2, MPI_COMM_WORLD);
    }
  }
  else /* all other processes receive */
  {
    src = 0;
    MPI_Recv(&numberRects, 1, MPI_INT, src, 0, MPI_COMM_WORLD, &status);
    MPI_Recv(&lowerLimit, 1, MPI_DOUBLE, src, 1, MPI_COMM_WORLD, &status);
    MPI_Recv(&upperLimit, 1, MPI_DOUBLE, src, 2, MPI_COMM_WORLD, &status);
  }

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

  /* collect information and print results */
  tag = 0;
  if (processId == 0)
  {
    total = area;
    for (src = 1; src < noProcesses; src++)
    {
      MPI_Recv(&area, 1, MPI_DOUBLE, src, tag, MPI_COMM_WORLD, &status);
      total += area;
    }
    fprintf(stderr, "The area from %f to %f is: %f\n", lowerLimit, upperLimit, total);
  }
  else
  {
    dest = 0;
    MPI_Send(&area, 1, MPI_DOUBLE, dest, tag, MPI_COMM_WORLD);
  }
  MPI_Finalize();
  return 0;
}