/* PowerTools.h
  prototype de la classe Power
    - Gestion de la consommation en courant
    - Watchdog
    - Memory free
    - outils
*/

#ifndef PowerTools_h
#define PowerTools_h

#include <MemoryFree.h> //https://github.com/McNeight/MemoryFree
#include "Arduino.h"

class PowerTools 
{

  public:

    // constructeur
    PowerTools();
    PowerTools( const boolean debug = false ); // constructeur avec debug
    ~PowerTools(); // destructeur

    void init();// initialisation


  protected:

    const boolean m_debug ; // pour affichage console si nécessaire


};

#endif

