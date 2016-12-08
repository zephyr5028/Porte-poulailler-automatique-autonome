/* radio 433MHz */

/*-----virtualWire pour la liaison RF 433Mhz-----*/
#include <VirtualWire.h>

//----routine envoi message radio----
void envoiMessage(char chaine1[]) {
  //if (chaine1[0] == "x") {
    
 // } else {
    strcat(chaine, chaine1);
    strcat(chaine, ";");
    messageRadio(chaine);
//  }
}

// message Radio
void messageRadio(char chaine1[]) {
  strcat(chaine1, '\0');
  vw_send((uint8_t *)chaine1, strlen(chaine1) + 1); // On envoie le message
  // strlen : Retourne le nombre de caractères de cs sans tenir compte du caractère de fin de chaîne.
  vw_wait_tx(); // On attend la fin de l'envoi
  delay(10);
  chaine[taille] = "";
}

//----chaine radio vide pour espacer-----
void chaineVide() {
  if (RADIO) {
    char chaine1[9] = "x x x x";
    envoiMessage(chaine1);// on envoie le message
  }
}
