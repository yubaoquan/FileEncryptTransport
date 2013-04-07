#include <sys/socket.h>
#include <arpa/inet.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <netinet/in.h>
#include <stdint.h>
#include <unistd.h>

#define SERVPORT 3333 /*服务器段口号 */ 
#define BACKLOG 10 /* 最大接收链接数*/ 
#define BUFLEN 100
#define SENDLEN 10
#define RECVLEN 10

int main(){
	int sockfd,client_fd;
	int sin_size;
	int n;
	unsigned char buf[BUFLEN];
	unsigned char bufsend[SENDLEN];
	unsigned char bufrecv[RECVLEN];
	memset(buf,0,BUFLEN);
	memset(bufsend,0,SENDLEN);
	memset(bufrecv,0,RECVLEN);
	
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
	while(1){
		sin_size = sizeof(struct sockaddr_in); 
		if ((client_fd = accept(sockfd, (struct sockaddr *)&remote_addr, &sin_size))== -1) { 
			perror("accept出错"); 
			continue; 
		} 
		printf("received a connection from %s\n", inet_ntoa(remote_addr.sin_addr));
		if((n = recv(client_fd,buf,BUFLEN,0))> 0){//接受文件名
			printf("file name: %s\n",buf);
			
			printf("\nreceived %d char,receive ?[y/n]",n);//请求接收许可
			char yn = getc(stdin);//等待用户输入,是否允许接受文件
			char * admitsign;//信号:允许/拒绝文件接受
			if(yn == 'y'){
				admitsign = "ok";
				memcpy(buf,admitsign,strlen(admitsign));
				send(client_fd,buf,strlen(admitsign),0);
				printf("admitted.\n");
				int len;
				if(recv(client_fd,&len,sizeof len,0) != 0){//接受文件大小
					printf("file lenth: %d\n",len);
				}
				
				//>>>>>>>>>>>>>后续处理,向客户端发送收到的文件大小进行确认,
				//>>>>>>>>>>>>>准备缓冲区,开始接收文件
			}else{//拒绝接收文件并发送拒绝信号
				admitsign = "no";
				memcpy(buf,admitsign,strlen(admitsign));
				send(client_fd,buf,strlen(admitsign),0);
				printf("denied.\n");
			}
			
		}
	}
	printf("receive finished!\n");
	close(sockfd);
}
