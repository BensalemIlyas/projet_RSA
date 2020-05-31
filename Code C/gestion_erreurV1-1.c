#include "rsa_headerV1-1.h"

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


int erreur_listcontacts(utilisateur_t* user, char* param1, char* param2, char* param3){
  if (strcmp(param3,"")){ //si 3eme parametre
    printf("Usage : listcontacts [<idcontact>] [<nom>]\n");
    return 0;
  }

  else if (strcmp(param1,"") && strcmp(param2,"")){  //si 1er et 2eme
    int idContact=atoi(param1);
    for (int i=0; i<user->nbContacts; ++i){
      if (idContact==user->contacts[i].idContact){
        if (!strcmp(param2,user->contacts[i].nameContact)){
          return 0;
        }
        else{
          printf("L'identifiant contact [%d] et le nom de contact %s ne correspondent pas.\n",idContact,user->contacts[i].nameContact);
          return 0;
        }
      }
    }
    printf("%s n'est pas un identificateur de contact valide\n", param1);
    return 0;
  }

  else if(strcmp(param1,"") && !strcmp(param2,"") && !strcmp(param1,"")){ //si 1 seul parmetre
    int idContact=atoi(param1);
    for (int i=0; i<user->nbContacts; ++i){
      if (idContact==user->contacts[i].idContact || !strcmp(param1,user->contacts[i].nameContact)){
        return 1;
      }
    }
    printf("[%s] n'est pas un paramètre enregitré dans les contacts\n", param1);
    return 0;
  }
  return 1;
}

int erreur_addcontact(utilisateur_t* user, char* param1, char* param2, char* param3){
  if (strcmp(param2,"") || strcmp(param3,"")){
    printf("Usage : addcontact <id>\n");
    return 0;
  }
  else if (!strcmp(param1,"")){
    printf("Usage : addcontact <id>\n");
    return 0;
  }

  else{
    int nbCont=atoi(param1);
    for (int i=0; i<user->nbContacts; ++i){
      if (nbCont==user->contacts[i].idContact){
        printf("L'indentificateur de contact [%d] existe déjà, il est donc impossible de le rajouter.\n",nbCont);
        return 0;
      }
    }
  }
  return 1;
}
