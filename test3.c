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


/*Use Dynamic Memory here. We can call a thread function, but in this case we passed
in some dynamically allocated (void * arg) which we used and extracted the value and 
stored it as a local variable and then freed it. We then do the usual processing inside
of the thread, and then when that's done we allocate some new space for the answer and 
pass it back (return the pointer using pthread_exit())

Down in main, when we call it, we allocate some space for the argument going in, set up that
value. We pass it in as the final argument to pthread_create, and then when we call pthread_join
this lets us get the value that was passed into pthread_exit(). Second param is a pointer to a 
pointer to the value*/


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
    if (argc < 2)
    {
        printf("Usage: %s <num>\n", argv[0]);
        exit(-1);
    }

    long long *limit = malloc(sizeof(*limit));
    *limit = atoll(argv[1]);



    // Thread ID
    pthread_t tid;

    // Create attributes
    pthread_attr_t attr;
    pthread_attr_init(&attr);

    /*No longer need to pass the address of limit because it is already 
    a pointer, if we passed the address we'd have to cast it in sumRunner(). */
    pthread_create(&tid, &attr, sumRunner, limit);


    /*We need to be able to hold a new value in that pointer so we pass in the 
    address of result. This says "give us the address of a pointer" and when we get that, we 
    create a pointer to a pointer. The value is now going to be passed back, we're passing back a 
    pointer, which is "answer in sumRunner()" Still have to cast it with (void*) tho... pthread_join is
    just saying: hey that pointer that was returned from the thread function, I'll give it to you. Heres 
    what it was.*/
    long long *result;
    pthread_join(tid, (void*)&result); 
    printf("Sum is: %lld\n", *result);
    free(result); 

    return EXIT_SUCCESS;

}

/* -----------------------------------------------------------------------------
FUNCTION:          
DESCRIPTION:       
RETURNS:         
NOTES:             
------------------------------------------------------------------------------- */
void *sumRunner(void* arg)
{
    long long *limitPTR = (long long*) arg;
    long long limit = *limitPTR;
    free(arg);

    
    for(long long i = 0; i <= limit; i++){
        sum += i;
    }

    /* Sizeof happens at compile timed so this is okay. Pass back data in dynamically 
    allocated memory. */
    long long *answer = malloc(sizeof(*answer));
    *answer = sum;
    // Give out a pointer to this memory that we have allocated. Won't be a memory leak
    // as long as someone calls free on it. 
    pthread_exit(answer);

    //return NULL;

}