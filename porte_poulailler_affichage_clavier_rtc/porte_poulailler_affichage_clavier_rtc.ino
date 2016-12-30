/* porte-poulailler : affichage + clavier + rtc */

/*
  28 12 2016 classe Lumiere - ok
  26 12 2016 classe Accus - ok
  21 12 2016 ajout de la classe servo - ok
  01 12 2016first commit sur gihub
  20 09 2016 classe clavier

  19 septembre 2016 :
    - suppression du blocage au demarrage (si = 200)
  18 septembre 2016 :
    En cas de probleme avec le sens de rotation du servo :
     - variable globale : sens
     - arret fin de course ( contact) quelque soit le sens
     - diminution de la vitesse reduite dans la descente de la trappe (inertie)
  17 septembre 2016 :
    - problème avec servotimer2  :
       Le pulse en ms ne fonctionne pas donc le servo est tjs dans le même sens (hs).
       Les valeurs du pulse étaient mauvaise.
       Il faut :
               90° : 1500ms
                0° : 1000ms
              180° : 2000ms
  10 septembre 2016 :
    - affichage compteur roue codeuse envoi radio
    - delay d'attente de la remontée du servo augmentée : 300
  Important :
  with ServoTimer2 et VirtualWire
  il faut commenter typedef dans la bibliothèque servotimer2
*/

/*-----debug-----*/
const boolean DEBUG = false; // positionner debug pour l'utiliser ou pas
const boolean RADIO = true; // positionner radio pour l'utiliser ou pas
const bool TESTSERVO = false; // pour utiliser ou non le test du servomoteur
const bool TEMPERATURE = true; // true = celsius , false = fahrenheit

/*------Bibliothèque Flash pour mise en mémoire flash  F()--------*/
#include <Flash.h>
#include <avr/pgmspace.h> // non nécessaire maintenant

/* radio 43MHz */
#include "Radio.h"
const byte pinEmRadio = 10; // pin D10 emetteur radio
const int vitesseTransmission(600);//nitialisation de la bibliothèque avec la vitesse (vitesse_bps)
Radio radio(pinEmRadio, vitesseTransmission, VW_MAX_MESSAGE_LEN, RADIO, DEBUG); // classe Radio

/* affichages */
#define LCDCol 16
#define LCDRow 2

/* led broche 13 */
#define LED_PIN 13

/* servo - montée et descente de la porte */
#include "ServoMoteur.h"
const byte servoCde (8); // pin D8 cde du servo
const byte servoPin (4); // pin D4 relais du servo
const byte securiteHaute(12); // pin 12 pour la securite d'ouverture de porte
const int pulseStop (1500); // value should usually be 750 to 2200 (1500 = stop)
bool reduit(false); // vitesse du servo, normal ou reduit(false)
// pulse stop, ouverture/fermeture , reduit et debug si nécessaire
ServoMoteur monServo(servoCde, servoPin, securiteHaute, pulseStop, 140, 70, DEBUG);

/*Accus*/
#include "Accus.h"
const byte accusPinCde(2); //analog pin A2 : tension batterie commandes
const byte accusPinServo(3); //analog pin A3 : tension batterie servo moteur
const float tensionMiniAccus(4.8); //valeur minimum de l'accu 4.8v
const float rapportConvertion(7.5);// rapport de convertion CAD float
boolean batterieFaible = false; // si batterie < 4,8v = true
Accus accusCde (accusPinCde, tensionMiniAccus, rapportConvertion, DEBUG); // objet accus commande mini 4.8v, convertion 7.5
Accus accusServo (accusPinServo); // objet accus servo moteur mini 4.8v, convertion 7.5

/* RTC_DS3231 */
const byte rtcINT = 5; // digital pin D5 as l'interruption du rtc ( alarme)
byte alarm_1 = 1; // alarme 1
byte alarm_2 = 2; //alarme 2

/* roue codeuse */
#include "Codeur.h"
// digital pin D7 has a détecteur roue codeuse
const byte roueCodeuse(7);//digital pin D7 pour entrée roue codeuse
volatile unsigned int compteRoueCodeuse(150);  // un compteur de position
unsigned int finDeCourseFermeture(250); // initialisation de la valeur de la fin de course fermeture
unsigned int finDeCourseOuverture(150); // initialisation de la valeur de la fin de course ouverture
volatile boolean interruptRoueCodeuse;    // gestion de l'anti-rebonds
volatile boolean positionRoueCodeuse; // gestion de l'anti-rebonds
Codeur codOpt (roueCodeuse, finDeCourseFermeture, finDeCourseOuverture, compteRoueCodeuse, DEBUG); // objet codeur optique

/* watchdog - Optimisation de la consommation */
#include <avr/power.h>
#include <avr/sleep.h>
#include <avr/wdt.h>
volatile int f_wdt = 1; // flag watchdog

/****************************/
const byte bouclesWatchdog(2); // 8 nombre de boucles du watchdog environ 64s
/****************************/
byte tempsWatchdog = bouclesWatchdog; // boucle temps du chien de garde

/* Affichage */
byte incrementation = 0; // incrementation verticale
byte decalage = 0; // decalage à droite pour reglage
boolean reglage = false; // menu=false ou reglage=true
const int boucleTemps(200); // 500 temps entre deux affichages de l'heure
int temps(0);
const int debounce = 350; // debounce latency in ms
unsigned long tempoDebounce(debounce); // temporisation pour debounce
boolean relacheBpOF = true; // relache Bp
byte BpOF = 9; // pin D9 bouton poussoir ouverture / fermeture
byte interOuvBoitier = 6; //pin D6 interrupteur ouverture boitier
boolean retroEclairage = true; // etat retro eclairage
boolean  boitierOuvert = true; // le boitier est ouvert

/* progmem  mémoire flash */
const char listeDayWeek[] PROGMEM = "DimLunMarMerJeuVenSam"; // day of week en mémoire flash
const char affichageMenu[] PROGMEM = "      Date      .      Heure     . Heure Ouverture. Heure Fermeture.  Temperature   .     Lumiere    .  Lumiere matin .  Lumiere soir  . Choix Ouv/Ferm . Fin de course  . Fin de course  . Volt bat Cdes  . Volt bat servo . Commande Manuel.";
const char affichageRadio[] PROGMEM = "Batterie faible !!!!.Temp = .Time : .Lum : .Fin course F = .Fin course O = .Ten Cdes = . Ten Servo = .Porte ouv !.Porte ferm !.";

/* lumiere */
#include "Lumiere.h"
const byte lumierePin(0); //analog pin A0 : luminosite
unsigned int lumMatin(300); // valeur de la lumière du matin
unsigned int lumSoir(900); // valeur de la lumiere du soir
const float convertion(5);// rapport de convertion CAD float
Lumiere lum(lumierePin, lumMatin, lumSoir, convertion, DEBUG); // objet lumiere

/* interruptions */
volatile boolean interruptBp = false; // etat interruption entree 9
volatile boolean interruptRTC = false; // etat interruption entree 10
volatile boolean interruptOuvBoi = false; // etat interruption entree 6

/* menus */
const byte menuDate = 1;
const byte menuHeure = 2;
const byte menuOuverture = 3;
const byte menuFermeture = 4;
const byte menuTemperature = 5;
const byte menuLumiere = 6;
const byte menuLumiereMatin = 7;
const byte menuLumiereSoir = 8;
const byte menuChoix = 9;
const byte menuFinDeCourseFermeture = 10;
const byte menuFinDeCourseOuverture = 11;
const byte menuTensionBatCdes = 12; // tension batterie commandes
const byte menuTensionBatServo = 13; // tension batterie servo
const byte menuManuel = 14; // nombre de lignes du  menu
const byte colonnes = 16; // colonnes de l'afficheur

/* Clavier */
#include "Clavier.h"
const byte oldkey(-1);
const byte sensorClavier(1); //pin A1 pour le clavier
boolean relache(false); // relache de la touche
byte touche(-1); // valeur de la touche appuyee
Clavier clav(menuManuel); // class Clavier avec le nombre de lignes du menu

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

//------UART setup----
#if defined(_Digole_Serial_UART_)
DigoleSerialDisp mydisp(&Serial, 9600); // UART:Arduino UNO: Pin 1(TX)on arduino to RX on module
#endif

//------I2C setup----
#if defined(_Digole_Serial_I2C_)
#include <Wire.h>
DigoleSerialDisp mydisp(&Wire, '\x27'); // I2C:Arduino UNO: SDA (data line) is on analog input pin 4, and SCL (clock line) is on analog input pin 5 on UNO and Duemilanove
#endif

//------SPI setup----
#if defined(_Digole_Serial_SPI_)
DigoleSerialDisp mydisp(8, 9, 10); // SPI:Pin 8:data, 9:clock, 10:SS, you can assign 255 to SS, and hard ground SS pin on module
#endif

