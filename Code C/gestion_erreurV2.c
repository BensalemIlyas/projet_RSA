#include "rsa_header.h"

//Fonction qui teste la possibilité d'ouvrir un fichier en lecture
int test_fichier_lecture(char *filename) {
    FILE* fichier;
    fichier = fopen(filename,"r");
    if(fichier == NULL) return 1;
    fclose(fichier);
    return 0;
}

//Fonction qui teste la possibilité d'ouvrir un fichier en écriture
int test_fichier_ecriture(char *filename) {
    FILE* fichier;
    fichier = fopen(filename,"w");
    if(fichier == NULL) return 1;
    fclose(fichier);
    return 0;
}

//Fonction qui gère des erreur générales
void gestion_erreur_commande(char* chaine, int tailleChaine, int tab_cmd[NB_CMD], int tailleCmd, int* nb_erreur) {
    int cmdOK = 0;
    int erreur = 0;

    //si on a aucune case du tableau = 1 alors: mauvaise commande
    for(int i=0; i<NB_CMD; i++) {
        if(tab_cmd[i] == 1) {
            cmdOK = 1; //si = 1 : on a une commande correcte
        }
    }
    if(cmdOK == 0) {
        printf("Erreur : commande non valide\n");
        erreur++;
    }

    //tester erreur si utilisateur écrit : "man autrechose"
    if((tab_cmd[CMD_M]==1) && (tailleChaine > tailleCmd)) {
        printf("Usage : man\n");
        erreur++;
    }

    //pas besoin de tester si erreur avec quit car meme si utilisateur marque quit avec quelque chose derriere on sort de l'interprète

    (*nb_erreur) = erreur;

}


/*******FONCTIONS QUI RENVOIENT 0 SI ERREUR SINON 1*******/
int erreur_listkeys(utilisateur_t* user, char* param1, char* param2, char* param3) {
    if(strcmp(param2,"")!=0 || strcmp(param3,"")!=0) {
        printf("Usage : listkeys [<keyid>]\n");
        return 0;
    }
    if(strcmp(param1,"")!=0) {
        int keyid=atoi(param1);
        int erreur=1;
        for(int i=0; i<user->nbId; i++) {
            if((user->tabId[i].id)==keyid) {
                erreur=0;
            }
        }
        if(erreur==1) {
            printf("keyid n'est pas un identificateur correct\n");
            return 0;
        }
    }

    return 1;
}

int erreur_rmkeys(utilisateur_t* user, char* param1, char* param2, char* param3) {
    if(strcmp(param1,"")==0 || strcmp(param2,"")!=0 || strcmp(param3,"")!=0) {
        printf("Usage : rmkeys <keyid>\n");
        return 0;
    }
    int keyid=atoi(param1);
    int erreur=1;
    for(int i=0; i<user->nbId; i++) {
        if((user->tabId[i].id)==keyid) {
            erreur=0;
        }
    }
    if(erreur==1) {
        printf("keyid n'est pas un identificateur correct\n");
        return 0;
    }

    return 1;
}

int erreur_newkeys(utilisateur_t* user, char* param1, char* param2, char* param3) {
    if(strcmp(param1,"")==0 || strcmp(param2,"")==0 || strcmp(param3,"")!=0) {
        printf("Usage : newkeys <keyid> <type>\n");
        return 0;
    }
    if(strcmp(param2,"crypt")!=0 && strcmp(param2,"sign")!=0) {
        printf("Erreur type : crypt ou sign\n");
        return 0;
    }
    int keyid=atoi(param1);
    for(int i=0; i<user->nbId; i++) {
        if((user->tabId[i].id)==keyid) {
            printf("Erreur : keyid déjà existant, choisir un différent\n");
            return 0;
        }
    }
    return 1;
}

int erreur_crypt(utilisateur_t* user, char* param1, char* param2, char* param3) {
    if(strcmp(param1,"")==0 || strcmp(param2,"")==0 || strcmp(param3,"")==0) {
        printf("Usage : crypt <fileIn> <fileOut> <keyid>\n");
        return 0;
    }
    if(test_fichier_lecture(param1)==1) {
        printf("Erreur ouverture fileIn : crypt <fileIn> <fileOut> <keyid>\n");
        return 0;
    }
    if(test_fichier_ecriture(param2)==1) {
        printf("Erreur ouverture fileOut : crypt <fileIn> <fileOut> <keyid>\n");
        return 0;
    }
    int keyid=atoi(param3);
    int erreur=1;
    for(int i=0; i<user->nbId; i++) {
        if((user->tabId[i].id)==keyid) {
            erreur=0;
        }
    }
    if(erreur==1) {
        printf("keyid n'est pas un identificateur correct\n");
        return 0;
    }
    return 1;
}

