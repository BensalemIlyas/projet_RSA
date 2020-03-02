#include <stdio.h>
#include <stdlib.h>
#include "rsa_header.h"
#include "Sha-256/sha256_utils.h"
#include "RSAcrypt.c"



/*convertion en base 64*/
// avec les fichiers
/* gcc -g -Wall phase2-2.c other_base64.c rsa_file_char.c bezout.c rsa_tools.c rsa_print_tools.c -o phase2-2
*/

void RSAcryptFile(char *inFilename,
                    char *outFilename,
                    rsaKey_t pubKey,
                    int *output_length)
{
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

  /*
  -mettre toutes les données du fichier en lecture dans une chaine de caractère*/
  //se positionner sur le dernier octet du fichier
  fseek(inFile,0,SEEK_END);
  // la fonction ftell() renvoit la position de l'octet sur lequel se trouve le fichier:  ce qui correspond ici à la taille du fichier en octet
  int tailleOctetFichier = ftell(inFile);

  rewind(inFile);
  uchar buffer[tailleOctetFichier];
  fread(buffer, tailleOctetFichier, 1 ,inFile);

  /*-Utiliser fonction d'aurélie void RSAcrypt(unsigned char *msg, uint64 * crypterMsg, rsaKey_t pubKey);*/

  uint64 cryptedMsg[tailleOctetFichier + 1];
  /*ok*/
  RSAcrypt(buffer, cryptedMsg, pubKey);
  for(int i = 0; i < tailleOctetFichier + 1; ++i){
    printf("fin : %d\n",cryptedMsg[i] );
  }
  /*
  utilisation la fonction donné sur le résultat de la fonction d'aurélie:
  */
  int length = 0;
  uint64 encode_int;
  for(int i  = 0; i <tailleOctetFichier + 1; i++){
    encode_int  = cryptedMsg[i];
    char *encode_char = base64_encode(&encode_int, sizeof(uint64), output_length);
    fseek(inFile,length,SEEK_SET);
    fwrite(encode_char, *output_length , 1, outFile);
    length += *output_length;
  }
  printf("%d\n",length );
  /*
  ecrire la chaine de caractère dans le fichier en écriture
  */

    fclose(inFile);
    fclose(outFile);

}


void RSAunCryptFile(char *inFilename,
                    char *outFilename,
                    rsaKey_t privKey,
                     int length)
{
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
    //se positionner sur le dernier octet du fichier
  fseek(inFile,0,SEEK_END);
  // la fonction ftell() renvoit la position de l'octet sur lequel se trouve le fichier:  ce qui correspond ici à la taille du fichier en octet
  int tailleOctetFichier = ftell(inFile);
  rewind(inFile);
 /******************************************/
 int nb_int64 = tailleOctetFichier /length;
  uint64 cryptedMsg[nb_int64];
  rewind(inFile);
  char * char_encode = malloc( length * sizeof(char));
  int location = 0;
  int output_length = 0;

  for(int i = 0; i <  length; i++){
    fseek(inFile,location,SEEK_SET);
    fread(char_encode, length , 1 ,inFile);

      uint64 * int_decode = base64_decode(char_encode,length, &output_length);
    if(location < 192){
      printf("%d\n",*int_decode );
    }
    cryptedMsg[i] = *int_decode;
    location += length;
  }


  /*-Utiliser fonction d'aurélie void RSAdecrypt(unsigned char *msg, uint64 * crypterMsg, rsaKey_t pubKey);*/
  uchar buffer[nb_int64 - 1];
  /*
  ecrire la chaine de caractère dans le fichier en écriture
  */

  RSAdecrypt(buffer, cryptedMsg, privKey);
  printf("%s\n",buffer );

  fwrite(buffer,nb_int64 - 1, 1, outFile);
  fclose(inFile);
  fclose(outFile);


}
