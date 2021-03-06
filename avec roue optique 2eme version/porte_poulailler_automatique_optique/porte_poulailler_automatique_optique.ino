/**@file*/
/**
    \file porte_poulailler_affichage_clavier_rtc
    \brief Automatisation de la porte du poulailler en utilisant l'heure ou la lumière
    \details Simplification d'utilisation. Electrinique avec microcontroleur alimentée par batterie, couplée à un capteur solaire
    \author Zephyr5028
    \date hiver 2017
*/
/**
    porte-poulailler : affichage + clavier + rtc
*/

/**
  01 2017 classes radio, lcd (digole et liquidcrystal), horloge, bouton.
  29 12 2016 classe Codeur (optique).
  28 12 2016 classe Lumiere - ok.
  26 12 2016 classe Accus - ok.
  21 12 2016 ajout de la classe servo - ok.
  01 12 2016first commit sur gihub.
  20 09 2016 classe clavier/

  19 septembre 2016 :
    - suppression du blocage au demarrage (si = 200).
  18 septembre 2016 :
    En cas de probleme avec le sens de rotation du servo :
    - variable globale : sens.
    - arret fin de course ( contact) quelque soit le sens.
    - diminution de la vitesse reduite dans la descente de la trappe (inertie).
  17 septembre 2016 :
  - problème avec servotimer2  :
       Le pulse en ms ne fonctionne pas donc le servo est tjs dans le même sens (hs).
       Les valeurs du pulse étaient mauvaise.
       Il faut :
               90° : 1500ms.
                0° : 1000ms.
              180° : 2000ms.
  10 septembre 2016 :
    - affichage compteur roue codeuse envoi radio.
    - delay d'attente de la remontée du servo augmentée : 300.
  Important :
    -with ServoTimer2 et VirtualWire.
    -il faut commenter typedef dans la bibliothèque servotimer2.
*/
/*
           nom macro  valeur de la macro                                forme syntaxique
           __LINE__  numéro de la ligne courante du programme source   entier
           __FILE__  nom du fichier source en cours de compilation     chaîne
           __DATE__  la date de la compilation                         chaîne
           __TIME__  l'heure de la compilation                         chaîne
           __STDC__  1 si le compilateur est ISO, 0 sinon              entier
*/


/*--------------------------------------------------------------------------------*/
/// choisir entre un afficheur lcd I2C de type Digole (PICF182) ou de type LiquidCrystal (PCF8574)

#define LCD_DIGOLE  // utilisation de lcd avec circuit I2C Digole - PIC16F182

//#define LCD_LIQIDCRYSTAL  // utilisation de lcd liquid crystal I2C - PCF8574

/*--------------------------------------------------------------------------------*/

/**------Bibliothèque Flash pour mise en mémoire flash de l'arduino F()--------*/
#include <Flash.h>
#include <avr/pgmspace.h> // non nécessaire maintenant

/** power and tools */
/** watchdog - Optimisation de la consommation */
#include "PowerTools.h"
const boolean debug = false; // positionner debug pour l'utiliser ou pas
/*-----------------------------*/
const byte bouclesWatchdog = 32;// nombre de boucles du watchdog : environ 64s pour 8 boucles
/*-----------------------------*/
unsigned int memoireLibre = 0; // variable pour calcul de la memoire libre
volatile int f_wdt = 1; // flag watchdog
byte tempsWatchdog = bouclesWatchdog ; // boucle temps du chien de garde
boolean reglage = false; // menu=false ou reglage=true
PowerTools tools (debug); // objet tools et power

/** radio 433MHz */
#include "Radio.h"
const boolean emissionRadio = true ; // positionner radio pour l'utiliser (true) ou pas
const byte pinEmRadio = 10; // pin D10 emetteur radio
const int vitesseTransmission = 600;//nitialisation de la bibliothèque avec la vitesse (vitesse_bps)
const byte pinSwitchEmissionRadio = A2; // broche A2 en entree digitale pour le switch emission on/off
Radio radio(pinEmRadio, pinSwitchEmissionRadio, vitesseTransmission, VW_MAX_MESSAGE_LEN, emissionRadio, debug); // classe Radio

/** led broche 13 */
#define LED_PIN 13

/** servo - montée et descente de la porte */
#include "ServoMoteur.h"
const bool testServoMoteur = false; // pour utiliser ou non le test du servomoteur
const byte servoCde = 8; // pin D8 cde du servo
const byte servoPin = 4; // pin D4 relais du servo
const byte securiteHaute = 12; // pin D12 pour l'ouverture de porte
const int pulseStop = 1500; // value should usually be 750 to 2200 (1500 = stop)
bool reduit = false; // vitesse du servo, normal ou reduit(false)
// pulse stop, ouverture/fermeture , reduit et debug si nécessaire
ServoMoteur monServo(servoCde, servoPin, securiteHaute, pulseStop, 140, 70, debug);

/** Accus */
#include "Accus.h"
const byte accusPinCde = A6; //analog pin A6 : tension batterie commandes
const byte accusPinServo = A7; //analog pin A7 : tension batterie servo moteur
const float tensionMiniAccus = 4.8; //valeur minimum de l'accu 4.8v
const float rapportConvertion = 7.5;// rapport de convertion CAD float
boolean batterieFaible = false; // si batterie < 4,8v = true
Accus accusCde (accusPinCde, tensionMiniAccus, rapportConvertion, debug); // objet accus commande mini 4.8v, convertion 7.5
Accus accusServo (accusPinServo); // objet accus servo moteur mini 4.8v, convertion 7.5

