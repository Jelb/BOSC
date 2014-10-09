#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <pthread.h>
#include <stdbool.h>

typedef struct state {
  int *resource;
  int *available;
  int **max;
  int **allocation;
  int **need;
} State;

bool smaller_or_equal(int *vec1, int *vec2, int size);
State *copy_state(State *state, int a, int b);

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
bool state_is_safe(State *st)
{
  int i, h, work[m], finish[n];
  bool no_such_i, all_true;

  // step 1, set up
  for (i = 0; i < n; i++)
    finish[i] = false;

  for (i = 0; i < m; i++)
    work[i] = st->available[i];

  no_such_i = true;

  while(no_such_i)
  {
    // step 2
    no_such_i = false;

    // keep looking for i's that match the description, but stop once you find one that does
    for (i = 0; i < n && no_such_i == false; i++)
      if (finish[i] == false && smaller_or_equal(st->need[i], work, m))
      {
        // step 3
        for (h = 0; h < m; h++)
          work[h] = work[h] + st->allocation[i][h];

        finish[i] = true;
        no_such_i = true;
      } 
  }

  // step 4
  all_true = true;

  for (i = 0; i < n; i++)
    if (finish[i] == false)
      all_true = false;

  return all_true;
}

/* If vec1 os smaller or equal to vec 2, return true, else return false */
bool smaller_or_equal(int *vec1, int *vec2, int size)
{
  int i;

  for (i = 0; i < size; i++)
    if (vec1[i] > vec2[i])
      return false;

  return true;
}

/* Allocate resources in request for process i, only if it 
   results in a safe state and return 1, else return 0 */
int resource_request(int i, int *request)
{
  if (state_is_safe(s))
  {
    // step 1
    if (smaller_or_equal(request, s->need[i], m))
    {
      // step 2
      while (smaller_or_equal(request, s->available, m) == false)
      {
        printf("request exceededs availabel, sleeping a bit...\n");
        Sleep(1000);
      }

      // step 3
      // kopier s over i pretend_state
      State *pretend_state;
      // pretend_state = ( State *) malloc(1 * sizeof(State));
      pretend_state = copy_state(s, n, m);
      // udfør på pretend_state følgende ændringer:
      // 
      // avail   = avail - req-i
      // alloc-i = alloc-i + req-i
      // need-i  = need-i - req-i

      int k;
      for (k = 0; k < m; k++)
        pretend_state->available[k] = pretend_state->available[k] - request[k];

      for (k = 0; k < m; k++)
        pretend_state->allocation[i][k] = pretend_state->allocation[i][k] + request[k];

      for (k = 0; k < m; k++)
        pretend_state->need[i][k] = pretend_state->need[i][k] - request[k];
      
      // check at pretend_state er safe
      // hvis den er, kopier indhold fra pretend_state til s og returner 1,
      // ellers returner 0
      if (state_is_safe(pretend_state))
      {
        s = copy_state(pretend_state, n, m);
        free(pretend_state);
        return 1;
      }
      else
        return 0;
    }
    else
    {
      printf("process has exceeded its maximum claim\n");
      return 0;
    }
  }

  return 0;
}

State *copy_state(State *state, int a, int b)
{
  int i, h;

  State *cpy_state;
  cpy_state = ( State *) malloc(1 * sizeof(State));

  cpy_state->resource = ( int * ) malloc(a * sizeof(int));
  for (i = 0; i < a; i++)
    cpy_state->resource[i] = state->resource[i];

  cpy_state->available = ( int * ) malloc(a * sizeof(int));
  for (i = 0; i < a; i++)
    cpy_state->available[i] = state->available[i];

  cpy_state->max = ( int ** ) malloc(b * sizeof(int));
  for (i = 0; i < b; i++)
  {
    cpy_state->max[i] = ( int * ) malloc(a * sizeof(int));
    for (h = 0; h < a; h++)
      cpy_state->max[i][h] = state->max[i][h];
  }

  cpy_state->allocation = ( int ** ) malloc(b * sizeof(int));
  for (i = 0; i < b; i++)
  {
    cpy_state->allocation[i] = ( int * ) malloc(a * sizeof(int));
    for (h = 0; h < a; h++)
      cpy_state->allocation[i][h] = state->allocation[i][h];
  }

  cpy_state->need = ( int ** ) malloc(b * sizeof(int));
  for (i = 0; i < b; i++)
  {
    cpy_state->need[i] = ( int * ) malloc(a * sizeof(int));
    for (h = 0; h < a; h++)
      cpy_state->need[i][h] = state->need[i][h];
  }

  return cpy_state;
}

/* Release the resources in request for process i */
/* Snuppet fra Jonas' version */
void resource_release(int i, int *request)
{
  int j;
  for (j = 0; j < m; j++) {
      s->available[j] += request[j];
      s->allocation[i][j] -= request[j];
      s->need[i][j] += request[j];
  }
  printf("\n{");
  for (j = 0; j < m; j++) {
      printf("[%i]", s->allocation[i][j]);
  }
  printf("}\n");
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

  s = (State *) malloc(sizeof(State));
  s->resource   = (int *)malloc(n * sizeof(int));
  s->available  = (int *)malloc(n * sizeof(int));
  s->max        = (int **)malloc(m * sizeof(int *));
  s->allocation = (int **)malloc(m * sizeof(int *));
  s->need       = (int **)malloc(m * sizeof(int *));
  
  for (i = 0; i < m; i++) {
    s->max[i]           = (int *) malloc(n * sizeof(int));
    s->allocation[i]    = (int *) malloc(n * sizeof(int));
    s->need[i]          = (int *) malloc(n * sizeof(int));
  }
  
  if (s == NULL) { printf("\nYou need to allocate memory for the state!\n"); exit(0); };

  /* Get current state as input */
  printf("Resource vector: \n");
  for(i = 0; i < n; i++)
    scanf("%d", &s->resource[i]);

  printf("Enter max matrix: \n");
  for(i = 0;i < m; i++)
  {
    for(j = 0;j < n; j++)
      scanf("%d", &s->max[i][j]);
  }

  printf("Enter allocation matrix: \n");
  for(i = 0; i < m; i++)
  {
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
  free(s->resource);
  free(s->available);
  for (i = 0; i < m; i++) {
    free(s->max[i]);
    free(s->allocation[i]);
    free(s->need[i]);
  }
  free(s);
}
