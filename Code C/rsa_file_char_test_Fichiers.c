#include <stdio.h>
#include <stdlib.h>
//#include "rsa_header.h"
#include "Sha-256/sha256_utils.h"
#include <string.h>

 

/*convertion en base 64*/


void  RSAfile_crypt(char *inFilename, char *outFilename){ // ,rsaKey_t pubKey)
    /*Alogirthme 
-Ouvrir le fichier sur lequel on va lire le message à encoder en lecture*/
    FILE *inFile= fopen(inFilename, "r");
   /* if(inFile == NULL){
        erreur("rsa_file_char.c:RSAfile_crypt:ouverture inFilename");
    }*/
     /*
-Ouvrir le fichier sur lequel on va écire en écriture (le créer s'il n'existe pas)*/
    
    FILE *outFile =  fopen(outFilename,"w");
      /*if(outFile == NULL){
        erreur("rsa_file_char.c:RSAfile_crypt:ouverture outFilename");
    }*/
    
   /*
-mettre toutes les données du fichier en lecture dans une chaine de caractère*/
    //se positionner sur le dernier octet du fichier
    fseek(inFile,0,SEEK_END);
    // la fonction ftell() renvoit la position de l'octet sur lequel se trouve le fichier:  ce qui correspond ici à la taille du fichier en octet
    int tailleOctetFichier = ftell(inFile);
    rewind(inFile);
    char buffer[tailleOctetFichier];
    fread(buffer, tailleOctetFichier , 1 ,inFile);
    printf("*%c*\n",buffer[13]);
     printf("*%c*\n",buffer[tailleOctetFichier - 1]);
     printf("%ld\n",strlen(buffer));
    //buffer[tailleOctetFichier - 1] = '\0';
    


    /*
    -Utiliser fonction d'aurélie void RSAcrypt(unsigned char *msg, uint64 * crypterMsg, rsaKey_t pubKey);
    uint64 crypterMsg[tailleOctetFichier];
    RSAcrypt(buffer, crypterMsg, rsaKey_t);
    strncpy(buffer,crypterMsg,tailleOctetFichier);*/
    /*
    utilisation la fonction donnée qui n'a pas encore été donné lol sur le résultat de la fonction d'aurélie: 
        char *base64_encode(const unsigned char *data, size_t input_length, size_t ouput_length); 
    size_t input_length = tailleOctetFichier;
    size_t ouput_length = 0;
    base64_encode(buffer, size_t input_length, *size_t ouput_length);*/
    /*
     * 
    ecrire la chaine de caractère dans le fichier en écriture
    */
    //fwrite(buffer,ouput_length, 1, outFile);
    fwrite(buffer,tailleOctetFichier, 1, outFile);
    fclose(inFile);
    fclose(outFile);
}
int main(int argc, char *argv[]){
    RSAfile_crypt(argv[1], argv[2]);
    int tabEntier [] = {66,67,68};
    (char *)tabEntier;
    printf("%s\n",tabEntier);
    exit(0);
}