/** roue codeuse */
#include "Codeur.h"
const byte roueCodeuse = 7;//digital pin D7 pour entrée roue codeuse
const unsigned int compteRoueCodeuse = 150;  // un compteur de position
const unsigned int finDeCourseFermeture = 250; // initialisation de la valeur de la fin de course fermeture
const unsigned int finDeCourseOuverture = 150; // initialisation de la valeur de la fin de course ouverture
Codeur codOpt (roueCodeuse, finDeCourseFermeture, finDeCourseOuverture, compteRoueCodeuse, debug); // objet codeur optique

/** lumiere */
#include "Lumiere.h"
const byte lumierePin = A0; //analog pin A0 : luminosite
const float convertion = 5;// rapport de convertion CAD float
const byte heureFenetreSoir = 17; //horaire de la fenetre de non declenchement lumiere si utilisation horaire : 17h
const byte boucleLumiere = 2; // 2 boucles pour valider l'ouverture / fermeture avec la lumière (compteur watchdog)
const unsigned int lumMatin = 300; // valeur de la lumière du matin
const unsigned int lumSoir = 900; // valeur de la lumiere du soir
Lumiere lum(lumierePin, lumMatin, lumSoir, heureFenetreSoir, convertion, boucleLumiere, debug); // objet lumiere

/** interruptions */
volatile boolean interruptBp = false; // etat interruption entree 9
volatile boolean interruptRTC = false; // etat interruption entree 5
volatile boolean interruptOuvBoi = false; // etat interruption entree 6

/** Clavier */
/* menus */
#include "Clavier.h"
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
const byte oldkey = -1;
const byte sensorClavier = A1; //pin A1 pour le clavier
const byte pinBp = 9; // pin D9 bouton poussoir ouverture / fermeture
const byte pinBoitier = 6; //pin D6 interrupteur ouverture boitier
const int debounce = 500; // debounce latency in ms
byte incrementation = 0; // incrementation verticale
boolean relache = false; // relache de la touche
byte touche = -1; // valeur de la touche appuyee (-1 pour non appuyée)
boolean  boitierOuvert = true; // le boitier est ouvert
Clavier clav(menuManuel, pinBp, pinBoitier, debounce, debug); // class Clavier avec le nombre de lignes du menu

/** LCD DigoleSerialI2C */
const int boucleTemps = 100; // temps entre deux affichages
int temps = 0;// pour calcul dans la fonction temporisationAffichage
bool LcdCursor = true; //curseur du lcd if true = enable
#ifdef  LCD_DIGOLE
/// I2C:Arduino UNO: SDA (data line) is on analog input pin 4, and SCL (clock line) is on analog input pin 5 on UNO and Duemilanove
#include "LcdDigoleI2C.h"
LcdDigoleI2C mydisp( &Wire, '\x27', colonnes, debug); // classe lcd digole i2c (lcd 2*16 caracteres)
#endif
#ifdef LCD_LIQIDCRYSTAL
#include "LcdPCF8574.h"
// Set the LCD address to 0x27 for a 16 chars and 2 line display
LcdPCF8574  mydisp(0x27, 16, 2);
#endif

/** RTC_DS3231 */
#include "HorlogeDS3232.h"
const byte rtcINT = 5; // digital pin D5 as l'interruption du rtc ( alarme)
const byte adresseBoitier24C32 = 0x57;// adresse du boitier memoire eeprom 24c32
const byte jourSemaine = 1, jour = 2, mois = 3, annee = 4, heure = 5, minutes = 6, secondes = 7;
const byte alarm_1 = 1; // alarme 1
const byte alarm_2 = 2; //alarme 2
const bool typeTemperature = true; // true = celsius , false = fahrenheit
tmElements_t tm; // declaration de tm pour la lecture des informations date et heure
HorlogeDS3232 rtc(adresseBoitier24C32, rtcINT, debug);

/** progmem  mémoire flash */
const char listeDayWeek[] PROGMEM = "DimLunMarMerJeuVenSam"; // day of week en mémoire flash
const char affichageMenu[] PROGMEM = "      Date      .      Heure     . Heure Ouverture. Heure Fermeture.  Temperature   .     Lumiere    .  Lumiere matin .  Lumiere soir  . Choix Ouv/Ferm . Fin course fer . Fin course ouv . Tension bat N1 . Tension bat N2 .Servo Pulse Rcod.";
const char affichageBatterieFaible[] PROGMEM = "*** Batterie faible ! ***";
const char ouvertureDuBoitier[] PROGMEM = "Ouverture du boitier.";
const char fermetureDuBoitier[] PROGMEM = "Fermeture du boitier.";
const char affichageBonjour[] PROGMEM = "Porte Poulailler. Version 1.4.1  .Porte Poulailler.Manque carte RTC";

#include "AA_fonctions.h" // prototypes des fonctions du programme

/* clavier */
///-----lecture clavier------
void  lectureClavier() {
  if (boitierOuvert) {
    touche = clav.read_key(sensorClavier); /// read key sensor = A1
    clav.relacheTouche(touche, relache);
    if (clav.deplacementDansMenu(touche, relache, reglage) or touche == 4) {
      clav.positionMenu(incrementation, touche); /// position du menu pour l'affichage
      deroulementMenu (incrementation); /// affichage du menu
    }
  }
}


///---- temporisation pour l'affichage: affichage du menu lorsque temps est > boucleTemps-----
void temporisationAffichage(const int boucleTemps) {
  if (  temps > boucleTemps and touche != -1) {
    deroulementMenu (incrementation); // affichage du menu en fonction de incrementation
    temps = 0;
  } else {
    temps += 1;
  }
}


///----routine lecture et ecriture date and time-----
void ecritureDateTime() {
  RTC.write(tm);
  if (incrementation == menuHeure) {
    displayTime(); // affichage de l'heure
  }
  if (incrementation == menuDate) {
    displayDate(); // affichage de la date
  }
}

