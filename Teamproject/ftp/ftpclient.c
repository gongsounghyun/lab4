#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/sendfile.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>

#define PORTA 4000
#define MAXLINE  511

int main(int argc, char *argv[]){
   	struct sockaddr_in server, servaddr;
   	struct stat obj;
   	int sock, s;
   	char bufmsg[MAXLINE];
   	char buf[100], command[5], filename[MAXLINE], *f;
   	char temp[20];
   	int k, size, status;
   	int filehandle;

   	if (argc != 3) {
      	printf(" %s server_ip port\n", argv[0]);
      	exit(1);
   	}


	sock = socket(PF_INET, SOCK_STREAM, 0);
	bzero((char *)&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr=inet_addr(argv[1]);
	servaddr.sin_port = htons(PORTA);

	if (connect(sock, (struct sockaddr *)&servaddr, sizeof(servaddr))< 0)
		return -1;

	if (sock == -1) {
		fprintf(stderr,"connect error");
		printf("\n");
		exit(1);
	}
	printf("다운로드할 파일 : ");
	scanf("%s", filename);
	fgets(temp, MAXLINE, stdin);
	strcpy(buf, filename);
	send(sock, buf, 100, 0);
	recv(sock, &size, sizeof(int), 0);
	if (!size) {
      	fprintf(stderr,"open error\n");
   	}
   	f = malloc(size);
   	recv(sock, f, size, 0);
   	filehandle = open(filename, O_CREAT | O_EXCL | O_WRONLY, 0666);
   	write(filehandle, f, size, 0);
   	close(filehandle);
   	printf("다운로드 완료\n");
  	 
   	return 0;
}
