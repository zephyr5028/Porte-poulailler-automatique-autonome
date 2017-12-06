/** PowerTools.h
  prototype de la classe Power
    - Gestion de la consommation en courant
    - Watchdog
    - Memory free
    - outils
*/

#ifndef PowerTools_h
#define PowerTools_h

#include <avr/power.h>
#include <avr/sleep.h>
#include <avr/wdt.h>
#include <MemoryFree.h> //https://github.com/McNeight/MemoryFree
#include "Arduino.h"

class PowerTools
{

  public:

    // constructeur
    PowerTools();
    PowerTools(const byte buzzer_pin, const boolean buzzer);
    PowerTools( const byte buzzer_pin, const boolean buzzer, const boolean debug = false ); // constructeur avec la broche du buzzzer et si present sur la carte , debug
    ~PowerTools(); // destructeur

    void  setupPower();//initialisation power
    //initialisation du watchdog - paramètre : 0=16ms, 1=32ms, 2=64ms, 3=128ms, 4=250ms, 5=500ms, 6=1 sec,7=2 sec, 8=4 sec, 9=8 secondes-----
    void setup_watchdog(int parametreTemps);
    byte tailleChaine (char *chaine);//taille d'une chaine de caractères
    void setupBuzzer(int temps); //initialisation du buzzer
    void fonctionnementBuzzer(unsigned int compteur, int temps); //fonctionnement du buzzer en fonction du parametre compteurWatchdogLumiere
    unsigned int analogReadReference(void) ; // Mesure la référence interne à 1.1 volts pour connaitre la tension d'alimentation


  protected:

    const boolean m_debug ; // pour affichage console si nécessaire
    const byte m_buzzer_pin ; // broche du buzzer
    const boolean m_buzzer ; // presence du buzzer sur la carte : true


};

#endif

