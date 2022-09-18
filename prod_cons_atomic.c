/*
**  PROGRAM: A simple serial producer/consumer program
**
**  One function generates (i.e. produces) an array of random values.  
**  A second functions consumes that array and sums it.
**
**  HISTORY: Written by Tim Mattson, April 2007.
*/
#include <omp.h>
#ifdef APPLE
#include <stdlib.h>
#else
#include <malloc.h>
#endif
#include <stdio.h>

#define N        10000

/* Some random number constants from numerical recipies */
#define SEED       2531
#define RAND_MULT  1366
#define RAND_ADD   150889
#define RAND_MOD   714025
int randy = SEED;
double *A;

/* function to fill an array with random numbers */
void fill_rand(int length)
{
   int i; 
   for (i=0;i<length;i++) {
     randy = (RAND_MULT * randy + RAND_ADD) % RAND_MOD;
     *(A+i) = ((double) randy)/((double) RAND_MOD);
   }   
}

/* function to sum the elements of an array */
double Sum_array(int length)
{
   int i;  double sum = 0.0;
   for (i=0;i<length;i++)  sum += *(A+i);  
   return sum; 
}
  
int main()
{
  double sum, runtime;
  int flag = 0,flg_tmp;

  A = (double *)malloc(N*sizeof(double));

  runtime = omp_get_wtime();
  #pragma omp parallel sections
  {
    #pragma omp section
    {
      fill_rand(N);        // Producer: fill an array of data
      #pragma omp flush
      #pragma atomic write
        flag=1;
      #pragma omp flush(flag)
    }
    #pragma omp section
    {
      while(1){
        #pragma omp flush(flag)
        #pragma atomic write 
            flg_tmp=flag;
            if(flg_tmp==1) break;
      }
      #pragma omp flush
      sum = Sum_array(N);  // Consumer: sum the array
    }
  }
   
  runtime = omp_get_wtime() - runtime;

  printf(" In %f seconds, The sum is %f \n",runtime,sum);
}
 