/* affichages */
///-----routine display Date-----
void displayDate() {
  if ( boitierOuvert) { // si le boitier est ouvert
    RTC.read(tm); // lecture date et heure
    // affichage du jour de la semaine
    char semaine[4] = "";
    byte j = ((tm.Wday - 1) * 3);
    for (byte i = j; i < j + 3; i++) {
      semaine[i - j] = pgm_read_byte(listeDayWeek + i);
    }
    mydisp.affichageDateHeure(semaine, tm.Day, tm.Month, tm.Year);
  }
}

///-----routine display Time-----
void displayTime () {
  if ( boitierOuvert) { // si le boitier est ouvert
    RTC.read(tm); // lecture date et heure
    mydisp.affichageDateHeure("H", tm.Hour, tm.Minute, tm.Second);
  } else   if (radio.get_m_radio()) {
    int timeHour = rtc.lectureRegistreEtConversion (RTC_HOURS, 0x3f); // heure
    int timeMinute = rtc.lectureRegistreEtConversion( RTC_MINUTES ) ; // minutes
    radio.envoiUnsignedInt(timeHour,  boitierOuvert, "h");// envoi message radio heure + etat  boitier
    radio.envoiUnsignedInt(timeMinute,  boitierOuvert, "m;");// envoi message radio minute  + etat  boitier
  }
}

///----routine door open time-----
void openTime() {
  if ( boitierOuvert) { // si le boitier est ouvert
    // affichage de l'horaire de l'alarme 1
    mydisp.affichageDateHeure("H", rtc.get_m_alarm1Hour(), rtc.get_m_alarm1Minute() , rtc.get_m_alarm1Second());
  }
}

///-----routine door close time-----
void closeTime() {
  if ( boitierOuvert) { // si le boitier est ouvert
    // affichage de l'horaire de l'alarme 2 - 61 pour ne pas afficher les secondes
    mydisp.affichageDateHeure("H", rtc.get_m_alarm2Hour(), rtc.get_m_alarm2Minute(), 61);
  }
}

///------affichage pulse et comptage roue codeuse------
void affiPulsePlusCptRoue() {
  int pulse = monServo.get_m_pulse();
  byte test = 0;
  test = codOpt.testCompteurRoueCodeuse (5); // tolerance de 5
  unsigned int compteRoueCodeuse = codOpt.get_m_compteRoueCodeuse();
  if ( boitierOuvert) { // si le boitier est ouvert
    byte ligne = 1;
    mydisp.affichageServo(pulse, compteRoueCodeuse, ligne);
  } else   if (radio.get_m_radio() and tempsWatchdog <= 0 ) { // eviter l'envoi à l'initialisation
    char chaine1[VW_MAX_MESSAGE_LEN - 1] = "";
    switch (test) {
      case 1: // mise sous tension du servo pour l'ouverture de la porte
        strcat(chaine1, "fer;");
        break;
      case 2: // mise sous tension du servo pour la fermeture de la porte
        strcat(chaine1, "ouv;");
        break;
      default:
        strcat(chaine1, " ? ;");
        break;
    }
    radio.envoiMessage(chaine1);// on envoie le message ouverture / fermeture
    radio.envoiUnsignedInt(compteRoueCodeuse, boitierOuvert, ";"); // envoi message radio compteur roue codeuse
  }
}

/* afficheur */
///-----retro eclairage de l'afficheur-----
void eclairageAfficheur() {
  if (boitierOuvert) {
    if (clav.testTouche5(touche, relache)) {
      mydisp.retroEclairage();// retro eclairage
    }
  }
}

/* batteries */
///-------affichage tension batterie commandes
void affiTensionBatCdes() {
  int valBatCdes = accusCde.tensionAccusCAD(); // tension batterie CAD
  float voltage = accusCde.tensionAccus(valBatCdes);// read the input on analog pin A2 : tension batterie commandes
  // print out the value you read:
  if ( boitierOuvert) { // si le boitier est ouvert
    byte ligne = 1;
    mydisp.affichageVoltage(  voltage, "V",  ligne);
  } else   if (radio.get_m_radio()) {
    radio.envoiFloat(voltage, boitierOuvert,  "V;" ); // envoi message radio tension accus}*/
  }
}

///-------affichage tension batterie servo-moteur
void affiTensionBatServo() {
  int valBatServo = accusServo.tensionAccusCAD(); // tension batterie CAD
  float voltage = accusServo.tensionAccus(valBatServo);// read the input on analog pin A3 : tension batterie servo moteur
  // print out the value you read:
  if ( boitierOuvert) { // si le boitier est ouvert
    byte ligne = 1;
    mydisp.affichageVoltage(  voltage, "V",  ligne);
  } else   if (radio.get_m_radio()) {
    radio.envoiFloat(voltage, boitierOuvert, "V;"); // envoi message radio tension accus
  }
}

/* choix pour l'ouverture et la fermeture :
     - heure
     - lumiere
*/
///------affichage du choix de l'ouverture et la fermeture------
void affiChoixOuvFerm() {
  if ( boitierOuvert) { // si le boitier est ouvert
    byte ligne = 1;
    mydisp.affichageChoix(lum.get_m_ouverture(), lum.get_m_fermeture(), ligne);
  }
}

///-----routine choix ouverture fermeture-----
void choixOuvFerm () {
  if (boitierOuvert) {
    byte deplacement = 7;
    if (incrementation == menuChoix) {
      mydisp.cursorPositionReglages (touche, relache, reglage, 15, deplacement, 14);// position du cuseur pendant les reglages
    }
    if ((touche == 2 or touche == 3) and incrementation == menuChoix and relache == true and reglage == true ) { // si appui sur les touches 2 ou 3 pour reglage des valeurs
      relache = false;
      if (mydisp.get_m_decalage() == deplacement) {
        lum.set_m_ouverture(rtc.choixTypeOuvertureFermeture(lum.get_m_ouverture(), alarm_1));//choix du type d'ouverture / fermeture
        affiChoixOuvFerm();
      }
      if (mydisp.get_m_decalage() == 2 * deplacement) {
        lum.set_m_fermeture(rtc.choixTypeOuvertureFermeture(lum.get_m_fermeture(), alarm_2));//choix du type d'ouverture / fermeture
        affiChoixOuvFerm();
      }
    }
  }
}

