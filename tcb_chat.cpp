#include <stdio.h> 
#include <string.h> 
#include <unistd.h> 
#include <time.h> 
#include <stdlib.h> 
#include <sys/socket.h>
#include <netinet/in.h> 
#include <arpa/inet.h> 
#include <pthread.h>
#include "iostream"
#define MAXLINE 1024
using namespace std;

void* server(void* argv)
{
	int sockfd, fd, n, m;
	char line[MAXLINE + 1];
	struct sockaddr_in6 servaddr, cliaddr; 
	time_t t_start = time(NULL);
	printf("time #: %ld\n", t_start);
	fputs(ctime(&t_start),stdout);
	
	if((sockfd = socket(AF_INET6, SOCK_STREAM, 0)) < 0) 
		perror("socket error");
	else
		printf("Socket successfully.\n");
	bzero(&servaddr, sizeof(servaddr)); 
	servaddr.sin6_addr = in6addr_any;
	servaddr.sin6_family = AF_INET6;  
	servaddr.sin6_port = htons(20000); 
	if(bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) == -1) 
		perror("Bind error.");
	else
		printf("Bind successfully.\n");
	if(listen(sockfd, 5) == -1) 
		perror("Listen error.");
	else
		printf("Listen successfully.\n");
	while(1)
	{
		printf("> Waiting clients please input the ip to connect...\n"); 
		socklen_t clilen = sizeof(struct sockaddr);
		fd = accept(sockfd, (struct sockaddr*)&cliaddr, &clilen); 
		if(fd == -1)
			perror("Accept error.");
		else
			printf("Accept successfully.\n");
		while((n = read(fd, line, MAXLINE)) > 0)
		{ 
			line[n] = 0;
			if(line[0]=='E' && line[1]=='N' && line[2]=='D')
				break;
			if(fputs(line, stdout) == EOF) 
				perror("Send message error.");
			else
				printf("Send message successfully.\n");
		}
		printf("Sever is down...\n");
		close(fd);
	}
		if(n < 0) 
			perror("Read error.");
}

void* client(void* argv)
{
	int sockfd, n, m;
	char line[MAXLINE + 1]; 
	struct sockaddr_in6 servaddr; 
	if((sockfd = socket(AF_INET6, SOCK_STREAM, 0)) < 0) 
		perror("Socket error.");
	else
		printf("Socket successfully.\n");
	bzero(&servaddr, sizeof(servaddr)); 
	servaddr.sin6_family = AF_INET6; 
	servaddr.sin6_port = htons(20000);
	char* argv_char = (char*)argv;  
	if(inet_pton (AF_INET6, argv_char, &servaddr.sin6_addr) <= 0) 
		perror("inet_pton error.");
	else
		printf("inet_pton successfully.\n");
	if(connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0)
		perror("Connect error.");
	else
		printf("Connect successfully.\n");
	while(fgets(line, MAXLINE, stdin) != NULL)
	{ 
		send(sockfd, line, strlen(line), 0);
		if(line[0]=='E' && line[1]=='N' && line[2]=='D')
			break;
	}
	close(sockfd);
	std::cout<<"Close client."<<std::endl;
	std::cout<<"You can client new server."<<std::endl;
}

int main(int argc, char **argv)
{
	pthread_t tids[2];
	char ip_num[200];
	if((pthread_create(&tids[0], NULL, server, NULL)))
        printf("pthread_create server error.");
	else
		printf("pthread_create server successfully.\n");
	
	std::cout<<"Please input the ip you want connect..."<<std::endl;
	cin.getline(ip_num, 200);
	if((pthread_create(&tids[1], NULL, client, ip_num)))
		printf("pthread_create client error.");
	else
		printf("pthread_create client successfully.");
    
    pthread_exit(NULL);
	return 0;
}
