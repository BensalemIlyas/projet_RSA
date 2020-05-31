#include "rsa_headerV1-1.h"

//Fonction qui initialise les structures
void init_user(utilisateur_t* user) {
    char prenom[NAME_MAX_SIZE] = "";    //name
    char nom[NAME_MAX_SIZE] = "";   //surname
    user->nbContacts = 1; //J'ai mis 1 pour voir si ça changeait qqc
    user->nbId=1;  //J'ai mis 1 pour voir si ça changeait qqc
    user->tabId=NULL;
    user->contacts = NULL;

    int i=0;
    printf("Entrez votre prénom : ");
    fgets(prenom,NAME_MAX_SIZE,stdin);
    while(prenom[i]!='\n') {
        user->name[i] = prenom[i];
        i++;
    }
    user->name[i] = '\0';
    i=0;
    printf("Entrez votre nom de famille : ");
    fgets(nom,NAME_MAX_SIZE,stdin);
    while(nom[i]!='\n') {
        user->surname[i] = nom[i];
        i++;
    }
    user->surname[i] = '\0';
}

//Fonction qui retourne la taille de la nouvelle chaine-->commande_cut qui contient si non erreur la commande, seulement la première partie, ex: crypt
int couper_chaine(char* chaine, char* commande_cut) {
    printf("Attention 2\n");
    int tailleC;
    int tailleChaine = strlen(chaine);
    int i=0;
    printf("\n Attention 3\n" );
    while(i<tailleChaine && chaine[i]!=' ' && chaine[i]!='\n') {
        commande_cut[i] = chaine[i];
        i++;
    }
    commande_cut[i] = '\0';
    tailleC = i;
    return tailleC;
}

//Teste la première partie de la commande marquée par utilisateur, ex: save
void analyse_commandes(char *commande_cut, int tab_cmd[NB_CMD]) {
    if(strcmp(commande_cut,"man")==0) tab_cmd[CMD_M]=1;
    else if(strcmp(commande_cut,"listkeys")==0) tab_cmd[CMD_LK]=1;
    else if(strcmp(commande_cut,"rmkeys")==0) tab_cmd[CMD_RM]=1;
    else if(strcmp(commande_cut,"newkeys")==0) tab_cmd[CMD_NK]=1;
    else if(strcmp(commande_cut,"crypt")==0) tab_cmd[CMD_C]=1;
    else if(strcmp(commande_cut,"uncrypt")==0) tab_cmd[CMD_U]=1;
    else if(strcmp(commande_cut,"save")==0) tab_cmd[CMD_S]=1;
    else if(strcmp(commande_cut,"savepub")==0) tab_cmd[CMD_SP]=1;
    else if(strcmp(commande_cut,"load")==0) tab_cmd[CMD_L]=1;
    else if(strcmp(commande_cut,"show")==0) tab_cmd[CMD_SH]=1;
    else if(strcmp(commande_cut,"quit")==0) tab_cmd[CMD_Q]=1;
    else if(strcmp(commande_cut,"listcontacts")==0) tab_cmd[CMD_LC]=1;
    else if(strcmp(commande_cut,"addcontact")==0) tab_cmd[CMD_AC]=1;
}


//Fonction qui stocke les paramètres de la commande tapée par utilisateur
void parametres(char* chaine, int tailleChaine, int tailleCmd, char* param1, char* param2, char* param3) {
    param1[0] = '\0';
    param2[0] = '\0';
    param3[0] = '\0';
    int i=tailleCmd;  //i = taille commande déjà stockée dans 1 variable
    int compteur=0;

    if(chaine[i] == ' ') {
        i++;
        while(i<tailleChaine && chaine[i]!=' ' && chaine[i]!='\n') {
            param1[compteur] = chaine[i];
            i++;
            compteur++;
        }
        param1[compteur] = '\0'; //si on ne fait pas ça, pb avec taille de param1
    }
//     printf("param1 : %s\n", param1);

    if(chaine[i] == ' ') {
        i++;
        compteur=0;
        while(i<tailleChaine && chaine[i]!=' ' && chaine[i]!='\n') {
            param2[compteur] = chaine[i];
            i++;
            compteur++;
        }
        param2[compteur] = '\0';
    }
//     printf("param2 : %s\n", param2);

    if(chaine[i] == ' ') {
        i++;
        compteur=0;
        while(i<tailleChaine && chaine[i]!=' ' && chaine[i]!='\n') {
            param3[compteur] = chaine[i];
            i++;
            compteur++;
        }
        param3[compteur] = '\0';
    }
//     printf("param3 : %s\n", param3);

}

