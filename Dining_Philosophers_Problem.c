// Code Written by
// Ian Odhiambo
// Enosh Nyarige
// Patrick Sikalinda

#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <unistd.h>

//Define the max number of threads to be created
#define COUNT_TO 5
#define THINKING 2
#define HUNGRY 1
#define EATING 0

//Define which side of the philosopher is left or right
#define RIGHT_SIDE (i + 1) % COUNT_TO
#define LEFT_SIDE (i + 4) % COUNT_TO

//Create an array of size equal to the total number of threads to be created
int state[COUNT_TO];
int phil[COUNT_TO] = { 0, 1, 2, 3, 4 };

sem_t mutex;
sem_t S[COUNT_TO];

//This method will define the state of the philosopher. Its either you are HUNGRY or EATING
void state_philosophers(int i){
	if (state[i] == HUNGRY
		&& state[i] != EATING
		&& state[RIGHT_SIDE] != EATING) {
		// state that eating
		state[i] = EATING;
		sleep(2);
		printf("Philosopher %d takes fork %d and %d\n",
               i + 1, LEFT_SIDE + 1, i + 1);
		printf("Philosopher %d is Eating\n", i + 1);
		sem_post(&S[i]);
	}
}

//This is the method to pick the chopsticks
void take_fork(int i){
	sem_wait(&mutex);
	state[i] = HUNGRY;
	printf("Philosopher %d is Hungry\n", i + 1);
//	A philosopher can only pick the chopsticks if the neighbors are not eating
    state_philosophers(i);
	sem_post(&mutex);
//	If the neighbor is eating, the philosopher will be signalled to wait for one second
	sem_wait(&S[i]);
	sleep(1);
}

//This is the method to put down the chopsticks
void put_fork(int i){
	sem_wait(&mutex);
//	Set the new state of the philosopher to thinking, so that the chopsticks can be available for use by another
	state[i] = THINKING;
	printf("Philosopher %d putting fork %d and %d down\n", i + 1, LEFT_SIDE + 1, i + 1);
	printf("Philosopher %d is thinking\n", i + 1);
    state_philosophers(LEFT_SIDE);
    state_philosophers(RIGHT_SIDE);
	sem_post(&mutex);
}

//Loop the actions for as long as all the resources are available
void* philosopher_Actions(void* num){
	while (1) {

		int* i = num;

		sleep(1);

		take_fork(*i);

		sleep(0);

		put_fork(*i);
	}
}

int main(){
	int i;
	pthread_t thread_id[COUNT_TO];

//	Initialize the threads, lock it
	sem_init(&mutex, 0, 1);
	for (i = 0; i < COUNT_TO; i++)
		sem_init(&S[i], 0, 0);
	for (i = 0; i < COUNT_TO; i++) {
		// create philosopher processes
		pthread_create(&thread_id[i], NULL, philosopher_Actions, &phil[i]);
		printf("Philosopher %d is thinking\n", i + 1);
	}
	for (i = 0; i < COUNT_TO; i++)
//	    Unlock the threads
		pthread_join(thread_id[i], NULL);
}
