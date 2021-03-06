// Author: Ori Shinsholker - 312421852

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>
#include <semaphore.h>

#define THINKING 1
#define HUNGRY 2
#define EATING 3

typedef struct
{
    int num;
    int* Sticks;
    pthread_t* id;
    pthread_mutex_t Mutex;

} Threads;

Threads* threads;

void get_Sticks(int th_id, int eatTime, int right, int left)
{
    printf("Philosopher %d trying to pick up sticks.\n", th_id);
    threads->Sticks[left] = 0;
    threads->Sticks[right] = 0;
    printf("Philosopher %d manages to pick up sticks.\n", th_id);

}

void put_Sticks(int th_id, int right, int left)
{
    threads->Sticks[right] = 1;
    threads->Sticks[left] = 1;
}

void* usePhilosopher(void *args)
{
    threads = (Threads*)args;
    int i=0, th_id=NULL, right, left;
    int eatTime, thinkTime;
    //Getting thread ID of current thread
    pthread_mutex_lock(&threads->Mutex);
    while((!th_id)&&(i<threads->num))
    {
        if(pthread_self() == threads->id[i])
            th_id = i;
        i++;
    }
    right = (th_id + 1) % threads->num;
    left = th_id;
    eatTime = (int)((float)(1+rand()%10));
    thinkTime = (int)((float)(10+rand()%10));
    pthread_mutex_unlock(&threads->Mutex);
    //Gets sticks
    pthread_mutex_lock(&threads->Mutex);
    get_Sticks(th_id, eatTime, right, left);
    pthread_mutex_unlock(&threads->Mutex);
    //Eating
    printf("Philosopher %d eating for %d seconds…\n", th_id, eatTime);
    sleep(eatTime);
    printf("Philosopher %d finised to eating in %d seconds.\n", th_id, eatTime);
    //Puts sticks
    pthread_mutex_lock(&threads->Mutex);
    put_Sticks(th_id, rigth, left);
    pthread_mutex_unlock(&threads->Mutex);
    //Thinking
    printf("Philosopher %d thinking for %d seconds…\n", th_id, thinkTime);
    sleep(thinkTime);
    printf("Philosopher %d finished to think in %d seconds.\n", th_id, thinkTime);
}

int main(int argc, const char* argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "usage: dining_philosophers <num_loops>\n");
        exit(1);
    }
    int i;
    while(1)    //Endless loop
    {
        printf("\n***NEW DINING***\n");
        threads = (Threads*)malloc(sizeof(Threads));
        threads->num = atoi(argv[1]);   //num of philosophers
        threads->id = (pthread_t*)malloc(sizeof(pthread_t)*threads->num);
        threads->Sticks = (int*)malloc(sizeof(int)*threads->num);
        pthread_condattr_t blocker;
        pthread_condattr_init(&blocker);
        for(i=0; i<threads->num; i++)
            threads->Sticks[i] = 1;
        for(i=0; i< threads->num; i++)
            pthread_create(&threads->id[i], NULL, &usePhilosopher, (void*)threads);
        for(i=0; i<threads->num; i++)
        {
            pthread_join(threads->id[i]);
            printf("Philosopher %d finished\n", i);
        }
        pthread_mutex_destroy(&threads->Mutex);
    }
    return 0;
}


