/**
 * This version is stamped on May 10, 2016
 *
 * Contact:
 *   Louis-Noel Pouchet <pouchet.ohio-state.edu>
 *   Tomofumi Yuki <tomofumi.yuki.fr>
 *
 * Web address: http://polybench.sourceforge.net
 */
/* 3mm.c: this file is part of PolyBench/C */

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include <sys/time.h>
#include <omp.h>

/* Include polybench common header. */
#include <polybench.h>

/* Include benchmark-specific header. */
#include "3mm.h"

/* Custom utilities */
#include <experiments.h>

static int balancedTileSize;
static int cores;

/* Array initialization. */
static void init_array(int ni, int nj, int nk, int nl, int nm,
                       DATA_TYPE POLYBENCH_2D(A, NI, NK, ni, nk),
                       DATA_TYPE POLYBENCH_2D(B, NK, NJ, nk, nj),
                       DATA_TYPE POLYBENCH_2D(C, NJ, NM, nj, nm),
                       DATA_TYPE POLYBENCH_2D(D, NM, NL, nm, nl))
{
  int i, j;

  for (i = 0; i < ni; i++)
    for (j = 0; j < nk; j++)
      A[i][j] = (DATA_TYPE)((i * j + 1) % ni) / (5 * ni);
  for (i = 0; i < nk; i++)
    for (j = 0; j < nj; j++)
      B[i][j] = (DATA_TYPE)((i * (j + 1) + 2) % nj) / (5 * nj);
  for (i = 0; i < nj; i++)
    for (j = 0; j < nm; j++)
      C[i][j] = (DATA_TYPE)(i * (j + 3) % nl) / (5 * nl);
  for (i = 0; i < nm; i++)
    for (j = 0; j < nl; j++)
      D[i][j] = (DATA_TYPE)((i * (j + 2) + 2) % nk) / (5 * nk);
}

/* DCE code. Must scan the entire live-out data.
   Can be used also to check the correctness of the output. */
static void print_array(int ni, int nl,
                        DATA_TYPE POLYBENCH_2D(G, NI, NL, ni, nl))
{
  int i, j;

  POLYBENCH_DUMP_START;
  POLYBENCH_DUMP_BEGIN("G");
  for (i = 0; i < ni; i++)
    for (j = 0; j < nl; j++)
    {
      if ((i * ni + j) % 20 == 0)
        fprintf(POLYBENCH_DUMP_TARGET, "\n");
      fprintf(POLYBENCH_DUMP_TARGET, DATA_PRINTF_MODIFIER, G[i][j]);
    }
  POLYBENCH_DUMP_END("G");
  POLYBENCH_DUMP_FINISH;
}

/* Main computational kernel. The whole function will be timed,
   including the call and return. */
static void kernel_3mm(int ni, int nj, int nk, int nl, int nm,
                       DATA_TYPE POLYBENCH_2D(E, NI, NJ, ni, nj),
                       DATA_TYPE POLYBENCH_2D(A, NI, NK, ni, nk),
                       DATA_TYPE POLYBENCH_2D(B, NK, NJ, nk, nj),
                       DATA_TYPE POLYBENCH_2D(F, NJ, NL, nj, nl),
                       DATA_TYPE POLYBENCH_2D(C, NJ, NM, nj, nm),
                       DATA_TYPE POLYBENCH_2D(D, NM, NL, nm, nl),
                       DATA_TYPE POLYBENCH_2D(G, NI, NL, ni, nl))
{
  int i, j, k;
  
  #pragma scop
  /* E := AB */
  int jj;
  int jTile = balancedTileSize;
  #pragma loop name kernel_3mm #1
  #pragma cetus private(i, j, jj, k)
  for ((jj = 0); jj < nj; jj += jTile)
  {
    #pragma loop name kernel_3mm #1 #0
    #pragma cetus private(i, j, k)
    #pragma cetus parallel
    #pragma omp parallel for private(i, j, k)
    for (i = 0; i < ni; i++)
    {
      #pragma loop name kernel_3mm #1 #0 #0
      #pragma cetus private(j, k)
      for ((j = jj); j < ((((-1 + jTile) + jj) < nj) ? ((-1 + jTile) + jj) : nj); j++)
      {
        #pragma loop name kernel_3mm #1 #0 #0 #0
        #pragma cetus private(k)
        for (k = 0; k < nk; ++k)
        {
          E[i][j] += (A[i][k] * B[k][j]);
        }
      }
    }
  }

  /* F := CD */

  #pragma loop name kernel_3mm #3
  #pragma cetus private(i, j, jj, k)
  for ((jj = 0); jj < nl; jj += jTile)
  {
    #pragma loop name kernel_3mm #3 #0
    #pragma cetus private(i, j, k)
    #pragma cetus parallel
    #pragma omp parallel for private(i, j, k)
    for (i = 0; i < nj; i++)
    {
      #pragma loop name kernel_3mm #3 #0 #0
      #pragma cetus private(j, k)
      for ((j = jj); j < ((((-1 + jTile) + jj) < nl) ? ((-1 + jTile) + jj) : nl); j++)
      {
        #pragma loop name kernel_3mm #3 #0 #0 #0
        #pragma cetus private(k)
        for (k = 0; k < nm; ++k)
        {
          F[i][j] += (C[i][k] * D[k][j]);
        }
      }
    }
  }

  /* G := EF */

  #pragma loop name kernel_3mm #5
  #pragma cetus private(i, j, jj, k)
  for ((jj = 0); jj < nl; jj += jTile)
  {
    #pragma loop name kernel_3mm #5 #0
    #pragma cetus private(i, j, k)
    #pragma cetus parallel
    #pragma omp parallel for private(i, j, k)
    for (i = 0; i < ni; i++)
    {
      #pragma loop name kernel_3mm #5 #0 #0
      #pragma cetus private(j, k)
      for ((j = jj); j < ((((-1 + jTile) + jj) < nl) ? ((-1 + jTile) + jj) : nl); j++)
      {
        #pragma loop name kernel_3mm #5 #0 #0 #0
        #pragma cetus private(k)
        for (k = 0; k < nj; ++k)
        {
          G[i][j] += (E[i][k] * F[k][j]);
        }
      }
    }
  }

  #pragma endscop

  // for (i = 0; i < ni; i++)
  //   for (j = 0; j < nl; j++)
  //      printf("G[%d][%d]=%f\n", i,j, G[i][j]);
}

