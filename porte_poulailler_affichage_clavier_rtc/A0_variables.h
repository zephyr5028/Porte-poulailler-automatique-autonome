/* variables */

/* RTC_DS3231 */
const byte rtcINT = 5; // digital pin D5 as l'interruption du rtc ( alarme)
byte alarm_1 = 1; // alarme 1
byte alarm_2 = 2; //alarme 2

/* roue codeuse */
// digital pin D7 has a détecteur roue codeuse
const byte roueCodeuse = 7;//digital pin D7 pour entrée roue codeuse
int compteRoueCodeuse = 200; // compteur roue codeuse, valeur au reset du montage
const byte securiteHaute = 12; // pin 12 pour la securite d'ouverture de porte

/* servo */
#include <ServoTimer2.h> // the servo library
ServoTimer2 monServo;
const byte servoPin = 4; // pin D4 relais du servo
const byte servoCde = 8; // pin D8 cde du servo
const int pulseStop = 1500; // value should usually be 750 to 2200 (1500 = stop)
int pulse = pulseStop; // stop
const int pulseMontee = pulseStop + 140; // vitesse montée
const int pulseDescente = pulseStop - 140; // vitesse descente
const int pulseMonteeReduit = pulseStop + 70; // vitesse montée réduite
const int pulseDescenteReduit = pulseStop - 70; // vitesse descente réduite
const int pulseMonteeReduitSens = pulseStop + 50 ; // pour ralentir la descente en fonction du sens de rotation du servo
const int pulseDescenteReduitSens = pulseStop - 50 ; // pour ralentir la descente en fonction du sens de rotation du servo
unsigned int finDeCourseH = 250; // initialisation de la valeur de la fin de course haute
unsigned int finDeCourseB = 150; // initialisation de la valeur de la fin de course basse
boolean montee = false; // montee de la porte
boolean descente = false; // descente de la porte
bool sens = false; //sens de fonctionnement du servo ... en cas de problème...permet de ne pas bloquer le servomoteur

/* watchdog - Optimisation de la consommation */
#include <avr/power.h>
#include <avr/sleep.h>
#include <avr/wdt.h>
volatile int f_wdt = 1; // flag watchdog
byte bouclesWatchdog = 8; // nombre de boucles du watchdog
byte tempsWatchdog = bouclesWatchdog; // boucle temps du chien de garde
boolean batterieFaible = false; // si batterie < 4,8v = true

/* Affichage */
byte incrementation = 0; // incrementation verticale
byte decalage = 0; // decalage à droite pour reglage
boolean reglage = false; // menu=false ou reglage=true
int temps = 0; // temps entre deux affichages de l heure
boolean servoAction = false ; // servo à l'arrêt
int debounce = 250; // debounce latency in ms
unsigned long tempoDebounce; // temporisation pour debounce
boolean relacheBpOF = true; // relache Bp
byte BpOF = 9; // pin D9 bouton poussoir ouverture / fermeture
byte interOuvBoitier = 6; //pin D6 interrupteur ouverture boitier
boolean retroEclairage = true; // etat retro eclairage
boolean  boitierOuvert = true; // le boitier est ouvert

/* Clavier */
//const int adc_key_val[5] = {50, 200, 400, 600, 800}; // Analogique digital Keyboard Module
//const byte NUM_KEYS = 6;
//int adc_key_in;
//byte key = -1;
byte oldkey = -1;
boolean relache = false; // relache de la touche
byte touche; // valeur de la touche appuyee
const byte sensorClavier = 1; //pin A1 pour le clavier

/* radio */
//int const taille=80;
//char chaine[taille] = ""; // initialisation de la ligne envoyee par radio

/* lumiere */
byte ouve = 1; // ouverture 1 donc heure
byte ferm = 0; // fermeture 0 donc lumière
int sensorValue; // valeur de la lumiere
unsigned int lumMatin = 300; // valeur de la lumière du matin
unsigned int lumSoir = 900; // valeur de la lumiere du soir
byte tempsLum = 2 ; // boucles pour valider l'ouverture - fermeture avec la lumière (compteur watchdog)
unsigned int  compteurWatchdogLumiere = 0; // compteur watchdog lumiere

/* emetteur 433Mhz */
const byte pinEmRadio = 10; // pin D10 emetteur radio

/* interruptions */
volatile boolean interruptBp = false; // etat interruption entree 9
volatile boolean interruptRTC = false; // etat interruption entree 10
volatile boolean interruptOuvBoi = false; // etat interruption entree 6
volatile boolean interruptRoueCodeuse = false; // etat interruption roue codeuse   entree D2 et D7

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
const byte menuFinDeCourseH = 10;
const byte menuFinDeCourseB = 11;
const byte menuTensionBatCdes = 12; // tension batterie commandes
const byte menuTensionBatServo = 13; // tension batterie servo
const byte menuManuel = 14; // nombre de lignes du  menu
const byte colonnes = 16; // colonnes de l'afficheur