/* reglage heure fermeture */
///-----reglage de l'heure de fermeture------
void reglageHeureFermeture() {
  if (boitierOuvert) {
    byte deplacement = 4;
    if (incrementation == menuFermeture) {
      mydisp.cursorPositionReglages (touche, relache, reglage, 10, deplacement, 10);// position du cuseur pendant les reglages
    }
    // Set Alarm2
    if ((touche == 2 or touche == 3) and incrementation == menuFermeture and relache == true and reglage == true ) { // si appui sur les touches 2 ou 3 pour reglage des valeurs
      relache = false;
      if (mydisp.get_m_decalage() == deplacement) {
        rtc.reglageAlarme( touche, alarm_2, heure); // reglage de l'alarme 2 - heure
        closeTime(); // affichage de l'heure de fermeture
      }
      if (mydisp.get_m_decalage() == 2 * deplacement) {
        rtc.reglageAlarme( touche, alarm_2, minutes); // reglage de l'alarme 2 - minutes
        closeTime(); // affichage de l'heure de fermeture
      }
    }
  }
}

/* reglage heure ouverture */
///-----routine de reglage de l'heure d'ouverture-----
void reglageHeureOuverture() {
  if (boitierOuvert) {
    byte deplacement = 4;
    if (incrementation == menuOuverture) {
      mydisp.cursorPositionReglages (touche, relache, reglage, 14, deplacement, 14);// position du cuseur pendant les reglages
    }
    // Set Alarm1
    if ((touche == 2 or touche == 3) and incrementation == menuOuverture and relache == true and reglage == true ) { // si appui sur les touches 2 ou 3 pour reglage des valeurs
      relache = false;
      if (mydisp.get_m_decalage() == deplacement) {
        rtc.reglageAlarme( touche, alarm_1, heure); // reglage de l'alarme 1 - heure
        openTime(); // affichage de l'heure d'ouverture
      }
      if (mydisp.get_m_decalage() == 2 * deplacement) {
        rtc.reglageAlarme( touche, alarm_1, minutes); // reglage de l'alarme 1 - minutes
        openTime(); // affichage de l'heure d'ouverture
      }
      if (mydisp.get_m_decalage() == 3 * deplacement) {
        rtc.reglageAlarme( touche, alarm_1, secondes); // reglage de l'alarme 1 - secondes
        openTime(); // affichage de l'heure d'ouverture
      }
    }
  }
}

/* reglage de la date */
///----routine reglage jour semaine, jour, mois, annee-----
void reglageDate () {
  if (boitierOuvert) {
    byte deplacement = 3;
    if (incrementation == menuDate) {
      mydisp.cursorPositionReglages (touche, relache, reglage, 14, deplacement, 14);// position du cuseur pendant les reglages
    }
    if ((touche == 2 or touche == 3) and incrementation == menuDate and relache == true and reglage == true) { // si appui sur les touches 2 ou 3 pour reglage des valeurs
      relache = false;
      if (mydisp.get_m_decalage() == deplacement) {
        tm.Wday = rtc.reglageHeure(touche, tm.Wday, jourSemaine);
        ecritureDateTime();
      }
      if (mydisp.get_m_decalage() == 2 * deplacement) {
        tm.Day = rtc.reglageHeure(touche, tm.Day, jour);
        ecritureDateTime();
      }
      if (mydisp.get_m_decalage() == 3 * deplacement) {
        tm.Month = rtc.reglageHeure(touche, tm.Month, mois);
        ecritureDateTime();
      }
      if (mydisp.get_m_decalage() == 4 * deplacement) {
        tm.Year = rtc.reglageHeure(touche, tm.Year, annee);
        ecritureDateTime();
      }
    }
  }
}

/* reglage de la lumiere du matin */
///------affichage de la lumiere du matin------
void affiLumMatin() {
  if (boitierOuvert) {
    unsigned int lumMatin = lum.get_m_lumMatin();
    byte ligne = 1;
    mydisp.affichageLumFinCourse(lumMatin,  ligne);
  }
}

///-----reglage du choix de la lumiere du matin-------
void choixLumMatin() {
  if (boitierOuvert) {
    byte deplacement = 8;
    if (incrementation == menuLumiereMatin) {
      mydisp.cursorPositionReglages (touche, relache, reglage, 15, deplacement, 14);// position du cuseur pendant les reglages
    }
    if ((touche == 2 or touche == 3) and incrementation == menuLumiereMatin and relache == true and reglage == true ) { // si appui sur les touches 2 ou 3 pour reglage des valeurs
      relache = false;
      if (mydisp.get_m_decalage() == deplacement) {
        bool matin = 1;
        bool lumiere = 1;
        unsigned int lumMatin = lum.reglageLumiere(matin, touche);// reglage de la lumiere du matin
        rtc.sauvEepromChoix ( lumMatin, matin, lumiere);// sauvegarde dans l'eeprom I2C le choix de la lumiere du matin @0x16 et 0x17
        affiLumMatin();
      }
    }
  }
}

/* reglage de la lumiere du soir  */
///------affichage de la lumiere du soir-------
void affiLumSoir() {
  if (boitierOuvert) {
    unsigned int lumSoir = lum.get_m_lumSoir();
    byte ligne = 1;
    mydisp.affichageLumFinCourse(lumSoir, ligne);
  }
}

