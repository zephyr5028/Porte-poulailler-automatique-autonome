/** LcdPCF8574.h
    /* LcdPCF8574.h
  définitions de la classe LCD circuit PCF8574 I2C
  afficheur lcd 2*16 caractères  avec liaison serie i2c LiquidCrystal_I2C
*/

#include "LcdPCF8574.h"
using namespace std;

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

///-----initialisation for text LCD adapter-----
void LcdPCF8574::init () {
  Wire.begin();
  begin();
  noBacklight();
  noAutoscroll();
  clear();
  blink();
  delay(10); //delay
}

///-----effacement de l'afficheur----
void LcdPCF8574::effacementAfficheur() {
  clear(); // CLear screen
}

///-----remise à zero du lcd-----
void LcdPCF8574::razLcd() {
  noBacklight();
  clear();
  noBlink();
}

///-----activation / desactivation du retro eclairage------
void LcdPCF8574::retroEclairage () {
  if (m_retroEclairage)   {
    backlight();
    m_retroEclairage = false;
  } else {
    noBacklight();
    m_retroEclairage = true;
  }
}

///-----choix activation / desactivation du retro eclairage------
void LcdPCF8574::choixRetroEclairage (bool choix) {
  if (choix)   {
    backlight();
    m_retroEclairage = false;
  } else {
    noBacklight();
    m_retroEclairage = true;
  }
}

///----affichage une ligne-----
void LcdPCF8574::affichageUneLigne(String chaine) {
  resetPos(m_ligne);// efface la ligne
  // Serial.println(chaine.length());
  for (byte i = 0; i < chaine.length(); i++)  {  //move string to right
    print(chaine[i]);
  }
  setCursor(m_decalage, m_ligne);
}

///-----reset display position and clean the line-----
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

///-----affichage de la date ou de l'heure-----
void LcdPCF8574::affichageDateHeure(String jourSemaine, byte jourHeure, byte moisMinute,  byte anneeSeconde)
{
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

///-----transformation donnees date et heure-----
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

///-----affichage lumiere et fin de course-----
void LcdPCF8574::affichageLumFinCourse( unsigned int LumFinCourse, byte ligne, bool siNonReglable)
{
  m_ligne = ligne;
  String chaineLigne = "";
  chaineLigne += "    =  ";
  chaineLigne += LumFinCourse;
  affichageUneLigne(chaineLigne);// affichage sur lcd
  if (siNonReglable)  cursorPosition(0, 0, "");
}

///-----affichage tensions-----
void LcdPCF8574::affichageVoltage( float voltage, String texte, byte ligne)
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
void LcdPCF8574::affichageChoix( bool ouverture, bool fermeture, byte ligne)
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
void LcdPCF8574::affichageServo(int pulse, int roueCodeuse, byte ligne)
{
  m_ligne = ligne;
  String chaineLigne = "";
  chaineLigne += " P:";
  chaineLigne += pulse;
  chaineLigne += "   R:";
  chaineLigne += roueCodeuse;
  affichageUneLigne(chaineLigne);// affichage sur lcd
  cursorPosition(0, 0, ""); // decalage, ligne, texte
}

///-----Bonjour-----
void LcdPCF8574::bonjour(String chaine1, String chaine2) {
  gestionCurseur (false);
  choixRetroEclairage(true);
  for (byte j = 0; j < 5; j++)  {  //texte clignotant
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
void LcdPCF8574::gestionCurseur (bool curseur) {
  if (curseur) blink(); else noBlink();
}

///-----position du curseur : decalage, ligne, texte-----
void LcdPCF8574::cursorPosition(byte decalage, byte ligne, char *texte) {
  m_ligne = ligne;
  setCursor(decalage, m_ligne);
}

///-----position du cuseur pendant les reglages-----
void LcdPCF8574::cursorPositionReglages (const byte &touche, bool &relache, bool &reglage, const byte decalageSup, const byte deplacement, const byte decalageInf) {
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
byte LcdPCF8574::get_m_decalage() {
  return m_decalage;
}

//-----mutateur - setter-----
void LcdPCF8574::set_m_decalage(byte decalage) {
  m_decalage = decalage;
}

