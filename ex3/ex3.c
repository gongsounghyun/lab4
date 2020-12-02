#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>
#include<sys/ipc.h>
#include<semaphore.h>
#define MAX_SIZE 10
time_t final_time;
pthread_mutex_t mutex_lock;
int number=0;
void *barber_thread(void *data);
void *customer_thread(void *data);

int main(){
    pthread_t p_thread1, p_thread2;
    final_time=time(NULL)+20;
    pthread_mutex_init(&mutex_lock, NULL);
    pthread_create(&p_thread1,NULL, barber_thread,NULL);
    pthread_create(&p_thread2,NULL, customer_thread,NULL);
    pthread_join(p_thread1,NULL);
    pthread_join(p_thread2,NULL);

    exit(0);
}
void *barber_thread(void *data){
    while(time(NULL)<final_time){
        if(number>0){
            pthread_mutex_lock(&mutex_lock);
            number--;
            printf("Barber cut hair => number : %d\n",number);
            pthread_mutex_unlock(&mutex_lock);
        }
        if(time(NULL)>final_time) break;
        sleep(3);
        if(number == 10) exit(0);
    }
}
void *customer_thread(void *data){
    while(time(NULL)<final_time){
        pthread_mutex_lock(&mutex_lock);
        if(number<MAX_SIZE){
            number++;
            printf("Customer add => number : %d\n",number);
            pthread_mutex_unlock(&mutex_lock);
        }
        if(time(NULL)>final_time) break;
        sleep(1);
        if(number == 10) exit(0);
    }
}
