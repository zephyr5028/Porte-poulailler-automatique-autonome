/* ServoMoteur.h
  prototype de la classe ServoMoteur
  gestion du servo moteur au travers de la bibliothèque ServoTimer2
*/

#ifndef ServoMoeur_h
#define ServoMoteur_h

#include "Arduino.h"
#include <ServoTimer2.h> // the servo library

class ServoMoteur : public ServoTimer2
{
  public:

    // constructeur avec numero de broche et debug si necessaire
    ServoMoteur( byte pinCde, byte pinRelais, boolean debug = false);

    ~ServoMoteur(); // destructeur


  protected:

    boolean m_debug ; // pour affichage console si nécessaire
    byte m_pinCde; // numero de la broche reliée à la commande du servo moteur
    byte m_pinRelais; // numero de la broche reliée à la commande du relais pour le servo moteur

};

#endif