/********************FONCTIONS POUR CHAQUE COMMANDE********************/

//Fonction qui permet d'ouvrir le fichier avec les commandes de l'interprète
void man() {
    FILE *fichier;
    char ligne[MAXBUF];
    fichier = fopen("fichierCmd.txt","r");

//     if(fichier == NULL) erreur("\nOuverture fichierCmd.txt impossible.\n");
    if(fichier == NULL) {
        printf("\nOuverture fichierCmd.txt impossible.\n");
        exit(1);
    }
    while(fgets(ligne,MAXBUF,fichier)!=NULL) printf("%s",ligne);

    fclose(fichier);
}


void listkeys(utilisateur_t* user, char* param1) {
    int numId=0;

    //si aucun identificateur
    if (user->nbId==0){
        printf("Aucun identificateur pour le moment.\n");
    }

    //si un identificateur précisé
    else if(strcmp(param1,"")!=0) {
        numId = atoi(param1);
        printf("%d identificateur(s) enregistré(s)\n",user->nbId);
        for (int i=0; i<user->nbId; i++){
            if(numId==user->tabId[i].id){
                printf("\n- Numéro d'identificateur : [%d] \t",user->tabId[i].id);
                printf("Type de la clé : %s\n",user->tabId[i].type);
            }
        }
    }

    //si aucun identificateur n'est précisé
    else {
        printf("Pas d'identificateur renseigné en particulier.\n");
        printf("Voici les informations sur tous les identificateurs.\n");
        printf("%d identificateur(s) enregistré(s)\n\n",user->nbId);
        for (int i=0; i<user->nbId; i++){
            printf("- Numéro d'identificateur : [%d] \t",user->tabId[i].id);
            printf("Type de clé : %s\n",user->tabId[i].type);
        }
    }
}


void rmkeys(utilisateur_t* user, char* param1) {
    int numId = atoi(param1);
    int tailleId = user->nbId;
    int limite;

    for (int i=0; i<tailleId;++i){
        if (numId==user->tabId[i].id){
            user->tabId[i].pairKey.pubKey.E=0;
            user->tabId[i].pairKey.pubKey.N=0;

            user->tabId[i].pairKey.privKey.E=0;
            user->tabId[i].pairKey.privKey.N=0;

            user->tabId[i].id=0;
            user->tabId[i].type[0]='\0';

            limite=i;
        }
    }
    for (int i=limite; i<(tailleId-1);++i){
        user->tabId[i].id = user->tabId[i+1].id;
        user->tabId[i].pairKey.pubKey.E = user->tabId[i+1].pairKey.pubKey.E;
        user->tabId[i].pairKey.pubKey.N = user->tabId[i+1].pairKey.pubKey.N;
        user->tabId[i].pairKey.privKey.E = user->tabId[i+1].pairKey.privKey.E;
        user->tabId[i].pairKey.privKey.N = user->tabId[i+1].pairKey.privKey.N;

        if (!strcmp(user->tabId[i+1].type,"crypt")){
            user->tabId[i].type[0]='c';
            user->tabId[i].type[1]='r';
            user->tabId[i].type[2]='y';
            user->tabId[i].type[3]='p';
            user->tabId[i].type[4]='t';
        }
        else if (!strcmp(user->tabId[i+1].type,"sign")){
            user->tabId[i].type[0]='s';
            user->tabId[i].type[1]='i';
            user->tabId[i].type[2]='g';
            user->tabId[i].type[3]='n';
            user->tabId[i].type[4]='\0';
        }
    }
  (user->nbId)--;
  printf("L'identificateur numéro %d a bien été supprimé.\n",numId);
  printf("Il reste %d identificateur.\n",user->nbId);
}


