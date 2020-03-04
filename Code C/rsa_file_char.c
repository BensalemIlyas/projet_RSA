#include <stdio.h>
#include <stdlib.h>
#include "rsa_header.h"
#include "Sha-256/sha256_utils.h"
#include "RSAcrypt.c"


/*convertion en base 64*/
// avec les fichiers
/*testez rapidement sans makefile: gcc -g -Wall phase2-2.c other_base64.c rsa_file_char.c bezout.c rsa_tools.c rsa_print_tools.c -o phase2-2*/



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

  fseek(inFile,0,SEEK_END);  //se positionner sur le dernier octet du fichier
  // la fonction ftell() renvoit la position de l'octet sur lequel se trouve le fichier:  ce qui correspond ici à la taille du fichier en octet
  int tailleOctetFichier = ftell(inFile);
  rewind(inFile);
  uchar buffer[tailleOctetFichier]; //chaine de caractère qui va contenir le message à crypter
  fread(buffer, tailleOctetFichier, 1 ,inFile);

  /*-Utiliser fonction d'aurélie void RSAcrypt(unsigned char *msg, uint64 * crypterMsg, rsaKey_t pubKey);*/
  uint64 cryptedMsg[tailleOctetFichier];// contiens le message crypté en RSA caractère par caractère
  RSAcrypt(buffer, cryptedMsg, pubKey);
  /*
  encoder chaque nombre(qui représente une lettre cryptée en RSA) en une chaine de caractère en base64
  écrire chaque  chaine de taille output_length sur le outputFile
  */
  int length = 0;
  uint64 encode_int;
  for(int i  = 0; i <tailleOctetFichier; i++){
    encode_int  = cryptedMsg[i];
    char *encode_char = base64_encode(&encode_int, sizeof(uint64), output_length);
    fseek(inFile,length,SEEK_SET);
    fwrite(encode_char,  1, *output_length,  outFile);
    free(encode_char);
    length += *output_length;
  }

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
 int nb_int64 = tailleOctetFichier /length; //nombre de chaines encodées en base64
  uint64 cryptedMsg[nb_int64]; //tableau qui va contenir le message décodé(mais toujours crypté en RSA)
  rewind(inFile);
  char * char_encode = malloc( length * sizeof(char)); //chaine de caractère représentant une lettre encodée en base64
  int location = 0;
  int output_length = 0;
  uint64 cUnCrypt;
//tant qu'il reste quelquechose à lire le ranger dans char_encode de taille length(output_length de la fonction RSAcrypt)File
  while(fread(char_encode,1, length ,inFile)){

    uint64 *int_decode = base64_decode(char_encode,length, &output_length); // entier décodé mais crypte
    cUnCrypt = puissance_mod_n( *int_decode, privKey.E, privKey.N );// CUnCRypt caractère décrypte chaque caractère représente une lettree du message initial
    //printf("%c\n",cUnCrypt);
    fprintf(outFile,"%c",cUnCrypt);
    location += length;

  }

  fclose(inFile);
  fclose(outFile);

}
