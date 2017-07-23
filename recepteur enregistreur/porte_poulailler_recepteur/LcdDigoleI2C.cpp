/** LcdDigoleI2C.h
  définitions de la classe LCD Digole I2C
  afficheur lcd 2*16 caractères  avec liaison serie i2c digoleSerial
*/

#include "LcdDigoleI2C.h"
using namespace std;

//constructeur avec debug
// I2C:Arduino UNO: SDA (data line) is on analog input pin 4, and SCL (clock line) is on analog input pin 5 on UNO and Duemilanove
LcdDigoleI2C::LcdDigoleI2C ( TwoWire*, char , byte taille, const boolean debug) : DigoleSerialDisp (&Wire, '\x27'), m_taille(taille), m_debug(debug), m_decalage(0),
  m_ligne(0), m_deplacement(0), m_retroEclairage(1)
{
}

LcdDigoleI2C::~LcdDigoleI2C()
{
}

///-----initialisation for text LCD adapter-----
void LcdDigoleI2C::init () {
  Wire.begin();
  begin();
  effacementAfficheur();
  //displayConfig(1);    //set config display ON, 0=off
  //setI2CAddress(0x29);  //this function only working when you connect using I2C, from 1 to 127
  //delay(1000);
  //setLCDColRow(16,2);  //set LCD Col and Row, only time set up is OK
  backLightOff(); // retro eclairage off
  enableCursor(); // enable cursor
  setPrintPos(0, 0); // position 0 ligne 0 pour débuter le clear screen
  //delay(10); //delay
}

///-----effacement de l'afficheur----
void LcdDigoleI2C::effacementAfficheur() {
  clearScreen(); // CLear screen
}

///-----remise à zero du lcd-----
void LcdDigoleI2C::razLcd() {
  clearScreen(); // CLear screen
  backLightOff(); // retro eclairage
  disableCursor();
}

///-----activation / desactivation du retro eclairage------
void LcdDigoleI2C::retroEclairage () {
  if (m_retroEclairage)   {
    backLightOn();
    m_retroEclairage = false;
  } else {
    backLightOff();
    m_retroEclairage = true;
  }
}

///-----choix activation / desactivation du retro eclairage------
void LcdDigoleI2C::choixRetroEclairage (bool choix) {
  if (choix)   {
    backLightOn();
    m_retroEclairage = false;
  } else {
    backLightOff();
    m_retroEclairage = true;
  }
}

///----affichage une ligne-----
void LcdDigoleI2C::affichageUneLigne(String chaine) {
  resetPos(m_ligne);// efface la ligne
  for (byte i = 0; i < chaine.length(); i++)  {  //move string to right
    print(chaine[i]);
  }
  cursorPosition(m_decalage, m_ligne, "");// decalage, ligne, texte
}

///-----reset display position and clean the line-----
void LcdDigoleI2C::resetPos(byte ligne)
{
  drawStr(0, ligne, "");// position du curseur en 0, ligne
  String chaine = "";
  for (byte i = 0; i < 16; i++)  {  //move string to right
    chaine += " "; // espace
    print(chaine[i]);
  }
  drawStr(0, ligne, "");// position du curseur en 0, ligne
}

///-----affichage de la date ou de l'heure-----
void LcdDigoleI2C::affichageDateHeure(String jourSemaine, byte jourHeure, byte moisMinute,  byte anneeSeconde) {
  m_ligne = 1;
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
  affichageUneLigne(chaineLigne);// affichage sur lcd
}

///-----transformation donnees date et heure------
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
    chaine += dateHeure;
  }
  return chaine;
}
///-----affichage lumiere et fin de course-----
void LcdDigoleI2C::affichageLumFinCourse( int LumFinCourse, byte ligne, String texte, bool siNonReglable)
{
  m_ligne = ligne;
  String chaineLigne = "";
  chaineLigne += "    =  ";
  chaineLigne += LumFinCourse;
  chaineLigne += texte;
  affichageUneLigne(chaineLigne);// affichage sur lcd
  if (siNonReglable)  cursorPosition(0, 0, "");
}

///-----affichage tensions-----
void LcdDigoleI2C::affichageVoltage( float voltage, String texte, byte ligne)
{
  m_ligne = ligne;
  String chaineLigne = "";
  chaineLigne += "    =  ";
  chaineLigne += voltage;
  chaineLigne += texte;
  affichageUneLigne(chaineLigne);// affichage sur lcd
  cursorPosition(0, 0, "");// decalage, ligne, texte
}

///-----affichage choix ouverture fermeture-----
void LcdDigoleI2C::affichageChoix( bool ouverture, bool fermeture,  byte ligne)
{
  m_ligne = ligne;
  String chaineLigne = "";
  chaineLigne += " ouv:";
  if (ouverture)  chaineLigne += "hre "; else  chaineLigne += "lum ";
  chaineLigne += "fer:";
  if (fermeture)  chaineLigne += "hre "; else  chaineLigne += "lum ";
  affichageUneLigne(chaineLigne);// affichage sur lcd
}

///-----affichage pulse et roue codeuse du servo-------
void LcdDigoleI2C::affichageServo(int pulse, int roueCodeuse,  byte ligne)
{
  m_ligne = ligne;
  String chaineLigne = "";
  chaineLigne += pulse;
   chaineLigne += "ms   ";
  chaineLigne += roueCodeuse;
    chaineLigne += "pas";
  affichageUneLigne(chaineLigne);// affichage sur lcd
  cursorPosition(0, 0, ""); // decalage, ligne, texte
}

///-----affichage au demarrage sur les deux lignes-----
void LcdDigoleI2C::bonjour(String chaine1, String chaine2) {
  gestionCurseur (false);
  choixRetroEclairage(true);
  for (byte j = 0; j < 3; j++)  {  //texte clignotant
    effacementAfficheur(); // CLear screen
    delay(400);
    m_ligne = 0;
    affichageUneLigne(chaine1);// affichage sur lcd
    m_ligne = 1;
    affichageUneLigne(chaine2);// affichage sur lcd
    delay (700);
  }
  gestionCurseur (true);
  cursorPosition(0, 0, ""); // decalage, ligne, texte
  choixRetroEclairage(false);
}

///------activation / desactivation du curseur-----
void LcdDigoleI2C::gestionCurseur (bool curseur) {
  if (curseur) enableCursor(); else disableCursor();
}

///-----position du curseur : decalage, ligne, texte-----
void LcdDigoleI2C::cursorPosition(byte decalage, byte ligne, char *texte) {
  m_ligne = ligne;
  drawStr(decalage, m_ligne, texte);
}

///-----position du cuseur pendant les reglages-----
void LcdDigoleI2C::cursorPositionReglages (const byte &touche, bool &relache, bool &reglage,  const byte decalageSup, const byte deplacement, const byte decalageInf) {
  if (touche == 4 and relache == true ) {
    relache = false;
    if (m_decalage < decalageSup ) { // boucle de reglage
      m_decalage += deplacement;   // incrementation decalage
      reglage = true; // reglages
    }
    if (m_decalage > decalageInf ) { // fin de la ligne d'affichage si > decalageInf
      m_decalage = 0;
      reglage = false;
    }
  }
}

//-----accesseur - getter-----
byte LcdDigoleI2C::get_m_decalage() {
  return m_decalage;
}

//-----mutateur - setter-----
void LcdDigoleI2C::set_m_decalage(byte decalage) {
  m_decalage = decalage;
}

