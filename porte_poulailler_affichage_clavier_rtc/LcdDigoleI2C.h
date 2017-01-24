/* LcdDigoleI2C.h
  prototype de la classe LCD Digole I2C
  afficheur lcd 2*16 caractères  avec liaison serie i2c digoleSerial
*/

#ifndef LcdDigoleI2C_h
#define LcdDigoleI2C_h

/* I2C */
/*----------NOTE----------
  new version of lib will save you compiled code size for the sample
  The size for 2 version(@Arduino 1.0.1):
      UART  I2C    SPI
  OLD   8998  8988   9132
  NEW   6966  7566   6354
  ------------------------*/
/*------all available are:_Digole_Serial_UART_, _Digole_Serial_I2C_ and _Digole_Serial_SPI_ ------*/

#define _Digole_Serial_I2C_  // To tell compiler compile the special communication only, 
#include <DigoleSerial.h> // bibliotheque afficheur serie
#include <Wire.h>

#include "Arduino.h"

class LcdDigoleI2C  : public DigoleSerialDisp  {
  public:

    // constructeur  avec debug
    // I2C:Arduino UNO: SDA (data line) is on analog input pin 4, and SCL (clock line) is on analog input pin 5 on UNO and Duemilanove
    LcdDigoleI2C ( TwoWire* , char , byte taille, boolean debug = false);
    ~LcdDigoleI2C(); // destructeur

    void init();// initialisation
    void affichageUneLigne(String chaine); // affichage une ligne
    void resetPos(byte ligne);//reset display position and clean the line
    void affichageDateHeure(String jourSemaine, byte jourHeure, byte moisMinute,  byte anneeSeconde, byte decalage);//affichage de la date ou de l'heure
    String transformation (String chaine, byte dateHeure); // transformation donnees date et heure
    void affichageLumFinCourse( unsigned int lum, byte decalage, byte ligne); //affichage lumiere et fin de course
    void affichageVoltage( float voltage, String texte, byte decalage, byte ligne); //affichage tensions
    void affichageChoix( bool ouverture, bool fermeture, byte decalage, byte ligne);//affichage choix ouverture fermeture
    void affichageServo(int pulse, int compteRoueCodeuse, byte decalage, byte ligne) ; //affichage pulse et roue codeuse du servo
    void razLcd(); //remise à zero du lcd
    void bonjour(); //Bonjour
    void cursorPosition(byte decalage, byte ligne, char *texte); // position du curseur
    void gestionCurseur (bool curseur);//activation / desactivation du curseur
    void retroEclairage ();//activation / desactivation du retro eclairage (bascule)
    void choixRetroEclairage (bool choix);//choix activation / desactivation du retro eclairage


  protected:

    const boolean m_debug ; // pour affichage console si nécessaire
    int const m_taille; // taille du tableau - 16 caractères
    byte m_decalage; // position du curseur
    byte m_ligne; // numero ligne
    byte m_deplacement; // deplacement du curseur
    bool m_retroEclairage; // position retro eclairage

};

#endif


