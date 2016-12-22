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
    ServoMoteur( const byte pinCde, const byte pinRelais, const byte pinSecuriteHaute, const int pulseStop = 1500, const int pulseOuvFerm = 140,
                 const int pulseReduit = 70,  boolean debug = false);

    ~ServoMoteur(); // destructeur

    void init();
    //mise sous tension du servo et ouverture de la porte
    bool servoOuvFerm(bool batterieFaible, bool servoAction, bool ouvFerm, bool reduit);
    void servoOuvFermVitesse( bool servoAction, bool ouvFerm, bool reduit);    //modificaton de la vitesse
    bool servoHorsTension(bool ouvFerm); // mise hors tension relais du servo
    //test de la broche securite haute
    unsigned int testSecuriteHaute(unsigned int compteRoueCodeuse, unsigned int finCourseOuverture);
    int get_m_pulse(); // accesseur - getter
    int set_m_pulse(int pulse); // mutateur - setter


  protected:

    boolean m_debug ; // pour affichage console si nécessaire
    const  byte m_pinCde; // numero de la broche reliée à la commande du servo moteur
    const byte m_pinRelais; // numero de la broche reliée à la commande du relais pour le servo moteur
    const byte m_pinSecuriteHaute; // pin pour la securite d'ouverture de porte
    const int m_pulseStop; // value should usually be 750 to 2200 (1500 = stop)
    const int m_pulseOuvFerm; // vitesse ouverture -140, fermeture 140
    const int m_pulseReduit; // vitesse ouverture réduite -70 , fermeture réduite 70
    int m_pulse;// vitesse du servo à un instant donné

};

#endif

