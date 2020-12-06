#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/file.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <time.h>
#define MAXLINE  511
#define MAX_SOCK 1024 
char *EXIT_STRING = "exit";   
char *START_STRING = "Connected to chat_server \n";
int maxfd;            
int num_user = 0;         
int num_chat = 0;         
int clisock_list[MAX_SOCK];      
char ip_list[MAX_SOCK][20];      
int listen_sock;         
int getmax();            
void removeClient(int s);   
int tcp_listen(int host, int port, int backlog); 
struct tm tm;
int main(int argc, char *argv[]) {
   struct sockaddr_in cliaddr;
   char buf[MAXLINE + 1]; 
   int i, j, nbyte, accp_sock, addrlen = sizeof(struct sockaddr_in);
   fd_set read_fds;   

   if (argc != 2) {
      printf("사용법 :%s port\n", argv[0]);
      exit(0);
   }
   int sd;
   struct sockaddr_in servaddr;

   sd = socket(AF_INET, SOCK_STREAM, 0);
   if (sd == -1) {
      perror("socket fail");
      exit(1);
   }
   bzero((char *)&servaddr, sizeof(servaddr));
   servaddr.sin_family = AF_INET;
   servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
   servaddr.sin_port = htons(atoi(argv[1]));
   if (bind(sd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
      perror("bind fail");  
      exit(1);
   }
   listen(sd, 5);
   listen_sock = sd;

   while (1) {
      FD_ZERO(&read_fds);
      FD_SET(listen_sock, &read_fds);
      for (i = 0; i < num_user; i++)
         FD_SET(clisock_list[i], &read_fds);
      
      int max = listen_sock;
      int i;
      char buf[20];
      for (i = 0; i < num_user; i++)
      if (clisock_list[i] > max)
      max = clisock_list[i];
      maxfd = max + 1;   

      if (select(maxfd, &read_fds, NULL, NULL, NULL) < 0)
      {   fprintf(stderr, "select error");
         printf("\n");
         exit(1);
      }

      if (FD_ISSET(listen_sock, &read_fds)) {
         accp_sock = accept(listen_sock,
            (struct sockaddr*)&cliaddr, &addrlen);
         if (accp_sock == -1) {   
            fprintf(stderr, "select error");
            printf("\n");
            exit(1);
         }
      
         inet_ntop(AF_INET, &cliaddr.sin_addr, buf, sizeof(buf));
         write(1, "\033[0G", 4);         
         
         clisock_list[num_user] = accp_sock;
         strcpy(ip_list[num_user], buf);
         num_user++; 
         send(accp_sock, START_STRING, strlen(START_STRING), 0);

         write(1, "\033[0G", 4);      
         printf("user count = %d\n", num_user);
         fprintf(stderr, "server>"); 
      }

      for (i = 0; i < num_user; i++) {
         if (FD_ISSET(clisock_list[i], &read_fds)) {
            num_chat++;            
            nbyte = recv(clisock_list[i], buf, MAXLINE, 0);
            if (nbyte <= 0||strstr(buf, EXIT_STRING) != NULL) {
               close(clisock_list[i]);
               if (i != num_user - 1) { 
                  clisock_list[i] = clisock_list[num_user - 1];
                  strcpy(ip_list[i], ip_list[num_user - 1]);
               }
               num_user--; 
               write(1,"", 4);      
               fprintf(stderr, "server>"); 
               continue;
            }
            buf[nbyte] = 0;
      
            for (j = 0; j < num_user; j++)
               send(clisock_list[j], buf, nbyte, 0);
            printf("%s", buf);         
            fprintf(stderr, "server>"); 
         }
      }

   }  
   return 0;
}

