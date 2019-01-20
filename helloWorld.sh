#!/bin/bash

# Setup my SBATCH directives
# time is hh:mm:ss
#SBATCH --time=1:00:00
#SBATCH --nodes=4
# -------SBATCH --ntasks=16
# -------SBATCH --account=owner-guest
# -------SBATCH --partition=kingspeak-guest
#SBATCH -o seanmcbride-%j

# Variables
NAME=helloWorld
DATADIR=/home/seanmcbride/mpi101
# SCRATCH=/lustre/groups/huanglab/sean
SLURM_NTASKS=4

# Setup Environment
module purge
module load openmpi

# Setup Scratch
# mkdir -p $SCRATCH

# copy data to scratch here if needed

# Run my job
mpirun -np $SLURM_NTASKS pw.x -inp $DATADIR/$NAME.in > $DATADIR/$NAME.$SLURM_JOB_ID.out
# mpirun -np $SLURM_NTASKS pw.x -inp $DATADIR/$NAME.in > $DATADIR/$NAME.$SLURM_JOB_ID.out
#srun pw.x -inp $DATADIR/$NAME.in > $DATADIR/$NAME.$SLURM_JOB_ID.out - depends on MPI

# Copy output from scratch
# copy data from strach here if needed

# Cleanup Scratch
# rm -rf $SCRATCH