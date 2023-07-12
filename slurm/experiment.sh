#!/bin/bash
#SBATCH --ntasks=1
#SBATCH --sockets-per-node=1
#SBATCH --nodes=1


for i in {1..10}; do
    "$@"
done