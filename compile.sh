#!/bin/bash

gcc -I utilities -I linear-algebra/kernels/3mm utilities/polybench.c linear-algebra/kernels/3mm/3mm-tiled-lv1.c -DPOLYBENCH_PAPI -lpapi -o atax_papi