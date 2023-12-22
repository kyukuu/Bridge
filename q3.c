#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>

#define MAX_CARS 5

// Semaphore declarations
sem_t mutex;
sem_t bridge;
sem_t left_sem;  // Semaphore for cars traveling from the left
sem_t right_sem; // Semaphore for cars traveling from the right

// Counter to keep track of the number of cars on the bridge
int cars_on_bridge = 0;

// Function to represent a car traveling on the bridge
void passing(int dir, int car_number)
{
    printf("Car %d from side %s is traveling on the bridge.\n", car_number, (dir == 0) ? "Left" : "Right");
    srand(time(NULL));
    sleep(rand() % 5 + 1); // Sleep for a random time between 1 and 5 seconds to simulate the time taken to cross the bridge
}

// Function for cars on the left side
void *left(void *args)
{
    int car_number = *((int *)args);

    sem_wait(&left_sem); // Wait for the left semaphore

    sem_wait(&mutex); // Enter critical section
    cars_on_bridge++;
    sem_post(&mutex); // Exit critical section

    passing(0, car_number);

    sem_wait(&mutex); // Enter critical section
    cars_on_bridge--;
    sem_post(&mutex); // Exit critical section

    sem_post(&left_sem); // Release the left semaphore

    pthread_exit(NULL);
}

// Function for cars on the right side
void *right(void *args)
{
    int car_number = *((int *)args);

    sem_wait(&right_sem); // Wait for the right semaphore

    sem_wait(&mutex); // Enter critical section
    cars_on_bridge++;
    sem_post(&mutex); // Exit critical section

    passing(1, car_number);

    sem_wait(&mutex); // Enter critical section
    cars_on_bridge--;
    sem_post(&mutex); // Exit critical section

    sem_post(&right_sem); // Release the right semaphore

    pthread_exit(NULL);
}

int main()
{
    // Initialize semaphores
    sem_init(&mutex, 0, 1);
    sem_init(&left_sem, 0, 5);  // Initialize left semaphore to 1
    sem_init(&right_sem, 0, 5); // Initialize right semaphore to 1

    // Number of cars on each side (to be taken as input from the user)
    int num_left_cars = 10;
    int num_right_cars = 2;
    
    printf("Enter the number of cars on the left: ");
    scanf("%d", &num_left_cars);

    // Taking input for num_right_cars
    printf("Enter the number of cars on the right: ");
    scanf("%d", &num_right_cars);

    // Thread IDs for left and right cars
    pthread_t left_cars[num_left_cars + num_right_cars];

    // Create threads for left and right cars
    // Create threads for left cars
    for (int i = 0; i < num_left_cars; i++)
    {
        int *car_number = malloc(sizeof(int));
        *car_number = i + 1;
        pthread_create(&left_cars[i], NULL, left, (void *)car_number);
    }

    // Wait for left cars to finish
    for (int i = 0; i < num_left_cars; i++)
    {
        pthread_join(left_cars[i], NULL);
    }

    // Create threads for right cars
    for (int i = 0; i < num_right_cars; i++)
    {
        int *car_number = malloc(sizeof(int));
        *car_number = i + 1;
        pthread_create(&left_cars[i], NULL, right, (void *)car_number);
    }

    // Wait for right cars to finish
    for (int i = 0; i < num_right_cars; i++)
    {
        pthread_join(left_cars[i], NULL);
    }

    return 0;
}
