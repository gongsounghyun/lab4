#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#define NUM_THREADS 5
#define ZERO 0
#define ONE 1

int parent = 0;
int chird =0;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t    cond = PTHREAD_COND_INITIALIZER;
pthread_t   threadid[NUM_THREADS];

void *result_thread(void *input_string) {
    while (chird==ZERO) {
        sleep(1);
        printf("메시지 전송 요청\n");
        pthread_mutex_lock(&mutex);
        parent = ONE;
        pthread_cond_wait(&cond, &mutex);
        printf(" input_result : %s\n",input_string);
        pthread_mutex_unlock(&mutex);
    }
}
void main() {
    char *input_string;
    for(int i = 0; i < NUM_THREADS; i++)
        pthread_create(&threadid[i],NULL,result_thread,(char*)&input_string);
    sleep(5);
    if (parent == ONE) {
		pthread_mutex_lock(&mutex);
    	pthread_cond_broadcast(&cond);
    	printf("\n메세지 전송 요청이 호출\n\n");
		printf("전송하고자 하는 내용을 입력 : ");
		scanf("%s",&input_string);
        parent = ZERO;
       	chird = ONE;
		pthread_mutex_unlock(&mutex);
    }   
    for(int i = 0; i < NUM_THREADS ; i++)
        pthread_join(threadid[i], NULL);
	
	printf("\n쓰레드를 모두 종료하고 프로그램을 종료합니다.\n");
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);
}
