#include "bit.h"
#include <stdio.h>

int bit_get(const unsigned char * bits,int pos){
	unsigned char mask;
	int i;
	mask = 0x80;
	for(i = 0; i < (pos % 8); i++ ){
		mask = mask >> 1;
	}
	return  (((mask & bits[(int)(pos / 8)]) == mask) ? 1 : 0);
}

void bit_set(unsigned char * bits,int pos ,int state){
	unsigned char mask;
	mask = 0x80;
	int i;
	for(i = 0;i < (pos % 8); i ++){
		mask = mask >> 1;
	}
	if(state){
		bits[pos / 8] = bits[pos / 8] | mask;
	}else{
		bits[pos / 8] = bits[pos / 8] & (~mask);
	}
	return;
}

void bit_xor(const unsigned char * bits1,const unsigned char * bits2,unsigned char * bitsx,int size){
	int i;
	for(i = 0; i < size; i ++){
		if(bit_get(bits1,i) != bit_get(bits2,i)){
			bit_set(bitsx,i,1);
		}else{
			bit_set(bitsx,i,0);
		}
	}
	return;
}

void bit_rot_left(unsigned char *bits,int size,int count){
	//printf("here0\n");
	int fbit,lbit;
	int i,j;
	if(size > 0){
		for(j = 0;j < count; j ++){
			for(i = 0;i <= ((size - 1) / 8); i ++){//printf("here1\n");
				lbit = bit_get(&bits[i],0);//printf("here2\n");
				if(i == 0){
					fbit = lbit;
				}else{
					bit_set(&bits[i - 1],7,lbit);
				}
				bits[i] = bits[i] << 1;
			}
		}
	}
	return;
}
