/* Accus.h
  prototype de la classeAccus
  bloc accus NIMH
*/

#ifndef Accus_h
#define Accus_h

#include "Arduino.h"

class Accus
{
  public:
    // constructeur avec debug
    Accus( boolean debug = false );
    ~Accus(); // destructeur


    void init(); // initialisation

  protected:
    boolean m_debug ; // pour affichage console si nécessaire

};

#endif

