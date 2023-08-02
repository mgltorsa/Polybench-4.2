#!/bin/bash
#SBATCH --ntasks=1
#SBATCH --sockets-per-node=1
#SBATCH --threads-per-core=1
#SBATCH --nodes=1
#SBATCH --exclusive
#SBATCH --mail-type=END,FAIL            # Mail events (NONE, BEGIN, END, FAIL, ALL)
#SBATCH --mail-user=mgltorsa@udel.edu   # Where to send mail
#SBATCH --time=32:00:00
#SBATCH --constraint='Gen2'

## README! the flag --cores-per-socket it's supposed
## to be passed from the orquestator algorithm.
## however, to ensure we are only using physical cores, 
## we have defined --threads-per-core=1

. ./setup.sh

NUM_CORES=$SLURM_CPUS_PER_TASK
echo $NUM_CORES
export OMP_NUM_THREADS=$NUM_CORES


# Wait a minute for some debug tasks
sleep 10 & 
wait
# export OMP_NUM_THREADS=3

for i in {1..10}; do
    "$@"
done

