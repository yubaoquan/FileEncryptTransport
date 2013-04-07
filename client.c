#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <netinet/in.h>
#include <stdint.h>
#include <unistd.h>

#define SERVERPORT 3333 /*server's port */ 
#define BUFLEN 100
#define RECVLEN 10

int main(int argc, char *argv[]){
	if(argc == 1){
		printf("need more argument!\n");
		return -1;
	}
	int sockfd;
	int sent;
	char * localhost = "127.0.0.1";
	unsigned char buf[BUFLEN];
	unsigned char bufrecv[RECVLEN];
	int n;
    struct sockaddr_in servaddr;
	
//     if(argc == 1){
//     	printf("please input the argument first!");
//     	exit(-1);
//     }
	
    memset(buf,0,BUFLEN);
	memset(bufrecv,0,RECVLEN);
    memcpy(buf,argv[1],strlen(argv[1]));//获取文件名
	//>>>>>>>>>>>>>>对文件进行加密
	
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
	    perror("error creating socket!\n");
	    exit(-1);
    }
    

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERVERPORT);
    inet_pton(AF_INET, localhost, &servaddr.sin_addr);

    if(connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0){
    	printf("connect fail!\n");
    	exit(-1);
    }
    sent = send(sockfd,buf,strlen(buf),0);//向服务器发送文件名

	n = recv(sockfd,bufrecv,RECVLEN,0);
	
	if(strcmp(bufrecv,"ok") == 0){//服务器允许文件传输,则发送文件大小
		printf("server permitted!\n");
		
		int filesize = 10000;//>>>>>>>>>>>此处的常数替换为获取文件大小函数
		memset(buf,0,BUFLEN);
		memcpy(buf,sizechar,strlen(sizechar));
		send(sockfd,&filesize,sizeof filesize,0);
		//>>>>>>>>>>>>>接受服务器的确认信息,即文件大小.
		//>>>>>>>>>>>>>>若文件大小和发送的相等,开始发送文件
	}else{//服务器拒绝接受文件,显示拒绝信息,推出.
		printf("server denied!\n");
	}
    exit(0);

}