/* RTC_DS3231 */
#include <DS3232RTC.h>    //http://github.com/JChristensen/DS3232RTC
#include <Time.h>         //http://www.arduino.cc/playground/Code/Time  
#define DS3231_I2C_ADDRESS 0x68
tmElements_t tm; // declaration de tm pour la lecture des informations date et heure

//-----routine decToBcd : Convert normal decimal numbers to binary coded decimal-----
byte decToBcd(byte val) {
  return ( (val / 10 * 16) + (val % 10) );
}

//-----routine bcdToDec : Convert binary coded decimal to normal decimal numbers-----
byte bcdToDec(byte val) {
  return ( (val / 16 * 10) + (val % 16) );
}

/* eeprom at24c32 */
//-----ecriture dans l'eeprom at24c32 de la carte rtc------
void i2c_eeprom_write_byte( int deviceaddress, unsigned int eeaddress, byte data ) {
  int rdata = data;
  Wire.beginTransmission(deviceaddress);   // adresse 0x57 pour l'i2c de l'eeprom de la carte rtc
  Wire.write((int)(eeaddress >> 8)); // MSB
  Wire.write((int)(eeaddress & 0xFF)); // LSB
  Wire.write(rdata);
  Wire.endTransmission();
}

//-----lecture de l'eeprom at24c32 de la carte rtc------
byte i2c_eeprom_read_byte( int deviceaddress, unsigned int eeaddress ) {
  byte rdata = 0xFF;
  Wire.beginTransmission(deviceaddress); // adresse 0x57 pour l'i2c de l'eeprom de la carte rtc
  Wire.write((int)(eeaddress >> 8)); // MSB
  Wire.write((int)(eeaddress & 0xFF)); // LSB
  Wire.endTransmission();
  Wire.requestFrom(deviceaddress, 1);
  if (Wire.available()) rdata = Wire.read();
  return rdata;
}

/* clavier */
//-----lecture clavier------
void  lectureClavier() {
  if (boitierOuvert) {
    touche = clav.read_key(sensorClavier); // read key sensor = A1
    if (touche == oldkey ) { // si touche == -1, donc touche relache=true
      relache = true;
    }
    if ((touche == 2 or touche == 3) and relache and !reglage) { // si appui sur les touches 2 ou 3 et la touche relache=true et mode reglage
      relache = false;
      incrementation = clav.positionMenu(incrementation, touche); // position du menu pour l'affichage
      deroulementMenu (incrementation); // affichage du menu
    }
  }
}

//----routine lecture et ecriture date and time-----
void ecritureDateTime() {
  RTC.write(tm);
  if (incrementation == menuHeure) {
    mydisp.drawStr(0, 1, "   ");
    displayTime(); // affichage de l'heure
  }
  if (incrementation == menuDate) {
    mydisp.drawStr(0, 1, " ");
    displayDate(); // affichage de la date
  }
  mydisp.drawStr(decalage, 1, ""); // curseur position : decalage, ligne 1
}

/* affichages */
//-----routine display Date-----
void displayDate() {
  if ( boitierOuvert) { // si le boitier est ouvert
    RTC.read(tm); // lecture date et heure
    // affichage du jour de la semaine
    byte j = ((tm.Wday - 1) * 3);
    for (byte i = j; i < j + 3; i++) {
      char dayWeek = pgm_read_byte(listeDayWeek + i);
      mydisp.print(dayWeek);
    }
    mydisp.print(F(" "));
    if (tm.Day < 10) {
      mydisp.print(F("0"));  // si < 10
    }
    mydisp.print(tm.Day, DEC); // print jour
    mydisp.print(F(" "));
    if (tm.Month < 10) {
      mydisp.print(F("0"));  // si < 10
    }
    mydisp.print(tm.Month, DEC); // print mois
    mydisp.print(F(" 20"));
    if (tm.Year < 10) {
      mydisp.print(F("0"));  // si < 10
    }
    mydisp.print(tm.Year - 30, DEC); // print année depuis 1970
    mydisp.print(F(" "));
    mydisp.drawStr(decalage, 1, ""); // curseur position 0 ligne 1
  }
}

//-----routine display Time-----
void displayTime () {
  if ( boitierOuvert) { // si le boitier est ouvert
    RTC.read(tm); // lecture date et heure
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
  }
  if (DEBUG or RADIO) {
    byte timeHour =  bcdToDec(RTC.readRTC(0x02) & 0x3f); // heure
    byte timeMinute = bcdToDec(RTC.readRTC(0x01)); // minutes
    if (DEBUG) {
      Serial.print(F("Time : "));
      Serial.print(timeHour, DEC);
      Serial.print(F("h "));
      Serial.print(timeMinute, DEC);
      Serial.println(F("m"));
    }
    radio.envoiUnsignedInt(timeHour,  boitierOuvert, "h");// envoi message radio heure si le boitier est fermé
    radio.envoiUnsignedInt(timeMinute,  boitierOuvert, "m;");// envoi message radio minute si le boitier est fermé
  }
}

//----routine door open time-----
void openTime() {
  if ( boitierOuvert) { // si le boitier est ouvert
    byte val;
    val = bcdToDec(RTC.readRTC(0x09) & 0x3f); // alarme 1 hours
    mydisp.drawStr(0, 1, "   ");
    if (val < 10) {
      mydisp.print(F("0"));  // si < 10
    }
    mydisp.print(val, DEC);
    mydisp.print(F("h "));
    val = bcdToDec(RTC.readRTC(0x08)); // alarme 1 minutes
    if (val < 10) {
      mydisp.print(F("0"));  // si < 10
    }
    mydisp.print(val, DEC);
    mydisp.print(F("m "));
    val = bcdToDec(RTC.readRTC(0x07) & 0x7f); // alarme 1 seconds
    if (val < 10) {
      mydisp.print(F("0"));  // si < 10
    }
    mydisp.print(val, DEC);
    mydisp.print(F("s   "));
    mydisp.drawStr(decalage, 1, ""); // curseur position 0 ligne 1
  }
}

//-----routine door close time-----
void closeTime() {
  if ( boitierOuvert) { // si le boitier est ouvert
    //Set Alarm2
    byte val;
    val = bcdToDec(RTC.readRTC(0x0C) & 0x3f); // alarme 2 hours
    mydisp.drawStr(0, 1, "   ");
    if (val < 10) {
      mydisp.print(F("0"));  // si < 10
    }
    mydisp.print(val, DEC);
    mydisp.print(F("h "));
    val = bcdToDec(RTC.readRTC(0x0B)); //alarme 2 minutes
    if (val < 10) {
      mydisp.print(F("0"));  // si < 10
    }
    mydisp.print(val, DEC);
    mydisp.print(F("m      "));
    mydisp.drawStr(decalage, 1, ""); // curseur position 0 ligne 1
  }
}

//------affichage pulse et comptage roue codeuse------
void affiPulsePlusCptRoue() {
  int pulse = monServo.get_m_pulse();
  byte test(0);
  test = codOpt.testCompteurRoueCodeuse (5); // tolerance de 5
  unsigned int compteRoueCodeuse = codOpt.get_m_compteRoueCodeuse();
  if ( boitierOuvert) { // si le boitier est ouvert
    mydisp.print(F("P: "));
    mydisp.print(pulse);
    mydisp.print(F("  R: "));
    mydisp.print(compteRoueCodeuse);
    mydisp.print(F(" "));
    mydisp.drawStr(0, 0, "");
  }
  if (DEBUG) {
    Serial.print(F("Pulse servo = "));
    Serial.println(pulse);
    Serial.print(F("Cpt roue codeuse = "));
    Serial.println(compteRoueCodeuse);
    switch (test) {
      case 1: // mise sous tension du servo pour l'ouverture de la porte
        Serial.println(F("Porte fermee"));
        break;
      case 2: // mise sous tension du servo pour la fermeture de la porte
        Serial.println(F("Porte ouverte"));
        break;
      default:
        Serial.println(F("Porte    "));
        break;
    }

    /*   if ( compteRoueCodeuse > (finDeCourseFermeture - 5) and compteRoueCodeuse < (finDeCourseFermeture + 5)) {
         Serial.println(F("Porte fermee"));
       }
       if ( compteRoueCodeuse > (finDeCourseOuverture - 5) and compteRoueCodeuse < (finDeCourseOuverture + 5)) {
         Serial.println(F("Porte ouverte"));
       }*/
  }
  if (RADIO and tempsWatchdog <= 0 and (!boitierOuvert)) { // eviter l'envoi à l'initialisation
    char chaine1[VW_MAX_MESSAGE_LEN - 1] = "";
    switch (test) {
      case 1: // mise sous tension du servo pour l'ouverture de la porte
        //  char chaine1[VW_MAX_MESSAGE_LEN - 1] = "";
        strcat(chaine1, "fer;");
        // radio.envoiMessage(chaine1);// on envoie le message
        break;
      case 2: // mise sous tension du servo pour la fermeture de la porte
        //  char chaine1[VW_MAX_MESSAGE_LEN - 1] = "";
        strcat(chaine1, "ouv;");
        // radio.envoiMessage(chaine1);// on envoie le message
        break;
      default:
        strcat(chaine1, "   ;");
        break;
    }
    /*    if ( compteRoueCodeuse > (finDeCourseFermeture - 5) and compteRoueCodeuse < (finDeCourseFermeture + 5)) {
          char chaine1[VW_MAX_MESSAGE_LEN - 1] = "";
          strcat(chaine1, "fer;");
          radio.envoiMessage(chaine1);// on envoie le message
        }
        if ( compteRoueCodeuse > (finDeCourseOuverture - 5) and compteRoueCodeuse < (finDeCourseOuverture + 5)) {
          char chaine1[VW_MAX_MESSAGE_LEN - 1] = "";
          strcat(chaine1, "ouv;");
          radio.envoiMessage(chaine1);// on envoie le message
        }*/
    radio.envoiMessage(chaine1);// on envoie le message
    radio.envoiUnsignedInt(compteRoueCodeuse, boitierOuvert, ";"); // envoi message radio compteur roue codeuse
  }
}

