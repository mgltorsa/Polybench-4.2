#!/bin/bash

# Imports cache
. ./slurm/setup.sh

# CACHE_SIZE=$CACHE
CACHE_SIZE=8000

# BIN FOLDER
BIN_FOLDER=bin

# GCC FLAGS

# DEBUG_FLAG="-DDEBUG"
GCC_FLAGS="-lpapi -fopenmp"
POLY_FLAGS="-DMINI_DATASET -DPOLYBENCH_CACHE_SIZE_KB=$CACHE_SIZE $DEBUG_FLAG"
POLY_UTILITIES="-I utilities utilities/polybench.c utilities/experiments.c"
ALGO_UTILITIES="-I linear-algebra/kernels/3mm"
GCC_OPTIMIZATIONS="-O0"


POLY_PAPI_FLAGS="-DPOLYBENCH_PAPI"

# 3MM Experiments
gcc $GCC_OPTIMIZATIONS $POLY_UTILITIES $ALGO_UTILITIES $POLY_FLAGS $POLY_PAPI_FLAGS linear-algebra/kernels/3mm/3mm.c $GCC_FLAGS -o $BIN_FOLDER/3mm_papi 
gcc $GCC_OPTIMIZATIONS $POLY_UTILITIES $ALGO_UTILITIES $POLY_FLAGS $POLY_PAPI_FLAGS linear-algebra/kernels/3mm/3mm-parallel.c $GCC_FLAGS -o $BIN_FOLDER/3mm_parallel_papi 
gcc $GCC_OPTIMIZATIONS $POLY_UTILITIES $ALGO_UTILITIES $POLY_FLAGS $POLY_PAPI_FLAGS linear-algebra/kernels/3mm/3mm-tiled-lv1.c $GCC_FLAGS -o $BIN_FOLDER/3mm_tiled-lv1_papi 
gcc $GCC_OPTIMIZATIONS $POLY_UTILITIES $ALGO_UTILITIES $POLY_FLAGS $POLY_PAPI_FLAGS linear-algebra/kernels/3mm/3mm-tiled-lv2.c $GCC_FLAGS -o $BIN_FOLDER/3mm_tiled-lv2_papi 


POLY_TIME_FLAGS="-DPOLYBENCH_TIME"
gcc $GCC_OPTIMIZATIONS $POLY_UTILITIES $ALGO_UTILITIES $POLY_FLAGS $POLY_TIME_FLAGS linear-algebra/kernels/3mm/3mm.c $GCC_FLAGS -o $BIN_FOLDER/3mm_time
gcc $GCC_OPTIMIZATIONS $POLY_UTILITIES $ALGO_UTILITIES $POLY_FLAGS $POLY_TIME_FLAGS linear-algebra/kernels/3mm/3mm-parallel.c $GCC_FLAGS -o $BIN_FOLDER/3mm_parallel_time
gcc $GCC_OPTIMIZATIONS $POLY_UTILITIES $ALGO_UTILITIES $POLY_FLAGS $POLY_TIME_FLAGS linear-algebra/kernels/3mm/3mm-tiled-lv1.c $GCC_FLAGS -o $BIN_FOLDER/3mm_tiled-lv1_time 
gcc $GCC_OPTIMIZATIONS $POLY_UTILITIES $ALGO_UTILITIES $POLY_FLAGS $POLY_TIME_FLAGS linear-algebra/kernels/3mm/3mm-tiled-lv2.c $GCC_FLAGS -o $BIN_FOLDER/3mm_tiled-lv2_time 


# JACOBI Experiments

