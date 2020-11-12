/* ------------------------------------------------------------------------------
FILE NAME:         
DESCRIPTION:       
USAGE:             Compile with --std=c99
COMPILER:          GNU gcc compiler on Linux
NOTES:             
MODIFICATION HISTORY:
Author             Date               Version
---------------    ----------         --------------

-------------------------------------------------------------------------------- */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>


// Thread function to generate sum of 0 to n
void *sumRunner(void* arg);

// Global variables
// The sum computed by the background thread
long long sum = 0;

/* -----------------------------------------------------------------------------
FUNCTION:          
DESCRIPTION:       
RETURNS:         
NOTES:             
------------------------------------------------------------------------------- */
int main(int argc, char *argv[])
{
    if (argc > 2)
    {
        printf("Usage: %s <num>\n", argv[0]);
        exit(-1);
    }

    long long limit = atoll(argv[1]);



    // Thread ID
    pthread_t tID;

    // Create attributes
    pthread_attr_t attr;
    pthread_attr_init(&attr);

    // Runner is a function pointer, so no brackets 
    pthread_create(&tID, &attr, sumRunner, &limit);


    // Can do other stuff here...
    // Could spawn other threads to do some work. Or fo other 
    // computations, interact with the user.. Would then possibly be running on a 
    // different processor, or at least seeming to run concurrently with the background
    // work. 

    // Ensures that main thread will wait until the second thread
    // finishes.Wait until the thread has done its work. Don't want 
    // to do anything with the return value so return NULL
    pthread_join(tID, NULL); 
    printf("Sum is: %lld\n", sum); 

    return EXIT_SUCCESS;

}

/* -----------------------------------------------------------------------------
FUNCTION:          
DESCRIPTION:       
RETURNS:         
NOTES:             One thread which (main thread) starts in main, continues 
            execution until it hits the wait, holds, finishes and quits the program.
            Our second thread, spawned by pthread_create, goes into sumRunner, executes 
            down past the for loop and its all going to be inside the same address space, 
            so either thread can access the same global data. 
------------------------------------------------------------------------------- */
void *sumRunner(void* arg)
{
    long long *limitPTR = (long long*) arg;
    long long limit = *limitPTR;

    
    for(long long i = 0; i <= limit; i++)
    {
        sum += i;
    }

    // TODO: what to do with the answer?
    // Sum is a global variable, so other threads can access it. 

    // Thread needs to finish, 0 because we're not actually returning anything. Whatever gets
    // put into the exit gets passed back to pthread_join if that was non-NULL
    pthread_exit(0);

    return NULL;

}