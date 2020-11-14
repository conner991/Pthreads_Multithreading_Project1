

/*This guarantees that the child will run first and then the parent. The parent should
sleep ONLY IF the child hasn't run but if the child HAS run (done = true) then the 
parent will not wait it will go ahead and execute.  */




#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>


bool done = false;

// Use default initialization
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

// Declaration of thread condition variable
pthread_cond_t cv = PTHREAD_COND_INITIALIZER;


void thread_exit()
{
    pthread_mutex_lock(&mutex);
    done = true;
    pthread_cond_signal(&cv);
    pthread_mutex_unlock(&mutex);
}

void* child(void* arg)
{
    printf("child\n");
    thread_exit();
    return NULL;
}

void thread_join()
{
    pthread_mutex_lock(&mutex);
    while(done == false)
        pthread_cond_wait(&cv, &mutex);
    pthread_mutex_unlock(&mutex);
}

int main(int argc, char* argv[])
{
    printf("parent: begin\n");

    pthread_t thread;
    pthread_create(&thread, NULL, child, NULL);

    thread_join(); 

    printf("parent: end\n");

    return 0;
}




/* -----------------------------------------------------------------------------
FUNCTION:          
DESCRIPTION:       Producer/Consumer Ex
RETURNS:         
NOTES:             
------------------------------------------------------------------------------- */

pthread_cond_t empty, fill;
pthread_mutex_t mutex;

void* producer(void* arg)
{
    int i;
    for(i = 0; i < loops; i++){
        pthread_mutex_lock(&mutex);
        
        while(count == max){
            pthread_cond_wait(&empty, &mutex);
        }
        put(i);
        pthread_cond_signal(&fill);
        
        pthread_mutex_unlock(&mutex);
    }
}


void* consumer(void* arg)
{
    int i;
    for(i = 0; i < loops; i++){
        pthread_mutex_lock(&mutex);
        
        while(count == 0){
            pthread_cond_wait(&fill, &mutex);
        }
        int temp = get();
        pthread_cond_signal(&empty);
        
        pthread_mutex_unlock(&mutex);
        printf("%d\n", temp);
    }
}