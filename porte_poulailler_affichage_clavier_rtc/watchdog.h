/* watchdog */

//----Watchdog Interrupt Service est exécité lors d'un timeout du WDT----
ISR(WDT_vect) {
  if (f_wdt == 0) {
    f_wdt = 1; // flag
  }
}

//-----paramètre : 0=16ms, 1=32ms, 2=64ms, 3=128ms, 4=250ms, 5=500ms, 6=1 sec,7=2 sec, 8=4 sec, 9=8 sec-----
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
    if ((( compteRoueCodeuse <= finDeCourseB + 1 ) or  (compteRoueCodeuse >= finDeCourseH - 1 )) and !servoAction) {
      tempsWatchdog--; // decrementation
      if (tempsWatchdog <= 0) {
        if (batterieFaible) { // affichage si la batterie est faible
          // N.B. La constante VW_MAX_MESSAGE_LEN est fournie par la lib VirtualWire
          //char chaine1[VW_MAX_MESSAGE_LEN - 1] = "Batterie faible !!!!";
          char chaine1[21] = "Batterie faible !!!!";
          messageRadio(chaine1);// on envoie le message
        }
        // informations à afficher
        displayTime();
        read_temp(true); // read temperature celsius=true
        affiTensionBatCdes(); // affichage tension batterie commandes sur terminal
        affiTensionBatServo(); // affichage tension batterie servomoteur sur terminal
        affiFinDeCourseHaut();
        affiFinDeCourseBas();
        affiPulsePlusCptRoue();
        affiChoixOuvFerm();
        lumiere();
        chaineVide();
        if (DEBUG) {
          Serial.print(F("compt watchdog lum : x * 8s  = ")); Serial.println(compteurWatchdogLumiere);
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
        compteurWatchdogLumiere++; //incrementation du compteur watchdog lumiere pour ne pas prendre en compte une ombre
      }
      f_wdt = 0;
      enterSleep(); //Revenir en mode veille
    }
  }
}
