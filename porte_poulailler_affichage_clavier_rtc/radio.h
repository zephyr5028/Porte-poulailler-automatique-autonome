/* radio 433MHz */

/*-----virtualWire pour la liaison RF 433Mhz-----*/
#include <VirtualWire.h>

//----routine envoi message radio----
void envoiMessage(char chaine1[]) {
  byte res = strcmp(chaine1, "xx");
  if (res != 0) { // test de la dernière chaine
    strcat(chaine, chaine1);
    strcat(chaine, ";");
    if (DEBUG) {
      Serial.println(chaine);
    }
  } else {
    messageRadio(chaine);
    chaine[0] = '\0';
  }
}

// message Radio
void messageRadio(char chaine1[]) {
  strcat(chaine1, '\0');
  vw_send((uint8_t *)chaine1, strlen(chaine1) + 1); // On envoie le message
  // strlen : Retourne le nombre de caractères de cs sans tenir compte du caractère de fin de chaîne.
  vw_wait_tx(); // On attend la fin de l'envoi
  delay(10);
}

//----chaine radio fin de ligne avant transmission
void chaineVide() {
  if (RADIO) {
    char chaine1[4] = "";
    strcat(chaine1, "xx");
    envoiMessage(chaine1);// on envoie le message
  }
}
