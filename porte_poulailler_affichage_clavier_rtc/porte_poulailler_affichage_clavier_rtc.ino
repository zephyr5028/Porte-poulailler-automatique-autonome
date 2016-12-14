/* porte-poulailler : affichage + clavier + rtc */
//pb avec l'ouverture et la fermeture au bout d'un moment : utilisation du contact fermeture....tjs pas resolu !!!
//first commit sur gihub le 01 12 2016
// 20 septembre 2016
/* 20 septembre 2016 :
    crétion d'une classe Clavier
    inclure le test servoAction dans le comptage de la roue codeuse
*/
/* 19 septembre 2016 :
    - suppression du blocage au demarrage (si = 200)
*/
/* date 18 septembre 2016 :
    En cas de probleme avec le sens de rotation du servo :
     - variable globale : sens
     - arret fin de course ( contact) quelque soit le sens
     - diminution de la vitesse reduite dans la descente de la trappe (inertie)
     Utilisation de fichiers *.h pour la compréhension du programme
*/
/* date : 17 septembre 2016 :
    - problème avec servotimer2  :
    le pulse en ms ne fonctionne pas donc le servo est tjs dans le même sens (hs).
    Les valeurs du pulse étaient mauvaise.
    Il faut :
    90° : 1500ms
    o° : 1000ms
    180° : 2000ms
*/
/* date : 10 septembre 2016 :
    - affichage compteur roue codeuse envoi radio (ligne 1138)
    - delay d'attente de la remontée du servo augmentée : 300 (ligne 1252)
*/
/*
  with ServoTimer2 et VirtualWire
  il faut aussi commenter typedef dans la bibliothèque servotimer2
*/

/*-----debug-----*/
const boolean DEBUG = true; // positionner debug pour l'utiliser ou pas
const boolean RADIO = true; // positionner radio pour l'utiliser ou pas
const bool TESTSERVO = false; // pour utiliser ou non le test du servomoteur

/* variables */
#include "A0_variables.h"

/* fonctions */
#include "A1_fonctions.h"

/* I2C */
#include "I2C.h"

/* --- ds3231 RTC --- */
#include "RTC_DS3231.h"

/* Progmem  mémoire flash */
#include "progmem.h"

/* lecture Clavier */
#include "Clavier.h"
Clavier clav(menuManuel); // class Clavier avec le nombre de lignes du menu
#include "clavier.h"

/* radio 43MHz */
/*-----virtualWire pour la liaison RF 433Mhz-----*/
#include <VirtualWire.h>
#include "Radio.h"
Radio rad(RADIO,DEBUG); // class Radio avec true pour envoi par radio

/* temperature */
#include "temperature.h"

/* affichages */
#include "affichages.h"

/* lumiere */
#include "lumiere.h"

/* reglage de la date */
#include "reg_date.h"

/* regalge time */
#include "reg_time.h"

/* reglage de l'heure d'ouverture */
#include "reg_H_ouv.h"

/* reglage de l'heure de fermeture */
#include "reg_H_ferm.h"

/* eeprom at24c32 */
#include "eeprom.h"

/* reglage de la lumiere du matin */
#include "reg_lum_matin.h"

/* reglage de la lumiere du soir */
#include "reg_lum_soir.h"

/* choix pour l'ouverture et la fermeture : lumiere ou heure */
#include "choix_ouv_ferm.h"

/* fins de course */
#include "fins_course.h"

/* batteries */
#include"batterie.h"

/* servo - montée et descente de la porte */
#include "servo.h"

/* roue codeuse - montee descente manuelle */
#include "roue_code.h"

/* afficheur - temporisation - eclairage */
#include "afficheur.h"

/* interruptions */
#include "interruptions.h"

/* menu */
#include "menu.h"

/* test du servo avec la console arduino */
#include "testServo.h"

/* watchdog */
#include "watchdog.h"

/* setup */
#include "setup.h"

/* loop */
#include "loop.h"





