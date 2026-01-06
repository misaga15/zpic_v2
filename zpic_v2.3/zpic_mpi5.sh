#!/bin/bash
#SBATCH --job-name=zpic_bench
#SBATCH --account=f202500010hpcvlabuminhoa
#SBATCH --output=zpic_bench_%j.out
#SBATCH --error=zpic_bench_%j.err
#SBATCH --nodes=4
#SBATCH --ntasks-per-node=2
#SBATCH --partition=normal-arm
#SBATCH --time=01:00:00

ml GCC/13.3.0
ml OpenMPI/5.0.3-GCC-13.3.0

cd $SLURM_SUBMIT_DIR

make clean
make

echo "=========================================="
echo "Running 5 iterations for benchmarking"
echo "=========================================="

for run in {1..5}; do
    echo ""
    echo "========== RUN $run =========="
    echo "Starting at $(date)"
    
    srun ./zpic_mpi
    
    echo "Finished at $(date)"
    echo "=============================="
done

echo ""
echo "All runs completed!"