/// \file rsa_header.h
/// \author Vincent Dugat
/// \date august 2019
#ifndef RSA_H
#define RSA_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <time.h>
#include "Sha-256/sha256_utils.h"
#include <gmp.h>

#define MAX_U_INT 4294967296
#define MAX_U_INT64 18446744073709551616
#define OK 0
#define ERREUR -1
#define READ_ERROR -1
#define BLOCK_SIZE 4 // octets
#define BLOCK_BASE_64 12
#define NAME_MAX_SIZE 32 // caractères
#define MAX_STR 10 //

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAXI(a,b) (((a)>(b))?(a):(b))

////////////////////////////A AJOUTER
#define COMMENTAIRE_SIZE 50
#define MAXBUF 256 //taille buffer de lecture de fichiers
#define NB_CMD 16 //nombre de commandes possibles pour (v1.1) et (v2)
#define CMD_M 0 //man
#define CMD_LK 1 //listkeys
#define CMD_RM 2 //rmkeys
#define CMD_NK 3 //newkeys
#define CMD_C 4 //crypt
#define CMD_U 5 //uncrypt
#define CMD_S 6 //save
#define CMD_SP 7 //savepub
#define CMD_L 8 //load
#define CMD_SH 9 //show
#define CMD_Q 10 //quit

#define CMD_LC 11 //listcontacts
#define CMD_AC 12 //addcontact
#define CMD_MC 13 //modifycontact
#define CMD_AK 14 //addkeys
#define CMD_RC 15 //rmcontact

///////////////////////////////////



extern FILE * logfp;

typedef unsigned long int uint64;
typedef unsigned int uint; // même taille que int aka uint32
typedef unsigned char uchar; // 8 bits = octet aka uint8

/* Type of a block of data */
typedef uchar block_t[BLOCK_SIZE]; // une case par octet

typedef struct tabUint64_s{
	uint64 * uint64array;
	int dim;
} tabUint64_t;

typedef struct rsaKey_s {
	uint64 E;// exposant
	uint64 N;// modulo
} rsaKey_t;

// struct pour définir une paire de clefs
typedef struct keyPair_s {
	rsaKey_t pubKey; // (C,N)
	rsaKey_t privKey; //(U,N)
} keyPair_t;

////////////////////////////////////////////////////////////////////////
//ma structure pour les identificateurs
typedef struct identificateur_s {
    int id;
    keyPair_t pairKey; //paire de clés: pairKey.pubKey && pairKey.privKey
    char type[NAME_MAX_SIZE];
} identificateur_t;


//ma structure pour les contacts
typedef struct contact_s {
    char identificateur[NAME_MAX_SIZE];
    char name[NAME_MAX_SIZE];
    char surname[NAME_MAX_SIZE];
    char commentaire[COMMENTAIRE_SIZE];
    int tailleId;
    identificateur_t *listeId;
} contact_t;

//ma structure pour les utilisateurs
typedef struct utilisateur_s {
    char name[NAME_MAX_SIZE]; //prenom
    char surname[NAME_MAX_SIZE];  //nom de famille
    int nbId; //nombre identificateur, taille tableau ci-dessous
    identificateur_t *tabId;    //pour avoir un tab d'identificateur
    int nbContact;
    contact_t *annuaire;
} utilisateur_t;
////////////////////////////////////////////////////////////////////////


// prototypes de sp
void erreur(char* msg); // pour afficher les msg d'erreurs

// phase 1
int decompose (uint facteur[], uint64 n) ; // reçoit n, remplit le tableau de facteurs premiers (realloc)
uint64 puissance_mod_n (uint64 a, uint64 e, uint64 n); // puissance modulaire
long bezoutRSA(uint a,uint b,long *u,long *v); // Bézout
void printKey(rsaKey_t key);
void printUint64Array(tabUint64_t tab);
void genKeysRabin(rsaKey_t *pubKey,rsaKey_t *privKey);

//phase 2
void RSAcrypt(unsigned char *msg, uint64 *cryptedMsg, rsaKey_t pKey);
void RSAdecrypt(unsigned char *msg, uint64 *cryptedMsg, rsaKey_t privKey);
uint convert_4byte2int(uchar *b);
void printBlock(block_t blk);
void convertInt2uchar(uint nb,uchar *tab4bytes);
char *base64_encode(const uchar *data,size_t input_length,size_t *output_length);
unsigned char *base64_decode(const char *data,size_t input_length,size_t *output_length);

