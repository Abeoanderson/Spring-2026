#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
//imports ^^^

//define num_gthreads and max_length
#define NUM_THREADS 4
#define MAX 1024


//define globals
char *s1;
char *s2;
int n1, n2;
FILE *fp;
int total_count = 0;       // shared count
pthread_mutex_t lock;      // protects total_count

//struct for thread args
typedef struct {
    int thread_id;
    int start_pos;  //starting index to check
    int end_pos;    //end of range to check
} ThreadArgs;


// read file func
int readf(FILE *fp)
{
    if((fp = fopen("strings.txt", "r")) == NULL){
        printf("ERROR: can't open strings.txt!\n");
        return 0;
    }
    s1 = (char *)malloc(sizeof(char) * MAX);
    if(s1 == NULL){ printf("ERROR: Out of memory!\n"); return -1; }

    s2 = (char *)malloc(sizeof(char) * MAX);
    if(s2 == NULL){ printf("ERROR: Out of memory!\n"); return -1; }

    s1 = fgets(s1, MAX, fp);   // get s1 from file
    s2 = fgets(s2, MAX, fp);   // get s2 from file

    n1 = strlen(s1) - 1;      // strip newline char from s1
    n2 = strlen(s2) - 1;      // strip newline char from s2

    if(s1 == NULL || s2 == NULL || n1 < n2)
        return -1;

    fclose(fp);
    return 1;
}


// check substrings sequentially for validation
int num_substring_seq()
{
    int i, j, k, count, total = 0;
    for(i = 0; i <= (n1 - n2); i++){
        count = 0;
        for(j = i, k = 0; k < n2; j++, k++){
            if(*(s1+j) != *(s2+k)) break;
            else count++;
            if(count == n2) total++;
        }
    }
    return total;
}
// thread search func
void *thread_search(void *arg)
{
    ThreadArgs *args = (ThreadArgs *)arg;
    int i, j, k, count;
    int local_count = 0;

    // Check each starting position assigned to this thread
    for(i = args->start_pos; i < args->end_pos; i++){
        count = 0;
        for(j = i, k = 0; k < n2; j++, k++){
            if(*(s1+j) != *(s2+k)) break;
            else count++;
            if(count == n2) local_count++;
        }
    }

    //lock and unlock mutex for safe global var update
    pthread_mutex_lock(&lock);
    total_count += local_count;
    printf("Thread %d searched positions %d-%d and found %d matches\n",
           args->thread_id, args->start_pos, args->end_pos - 1, local_count);
    pthread_mutex_unlock(&lock);

    pthread_exit(NULL);
}

//main func
int main() {
    readf(fp);

    printf("s1 = \"%s\" (length %d)\n", s1, n1);
    printf("s2 = \"%s\" (length %d)\n", s2, n2);
    printf("Number of threads: %d\n\n", NUM_THREADS);

    // valid start positions n1 - n2 so we safe time at the end
    int valid_positions = n1 - n2 + 1;
    // if fewer positions then threads each gets 1
    int actual_threads = (valid_positions < NUM_THREADS) ? valid_positions : NUM_THREADS; // check to make sure theirs enough work for all 4 threads
    int chunk = valid_positions / actual_threads; // divy up valid posistns amongs threads

    pthread_t threads[NUM_THREADS];
    ThreadArgs args[NUM_THREADS];

    pthread_mutex_init(&lock, NULL);

    // create thread and pass slice of work to do
    for (int t = 0; t < actual_threads; t++) {
        args[t].thread_id = t;
        args[t].start_pos = t * chunk;

        // last thread just gets rest of slice
        if (t == actual_threads - 1)
            args[t].end_pos = valid_positions;
        else
            args[t].end_pos = args[t].start_pos + chunk;

        pthread_create(&threads[t], NULL, thread_search, &args[t]);
    }

    // after threads finish sum up
    for (int t = 0; t < actual_threads; t++) {
        pthread_join(threads[t], NULL);
    }

    pthread_mutex_destroy(&lock);

    printf("\nparallel result:     %d\n", total_count);
    printf("sequential result:   %d\n", num_substring_seq());

    return 0;
}
