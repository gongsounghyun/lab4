#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<pthread.h>

#define USER_SIZE 100
#define LOCALPORT 4000
 
void* send_msg(void* arg);
void* receive_msg(void* arg);
 
char name[50]="[DEFALT]";
char msg_form[50];
char serv_time[50];
char msg[USER_SIZE];
char serv_port[50];
char clnt_ip[50];

int main(int argc, char *argv[]){
    int socketa;
    struct sockaddr_in server_address;
    pthread_t send_thread, receive_thread;
    void* thread_return;
    if (argc!=3){
        printf(" Usage : %s <ip> <name>\n", argv[0]);
        exit(1);
    }

    sprintf(name, "user : %s\ninput : ", argv[2]);
    printf("\n");
    sprintf(clnt_ip, "%s", argv[1]);
    socketa=socket(PF_INET, SOCK_STREAM, 0);
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family=AF_INET;
    server_address.sin_addr.s_addr=inet_addr(argv[1]);
    server_address.sin_port=htons(LOCALPORT);

    if (connect(socketa, (struct sockaddr*)&server_address, sizeof(server_address))==-1){
        fprintf(stderr, "conncet error");
        printf("\n");
        exit(1);
    }
 
    printf("Start!!!\n");
    pthread_create(&send_thread, NULL, send_msg, (void*)&socketa);
    pthread_create(&receive_thread, NULL, receive_msg, (void*)&socketa);
    pthread_join(send_thread, &thread_return);
    pthread_join(receive_thread, &thread_return);
    close(socketa);
    return 0;
}
 
void* send_msg(void* arg){
    int socketa=*((int*)arg);
    char name_msg[50];
    char myInfo[USER_SIZE];
 	int loop = 0;
    write(socketa, myInfo, strlen(myInfo));
 
    while(loop == 0){
        fgets(msg, USER_SIZE, stdin);
        sprintf(name_msg, "%s %s", name,msg);
        write(socketa, name_msg, strlen(name_msg));
    }
    return NULL;
}
 
void* receive_msg(void* arg){
    int socketa=*((int*)arg);
    int loop = 0;
    char name_msg[50];
    int str;

    while(loop == 0){
        str=read(socketa, name_msg, 100);
        if (str==-1)
            return (void*)-1;
        name_msg[str]=0;
        fputs(name_msg, stdout);
        printf("\n");
    }
    return NULL;
}
