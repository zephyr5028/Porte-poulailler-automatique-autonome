/* LcdPCF8574.h
    /* LcdPCF8574.h
  définitions de la classe LCD circuit PCF8574 I2C
  afficheur lcd 2*16 caractères  avec liaison serie i2c LiquidCrystal_I2C
*/

#include "LcdPCF8574.h"

//constructeur avec debug
// I2C:Arduino UNO: SDA (data line) is on analog input pin 4, and SCL (clock line) is on analog input pin 5 on UNO and Duemilanove
//LiquidCrystal_I2C lcd(0x27, 16, 2);
LcdPCF8574::LcdPCF8574 ( char, const byte taille, const byte nbLigne, const boolean debug) : LiquidCrystal_I2C (0x27, taille, nbLigne), m_debug(debug), m_decalage(0),
  m_ligne(0), m_deplacement(0), m_retroEclairage(1)
{

}

LcdPCF8574::~LcdPCF8574()
{
}

//-----initialisation for text LCD adapter-----
void LcdPCF8574::init () {
  Wire.begin();
  begin();
  noBacklight();
  noAutoscroll();
  clear();
  blink();
  delay(10); //delay
}

//-----remise à zero du lcd-----
void LcdPCF8574::razLcd() {
  clear();
}

//------activation / desactivation du curseur-----
void LcdPCF8574::gestionCurseur (bool curseur) {
  if (curseur) blink(); else noBlink();
}

//-----activation / desactivation du retro eclairage------
void LcdPCF8574::retroEclairage () {
  if (m_retroEclairage)   {
    backlight();
    m_retroEclairage = false;
  } else {
    noBacklight();
    m_retroEclairage = true;
  }
}

//-----choix activation / desactivation du retro eclairage------
void LcdPCF8574::choixRetroEclairage (bool choix) {
  if (choix)   {
    backlight();
    m_retroEclairage = false;
  } else {
    noBacklight();
    m_retroEclairage = true;
  }
}

//----affichage une ligne-----
void LcdPCF8574::affichageUneLigne(String chaine) {
  resetPos(1);// efface la ligne 1
  // Serial.println(chaine.length());
  for (byte i = 0; i < chaine.length(); i++)  {  //move string to right
    print(chaine[i]);
  }
  setCursor(m_decalage, m_ligne);
}

//-----reset display position and clean the line-----
void LcdPCF8574::resetPos(byte ligne)
{
  setCursor(0, ligne);
  String chaine = "";
  for (byte i = 0; i < 16; i++)  {  //move string to right
    chaine += " "; // espace
    print(chaine[i]);
  }
  setCursor(0, ligne);
}

//-----affichage de la date ou de l'heure-----
void LcdPCF8574::affichageDateHeure(String jourSemaine, byte jourHeure, byte moisMinute,  byte anneeSeconde, byte decalage) {
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
    chaineLigne.concat(transformation( " ", moisMinute));// print mois
    chaineLigne += " ";
    chaineLigne += anneeSeconde + 1970; // année depuis 1970
  }
  if (m_debug) {
    Serial.println(chaineLigne);
  }
  affichageUneLigne(chaineLigne);// affichage sur lcd
}

// transformation donnees date et heure
String LcdPCF8574::transformation (String texte, byte dateHeure ) {
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
    chaine += dateHeure;
  }
  return chaine;
}

//-----affichage lumiere et fin de course-----
void LcdPCF8574::affichageLumFinCourse( unsigned int LumFinCourse, byte decalage, byte ligne)
{
  m_ligne = ligne;
  m_decalage = decalage;
  String chaineLigne = "";
  chaineLigne += "    =  ";
  chaineLigne += LumFinCourse;
  affichageUneLigne(chaineLigne);// affichage sur lcd
}

//-----affichage tensions-----
void LcdPCF8574::affichageVoltage( float voltage, String texte, byte decalage, byte ligne)
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
void LcdPCF8574::affichageChoix( bool ouverture, bool fermeture, byte decalage, byte ligne)
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
void LcdPCF8574::affichageServo(int pulse, int roueCodeuse, byte decalage, byte ligne)
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
void LcdPCF8574::bonjour() {
  backlight();
  for (byte j = 0; j < 1; j++)  {  //making "Hello" string moving
    for (byte i = 0; i < 9; i++)  {  //move string to right
      setCursor(i, 0);
      print(F(" Bonjour"));// F() pour memoire flash
      delay(200); //delay
    }
    for (byte i = 0; i < 9; i++) {  //move string to left
      setCursor(8 - i, 0);
      print(F("Bonjour "));
      delay(200);
    }
  }
  noBacklight();
}

// -----position du curseur : decalage, ligne, texte-----
void LcdPCF8574::cursorPosition(byte decalage, byte ligne, char *texte) {
  m_decalage = decalage;
  m_ligne = ligne;
  setCursor(m_decalage, m_ligne);
}

//-----position du cuseur pendant les reglages-----
void LcdPCF8574::cursorPositionReglages (const byte &touche, bool &relache, bool &reglage, byte &decalage, const byte decalageSup, const byte deplacement, const byte decalageInf) {
  if (touche == 4 and relache == true ) {
    relache = false;
    if (decalage < decalageSup ) { // boucle de reglage
      decalage = decalage + deplacement;   // incrementation decalage
      reglage = true; // reglages
    }
    if (decalage > decalageInf ) { // fin de la ligne d'affichage si > decalageInf
      decalage = 0;
      reglage = false;
    }
    m_decalage = decalage;
  }
}