void newkeys(utilisateur_t* user, char* param1, char* param2) {
    int numId = atoi(param1);
    rsaKey_t pubKey;
    rsaKey_t privKey;

    genKeysRabin(&pubKey,&privKey);
    ++(user->nbId);
    user->tabId = (identificateur_t*)realloc(user->tabId,(user->nbId)*sizeof(identificateur_t));

    user->tabId[user->nbId-1].id=numId;
    user->tabId[user->nbId-1].pairKey.pubKey=pubKey;
    user->tabId[user->nbId-1].pairKey.privKey=privKey;

    for (int i=0; i<sizeof(param2); i++){
        user->tabId[user->nbId-1].type[i]= param2[i];
    }

    printf("Identificateur numéro [%d] créé.\n",numId);

}


void crypt(utilisateur_t* user, char* param1, char* param2, char* param3) {
    int numId = atoi(param3);
    rsaKey_t clePublique;
    int nbCar;

    for(int i=0; i<(user->nbId); ++i){
      if(numId==user->tabId[i].id){
        clePublique = user->tabId[i].pairKey.pubKey;
        RSAcryptFile(param1,param2,clePublique,&nbCar);
        printf("%s est maintenant crypté dans %s\n",param1,param2);
        i = user->nbId;
      }
    }
}

void uncrypt(utilisateur_t* user, char* param1, char* param2, char* param3) {
    int numId = atoi(param3);
    rsaKey_t clePrivee;
    int nbCar=12;

    for(int i=0; i<(user->nbId); ++i){
      if (numId==user->tabId[i].id){
        clePrivee=user->tabId[i].pairKey.privKey;
        RSAunCryptFile(param1,param2, clePrivee, nbCar);

        printf("%s est maintenant décrypté dans %s\n",param1,param2);
        i=user->nbId;
      }
    }

    printf("ATTENTION : si après ouverture de %s, c'est illisible/non décrypté, vous n'avez sûrement pas utiliser le bon identificateur.\n", param2);

}


