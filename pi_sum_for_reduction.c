#include<omp.h>
#include<stdio.h>
static long num_steps=100000;
double step;
#define NUM_THREADS 2
void main(){
    int i;
    double pi=0.0;
    step=1.0/(double) num_steps;
    double start,end;
    omp_set_num_threads(NUM_THREADS);
    start=omp_get_wtime();
    #pragma omp parallel
    {
        double x;
        #pragma omp for schedule(auto) reduction(+:pi)
        for(i=0;i<num_steps;i++){
            x=(i+0.5)*step;
            pi+=4.0/(1.0+x*x)*step;
        }
    }
    end=omp_get_wtime();
    printf("%f %f",end-start,pi);
}