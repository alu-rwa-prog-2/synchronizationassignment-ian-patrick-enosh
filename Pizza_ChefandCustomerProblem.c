// Code Written by
// Ian Odhiambo
// Enosh Nyarige
// Patrick Sikalinda

# include <stdio.h>
#include <stdlib.h>
# include <pthread.h>
#include <unistd.h>

// the pizza buffer acts like the tray for the pizza, it holds max 5 pizzas at a time.
// the reason it's initiated as 4 is to account for the 0 when counting
# define PizzaBufferSize 4

//This two functions control how thw chef and the customer access the pizza tray
void *chefFunction();
void *customerFunction();

//This integers keep track of the pizza tray and the pizza number
int PizzaBufferIndex = 0;
char *PizzaBUFFER;

pthread_cond_t Pizza_Buffer_Not_Full = PTHREAD_COND_INITIALIZER;
pthread_cond_t Pizza_Buffer_Not_Empty = PTHREAD_COND_INITIALIZER;
pthread_mutex_t mutexVariable = PTHREAD_MUTEX_INITIALIZER;

int main()
{
    pthread_t chefThread, customerThread;

    PizzaBUFFER=(char *) malloc(sizeof(char) * PizzaBufferSize);

    pthread_create(&chefThread, NULL, chefFunction, NULL);
    pthread_create(&customerThread, NULL, customerFunction, NULL);

    pthread_join(chefThread, NULL);
    pthread_join(customerThread, NULL);


    return 0;
}

void *chefFunction()
{
    // Endless for loop
    for(;;)
    {
        pthread_mutex_lock(&mutexVariable);
        if(PizzaBufferIndex == PizzaBufferSize)
        {
            pthread_cond_wait(&Pizza_Buffer_Not_Full, &mutexVariable);
        }
        sleep(1);

        PizzaBUFFER[PizzaBufferIndex++] ='@';
        printf("Chef made Pizza : %d \n", PizzaBufferIndex + 1);
        pthread_mutex_unlock(&mutexVariable);
        pthread_cond_signal(&Pizza_Buffer_Not_Empty);
    }

}

void *customerFunction()
{
    // Endless for loop
    for(;;)
    {
        pthread_mutex_lock(&mutexVariable);
        if(PizzaBufferIndex == -1)
        {
            pthread_cond_wait(&Pizza_Buffer_Not_Empty, &mutexVariable);
        }
        sleep(1);
        printf("Customer ate pizza : %d \n", PizzaBufferIndex-- + 1);
        pthread_mutex_unlock(&mutexVariable);
        pthread_cond_signal(&Pizza_Buffer_Not_Full);
    }
}
