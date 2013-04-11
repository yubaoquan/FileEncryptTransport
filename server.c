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
	int i,n;
	unsigned char buf[BUFLEN];
	unsigned char * filebuf = NULL;//接收到的文件,未解密
	unsigned char * defilebuf = NULL;//解密后的文件
	char  finishSign = 'f';
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
			//发送文件长度，准备缓冲区
			send(client_fd,&len,sizeof len,0);
			filebuf = calloc(len,sizeof(char));
			defilebuf = calloc(len,sizeof(char));
			
			if(filebuf == NULL){
				printf("fail to alloc the buffer!\n");
				exit(-1);
			}else{
				char key[8];
				n = recv(client_fd,filebuf,len,0);
				if(n != 0){
					printf("receiving...\n");
					printf("%d\n",n);
				}
				//文件内容接收完毕后向客户端发送确认信息，准备接收密钥
				send(client_fd,&yn,sizeof(yn),0);
				printf("receive finished!now receiving the the key...\n");
				recv(client_fd,key,8 * sizeof(char),0);
				printf("the key is %ld\n",*(long *)key);
				//收到密钥以后向客户端发送最终确认,中断链接
				send(client_fd,&finishSign,sizeof(finishSign),0);
				//解密文件
				
				for(i = 0;i < len; i += 8){
					des_decipher(filebuf + i,defilebuf + i,key);
					//printf("i = %d, %s\n",i,defilebuf + i);
				}
				//存储文件
				FILE *targetFile = NULL;
				targetFile = fopen(filename,"wb");
				
				if(targetFile == NULL){
					printf("open file failed!\n");
					exit(-1);
				}else{
					fwrite(defilebuf,sizeof(char),len,targetFile);
					//fclose(targetFile);
				}
				
				printf("all operation finished!bye\n");
				exit(0);
			}
			
		}else{//拒绝接收文件并发送拒绝信号
			len = 0;
			send(client_fd,&len,sizeof len,0);
			printf("denied.\n");
			close(sockfd);
			return 0;
		}
	
	free(filebuf);
	filebuf = NULL;
	free(defilebuf);
	defilebuf = NULL;
	
	close(client_fd);
	close(sockfd);
	return 0;
}