/* afficheur */
//-----retro eclairage de l'afficheur-----
void eclairageAfficheur() {
  if (boitierOuvert) {
    if (touche == 5 and relache == true ) { // retro eclairage si appuis sur la touche 5
      relache = false;
      if (retroEclairage)  {
        mydisp.backLightOn(); // retro eclairage on
        retroEclairage = false;
      } else {
        mydisp.backLightOff(); // retro eclairage off
        retroEclairage = true;
      }
    }
  }
}

//---- temporisation pour l'affichage, affichage du menu lorsque temps est >....
void temporisationAffichage(int boucleTemps) {
  if (  temps > boucleTemps) {
    deroulementMenu (incrementation); // affichage du menu en fonction de incrementation
    temps = 0;
  } else {
    temps = temps + 1;
  }
}

/* batteries */
//-------affichage tension batterie commandes
void affiTensionBatCdes() {
  int valBatCdes = accusCde.tensionAccusCAD(); // tension batterie CAD
  float voltage = accusCde.tensionAccus(valBatCdes);// read the input on analog pin A2 : tension batterie commandes
  // print out the value you read:
  if ( boitierOuvert) { // si le boitier est ouvert
    mydisp.print(F(" "));
    mydisp.print(valBatCdes);
    mydisp.print(F(" = "));
    mydisp.print(voltage);
    mydisp.print(F("V    "));
    mydisp.drawStr(0, 0, "");
  }
  if (DEBUG) {
    Serial.print(F("Ten bat cdes = "));
    Serial.print(valBatCdes);
    Serial.print(F(" = "));
    Serial.print(voltage);
    Serial.println(F(" V"));
  }
  radio.envoiFloat(voltage, boitierOuvert, "V;" ); // envoi message radio tension accus}*/
}

//-------affichage tension batterie servo-moteur
void affiTensionBatServo() {
  int valBatServo = accusServo.tensionAccusCAD(); // tension batterie CAD
  float voltage = accusServo.tensionAccus(valBatServo);// read the input on analog pin A3 : tension batterie servo moteur
  // print out the value you read:
  if ( boitierOuvert) { // si le boitier est ouvert
    mydisp.print(F(" "));
    mydisp.print(valBatServo); //print tension batterie servo moteur
    mydisp.print(F(" = "));
    mydisp.print(voltage);
    mydisp.print(F("V    "));
    mydisp.drawStr(0, 0, "");
  }
  if (DEBUG) {
    Serial.print(F("Ten bat servo = "));
    Serial.print(valBatServo);
    Serial.print(F(" = "));
    Serial.print(voltage);
    Serial.println(F(" V"));
  }
  radio.envoiFloat(voltage, boitierOuvert, "V;" ); // envoi message radio tension accus
}

/* choix pour l'ouverture et la fermeture :
     - heure
     - lumiere
*/
//------affichage du choix de l'ouverture et la fermeture------
void affiChoixOuvFerm() {
  if ( boitierOuvert) { // si le boitier est ouvert
    mydisp.print(F(" Ouv:"));
    if (lum.get_m_ouverture()) {
      mydisp.print(F("Hre"));
    } else {
      mydisp.print(F("Lum"));
    }
    mydisp.print(F(" "));
    mydisp.print(F("Fer:"));
    if (!lum.get_m_fermeture()) {
      mydisp.print(F("Lum"));
    } else {
      mydisp.print(F("Hre"));
    }
    mydisp.drawStr(decalage, 1, "");
  }
  if (DEBUG) {
    Serial.print(F("Ouv : "));
    if (lum.get_m_ouverture()) {
      Serial.print(F("Hre"));
    } else {
      Serial.print(F("Lum "));
    }
    Serial.print(F("   Fer : "));
    if (!lum.get_m_fermeture()) {
      Serial.println(F("Lum "));
    } else {
      Serial.println(F("Hre"));
    }
  }
}

//-----routine choix ouverture fermeture-----
void choixOuvFerm () {
  if (boitierOuvert) {
    if (touche == 4 and relache == true and incrementation == menuChoix) {
      relache = false;
      if (decalage < 15 ) { // boucle de reglage
        decalage = decalage + 7;   // incrementation decalage
        reglage = true; // reglages
      }
      if (decalage > 14 ) { // fin de la ligne d'affichage si >14
        decalage = 0;
        reglage = false;
      }
      mydisp.drawStr(decalage, 1, ""); // curseur position : decalage, ligne 1
    }
    if ((touche == 2 or touche == 3) and incrementation == menuChoix and relache == true and reglage == true ) { // si appui sur les touches 2 ou 3 pour reglage des valeurs
      relache = false;
      if (decalage == 7) {
        if (lum.get_m_ouverture()) {
          lum.set_m_ouverture(0); // ouverture 0 donc lumière
          RTC.alarmInterrupt(alarm_1, false);     //disable Alarm1
        } else {
          lum.set_m_ouverture(1); // ouverture 1 donc heure
          RTC.alarmInterrupt(alarm_1, true); // activation alarme 1 ouverture
        }
        i2c_eeprom_write_byte(0x57, 0x14, lum.get_m_ouverture()); // écriture du type d'ouverture @14 de l'eeprom de la carte rtc (i2c @ 0x57)
        mydisp.drawStr(0, 1, "");
        affiChoixOuvFerm();
        mydisp.drawStr(decalage, 1, ""); // curseur position : decalage, ligne 1
      }
      if (decalage == 14) {
        if (!lum.get_m_fermeture()) {
          lum.set_m_fermeture(1); // fermeture 1 donc heure
          RTC.alarmInterrupt(alarm_2, true);// activation alarme 2 fermeture
        } else {
          lum.set_m_fermeture(0); // fermeture 0 donc lumiere
          RTC.alarmInterrupt(alarm_2, false);     //disable Alarm2
        }
        i2c_eeprom_write_byte(0x57, 0x15, lum.get_m_fermeture()); // écriture du type de fermeture @15 de l'eeprom de la carte rtc (i2c @ 0x57)
        mydisp.drawStr(0, 1, "");
        affiChoixOuvFerm();
        mydisp.drawStr(decalage, 1, ""); // curseur position : decalage, ligne 1
      }
    }
  }
}

/* reglage heure fermeture */
//-----reglage de l'heure de fermeture------
void reglageHeureFermeture() {
  if (boitierOuvert) {
    if (touche == 4 and relache == true and incrementation == menuFermeture) {
      relache = false;
      if (decalage < 10 ) { // boucle de reglage
        decalage = decalage + 4;   // incrementation decalage
        reglage = true; // reglages
      }
      if (decalage > 10 ) { // fin de la ligne d'affichage si >10
        decalage = 0;
        reglage = false;
      }
      mydisp.drawStr(decalage, 1, ""); // curseur position : decalage, ligne 1
    }
    if ((touche == 2 or touche == 3) and incrementation == menuFermeture and relache == true and reglage == true ) { // si appui sur les touches 2 ou 3 pour reglage des valeurs
      relache = false;
      byte alarm2Hour =  bcdToDec(RTC.readRTC(0x0C) & 0x3f); // alarme 2 hours
      byte alarm2Minute = bcdToDec(RTC.readRTC(0x0B)); // alarme 2 minutes
      if (decalage == 4) {
        if (touche == 2 ) {
          if (alarm2Hour < 24) {
            alarm2Hour++; // incrementation de l heure
          } else {
            alarm2Hour = 0;
          }
        }
        if (touche == 3 ) {
          if (alarm2Hour > 0) {
            alarm2Hour--; // decrementation de l heure
          } else {
            alarm2Hour = 24;
          }
        }
        RTC.setAlarm(ALM2_MATCH_HOURS, alarm2Minute, alarm2Hour, 0); // écriture de l'heure alarme 2
        mydisp.drawStr(0, 1, "   ");
        closeTime(); // affichage de l'heure d'ouverture
        mydisp.drawStr(decalage, 1, ""); // curseur position : decalage, ligne 1
      }
      if (decalage == 8) {
        if (touche == 2) {
          if (alarm2Minute < 59) {
            alarm2Minute++; // incrementation des minutes
          } else {
            alarm2Minute = 0;
          }
        }
        if (touche == 3 ) {
          if (alarm2Minute > 0) {
            alarm2Minute--; // decrementation de l'heure
          } else {
            alarm2Minute = 59;
          }
        }
        RTC.setAlarm(ALM2_MATCH_HOURS, alarm2Minute, alarm2Hour, 0);  // écriture de l'heure alarme 2
        mydisp.drawStr(0, 1, "   ");
        closeTime(); // affichage de l'heure d'ouverture
        mydisp.drawStr(decalage, 1, ""); // curseur position : decalage, ligne 1
      }
    }
  }
}

