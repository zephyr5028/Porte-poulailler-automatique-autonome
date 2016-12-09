/* Radio.cpp
  définitions de la classe Radio
  radio 433MHz
*/

/*-----virtualWire pour la liaison RF 433Mhz-----*/
#include <VirtualWire.h>
#include "Arduino.h"
#include "Radio.h"

//constructeur avec debug et radio pour affichage si nécessaire
Radio::Radio( boolean radio, boolean debug) :  m_radio(radio), m_debug(debug), m_taille(80)
{
  m_chaine[m_taille] = "";
  m_chaine[0] = '\0'; // initialisation du tableau m_chaine
}

Radio::~Radio()
{
}

//----routine envoi message radio----
void Radio::envoiMessage(char chaine1[]) {
  byte resultat = strcmp(chaine1, "\0");
  if (resultat != 0) { // test de la dernière chaine
    strcat(m_chaine, chaine1);
    strcat(m_chaine, ";");
    if (m_debug) {
      Serial.println(m_chaine);
    }
  } else {
    Radio::messageRadio(m_chaine);
    m_chaine[0] = '\0';
  }
}

//----message Radio-----
void Radio::messageRadio(char chaine1[]) {
  strcat(chaine1, '\0');
  vw_send((uint8_t *)chaine1, strlen(chaine1) + 1); // On envoie le message
  // strlen : Retourne le nombre de caractères de cs sans tenir compte du caractère de fin de chaîne.
  vw_wait_tx(); // On attend la fin de l'envoi
  delay(10);
}

//----chaine radio fin de ligne avant transmission-----
void Radio::chaineVide() {
  if (m_radio) {
    char chaine1[1] = "";
    strcat(chaine1, "\0");
    Radio::envoiMessage(chaine1);// on envoie le message
  }
}
