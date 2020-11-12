/* ------------------------------------------------------------------------------
FILE NAME:         
DESCRIPTION:       
USAGE:             Compile with --std=c99
COMPILER:          GNU gcc compiler on Linux
NOTES:             
MODIFICATION HISTORY:
Author             Date               Version
---------------    ----------         --------------

----------------------------------------------------------------------------- */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>


/*
Here we create a pthread and pass it in data/return data via pointers plus 
running the same thread function in multiple separate threads. 
*/



// Thread function to generate sum of 0 to n
void *sumRunner(void* arg);

struct sumRunnerStruct{
    long long limit; 
    long long answer;
};

/* -----------------------------------------------------------------------------
FUNCTION:          
DESCRIPTION:       
RETURNS:         
NOTES:             
------------------------------------------------------------------------------- */
int main(int argc, char *argv[])
{
    if (argc < 2){
        printf("Usage: %s <num1> <num2> ... <num-n>\n", argv[0]);
        exit(-1);
    }

    int numArgs = argc - 1;


   


    /*Here we want to launch all the threads, let them run in parallel on all 
    of the CPUs and then wait for them all at the end. Need to be able to access 
    multiple thread IDs, use an array.*/
    
    // Thread IDs
    pthread_t tids[numArgs];

    //Each argument has its own sumRunnerStruct
    struct sumRunnerStruct args[numArgs];
    
    for(int i = 0; i < numArgs; i++) {
        args[i].limit = atoll(argv[i + 1]);

        /*Every loop iteration, we're gonna build a new struct and we're gonna pass
        in the pointer in the create function. */
        pthread_attr_t attr;
        pthread_attr_init(&attr);
        pthread_create(&tids[i], &attr, sumRunner, &args[i]);
    }
    


    // Wait until the thread has done its work. Don't want 
    // to do anything with the return value so return NULL
    for(int i = 0; i < numArgs; i++){
        pthread_join(tids[i], NULL);
        printf("Sum for thread %d is: %lld\n", i, args[i].answer);  
    }
    
    /*So this will create n threads and then each thread executes, each thread is passed its own structure of
    an argument as well as a place to put the return value. Passed a pointer in, execute multiple copies of the same
    thread function, all of the threads are doing the same work for us, they're just doing it on different data. */

    return EXIT_SUCCESS;

}

/* -----------------------------------------------------------------------------
FUNCTION:          
DESCRIPTION:       Thread function to generate the sum of 0 to n
RETURNS:         
NOTES:             The way it is from test1 only gives us one answer, even with the 
                multiple threads, because sum is globally shared. We wont get the right 
                answer because we had three separate copies of sumRunner(), all executing at
                once, all accessing a global variable and adding to it. There are likely to 
                be synchronization issues in the critical section (sum += 1;). Don't want to 
                continuously add to the same value and I can't just use a single global variable
                to return the value. So we want to pass back the answer. Dynamically allocate 
                some space and then pass a pointer back, next example. Can pass the value in
                somehow. 
------------------------------------------------------------------------------- */
void *sumRunner(void* arg)
{   
    /*Assuming that the caller passes in the appropriate argument here, we can access
    the limit as well as the answer. This is good bacause now we can pass any data 
    into our struct. We can begin adding extra fields, and access them in here. We are limited
    by passing in only a single pointer so the struct our way around that. */

    struct sumRunnerStruct *argStruct = (struct sumRunnerStruct*) arg;

    long long sum = 0;
    for(long long i = 0; i <= argStruct->limit; i++)
    {
        sum += i;
    }



    // We got the answer in sum and need to move it into the struct
    // that was passed in. 
    argStruct->answer = sum;
    
    pthread_exit(0);

    return NULL;

}