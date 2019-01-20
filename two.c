#include <stdio.h>
#include <mpi.h>

/* problem parameters */
/* We are using these to maintain readibility but maximize use of inlining for perf reasons */
#define f(x) ((x) * (x))
#define numberRects 50
#define lowerLimit 2.0
#define upperLimit 5.0

int main(int argc, char *argv[])
{
  /* MPI Vars */
  int dest, noProcesses, processId, src, tag;
  MPI_Status status;

  /* Problem variables */
  int i;
  double area, at, height, lower, width, total, range;

  /* MPI Setup */
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &noProcesses);
  MPI_Comm_rank(MPI_COMM_WORLD, &processId);

  /* Adjust problem size for subproblem */
  range = (upperLimit - lowerLimit) / noProcesses;
  width = range / numberRects;  // So the number of rectanges goes up with the number of processes? This scales accuracy with processes? It seems like we want a fixed accuracy and to scale up performance, right?
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
  if (processId == 0){
    total = area;
    for (src=1; src < noProcesses; src++) {
      MPI_Recv(&area, 1, MPI_DOUBLE, src, tag, MPI_COMM_WORLD, &status);
      total += area;
    }
    fprintf(stderr, "The area from %f to %f is: %f\n", lowerLimit, upperLimit, total);
  } else {
    dest = 0;
    MPI_Send(&area, 1, MPI_DOUBLE, dest, tag, MPI_COMM_WORLD);

  }
  MPI_Finalize();
  return 0;
}