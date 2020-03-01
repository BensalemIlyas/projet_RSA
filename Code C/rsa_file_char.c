#include <stdio.h>
#include <stdlib.h>
#include "rsa_header.h"
#include "Sha-256/sha256_utils.h"
#include "RSAcrypt.c"



/*convertion en base 64*/
// avec les fichiers

void RSAcryptFile(char *inFilename,
                    char *outFilename,
                    rsaKey_t pubKey,
                    int *output_length)
{

  RSAfile_crypt(inFilename, outFilename, pubKey);
  FILE *outFile= fopen(outFilename, "r");
  if(outFile == NULL){
       erreur("rsa_file_char.c:RSAcryptFile :ouverture outFilename");
  }
  fseek(outFile,0,SEEK_END);
  *(output_length)= ftell(outFile);

}


void RSAunCryptFile(char *inFilename,
                    char *outFilename,
                    rsaKey_t privKey,
                     int length)
{

  RSAfile_decrypt(inFilename, outFilename, privKey);

}



void  RSAfile_crypt(char *inFilename, char *outFilename,rsaKey_t pubKey){
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
        char *base64_encode(const unsigned char *data, size_t input_length, size_t ouput_length);
        */


    size_t output_length = 0;
  /**attends u_*/

    uchar * encodeMsg = malloc( (tailleOctetFichier + 1) * sizeof(uchar));
    encodeMsg = base64_encode(cryptedMsg, tailleOctetFichier, &output_length);
    printf("output : %d\n",sizeof(uint64)* tailleOctetFichier );
    encodeMsg[tailleOctetFichier] = cryptedMsg[tailleOctetFichier];



      for(int i = 0 ; i <tailleOctetFichier + 1; ++i){
        printf("nbre encoder : %d\n",encodeMsg[i] );
      }

    /*
     *
    ecrire la chaine de caractère dans le fichier en écriture
    */

    fwrite(encodeMsg, tailleOctetFichier + 1 , 1, outFile);
    fclose(inFile);
    fclose(outFile);
}

void RSAfile_decrypt(char* inFilename, char *outFilename, rsaKey_t privKey){

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


   /*
-mettre toutes les données du fichier en lecture dans une chaine de caractère*/
    //se positionner sur le dernier octet du fichier
    fseek(inFile,0,SEEK_END);
    // la fonction ftell() renvoit la position de l'octet sur lequel se trouve le fichier:  ce qui correspond ici à la taille du fichier en octet
    int tailleOctetFichier = ftell(inFile);
    rewind(inFile);
    uchar cryptedMsg[tailleOctetFichier];
    fread(cryptedMsg, tailleOctetFichier , 1 ,inFile);
    for(int i = 0 ; i <tailleOctetFichier; ++i){
      printf("nbre encoder dans decrypt : %d\n",cryptedMsg[i] );
    }

/*utiliser la fonction decode64*/
    uint64 * msgDecode = malloc( tailleOctetFichier * sizeof(uint64));
    size_t output_length = 0;
    /*  unsigned char *msgd = malloc(100*sizeof(unsigned char));*/
    msgDecode  = base64_decode(cryptedMsg ,4 * (((tailleOctetFichier ) + 2) / 3) , &output_length);
;
/*4 * ((input_length + 2) / 3)*/
    msgDecode[tailleOctetFichier - 1] = cryptedMsg[tailleOctetFichier - 1];
    for(int i = 0 ; i <tailleOctetFichier; ++i){
      printf("nbre decoder dans decrypt : %d\n",msgDecode[i] );
    }
    /*-Utiliser fonction d'aurélie void RSAdecrypt(unsigned char *msg, uint64 * crypterMsg, rsaKey_t pubKey);*/
    uchar buffer[tailleOctetFichier - 1];/*+ 1 car chaine*/
    RSAdecrypt(buffer, msgDecode , privKey);
    printf("%s\n",buffer );

    /*
    ecrire la chaine de caractère dans le fichier en écriture
    */

    fwrite(buffer,output_length, 1, outFile);
    fclose(inFile);
    fclose(outFile);
}
