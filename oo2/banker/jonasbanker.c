#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <pthread.h>

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

int safe_state(State *s);
void printArr(int *arr, int lenght);
void subArr(int * dest, int *sub, int lenght);
void addArr(int * dest, int *add, int lenght);

/* Random sleep function */
void Sleep(float wait_time_ms)
{
    // add randomness
    wait_time_ms = ((float)rand())*wait_time_ms / (float)RAND_MAX;
    usleep((int) (wait_time_ms * 1e3f)); // convert from ms to us
}

/* Allocate resources in request for process i, only if it 
   results in a safe state and return 1, else return 0 */
int resource_request(int i, int *request)
{    
//    int j;
    pthread_mutex_lock(&state_mutex);
//    if (islarger(request, s->available)) {
//        printf("process requires too much\n");
//        pthread_mutex_unlock(&state_mutex);
//        return 0;
//    }
    
    subArr(s->available, request, n);
    addArr(s->allocation[i], request, n);
    subArr(s->need[i], request, n);
//    for (j = 0; j < n; j++) {
//        s->available[j] -= request[j];
//        s->allocation[i][j] += request[j];
//        s->need[i][j] -= request[j];
//    }
    
    if (safe_state(s)) {
        printArr(s->allocation[i]);
        pthread_mutex_unlock(&state_mutex);
        return 1;
    } else {
        addArr(s->available, request, n);
        subArr(s->allocation[i], request, n);
        addArr(s->need[i], request, n);
//        for (j = 0; j < n; j++) {
//            s->available[j] += request[j];
//            s->allocation[i][j] -= request[j];
//            s->need[i][j] += request[j];
//        }
        pthread_mutex_unlock(&state_mutex);
        return 0;
    }
}

/* Release the resources in request for process i */
void resource_release(int i, int *request)
{
    pthread_mutex_lock(&state_mutex);
    
    addArr(s->available, request, n);
    subArr(s->allocation[i], request, n);
    addArr(s->need[i], request, n);
//    int j;
//    for (j = 0; j < n; j++) {
//        s->available[j] += request[j];
//        s->allocation[i][j] -= request[j];
//        s->need[i][j] += request[j];
//    }
//    printf("\n{");
//    for (j = 0; j < n; j++) {
//        printf("[%i]", s->allocation[i][j]);
//    }
//    printf("}\n");
    printArr(s->allocation[i], n);
    pthread_mutex_unlock(&state_mutex);
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
    printf("Number of processes: ");
    scanf("%d", &m);
    printf("Number of resources: ");
    scanf("%d", &n);

    /* Allocate memory for state */
    s             = (State *) malloc(sizeof(State));
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
    printf("Resource vector: ");
    for(i = 0; i < n; i++)
        scanf("%d", &s->resource[i]);
    printf("Enter max matrix: ");
    for(i = 0;i < m; i++)
        for(j = 0;j < n; j++)
            scanf("%d", &s->max[i][j]);
    printf("Enter allocation matrix: ");
    for(i = 0; i < m; i++)
        for(j = 0; j < n; j++)
            scanf("%d", &s->allocation[i][j]);
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
    if(safe_state(s)) {
        pthread_mutex_init(&state_mutex, NULL)
        
        /* Seed the random number generator */
        struct timeval tv;
        gettimeofday(&tv, NULL);
        srand(tv.tv_usec);
      
        /* Create m threads */
        pthread_t *tid = malloc(m*sizeof(pthread_t));
        for (i = 0; i < m; i++)
            pthread_create(&tid[i], NULL, process_thread, (void *) (long) i);
      
        /* Wait for threads to finish */
        for (i = 0; i < m; i++)
            pthread_join(tid[i],NULL);
            
    } else {
        printf("Start state is unsafe\n");
        /* Free state memory */
        
        free(s->resource);
        free(s->available);
        for (i = 0; i < m; i++) {
            free(s->max[i]);
            free(s->allocation[i]);
            free(s->need[i]);
        }
        free(s);
        exit(1);
    }
}

int safe_state(State *s) {
    int count = m, i, isSafe, can_run, j;
    int *finish = (int *)malloc(n * sizeof(int));
    int *work = (int *)malloc(m * sizeof(int));
    for (i = 0; i < m; i++) {
        finish[i] = 0;
        work[i] = s->available[i];
    }
    
    while (count != 0) {
        isSafe = 0;
        for (i = 0; i < m; i++) {
            if (!finish[i]) {
//                can_run = 1;
//                for (j = 0; j < n; j++) {
//                    if ( need[i][j]> work[j]) {
//                        can_run = 0;
//                        break;
//                    }
//                }
                if (!islarger(work, need[i], n)) {
                    finish[i] = 1;
                    count--;
                    isSafe = 1;
 
                    addArr(work,allocation[i],n);
//                    for (j = 0; j < n; j++) {
//                        work[j] += s->allocation[i][j];
//                    }
                    break;
                }
            }
        }
        if (!isSafe) {
            printf("\nThe processes are in unsafe state.\n");
            return 0;
        }
    }
    return 1;
}

int islarger(int *sub, int *match, int size) {
    int i;
    for (i = 0; i < size; i++) 
        if (sub[i] > match[i]) return 0;
    return 1;
}


void printArr(int *arr, int lenght) {
    int i;
    printf("{")
    for (i = 0; i < lenght; i++) {
        printf("[%i]",arr[i]);
    }
    printf("}");
}

void addArr(int *dest, int* add, int lenght) {
    int i;
    for (i = 0; i < lenght; i++) {
        dest[i] += add[i];
    }
}

void subArr(int * dest, int *sub, int lenght) {
    int i;
    for (i = 0; i < lenght; i++) {
        dest[i] -= sub[i];
    }
}
