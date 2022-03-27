/******************************************************************************
 * * FILE: pthread.c
 * * DESCRIPTION:
 * *   Calculates the value of pi using a parallel implementation using pthreads. 
 * *   Also shows the time taken for the operation.
 * *
 * * HOW TO COMPILE:
 * *   gcc -lm -pthread pthreadTimed.c
 * * HOW TO RUN:
 * *    ./a.out
 * *
 * * AUTHOR: Ishan Thilina Somasiri
 * *
 * *
 * * Time measurement code is from - 
 * * http://stackoverflow.com/questions/3946842/measuring-time-taken-by-a-function-clock-gettime
 * ******************************************************************************/
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h> 
#define NUM_THREADS 100         //number of threads
#define TOT_COUNT 10000055      //total number of iterations
#define BILLION  1E9; 

/**
A random number generator. 
Guidance from from http://stackoverflow.com/a/3067387/1281089
**/
float randNumGen(){

   int random_value = rand(); //Generate a random number   
   float unit_random = random_value / (float) RAND_MAX; //make it between 0 and 1 
   return unit_random;
}

/**
The task allocated to a thread
**/
void *doThreadWork(void *iteration)
{
   double iterations;
   iterations = (double)iteration;
   
    //using malloc for the return variable in order make
   //sure that it is not destroyed once the thread call is finished
   float *in_subcount = (float *)malloc(sizeof(float));
   *in_subcount=0;
   
   
   
   int counter=0;
   
   //calculation
   for(counter=0;counter<iterations;counter++){
      float x = randNumGen();
      float y = randNumGen();
      
      float result = sqrt((x*x) + (y*y));
      
      if(result<1){
         *in_subcount+=1;         //check if the generated value is inside a unit circle
      }
      
   }
   
   
   }


   //printf("In count from #%d : %f\n",tid,*in_count);
   
   pthread_exit((void *)in_subcount);     //return the in count
}

int main(int argc, char *argv[])
{
   int rc;
   long t;
   void *status;
   float tot_in=0;
   
   
   char *a =  argc[1];
   int numThreads = atoi(a);
   
   *a =  argc[2];
   int iterations = atoi(a);
   
  pthread_t threads[numThreads];
  double threadIteration = iterations/numThreads;
   
  int remainder = iterations%numThreads;
   
  double threadIterationOne = threadIteration +remainder;
   
   // Calculate time taken by a request
  struct timespec requestStart, requestEnd;
  clock_gettime(CLOCK_REALTIME, &requestStart);
   
   for(t=0;t<numThreads;t++){

      if (t == 0)
           rc = pthread_create(&threads[t], NULL, doThreadWork, (void *)threadIterationOne);     
         else
      rc = pthread_create(&threads[t], NULL, doThreadWork, (void *)threadIteration);
     if (rc){
       printf("ERROR; return code from pthread_create() is %d\n", rc);
       exit(-1);
       }
     }

   //join the threads
   for(t=0;t<numThreads;t++){
           
      pthread_join(threads[t], &status);
      //printf("Return from thread %ld is : %f\n",t, *(float*)status);
      
      tot_in+=*(float*)status;            //keep track of the total in count
     
     }
     clock_gettime(CLOCK_REALTIME, &requestEnd);
     // Calculate time it took
     double accum = ( requestEnd.tv_sec - requestStart.tv_sec )
       + ( requestEnd.tv_nsec - requestStart.tv_nsec )
       / BILLION;
     printf( "Time taken: %lf\n", accum );
     printf("Value for Pi: %f \n",1, 4*(tot_in/TOT_COUNT));
   
   /* Last thing that main() should do */
   pthread_exit(NULL);
}
