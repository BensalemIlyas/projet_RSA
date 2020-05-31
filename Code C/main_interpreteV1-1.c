#include "rsa_header.h"

FILE* logfp;

int main(int argc, char** argv) {
    logfp = fopen("logfp","w");
    char maChaine[100]="";
    int tailleCommandeCoupee;
    int tab_cmd[NB_CMD];
    int tailleChaine;
    char param1[50];
    char param2[50];
    char param3[50];
    int nb_erreur;
    utilisateur_t user;


    init_user(&user);
    user.tabId = (identificateur_t*)malloc(user.nbId*sizeof(identificateur_t));
    user.contacts=(contact_t*)malloc(user.nbContacts*sizeof(contact_t));
    user.contacts[0].identificateurs=(identificateur_t*)malloc(user.nbId*sizeof(identificateur_t));

    printf("\nBienvenue %s %s, vous êtes dans l'interpréteur de commandes.\n",user.name,user.surname);
    printf("Veuillez taper <man> pour connaître toutes les commandes.");

    for(int i=0; i<NB_CMD; i++) tab_cmd[i]=0;
    while(tab_cmd[CMD_Q] == 0) {

        for(int i=0; i<NB_CMD; i++) tab_cmd[i]=0;
        tailleChaine = 0;
        printf("\nEntrez votre commande : ");
        fgets(maChaine,100,stdin);

        while(maChaine[tailleChaine]!='\n') tailleChaine++;
        char commande_cut[tailleCommandeCoupee];
        printf("\n Attention !!!! 1\n");
        tailleCommandeCoupee = couper_chaine(maChaine,commande_cut);

        analyse_commandes(commande_cut,tab_cmd);
        parametres(maChaine, tailleChaine, tailleCommandeCoupee, param1, param2, param3);

        gestion_erreur_commande(maChaine, tailleChaine, tab_cmd, tailleCommandeCoupee, &nb_erreur);

        if(nb_erreur==0) {
            execution_commande(tab_cmd, &user, param1, param2, param3);
        }

    }

    for (int i=0; i<user.nbContacts;++i){
      for (int j=0; j<user.contacts[i].nbId; ++j){
        free(&user.contacts[i].identificateurs[j]);
      }
    }
    for (int i=0; i<user.nbContacts;++i){
      free(&user.contacts[i]);
    }
    for (int i=0; i<user.nbId; ++i){
      free(&user.tabId[i]);
    }


    return 0;
}