void save(utilisateur_t* user, char* param1) {
    FILE* fichierOut;
    char decisionCryptage[5]="";

    if(strcmp(param1,"")!=0) {
        fichierOut = fopen(param1,"w");
    }
    else {
        fichierOut = fopen("FILEDEF.TXT","w");
    }

    fprintf(fichierOut,"Nom de l'utilisateur : %s\n",user->name);
    fprintf(fichierOut,"Prénom de l'utilisateur : %s\n",user->surname);

    for (int i=0; i<(user->nbId);++i){
        fprintf(fichierOut,"\nNuméro d'identificateur : [%d]\n",user->tabId[i].id);
        fprintf(fichierOut,"\tType : %s\n",user->tabId[i].type);
        fprintf(fichierOut,"\tClé publique = (%lu,%lu)\n",user->tabId[i].pairKey.pubKey.E,user->tabId[i].pairKey.pubKey.N);
        fprintf(fichierOut,"\tClé privée = (%lu,%lu)\n",user->tabId[i].pairKey.privKey.E,user->tabId[i].pairKey.privKey.N);
    }

    printf("Voulez-vous chiffrer ce fichier de sauvegarde? [y/n] : ");

    fgets(decisionCryptage,5,stdin);
    while(strcmp(decisionCryptage,"y\n")!=0 && strcmp(decisionCryptage,"n\n")!=0){
        printf("\nVotre réponse n'a pas été comprise par l'interprete.\n");
        printf("Tapez y pour yes, n pour no : ");
        fgets(decisionCryptage,5,stdin);
    }

    if(strcmp(decisionCryptage,"n\n")==0) {
        fclose(fichierOut);
        if(strcmp(param1,"")!=0) {
            printf("%s est le fichier de sauvegarde.\n",param1);
        }
        else {
            printf("FILEDEF.TXT est le fichier de sauvegarde.\n");
        }

    }

    else if(strcmp(decisionCryptage,"y\n")==0){
        rsaKey_t clePublique;
        int nbCaract;
        char* fileIn;
        char* fileOut = "FILEDEFCRYPT.TXT";
        char numeroIdDefaut[10]="";
        int numIdDef;
        int condition1 = 0;
        int condition2 = 0;

        printf("Entrez un numéro d'identificateur pour ce cryptage de fichier : ");
        fgets(numeroIdDefaut,10,stdin);

        while(condition1==0) {
            numIdDef = atoi(numeroIdDefaut);
            condition2=0;
            for(int i=0; i<user->nbId; i++) {
                if(numIdDef == user->tabId[i].id) {
                    printf("Ce numéro existe déjà, entrez en un différent : ");
                    fgets(numeroIdDefaut,10,stdin);
                    condition2 = 1;
                    i = user->nbId;
                }
            }

            if(condition2 == 0) condition1=1;
        }

        newkeys(user,numeroIdDefaut,"crypt");

        int nombreId = user->nbId;
        clePublique = user->tabId[nombreId-1].pairKey.pubKey;

        fprintf(fichierOut,"\nNuméro d'identificateur : [%d]\n",numIdDef);
        fprintf(fichierOut,"\tType : crypt\n");
        fprintf(fichierOut,"\tClé publique = (%lu,%lu)\n",user->tabId[nombreId-1].pairKey.pubKey.E,user->tabId[nombreId-1].pairKey.pubKey.N);
        fprintf(fichierOut,"\tClé privée = (%lu,%lu)\n",user->tabId[nombreId-1].pairKey.privKey.E,user->tabId[nombreId-1].pairKey.privKey.N);

        fclose(fichierOut);

        if(strcmp(param1,"")!=0) {
            fileIn = param1;
        }
        else {
            fileIn = "FILEDEF.TXT";
        }

        RSAcryptFile(fileIn,fileOut,clePublique,&nbCaract);
        printf("FILEDEFCRYPT.TXT est le fichier de sauvegarde crypté.\n");
    }



}


void savepub(utilisateur_t* user, char* param1, char* param2) {
    FILE* fichierOut = fopen(param2,"w");
    int numId = atoi(param1);
    rsaKey_t keyPublique;
    int output_length1 = 0;
    int output_length2 = 0;
    int monNb;


    for (int i=0; i<user->nbId; ++i){
        if (numId==user->tabId[i].id){
            monNb=i;
        }
    }

    keyPublique = user->tabId[monNb].pairKey.pubKey;
    uint64 clePubE = user->tabId[monNb].pairKey.pubKey.E;
    uint64 clePubN = user->tabId[monNb].pairKey.pubKey.N;

    uchar* cleCryptE = base64_encode(&clePubE, sizeof(uint64), &output_length1);
    uchar* cleCryptN = base64_encode(&clePubN, sizeof(uint64), &output_length2);

    fwrite(cleCryptE,1,output_length1,fichierOut);
    fwrite(cleCryptN,1,output_length2,fichierOut);

    printf("Le fichier %s contient la clé publique [%d].\n",param2,numId);
    printf("Cette clé (decomposée E puis N) encodée en base 64.\n");

    free(cleCryptE);
    free(cleCryptN);
    fclose(fichierOut);
}


void load(utilisateur_t* user, char* param1) {
    FILE* fichierIn;
    char decisionFichierIn[5]="";
    char ligne[MAXBUF];


    if(strcmp(param1,"")!=0) fichierIn = fopen(param1,"r");
    else {
        printf("De quel fichier voulez-vous afficher le contenu ? [1/2]\n");
        printf("\t1 : FILEDEF.TXT\n");
        printf("\t2 : FILEDEFCRYPT.TXT\n");
        fgets(decisionFichierIn,5,stdin);

        while(strcmp(decisionFichierIn,"1\n")!=0 && strcmp(decisionFichierIn,"2\n")!=0){
            printf("\nVotre réponse n'a pas été comprise par l'interprete.\n");
            printf("Tapez 1 ou 2 : ");
            fgets(decisionFichierIn,5,stdin);
        }

        if(strcmp(decisionFichierIn,"1\n")==0) fichierIn = fopen("FILEDEF.TXT","r");
        else {
            fichierIn = fopen("FILEDEFCRYPT.TXT","r");
        }
    }

    printf("\n");
    while(fgets(ligne,MAXBUF,fichierIn)!=NULL) {
        printf("%s",ligne);
    }
    printf("\n");

    fclose(fichierIn);

}



