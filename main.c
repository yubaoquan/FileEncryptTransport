#include <stdio.h>
#include <stdlib.h>

#include "client.h"
#include "server.h"

int main(void){
	printf("want to be a server or client?[s/c]\n");
	char choice = 'a';
	scanf("%c",&choice);
	switch(choice){
		case 's':
			main_s();
			break;
		case 'c':
			main_c();
			break;
		default:
			printf("error!\n");
			break;
	}
	return 0;
}