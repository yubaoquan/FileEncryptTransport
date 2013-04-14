#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <netinet/in.h>
#include <stdint.h>
#include <unistd.h>
#include <time.h>

#include "encrypt.h"
#include "bit.h"
#include "client.h"

#define SERVERPORT 3333 /*server's port */ 

int main_c(void){

	FILE * fileToSend = NULL;
	char filename[20];
	printf("请输入要传输的文件名：\n");
	scanf("%s",filename);
	printf("filename: %s\n",filename);
	fileToSend = fopen(filename,"rb");
	
	if(fileToSend == NULL){
		printf("open file failed!\n");
		exit(-1);
	}
	
	int sockfd;
	char localhost[20];// = "127.0.0.1";
	printf("请输入要传输到的主机IP：\n");
	scanf("%s",localhost);
	unsigned char * fileBuf = NULL;
	unsigned char * encryptBuf = NULL;
	int i = 0;
    struct sockaddr_in servaddr;
	
	//获取文件长度
	fseek(fileToSend,0,SEEK_END);
	long filesize = ftell(fileToSend);
	
	int bu = 8 - (filesize % 8);//用于申请内存时使内存的大小是8的整数倍，避免加密时指针越界
	fileBuf = calloc(filesize + bu,sizeof(char));//初始化文件缓冲区
	//printf("before encrypt fileBuf: %ld\n",(long)fileBuf);
	encryptBuf = calloc(filesize + bu,sizeof(char));//初始化加密缓冲区
	
	if(fileBuf == NULL || encryptBuf == NULL){
		printf("calloc buffer for file failed!\n");
		exit(-1);
	}
	fseek(fileToSend,0,SEEK_SET);//指针指向文件开头
	fread(fileBuf,sizeof(unsigned char), filesize,fileToSend);//读入文件
	fclose(fileToSend);
	//获取当前系统时间、对密钥赋值初始化
	time_t timekey = time(NULL);//long 32bit
	unsigned char key[8];
	memcpy(key,&timekey,sizeof(timekey));
	memcpy(key + 4,&timekey,sizeof(timekey));

	//开始加密
	
	for(i = 0;i < filesize; i += 8){
		des_encipher(fileBuf + i,encryptBuf + i,key);
	}
		
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
	
	//发送文件长度
	send(sockfd,&filesize,sizeof filesize,0);

	long num;//从服务器收到的确认信息
	char yn;
	char finishSign;
	memset(&finishSign,0,10);
	recv(sockfd,&num,sizeof num,0);
	//服务器返回的数字等于文件长度,则发送文件内容.
	if(num == filesize	){
		printf("server permitted!\n");
		printf("begin file transporting...\n");
		send(sockfd,encryptBuf,filesize,0);
		
		//收到服务器的接收完毕信息后,发送解密密钥.
		recv(sockfd,&yn,sizeof(yn),0);
		if(yn == 'y'){
			printf("transporting the key : %ld...\n",*(long *)key);
			send(sockfd,&key,8 * sizeof(char),0);
		}
		//收到服务器的最终确认信息
		recv(sockfd,&finishSign,sizeof(finishSign),0);
		if(finishSign == 'f'){
			printf("all operation finished!bye\n");
		}
		
	}else{//服务器返回的数字不等于文件长度,显示拒绝信息,推出.
		printf("server denied!\n");
	}
	//中断连接
	close(sockfd);
	//释放动态内存
	
	free(fileBuf);
	fileBuf = NULL;
	free(encryptBuf);
	encryptBuf = NULL;
    exit(0);

}
