#include "rsa_header.h"

void listcontacts(utilisateur_t* user, char* param1, char* param2) {
    if(user->nbContact==0) printf("Aucun contact enregistré.\n");

    if(strcmp(param1,"")==0) {
        for(int i=0; i<user->nbContact; i++) {
            printf("- Contact : %s,",user->annuaire[i].identificateur);
            printf(" %s %s\n",user->annuaire[i].name,user->annuaire[i].surname);

            for(int j=0; j<user->annuaire[i].tailleId; j++) {
                printf("\t%d- Identificateur : [%d]\tType : %s\n",j+1,user->annuaire[i].listeId[j].id,user->annuaire[i].listeId[j].type);
            }
        }
    }

    else {
        for(int i=0; i<user->nbContact; i++) {
            if((strcmp(param1,user->annuaire[i].identificateur)==0) || (strcmp(param1,user->annuaire[i].surname)==0)) {
                printf("- Contact : %s,",user->annuaire[i].identificateur);
                printf(" %s %s\n",user->annuaire[i].name,user->annuaire[i].surname);

                for(int j=0; j<user->annuaire[i].tailleId; j++) {
                    printf("\t%d- Identificateur : [%d]\tType : %s\n",j+1,user->annuaire[i].listeId[j].id,user->annuaire[i].listeId[j].type);
                }

            }
        }
    }
}


void addcontact(utilisateur_t* user, char* param1) {
    char nom[NAME_MAX_SIZE]="";
    char prenom[NAME_MAX_SIZE]="";
    char comm[COMMENTAIRE_SIZE]="";
    int i=0;

    ++(user->nbContact);
    user->annuaire = (contact_t*)realloc(user->annuaire,(user->nbContact)*sizeof(contact_t));

    user->annuaire[user->nbContact-1].tailleId = 0;

    while(param1[i]!='\0') {
        user->annuaire[user->nbContact-1].identificateur[i] = param1[i];
        i++;
    }
    user->annuaire[user->nbContact-1].identificateur[i] = '\0';
    i=0;
    printf("Vous allez remplir les informations sur votre contact ci-dessous :\n");
    printf("Entrez le prénom de votre contact : ");
    fgets(prenom,NAME_MAX_SIZE,stdin);

    while(prenom[i]!='\n') {
        user->annuaire[user->nbContact-1].name[i] = prenom[i];
        i++;
    }
    user->annuaire[user->nbContact-1].name[i] = '\0';
    i=0;

    printf("Entrez le nom de famille de votre contact : ");
    fgets(nom,NAME_MAX_SIZE,stdin);

    while(nom[i]!='\n') {
        user->annuaire[user->nbContact-1].surname[i] = nom[i];
        i++;
    }
    user->annuaire[user->nbContact-1].surname[i] = '\0';
    i=0;

    printf("Entrez un commentaire : ");
    fgets(comm,COMMENTAIRE_SIZE,stdin);

    while(comm[i]!='\n') {
        user->annuaire[user->nbContact-1].commentaire[i] = comm[i];
        i++;
    }
    user->annuaire[user->nbContact-1].commentaire[i] = '\0';
}


