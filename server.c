#include <sys/socket.h>
#include <arpa/inet.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <netinet/in.h>
#include <stdint.h>
#include <unistd.h>

#include "encrypt.h"
#include "bit.h"

#define SERVPORT 3333 /*服务器段口号 */ 
#define BACKLOG 10 /* 最大接收链接数*/ 
#define BUFLEN 20

int main(){
	int sockfd,client_fd;
	int sin_size;
	int n;
	unsigned char buf[BUFLEN];
	unsigned char *filebuf = NULL;
	char  filename[BUFLEN];
	memset(buf,0,BUFLEN);
	memset(filename,0,BUFLEN);
	
	struct sockaddr_in my_addr; /*本地socket地址*/ 
	struct sockaddr_in remote_addr; /*远端socket地址*/ 

	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
		perror("error creating socket!");
		exit(-1);
	}
	my_addr.sin_family=AF_INET; 
	my_addr.sin_port=htons(SERVPORT); 
	my_addr.sin_addr.s_addr = INADDR_ANY; 
	bzero(&(my_addr.sin_zero),8); 
	
	if (bind(sockfd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr))  == -1) { /*将套接字与地址绑定*/
		perror("bind出错！"); 
		exit(1); 
	} 
	if (listen(sockfd, BACKLOG) == -1) { 
		perror("listen出错"); 
		exit(1); 
	} 
		sin_size = sizeof(struct sockaddr_in); 
		if ((client_fd = accept(sockfd, (struct sockaddr *)&remote_addr, &sin_size))== -1) { 
			perror("accept出错"); 
			exit(-1);
		} 
		printf("received a connection from %s\n", inet_ntoa(remote_addr.sin_addr));
		
		if((n = recv(client_fd,buf,BUFLEN,0))> 0){//接受文件名
			memcpy(filename,buf,strlen(buf));
			printf("file name: %s\n",filename);//打印文件名
		}
		long len;//文件长度
		if(recv(client_fd,&len,sizeof len,0) != 0){//接受文件大小
			printf("file lenth: %ld\n",len);
			printf("receive ?[y/n]");//请求接收许可
		}
			
		char yn = getc(stdin);//等待用户输入,是否允许接受文件
		
		if(yn == 'y'){
			printf("admitted.\n");
			
			send(client_fd,&len,sizeof len,0);//>>>>>>>>>>>>>发送文件长度，准备缓冲区，开始接受文件内容
												//>>>>>>>>>>>>文件内容接收完毕后向客户端发送确认信息，准备接收密钥
												//>>>>>>>>>>>>收到密钥以后向客户端发送最终确认,中断链接
												//>>>>>>>>>>>>进行解密并存储文件
			//filebuf = calloc(len,sizeof(char));
			/*
			if(filebuf == NULL){
				printf("fail to alloc the buffer!\n");
				exit(-1);
			}else{
				while(recv(client_fd,filebuf,len,0) != 0);
				printf("receive finished!decrypting...\n");
				//>>>>>>>>>>>>>解密文件
				//>>>>>>>>>>>>>存储文件
				FILE *targetFile = NULL;
				targetFile = fopen(filename,"wb");
				
				if(targetFile == NULL){
					printf("open file failed!\n");
					exit(-1);
				}else{
				//	fwrite(filebuf,sizeof(char),len,targetFile);
				}
				printf("all operation finished!bye\n");
				exit(0);
			}
			*/
		}else{//拒绝接收文件并发送拒绝信号
			len = 0;
			send(client_fd,&len,sizeof len,0);
			printf("denied.\n");
			close(sockfd);
			return 0;
		}
	close(client_fd);
	close(sockfd);
	return 0;
}
