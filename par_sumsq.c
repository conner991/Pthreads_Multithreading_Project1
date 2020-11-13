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
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

// Declaration of thread condition variable
pthread_cond_t cv1, cv2 = PTHREAD_COND_INITIALIZER;

// function prototypes
void* calculate_square(void* arg);
struct QueueNode* newNode(char task);
struct TaskQueue* createQueue();
void enQueue(struct TaskQueue* queue, char task);
void deQueue(struct TaskQueue* queue);


// Linked List node that holds the task
struct QueueNode {
  char task;
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

  char *fn = argv[1];
  int workerCount = atoi(argv[2]);

  // load numbers and add them to the queue
  FILE* fin = fopen(fn, "r");
  char action;
  long num;





  // Thread ID
  pthread_t thread1[workerCount];


  

  // Create worker threads
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

  long* number = (long*) arg;


  // calculate the square
  long the_square = (*number) * (*number);

  // ok that was not so hard, but let's pretend it was
  // simulate how hard it is to square this number!
  sleep(*number);

  // let's add this to our (global) sum
  sum += the_square;

  // now we also tabulate some (meaningless) statistics
  if (*number % 2 == 1) {
    // how many of our numbers were odd?
    odd++;
  }

  // what was the smallest one we had to deal with?
  if (*number < min) {
    min = *number;
  }

  // and what was the biggest one?
  if (*number > max) {
    max = *number;
  }

  pthread_exit(0);

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
  queue->front = queue->rear; 
  queue->rear = NULL;

  return queue;
}

/* -----------------------------------------------------------------------------
FUNCTION:          
DESCRIPTION:       
RETURNS:         
NOTES:             
------------------------------------------------------------------------------- */
void enQueue(struct TaskQueue* queue, char task)
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
void deQueue(struct TaskQueue* queue)
{
  // if queue is empty, then return NULL
  if(queue->front == NULL){
    printf("The task queue is empty..\n");
  }

  else{
    // Store front in temp and move it one node back
    struct QueueNode* temp = queue->front;
    queue->front = queue->front->next;

    // if front becomes NULL, then rear becomes NULL
    if(queue->front == NULL)
      queue->rear  = NULL;

    free(temp); 
  }
}