//A FAIRE
void modifycontact(utilisateur_t* user, char* param1) {
    int x=0;
    char option[3]="";
    char identificateur[NAME_MAX_SIZE]="";
    char nom[NAME_MAX_SIZE]="";
    char prenom[NAME_MAX_SIZE]="";
    char commentaire[COMMENTAIRE_SIZE]="";

    for (int i=0; i<user->nbContact; ++i){
        if (!strcmp(param1, user->annuaire[i].identificateur)){
            x=i;
        }
    }

    printf(" %s %s\n",user->annuaire[x].name,user->annuaire[x].surname);
    printf("Commentaire sur le contact [%s] : %s.\n", param1, user->annuaire[x].commentaire);

    for(int j=0; j<user->annuaire[x].tailleId; j++) {
        printf("\t%d- Identificateur : [%d]\tType : %s\n",j+1,user->annuaire[x].listeId[j].id,user->annuaire[x].listeId[j].type);
        printKeyPair(user->annuaire[x].listeId[j].pairKey);
    }


    printf("\nVous allez modifier les informations sur votre contact d'indentificateur [%s].\nPour afficher le menu d'informations, tapez 5.\n", user->annuaire[x].identificateur);
    int i=0;
    fgets(option,3,stdin);

    while (strcmp(option,"7\n")){
      if (i!=0){
        printf("\n\nVeuillez saisir à nouveau l'élément du contact [%s] à modifier.\nTaper 5 pour afficher les commandes.\n",user->annuaire[x].identificateur);
        fgets(option,3,stdin);
        //scanf("%d",&y);
      }
      i=0;
        if (!strcmp(option,"1\n")){
            printf("Veuillez saisir le nouvel identificateur du contact à modifier : ");
            fgets(identificateur,NAME_MAX_SIZE,stdin);

            while(identificateur[i]!='\n') {
                user->annuaire[x].identificateur[i] = identificateur[i];
                i++;
              }
              user->annuaire[x].identificateur[i] = '\0';
              printf("Identificateur du contact modifié : %s.\n",user->annuaire[x].identificateur);
        }
        else if (!strcmp(option,"2\n")){
            printf("Veuillez saisir le nouveau nom de famille du contact [%s] à modifier :\t",user->annuaire[x].identificateur);
            fgets(nom,NAME_MAX_SIZE,stdin);

            while(nom[i]!='\n') {
                user->annuaire[x].surname[i] = nom[i];
                i++;
            }
            user->annuaire[x].surname[i] = '\0';
            printf("Nom du contact [%s] modifié : %s.\n", user->annuaire[x].identificateur, user->annuaire[x].surname);
        }
        else if (!strcmp(option,"3\n")){
            printf("Veuillez saisir le nouveau prénom du contact [%s] à modifier :\t",user->annuaire[x].identificateur);
            fgets(prenom,NAME_MAX_SIZE,stdin);

            while(prenom[i]!='\n') {
                user->annuaire[x].name[i] = prenom[i];
                i++;
            }
            user->annuaire[x].name[i] = '\0';
            printf("Prénom du contact [%s] modifié : %s.\n", user->annuaire[x].identificateur, user->annuaire[x].name);
        }
        else if (!strcmp(option,"4\n")){
            printf("Veuillez saisir le nouveau commentaire du contact [%s] :\n",user->annuaire[x].identificateur);
            fgets(commentaire,COMMENTAIRE_SIZE,stdin);

            while(commentaire[i]!='\n') {
                user->annuaire[x].commentaire[i] = commentaire[i];
                i++;
            }
            user->annuaire[x].commentaire[i] = '\0';
            printf("Commentaire du contact [%s] modifié : %s.\n", user->annuaire[x].identificateur, user->annuaire[x].commentaire);
        }
        else if (!strcmp(option,"5\n")){
            printf("\nPour modifier :\n- l'indentificateur, tapez 1;\n- le nom, tapez 2;\n- le prénom, tapez 3;\n- le commentaire, tapez 4;\n- afficher le menu d'informations, tapez 5;\n- afficher les données du contact [%s], tapez 6;\n- quitter la fonction 'modifycontact <idcontact>', tapez 7.\n\n", param1);
            fgets(option,3,stdin);
        }

        else if (!strcmp(option,"6\n")){
            printf(" %s %s\n",user->annuaire[x].name,user->annuaire[x].surname);
            printf("Commentaire sur le contact [%s] : %s.\n", param1, user->annuaire[x].commentaire);

            for(int j=0; j<user->annuaire[x].tailleId; j++) {
                printf("\t%d- Identificateur : [%d]\tType : %s\n",j+1,user->annuaire[x].listeId[j].id,user->annuaire[x].listeId[j].type);
                printKeyPair(user->annuaire[x].listeId[j].pairKey);
            }
            printf("\n\nVeuillez saisir à nouveau l'élément du contact [%s] à modifier.\nTaper 5 pour afficher les commandes.\n",user->annuaire[x].identificateur);
            fgets(option,3,stdin);
        }

        else if (!strcmp(option,"7\n")){
            printf("fin\n");
        }

        else{
            printf("L'interprète de commande n'a pas saisi votre réponse.\n\n\n");
            fgets(option,3,stdin);
        }
    }
    printf("\n\nSortie de la fonction modifycontact\n");
}