void show(utilisateur_t* user, char* param1, char* param2, char* param3) {
    int numId = atoi(param1);
    int taillePubE = 0;
    int taillePubN = 0;
    int taillePrivE = 0;
    int taillePrivN = 0;
    int monNb;
    int variablePub=1;
    int variablePriv=1;

    if((strcmp(param2,"pub")==0) || (strcmp(param3,"pub")==0)) {
        variablePub=0;
    }
    if((strcmp(param2,"priv")==0) || (strcmp(param3,"priv")==0)) {
        variablePriv=0;
    }

    for (int i=0; i<user->nbId; ++i){
        if (numId==user->tabId[i].id) monNb=i;
    }

    uint64 clePubE = user->tabId[monNb].pairKey.pubKey.E;
    uint64 clePubN = user->tabId[monNb].pairKey.pubKey.N;
    uint64 clePrivE = user->tabId[monNb].pairKey.privKey.E;
    uint64 clePrivN = user->tabId[monNb].pairKey.privKey.N;

    uchar* clePubCryptE = base64_encode(&clePubE, sizeof(uint64), &taillePubE);
    uchar* clePubCryptN = base64_encode(&clePubN, sizeof(uint64), &taillePubN);

    uchar* clePrivCryptE = base64_encode(&clePrivE, sizeof(uint64), &taillePrivE);
    uchar* clePrivCryptN = base64_encode(&clePrivN, sizeof(uint64), &taillePrivN);

    if((variablePub==0 && variablePriv==0) || (variablePub==1 && variablePriv==1)){
        printf("\nClé publique en base 64 : (");
        for(int i=0; i<taillePubE; i++) {
            printf("%c",clePubCryptE[i]);
        }
        printf(",");
        for(int i=0; i<taillePubN; i++) {
            printf("%c",clePubCryptN[i]);
        }
        printf(")\nClé privée en base 64 : (");
        for(int i=0; i<taillePrivE; i++) {
            printf("%c",clePrivCryptE[i]);
        }
        printf(",");
        for(int i=0; i<taillePrivN; i++) {
            printf("%c",clePrivCryptN[i]);
        }
        printf(")\n");
    }

    if(variablePub==0 && variablePriv==1) {
        printf("\nClé publique en base 64 : (");
        for(int i=0; i<taillePubE; i++) {
            printf("%c",clePubCryptE[i]);
        }
        printf(",");
        for(int i=0; i<taillePubN; i++) {
            printf("%c",clePubCryptN[i]);
        }
        printf(")\n");
    }

    if(variablePub==1 && variablePriv==0) {
        printf("\nClé privée en base 64 : (");
        for(int i=0; i<taillePrivE; i++) {
            printf("%c",clePrivCryptE[i]);
        }
        printf(",");
        for(int i=0; i<taillePrivN; i++) {
            printf("%c",clePrivCryptN[i]);
        }
        printf(")\n");
    }
}

