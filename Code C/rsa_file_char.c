#include <stdio.h>
#include <stdlib.h>
#include "rsa_header.h"
#include "Sha-256/sha256_utils.h"



/*convertion en base 64*/


void  RSAfile_crypt(char *inFilename, char *outFilename,rsaKey_t pubKey){
    /*Alogirthme
-Ouvrir le fichier sur lequel on va lire le message à encoder en lecture*/
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
    fread(buffer, tailleOctetFichier , 1 ,inFile);

    /*-Utiliser fonction d'aurélie void RSAcrypt(unsigned char *msg, uint64 * crypterMsg, rsaKey_t pubKey);*/
    uint64 crypterMsg[tailleOctetFichier];

    RSAcrypt(buffer, crypterMsg, pubKey);
    for(int i = 0; i < tailleOctetFichier; i++){
        buffer[i] = crypterMsg[i];
    }
    /*
    utilisation la fonction donné sur le résultat de la fonction d'aurélie:
        char *base64_encode(const unsigned char *data, size_t input_length, size_t ouput_length);
        */
    size_t input_length = tailleOctetFichier;
    printf("%d\n", tailleOctetFichier);
    size_t output_length = 0;
  
    base64_encode(buffer, input_length, &output_length);
    /*
     *
    ecrire la chaine de caractère dans le fichier en écriture
    */

    fwrite(buffer,output_length, 1, outFile);
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
    uchar buffer[tailleOctetFichier];
    fread(buffer, tailleOctetFichier , 1 ,inFile);

/*utiliser la fonction decode64*/
    size_t input_length = tailleOctetFichier;
    size_t output_length = 0;
    base64_decode(buffer, input_length, &output_length);
    /*
     *

    /*-Utiliser fonction d'aurélie void RSAdecrypt(unsigned char *msg, uint64 * crypterMsg, rsaKey_t pubKey);*/
    uint64 crypterMsg[tailleOctetFichier];
    RSAcrypt(buffer, crypterMsg, privKey);
    for(int i = 0; i < output_length; i++){
        buffer[i] = crypterMsg[i];
    }

    /*
    ecrire la chaine de caractère dans le fichier en écriture
    */

    fwrite(buffer,tailleOctetFichier, 1, outFile);
    fclose(inFile);
    fclose(outFile);
}
}
