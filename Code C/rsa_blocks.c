#include "rsa_header.h"

/*gcc -Wall rsa_blocks.c phase2-3.c int2char.c other_base64.c rsa_tools_gmp.c rsa_tools.c bezout.c -o test -lgmp */

uint64 RSAcrypt1BlockGmp(uint64 blockInt, rsaKey_t pubKey) {
    mpz_t cryptBlock;
    puissance_mod_n_gmp(cryptBlock,blockInt,pubKey.E,pubKey.N);
    return mpz_get_ui(cryptBlock);
}

uint64 RSAdecrypt1BlockGmp(uint64 blockInt, rsaKey_t privKey) {
    mpz_t decryptBlock;
    puissance_mod_n_gmp(decryptBlock,blockInt,privKey.E,privKey.N);
    return mpz_get_ui(decryptBlock);
}


void RSAfile_crypt(char *inFilename, char *outFilename, rsaKey_t pubKey) {
    int tailleOctetFichierIn;
    uchar buffer[tailleOctetFichierIn]; //y stocker contenu du fichierIn
    block_t monBlock = {0,0,0,0}; //mon bloc
    uint64 blockInt,blockCrypted;
    char* encode_char;
    int output_length;
    
    FILE* fichierIn = fopen(inFilename,"r");
    if(fichierIn == NULL) {
        erreur("\nOuverture du fichier inFilename impossible!\n");
    }
    
    FILE* fichierOut = fopen(outFilename,"w");
    if(fichierOut == NULL) {
        erreur("\nOuverture du fichier outFilename impossible!\n");
    }
    
    fseek(fichierIn,0,SEEK_END); //positionne sur dernier octet du fichierIn
    tailleOctetFichierIn = ftell(fichierIn)-1; //connaître position de l'octet sur lequel l'index est: connaître donc la taille du fichier en octet
    rewind(fichierIn); //revient au début du fichier 
    fread(buffer,tailleOctetFichierIn,1,fichierIn); //dans buffer contenu de fichierIn
    
    
    for(int i=0; i<tailleOctetFichierIn; i=i+4) {
        int cmpt=i;
        for(int j=0; j<BLOCK_SIZE; j++,cmpt++) monBlock[j]=buffer[cmpt];
        blockInt = convert_4byte2int(monBlock); //uchar --> uint64:
        blockCrypted = RSAcrypt1BlockGmp(blockInt,pubKey);
        encode_char = base64_encode(&blockCrypted,sizeof(uint64),&output_length); //uint64 --> base64 
        fprintf(fichierOut,"%s",encode_char); //on écrit dans fichierOut ce qu'on vient d'encoder
    }
  
  
    fclose(fichierIn);
    fclose(fichierOut);
    
}



void RSAfile_decrypt(char *inFilename, char *outFilename, rsaKey_t privKey) {
    uchar* tabBlock = malloc(BLOCK_SIZE*sizeof(uchar));
    uchar* encode_char = malloc(BLOCK_BASE_64*sizeof(uchar));
    uint64* blockCryptedInt;
    uint64 blockDecrypted;
    int* output_length;
    
    FILE* fichierIn = fopen(inFilename,"r");
    if(fichierIn == NULL) {
        erreur("\nOuverture du fichier inFilename impossible!\n");
    }
    
    FILE* fichierOut = fopen(outFilename,"w");
    if(fichierOut == NULL) {
        erreur("\nOuverture du fichier outFilename impossible!\n");
    }
    
    //on lit le fichierIn par 1bloc de 12octets
    while(fread(encode_char,BLOCK_BASE_64,1,fichierIn)) {
        blockCryptedInt = base64_decode(encode_char,BLOCK_BASE_64,&output_length);
        blockDecrypted = RSAdecrypt1BlockGmp(*blockCryptedInt,privKey);
        convertInt2uchar(blockDecrypted,tabBlock);
        
        for(int i=0; i<BLOCK_SIZE; i++) {
            /* boucle if nécessaire --> quand taille d'un bloc <4:
            si bloc={'o','k',' ',' '}, affiche: ok\00\00 */
            if(tabBlock[i] != 0) fprintf(fichierOut,"%c",tabBlock[i]);
        }
    }
    
    fclose(fichierIn);
    fclose(fichierOut);
    
}

