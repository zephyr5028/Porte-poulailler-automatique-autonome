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
    PowerTools( const boolean debug = false ); // constructeur avec debug
    ~PowerTools(); // destructeur

    void  setupPower();//initialisation power
    //initialisation du watchdog - paramètre : 0=16ms, 1=32ms, 2=64ms, 3=128ms, 4=250ms, 5=500ms, 6=1 sec,7=2 sec, 8=4 sec, 9=8 secondes-----
    void setup_watchdog(int parametreTemps);
    byte PowerTools::tailleChaine (char * chaine);//taille d'une chaine de caractères


  protected:

    const boolean m_debug ; // pour affichage console si nécessaire


};

#endif

