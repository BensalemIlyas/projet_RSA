
#include "rsa_header.h"


/*gcc -g -Wall phase2-1.c RSAcrypt.c bezout.c rsa_tools.c rsa_print_tools.c -o phase2-1*/

void RSAcrypt(unsigned char *msg, uint64 *cryptedMsg, rsaKey_t pubKey){

  int taille_crypted_message = strlen(msg);

  for(int i =  0; i < taille_crypted_message ; i++){
      cryptedMsg[i]  = puissance_mod_n( msg[i], pubKey.E, pubKey.N );
  }



}
void RSAdecrypt(unsigned char *msg, uint64 *cryptedMsg, rsaKey_t privKey){

  uint64 char_decode;
  int i = 0;
  while(cryptedMsg[i] != '\0'){

      char_decode = puissance_mod_n( cryptedMsg[i], privKey.E, privKey.N );/*C^privKey mod n > capacité uchar ==> dois passer par uint64*/
      msg[i] = char_decode;
      ++i;

  }
  *cryptedMsg = NULL;


}
