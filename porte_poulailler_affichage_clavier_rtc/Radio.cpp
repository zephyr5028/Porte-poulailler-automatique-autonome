/* Radio.cpp
  définitions de la classe Radio
  radio 433MHz
*/

#include "Radio.h"

//constructeur avec debug et radio pour affichage si nécessaire
Radio::Radio(byte taille, boolean radio, boolean debug) :  m_taille(taille), m_radio(radio), m_debug(debug)
{
  m_chaine[m_taille] = "";// initialisation du tableau
}

Radio::~Radio()
{
}

//----routine envoi message radio----
void Radio::envoiMessage(char chaine1[]) {
  char chaineComp[] = "Fin";
  if (strcmp(chaineComp, chaine1) != 0) { // test de la dernière chaine
    strcat(m_chaine, chaine1);
    if (m_debug) {
      Serial.println(m_chaine);
    }
  } else {
    Radio::messageRadio(m_chaine);
    strcpy(m_chaine, "");// effacement du tableau
    //*m_chaine=0;// effacement du tableau
    //m_chaine[0] = '\0'; // effacement du tableau
  }
}

//----message Radio-----
void Radio::messageRadio(char chaine1[]) {
  strcat(chaine1, "\0");
  vw_send((uint8_t *)chaine1, strlen(chaine1) + 1); // On envoie le message
  // strlen : Retourne le nombre de caractères de cs sans tenir compte du caractère de fin de chaîne.
  vw_wait_tx(); // On attend la fin de l'envoi
  delay(10);
}

//----chaine radio fin de ligne avant transmission-----
void Radio::chaineVide() {
  if (m_radio) {
    char chaine1[m_taille - 1] = "";
    strcat(chaine1, "Fin");
    Radio::envoiMessage(chaine1);// on envoie le message
  }
}

//-----envoi message float avec test de l'ouverture du boitier plus texte-----
void Radio::envoiFloat(float valeur , boolean boitierOuvert, char texte[]) {
  if (m_radio and !boitierOuvert) {
    char chaine1[m_taille - 1] = "";
    char valeur_temp[6] = "";
    dtostrf(valeur, 2, 2, valeur_temp);
    strcat(chaine1, valeur_temp);
    strcat(chaine1, texte);
    Radio::envoiMessage(chaine1);// on envoie le message
  }
}

//-----envoi message unsigned int avec test de l'ouverture du boitier plus texte-----
void Radio::envoiUnsignedInt(unsigned int valeur, boolean boitierOuvert, char texte[]) {
  if (m_radio and !boitierOuvert) {
    char chaine1[m_taille - 1] = "";
    char valeur_temp[5];
    sprintf(valeur_temp, "%i", valeur);
    strcat(chaine1, valeur_temp);
    strcat(chaine1, texte);
    Radio::envoiMessage(chaine1);// on envoie le message
  }
}

