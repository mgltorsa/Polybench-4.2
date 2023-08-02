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
#include <omp.h>
#include <polybench.h>

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


void printe(char *algorithm, char *version, long size, int tile_size, int cache_size){
    int num_threads, threads_max, thread_id=0;
  
    #pragma omp parallel
    {

        thread_id = omp_get_thread_num();
        if(thread_id == 0) {
            num_threads = omp_get_num_threads();
            threads_max = omp_get_max_threads(); 
        }
    }

    logger("CACHE: %d, TNms: %d, TMax: %d \n", cache_size, num_threads, threads_max);


    printf("%s %s %ld %d ", algorithm, version, size, cache_size);

    printf("%s %d %d ", get_experiment_type(), tile_size, threads_max);
}