#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

#define BRIDGE_CAPACITY 5

sem_t mutex, empty, full, leftFlag, rightFlag;
int buffer[BRIDGE_CAPACITY];
int in = 0, out = 0;

void put(int value) {
    buffer[in] = value;
    in = (in + 1) % BRIDGE_CAPACITY;
}

int get() {
    int value = buffer[out];
    out = (out + 1) % BRIDGE_CAPACITY;
    return value;
}

void passing(int dir, int car_number) {
    if (dir == 0) {
        printf("Car %d from left side is crossing the bridge.\n", car_number);
    } else {
        printf("Car %d from right side is crossing the bridge.\n", car_number);
    }
    sleep(1); // Simulate the time taken to cross the bridge
}

void* left(void* arg) {
    int left_cars = *((int*)arg);
    for (int i = 0; i < left_cars; i++) {
        sem_wait(&rightFlag);
        sem_wait(&empty);
        sem_wait(&mutex);
        put(i);
        passing(0, i);
        sem_post(&mutex);
        sem_post(&full);
        sem_post(&leftFlag);
    }
    return NULL;
}

void* right(void* arg) {
    int right_cars = *((int*)arg);
    for (int i = 0; i < right_cars; i++) {
        sem_wait(&leftFlag);
        sem_wait(&empty);
        sem_wait(&mutex);
        put(i);
        passing(1, i);
        sem_post(&mutex);
        sem_post(&full);
        sem_post(&rightFlag);
    }
    return NULL;
}

int main() {
    int left_cars, right_cars;

    printf("Enter the number of cars on the left side: ");
    scanf("%d", &left_cars);

    printf("Enter the number of cars on the right side: ");
    scanf("%d", &right_cars);

    sem_init(&mutex, 0, 1);
    sem_init(&empty, 0, BRIDGE_CAPACITY);
    sem_init(&full, 0, 0);
    sem_init(&leftFlag, 0, 0);
    sem_init(&rightFlag, 0, 0);

    pthread_t left_thread, right_thread;

    pthread_create(&left_thread, NULL, left, (void*)&left_cars);
    pthread_create(&right_thread, NULL, right, (void*)&right_cars);

    pthread_join(left_thread, NULL);
    pthread_join(right_thread, NULL);

    sem_destroy(&mutex);
    sem_destroy(&empty);
    sem_destroy(&full);

    return 0;
}
