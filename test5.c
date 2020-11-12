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

                Second Situation:
                Lets try using a Pthread Mutex for this. This is the simplest form of locking that will ensure Mutual 
                Exclusion. This gives us the right answer but increases the execution runtime by A LOT. So lets look at 
                Concurrency and Parallelism... 
                
                If two processes or threads are working in parallel that means that they are doing work at exactly the same time.
                Parallelism typically requires some kind of hardware support like multiple cores or some other co-processor. The concept of 
                concurrency is looser. If our machine only has one core, then it can only be running one thread at a time. But we really 
                have many threads in the system at any given time and so what it's going to do is run one thread for a short amount of time
                and then switch to the other for a short amount of time. It'll switch back and forth and the if it happens quick enough, the 
                user may not notice the switching. This is not parallel but definitely concurrent. As far as things appear, things are still 
                making progress at the same time. 
                
                Theres alot of things that can prevent us from getting parallelism in our threads. One of those is memory sharing (ex. Global variable)
                So the machine is trying to keep that memory coherent and it keeps shuttling memory back and forth, the OS might just stick both of these
                on the same core just to avoid all of this memory sharing contention. The sharing is preventing us from getting really good parallelism. 
                Now with locks, we have a lock and unlock function that is being called millions or billions of times and the overhead adds up. Thats why 
                it takes longer with the mutex locks. 

MODIFICATION HISTORY:
Author             Date               Version
---------------    ----------         --------------

----------------------------------------------------------------------------- */
#include <stdio.h>
#include <pthread.h>

//Prototypes
void* countingThread(void*);

#define NUM_LOOPS 50000000
long long sum = 0;

// Use default initialization
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

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
        
        /*So we create a mutex lock, before we use the shared resource we lock the mutex lock, then 
        when we're done we unlock the mutex lock. This forces all access to these lines of code to 
        be protected by a mutex which means only 1 thread can execute this area at once. */

        //Start critical section, Needs a pointer to a mutex
        pthread_mutex_lock(&lock);

        sum += offset;

        // End critical section
        pthread_mutex_unlock(&lock);
    }
    pthread_exit(NULL);
}