///------reglage du choix de la lumiere du soir--------
void choixLumSoir() {
  if (boitierOuvert) {
    byte deplacement = 8;
    if (incrementation == menuLumiereSoir) {
      mydisp.cursorPositionReglages (touche, relache, reglage, 15, deplacement, 14);// position du cuseur pendant les reglages
    }
    if ((touche == 2 or touche == 3) and incrementation == menuLumiereSoir and relache == true and reglage == true ) { // si appui sur les touches 2 ou 3 pour reglage des valeurs
      relache = false;
      if (mydisp.get_m_decalage() == deplacement) {
        bool soir = 0;
        bool lumiere = 1;
        unsigned int lumSoir = lum.reglageLumiere(soir, touche); // reglage de la lumiere du soir
        rtc.sauvEepromChoix ( lumSoir, soir, lumiere);// sauvegarde dans l'eeprom I2C le choix de la lumiere du soir  @0x18 et 0x19
        affiLumSoir();
      }
    }
  }
}

/* reglage time */
///-----routine reglage heure , minute , seconde-----
void reglageTime () {
  if (boitierOuvert) {
    byte deplacement = 4;
    if (incrementation == menuHeure) {
      mydisp.cursorPositionReglages (touche, relache, reglage, 14, deplacement, 14);// position du cuseur pendant les reglages
    }
    if ((touche == 2 or touche == 3) and incrementation == menuHeure and relache == true and reglage == true ) { // si appui sur les touches 2 ou 3 pour reglage des valeurs
      relache = false;
      if (mydisp.get_m_decalage() == deplacement) {
        tm.Hour = rtc.reglageHeure(touche, tm.Hour, heure);
        ecritureDateTime(); // routine écriture date and time
      }
      if (mydisp.get_m_decalage() == 2 * deplacement) {
        tm.Minute = rtc.reglageHeure(touche, tm.Minute, minutes);
        ecritureDateTime();
      }
      if (mydisp.get_m_decalage() == 3 * deplacement) {
        tm.Second = rtc.reglageHeure(touche, tm.Second, secondes);
        ecritureDateTime();
      }
    }
  }
}

/* fins de course ouverture et fermeture */
///------affichage fin de course Fermeture-----
void affiFinDeCourseFermeture() {
  unsigned int finDeCourseFermeture = codOpt.get_m_finDeCourseFermeture();
  if ( boitierOuvert) { // si le boitier est ouvert
    byte ligne = 1;
    mydisp.affichageLumFinCourse(finDeCourseFermeture, ligne);
  } else   if (radio.get_m_radio()) {
    radio.envoiUnsignedInt(finDeCourseFermeture, boitierOuvert, ";"); // envoi message radio fin de course fermeture
  }
}

///------affichage fin de course Ouverture-------
void affiFinDeCourseOuverture() {
  unsigned int finDeCourseOuverture = codOpt.get_m_finDeCourseOuverture();
  if ( boitierOuvert) { // si le boitier est ouvert
    byte ligne = 1;
    mydisp.affichageLumFinCourse(finDeCourseOuverture, ligne);
  } else   if (radio.get_m_radio()) {
    radio.envoiUnsignedInt(finDeCourseOuverture, boitierOuvert, ";"); // envoi message radio fin de course Ouverture
  }
}

///------reglage fin de course Fermeture------
void regFinDeCourseFermeture() {
  if (boitierOuvert) {
    byte deplacement = 8;
    if (incrementation == menuFinDeCourseFermeture) {
      mydisp.cursorPositionReglages (touche, relache, reglage, 15, deplacement, 12);// position du cuseur pendant les reglages
    }
    if ((touche == 2 or touche == 3) and incrementation == menuFinDeCourseFermeture and relache == true and reglage == true ) { // si appui sur les touches 2 ou 3 pour reglage des valeurs
      relache = false;
      if (mydisp.get_m_decalage() == deplacement) {
        bool fermeture = 0;
        bool finDeCourse = 0;
        unsigned int finDeCourseFermeture = codOpt.reglageFinDeCourse(fermeture, touche);// reglage de la fin de course
        rtc.sauvEepromChoix ( finDeCourseFermeture, fermeture, finDeCourse);// sauvegarde dans l'eeprom I2C de la valeur de fin de course fermeture @0x20 et 0x21
        affiFinDeCourseFermeture();
      }
    }
  }
}

///-----regalge fin de course ouverture------
void regFinDeCourseOuverture() {
  if (boitierOuvert) {
    byte deplacement = 8;
    if (incrementation == menuFinDeCourseOuverture) {
      mydisp.cursorPositionReglages (touche, relache, reglage, 15, deplacement, 12);// position du cuseur pendant les reglages
    }
    if ((touche == 2 or touche == 3) and incrementation == menuFinDeCourseOuverture and relache == true and reglage == true ) { // si appui sur les touches 2 ou 3 pour reglage des valeurs
      relache = false;
      if (mydisp.get_m_decalage() == deplacement) {
        bool ouverture = 1;
        bool finDeCourse = 0;
        unsigned int finDeCourseOuverture = codOpt.reglageFinDeCourse(ouverture, touche);// reglage de la fin de course
        rtc.sauvEepromChoix ( finDeCourseOuverture, ouverture, finDeCourse);// sauvegarde dans l'eeprom I2C de la valeur de fin de course ouverture @0x22 et 0x23
        affiFinDeCourseOuverture();
      }
    }
  }
}

/** Test du servo avec la console arduino

    a = -1
    q = -1

    z = +10
    s = -10

    e = arret du servo (relais)
    d = marche du servo (relais)

    r = montée
    f = descente

*/
/// reglage du servo plus test de la roue codeuse et du servo, à l'aide de la console
void testServo() {
  if (testServoMoteur) {
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
      Serial.print(F("Etat de l'impulsion du servo = "));
      Serial.print(pulse);
      Serial.println(F(" ms"));
    }
  }
}

