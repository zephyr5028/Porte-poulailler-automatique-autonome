/* LcdDigoleI2C.h
  définitions de la classe LCD Digole I2C
  afficheur lcd 2*16 caractères  avec liaison serie i2c digoleSerial
*/
/*
  #include <string.h>
  using namespace std;
*/
#include "LcdDigoleI2C.h"

//constructeur avec debug
// I2C:Arduino UNO: SDA (data line) is on analog input pin 4, and SCL (clock line) is on analog input pin 5 on UNO and Duemilanove
LcdDigoleI2C::LcdDigoleI2C ( TwoWire*, char , byte taille, const boolean debug) : DigoleSerialDisp (&Wire, '\x27'), m_taille(taille), m_debug(debug)
{
  m_chaineLigne[m_taille] = "";// initialisation du tableau
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
void LcdDigoleI2C::affichageUneLigne(char chaine[]) {
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

//----remplissage de la ligne----
void LcdDigoleI2C::remplissageLigne(char chaine1[]) {
  strcat(m_chaineLigne, chaine1);
}

//----affichage-----
void LcdDigoleI2C::affichage() {
  setPrintPos(0, 1);//curseur position 0 ligne 1
  print("                 "); //display space, use to clear the demo line
  for (byte i = 0; i <= 15; i++)  {  //move string to right
    setPrintPos(i, 1); // ligne 1
    print(m_chaineLigne[i]);
  }
  drawStr(0, 1, ""); // curseur position 0 ligne 1
  // strcpy(m_chaineLigne, "");// effacement du tableau
}

//-----affichage de la date-----
void LcdDigoleI2C::affichageDate(char jourSemaine[], byte jour, byte mois, byte annee) {
  // affichage du jour de la semaine
  char m_chaine[16] = "";
  strcat(m_chaine, " ");
  strcat(m_chaine, jourSemaine);
  strcat(m_chaine, " ");

  if (jour < 10) {
    strcat(m_chaine, "0"); // si < 10
  }
  char valeur_temp[5];
  sprintf(valeur_temp, "%u", jour);
  strcat(m_chaine, valeur_temp); // print jour

  /*
    string prenom("Albert");
     string nom("Einstein");

     string total;    //Une chaîne vide
     total += prenom; //On ajoute le prénom à la chaîne vide
     total += " ";    //Puis un espace
     total += nom;    //Et finalement le nom de famille
  */

  //    strcat(m_chaine, transformation ("0", jour));

  strcat(m_chaine, " ");

  if (mois < 10) {
    strcat(m_chaine, "0");  // si < 10
  }
  // char valeur_temp[5];
  sprintf(valeur_temp, "%u", mois); // print mois
  strcat(m_chaine, valeur_temp);

  strcat(m_chaine, " 20");

  if (annee < 10) {
    strcat(m_chaine, " "); // si < 10
  }
  sprintf(valeur_temp, "%u", annee - 30); // print année depuis 1970
  strcat(m_chaine, valeur_temp);

 // Serial.println(m_chaine);
  affichageUneLigne(m_chaine);
  //  strcpy(m_chaineLigne, "");// effacement du tableau
  /*
    // char m_chaine[16] = "";

    strcat(m_chaineLigne, " ");
    strcat(m_chaineLigne, jourSemaine);
    strcat(m_chaineLigne, " ");

    if (jour < 10) {
    strcat(m_chaineLigne, "0"); // si < 10
    }
    char valeur_temp[5];
    sprintf(valeur_temp, "%u", jour);
    strcat(m_chaineLigne, valeur_temp); // print jour

    //   char chaine1[7]="";
    //  chaine1 = transformation ("0", jour);
    //  strcat(m_chaine, chaine1);
    strcat(m_chaineLigne, " ");

    if (mois < 10) {
    strcat(m_chaineLigne, "0");  // si < 10
    }
    //  char valeur_temp[5];
    sprintf(valeur_temp, "%u", mois); // print mois
    strcat(m_chaineLigne, valeur_temp);

    strcat(m_chaineLigne, " 20");

    if (annee < 10) {
    strcat(m_chaineLigne, " "); // si < 10
    }
    sprintf(valeur_temp, "%u", annee - 30); // print année depuis 1970
    strcat(m_chaineLigne, valeur_temp);
  */
  // strcat(m_chaineLigne, "abcdefghijklmnop");
  // Serial.println(m_chaineLigne);
  // affichage();
}

// transformation donnees date et heure
char transformation (char chaine, byte dateHeure ) {
  char m_chaine[7] = "";
  /* if (dateHeure < 10) {
     strcat(m_chaine, chaine); // si < 10
    }
    char valeur_temp[5];
    sprintf(valeur_temp, "%u", dateHeure);
    strcat(m_chaine, valeur_temp); // print jour
  */
  // return m_chaine;
  byte length(7);
  String myArray[length];
  myArray[0] = String("text");
  return myArray;

}


