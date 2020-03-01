/*ideeGmp
void RSAcrypt(unsigned char *msg, uint64 *cryptedMsg, rsaKey_t pubKey){
  cryptedMsg= giveMeMem(strlen(msg) - 1);
  int taille_crypterMsg = 0;
  int i = 0;
  uchar char2Int [4];
  uint64 entierCourant; /*entier obtenu grace à la fonction */
  while (i != ((int)strlen(msg)/4 * 4)  {
      for(int j =  0; j < 4 ; j++){
        char2Int[j] = msg[i + j];
      }
      entierCourant  =convert_4byte2int(char2Int);
      cryptedMsg[i]  = puissance_mod_n( entierCourant , pubKey.E, pubKey.N );
      taille_crypterMsg++;
      i+=4;
  }
  if(strlen(msg)%4 != 0){
    int nbLeft = strlen(msg)%4;
    uchar charLeft[nbRestants];
    for(int j =  0; j < nbLeft ; j++){
      charLeft[j] = msg[i + j];
    }
    entierCourant  =convert_4byte2int(charLeft);
    cryptedMsg[i]  = puissance_mod_n( entierCourant , pubKey.E, pubKey.N );
    taille_crypterMsg++;
    cryrealloc()
  }
}*/
