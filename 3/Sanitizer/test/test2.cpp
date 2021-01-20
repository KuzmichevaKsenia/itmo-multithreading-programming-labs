#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

pthread_mutex_t a_mutex, b_mutex;

void* first_routine(void*) {
    pthread_mutex_lock(&a_mutex);
    printf("1 locked a\n");
    sleep(2);
    pthread_mutex_lock(&b_mutex);
    printf("1 locked b\n");
    pthread_mutex_unlock(&b_mutex);
    printf("1 unlocked b\n");
    pthread_mutex_unlock(&a_mutex);
    printf("1 unlocked a\n");

    pthread_exit(nullptr);
}

void* second_routine(void*) {
    pthread_mutex_lock(&b_mutex);
    printf("2 locked b\n");
    sleep(2);
    pthread_mutex_lock(&a_mutex);
    printf("2 locked a\n");
    pthread_mutex_unlock(&a_mutex);
    printf("2 unlocked a\n");
    pthread_mutex_unlock(&b_mutex);
    printf("2 unlocked b\n");

    pthread_exit(nullptr);
}

int main() {
    int status;
    pthread_t first, second;
    pthread_mutex_init(&a_mutex, nullptr);
    pthread_mutex_init(&b_mutex, nullptr);

    status = pthread_create(&first, nullptr, first_routine, nullptr);
    if (status != 0) {
        printf("error: can't create first thread, status = %d\n", status);
        exit(EXIT_FAILURE);
    }

    status = pthread_create(&second, nullptr, second_routine, nullptr);
    if (status != 0) {
        printf("error: can't create second thread, status = %d\n", status);
        exit(EXIT_FAILURE);
    }

    status = pthread_join(first, nullptr);
    if (status != 0) {
        printf("error: can't join first thread, status = %d\n", status);
        exit(EXIT_FAILURE);
    }

    status = pthread_join(second, nullptr);
    if (status != 0) {
        printf("error: can't join second thread, status = %d\n", status);
        exit(EXIT_FAILURE);
    }

    pthread_mutex_destroy(&a_mutex);
    pthread_mutex_destroy(&b_mutex);
}