// avec les blocs
uint64 RSAcrypt1BlockGmp(uint64 blockInt, rsaKey_t pubKey);
uint64 RSAdecrypt1BlockGmp(uint64 blockInt, rsaKey_t privKey);

// avec les fichiers
void RSAfile_crypt(char *inFilename,char *outFilename, rsaKey_t pubKey);
void RSAfile_decrypt(char *inFilename,char *outFilename,rsaKey_t privKey);
void RSAcryptFile(char *inFilename,
                    char *outFilename,
                    rsaKey_t pubKey,
                    int *output_length);
void RSAunCryptFile(char *inFilename,char *outFilename,rsaKey_t privKey, int length);

// signature
tabUint64_t * signText(char *inFilename,rsaKey_t signKey);
void inputKey(uint64 E,uint64 N,rsaKey_t *key);
tabUint64_t * string2intBlocks(uchar *str);
uchar * num2string(tabUint64_t numArray);
uint64 * giveMeMem(int dim);
tabUint64_t * RSAcryptUint64(tabUint64_t numMsg,rsaKey_t pubKey);
tabUint64_t * RSAunCryptUint64(tabUint64_t numMsgC,rsaKey_t privKey);
// GMP
void mersenneGmp(mpz_t resGmp,uint64 max,uint64 p); // ovni
void printKeyPair(keyPair_t keyPair);
char * verifySignText(char *inFilename,tabUint64_t cryptAr, rsaKey_t verifyKey);
void puissance_mod_n_gmp(mpz_t res,uint64 a, uint64 e, uint64 n); // puis mod avec gmp

///fichier rsa_cmd.c - gestion_erreur.c
void man();
void listkeys(utilisateur_t* user, char* param1);
void rmkeys(utilisateur_t* user, char* param1);
void newkeys(utilisateur_t* user, char* param1, char* param2);
void crypt(utilisateur_t* user, char* param1, char* param2, char* param3);
void uncrypt(utilisateur_t* user, char* param1, char* param2, char* param3);
void save(utilisateur_t* user, char* param1);
void savepub(utilisateur_t* user, char* param1, char* param2);
void load(utilisateur_t *user, char* param1);
void show(utilisateur_t* user, char* param1, char* param2, char* param3);

int test_fichier_lecture(char *filename);
int test_fichier_ecriture(char *filename);
int couper_chaine(char *chaine, char* commande_cut);
void analyse_commandes(char* commande_cut, int tab_cmd[NB_CMD]);
void parametres(char* chaine, int tailleChaine, int tailleCmd, char* param1, char* param2, char* param3);
void execution_commande(int tab_cmd[NB_CMD], utilisateur_t* user, char* param1, char* param2, char* param3);
void gestion_erreur_commande(char* chaine, int tailleChaine, int tab_cmd[NB_CMD], int tailleCmd, int* nb_erreur);
void init_user(utilisateur_t* user);

int erreur_listkeys(utilisateur_t* user, char* param1, char* param2, char* param3);
int erreur_rmkeys(utilisateur_t* user, char* param1, char* param2, char* param3);
int erreur_newkeys(utilisateur_t* user, char* param1, char* param2, char* param3);
int erreur_crypt(utilisateur_t* user, char* param1, char* param2, char* param3);
int erreur_uncrypt(utilisateur_t* user, char* param1, char* param2, char* param3);
int erreur_save(char* param1, char* param2, char* param3);
int erreur_savepub(utilisateur_t* user, char* param1, char* param2, char* param3);
int erreur_load(char* param1, char* param2, char* param3);
int erreur_show(utilisateur_t* user, char* param1, char* param2, char* param3);
int erreur_listcontacts(utilisateur_t* user, char* param1, char* param2, char* param3);
int erreur_addcontact(utilisateur_t* user, char* param1, char* param2, char* param3);
int erreur_modifycontact(utilisateur_t* user, char* param1, char* param2, char* param3);
int erreur_addkeys(utilisateur_t* user, char* param1, char* param2, char* param3);
int erreur_rmcontact(utilisateur_t* user, char* param1, char* param2, char* param3);


//fichier rsa_contacts.c
void listcontacts(utilisateur_t* user, char* param1, char* param2);
void addcontact(utilisateur_t* user, char* param1);
void modifycontact(utilisateur_t* user, char* param1);
void addkeys(utilisateur_t* user, char* param1);
void rmcontact(utilisateur_t* user, char* param1);

#endif
