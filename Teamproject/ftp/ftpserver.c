#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/sendfile.h>
#include <netinet/in.h>
#include <fcntl.h>

#define PORTA 4000

int main(int argc, char *argv[]){
   	struct sockaddr_in server, client;
   	struct stat obj;
   	struct sockaddr_in serv_addr;
   	int ssock;
   	int sock1, sock2;
   	char buf[100], command[5], filename[20];
   	int k, i, size, len, c;
   	int filehandle;
   	if ((ssock = socket(AF_INET, SOCK_STREAM, 0))== -1) {
   		perror("socket");
   		exit(1);
   	}
   	bzero((char *)&serv_addr, sizeof(serv_addr));
   	serv_addr.sin_family = AF_INET;
   	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
   	serv_addr.sin_port = htons(atoi(argv[1]));
   	if (bind(ssock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1) {
      	perror("bind");  
      	exit(1);
   	}
   	listen(ssock, 5);
   	len = sizeof(client);
   	sock1 = ssock;
   	sock2 = accept(sock1, (struct sockaddr*)&client, &len);
   
   	recv(sock2, buf, 100, 0);
   	sscanf(buf, "%s", command);
   	sscanf(buf, "%s%s", filename, filename);
   	stat(filename, &obj);
   	filehandle = open(filename, O_RDONLY);
   	size = obj.st_size;
   	if (filehandle == -1)
      	size = 0;
   	send(sock2, &size, sizeof(int), 0);
   	if (size)
      	sendfile(sock2, filehandle, NULL, size);
   	return 0;
}