/* temperature */
///-----routine lecture température sur ds3231 rtc type celsius=true ,fahrenheit=false------
void read_temp(const boolean typeTemperature) {
  float t = rtc.calculTemperature (typeTemperature);//valeur de la temperature en fonction du type
  if ( boitierOuvert) { // si le boitier est ouvert
    byte ligne = 1;
    String texte = "";
    if (typeTemperature) texte = "C"; else texte = "F";
    mydisp.affichageVoltage(t, texte, ligne);
  } else   if (radio.get_m_radio()) {
    char txt[3] = "";
    if (typeTemperature)  strcat(txt, "C;"); else strcat(txt, "F;");
    radio.envoiFloat( t, boitierOuvert, txt);
  }
}

/* servomoteur :
    -montée
    -descente
*/
///------sequence ouverture de la porte------
void ouverturePorte() {
  unsigned int compteRoueCodeuse = codOpt.get_m_compteRoueCodeuse();
  unsigned int finDeCourseOuverture = codOpt.get_m_finDeCourseOuverture();
  if (monServo.get_m_servoAction() and monServo.get_m_ouvFerm()) {
    if (compteRoueCodeuse <= finDeCourseOuverture + 20) {  // 100 + 20
      reduit = 0;// vitesse reduite
      monServo.servoVitesse( reduit);
    }
    if (!digitalRead(securiteHaute) or (touche == 4 and boitierOuvert)) {
      codOpt.set_m_compteRoueCodeuse (monServo.servoHorsTension(compteRoueCodeuse, finDeCourseOuverture));
    }
  }
}

///-----sequence fermeture de la porte-----
void  fermeturePorte() {
  unsigned int compteRoueCodeuse = codOpt.get_m_compteRoueCodeuse();
  unsigned int finDeCourseFermeture = codOpt.get_m_finDeCourseFermeture();
  if (monServo.get_m_servoAction() and !monServo.get_m_ouvFerm()) {
    if (compteRoueCodeuse >= finDeCourseFermeture - 30) { // 195 - 30
      reduit = 0;// vitesse reduite
      monServo.servoVitesse( reduit);
    }
    if ((compteRoueCodeuse >= finDeCourseFermeture) or !digitalRead(securiteHaute) or  (touche == 4 and boitierOuvert)) {
      codOpt.set_m_compteRoueCodeuse (monServo.servoHorsTension(compteRoueCodeuse, codOpt.get_m_finDeCourseOuverture()));
    }
  }
}

/** interruptions
  -routine interruption D2 INT0
  -routine interruption D3 INT1
  -routine interruption Bp
  -routine alarme 2
  -routine alarme 1
  -routine interruption boitier ouvert
  -test fermeture boitier
*/
///-----routine interruption D2 INT0------
void myInterruptINT0() {
  codOpt.compteurRoueCodeuse(monServo.get_m_ouvFerm());
}

//-----routine interruption D3 INT1-----
void myInterruptINT1() {
  rtc.testInterruptRTC(interruptRTC);// test de l'entree 5 - interruption du rtc
  clav.testInterruptionBp (interruptBp);// test l'it du bp
  clav.testInterruptionBoitier (interruptOuvBoi);// test l'it de l'interrupteur du boitier
}

///-----routine interruption Bp-----
void routineInterruptionBp() {
  if (interruptBp) { // test de l'appui sur bouton bp
    if (clav.testToucheBp ()) { //debounce pour le bp
      if (monServo.get_m_ouvFerm())  monServo.set_m_ouvFerm(false); else  monServo.set_m_ouvFerm(true);
      reduit = 1;// vitesse normale
      monServo.servoOuvFerm(batterieFaible, reduit);
    }
    clav.testRelacheBp(interruptBp);// test du relache du bp
  }
}

///-----routine alarme 2-----
void  routineInterrruptionAlarme2() {
  if ( RTC.alarm(alarm_2) and interruptRTC ) {    // has Alarm2 (fermeture) triggered?  alarme rtc
    // mise sous tension du servo pour la fermeture de la porte
    monServo.set_m_ouvFerm(false); // fermeture
    reduit = 1;// vitesse normale
    monServo.servoOuvFerm(batterieFaible, reduit);
    interruptRTC = false; // autorisation de la prise en compte de l'IT
  }
}

///-----routine alarme 1-----
void  routineInterruptionAlarme1() {
  if ( RTC.alarm(alarm_1) and interruptRTC ) {    // has Alarm1 (ouverture) triggered?  alarme rtc
    // mise sous tension du servo pour l'ouverture de la porte
    monServo.set_m_ouvFerm(true); // ouverture
    reduit = 1;// vitesse normale
    monServo.servoOuvFerm(batterieFaible, reduit);
    interruptRTC = false; // autorisation de la prise en compte de l'IT
  }
}

///-----test ouverture boitier-----
void routineTestOuvertureBoitier()  {
  if ( clav.testBoitierOuvert( interruptOuvBoi, boitierOuvert)) {
    char chaine[22] = "";
    for (byte i = 0; i < 22 ; i++) {
      chaine[i] = pgm_read_byte(ouvertureDuBoitier + i);
    }
    if (radio.get_m_radio()) {
      radio.envoiMessage(chaine);// message radio à l'ouverture du boitier
      radio.chaineVide();
    }
    boitierOuvert = true; // boitier ouvert
    mydisp.gestionCurseur(1); // activation du curseur
    deroulementMenu (incrementation); /// affichage du menu
  }
}

