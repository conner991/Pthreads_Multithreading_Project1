/* ------------------------------------------------------------------------------
FILE NAME:         par_sumsq.c
DESCRIPTION:       CS 446 Project 1 (Pthreads)
USAGE:             Compile with -std=c99
COMPILER:          GNU gcc compiler on Linux
NOTES:             
MODIFICATION HISTORY:
Author              Date                Version
---------------     ----------          --------------
Conner Fissell      11-08-2020          1.0 - Started messing around
                    11-11-2020          1.1 - Added inital struct, changed main and
                                              runner function to align with Pthreads
                    11-12-2020          1.2 - Added a LL queue. 
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

// Use default initialization
pthread_mutex_t queueMutex, globalVarMutex;

// Declaration of thread condition variable
pthread_cond_t cv1, cv2;

// function prototypes
void* createWorker(void* arg);
void calculateSquare(long num);
struct QueueNode* newNode(char task);
struct TaskQueue* createQueue();
void enQueue(struct TaskQueue* queue, long task);
long deQueue(struct TaskQueue* queue);
bool isEmpty(struct TaskQueue* queue);


// Linked List node that holds the task
struct QueueNode {
  long task;
  struct QueueNode* next;
};

// Front stores the front node, rear stores the previous node
struct TaskQueue {
  struct QueueNode *front, *rear;
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

  // argv1 one is the input file
  char *fn = argv[1];
  
  // argv2 is the number of threads entered on the command line
  int workerCount = atoi(argv[2]);

  // open up the input file
  FILE* fin = fopen(fn, "r");
  
  


  pthread_mutex_init(&queueMutex, NULL);
  pthread_mutex_init(&globalVarMutex, NULL);
  pthread_cond_init(&cv1, NULL);
  //pthread_cond_init(&cv2, NULL);

  volatile struct TaskQueue* taskQueue = createQueue();

  // Create worker threads
  pthread_t workers[workerCount];

  // Initialize Threads
  for(int i = 0; i < workerCount; i++){
    pthread_create(&workers[i], NULL, createWorker, (void*)taskQueue);
  }
  

  
  //int processNum = 0;
  

  char action;
  long num;
  

  while (fscanf(fin, "%c %ld\n", &action, &num) == 2) {
    
    if (action == 'p') {
      // process, do some work
      
      pthread_mutex_lock(&queueMutex);

      // enqueue a task
      enqueue(taskQueue, num);

      // condition signal
      pthread_cond_signal(&cv1);

      // unlock mutex
      pthread_mutex_unlock(&queueMutex);
      
      
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

  // close the input file
  fclose(fin);


  done = true;
  pthread_mutex_lock(&queueMutex);
  pthread_cond_broadcast(&cv1);
  pthread_mutex_unlock(&queueMutex);

  // Wait for the worker threads to finish
  for(int i = 0; i < workerCount; i++){
    pthread_join(workers[i], NULL);
  }

  // clean up tasks
  clearQueue(taskQueue);
  
  // print results
  printf("%ld %ld %ld %ld\n", sum, odd, min, max);
  
  // clean up and return
  return (EXIT_SUCCESS);
}


/* -----------------------------------------------------------------------------
FUNCTION:          
DESCRIPTION:       
RETURNS:         
NOTES:             
------------------------------------------------------------------------------- */
void* createWorker(void* arg)
{   
  struct TaskQueue* taskQueue = (struct TaskQueue*)arg;
  
  while(!done){
    pthread_mutex_lock(&queueMutex);

    while(!done && !isEmpty(taskQueue)){
      pthread_cond_wait(&cv1, &queueMutex);
    }

    if(done){
      pthread_mutex_unlock(&queueMutex);
      break;
    }

    long task = deQueue(taskQueue);
    pthread_mutex_unlock(&queueMutex);
    calculateSquare(task);

  }
  
  return NULL;

}


/* -----------------------------------------------------------------------------
FUNCTION:          
DESCRIPTION:      
RETURNS:         
NOTES:             
------------------------------------------------------------------------------- */
void calculateSquare(long taskNumber)
{


  // calculate the square
  long the_square = (taskNumber) * (taskNumber);

  // ok that was not so hard, but let's pretend it was
  // simulate how hard it is to square this number!
  sleep(taskNumber);

  pthread_mutex_lock(&globalVarMutex);

  // let's add this to our (global) sum
  sum += the_square;

  // now we also tabulate some (meaningless) statistics
  if (taskNumber % 2 == 1) {
    // how many of our numbers were odd?
    odd++;
  }

  // what was the smallest one we had to deal with?
  if (taskNumber < min) {
    min = taskNumber;
  }

  // and what was the biggest one?
  if (taskNumber > max) {
    max = taskNumber;
  }

  pthread_mutex_unlock(&globalVarMutex);

}

/* -----------------------------------------------------------------------------
FUNCTION:          
DESCRIPTION:       
RETURNS:         
NOTES:             
------------------------------------------------------------------------------- */
struct QueueNode* newNode(char task)
{
  struct QueueNode* temp = (struct QueueNode*)malloc(sizeof(struct QueueNode));
  temp->task = task;
  temp->next = NULL;

  return temp;
}

/* -----------------------------------------------------------------------------
FUNCTION:          
DESCRIPTION:       
RETURNS:         
NOTES:             
------------------------------------------------------------------------------- */
struct TaskQueue* createQueue()
{
  struct TaskQueue* queue = (struct TaskQueue*)malloc(sizeof(struct TaskQueue));
  queue->front = queue->rear = NULL;

  return queue;
}

/* -----------------------------------------------------------------------------
FUNCTION:          
DESCRIPTION:       
RETURNS:         
NOTES:             
------------------------------------------------------------------------------- */
void enQueue(struct TaskQueue* queue, long task)
{
  // create a new node
  struct QueueNode* temp = newNode(task);

  // if queue is empty, then make temp both the front and rear node
  if(queue->rear == NULL){
    queue->front = queue->rear = temp;
  }

  else{
    queue->rear->next = temp;
    queue->rear = temp; 
  }
}

/* -----------------------------------------------------------------------------
FUNCTION:          
DESCRIPTION:       
RETURNS:         
NOTES:             
------------------------------------------------------------------------------- */
long deQueue(struct TaskQueue* queue)
{
  // if queue is empty, then return NULL
  if(isEmpty(queue)){
    printf("The task queue is empty..\n");
    return 0;
  }

  else{
    // Store front in temp and move it one node back
    struct QueueNode* temp = queue->front;
    queue->front = temp->next;

    // if front becomes NULL, then rear becomes NULL
    if(queue->front == NULL)
      queue->rear  = NULL;

    long t = queue->front->task;

    free(temp); 

    return t;
  }
}

/* -----------------------------------------------------------------------------
FUNCTION:          
DESCRIPTION:       
RETURNS:         
NOTES:             
------------------------------------------------------------------------------- */
bool isEmpty(struct TaskQueue* queue)
{
  bool status;

  if(queue->front == NULL)
    status = false;
  else
    status = true;

  return status;
  
}

/* -----------------------------------------------------------------------------
FUNCTION:          
DESCRIPTION:       
RETURNS:         
NOTES:             
------------------------------------------------------------------------------- */
void clearQueue(struct TaskQueue* queue)
{
  long value;

  while(!isEmpty(queue)){
    value = dequeue(queue);
  }

  free(queue);
}

