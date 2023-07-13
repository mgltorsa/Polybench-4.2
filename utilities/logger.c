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

#include <logger.h>

#ifndef DEBUG
#define DEBUG 0
#endif

static void logger(const char *format, ...)
{

    if (DEBUG != 0)
    {
        va_list args;
        va_start(args, format);
        vprintf(format, args);
        va_end(args);
    }
}