void listcontacts(utilisateur_t* user, char* param1, char* param2){
  if (user->nbContacts==0){
    printf("Aucun contact enregistré.\n");
  }
  if (!strcmp(param1,"")){
    for (int i=0; i<user->nbContacts; ++i){
      printf("- Contats enregistré : %s\tidentifiant contact : [%d]. \n", user->contacts[i].nameContact, user->contacts[i].idContact);
      for (int j=0; j<user->contacts[i].nbId; ++j){
        printf("\t+ %d Clé enregistrée : [%d]\ttype : %s\n",j+1, user->contacts[i].identificateurs[j].id, user->contacts[i].identificateurs[j].type);
      }
    }
  }
  else{
    int idContact=atoi(param1);
    for (int i=0; i<user->nbContacts; ++i){
      if (idContact==user->contacts[i].idContact || !strcmp(param1,user->contacts[i].nameContact)){
        printf("- Contats enregistré : %s\tidentifiant contact : [%d]. \n", user->contacts[i].nameContact, user->contacts[i].idContact);
        for (int j=0; j<user->contacts[i].nbId; ++j){
          printf("\t+ %d Clé enregistrée : [%d]\ttype : %s\n", j+1, user->contacts[i].identificateurs[j].id, user->contacts[i].identificateurs[j].type);
        }
      }
    }
  }
}


void addcontact(utilisateur_t* user, char* param1){
  char nom[NAME_MAX_SIZE] = "";   //surname
  ++(user->nbContacts);
  if (user->nbContacts==1){
    user->contacts=(contact_t*)malloc(sizeof(contact_t));
  }
  else{
    realloc(user->contacts,user->nbContacts*sizeof(contact_t));
  }
  user->contacts[user->nbContacts-1].nbId=0;
  int i=0;
  printf("Entrez le nom de votre contact : ");
  fgets(nom,NAME_MAX_SIZE,stdin);
  while(nom[i]!='\n') {
      user->contacts[user->nbContacts-1].nameContact[i] = nom[i];
      i++;
  }
  user->contacts[user->nbContacts-1].nameContact[i] = '\0';
}


/**********************************************************************/

//Fonction qui permet d'exécuter fonctions après avoir testé si erreur ou non
void execution_commande(int tab_cmd[NB_CMD], utilisateur_t* user, char* param1, char* param2, char* param3) {
    if(tab_cmd[CMD_M]==1) {
        man();
    }

    else if(tab_cmd[CMD_LK]==1) {
        if(erreur_listkeys(user,param1,param2,param3)!=0) {
            listkeys(user,param1);
        }
    }

    else if(tab_cmd[CMD_RM]==1) {
        if(erreur_rmkeys(user,param1,param2,param3)!=0) {
            rmkeys(user,param1);
        }
    }

    else if(tab_cmd[CMD_NK]==1) {
        if(erreur_newkeys(user,param1,param2,param3)!=0) {
            newkeys(user,param1,param2);
        }
    }

    else if(tab_cmd[CMD_C]==1) {
        if(erreur_crypt(user,param1,param2,param3)!=0) {
            crypt(user,param1,param2,param3);
        }
    }

    else if(tab_cmd[CMD_U]==1) {
        if(erreur_uncrypt(user,param1,param2,param3)!=0) {
            uncrypt(user,param1,param2,param3);
        }
    }

    else if(tab_cmd[CMD_S]==1) {
        if(erreur_save(param1,param2,param3)!=0) {
            save(user,param1);
        }
    }

    else if(tab_cmd[CMD_SP]==1) {
        if(erreur_savepub(user,param1,param2,param3)!=0) {
            savepub(user,param1,param2);
        }
    }

    else if(tab_cmd[CMD_L]==1) {
        if(erreur_load(param1,param2,param3)!=0) {
            load(user,param1);
        }
    }

    else if(tab_cmd[CMD_SH]==1) {
        if(erreur_show(user,param1,param2,param3)!=0) {
            show(user,param1,param2,param3);
        }
    }
    else if(tab_cmd[CMD_LC]==1) {
        if(erreur_listcontacts(user,param1,param2,param3)!=0) {
            listcontacts(user,param1,param2);
        }
    }
    else if(tab_cmd[CMD_AC]==1) {
        if(erreur_addcontact(user,param1,param2,param3)!=0) {
            addcontact(user,param1);
        }
    }
    /*
    else if(tab_cmd[CMD_MC]==1) {
        if(erreur_modifycontact(user,param1,param2,param3)!=0) {
            modifycontact(user,param1);
        }
    }*/

}
