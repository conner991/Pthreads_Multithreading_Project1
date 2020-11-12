/* ------------------------------------------------------------------------------
FILE NAME:         
DESCRIPTION:       
USAGE:             Compile with --std=c99
COMPILER:          GNU gcc compiler on Linux
NOTES:          
                First Situation:
                First we get a race condition, where the order of execution makes a 
                big difference in the actual processing. We need some way of doing synchronization
                because we have a critical section problem here, need to identify what the critical 
                section is and fix it with some form of locking mechanism. This problem happens when
                you have shared data between multiple threads. Here, the multiple threads are using the
                variable sum. Everything else is separated, we have separate instances of our threadFunction()
                running, we have separate variables being passed in, we have local variables used in main/threadFunction(),
                the only thing thats shared is sum. So we need to protect acces to sum in some form of critical section.
MODIFICATION HISTORY:
Author             Date               Version
---------------    ----------         --------------

----------------------------------------------------------------------------- */
#include <stdio.h>
#include <pthread.h>

#define NUM_LOOPS 500000000
long long sum = 0;

//Prototypes
void* countingThread(void*);

/* -----------------------------------------------------------------------------
FUNCTION:          
DESCRIPTION:       
RETURNS:         
NOTES:             
------------------------------------------------------------------------------- */
int main(void)
{   
    // Spawn threads
    pthread_t id1;
    int offset1 = 1;
    pthread_create(&id1, NULL, countingThread, &offset1);

    pthread_t id2;
    int offset2 = -1;
    pthread_create(&id2, NULL, countingThread, &offset2);
    

    /*Wait for threads to finish*/
    pthread_join(id1, NULL);
    pthread_join(id2, NULL);


    printf("Sum = %lld\n", sum);
    return 0; 
}

/* -----------------------------------------------------------------------------
FUNCTION:          
DESCRIPTION:       
RETURNS:         
NOTES:             Gonna convert our counting function to being a thread function, 
                which means that this is going to be the function that gets executed
                when a thread begins processing. 
------------------------------------------------------------------------------- */
void *countingThread(void* arg)
{   
    /*Takes arg, makes it a pointer to an int, then dereferences it (follow the pointer)
    then give me the value and then put that in an offset. */
    int offset = *(int*) arg;
    for(int i = 0; i < NUM_LOOPS; i++){
        sum += offset;
    }
    pthread_exit(NULL);
}