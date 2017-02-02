/* PowerTools.h
  définitions de la classe PowerToos
    - Gestion de la consommation en courant
    - Watchdog
    - Memory free
    - outils
*/


#include "PowerTools.h"

PowerTools::PowerTools() :  m_debug(false)
{

}

/* sucharge du constructeur */
PowerTools::PowerTools( const boolean debug) :  m_debug(debug)
{

}

PowerTools::~PowerTools() 
{

}

//-----initialisation-----
void PowerTools::init() {
 
}

/*
//-----initialisation power-----
void  setupPower() {
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
}
*/

/*
//-----initialisation du watchdog - paramètre : 0=16ms, 1=32ms, 2=64ms, 3=128ms, 4=250ms, 5=500ms, 6=1 sec,7=2 sec, 8=4 sec, 9=8 secondes-----
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
*/

/*
//-----taille d'une chaine de caractères-----
byte tailleChaine (char * chaine) {
  byte i(1);
  while (chaine[i] != '\0') i++;
  return i;
}
*/


