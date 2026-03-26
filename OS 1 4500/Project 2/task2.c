    #include <stdio.h>
    #include <stdlib.h>
    #include <pthread.h>
    #define BUFFER_SIZE 15

    //
    char buffer[BUFFER_SIZE];
    int in = 0;   // write position
    int out = 0;  // read position
    int count = 0;

    pthread_mutex_t mutex;
    pthread_cond_t not_full;
    pthread_cond_t not_empty;

    int done = 0; // signals producer is finished

    // producer thread reads char from message.txt and adds to buffer
    void* producer(void* arg) {
        // file open
        FILE* file = fopen("message.txt", "r");
        if (!file) {
            perror("Error opening file");
            exit(1);
        }
        // put char in buffer using mutex to stop race conditions with consumer
        char ch;
        while ((ch = fgetc(file)) != EOF) {
            pthread_mutex_lock(&mutex);
            // checks buffer is full so that it can pass to consumer and not deadlock
            while (count == BUFFER_SIZE) {
                pthread_cond_wait(&not_full, &mutex);
            }
            // files buffer spot if empty and updates in for next buffer update
            buffer[in] = ch;
            in = (in + 1) % BUFFER_SIZE;
            count++;
            // use not empty condition to signal consumer if buffer is empty and unlock mutex so consumer can access
            pthread_cond_signal(&not_empty);
            pthread_mutex_unlock(&mutex);
        }

        fclose(file);

        // Signal consumer we're done
        pthread_mutex_lock(&mutex);
        done = 1;
        pthread_cond_signal(&not_empty);
        pthread_mutex_unlock(&mutex);

        return NULL;
    }

    // consumer thread takes char from buffer and prints to stdout
    void* consumer(void* arg) {
        while (1) {
            // while 1 is same as while true but its 3 char less lol
            pthread_mutex_lock(&mutex);

            while (count == 0 && !done) {
                pthread_cond_wait(&not_empty, &mutex); // if theirs more for producer to do consuemr waits
            }

            if (count == 0 && done) {
                pthread_mutex_unlock(&mutex); // if all done then break fromm loop
                break;
            }
            // get ch from buffer and update out and count to match
            char ch = buffer[out];
            out = (out + 1) % BUFFER_SIZE;
            count--;
            // unlock mutex and signal not full
            pthread_cond_signal(&not_full);
            pthread_mutex_unlock(&mutex);
            // print ch to stdout immediatly
            putchar(ch);
            fflush(stdout); // use fflush to force char to stdout immediatly cause it was getting stuck in buffer

        }

        return NULL;
    }

    int main() {
        //create  mutex and conditions for full/empty
        pthread_t prod, cons;
        pthread_mutex_init(&mutex, NULL);
        pthread_cond_init(&not_full, NULL);
        pthread_cond_init(&not_empty, NULL);
        // create threads for consumer and producer
        pthread_create(&prod, NULL, producer, NULL);
        pthread_create(&cons, NULL, consumer, NULL);
        // wait for threads to finish up
        pthread_join(prod, NULL);
        pthread_join(cons, NULL);
        // clean up everything
        pthread_mutex_destroy(&mutex);
        pthread_cond_destroy(&not_full);
        pthread_cond_destroy(&not_empty);
        return 0;
    }
