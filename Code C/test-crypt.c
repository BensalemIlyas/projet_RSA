#include "rsa_header.h"
const int batch_length = 4;
FILE *logfp;
/* gcc -g -Wall test-crypt.c int2char.c rsa_tools_gmp.c RSAcrypt.c bezout.c rsa_tools.c rsa_print_tools.c -o test-crypt -lgmp
*/
int main(){
  logfp = stdout;
  clock_t now,end;
  srand(time(NULL));
	rsaKey_t pubKey;
	rsaKey_t privKey;

  printf("Création des clefs aléatoires\n\n");
  now = clock();
  genKeysRabin(&pubKey,&privKey);
  end = clock();

  // TESTS

    block_t test = {'a','b','c','d'};
    block_t tabres;
    uchar *tabRes = malloc(BLOCK_SIZE*sizeof(uchar));
    // avec des uint64
    uint64 rest2 = convert_4byte2int(test);// abcd en uint

    uint64 crypt_gmp = RSAcrypt1BlockGmp(rest2, pubKey);
    uint64 decrypt_gmp = RSAdecrypt1BlockGmp(crypt_gmp, privKey);
    convertInt2uchar(rest2,tabres);
    printBlock(tabres);
    exit(0);
}
