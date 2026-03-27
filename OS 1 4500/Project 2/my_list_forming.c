/*
  my_list-forming.c:
  Each thread generates a local data node and attaches it to a local tree k times.
  It then attaches the local tree to the global tree to cut down  the # of critical region entries
  There are num_threads threads. The value of "num_threads" is input by the student.
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>
#include <sys/param.h>
#include <sched.h>

#define K 200

struct Node {
    int data;
    struct Node* next;
};

struct list {
    struct Node *header;
    struct Node *tail;
};

pthread_mutex_t mutex_lock;

struct list *List;

struct Node* generate_data_node() {
    struct Node *ptr = (struct Node *)malloc(sizeof(struct Node));
    if (ptr != NULL) {
        ptr->next = NULL;
    } else {
        printf("Node allocation failed!\n");
    }
    return ptr;
}

void *producer_thread(void *arg) {
    //init local node list
    struct Node *local_head = NULL;
    struct Node *local_tail = NULL;
    struct Node *ptr;
    // build local list with k nodes
    for (int i = 0; i < K; i++) {
        ptr = generate_data_node();
        if (ptr != NULL) {
            ptr->data = 1;

            if (local_head == NULL) {
                local_head = local_tail = ptr;
            } else {
                local_tail->next = ptr;
                local_tail = ptr;
            }
        }
    }
    // attach local nodes to global list with one time using mutex lock
    pthread_mutex_lock(&mutex_lock);
    if (List->header == NULL) {
        List->header = local_head;
        List->tail = local_tail;
    } else {
        List->tail->next = local_head;
        List->tail = local_tail;
    }
    // unlock and return
    pthread_mutex_unlock(&mutex_lock);
    return NULL;
}

int main(int argc, char* argv[]) {
    // add error handeling for inputs
    if (argc == 1) {
        printf("ERROR: please provide number of threads\n");
        exit(1);
    }
    int num_threads = atoi(argv[1]);
    pthread_t producer[num_threads];
    pthread_mutex_init(&mutex_lock, NULL); // init mutex
    List = (struct list *)malloc(sizeof(struct list)); // allocate and start list
    List->header = List->tail = NULL;
    struct timeval starttime, endtime; // struct to store times
    gettimeofday(&starttime, NULL); // get start time
    // create all threads
    for (int i = 0; i < num_threads; i++) {
        pthread_create(&producer[i], NULL, producer_thread, NULL);
    }
    // join all threads
    for (int i = 0; i < num_threads; i++) {
        pthread_join(producer[i], NULL);
    }
    gettimeofday(&endtime, NULL); // get end time
    // free da list node by node
    struct Node *tmp = List->header;
    while (tmp != NULL) {
        struct Node *next = tmp->next;
        free(tmp);
        tmp = next;
    }
    free(List);
    // calc run time in microsecoods to simplify printf
    long runtime =
        (endtime.tv_sec - starttime.tv_sec) * 1000000 +
        (endtime.tv_usec - starttime.tv_usec);
    printf("total run time is: %ld microseconds.\n", runtime);
    return 0;
}
