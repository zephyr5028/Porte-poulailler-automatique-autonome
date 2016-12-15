/* fonctions */

/* --- ds3231 RTC --- */
byte decToBcd(byte val);//-----routine decToBcd : Convert normal decimal numbers to binary coded decimal
byte bcdToDec(byte val);//-----routine bcdToDec : Convert binary coded decimal to normal decimal numbers
void ecritureDateTime();//----routine lecture et ecriture date and time

/* affichages */
void displayDate();//-----routine display Date
void displayTime();//-----routine display Time
void openTime();//----routine door open time
void closeTime();//-----routine door close time
void affiPulsePlusCptRoue();//------affichage pulse et comptage roue codeuse

/*afficheur */
void temporisationAffichage();//---- temporisation pour l'affichage, affichage du menu lorsque temps est >...
void eclairageAfficheur();//-----retro eclairage de l'afficheur

/* batteries */
void affiTensionBatCdes();//-------affichage tension batterie commandes
void affiTensionBatServo();//-------affichage tension batterie servo-moteur
void accusFaible();//-----batterie cdes < 4 volt

/* choix pour l'ouverture et la fermeture */
void affiChoixOuvFerm();//------affichage du choix de l'ouverture et la fermeture
void choixOuvFerm ();//-----routine choix ouverture fermeture

/* Clavier */
void lectureClavier(); //-----lecture clavier

/* eeprom */
void i2c_eeprom_write_byte( int deviceaddress, unsigned int eeaddress, byte data );//-----ecriture dans l'eeprom at24c32 de la carte rtc
byte i2c_eeprom_read_byte( int deviceaddress, unsigned int eeaddress );//-----lecture de l'eeprom at24c32 de la carte rtc

/* fins de course fermeture et ouverture */
void affiFinDeCourseFermeture();//------affichage fin de course fermeture
void affiFinDeCourseOuverture();//------affichage fin de course ouverture
void regFinDeCourseFermeture();//------reglage fin de course fermeture
void regFinDeCourseOuverture();//-----regalge fin de course ouverture

/* interruptions */
void myInterruptINT0();//-----routine interruption D2 INT0
void myInterruptINT1();//-----routine interruption D3 INT1
void routineInterruptionBp();//-----routine interruption Bp
void  routineInterrruptionAlarme2();//-----routine alarme 2
void  routineInterruptionAlarme1();//-----routine alarme 1
//void  routineInterruptionBoitierOuvert();//-----routine interruption boitier ouvert
//void  routineTestFermetureBoitier();//-----test fermeture boitier

/* lumiere */
void lumiere(); //-----routine lecture et affichage de la lumière
void ouvFermLum(); //-----ouverture/fermeture par test de la lumière

/* menu */
void deroulementMenu (byte increment);//-----routine affichage menus

/* radio */

/* reglage heure fermeture */
void reglageHeureFermeture();//-----reglage de l'heure de fermeture

/* reglage heure ouverture */
void reglageHeureOuverture();//-----routine de reglage de l'heure d'ouverture

/* reglage de la date */
void reglageDate ();//----routine reglage jour semaine, jour, mois, annee

/* reglage de la lumiere du matin */
void affiLumMatin();//------affichage de la lumiere du matin
void choixLumMatin();//-----reglage du choix de la lumiere du matin

/* reglage de la lumier du soir  */
void affiLumSoir();//------affichage de la lumiere du soir
void choixLumSoir();//------reglage du choix de la lumiere du soir

/* reglahe time */
void reglageTime ();//-----routine reglage heure , minute , seconde

/* servo */
void servoOuverture();//------mise sous tension du servo et montee de la porte
void  servoFermeture();//-----mise sous tension du servo et descente de la porte

//------sequence d'ouverture de la porte------
void ouverturePorte();
//-----sequence de descente de la porte-----
void  fermeturePorte();

/* temperature */
void read_temp(boolean type);//-----routine lecture température sur ds3231 rtc type celsius=true ,fahrenheit=false

/* roue codeuse */
void monteeDescenteManuelle();//----montee , descente manuelle, position de la trappe
void compteurRoueCodeuse();//-----compteur roue codeuse

/* watchdog */
ISR(WDT_vect);//----Watchdog Interrupt Service est exécité lors d'un timeout du WDT
void setup_watchdog(int ii);//-----paramètre : 0=16ms, 1=32ms, 2=64ms, 3=128ms, 4=250ms, 5=500ms, 6=1 sec,7=2 sec, 8=4 sec, 9=8 sec
void enterSleep(void);//-----entree dans le mode sleep-----
void routineGestionWatchdog();//-----routine de gestion du watchdog


