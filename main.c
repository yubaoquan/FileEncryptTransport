#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "encrypt.h"

#define MAXLEN 80000
/*目前密钥只能支持八个字符,明文只能支持八个字符*/
int  main(){
	
	
	unsigned char array[MAXLEN];
	unsigned char * fileBeforeEn = calloc(MAXLEN,sizeof(char));
	unsigned char * fileAfterDe = calloc(MAXLEN,sizeof(char)); 
	unsigned char * fbecpy;
	if(fileBeforeEn == NULL || fileAfterDe == NULL){
		printf("alloc fail!\n");
		exit(-1);	
	}else{
		printf("alloc succeed!\n");
		fbecpy = fileBeforeEn;
		char * key = "abcd";
		
		memcpy(fileBeforeEn,"xyz xyz",8);
		
		printf("plaintext: %s\n",fileBeforeEn);
		des_encipher(fileBeforeEn,fileAfterDe,key);
		printf("ciphertext: %s\n",fileAfterDe);
		des_decipher(fileAfterDe,array,key);
		printf("after decrypt: %s \n",array);
		
		free(fileBeforeEn);
 		fileBeforeEn = NULL;
		free(fileAfterDe);
 		fileAfterDe = NULL;
 		

	}
	
	getc(stdin);
	return;
}
