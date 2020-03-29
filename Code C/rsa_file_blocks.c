#include "rsa_header.h"


uint64 RSAcrypt1BlockGmp(uint64 blockInt, rsaKey_t pubKey){
  mpz_t res;
  puissance_mod_n_gmp(res, blockInt, pubKey.E, pubKey.N);
  return mpz_get_ui(res);
}

uint64 RSAdecrypt1BlockGmp(uint64 blockInt, rsaKey_t privKey){

  mpz_t res;
  puissance_mod_n_gmp(res, blockInt, privKey.E, privKey.N);
  return mpz_get_ui(res);
}

void RSAfile_crypt(char *inFilename,char *outFilename, rsaKey_t pubKey){
  /*Alogirthme
  -   Ouvrir le fichier sur lequel on va lire le message à encoder en lecture*/
  FILE *inFile= fopen(inFilename, "r");
  if(inFile == NULL){
       erreur("rsa_file_char.c:RSAfile_crypt:ouverture inFilename");
  }
   /*
  -Ouvrir le fichier sur lequel on va écire en écriture (le créer s'il n'existe pas)*/

  FILE *outFile =  fopen(outFilename,"w");
  if(outFile == NULL){
       erreur("rsa_file_char.c:RSAfile_crypt:ouverture outFilename");
  }
  block_t block_crypt  = {0,0,0,0};
  int output_length ;
  while(fread(block_crypt, 1 , BLOCK_SIZE, inFile) ){
    uint64 block64 = convert_4byte2int(block_crypt);
    uint64 block64_crypt =  RSAcrypt1BlockGmp(block64,pubKey);
    char *encode_char = base64_encode(&block64_crypt, sizeof(uint64), &output_length);
    fwrite(encode_char,  1, output_length,  outFile);
    for(int i = 0;  i< BLOCK_SIZE ; ++i)
      {block_crypt[i] = 0;}
  }

    fclose(inFile);
    fclose(outFile);


}
void RSAfile_decrypt(char *inFilename,char *outFilename,rsaKey_t privKey){
  /*Alogirthme
-Ouvrir le fichier sur lequel on va lire le message à décoder en lecture*/
  FILE *inFile= fopen(inFilename, "r");
  if(inFile == NULL){
       erreur("rsa_file_char.c:RSAfile_decrypt:ouverture inFilename");
  }
   /*
-Ouvrir le fichier sur lequel on va écire en écriture (le créer s'il n'existe pas)*/

  FILE *outFile =  fopen(outFilename,"w");
  if(outFile == NULL){
       erreur("rsa_file_char.c:RSAfile_decrypt:ouverture outFilename");
  }
  /****************************************/


  uchar* char_encode = malloc( BLOCK_BASE_64 * sizeof(char));
  uchar * block_char = malloc( BLOCK_SIZE * sizeof(char));
  int output_length;
  uint64 deCrypt;

  while(fread(char_encode,1, BLOCK_BASE_64 ,inFile)){

    uint64 *int_decode = base64_decode(char_encode, BLOCK_BASE_64, &output_length);
    deCrypt = RSAdecrypt1BlockGmp(*int_decode,privKey);
    convertInt2uchar(deCrypt,block_char);
    for(int i = 0 ; i < BLOCK_SIZE ; i++ ){
      if(block_char[i] != 0){
        fprintf(outFile,"%c",block_char[i]);
      }

    }

  }

  fclose(inFile);
  fclose(outFile);
}
