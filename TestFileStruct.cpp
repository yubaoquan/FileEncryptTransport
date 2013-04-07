#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define FILENAMELEN 20
#define MAXLEN 100

int main(){
	typedef struct _file{
		unsigned char fileName[FILENAMELEN];
		unsigned char fileContent[MAXLEN];
	}File;
	unsigned char temp[FILENAMELEN + MAXLEN];
	
	char * name = "filename";
	char * content = "this is the content of the file";
	
	File file,*fileptr ;
	fileptr = &file;
	
	memset(fileptr,0,sizeof(struct _file));
	
	memcpy(file.fileName,name,strlen(name));
	memcpy(file.fileContent,content,strlen(content));
	
	printf("%s\n",file.fileName);
	printf("%s\n",file.fileContent);
	
	memcpy(temp,fileptr,sizeof(temp));
	printf("%s\n",temp);
	system("PAUSE");
	return 0;	
} 