/* reglage heure ouverture */
//-----routine de reglage de l'heure d'ouverture-----
void reglageHeureOuverture() {
  if (boitierOuvert) {
    if (touche == 4 and relache == true and incrementation == menuOuverture) {
      relache = false;
      if (decalage < 14 ) { // boucle de reglage
        decalage = decalage + 4;   // incrementation decalage
        reglage = true; // reglages
      }
      if (decalage > 14 ) { // fin de la ligne d'affichage si >14
        decalage = 0;
        reglage = false;
      }
      mydisp.drawStr(decalage, 1, ""); // curseur position : decalage, ligne 1
    }
    // Set Alarm1
    if ((touche == 2 or touche == 3) and incrementation == menuOuverture and relache == true and reglage == true ) { // si appui sur les touches 2 ou 3 pour reglage des valeurs
      relache = false;
      byte alarm1Hour =  bcdToDec(RTC.readRTC(0x09) & 0x3f); // alarme 1 hours
      byte alarm1Minute = bcdToDec(RTC.readRTC(0x08)); // alarme 1 minutes
      byte alarm1Second =  bcdToDec(RTC.readRTC(0x07) & 0x7f); // alarme 1 seconds
      if (decalage == 4) {
        if (touche == 2 ) {
          if (alarm1Hour < 24) {
            alarm1Hour++; // incrementation de l heure
          } else {
            alarm1Hour = 0;
          }
        }
        if (touche == 3 ) {
          if (alarm1Hour > 0) {
            alarm1Hour--; // decrementation de l heure
          } else {
            alarm1Hour = 24;
          }
        }
        RTC.setAlarm(ALM1_MATCH_HOURS, alarm1Second, alarm1Minute, alarm1Hour, 0); // écriture alarm 1
        mydisp.drawStr(0, 1, "   ");
        openTime(); // affichage de l'heure d'ouverture
        mydisp.drawStr(decalage, 1, ""); // curseur position : decalage, ligne 1
      }
      if (decalage == 8) {
        // byte alarm1Minute = bcdToDec(RTC.readRTC(0x08)); // alarme 1 minutes
        if (touche == 2) {
          if (alarm1Minute < 59) {
            alarm1Minute++; // incrementation des minutes
          } else {
            alarm1Minute = 0;
          }
        }
        if (touche == 3 ) {
          if (alarm1Minute > 0) {
            alarm1Minute--; // decrementation de l heure
          } else {
            alarm1Minute = 59;
          }
        }
        RTC.setAlarm(ALM1_MATCH_HOURS, alarm1Second, alarm1Minute, alarm1Hour, 0); // écriture alarm 1
        mydisp.drawStr(0, 1, "   ");
        openTime(); // affichage de l'heure d'ouverture
        mydisp.drawStr(decalage, 1, ""); // curseur position : decalage, ligne 1
      }
      if (decalage == 12) {
        if (touche == 2 ) {
          if (alarm1Second < 59) {
            alarm1Second++; // incrementation des secondes
          } else {
            alarm1Second = 0;
          }
        }
        if (touche == 3) {
          if ( alarm1Second > 0) {
            alarm1Second--; // decrementation des secondes
          } else {
            alarm1Second = 59;
          }
        }
        RTC.setAlarm(ALM1_MATCH_HOURS, alarm1Second, alarm1Minute, alarm1Hour, 0); // écriture alarm 1
        mydisp.drawStr(0, 1, "   ");
        openTime(); // affichage de l'heure d'ouverture
        mydisp.drawStr(decalage, 1, ""); // curseur position : decalage, ligne 1
      }
    }
  }
}

/* reglage de la date */
//----routine reglage jour semaine, jour, mois, annee-----
void reglageDate () {
  if (boitierOuvert) {
    if (touche == 4 and relache == true and incrementation == menuDate) {
      relache = false;
      if (decalage < 14 ) { // boucle de reglage
        decalage = decalage + 3;   // incrementation decalage
        reglage = true; // reglages
      }
      if (decalage > 14 ) { // fin de la ligne d'affichage si >14
        decalage = 0;
        reglage = false;
      }
      mydisp.drawStr(decalage, 1, ""); // curseur position : decalage, ligne 1
    }
    if ((touche == 2 or touche == 3) and incrementation == menuDate and relache == true and reglage == true) { // si appui sur les touches 2 ou 3 pour reglage des valeurs
      relache = false;
      if (decalage == 3) {
        if (touche == 2) {
          if (tm.Wday < 7) {
            mydisp.drawStr(decalage - 2, 1, ""); // curseur position decalage-2, ligne 1
            tm.Wday++; // incrementation du jour de la semaine
          } else {
            mydisp.drawStr(decalage - 2, 1, ""); // curseur position decalage-2, ligne 1
            tm.Wday = 1; // dimanche
          }
        }
        if (touche == 3) {
          if (tm.Wday > 1) {
            mydisp.drawStr(decalage - 2, 1, ""); // curseur position decalage-2, ligne 1
            tm.Wday--; // decrementation du jour de la semaine
          } else {
            mydisp.drawStr(decalage - 2, 1, ""); // curseur position decalage-2, ligne 1
            tm.Wday = 7;
          }
        }
        ecritureDateTime();
      }
      if (decalage == 6) {
        if (touche == 2) {
          if (tm.Day < 31) {
            tm.Day++; // incrementation des jours
          } else {
            tm.Day = 1;
          }
        }
        if (touche == 3 ) {
          mydisp.drawStr(decalage - 1, 1, ""); // curseur position decalage-2, ligne 1
          if (tm.Day > 1) {
            tm.Day--; // decrementation du jour
          } else {
            tm.Day = 31;
          }
        }
        ecritureDateTime();
      }
      if (decalage == 9) {
        if (touche == 2) {
          if (tm.Month < 12) {
            tm.Month++; // incrementation des mois
          } else {
            tm.Month = 1;
          }
        }
        if (touche == 3) {
          if (tm.Month > 1) {
            tm.Month--; // decrementation des mois
          } else {
            tm.Month = 12;
          }
        }
        ecritureDateTime();
      }
      if (decalage == 12 ) {
        if (touche == 2 ) {
          if (tm.Year < 9) {
            mydisp.drawStr(decalage + 1, 1, "0"); // curseur position decalage-2, ligne 1
          } else {
            mydisp.drawStr(decalage + 1, 1, ""); // curseur position decalage-2, ligne 1
          }
          tm.Year++; // incrementation de l'année
        }
        if (touche == 3 ) {
          if (tm.Year < 11) {
            mydisp.drawStr(decalage + 1, 1, "0"); // curseur position decalage-2, ligne 1
          } else {
            mydisp.drawStr(decalage + 1, 1, ""); // curseur position decalage-2, ligne 1
          }
          if (tm.Year > 0) {
            tm.Year--; // decrementation de l'année si > 2000
          }
        }
        ecritureDateTime();
      }
    }
  }
}

/* reglage de la lumiere du matin */
//------affichage de la lumiere du matin------
void affiLumMatin() {
  if (boitierOuvert) {
    unsigned int lumMatin = lum.get_m_lumMatin();
    mydisp.print(F("   Lum: "));
    mydisp.print(lumMatin);
    if (lumMatin == 0) {
      mydisp.print(F("       "));
    } else {
      mydisp.print(F("      "));
    }
    mydisp.drawStr(decalage, 1, "");
  }
}

