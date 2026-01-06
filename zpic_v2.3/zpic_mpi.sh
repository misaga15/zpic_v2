#!/bin/bash
#SBATCH --job-name=zpic_mpi
#SBATCH --account=f202500010hpcvlabuminhoa
#SBATCH --output=zpic_mpi_%j.out
#SBATCH --error=zpic_mpi_%j.err
#SBATCH --nodes=4
#SBATCH --ntasks-per-node=2
#SBATCH --partition=normal-arm
#SBATCH --time=00:30:00

# Load modules
ml GCC/13.3.0
ml OpenMPI/5.0.3-GCC-13.3.0

# Go to directory where you submitted the job
cd $SLURM_SUBMIT_DIR

# Compile
make clean
make

# Run (8 total MPI processes:  4 nodes × 2 tasks each)
echo "Running on $SLURM_NNODES nodes with $SLURM_NTASKS total MPI ranks"
srun ./zpic_mpi

echo "Job finished at $(date)"