///-----test fermeture boitier-----
void  routineTestFermetureBoitier() {
  if (clav.testBoitierFerme(interruptOuvBoi, boitierOuvert)) {
    mydisp.razLcd(); // extinction et raz du lcd
    boitierOuvert = false; // boitier ferme
    interruptOuvBoi = false; // autorisation de la prise en compte de l'IT
    mydisp.choixRetroEclairage (0);// extinction retro eclairage
    char chaine[22] = "";
    for (byte i = 0; i < 22 ; i++) {
      chaine[i] = pgm_read_byte(fermetureDuBoitier + i);
    }
    if (radio.get_m_radio()) {
      radio.envoiMessage(chaine);// message radio à l'ouverture du boitier
      displayTime();// avec affichage de l'heure de fermeture
      radio.chaineVide();
    }
  }
}

/* lumiere
  -affichage
*/
///-----routine lecture et affichage de la lumière-----
void lumiere() {
  int lumValue = lum.luminositeCAD(); // luminosite CAD sur pin A0
  if ( boitierOuvert) { // si le boitier est ouvert
    byte ligne = 1;// première ligne car non reglable
    bool nonReglable = 1; // pour afficher le curseur sur la premiere ligne car non reglable
    mydisp.affichageLumFinCourse(lumValue, ligne, nonReglable);
  } else   if (radio.get_m_radio())  {
    radio.envoiUnsignedInt(lum.get_m_lumSoir(), boitierOuvert, ";"); // envoi message radio lumiere du soir
    radio.envoiUnsignedInt(lumValue, boitierOuvert, ";"); // envoi message radio lumiere
  }
}

