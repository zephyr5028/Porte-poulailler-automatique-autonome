/* variables */

/* RTC_DS3231 */
const byte rtcINT = 5; // digital pin D5 as l'interruption du rtc ( alarme)
byte alarm_1 = 1; // alarme 1
byte alarm_2 = 2; //alarme 2

/* roue codeuse */
// digital pin D7 has a détecteur roue codeuse
const byte roueCodeuse = 7;//digital pin D7 pour entrée roue codeuse
volatile unsigned int compteRoueCodeuse(200);  // un compteur de position
volatile boolean interruptRoueCodeuse;    // gestion de l'anti-rebonds
volatile boolean positionRoueCodeuse;
const byte securiteHaute = 12; // pin 12 pour la securite d'ouverture de porte

/* servo */
const byte servoPin = 4; // pin D4 relais du servo
const byte servoCde = 8; // pin D8 cde du servo
const int pulseStop = 1500; // value should usually be 750 to 2200 (1500 = stop)
int pulse = pulseStop; // stop
const int pulseOuverture = pulseStop - 140; // vitesse montée
const int pulseFermeture = pulseStop + 140; // vitesse descente
const int pulseOuvertureReduit = pulseStop - 70; // vitesse montée réduite
const int pulseFermetureReduit = pulseStop + 70; // vitesse descente réduite
unsigned int finDeCourseFermeture = 250; // initialisation de la valeur de la fin de course fermeture
unsigned int finDeCourseOuverture = 150; // initialisation de la valeur de la fin de course ouverture
boolean servoAction(false) ; // servo à l'arrêt
boolean ouverture = false; // montee de la porte
boolean fermeture = false; // descente de la porte
byte boucleTempo(0); // boucle tempo pour l'affichage lcd
byte tempoLcd(5);// tempo pour l'afficahge lcd

/* watchdog - Optimisation de la consommation */
#include <avr/power.h>
#include <avr/sleep.h>
#include <avr/wdt.h>
volatile int f_wdt = 1; // flag watchdog
const byte bouclesWatchdog = 8; // nombre de boucles du watchdog
byte tempsWatchdog = bouclesWatchdog; // boucle temps du chien de garde
boolean batterieFaible = false; // si batterie < 4,8v = true

/* Affichage */
byte incrementation = 0; // incrementation verticale
byte decalage = 0; // decalage à droite pour reglage
boolean reglage = false; // menu=false ou reglage=true
int temps = 0; // temps entre deux affichages de l heure
const int debounce = 250; // debounce latency in ms
unsigned long tempoDebounce(debounce); // temporisation pour debounce
boolean relacheBpOF = true; // relache Bp
byte BpOF = 9; // pin D9 bouton poussoir ouverture / fermeture
byte interOuvBoitier = 6; //pin D6 interrupteur ouverture boitier
boolean retroEclairage = true; // etat retro eclairage
boolean  boitierOuvert = true; // le boitier est ouvert

/* Clavier */
byte oldkey = -1;
boolean relache = false; // relache de la touche
byte touche(-1); // valeur de la touche appuyee
const byte sensorClavier = 1; //pin A1 pour le clavier

/* lumiere */
byte ouve = 1; // ouverture 1 donc heure
byte ferm = 0; // fermeture 0 donc lumière
int sensorValue; // valeur de la lumiere
unsigned int lumMatin = 300; // valeur de la lumière du matin
unsigned int lumSoir = 900; // valeur de la lumiere du soir
const byte tempsLum = 2 ; // boucles pour valider l'ouverture / fermeture avec la lumière (compteur watchdog)
unsigned int  compteurWatchdogLumiere = 0; // compteur watchdog lumiere

/* emetteur 433Mhz */
const byte pinEmRadio = 10; // pin D10 emetteur radio

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


