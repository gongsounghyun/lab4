#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>

#define MAXLINE     1000
#define NAME_LEN    20

int main(int argc, char *argv[]) {
	char bufname[NAME_LEN];
	char bufmsg[MAXLINE];
	char bufall[MAXLINE + NAME_LEN];
	int maxfdp1;
	int sock;
	int namelen;
	fd_set read_fds;
	struct sockaddr_in servaddr;

	if (argc != 4) {
		printf("help : %s <SERVER_IP> <PORT_NUM> <PORT_NAME> \n", argv[0]);
		exit(0);
	}
	
	sock = socket(PF_INET, SOCK_STREAM, 0);
	bzero((char *)&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr=inet_addr(argv[1]);
	servaddr.sin_port = htons(atoi(argv[2]));

	if (connect(sock, (struct sockaddr *)&servaddr, sizeof(servaddr))< 0){
		fprintf(stderr,"connect error");
		printf("\n");
		exit(1);
	}
	
	puts("connect success.");
	maxfdp1 = sock + 1;
	FD_ZERO(&read_fds);

	while (1) {
		FD_SET(0, &read_fds);
		FD_SET(sock, &read_fds);
		if (select(maxfdp1, &read_fds, NULL, NULL, NULL) < 0){
			fprintf(stderr, "select fail");
			printf("\n");
			exit(1);
		}
		if (FD_ISSET(sock, &read_fds)) {
			int nbyte;
			if ((nbyte = recv(sock, bufmsg, MAXLINE, 0)) > 0) {
				bufmsg[nbyte] = 0;
				write(1, "\033[0G", 4);
				printf("%s", bufmsg);
				fprintf(stderr, "%s>", argv[3]);
			}
		}
		if (FD_ISSET(0, &read_fds)) {
			if (fgets(bufmsg, MAXLINE, stdin)) {
				fprintf(stderr, "\033[1A");
				sprintf(bufall,"%s>%s", argv[3], bufmsg);
				if (send(sock, bufall, strlen(bufall), 0) < 0)
					puts("Error : Write error on socket.");
			}
		}
	}
}