//-----reglage du choix de la lumiere du matin-------
void choixLumMatin() {
  if (boitierOuvert) {
    if (touche == 4 and relache == true and incrementation == menuLumiereMatin) {
      relache = false;
      if (decalage < 15 ) { // boucle de reglage
        decalage = decalage + 8;   // incrementation decalage
        reglage = true; // reglages
      }
      if (decalage > 14 ) { // fin de la ligne d'affichage si >14
        decalage = 0;
        reglage = false;
      }
      mydisp.drawStr(decalage, 1, ""); // curseur position : decalage, ligne 1
    }
    if ((touche == 2 or touche == 3) and incrementation == menuLumiereMatin and relache == true and reglage == true ) { // si appui sur les touches 2 ou 3 pour reglage des valeurs
      relache = false;
      if (decalage == 8) {
        bool matin(1);
        unsigned int lumMatin = lum.reglageLumiere(matin, touche);// reglage de la lumiere du matin
        byte val1 = lumMatin & 0xFF; // ou    byte val1= lowByte(sensorValue);// pf
        byte val2 = (lumMatin >> 8) & 0xFF; // ou  //byte val1= highByte(sensorValue); // Pf
        i2c_eeprom_write_byte(0x57, 0x16, val1); // écriture de la valeur du reglage de la lumiere du matin low @16  de l'eeprom de la carte rtc (i2c @ 0x57)
        delay(10);
        i2c_eeprom_write_byte(0x57, 0x17, val2); // écriture de la valeur du reglage de la lumiere du matin high @17 de l'eeprom de la carte rtc (i2c @ 0x57)
        delay(10);
        mydisp.drawStr(0, 1, "");
        affiLumMatin();
        mydisp.drawStr(decalage, 1, ""); // curseur position : decalage, ligne 1
      }
    }
  }
}

/* reglage de la lumier du soir  */
//------affichage de la lumiere du soir-------
void affiLumSoir() {
  if (boitierOuvert) {
    unsigned int lumSoir = lum.get_m_lumSoir();
    mydisp.print(F("   Lum: "));
    mydisp.print(lumSoir);
    if (lumSoir == 0) {
      mydisp.print(F("       "));
    } else {
      mydisp.print(F("      "));
    }
    mydisp.drawStr(decalage, 1, "");
  }
}

//------reglage du choix de la lumiere du soir--------
void choixLumSoir() {
  if (boitierOuvert) {
    if (touche == 4 and relache == true and incrementation == menuLumiereSoir) {
      relache = false;
      if (decalage < 15 ) { // boucle de reglage
        decalage = decalage + 8;   // incrementation decalage
        reglage = true; // reglages
      }
      if (decalage > 14 ) { // fin de la ligne d'affichage si >14
        decalage = 0;
        reglage = false;
      }
      mydisp.drawStr(decalage, 1, ""); // curseur position : decalage, ligne 1
    }
    if ((touche == 2 or touche == 3) and incrementation == menuLumiereSoir and relache == true and reglage == true ) { // si appui sur les touches 2 ou 3 pour reglage des valeurs
      relache = false;
      if (decalage == 8) {
        bool soir(0);
        unsigned int lumSoir = lum.reglageLumiere(soir, touche); // reglage de la lumiere du soir
        byte val1 = lumSoir & 0xFF; // ou    byte val1= lowByte(sensorValue); // pf
        byte val2 = (lumSoir >> 8) & 0xFF; // ou  byte val1= highByte(sensorValue); // Pf
        i2c_eeprom_write_byte(0x57, 0x18, val1); // écriture de la valeur du reglage de la lumiere du soir low @18  de l'eeprom de la carte rtc (i2c @ 0x57)
        delay(10);
        i2c_eeprom_write_byte(0x57, 0x19, val2); // écriture de la valeur du reglage de la lumiere du soir high @19 de l'eeprom de la carte rtc (i2c @ 0x57)
        delay(10);
        mydisp.drawStr(0, 1, "");
        affiLumSoir();
        mydisp.drawStr(decalage, 1, ""); // curseur position : decalage, ligne 1
      }
    }
  }
}

/* reglage time */
//-----routine reglage heure , minute , seconde-----
void reglageTime () {
  if (boitierOuvert) {
    if (touche == 4 and relache == true and incrementation == menuHeure) {
      relache = false;
      if (decalage < 14 ) { // boucle de reglage
        decalage = decalage + 4;   // incrementation decalage
        reglage = true; // reglages
      }
      if (decalage > 14 ) { // fin de la ligne d'affichage si >14
        decalage = 0;
        reglage = false;
      }
      mydisp.drawStr(decalage, 1, ""); // curseur position : decalage, ligne 1
    }
    if ((touche == 2 or touche == 3) and incrementation == menuHeure and relache == true and reglage == true ) { // si appui sur les touches 2 ou 3 pour reglage des valeurs
      relache = false;
      if (decalage == 4) {
        if (touche == 2 ) {
          if (tm.Hour < 24) {
            tm.Hour++; // incrementation de l heure
          } else {
            tm.Hour = 0;
          }
        }
        if (touche == 3 ) {
          if (tm.Hour > 0) {
            tm.Hour--; // decrementation de l heure
          } else {
            tm.Hour = 24;
          }
        }
        ecritureDateTime(); // routine écriture date and time
      }
      if (decalage == 8) {
        if (touche == 2) {
          if (tm.Minute < 59) {
            tm.Minute++; // incrementation des minutes
          } else {
            tm.Minute = 0;
          }
        }
        if (touche == 3 ) {
          if (tm.Minute > 0) {
            tm.Minute--; // decrementation de l heure
          } else {
            tm.Minute = 59;
          }
        }
        ecritureDateTime();
      }
      if (decalage == 12) {
        if (touche == 2 ) {
          if (tm.Second < 59) {
            tm.Second++; // incrementation des secondes
          } else {
            tm.Second = 0;
          }
        }
        if (touche == 3) {
          if (tm.Second > 0) {
            tm.Second--; // decrementation des secondes
          } else {
            tm.Second = 59;
          }
        }
        ecritureDateTime();
      }
    }
  }
}

/* fins de course ouverture et fermeture */
//------affichage fin de course Fermeture-----
void affiFinDeCourseFermeture() {
  if ( boitierOuvert) { // si le boitier est ouvert
    unsigned int finDeCourseFermeture = codOpt.get_m_finDeCourseFermeture();
    mydisp.print(F("   Fer : "));
    mydisp.print(finDeCourseFermeture);
    if (finDeCourseFermeture < 10) {
      mydisp.print(F("      "));
    } else {
      mydisp.print(F("     "));
    }
    mydisp.drawStr(decalage, 1, "");
  }
  if (DEBUG) {
    Serial.print(F("Fin course fermeture = ")); Serial.println(finDeCourseFermeture);
  }
  radio.envoiUnsignedInt(finDeCourseFermeture, boitierOuvert, ";"); // envoi message radio fin de course fermeture
}

//------affichage fin de course Ouverture-------
void affiFinDeCourseOuverture() {
  if ( boitierOuvert) { // si le boitier est ouvert
     unsigned int finDeCourseOuverture = codOpt.get_m_finDeCourseOuverture();
    mydisp.print(F("   Ouv : "));
    mydisp.print(finDeCourseOuverture);
    if (finDeCourseOuverture < 10) {
      mydisp.print(F("      "));
    } else {
      mydisp.print(F("     "));
    }
    mydisp.drawStr(decalage, 1, "");
  }
  if (DEBUG) {
    Serial.print(F("Fin course ouverture = ")); Serial.println(finDeCourseOuverture);
  }
  radio.envoiUnsignedInt(finDeCourseOuverture, boitierOuvert, ";"); // envoi message radio fin de course Ouverture
}

//------reglage fin de course Fermeture------
void regFinDeCourseFermeture() {
  if (boitierOuvert) {
    if (touche == 4 and relache == true and incrementation == menuFinDeCourseFermeture) {
      relache = false;
      if (decalage < 15 ) { // boucle de reglage
        decalage += 9;   // incrementation decalage
        reglage = true; // reglages
      }
      if (decalage > 12 ) { // fin de la ligne d'affichage si >12
        decalage = 0;
        reglage = false;
      }
      mydisp.drawStr(decalage, 1, ""); // curseur position : decalage, ligne 1
    }
    if ((touche == 2 or touche == 3) and incrementation == menuFinDeCourseFermeture and relache == true and reglage == true ) { // si appui sur les touches 2 ou 3 pour reglage des valeurs
      relache = false;
      if (decalage == 9) {
        bool fermeture(1);
        unsigned int finDeCourse = codOpt.reglageFinDeCourse(fermeture, touche);// reglage de la fin de course
    /*    if (touche == 2 ) {
          if (finDeCourseFermeture < 500) {
            finDeCourseFermeture++; //incrementation de la fin de course haut
          } else {
            finDeCourseFermeture = 0;
          }
        }
        if (touche == 3 ) {
          if (finDeCourseFermeture > 0) {
            finDeCourseFermeture--; //decrementation de la fin de course haut
          } else {
            finDeCourseFermeture = 500;
          }
        }*/
        byte val1 = finDeCourse & 0xFF; // ou    byte val1= lowByte(sensorValue); // pf
        byte val2 = (finDeCourse >> 8) & 0xFF; // ou  byte val1= highByte(sensorValue); // Pf
        i2c_eeprom_write_byte(0x57, 0x20, val1); // écriture de la valeur du reglage de la fin de course haut low @20  de l'eeprom de la carte rtc (i2c @ 0x57)
        delay(10);
        i2c_eeprom_write_byte(0x57, 0x21, val2); // écriture de la valeur du reglage de la fin de course haut  high @21 de l'eeprom de la carte rtc (i2c @ 0x57)
        delay(10);
        mydisp.drawStr(0, 1, "");
        affiFinDeCourseFermeture();
        mydisp.drawStr(decalage, 1, ""); // curseur position : decalage, ligne 1
      }
    }
  }
}

