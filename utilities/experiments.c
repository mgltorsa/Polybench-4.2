#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <time.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sched.h>
#include <math.h>
#ifdef _OPENMP
#include <omp.h>
#endif

#include <experiments.h>

#ifndef DEBUG
#define DEBUG 0
#endif

#if defined(POLYBENCH_TIME)
#define EXPERIMENT "time"
#endif

#ifdef POLYBENCH_GFLOPS
#define EXPERIMENT "gflops"
#endif

#ifdef POLYBENCH_PAPI
#define EXPERIMENT "papi"
#endif

#if !defined(POLYBENCH_TIME) && !defined(POLYBENCH_GFLOPS) && !defined(POLYBENCH_PAPI)
#define EXPERIMENT "unknown"
#endif

void logger(const char *format, ...)
{

    if (DEBUG != 0)
    {
        va_list args;
        va_start(args, format);
        vprintf(format, args);
        va_end(args);
    }
}

char* get_experiment_type(){
    return EXPERIMENT;
}

char* slurm_get_node_id(){
    char *node_id= getenv("SLURM_NODEID");
    if(node_id == NULL){
        return "unknown";
    }
    return node_id;
}


void printe(char *algorithm, char *version, long size, int tile_size){
    printf("%s %s %ld %s %d ",algorithm, version, size, get_experiment_type(), tile_size);
}