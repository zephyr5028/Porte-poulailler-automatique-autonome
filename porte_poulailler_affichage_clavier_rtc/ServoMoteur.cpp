/* ServoMoteur.cpp
  définitions de la classe ServoMoteur
  gestion du servo moteur au travers de la bibliothèque ServoTimer2
*/

#include "ServoMoteur.h"

//constructeur avec debug si nécessaire
ServoMoteur::ServoMoteur( byte pinCde, byte pinRelais, const int pulseStop, const int pulseOuvFerm ,
                          const int pulseReduit, boolean debug) :  m_pinCde(pinCde), m_pinRelais(pinRelais),
  m_pulseStop(pulseStop),  m_pulseOuvFerm(pulseOuvFerm), m_pulseReduit(pulseReduit), m_debug(debug)
{
  pinMode(m_pinRelais, OUTPUT); // relais servo's pin  an OUTPUT
  digitalWrite(m_pinRelais, LOW); // mise hors tension du relais du servo
  attach(m_pinCde); // use digital pin for commande du servo
  // on démarre à une valeur censée être la moitié de l'excursion totale de l'angle réalisé par le servomoteur
  write(m_pulseStop);  // value should usually be 750 to 2200 (environ 1500 = stop)
}

ServoMoteur::~ServoMoteur()
{
}

//------mise sous tension du servo et ouverture de la porte-------
bool ServoMoteur::servoOuverture(boolean batterieFaible, bool servoAction)
{
  if (!batterieFaible and  !servoAction) { // si la batterie n'est pas faible et le servo non en action
    digitalWrite(m_pinRelais, HIGH); // mise sous tension du servo
    write(m_pulseStop - m_pulseOuvFerm); // modification vitesse servo 1500 - 140
    delay(100);
    return servoAction = true; // servo en action
  }
}