//-----regalge fin de course ouverture------
void regFinDeCourseOuverture() {
  if (boitierOuvert) {
    if (touche == 4 and relache == true and incrementation == menuFinDeCourseOuverture) {
      relache = false;
      if (decalage < 15 ) { // boucle de reglage
        decalage +=  9;   // incrementation decalage
        reglage = true; // reglages
      }
      if (decalage > 12 ) { // fin de la ligne d'affichage si >12
        decalage = 0;
        reglage = false;
      }
      mydisp.drawStr(decalage, 1, ""); // curseur position : decalage, ligne 1
    }
    if ((touche == 2 or touche == 3) and incrementation == menuFinDeCourseOuverture and relache == true and reglage == true ) { // si appui sur les touches 2 ou 3 pour reglage des valeurs
      relache = false;
      if (decalage == 9) {
        bool ouverture(0);
         unsigned int finDeCourse = codOpt.reglageFinDeCourse(ouverture, touche);// reglage de la fin de course
    /*    if (touche == 2 ) {
          if (finDeCourseOuverture < 500) {
            finDeCourseOuverture++; //incrementation de la fin de course bas
          } else {
            finDeCourseOuverture = 0;
          }
        }
        if (touche == 3 ) {
          if (finDeCourseOuverture > 0) {
            finDeCourseOuverture--; //decrementation de la fin de course bas
          } else {
            finDeCourseOuverture = 500;
          }
        }*/
        byte val1 = finDeCourse & 0xFF; // ou    byte val1= lowByte(sensorValue); // pf
        byte val2 = (finDeCourse >> 8) & 0xFF; // ou  byte val1= highByte(sensorValue); // Pf
        i2c_eeprom_write_byte(0x57, 0x22, val1); // écriture de la valeur du reglage de la fin de course bas low @22  de l'eeprom de la carte rtc (i2c @ 0x57)
        delay(10);
        i2c_eeprom_write_byte(0x57, 0x23, val2); // écriture de la valeur du reglage de la fin de course bas  high @23 de l'eeprom de la carte rtc (i2c @ 0x57)
        delay(10);
        mydisp.drawStr(0, 1, "");
        affiFinDeCourseOuverture();
        mydisp.drawStr(decalage, 1, ""); // curseur position : decalage, ligne 1
      }
    }
  }
}

/* Test du servo avec la console arduino

    a = -1
    q = -1

    z = +10
    s = -10

    e = arret du servo (relais)
    d = marche du servo (relais)

    r = montée
    f = descente

*/
// reglage du servo plus test de la roue codeuse et du servo, à l'aide de la console
void testServo() {
  if (TESTSERVO) {
    int pulse = monServo.get_m_pulse();
    //des données sur la liaison série : (lorsque l'on appuie sur a, q, z, s, e, d )
    if (Serial.available())    {
      char commande = Serial.read(); //on lit
      //on modifie la consigne si c'est un caractère qui nous intéresse
      if (commande == 'a') {
        pulse += 1;  //ajout de 1µs au temps HAUT
        monServo.set_m_pulse(pulse);
        Serial.println(monServo.read());
        monServo.write(pulse); // modification vitesse servo
        Serial.println(monServo.read());
      }
      else if (commande == 'q') {
        pulse -= 1;  //retrait de 1µs au temps HAUT
        monServo.set_m_pulse(pulse);
        Serial.println(monServo.read());
        monServo.write(pulse); // modification vitesse servo
        Serial.println(monServo.read());
      }
      else if (commande == 'z') {
        pulse += 10;  //ajout de 10µs au temps HAUT
        monServo.set_m_pulse(pulse);
        Serial.println(monServo.read());
        monServo.write(pulse); // modification vitesse servo
        Serial.println(monServo.read());
      }
      else if (commande == 's') {
        pulse -= 10;  //retrait de 10µs au temps HAUT
        monServo.set_m_pulse(pulse);
        Serial.println(monServo.read());
        monServo.write(pulse); // modification vitesse servo
        Serial.println(monServo.read());
      }
      else if (commande == 'e') {
        monServo.relaisHorsTension();// mise hors tension du servo
      }
      else if (commande ==  'd') {
        monServo.relaisSousTension();// mise soustension du servo
      }
      else if (commande == 'r' ) {
        // mise sous tension du servo et ouverture de la porte
        monServo.set_m_ouvFerm(true);// ouverture
        monServo.servoOuvFerm(batterieFaible, true);// mise soustension du servo
        delay(50);
        Serial.println(monServo.read());
      }
      else if (commande == 'f') {
        // mise sous tension du servo et descente de la porte
        monServo.set_m_ouvFerm(false);// fermeture
        monServo.servoOuvFerm(batterieFaible, true);// mise soustension du servo
        delay(50);
        Serial.println(monServo.read());
      }
      //et on fait un retour sur la console
      Serial.print("Etat de l'impulsion du servo = ");
      Serial.print(pulse);
      Serial.println(" ms");
    }
  }
}

/* temperature */
//-----routine lecture température sur ds3231 rtc type celsius=true ,fahrenheit=false------
void read_temp(boolean typeTemperature) {
  int temperature = RTC.temperature();
  float celsius = temperature / 4.0;
  float fahrenheit = celsius * 9.0 / 5.0 + 32.0;
  if ( boitierOuvert) { // si le boitier est ouvert
    if (typeTemperature) {
      mydisp.print(celsius); // affichage celsius
      mydisp.print(F(" C     "));
    } else {
      mydisp.print(fahrenheit); // affichage fahrenheit
      mydisp.print(F(" F     "));
    }
  } else {
    if (DEBUG) {
      Serial.print(F("Temp = "));
      if (typeTemperature) {
        Serial.print(celsius); Serial.println(F(" °C"));// affichage celsius
      } else {
        Serial.print(fahrenheit); Serial.println(F(" F"));// affichage fahrenheit
      }
    }
    char temp[2] = "";
    if (typeTemperature)  strcat(temp, "°C;"); else strcat(temp, "F;");
    radio.envoiFloat(celsius, boitierOuvert, temp);
  }
}

/* roue codeuse */
//-----compteur roue codeuse-----
void compteurRoueCodeuse() {
  // debounce
  // if (interruptRoueCodeuse) delay (2);  // attendre
  interruptRoueCodeuse = true; //activation de l'anti-rebond
  bool cpt = digitalRead(roueCodeuse);
  // Confirmation du changement
  if (cpt != positionRoueCodeuse) {
    positionRoueCodeuse = !positionRoueCodeuse;
    //  if (pulse >= pulseStop) {
    if (!monServo.get_m_ouvFerm()) {
      compteRoueCodeuse++;
    } else {
      compteRoueCodeuse--;
    }
    interruptRoueCodeuse = false; //libération de l'anti-rebond
  }
}

/* servomoteur :
    -montée
    -descente
*/
//------sequence ouverture de la porte------
void ouverturePorte() {
  if (monServo.get_m_servoAction() and monServo.get_m_ouvFerm()) {
    if (compteRoueCodeuse <= finDeCourseOuverture + 20) {  // 1 + 20
      reduit = 0;// vitesse reduite
      monServo.servoVitesse( reduit);
    }
    if (!digitalRead(securiteHaute) or (touche == 4 and boitierOuvert)) {
      //  if (touche == 4  and ouvFerm == false and relache == true)  relache = false;
      compteRoueCodeuse = monServo.servoHorsTension(compteRoueCodeuse, finDeCourseOuverture);
    }
  }
}

//-----sequence fermeture de la porte-----
void  fermeturePorte() {
  if (monServo.get_m_servoAction() and !monServo.get_m_ouvFerm()) {
    if (compteRoueCodeuse >= finDeCourseFermeture - 30) { // 195 - 30
      reduit = 0;// vitesse reduite
      monServo.servoVitesse( reduit);
    }
    if ((compteRoueCodeuse >= finDeCourseFermeture)
        or !digitalRead(securiteHaute) or  (touche == 4 and boitierOuvert)) {
      //  if (touche == 4 and ouvFerm == false and relache == true)   relache = false;
      compteRoueCodeuse = monServo.servoHorsTension(compteRoueCodeuse, finDeCourseOuverture);
    }
  }
}

