/* ------------------------------------------------------------------------------
FILE NAME:         par_sumsq.c
DESCRIPTION:       CS 446 Project 1 (Pthreads)
USAGE:             Compile with -std=c99
COMPILER:          GNU gcc compiler on Linux
NOTES:             
MODIFICATION HISTORY:
Author             Date               Version
---------------    ----------         --------------
Conner Fissell     11-08-2020         1.0 - Started messing around
----------------------------------------------------------------------------- */
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

// aggregate variables
long sum = 0;
long odd = 0;
long min = INT_MAX;
long max = INT_MIN;
bool done = false;

// function prototypes
void* calculate_square(void* arg);
void *runner(void *param);

struct MathRunnerStruct{
    long long limit; 
    long long answer;
};


/* -----------------------------------------------------------------------------
FUNCTION:          int main()
DESCRIPTION:       
RETURNS:           EXIT_SUCCESS or EXIT FAILURE
NOTES:             
------------------------------------------------------------------------------- */
int main(int argc, char* argv[])
{
  // check and parse command line options
  if ((argc < 3) || (argc > 3)) {
    printf("Usage: par_sumsq <infile> <NumOfWorkerThreads>\n");
    exit(EXIT_FAILURE);
  }

  char *fn = argv[1];
  int workerCount = atoi(argv[2]);

  // load numbers and add them to the queue
  FILE* fin = fopen(fn, "r");
  char action;
  long num;





  // Thread ID
  pthread_t thread1[workerCount];

  // Create thread
  pthread_create(&thread1, NULL, calculate_square, &num);



  while (fscanf(fin, "%c %ld\n", &action, &num) == 2) {
    
    if (action == 'p') {
      // process, do some work
      calculate_square(num);
    }

    else if (action == 'w') {
      // wait, nothing new happening
      sleep(num);
    }

    else {
      printf("ERROR: Unrecognized action: '%c'\n", action);
      exit(EXIT_FAILURE);
    }
  }






  fclose(fin);
  
  // print results
  printf("%ld %ld %ld %ld\n", sum, odd, min, max);
  
  // clean up and return
  return (EXIT_SUCCESS);
}


/* -----------------------------------------------------------------------------
FUNCTION:          
DESCRIPTION:     update global aggregate variables given a number  
RETURNS:         void
NOTES:             
------------------------------------------------------------------------------- */
void* calculate_square(void* arg)
{

  int* number = (int*) arg;


  // calculate the square
  long the_square = (*number) * (*number);

  // ok that was not so hard, but let's pretend it was
  // simulate how hard it is to square this number!
  sleep(*number);

  // let's add this to our (global) sum
  sum += the_square;

  // now we also tabulate some (meaningless) statistics
  if (*number % 2 == 1) 
  {
    // how many of our numbers were odd?
    odd++;
  }

  // what was the smallest one we had to deal with?
  if (*number < min) 
  {
    min = *number;
  }

  // and what was the biggest one?
  if (*number > max) 
  {
    max = *number;
  }

  pthread_exit(0);

}
