#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<sys/select.h>
#include<netinet/in.h>
#include<pthread.h>

#define BUF_SIZE 100
#define LOCALPORT 4000

void *handle_client(void *arg);
void send_msg(char *msg, int len);
 
int client_count=0;
int client_socks[50];
pthread_mutex_t mutx;
 
int main(int argc, char *argv[]){
    int server_sock, client_sock;
    struct sockaddr_in server_address, client_address;
    int client_address_sz;
    fd_set readfds, allfds;
    pthread_t thread_id;
 
    pthread_mutex_init(&mutx, NULL);
    server_sock=socket(PF_INET, SOCK_STREAM, 0);
    
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family=AF_INET;
    server_address.sin_addr.s_addr=htonl(INADDR_ANY);
    server_address.sin_port=htons(LOCALPORT);
 
    if (bind(server_sock, (struct sockaddr*)&server_address, sizeof(server_address))==-1){
        fprintf(stderr, "bind error");
        printf("\n");
        exit(1);
	}
    if (listen(server_sock, 5)==-1){
        fprintf(stderr, "listen error");
        printf("\n");
        exit(1);        
	}
    printf("Server start!!\n");
     
    while(1){
        client_address_sz=sizeof(client_address);
        client_sock=accept(server_sock, (struct sockaddr*)&client_address, &client_address_sz);
 
        pthread_mutex_lock(&mutx);
        client_socks[client_count++]=client_sock;
        pthread_mutex_unlock(&mutx);
 
        pthread_create(&thread_id, NULL, handle_client, (void*)&client_sock);
        printf(" Connceted success new member");
        printf(" chatter (%d/50)\n", client_count);
    }
    close(server_sock);
    return 0;
}
 
void *handle_client(void *arg){
    int client_sock=*((int*)arg);
    int len=0;
    char msg[BUF_SIZE];

    while((len=read(client_sock, msg, sizeof(msg)))!=0)
        send_msg(msg, len);
 
    pthread_mutex_lock(&mutx);
    for (int i=0; i<client_count; i++){
        if (client_sock==client_socks[i]){
            while(i++<client_count-1)
                client_socks[i]=client_socks[i+1];
            break;
        }
    }
    pthread_mutex_unlock(&mutx);
    close(client_sock);
    return NULL;
}
 
void send_msg(char* msg, int len){
    pthread_mutex_lock(&mutx);
    for (int i=0; i<client_count; i++)
        write(client_socks[i], msg, len);
    pthread_mutex_unlock(&mutx);
}
