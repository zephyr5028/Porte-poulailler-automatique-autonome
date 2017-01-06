/* LcdDigoleI2C.h
  définitions de la classe LCD Digole I2C
  afficheur lcd 2*16 caractères  avec liaison serie i2c digoleSerial
*/

#include "LcdDigoleI2C.h"
using namespace std;

//constructeur avec debug
// I2C:Arduino UNO: SDA (data line) is on analog input pin 4, and SCL (clock line) is on analog input pin 5 on UNO and Duemilanove
LcdDigoleI2C::LcdDigoleI2C ( TwoWire*, char , byte taille, const boolean debug) : DigoleSerialDisp (&Wire, '\x27'), m_taille(taille), m_debug(debug)
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
  delay(100); //delay
}

//----affichage une ligne-----
void LcdDigoleI2C::affichageUneLigne(String chaine) {
  setPrintPos(0, 1);//curseur position 0 ligne 1
  print("                 "); //display space, use to clear the demo line
  for (byte i = 0; i <= 15; i++)  {  //move string to right
    setPrintPos(i, 1); // ligne 1
    print(chaine[i]);
  }
  drawStr(0, 1, ""); // curseur position 0 ligne 1
}

//-----reset display position and clean the line-----
void LcdDigoleI2C::resetpos(void)
{
  setPrintPos(0, 1);
  println("                "); //display space, use to clear the demo line
  setPrintPos(0, 1);
}

//-----affichage de la date-----
void LcdDigoleI2C::affichageDateHeure(String jourSemaine, byte jourHeure, byte moisMinute, byte anneeSeconde) {
  // affichage du jour de la semaine
  String chaineLigne = "";
  if (jourSemaine == "H") {
    chaineLigne += "   ";
    chaineLigne.concat(transformation( "h ", jourHeure));// print heure
    chaineLigne.concat(transformation( "m ", moisMinute));;// print minutes
    chaineLigne.concat(transformation( "s ", anneeSeconde));// print secondes
    /*
      if (tm.Hour < 10) {
      mydisp.print(F("0"));  // si < 10
      }
      mydisp.print(tm.Hour, DEC); // print heure
      mydisp.print(F("h "));
      if (tm.Minute < 10) {
      mydisp.print(F("0"));  // si < 10
      }
      mydisp.print(tm.Minute, DEC); // print minute
      mydisp.print(F("m "));
      if (tm.Second < 10) {
      mydisp.print(F("0"));  // si < 10
      }
      mydisp.print(tm.Second, DEC); // print seconde
      mydisp.print(F("s  "));
      mydisp.drawStr(decalage, 1, ""); // curseur position 0 ligne 1
    */
  } else {
    chaineLigne += " ";
    chaineLigne += jourSemaine;
    chaineLigne.concat(transformation( " ", jourHeure));// print jour
    chaineLigne.concat(transformation( " ", moisMinute));;// print mois
    chaineLigne.concat(transformation( " 20", anneeSeconde));// print année depuis 1970
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


