/* 
 *  
 *  fonctions du programme  : Porte du poulailler
 *  
 */
 
#ifndef AA_fonctions_h
#define AA_fonctions_h

/* clavier */
//-----lecture clavier------
void  lectureClavier();

//---- temporisation pour l'affichage: affichage du menu lorsque temps est > boucleTemps-----
//void temporisationAffichage(const int boucleTemps);

//----routine lecture et ecriture date and time-----
void ecritureDateTime();

/* affichages */
//-----routine display Date-----
void displayDate();

//-----routine display Time-----
void displayTime ();

//----routine door open time-----
void openTime();

//-----routine door close time-----
void closeTime();

//------affichage pulse et comptage roue codeuse------
void affiPulsePlusCptRoue();

/* afficheur */
//-----retro eclairage de l'afficheur-----
void eclairageAfficheur();

/* batteries */
//-------affichage tension batterie commandes
void affiTensionBatCdes();

//-------affichage tension batterie servo-moteur
void affiTensionBatServo();

/* choix pour l'ouverture et la fermeture :
     - heure
     - lumiere
*/
//------affichage du choix de l'ouverture et la fermeture------
void affiChoixOuvFerm();

//-----routine choix ouverture fermeture-----
void choixOuvFerm ();

/* reglage heure fermeture */
//-----reglage de l'heure de fermeture------
void reglageHeureFermeture();

/* reglage heure ouverture */
//-----routine de reglage de l'heure d'ouverture-----
void reglageHeureOuverture();

/* reglage de la date */
//----routine reglage jour semaine, jour, mois, annee-----
void reglageDate ();

/* reglage de la lumiere du matin */
//------affichage de la lumiere du matin------
void affiLumMatin();

/* reglage de la lumiere du soir  */
//------affichage de la lumiere du soir-------
void affiLumSoir();

//------reglage du choix de la lumiere du soir--------
void choixLumSoir();

/* reglage time */
//-----routine reglage heure , minute , seconde-----
void reglageTime ();

/* fins de course ouverture et fermeture */
//------affichage fin de course Fermeture-----
void affiFinDeCourseFermeture();

//------affichage fin de course Ouverture-------
void affiFinDeCourseOuverture();

//------reglage fin de course Fermeture------
void regFinDeCourseFermeture();

//-----regalge fin de course ouverture------
void regFinDeCourseOuverture();

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
void testServo();

/* temperature */
//-----routine lecture température sur ds3231 rtc type celsius=true ,fahrenheit=false------
void read_temp(const boolean typeTemperature);

/* servomoteur :
    -montée
    -descente
*/
//------sequence ouverture de la porte------
void ouverturePorte();

//-----sequence fermeture de la porte-----
void  fermeturePorte();

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
void myInterruptINT0();

//-----routine interruption D3 INT1-----
void myInterruptINT1();

//-----routine interruption Bp-----
void routineInterruptionBp();

//-----routine alarme 2-----
void  routineInterrruptionAlarme2();

//-----routine alarme 1-----
void  routineInterruptionAlarme1();

//-----test ouverture boitier-----
void routineTestOuvertureBoitier();

//-----test fermeture boitier-----
void  routineTestFermetureBoitier();

/* lumiere
  -affichage
*/
//-----routine lecture et affichage de la lumière-----
void lumiere();

//-----ouverture/fermeture par test de la lumière----
void ouvFermLum();

/* menu */
//-----routine affichage menus------
void deroulementMenu (byte increment);

/* interruption du watchdog */
//----Watchdog Interrupt Service est exécité lors d'un timeout du WDT----
ISR(WDT_vect);

//-----entree dans le mode sleep-----
void enterSleep(void);

//-----routine de gestion du watchdog-----
void routineGestionWatchdog();

//-----routine affichage au demarrage-----
void affichageDemarrage (byte colonne);

#endif