int erreur_uncrypt(utilisateur_t* user, char* param1, char* param2, char* param3) {
    if(strcmp(param1,"")==0 || strcmp(param2,"")==0 || strcmp(param3,"")==0) {
        printf("Usage : uncrypt <fileIn> <fileOut> <keyid>\n");
        return 0;
    }
    if(test_fichier_lecture(param1)==1) {
        printf("Erreur ouverture fileIn : uncrypt <fileIn> <fileOut> <keyid>\n");
        return 0;
    }
    if(test_fichier_ecriture(param2)==1) {
        printf("Erreur ouverture fileOut : uncrypt <fileIn> <fileOut> <keyid>\n");
        return 0;
    }
    int keyid=atoi(param3);
    int erreur=1;
    for(int i=0; i<user->nbId; i++) {
        if((user->tabId[i].id)==keyid) {
            erreur=0;
        }
    }
    if(erreur==1) {
        printf("keyid n'est pas un identificateur correct\n");
        return 0;
    }
    return 1;
}

int erreur_save(char* param1, char* param2, char* param3) {
    if(strcmp(param2,"")!=0 || strcmp(param3,"")!=0) {
        printf("Usage : save [<fileOut>]\n");
        return 0;
    }
    if(strcmp(param1,"")!=0 && test_fichier_ecriture(param1)==1) {
        printf("Erreur ouverture fileOut : save [<fileOut>]\n");
        return 0;
    }
    return 1;
}

int erreur_savepub(utilisateur_t* user, char* param1, char* param2, char* param3) {
    if(strcmp(param1,"")==0 || strcmp(param2,"")==0 || strcmp(param3,"")!=0) {
        printf("Usage : <savepub> <keyid> <file>\n");
        return 0;
    }
    if(test_fichier_ecriture(param2)==1) {
        printf("Erreur ouverture file : savepub <keyid> <file>\n");
        return 0;
    }
    int keyid=atoi(param1);
    int erreur=1;
    for(int i=0; i<user->nbId; i++) {
        if((user->tabId[i].id)==keyid) {
            erreur=0;
        }
    }
    if(erreur==1) {
        printf("keyid n'est pas un identificateur correct\n");
        return 0;
    }
    return 1;
}

int erreur_load(char* param1, char* param2, char* param3) {
    if(strcmp(param2,"")!=0 || strcmp(param3,"")!=0) {
        printf("Usage : load [<fileIn>]\n");
        return 0;
    }
    if(strcmp(param1,"")!=0 && test_fichier_lecture(param1)==1) {
        printf("Erreur ouverture fileIn : load [<fileIn>]\n");
        return 0;
    }
    return 1;
}

int erreur_show(utilisateur_t* user, char* param1, char* param2, char* param3) {
    if(strcmp(param1,"")==0) {
        printf("Usage : show <keyid> ['pub'] ['priv']\n");
        return 0;
    }
    if(strcmp(param2,"")!=0) {
        if(strcmp(param2,"pub")!=0 && strcmp(param2,"priv")!=0) {
            printf("Usage : show <keyid> ['pub'] ['priv']\n");
            return 0;
        }
    }

    if(strcmp(param3,"")!=0){
        if (strcmp(param3,"pub")!=0 && strcmp(param3,"priv")!=0) {
            printf("Usage : show <keyid> ['pub'] ['priv']\n");
            return 0;
        }
    }

    int keyid=atoi(param1);
    int erreur=1;
    for(int i=0; i<user->nbId; i++) {
        if((user->tabId[i].id)==keyid) {
            erreur=0;
        }
    }
    if(erreur==1) {
        printf("keyid n'est pas un identificateur correct\n");
        return 0;
    }
    return 1;
}

//////////////////////////////////////////////////////////////////////////////

