/** Radio.h
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
    Radio(byte pinEmRadio, byte pinSwitchEmissionRadio, int vitesseTransmission, byte taille, boolean radio = false, boolean debug = false);
    ~Radio(); // destructeur

    void init();// initialisation
    void raz_m_chaine(); // raz de m_chaine
    void envoiMessage(char *chaine1);//routine construction message radio
    void messageRadio(char *chaine1);  // message Radio
    void messageSansParametre();//message Radio sans parametre
    void chaineVide();//chaine radio fin de ligne avant transmission
    //envoi message float avec test de l'ouverture du boitier plus texte
    void envoiFloat(float valeur, boolean boitierOuvert = true, char *texte = (char *)"");
    // envoi du message unsigned int avec test de l'ouverture du boitier
    void envoiUnsignedInt(unsigned int valeur, boolean boitierOuvert = true, char *texte = (char *)"");
    // envoi du message int avec test de l'ouverture du boitier
    void envoiInt(int valeur, boolean boitierOuvert = true, char *texte = (char *)"");
    void envoiTexte(boolean boitierOuvert, char *texte = (char *)""); // envoi d'un message texte avec test de l'ouverture du boitier
    void testSwitchEmissionRadio();//test du switch emission radio on/off
    bool get_m_radio(); //accesseur - getter
    void set_m_radio(bool radio);//mutateur - setter

  protected:

    const boolean m_debug ; // pour affichage console si nécessaire
    byte m_pinEmRadio; // pin D10 emetteur radio
    byte m_pinSwitchEmissionRadio; // pin A2 pour le choix avec ou sans emission radio
    int m_vitesseTransmission;// vitese de la transmission radio
    bool m_radio; // pour envoi radio si necessaire
    int m_taille; // taille du tableau
    char m_chaine[]; // tableau de la chaine avant envoi

};

#endif


