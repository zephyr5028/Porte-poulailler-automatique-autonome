/** PowerTools.h
  définitions de la classe PowerToos
    - Gestion de la consommation en courant
    - Watchdog
    - Memory free
    - outils
    - buzzer
*/


#include "PowerTools.h"

PowerTools::PowerTools() :  m_buzzer_pin(7), m_buzzer(false), m_debug(false)
{

}
PowerTools::PowerTools(const byte buzzer_pin, const boolean buzzer) :  m_buzzer_pin(buzzer_pin), m_buzzer(buzzer), m_debug(false)
{

}

/* sucharge du constructeur */
PowerTools::PowerTools( const byte buzzer_pin, const boolean buzzer, const boolean debug) :   m_buzzer_pin(buzzer_pin), m_buzzer(buzzer), m_debug(debug)
{

}

PowerTools::~PowerTools()
{

}

///-----initialisation power-----
void  PowerTools::setupPower() {
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

///-----initialisation du watchdog - paramètre : 0=16ms, 1=32ms, 2=64ms, 3=128ms, 4=250ms, 5=500ms, 6=1 sec,7=2 sec, 8=4 sec, 9=8 secondes-----
void PowerTools::setup_watchdog(int parametreTemps) {
  byte bb;
  int ww;
  if (parametreTemps > 9 ) parametreTemps = 9;
  bb = parametreTemps & 7;
  if (parametreTemps > 7) bb |= (1 << 5);
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

///-----taille d'une chaine de caractères-----
byte PowerTools::tailleChaine (char * chaine) {
  byte i(1);
  while (chaine[i] != '\0') i++;
  return i;
}

///-----initialisation du buzzer et test----
void PowerTools::setupBuzzer (int temps) {
  if (m_buzzer) {
    digitalWrite(m_buzzer_pin, HIGH);
    pinMode(m_buzzer_pin, OUTPUT); // buzzer 3,5 à 5,5v <25ma 2300hz +/-500hz
    digitalWrite(m_buzzer_pin, LOW);
    delay(temps);
    digitalWrite(m_buzzer_pin, HIGH);
  }
}

///-----fonctionnement du buzzer en fonction du parametre compteurWatchdogLumiere----
void PowerTools::fonctionnementBuzzer (unsigned int compteur, int temps) {
  if (m_buzzer) {
    //si le compteur est > 0 , le buzzer fonctionne
    if (compteur > 0) {
      digitalWrite(m_buzzer_pin, LOW);
      delay(temps);
      digitalWrite(m_buzzer_pin, HIGH);
    }
  }
}

///----- Mesure la référence interne à 1.1 volts pour connaitre la tension d'alimentation------
unsigned int PowerTools::analogReadReference(void) {
  /* Elimine toutes charges résiduelles */
  ADMUX = 0x4F;
  delayMicroseconds(5);
  /* Sélectionne la référence interne à 1.1 volts comme point de mesure, avec comme limite haute VCC */
  ADMUX = 0x4E;
  delayMicroseconds(200);
  /* Active le convertisseur analogique -> numérique */
  ADCSRA |= (1 << ADEN);
  /* Lance une conversion analogique -> numérique */
  ADCSRA |= (1 << ADSC);
  /* Attend la fin de la conversion */
  while(ADCSRA & (1 << ADSC));
  /* Récupère le résultat de la conversion */
  return ADCL | (ADCH << 8);
}



