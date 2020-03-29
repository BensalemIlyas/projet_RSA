/// \author Vincent Dugat
/// \date summer 2019
#include "rsa_header.h"
/* gcc -g -Wall test_crypt.c int2char.c rsa_tools_gmp.c rsa_file_blocks.c bezout.c rsa_tools.c rsa_print_tools.c -o test_crypt -lgmp
*/
FILE *logfp;

int main(int argc,char **argv){
  if (argc>1){
    logfp = fopen(argv[1],"w+");
    assert(logfp!=NULL);
  }
  else
    logfp = stdout;

// Phase 6.2
// numérisation, cryptage et inverse.
  rsaKey_t pubKeyb = {23,8889895013};
  rsaKey_t privKeyb = {3865086887,8889895013};

  FILE *f_in =fopen("Tests/res2.txt","r");
  char num[32];
  char res[32];
  //printf("%lu\n",puissance_mod_n (1000, 23, 8889895013));

  while (!feof(f_in)){
    fscanf(f_in,"%s",num);
    fscanf(f_in,"%s",res);
    uint64 numInt = (uint64) strtol(num, (char **)NULL, 10);
    uint64 resInt = (uint64) strtol(res, (char **)NULL, 10);
    printf("num=%s res=%s ",num,res);
    printf("numInt=%lu resInt=%lu ",numInt,resInt);
    mpz_t calc;
    mpz_init (calc);
    puissance_mod_n_gmp(calc,numInt, pubKeyb.E, pubKeyb.N);
    gmp_printf("%s is calc %Zd\n", "here", calc);
    if (mpz_cmp_ui (calc,resInt)!=0)
      printf("PB");
    printf("\n");
  }
  return 0;
}
