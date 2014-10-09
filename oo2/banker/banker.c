#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <pthread.h>
#include <stdbool.h>

bool smaller_or_equal(int *vec1, int *vec2, int size);

typedef struct state {
  int *resource;
  int *available;
  int **max;
  int **allocation;
  int **need;
} State;

// Global variables
int m, n;
State *s = NULL;

// Mutex for access to state.
pthread_mutex_t state_mutex;

/* Random sleep function */
void Sleep(float wait_time_ms)
{
  // add randomness
  wait_time_ms = ((float)rand())*wait_time_ms / (float)RAND_MAX;
  usleep((int) (wait_time_ms * 1e3f)); // convert from ms to us
}

/* Checks if the current state is deemed safe */
bool state_is_safe()
{
  int i, h, work[m], finish[n];
  bool no_such_i, all_true;

  // step 1, set up
  for (i = 0; i < n; i++)
    finish[i] = false;

  for (i = 0; i < m; i++)
    work[i] = s->available[i];

  no_such_i = true;

  while(no_such_i)
  {
    // step 2
    no_such_i = false;

    // keep looking for i's that match the description, but stop once you find one that does
    for (i = 0; i < n && no_such_i == false; i++)
      if (finish[i] == false && smaller_or_equal(s->need[i], work, m))
      {
        // step 3
        for (h = 0; h < m; h++)
          work[h] = work[h] + s->allocation[i][h];

        finish[i] = true;
        no_such_i = true;
      } 
  }

  // check step 4
  all_true = true;

  for (i = 0; i < n; i++)
    if (finish[i] == false)
      all_true = false;

  return all_true;
}

/* If vec1 os smaller or equal to vec 2, return true, else return false */
bool smaller_or_equal(int *vec1, int *vec2, int size)
{
  // TO DO
  int i;

  for (i = 0; i < size; i++)
  {
    if (vec1[i] > vec2[i])
      return false;
  }
  return true;
}

/* Allocate resources in request for process i, only if it 
   results in a safe state and return 1, else return 0 */
int resource_request(int i, int *request)
{
  if (state_is_safe())
  {
    // now do stuff!
  }

  return 0;
}

/* Release the resources in request for process i */
void resource_release(int i, int *request)
{
}

/* Generate a request vector */
void generate_request(int i, int *request)
{
  int j, sum = 0;
  while (!sum) {
    for (j = 0;j < n; j++) {
      request[j] = s->need[i][j] * ((double)rand())/ (double)RAND_MAX;
      sum += request[j];
    }
  }
  printf("Process %d: Requesting resources.\n",i);
}

/* Generate a release vector */
void generate_release(int i, int *request)
{
  int j, sum = 0;
  while (!sum) {
    for (j = 0;j < n; j++) {
      request[j] = s->allocation[i][j] * ((double)rand())/ (double)RAND_MAX;
      sum += request[j];
    }
  }
  printf("Process %d: Releasing resources.\n",i);
}

/* Threads starts here */
void *process_thread(void *param)
{
  /* Process number */
  int i = (int) (long) param, j;
  /* Allocate request vector */
  int *request = malloc(n*sizeof(int));
  while (1) {
    /* Generate request */
    generate_request(i, request);
    while (!resource_request(i, request)) {
      /* Wait */
      Sleep(100);
    }
    /* Generate release */
    generate_release(i, request);
    /* Release resources */
    resource_release(i, request);
    /* Wait */
    Sleep(1000);
  }
  free(request);
}

int main(int argc, char* argv[])
{
  /* Get size of current state as input */
  int i, j;
  printf("Number of processes: \n");
  scanf("%d", &m);
  printf("Number of resources: \n");
  scanf("%d", &n);

  /* Allocate memory for state */
  s = ( State *) malloc(1 * sizeof(State));
  
  if (s == NULL) { printf("\nYou need to allocate memory for the state!\n"); exit(0); };

  /* Get current state as input */
  s->resource = ( int * ) malloc(n * sizeof(int));
  printf("Resource vector: \n");
  for(i = 0; i < n; i++)
    scanf("%d", &s->resource[i]);

  s->max = ( int ** ) malloc(m * sizeof(int));
  printf("Enter max matrix: \n");
  for(i = 0;i < m; i++)
  {
    s->max[i] = ( int * ) malloc(n * sizeof(int));
    for(j = 0;j < n; j++)
      scanf("%d", &s->max[i][j]);
  }

  s->allocation = ( int ** ) malloc(m *  sizeof(int));
  printf("Enter allocation matrix: \n");
  for(i = 0; i < m; i++)
  {
    s->allocation[i] = ( int * ) malloc(n * sizeof(int));
    for(j = 0; j < n; j++)
      scanf("%d", &s->allocation[i][j]);
  } 
  printf("\n");

  /* Calcuate the need matrix */
  for(i = 0; i < m; i++)
    for(j = 0; j < n; j++)
      s->need[i][j] = s->max[i][j]-s->allocation[i][j];

  /* Calcuate the availability vector */
  for(j = 0; j < n; j++) {
    int sum = 0;
    for(i = 0; i < m; i++)
      sum += s->allocation[i][j];
    s->available[j] = s->resource[j] - sum;
  }

  /* Output need matrix and availability vector */
  printf("Need matrix:\n");
  for(i = 0; i < n; i++)
    printf("R%d ", i+1);
  printf("\n");
  for(i = 0; i < m; i++) {
    for(j = 0; j < n; j++)
      printf("%d  ",s->need[i][j]);
    printf("\n");
  }
  printf("Availability vector:\n");
  for(i = 0; i < n; i++)
    printf("R%d ", i+1);
  printf("\n");
  for(j = 0; j < n; j++)
    printf("%d  ",s->available[j]);
  printf("\n");

  /* If initial state is unsafe then terminate with error */

  /* Seed the random number generator */
  struct timeval tv;
  gettimeofday(&tv, NULL);
  srand(tv.tv_usec);
  
  /* Create m threads */
  pthread_t *tid = malloc(m*sizeof(pthread_t));
  for (i = 0; i < m; i++)
    pthread_create(&tid[i], NULL, process_thread, (void *) (long) i);
  
  /* Wait for threads to finish */
  pthread_exit(0);
  free(tid);

  /* Free state memory */
}
