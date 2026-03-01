#include "threading.h"
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

// Optional: use these functions to add debug or error prints to your application
#define DEBUG_LOG(msg,...)
//#define DEBUG_LOG(msg,...) printf("threading: " msg "\n" , ##__VA_ARGS__)
#define ERROR_LOG(msg,...) printf("threading ERROR: " msg "\n" , ##__VA_ARGS__)


void* threadfunc(void* thread_param) {
    struct thread_data* params = (struct thread_data*) thread_param;
    usleep(params->wt_obtain * 1e+3);
    pthread_mutex_lock(params->mutex);
    usleep(params->wt_release * 1e+3);
    pthread_mutex_unlock(params->mutex);
    params->thread_complete_success = true;

    return params;
}

bool start_thread_obtaining_mutex(pthread_t *thread, pthread_mutex_t *mutex,int wait_to_obtain_ms, int wait_to_release_ms) {
    int err;
    struct thread_data* params = (struct thread_data*) malloc(sizeof(struct thread_data));
    if (!params) return false;

    params->mutex = mutex;
    params->wt_obtain = wait_to_obtain_ms;
    params->wt_release= wait_to_release_ms;
    params->thread_complete_success = false;

    err = pthread_create(thread, NULL, threadfunc, (void*) params);
    if (err != 0) {
        ERROR_LOG("failed to create thread %p with error code %d\n", thread, err);
        free(params);
        return false;
    }
    params->thread = *thread;
    
    return true;
}
