/** LcdPCF8574.h
  prototype de la classe LCD circuit PCF8574 I2C
  afficheur lcd 2*16 caractères  avec liaison serie i2c LiquidCrystal_I2C
*/

#ifndef LcdPCF8574_h
#define LcdPCF8574_h

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "Arduino.h"

class LcdPCF8574  : public LiquidCrystal_I2C
{
  public:

    // constructeur  avec debug
    // I2C:Arduino UNO: SDA (data line) is on analog input pin 4, and SCL (clock line) is on analog input pin 5 on UNO and Duemilanove
    LcdPCF8574 ( char , const byte taille = 16, const byte nbLigne = 2,  boolean debug = false);

    ~LcdPCF8574(); // destructeur


    void init();// initialisation
    void effacementAfficheur(); //effacement de l'afficheur
    void affichageUneLigne(String chaine); // affichage une ligne
    void resetPos(byte ligne);//reset display position and clean the line
    void affichageDateHeure(String jourSemaine, byte jourHeure, byte moisMinute,  byte anneeSeconde);//affichage de la date ou de l'heure
    String transformation (String chaine, byte dateHeure); // transformation donnees date et heure
    void affichageLumFinCourse( int lum,  byte ligne,String texte, bool siNonReglable = 0); //affichage lumiere et fin de course
    void affichageVoltage( float voltage, String texte,  byte ligne); //affichage tensions
    void affichageChoix( bool ouverture, bool fermeture,  byte ligne);//affichage choix ouverture fermeture
    void affichageServo(int pulse, int compteRoueCodeuse,  byte ligne) ; //affichage pulse et roue codeuse du servo
    void razLcd(); //remise à zero du lcd
    void bonjour(String chaine1, String chaine2); //Bonjour
    void cursorPosition(byte decalage, byte ligne, char *texte); // position du curseur
    void gestionCurseur (bool curseur);//activation / desactivation du curseur
    void retroEclairage ();//activation / desactivation du retro eclairage (bascule)
    void choixRetroEclairage (bool choix);//choix activation / desactivation du retro eclairage
    //position du cuseur pendant les reglages
    void cursorPositionReglages (const byte &touche, bool &relache, bool &reglage, const byte decalageSup, const byte deplacement, const byte decalageInf);
    byte get_m_decalage();//accesseur - getter
    void set_m_decalage(byte decalage);//mutateur - setter


  protected:

    const boolean m_debug ; // pour affichage console si nécessaire
    byte m_taille; // taille du tableau - 16 caractères
    byte m_decalage; // position du curseur
    byte m_ligne; // numero ligne
    byte m_deplacement; // deplacement du curseur
    bool m_retroEclairage; // position retro eclairage

};
#endif


