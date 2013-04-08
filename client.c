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
// #define RECVLEN 10

int main(int argc, char *argv[]){
	
	if(argc == 1){
		printf("need more argument!\n");
		return -1;
	}
	FILE * fileToSend = NULL;
	char * filename = argv[1];
	fileToSend = fopen(filename,"rb");
	
	if(fileToSend == NULL){
		printf("open file failed!\n");
		exit(-1);
	}
	
	int sockfd;
	char * localhost = "127.0.0.1";
	unsigned char buf[BUFLEN];
// 	unsigned char bufrecv[RECVLEN];
	int n;
    struct sockaddr_in servaddr;
	
    memset(buf,0,BUFLEN);
// 	memset(bufrecv,0,RECVLEN);
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
    send(sockfd,filename,strlen(filename),0);//向服务器发送文件名
	
	fseek(fileToSend,0,SEEK_END);
	long filesize = ftell(fileToSend);//发送文件长度
	send(sockfd,&filesize,sizeof filesize,0);

	long num;//从服务器收到的确认信息
	n = recv(sockfd,&num,sizeof num,0);
	if(num == filesize	){//>>>>>>>>>>>>>>>>>>>>>>>服务器返回的数字等于文件长度,则发送文件内容.
							//>>>>>>>>>>>>>>>>>>>>>收到服务器的接收完毕信息后,发送解密密钥.
							//>>>>>>>>>>>>>>>>>>>>>收到服务器的最终确认信息,中断连接
		printf("server permitted!\n");
		printf("begin transporting...\n");
				
	}else{//服务器返回的数字不等于文件长度,显示拒绝信息,推出.
		printf("server denied!\n");
	}
	close(sockfd);
    exit(0);

}