///-----ouverture/fermeture par test de la lumière----
void ouvFermLum() {
  byte  valHeure = rtc.lectureRegistreEtConversion(RTC_HOURS & 0x3f); // lecture de l'heure
  lum.testLuminosite(); // test de la luminosite pour mise à jour du compteur watchdog lumiere
  //fenetre de non declenchement pour ne pas declencher la fermeture avant 17h00 et l'ouverture après 17h00 et mise à jour du compteur watchdog lumiere
  lum.fenetreNonDeclenchement(valHeure) ;
  //non eclenchement en fonction de la position du servo et mise à jour du compteur watchdog lumiere
  lum.nonDeclenchementPositionServo (codOpt.get_m_compteRoueCodeuse(), codOpt.get_m_finDeCourseFermeture(), codOpt.get_m_finDeCourseOuverture());
  byte declenchementLuminosite = lum.declenchementServoLuminosite(); // test de la luninosite et declenchement du servo
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
///-----routine affichage menus------
void deroulementMenu (byte increment) {
  if (boitierOuvert) {
    byte j = ((increment - 1) * (colonnes + 1)); // tous les 16 caractères
    mydisp.cursorPosition(0, 0, ""); // decalage, ligne, texte
    for (byte i = j; i < j + colonnes; i++) { // boucle pour afficher 16 caractères sur le lcd
      char temp = pgm_read_byte(affichageMenu + i); // utilisation du texte présent en mèmoire flash
      mydisp.print(temp);// valable pour digoleSerial et liquidCrystal
    }
    switch (increment) { // test de la valeur de incrementation pour affichage des parametres
      case 1: // Date
        displayDate(); // affichage de la date
        break;
      case 2: // heure
        displayTime(); // affichage de l'heure
        break;
      case 3: // heure ouverture
        openTime();  // affichage de l'heure d'ouverture de la porte
        break;
      case 4: // heure fermeture
        closeTime();  // affichage de l'heure de fermeture de la porte
        break;
      case 5: // temperature
        read_temp(true); // read temperature celsius=true
        break;
      case 6: //lumiere
        lumiere(); // lecture et affichage de la lumiere
        break;
      case 7: // lumiere matin
        affiLumMatin(); // affichage de la lumiere du matin
        break;
      case 8: // lumiere soir
        affiLumSoir(); // affichage de la lumiere du soir
        break;
      case 9: // choix Ouverture / Fermeture
        affiChoixOuvFerm(); // choix
        break;
      case 10:  // fin de course Fermeture
        affiFinDeCourseFermeture(); // fin de course Haut
        break;
      case 11:  // fin de course ouverture
        affiFinDeCourseOuverture();  // fin de course ouverture
        break;
      case 12:  // tension batterie commandes
        affiTensionBatCdes(); //
        break;
      case 13:  // tension batterie servo
        affiTensionBatServo(); //
        break;
      case 14:  // commande manuelle
        affiPulsePlusCptRoue(); // affichage pulse et comptage roue codeuse
        break;
    }
  }
}

/* interruption du watchdog */
///----Watchdog Interrupt Service est exécité lors d'un timeout du WDT----
ISR(WDT_vect) {
  if (f_wdt == 0) {
    f_wdt = 1; // flag
  }
}

///-----entree dans le mode sleep-----
void enterSleep(void) {
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_enable();
  sleep_mode(); //Entre dans le mode veille choisi
  // Le programme va reprendre ici après le timeout du WDT
  sleep_disable(); // La 1ère chose à faire est de désactiver le mode veille
}

///-----routine de gestion du watchdog-----
void routineGestionWatchdog() {
  if ((f_wdt == 1 ) and (!boitierOuvert)) { // si le boitier est ferme et watchdog=1
    if ( !monServo.get_m_servoAction()) { // servo à l'arrêt
      tempsWatchdog--;
      if (tempsWatchdog <= 0) {
        //indicateur led 13 pour le mode sleep
        digitalWrite(LED_PIN, HIGH);
        delay(10);
        digitalWrite(LED_PIN, LOW);
        if (batterieFaible) { // affichage si la batterie est faible
          char chaine[27] = "";
          for (byte i = 0; i < 27 ; i++) {
            chaine[i] = pgm_read_byte(affichageBatterieFaible + i);
          }
          radio.messageRadio(chaine);// on envoie le message
        }
        // informations à afficher
        if (radio.get_m_radio()) {
          displayTime();
          read_temp(typeTemperature); // read temperature celsius=true
          affiTensionBatCdes(); // affichage tension batterie commandes sur terminal
          affiTensionBatServo(); // affichage tension batterie servomoteur sur terminal
          affiPulsePlusCptRoue();
          affiFinDeCourseFermeture();
          affiFinDeCourseOuverture();
          lumiere();
          radio.envoiUnsignedInt( memoireLibre, boitierOuvert, ";"); // envoi message radio : memoire sram restante
          radio.chaineVide();
        }
        tempsWatchdog = bouclesWatchdog ; // initialisation du nombre de boucles
        lum.set_m_compteurWatchdogLumiere(lum.get_m_compteurWatchdogLumiere() + 1);// incrementation compteur watchdog lumiere
      }
      f_wdt = 0;
      enterSleep(); //Revenir en mode veille
    }
  }
}

///-----routine affichage au demarrage-----
void affichageDemarrage (byte colonne) {
  char temp[16] = {0};
  char temp1[16] = {0};
  for (byte i = colonne; i < 16 + colonne; i++) { // boucle pour afficher 16 caractères sur le lcd
    temp[i - colonne] = pgm_read_byte(affichageBonjour + i); // utilisation du texte présent en mèmoire flash
  }
  colonne += 17;
  for (byte i = colonne; i < 16 + colonne; i++) { // boucle pour afficher 16 caractères sur le lcd
    temp1[i - colonne] = pgm_read_byte(affichageBonjour + i); // utilisation du texte présent en mèmoire flash
  }
  mydisp.bonjour(temp, temp1); // affichage version sur les deux lignes
}

/* setup */
void setup() {

  Serial.begin(9600);
  pinMode(LED_PIN, OUTPUT); // led broche 13

  rtc.init();// initialisation de l'horloge etverification de la presence de la carte RTC / memoire 24C32

  mydisp.init(); // initialisation for text LCD adapter

  byte colonne = 0;
  // affichage version au demarrage ou  defaut RTC
  if (rtc.testPresenceCarteRTC()) {
    affichageDemarrage (colonne);
  } else {
    colonne = 34;// position dans la mèmoire flash
    affichageDemarrage(colonne);
  }

  incrementation = menuManuel; // pour affichage menu
  deroulementMenu (incrementation); // affichage du menu

  lum.set_m_ouverture( rtc.i2c_eeprom_read_byte( 0x14));// lecture du type d'ouverture @14  de l'eeprom de la carte rtc (i2c @ 0x57)
  delay(10);
  lum.set_m_fermeture( rtc.i2c_eeprom_read_byte( 0x15)); // lecture du type de fermeture @15   de l'eeprom de la carte rtc (i2c @ 0x57)
  delay(10);

  byte val1 = rtc.i2c_eeprom_read_byte( 0x16); // lecture pf lumière du matin (byte)
  delay(10);
  byte val2 = rtc.i2c_eeprom_read_byte( 0x17); // lecture Pf lumiere du matin (byte)
  delay(10);
  lum.set_m_lumMatin((val2 << 8) + val1);// mots 2 byte vers mot int lumMatin

  val1 = rtc.i2c_eeprom_read_byte( 0x18); // lecture pf lumière du soir (byte)
  delay(10);
  val2 = rtc.i2c_eeprom_read_byte( 0x19); // lecture Pf lumiere du soir (byte)
  delay(10);
  lum.set_m_lumSoir((val2 << 8) + val1);// mots 2 byte vers mot int lumSoir

  val1 = rtc.i2c_eeprom_read_byte( 0x20); // lecture pf fin de course haut (byte)
  delay(10);
  val2 = rtc.i2c_eeprom_read_byte( 0x21); // lecture Pf fin de course haut (byte)
  delay(10);
  codOpt.set_m_finDeCourseFermeture ((val2 << 8) + val1);  // mots 2 byte vers mot int finDeCourseFermeture

  val1 = rtc.i2c_eeprom_read_byte( 0x22); // lecture pf fin de course bas (byte)
  delay(10);
  val2 = rtc.i2c_eeprom_read_byte( 0x23); // lecture Pf fin de course bas (byte)
  delay(10);
  codOpt.set_m_finDeCourseOuverture ((val2 << 8) + val1);  // mots 2 byte vers mot int finDeCourseOuverture

  attachInterrupt(1, myInterruptINT1, FALLING); // validation de l'interruption sur int1 (d3)
  attachInterrupt(0, myInterruptINT0, CHANGE); // validation de l'interruption sur int0 (d2)

  tools.setupPower(); // initialisation power de l'arduino

  tools.setup_watchdog(9); // initialisation : maxi de 8 secondes pour l'it du watchdog

  radio.init();//initialisation radio

  monServo.init(); // initialisation du servo moteur et du relais

  codOpt.init();// initialisation de la position de la roue codeuse

  if (!testServoMoteur) {
    monServo.servoOuvFerm(batterieFaible, reduit);// mise sous tension du servo et ouverture de la porte
  }
}

/* loop */
void loop() {

  lectureClavier(); // lecture du clavier
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

  if (testServoMoteur) {
    testServo(); // reglage du servo plus test de la roue codeuse et du servo, à l'aide de la console
  }

  radio.testSwitchEmissionRadio(); // test du switch emission radio on/off

  memoireLibre = freeMemory(); // calcul de la  memoire sram libre

  ouvFermLum() ;  // ouverture/fermeture par test de la lumière

  batterieFaible = accusCde.accusFaible(); // test de la batterie commande < 4.8v

  ouverturePorte();
  fermeturePorte();

  routineTestFermetureBoitier(); // test fermeture boitier
  routineTestOuvertureBoitier();// test ouvertuer boitier

  routineInterruptionBp(); // routine interruption Bp

  routineInterrruptionAlarme2() ; // routine alarme 2
  routineInterruptionAlarme1(); // routine alarme 1

  routineGestionWatchdog(); // routine de gestion du watchdog
}


