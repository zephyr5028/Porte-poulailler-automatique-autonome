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
    void servoOuvFerm(bool batterieFaible, bool reduit);
    void servoVitesse( bool reduit);    //modificaton de la vitesse
    // mise hors tension relais du servo et test de la sécurité haute
    unsigned int servoHorsTension(unsigned int compteRoueCodeuse, unsigned int finCourseOuverture);
    void relaisSousTension(); //relais sous tension
    void relaisHorsTension();//relais hors tension
    int get_m_pulse(); // accesseur - getter
    void set_m_pulse(int pulse); // mutateur - setter
    bool get_m_ouvFerm(); // accesseur - getter
    void set_m_ouvFerm(bool ouvFerm); // mutateur - setter
    bool get_m_servoAction(); // accesseur - getter
    void set_m_servoAction(bool servoAction); // mutateur - setter


  protected:

    const boolean m_debug ; // pour affichage console si nécessaire
    const  byte m_pinCde; // numero de la broche reliée à la commande du servo moteur
    const byte m_pinRelais; // numero de la broche reliée à la commande du relais pour le servo moteur
    const byte m_pinSecuriteHaute; // pin pour la securite d'ouverture de porte
    const int m_pulseStop; // value should usually be 750 to 2200 (1500 = stop)
    const int m_pulseOuvFerm; // vitesse ouverture -140, fermeture 140
    const int m_pulseReduit; // vitesse ouverture réduite -70 , fermeture réduite 70
    int m_pulse;// vitesse du servo à un instant donné
    bool m_ouvFerm;// ouverture / fermeture - true ouverture
    bool m_servoAction; // servo en action - false servo à l'arrêt

};

#endif


