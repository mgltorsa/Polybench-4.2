#!/bin/bash
#SBATCH
#SBATCH

# Defining parameters
coresList=(1 2 4 8 10)


# Running baselines


# running by cores

for cores in "${coresList[@]}"; do
    
    sbatch script --cores-per-socket="$cores"

# Running tile size experiments
    for tile_size in {1..10}; do

        $tile_size
    done

done

# Running