/* interruptions
  -routine interruption D2 INT0
  -routine interruption D3 INT1
  -routine interruption Bp
  -routine alarme 2
  -routine alarme 1
  -routine interruption boitier ouvert
  -test fermeture boitier
*/
//-----routine interruption D2 INT0------
void myInterruptINT0() {
  compteurRoueCodeuse();
}

//-----routine interruption D3 INT1-----
void myInterruptINT1() {
  if (!digitalRead(rtcINT)) { // entree 10 pour interruption RTC
    interruptRTC = true;
  }
  if (!digitalRead(BpOF) and !interruptBp) { // entree 9 pour interruption BP
    interruptBp = true;
    tempoDebounce = millis();
  }
  if (!digitalRead(interOuvBoitier) and !interruptOuvBoi) { // entree 6 pour interruption ouverture boitier
    interruptOuvBoi = true;
  }
}

//-----routine interruption Bp-----
void routineInterruptionBp() {
  if (interruptBp) { // test de l'appui sur bouton bp
    // If the switch changed, due to noise or pressing:
    if (((millis() - tempoDebounce) > debounce)  and  relacheBpOF == true and !digitalRead(BpOF) ) {
      relacheBpOF = false;
      if (monServo.get_m_ouvFerm())  monServo.set_m_ouvFerm(false); else  monServo.set_m_ouvFerm(true);
      reduit = 1;// vitesse normale
      monServo.servoOuvFerm(batterieFaible, reduit);
      tempoDebounce = millis(); // attente relache du bouton
    }
    if (((millis() - tempoDebounce) > debounce)  and  relacheBpOF == false and digitalRead(BpOF) ) {
      interruptBp = false; // autorisation de la prise en compte de l'IT
      relacheBpOF = true;
    }
  }
}

//-----routine alarme 2-----
void  routineInterrruptionAlarme2() {
  if ( RTC.alarm(alarm_2) and interruptRTC ) {    // has Alarm2 (fermeture) triggered?  alarme rtc
    // mise sous tension du servo pour la fermeture de la porte
    monServo.set_m_ouvFerm(false); // fermeture
    // ouvFerm = 0; // fermeture
    reduit = 1;// vitesse normale
    monServo.servoOuvFerm(batterieFaible, reduit);
    interruptRTC = false; // autorisation de la prise en compte de l'IT
  }
}

//-----routine alarme 1-----
void  routineInterruptionAlarme1() {
  if ( RTC.alarm(alarm_1) and interruptRTC ) {    // has Alarm1 (ouverture) triggered?  alarme rtc
    // mise sous tension du servo pour l'ouverture de la porte
    monServo.set_m_ouvFerm(true); // ouverture
    // ouvFerm = 1; // ouverture
    reduit = 1;// vitesse normale
    monServo.servoOuvFerm(batterieFaible, reduit);
    interruptRTC = false; // autorisation de la prise en compte de l'IT
  }
}

//-----routine interruption boitier ouvert------
void  routineInterruptionBoitierOuvert() {
  if ( interruptOuvBoi) {    //  interruption ouverture boitier
    if (!digitalRead(interOuvBoitier) ) { // ouverture boitier
      boitierOuvert = true; // boitier ouvert
    }
  }
}

//-----test fermeture boitier-----
void  routineTestFermetureBoitier() {
  if (digitalRead(interOuvBoitier)) { //  fermeture boitier
    if (!retroEclairage)  {
      mydisp.backLightOff(); // retro eclairage off
      retroEclairage = true;
    }
    boitierOuvert = false; // boitier ferme
    interruptOuvBoi = false; // autorisation de la prise en compte de l'IT
  }
}

/* lumiere
  -affichage
*/
//-----routine lecture et affichage de la lumière-----
void lumiere() {
  int lumValue = lum.luminositeCAD(); // luminosite CAD sur pin A0
  float voltage = lum.tensionLuminosite(lumValue);// convertion CAD  vers tension luminosite
  if ( boitierOuvert) { // si le boitier est ouvert
    // print out the value you read:
    mydisp.print(F(" "));
    mydisp.print(lumValue);
    mydisp.print(F(" = "));
    mydisp.print(voltage);
    mydisp.print(F("V    "));
  }
  if (DEBUG) {
    Serial.print(F("lum : "));
    Serial.println(lumValue);
  }
  radio.envoiUnsignedInt(lum.get_m_lumSoir(), boitierOuvert, ";"); // envoi message radio lumiere du soir
  radio.envoiUnsignedInt(lumValue, boitierOuvert, ";"); // envoi message radio lumiere
}

//-----ouverture/fermeture par test de la lumière----
void ouvFermLum() {
  byte  valHeure = bcdToDec(RTC.readRTC(0x02) & 0x3f); // lecture de l'heure
  lum.testLuminosite(); // test de la luminosite pour mise à jour du compteur watchdog lumiere
  //fenetre de non declenchement pour ne pas declencher la fermeture avant 17h00 et l'ouverture après 17h00 et mise à jour du compteur watchdog lumiere
  lum.fenetreNonDeclenchement(valHeure) ;
  //non eclenchement en fonction de la position du servo et mise à jour du compteur watchdog lumiere
  lum.nonDeclenchementPositionServo (compteRoueCodeuse, finDeCourseFermeture, finDeCourseOuverture);
  byte declenchementLuminosite = lum.declenchementServoLuminosite(); // test de laluninosite et declenchement du servo
  switch (declenchementLuminosite) {
    case 1: // mise sous tension du servo pour l'ouverture de la porte
      monServo.set_m_ouvFerm(true); // ouverture
      reduit = 1;// vitesse normale
      monServo.servoOuvFerm(batterieFaible, reduit);
      break;
    case 2: // mise sous tension du servo pour la fermeture de la porte
      monServo.set_m_ouvFerm(false); // fermeture
      reduit = 1;// vitesse normale
      monServo.servoOuvFerm(batterieFaible, reduit);
      break;
  }
}

/* menu */
//-----routine affichage menus------
void deroulementMenu (byte increment) {
  if (boitierOuvert) {
    byte j = ((increment - 1) * (colonnes + 1)); // tous les 16 caractères
    mydisp.drawStr(0, 0, ""); // position du curseur en 0,0
    for (byte i = j; i < j + colonnes; i++) { // boucle pour afficher 16 caractères sur le lcd
      char temp = pgm_read_byte(affichageMenu + i); // utilisation du texte présent en mèmoire flash
      mydisp.print(temp);
    }
    switch (increment) { // test de la valeur de incrementation pour affichage des parametres
      case 1: // Date
        mydisp.drawStr(0, 1, " ");
        displayDate(); // affichage de la date
        break;
      case 2: // heure
        mydisp.drawStr(0, 1, "   ");
        displayTime(); // affichage de l'heure
        break;
      case 3: // heure ouverture
        mydisp.drawStr(0, 1, "   ");
        openTime();  // affichage de l'heure d'ouverture de la porte
        break;
      case 4: // heure fermeture
        mydisp.drawStr(0, 1, "   ");
        closeTime();  // affichage de l'heure de fermeture de la porte
        break;
      case 5: // temperature
        mydisp.drawStr(0, 1, "    ");
        read_temp(true); // read temperature celsius=true
        break;
      case 6: //lumiere
        mydisp.drawStr(0, 1, "");
        lumiere(); // lecture et affichage de la lumiere
        break;
      case 7: // lumiere matin
        mydisp.drawStr(0, 1, "");
        affiLumMatin(); // affichage de la lumiere du matin
        break;
      case 8: // lumiere soir
        mydisp.drawStr(0, 1, "");
        affiLumSoir(); // affichage de la lumiere du soir
        break;
      case 9: // choix Ouverture / Fermeture
        mydisp.drawStr(0, 1, "");
        affiChoixOuvFerm(); // choix
        break;
      case 10:  // fin de course Fermeture
        mydisp.drawStr(0, 1, "");
        affiFinDeCourseFermeture(); // fin de course Haut
        break;
      case 11:  // fin de course ouverture
        mydisp.drawStr(0, 1, "");
        affiFinDeCourseOuverture();  // fin de course ouverture
        break;
      case 12:  // tension batterie commandes
        mydisp.drawStr(0, 1, "");
        affiTensionBatCdes(); //
        break;
      case 13:  // tension batterie servo
        mydisp.drawStr(0, 1, "");
        affiTensionBatServo(); //
        break;
      case 14:  // commande manuelle
        mydisp.drawStr(0, 1, "");
        affiPulsePlusCptRoue(); // affichage pulse et comptage roue codeuse
        break;
    }
  }
}

/* watchdog */
//----Watchdog Interrupt Service est exécité lors d'un timeout du WDT----
ISR(WDT_vect) {
  if (f_wdt == 0) {
    f_wdt = 1; // flag
  }
}

