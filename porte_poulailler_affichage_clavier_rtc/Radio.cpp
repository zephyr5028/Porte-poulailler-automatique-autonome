/* Radio.cpp
  définitions de la classe Radio
  radio 433MHz
*/

#include "Radio.h"

//constructeur avec debug et radio pour affichage si nécessaire
Radio::Radio(byte pinEmRadio, int vitesseTransmission, byte taille, const boolean radio, const boolean debug) :  m_pinEmRadio(pinEmRadio),
  m_vitesseTransmission(vitesseTransmission), m_taille(taille), m_radio(radio), m_debug(debug)
{
  m_chaine[m_taille] = "";// initialisation du tableau
}

Radio::~Radio()
{
}

//initialisation-----
void Radio::init () {
  vw_set_tx_pin(m_pinEmRadio); // broche d10 emetteur
  vw_setup(m_vitesseTransmission); // initialisation de la bibliothèque avec la vitesse (vitesse_bps)
}

//----message Radio-----
//void Radio::messageRadio(char chaine1[]) {
void Radio::messageRadio(char *chaine1) {
  strcat(chaine1, "\0");
  vw_send((uint8_t *)chaine1, strlen(chaine1) + 1); // On envoie le message
  // strlen : Retourne le nombre de caractères de cs sans tenir compte du caractère de fin de chaîne.
  vw_wait_tx(); // On attend la fin de l'envoi
  delay(10);
}

//----routine construction message radio----
//void Radio::envoiMessage(char chaine1[]) {
void Radio::envoiMessage(char *chaine1) {
  char chaineComp[] = "Fin";
  if (strcmp(chaineComp, chaine1) != 0) { // test de la dernière chaine
    //  strcat(m_chaine, chaine1);
    byte lenChaine1(0);
    while (chaine1[lenChaine1] != '\0') lenChaine1++;
    byte  len_m_chaine(0);
    while (m_chaine[len_m_chaine] != '\0')  len_m_chaine++;
    for (byte pos = 0; pos < lenChaine1; pos++) {
      m_chaine[len_m_chaine + pos] = chaine1[pos];
      m_chaine[len_m_chaine + lenChaine1] = '\0';
    }
    if (m_debug) {
      Serial.println(m_chaine);
    }
  } else {
    messageSansParametre();// envoi du message contenu dans m_chaine[]
  }
}

//----message Radio sans parametre-----
void Radio::messageSansParametre() {
  strcat(m_chaine, "\0");
  vw_send((uint8_t *)m_chaine, strlen(m_chaine) + 1); // On envoie le message
  // strlen : Retourne le nombre de caractères de cs sans tenir compte du caractère de fin de chaîne.
  vw_wait_tx(); // On attend la fin de l'envoi
  delay(10);
  m_chaine[0] = '\0'; // effacement du tableau
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
//void Radio::envoiFloat(float valeur , boolean boitierOuvert, char texte[]) {
void Radio::envoiFloat(float valeur, boolean boitierOuvert, char *texte ) {
  if (m_radio and !boitierOuvert) {
    char chaine[m_taille - 1] = "";
    char valeur_temp[8] = "";
    dtostrf(valeur, 2, 2, valeur_temp);
    strcat(chaine, valeur_temp);
    strcat(chaine, texte);
    Radio::envoiMessage(chaine);// on envoie le message
  }
}

//-----envoi message unsigned int avec test de l'ouverture du boitier plus texte-----
//void Radio::envoiUnsignedInt(unsigned int valeur, boolean boitierOuvert, char texte[]) {
void Radio::envoiUnsignedInt(unsigned int valeur, boolean boitierOuvert, char *texte) {
  if (m_radio and !boitierOuvert) {
    char chaine1[m_taille - 1] = "";
    char valeur_temp[5] = "";
    sprintf(valeur_temp, "%i", valeur);
    strcat(chaine1, valeur_temp);
    strcat(chaine1, texte);
    Radio::envoiMessage(chaine1);// on envoie le message
  }
}


