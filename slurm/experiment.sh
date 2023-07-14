#!/bin/bash
#SBATCH --ntasks=1
#SBATCH --sockets-per-node=1
#SBATCH --threads-per-core=1
#SBATCH --nodes=1

## README! the flag --cores-per-socket it's supposed
## to be passed from the orquestator algorithm.
## however, to ensure we are only using physical cores, 
## we have defined --threads-per-core=1


# NUM_CORES="$SLURM_CPUS_PER_TASK"

# export OMP_NUM_THREADS=$NUM_CORES

export OMP_NUM_THREADS=2

for i in {1..10}; do
    "$@"
done