int erreur_listcontacts(utilisateur_t* user, char* param1, char* param2, char* param3) {
    int erreur1 = 0;
    int erreur2 = 0;
    int erreur3 = 0;

    if (strcmp(param3,"")!=0){
        printf("Usage : listcontacts [<idcontact>] [<nomfamille>]\n");
        return 0;
    }



    if(strcmp(param1,"")!=0 && strcmp(param2,"")!=0){  //si 1er ET 2eme
        for(int i=0; i<user->nbContact; i++){
            if(strcmp(param1,user->annuaire[i].identificateur)==0) {
                erreur1 = 1;
                if(strcmp(param2,user->annuaire[i].surname)!=0){
                    printf("Identificateur (%s) et nom de contact (%s) ne correspondent pas.\n",param1,param2);
                    return 0;
                }
            }

            if(strcmp(param2,user->annuaire[i].surname)==0) {
                erreur2 = 1;
                if(strcmp(param1,user->annuaire[i].identificateur)!=0) {
                    printf("Identificateur (%s) et nom de contact (%s) ne correspondent pas.\n",param1,param2);
                    return 0;
                }
            }
        }

        if(erreur1 == 0) {
            printf("L'identifiant %s inexistant dans vos contacts\n",param1);
            return 0;
        }
        if(erreur2 == 0) {
            printf("Le nom de famille %s inexistant dans vos contacts.\n",param2);
            return 0;
        }

    }

    if(strcmp(param1,"")!=0 && strcmp(param2,"")==0) {
        for(int i=0; i<user->nbContact; i++) {
            if(strcmp(param1,user->annuaire[i].identificateur)==0) {
                erreur3 = 1;
            }
            else if(strcmp(param1,user->annuaire[i].surname)==0) {
                erreur3 = 1;
            }
        }

        if(erreur3 == 0) {
            printf("%s n'est pas un nom de famille ou identificateur existant.\n",param1);
            return 0;
        }
    }


    return 1;
}


int erreur_addcontact(utilisateur_t* user, char* param1, char* param2, char* param3) {
    if(strcmp(param1,"")==0 || strcmp(param2,"")!=0 || strcmp(param3,"")!=0) {
        printf("Usage : addcontact <id>\n");
        return 0;
    }

    else {
        for(int i=0; i<user->nbContact; i++) {
            if(strcmp(param1,user->annuaire[i].identificateur)==0) {
                printf("L'identificateur de contact %s existe déjà, il est donc impossible de le rajouter.\n",param1);
                return 0;
            }
        }
    }

    return 1;
}

//A FAIRE
int erreur_modifycontact(utilisateur_t* user, char* param1, char* param2, char* param3) {
  if (!strcmp(param1,"") || strcmp(param2,"") || strcmp(param3,"")){
    printf("Usage : modifycontact <id>\n");
    return 0;
  }
  for (int i=0; i<user->nbContact; ++i){
    if (!strcmp(param1, user->annuaire[i].identificateur)){
      return 1;
    }
  }
  printf("L'identificateur de contact %s n'existe pas, il est donc impossible de le modifier.\n",param1);
  return 0;
}

//A FAIRE
int erreur_addkeys(utilisateur_t* user, char* param1, char* param2, char* param3) {
  if (!strcmp(param1,"") || strcmp(param2,"") || strcmp(param3,"")){
    printf("Usage : addkeys <id> ou <nom>\n");
    return 0;
  }
  for (int i=0; i<user->nbContact; ++i){
    if (!strcmp(param1, user->annuaire[i].identificateur) || !strcmp(param1, user->annuaire[i].surname)){
      return 1;
    }
  }
  printf("L'identificateur/nom du contact %s n'existe pas, il est donc impossible d'y ajouter une clé.\n",param1);
  return 0;
}

//A FAIRE
int erreur_rmcontact(utilisateur_t* user, char* param1, char* param2, char* param3) {
  if (!strcmp(param1,"") || strcmp(param2,"") || strcmp(param3,"")){
    printf("Usage : rmcontact <id>\n");
    return 0;
  }
  for (int i=0; i<user->nbContact; ++i){
    if (!strcmp(param1, user->annuaire[i].identificateur)){
      return 1;
    }
  }
  printf("L'identificateur de contact %s n'existe pas, il est donc impossible de le supprimer.\n",param1);
  return 0;
}
/*
-- modifycontact <id> : affiche des informations et un menu de modification.
— addkeys <id> ou <nom> : ajoute une clef à un contact
— rmcontact <id> : supprime le contact et toutes ses clefs.
*/
