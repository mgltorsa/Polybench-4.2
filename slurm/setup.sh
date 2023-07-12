#!/bin/bash

export CAVINESS_CACHE=28835840
export DARWIN_CACHE=16777216
export CACHE=$CAVINESS_CACHE
#export CACHE=$DARWIN_CACHE

# export MATRIX_MULT_M=1673
# export MATRIX_VECTOR_MULT_M=2500
# export MATRIX_VECTOR_MULT_N=3353
# export JACOBI_M=2897

# Uncomment when running in CAVINESS
export MATRIX_MULT_M=2805
export MATRIX_VECTOR_MULT_M=4000
export MATRIX_VECTOR_MULT_N=5896
export JACOBI_M=4858
