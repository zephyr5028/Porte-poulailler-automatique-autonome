/* Radior.h
  prototype de la classe Radio
  radio 433MHz
*/

#ifndef Radio_h
#define Radio_h

/*-----virtualWire pour la liaison RF 433Mhz-----*/
#include <VirtualWire.h>
#include "Arduino.h"

class Radio
{
  public:

    // constructeur avec la taille du tableau, debug, radio et le type de temperature
    Radio(byte pinEmRadio, int vitesseTransmission, byte taille, boolean radio =false, boolean debug =false);
    ~Radio(); // destructeur

    void init();// initialisation
    void envoiMessage(char chaine1[]);//routine envoi message radio
    void messageRadio(char chaine1[]);  // message Radio
    void chaineVide();//chaine radio fin de ligne avant transmission
    //envoi message float avec test de l'ouverture du boitier plus texte
    void envoiFloat(float valeur , boolean boitierOuvert = true, char texte[] = "");
    // envoi du message unsigned int avec test de l'ouverture du boitier
    void envoiUnsignedInt(unsigned int valeur, boolean boitierOuvert = true, char texte[] = "");

  protected:

    byte m_pinEmRadio; // pin D10 emetteur radio
    int m_vitesseTransmission;// vitese de la transmission radio
    boolean m_debug ; // pour affichage console si nécessaire
    boolean m_radio; // pour envoi radio si necessaire
    int const m_taille; // taille du tableau
    char m_chaine[]; // tableau de la chaine avant envoi

};

#endif