//-----paramètre : 0=16ms, 1=32ms, 2=64ms, 3=128ms, 4=250ms, 5=500ms, 6=1 sec,7=2 sec, 8=4 sec, 9=8 secondes-----
void setup_watchdog(int ii) {
  byte bb;
  int ww;
  if (ii > 9 ) ii = 9;
  bb = ii & 7;
  if (ii > 7) bb |= (1 << 5);
  bb |= (1 << WDCE);
  ww = bb;
  // Clear the reset flag
  MCUSR &= ~(1 << WDRF);
  // start timed sequence
  WDTCSR |= (1 << WDCE) | (1 << WDE);
  // set new watchdog timeout value
  WDTCSR = bb;
  WDTCSR |= _BV(WDIE);
}

//-----entree dans le mode sleep-----
void enterSleep(void) {
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_enable();
  sleep_mode(); //Entre dans le mode veille choisi
  // Le programme va reprendre ici après le timeout du WDT
  sleep_disable(); // La 1ère chose à faire est de désactiver le mode veille
}

//-----routine de gestion du watchdog-----
void routineGestionWatchdog() {
  if ((f_wdt == 1 ) and (!boitierOuvert)) { // si le boitier est ferme et watchdog=1
    if ( !monServo.get_m_servoAction()) { // servo à l'arrêt
      //indicateur led 13 pour le mode sleep
      digitalWrite(LED_PIN, HIGH);
      delay(10);
      digitalWrite(LED_PIN, LOW);
      tempsWatchdog--;
      if (tempsWatchdog <= 0) {
        if (batterieFaible) { // affichage si la batterie est faible
          radio.messageRadio("*** Batterie faible ! ***");// on envoie le message
        }
        // informations à afficher
        if (RADIO) {
          displayTime();
          read_temp(TEMPERATURE); // read temperature celsius=true
          affiTensionBatCdes(); // affichage tension batterie commandes sur terminal
          affiTensionBatServo(); // affichage tension batterie servomoteur sur terminal
          affiFinDeCourseFermeture();
          affiFinDeCourseOuverture();
          affiPulsePlusCptRoue();
          affiChoixOuvFerm();
          lumiere();
          radio.chaineVide();
        }
        if (DEBUG) {
          Serial.print(F("compt watchdog lum : x * 8s  = ")); Serial.println(lum.get_m_compteurWatchdogLumiere());
          Serial.print(F("Bat faible : "));
          if (batterieFaible) {
            Serial.println(F("oui"));
          } else {
            Serial.println(F("non"));
          }
          Serial.println(F("Sommeil."));
          Serial.println(F("* * * *"));
          delay(100);
        }
        tempsWatchdog = bouclesWatchdog ; // initialisation du nombre de boucles
        lum.set_m_compteurWatchdogLumiere(lum.get_m_compteurWatchdogLumiere() + 1);// incrementation compteur watchdog lumiere
      }
      f_wdt = 0;
      enterSleep(); //Revenir en mode veille
    }
  }
}

/* setup */
void setup() {
  Wire.begin();
  Serial.begin(9600);
  /*----------for text LCD adapter and graphic LCD adapter ------------*/
  mydisp.begin();
  mydisp.setPrintPos(0, 0); // position 0 ligne 0 pour débuter le clear screen
  mydisp.clearScreen(); // CLear screen
  mydisp.backLightOff(); // retro eclairage off
  mydisp.enableCursor(); // enable cursor
  for (uint8_t j = 0; j < 1; j++)  {  //making "Hello" string moving
    for (uint8_t i = 0; i < 9; i++)  {  //move string to right
      mydisp.setPrintPos(i, 0); // ligne 0
      mydisp.print(F(" Bonjour"));
      delay(200); //delay
    }
    for (uint8_t i = 0; i < 9; i++) {  //move string to left
      mydisp.setPrintPos(8 - i, 0);
      mydisp.print(F("Bonjour "));
      delay(200);
    }
  }
  pinMode(LED_PIN, OUTPUT); // led broche 13
  incrementation = menuManuel; // pour affichage menu
  deroulementMenu (incrementation); // affichage du menu

  lum.set_m_ouverture( i2c_eeprom_read_byte(0x57, 0x14));// lecture du type d'ouverture @14  de l'eeprom de la carte rtc (i2c @ 0x57)
  delay(10);
  lum.set_m_fermeture( i2c_eeprom_read_byte(0x57, 0x15)); // lecture du type de fermeture @15   de l'eeprom de la carte rtc (i2c @ 0x57)
  delay(10);

  byte val1 = i2c_eeprom_read_byte(0x57, 0x16); // lecture pf lumière du matin (byte)
  delay(10);
  byte val2 = i2c_eeprom_read_byte(0x57, 0x17); // lecture Pf lumiere du matin (byte)
  delay(10);
  lum.set_m_lumMatin((val2 << 8) + val1);// mots 2 byte vers mot int lumMatin

  val1 = i2c_eeprom_read_byte(0x57, 0x18); // lecture pf lumière du soir (byte)
  delay(10);
  val2 = i2c_eeprom_read_byte(0x57, 0x19); // lecture Pf lumiere du soir (byte)
  delay(10);
  lum.set_m_lumSoir((val2 << 8) + val1);// mots 2 byte vers mot int lumSoir

  val1 = i2c_eeprom_read_byte(0x57, 0x20); // lecture pf fin de course haut (byte)
  delay(10);
  val2 = i2c_eeprom_read_byte(0x57, 0x21); // lecture Pf fin de course haut (byte)
  delay(10);
  finDeCourseFermeture = (val2 << 8) + val1;  // mots 2 byte vers mot int finDeCourseFermeture

  val1 = i2c_eeprom_read_byte(0x57, 0x22); // lecture pf fin de course bas (byte)
  delay(10);
  val2 = i2c_eeprom_read_byte(0x57, 0x23); // lecture Pf fin de course bas (byte)
  delay(10);
  finDeCourseOuverture = (val2 << 8) + val1;  // mots 2 byte vers mot int finDeCourseOuverture

  // roue codeuse
  pinMode(roueCodeuse, INPUT); // make the roueCodeuse's pin 7 an input

  attachInterrupt(1, myInterruptINT1, FALLING); // validation de l'interruption sur int1 (d3)
  attachInterrupt(0, myInterruptINT0, CHANGE); // validation de l'interruption sur int0 (d2)

  //RTC.writeRTC(0x0E,0x06); // registre control rtc
  RTC.writeRTC(0x0F, 0x00); // registre status rtc

  //Optimisation de la consommation
  //power_adc_disable(); // Convertisseur Analog / Digital pour les entrées analogiques
  power_spi_disable();
  //power_twi_disable();
  //Si pas besoin de communiquer par l'usb
  //power_usart0_disable();
  //Extinction des timers, attention timer0 utilisé par millis ou delay
  //power_timer0_disable();
  //power_timer1_disable();
  //power_timer2_disable();

  setup_watchdog(9); // maxi de 8 secondes

  radio.init();//initialisation radio

  monServo.init(); // initialisation du servo moteur et du relais

  if (!TESTSERVO) {
    monServo.servoOuvFerm(batterieFaible, reduit);// mise sous tension du servo et ouverture de la porte
  }
}

/* loop */
void loop() {

  lectureClavier(); // lecture du clavier
  //affichage du menu lorsque temps est > ....
  temporisationAffichage(boucleTemps) ; // temporisation pour l'affichage

  //reglages
  reglageTime(); // reglages de l'heure, minute, seconde si touche fleche droite
  reglageDate(); // reglage de la date si touche fleche droite
  reglageHeureOuverture();// reglage de l'heure d'ouverture
  reglageHeureFermeture();// reglage de l'heure de fermeture
  choixOuvFerm (); // choix de l'ouverture et la fermeture
  choixLumMatin();//  reglage de la lumiere du matin
  choixLumSoir();//  reglage de la lumiere du soir
  regFinDeCourseFermeture(); // reglage fin de course fermeture
  regFinDeCourseOuverture(); // reglage fin de course ouverture
  eclairageAfficheur(); // retro eclairage de l'afficheur

  if (TESTSERVO) {
    testServo(); // reglage du servo plus test de la roue codeuse et du servo, à l'aide de la console
  }

  ouvFermLum() ;  // ouverture/fermeture par test de la lumière

  batterieFaible = accusCde.accusFaible(); // test de la batterie commande < 4.8v
  // if (batterieFaible)  bouclesWatchdog *= 2; // doublement du temps watchdog si batterie faible

  ouverturePorte();
  fermeturePorte();

  routineTestFermetureBoitier(); // test fermeture boitier
  routineInterruptionBoitierOuvert(); // routine interruption boitier ouvert
  routineInterruptionBp(); // routine interruption Bp

  routineInterrruptionAlarme2() ; // routine alarme 2
  routineInterruptionAlarme1(); // routine alarme 1

  routineGestionWatchdog(); // routine de gestion du watchdog
}

