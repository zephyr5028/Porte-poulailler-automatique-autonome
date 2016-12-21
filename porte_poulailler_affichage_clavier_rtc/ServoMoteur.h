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
    ServoMoteur( byte pinCde, byte pinRelais, const int pulseStop = 1500, const int pulseOuvFerm = 140,
                 const int pulseReduit = 70,  boolean debug = false);

    ~ServoMoteur(); // destructeur

    //mise sous tension du servo et ouverture de la porte
    bool servoOuverture(bool batterieFaible, bool servoAction);


  protected:

    boolean m_debug ; // pour affichage console si nécessaire
    byte m_pinCde; // numero de la broche reliée à la commande du servo moteur
    byte m_pinRelais; // numero de la broche reliée à la commande du relais pour le servo moteur
    const int m_pulseStop; // value should usually be 750 to 2200 (1500 = stop)
    const int m_pulseOuvFerm; // vitesse ouverture -140, fermeture 140
    const int m_pulseReduit; // vitesse ouverture réduite -70 , fermeture réduite 70
    //   boolean m_servoAction ; // servo à l'arrêt
    //   boolean m_ouvFerm; //// ouverture true et fermeture false

};

#endif

