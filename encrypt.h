#ifndef ENCRYPT_H
#define ENCRYPT_H

typedef unsigned long Huge;

void des_encipher(const unsigned char * plaintext,unsigned char * ciphertext,const unsigned char * key);
void des_decipher(const unsigned char * ciphertest,unsigned char * plaintext,const unsigned char * key);
#endif