//A FAIRE
//— addkeys <id> ou <nom> : ajoute une clef à un contact
void addkeys(utilisateur_t* user, char* param1) {
    int x;
    char type[NAME_MAX_SIZE]="";
    rsaKey_t pubKey;
    rsaKey_t privKey;
    genKeysRabin(&pubKey,&privKey);

    for (int i=0; i<user->nbContact; ++i){
        if (!strcmp(param1,user->annuaire[i].identificateur) || !strcmp(param1,user->annuaire[i].surname)){
            x=i;
        }
    }

    ++(user->annuaire[x].tailleId);
    user->annuaire[x].listeId = (identificateur_t*)realloc(user->annuaire[x].listeId,(user->annuaire[x].tailleId)*sizeof(identificateur_t));

    user->annuaire[x].listeId[user->annuaire[x].tailleId-1].pairKey.pubKey=pubKey;
    user->annuaire[x].listeId[user->annuaire[x].tailleId-1].pairKey.privKey=privKey;
    char idInit[10];
    int id;   //    fgets(type,NAME_MAX_SIZE,stdin);

    printf("\nVeuillez saisir l'identifiant de la nouvelle clé du contact %s (celui-ci doit être un entier) : ", param1);
    fgets(idInit,10,stdin);
    id=atoi(idInit);
etiq1:    for (int i=0; i<user->annuaire[x].tailleId; ++i){
              while (id==user->annuaire[x].listeId[i].id){
                  printf("L'identificateur de clé [%d] existe déjà pour ce contact, veuillez en entrer un autre : ",id);
                  fgets(idInit,10,stdin);
                  id=atoi(idInit);
                  goto etiq1;
              }
            }
    user->annuaire[x].listeId[user->annuaire[x].tailleId-1].id=id;

    printf("\nVeuillez à présent saisir le type de la nouvelle clé du contact %s : ", param1);
    int i=0;
    fgets(type,NAME_MAX_SIZE,stdin);

    while(strcmp(type, "sign\n") && strcmp(type,"crypt\n")){
        fgets(type,NAME_MAX_SIZE,stdin);
        if (i!=0){
          printf("Le type de la clé n'étant pas reconnu par l'interprete, veuillez le resaisir : ");
        }
        i++;
    }

    i=0;
    while(type[i]!='\n') {
        user->annuaire[x].listeId[user->annuaire[x].tailleId-1].type[i] = type[i];
        i++;
    }
    user->annuaire[x].listeId[user->annuaire[x].tailleId-1].type[i] = '\0';

    printf("\nLa clé [%d] de type %s a bien été ajoutée au contact d'identificateur [%s].\n",id,user->annuaire[x].listeId[user->annuaire[x].tailleId-1].type, user->annuaire[x].identificateur);
    printf("Le contact [%s] a à présent %d paire(s) de clés.\n",param1,user->annuaire[x].tailleId);
}

//A FAIRE
//— rmcontact <id> : supprime le contact et toutes ses clefs.

void rmcontact(utilisateur_t* user, char* param1) {
  int x;
  for (int i=0; i<user->nbContact; ++i){
      if (!strcmp(param1,user->annuaire[i].identificateur)){
          x=i;
      }
  }

  for (int i=x; i<user->nbContact-1; ++i){
      for (int j=0; j<NAME_MAX_SIZE;++j){
          user->annuaire[i].identificateur[j]=user->annuaire[i+1].identificateur[j];
      }
      for (int j=0; j<NAME_MAX_SIZE;++j){
          user->annuaire[i].name[j]=user->annuaire[i+1].name[j];
      }
      for (int j=0; j<NAME_MAX_SIZE;++j){
          user->annuaire[i].surname[j]=user->annuaire[i+1].surname[j];
      }
      for (int j=0; j<COMMENTAIRE_SIZE;++j){
          user->annuaire[i].commentaire[j]=user->annuaire[i+1].commentaire[j];
      }


      for (int j=0; j<user->annuaire[i+1].tailleId; ++j){
            user->annuaire[i].listeId[j].id=user->annuaire[i+1].listeId[j].id;

            user->annuaire[i].listeId[j].pairKey.pubKey.E=user->annuaire[i+1].listeId[j].pairKey.pubKey.E;
            user->annuaire[i].listeId[j].pairKey.pubKey.N=user->annuaire[i+1].listeId[j].pairKey.pubKey.N;
            user->annuaire[i].listeId[j].pairKey.privKey.E=user->annuaire[i+1].listeId[j].pairKey.privKey.E;
            user->annuaire[i].listeId[j].pairKey.privKey.N=user->annuaire[i+1].listeId[j].pairKey.privKey.N;

            if (!strcmp(user->annuaire[i+1].listeId[j].type,"crypt")){
                user->annuaire[i].listeId[j].type[0]='c';
                user->annuaire[i].listeId[j].type[1]='r';
                user->annuaire[i].listeId[j].type[2]='y';
                user->annuaire[i].listeId[j].type[3]='p';
                user->annuaire[i].listeId[j].type[4]='t';
            }
            else if (!strcmp(user->annuaire[i+1].listeId[j].type,"sign")){
                user->annuaire[i].listeId[j].type[0]='s';
                user->annuaire[i].listeId[j].type[1]='i';
                user->annuaire[i].listeId[j].type[2]='g';
                user->annuaire[i].listeId[j].type[3]='n';
                user->annuaire[i].listeId[j].type[4]='\0';
            }
      }
      user->annuaire[i].tailleId=user->annuaire[i+1].tailleId;
  }
  user->nbContact--;
  user->annuaire[user->nbContact].identificateur[0]='\0';
  user->annuaire[user->nbContact].name[0]='\0';
  user->annuaire[user->nbContact].surname[0]='\0';
  user->annuaire[user->nbContact].commentaire[0]='\0';

  printf("Le contact d'indentifiant [%s] ainsi que toutes ses clés ont été supprimées.\n", param1);
  printf("Il rete à présent %d contacts dans votre annuaire.\n",user->nbContact);
}