int main(int argc, char **argv)
{
  /* Retrieve problem size. */
  int ni = NI;
  int nj = NJ;
  int nk = NK;
  int nl = NL;
  int nm = NM;


  if(argc > 1 && argv[1] != "") {
    balancedTileSize = atoi(argv[1]);
  }else{
    printf("NO BALANCED TILE SIZE ENV");
    return -1;
  }



  int i, j;

  /* Variable declaration/allocation. */
  POLYBENCH_2D_ARRAY_DECL(E, DATA_TYPE, NI, NJ, ni, nj);
  POLYBENCH_2D_ARRAY_DECL(A, DATA_TYPE, NI, NK, ni, nk);
  POLYBENCH_2D_ARRAY_DECL(B, DATA_TYPE, NK, NJ, nk, nj);
  POLYBENCH_2D_ARRAY_DECL(F, DATA_TYPE, NJ, NL, nj, nl);
  POLYBENCH_2D_ARRAY_DECL(C, DATA_TYPE, NJ, NM, nj, nm);
  POLYBENCH_2D_ARRAY_DECL(D, DATA_TYPE, NM, NL, nm, nl);
  POLYBENCH_2D_ARRAY_DECL(G, DATA_TYPE, NI, NL, ni, nl);

  /* Initialize array(s). */
  init_array(ni, nj, nk, nl, nm,
             POLYBENCH_ARRAY(A),
             POLYBENCH_ARRAY(B),
             POLYBENCH_ARRAY(C),
             POLYBENCH_ARRAY(D));

  /* Start timer. */
  polybench_start_instruments;

  for (i = 0; i < ni; i++)
    for (j = 0; j < nj; j++)
      (*E)[i][j] = SCALAR_VAL(0.0);
  for (i = 0; i < nj; i++)
    for (j = 0; j < nl; j++)
      (*F)[i][j] = SCALAR_VAL(0.0);
  for (i = 0; i < ni; i++)
    for (j = 0; j < nl; j++)
      (*G)[i][j] = SCALAR_VAL(0.0);

  /* Run kernel. */
  kernel_3mm(ni, nj, nk, nl, nm,
             POLYBENCH_ARRAY(E),
             POLYBENCH_ARRAY(A),
             POLYBENCH_ARRAY(B),
             POLYBENCH_ARRAY(F),
             POLYBENCH_ARRAY(C),
             POLYBENCH_ARRAY(D),
             POLYBENCH_ARRAY(G));

  /* Stop and print timer. */
  polybench_stop_instruments;

  //TODO: Print in an specific format
  printe("3mm","til-1", (long)ni+nj+nl+nm+nk,balancedTileSize, POLYBENCH_CACHE_SIZE_KB);
  polybench_print_instruments;

  /* Prevent dead-code elimination. All live-out data must be printed
     by the function call in argument. */
  polybench_prevent_dce(print_array(ni, nl, POLYBENCH_ARRAY(G)));

  /* Be clean. */
  POLYBENCH_FREE_ARRAY(E);
  POLYBENCH_FREE_ARRAY(A);
  POLYBENCH_FREE_ARRAY(B);
  POLYBENCH_FREE_ARRAY(F);
  POLYBENCH_FREE_ARRAY(C);
  POLYBENCH_FREE_ARRAY(D);
  POLYBENCH_FREE_ARRAY(G);

  return 0;
}
