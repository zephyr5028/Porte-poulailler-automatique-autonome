/* LcdDigoleI2C.h
  définitions de la classe LCD Digole I2C
  afficheur lcd 2*16 caractères  avec liaison serie i2c digoleSerial
*/

#include "LcdDigoleI2C.h"
using namespace std;

//constructeur avec debug
// I2C:Arduino UNO: SDA (data line) is on analog input pin 4, and SCL (clock line) is on analog input pin 5 on UNO and Duemilanove
LcdDigoleI2C::LcdDigoleI2C ( TwoWire*, char , byte taille, const boolean debug) : DigoleSerialDisp (&Wire, '\x27'), m_taille(taille), m_debug(debug), m_decalage(0), m_ligne(0)
{
}

LcdDigoleI2C::~LcdDigoleI2C()
{
}

//-----initialisation for text LCD adapter-----
void LcdDigoleI2C::init () {
  Wire.begin();
  begin();
  clearScreen(); // CLear screen
  //displayConfig(1);    //set config display ON, 0=off
  //setI2CAddress(0x29);  //this function only working when you connect using I2C, from 1 to 127
  //delay(1000);
  //setLCDColRow(16,2);  //set LCD Col and Row, only time set up is OK
  backLightOff(); // retro eclairage off
  enableCursor(); // enable cursor
  setPrintPos(0, 0); // position 0 ligne 0 pour débuter le clear screen
  delay(10); //delay
}

// -----remise à zero du lcd-----
void LcdDigoleI2C::razLcd() {
  clearScreen(); // CLear screen
  backLightOff(); // retro eclairage
  disableCursor(); // cursor
  setPrintPos(0, 0); // position 0 ligne 0 pour débuter le clear screen
  delay(10); //delay
}

//----affichage une ligne-----
void LcdDigoleI2C::affichageUneLigne(String &chaine) {
  resetPos(1);// efface la ligne 1
  for (byte i = 0; i < 16; i++)  {  //move string to right
    DigoleSerialDisp::print(chaine[i]);
  }
  chaine[0] = '\0'; // effacement du tableau
  drawStr(m_decalage, m_ligne, ""); // curseur position : decalage, ligne 1
}

//-----reset display position and clean the line-----
void LcdDigoleI2C::resetPos(byte ligne)
{
  String chaine = "";
  drawStr(0, ligne, ""); // position du curseur en 0, ligne
  //chaine += "                "; // chaine vide
  for (byte i = 0; i < 16; i++)  {  //move string to right
    chaine  += " "; // chaine vide
    DigoleSerialDisp::print(chaine[i]);
  }
  chaine[0] = '\0'; // effacement du tableau
  drawStr(0, ligne, ""); // position du curseur en 0, ligne
}

//-----affichage de la date ou de l'heure-----
void LcdDigoleI2C::affichageDateHeure(String jourSemaine, byte jourHeure, byte moisMinute, byte anneeSeconde, byte decalage) {
  m_ligne = 1;
  m_decalage = decalage;
  String chaineLigne = "";
  if (jourSemaine == "H") {
    chaineLigne += "   ";
    chaineLigne.concat(transformation( "h ", jourHeure));// print heure
    chaineLigne.concat(transformation( "m ", moisMinute));;// print minutes
    if ( anneeSeconde <= 60) {
      chaineLigne.concat(transformation( "s ", anneeSeconde));// print secondes
    }
  } else {
    chaineLigne += " ";
    chaineLigne += jourSemaine;
    chaineLigne.concat(transformation( " ", jourHeure));// print jour
    chaineLigne.concat(transformation( " ", moisMinute));;// print mois
    chaineLigne.concat(transformation( " 20", anneeSeconde));// print année depuis 1970
  }
  if (m_debug) {
    Serial.println(chaineLigne);
  }
  affichageUneLigne(chaineLigne);// affichage sur lcd
}

// transformation donnees date et heure
String LcdDigoleI2C::transformation (String texte, byte dateHeure ) {
  String chaine = "";
  if (texte == "h " or texte == "m " or texte == "s ") {
    if (dateHeure < 10) {
      chaine += "0"; // si < 10
    }
    chaine += dateHeure;
    chaine += texte;
  } else {
    chaine += texte;
    if (dateHeure < 10) {
      chaine += "0"; // si < 10
    }
    if (texte == " 20")  chaine += dateHeure - 30; else  chaine += dateHeure;//  si -30 : année depuis 1970
  }
  return chaine;
}

//-----affichage lumiere et fin de course-----
void LcdDigoleI2C::affichageLumFinCourse( unsigned int LumFinCourse, byte decalage, byte ligne)
{
  m_ligne = ligne;
  m_decalage = decalage;
  String chaineLigne = "";
  chaineLigne += "    =  ";
  chaineLigne += LumFinCourse;
  affichageUneLigne(chaineLigne);// affichage sur lcd
}

//-----affichage tensions-----
void LcdDigoleI2C::affichageVoltage( float voltage, String texte, byte decalage, byte ligne)
{
  m_ligne = ligne;
  m_decalage = decalage;
  String chaineLigne = "";
  chaineLigne += "    =  ";
  chaineLigne += voltage;
  chaineLigne += texte;
  affichageUneLigne(chaineLigne);// affichage sur lcd
}

//-----affichage choix ouverture fermeture-----
void LcdDigoleI2C::affichageChoix( bool ouverture, bool fermeture, byte decalage, byte ligne)
{
  m_ligne = ligne;
  m_decalage = decalage;
  String chaineLigne = "";
  chaineLigne += " ouv:";
  if (ouverture)  chaineLigne += "hre "; else  chaineLigne += "lum ";
  chaineLigne += "fer:";
  if (fermeture)  chaineLigne += "hre "; else  chaineLigne += "lum ";
  affichageUneLigne(chaineLigne);// affichage sur lcd
}

//-----affichage pulse et roue codeuse du servo-------
void LcdDigoleI2C::affichageServo(int pulse, int roueCodeuse, byte decalage, byte ligne)
{
  m_ligne = ligne;
  m_decalage = decalage;
  String chaineLigne = "";
  chaineLigne += " P:";
  chaineLigne += pulse;
  chaineLigne += "   R:";
  chaineLigne += roueCodeuse;
  affichageUneLigne(chaineLigne);// affichage sur lcd
}

//-----Bonjour-----
void LcdDigoleI2C::bonjour() {
  for (byte j = 0; j < 1; j++)  {  //making "Hello" string moving
    for (byte i = 0; i < 9; i++)  {  //move string to right
      setPrintPos(i, 0); // ligne 0
      DigoleSerialDisp::print(F(" Bonjour"));
      delay(200); //delay
    }
    for (byte i = 0; i < 9; i++) {  //move string to left
      setPrintPos(8 - i, 0);
      DigoleSerialDisp::print(F("Bonjour "));
      delay(200);
    }
  }
}
/*
//-----affichage ligne titres----
void LcdDigoleI2C::ligneTitres(const char &affichageMenu, const byte &incrementation) {
  byte j = ((incrementation - 1) * (m_taille + 1)); // tous les 16 caractères
  drawStr(0, 0, ""); // position du curseur en 0,0
  for (byte i = j; i < j + m_taille; i++) { // boucle pour afficher 16 caractères sur le lcd
    char temp = pgm_read_byte(affichageMenu + i); // utilisation du texte présent en mèmoire flash
    print(temp);
  }
}*/
