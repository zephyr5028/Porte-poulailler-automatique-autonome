/* ServoMoteur.cpp
  définitions de la classe ServoMoteur
  gestion du servo moteur au travers de la bibliothèque ServoTimer2
*/

#include "ServoMoteur.h"

//constructeur avec debug si nécessaire
ServoMoteur::ServoMoteur( byte pinCde, byte pinRelais, const byte pinSecuriteHaute, const int pulseStop, const int pulseOuvFerm ,
                          const int pulseReduit, boolean debug) :  m_pinCde(pinCde), m_pinRelais(pinRelais),
  m_pulseStop(pulseStop),  m_pulseOuvFerm(pulseOuvFerm), m_pinSecuriteHaute(pinSecuriteHaute),
  m_pulseReduit(pulseReduit), m_debug(debug), m_pulse(pulseStop)
{
}

ServoMoteur::~ServoMoteur()
{
}

void ServoMoteur::init () {
  pinMode(m_pinRelais, OUTPUT); // relais servo's pin  an OUTPUT
  digitalWrite(m_pinRelais, LOW); // mise hors tension du relais du servo
  ServoTimer2:: attach(m_pinCde); // use digital pin for commande du servo
  // on démarre à une valeur censée être la moitié de l'excursion totale de l'angle réalisé par le servomoteur
  ServoTimer2::write(m_pulseStop);  // value should usually be 750 to 2200 (environ 1500 = stop)

}

//------mise sous tension du servo et ouverture de la porte-------
// value should usually be 500 to 2500 (1280 = stop)
bool ServoMoteur::servoOuvFerm(boolean batterieFaible, bool servoAction, bool ouvFerm, bool reduit)
{
  if (m_debug) {
    Serial.print(batterieFaible);
    Serial.print(" ");
    Serial.print(servoAction);
    Serial.print("  ");
    Serial.print(ouvFerm);
    Serial.print("  ");
    Serial.println(reduit);
  }
  if (!batterieFaible and  !servoAction) { // si la batterie n'est pas faible et le servo non en action
    digitalWrite(m_pinRelais, HIGH); // mise sous tension du servo
    // modification vitesse ouverture : 1500 - (140 ou 70)  / fermeture : 1500 + (140 ou 70)
    if (reduit) m_pulse = m_pulseOuvFerm; else m_pulse = m_pulseReduit;
    if (ouvFerm) write(m_pulse = m_pulseStop - m_pulse); else  write(m_pulse = m_pulseStop + m_pulse);
    delay(100);
    return servoAction = true; // servo en action
  } else {
    return servoAction;
  }
}

//------modificaton de la vitesse-------
// value should usually be 500 to 2500 (1280 = stop)
void ServoMoteur::servoOuvFermVitesse( bool servoAction, bool ouvFerm, bool reduit)
{
  if (servoAction ) {
    // modification vitesse ouverture : 1500 - (140 ou 70)  / fermeture : 1500 + (140 ou 70)
    if (reduit) m_pulse = m_pulseOuvFerm; else m_pulse = m_pulseReduit;
    if (ouvFerm) write(m_pulse = m_pulseStop - m_pulse); else  write(m_pulse = m_pulseStop + m_pulse);
  }
}

//-----mise hors tension relais du servo-----
bool ServoMoteur::servoHorsTension (bool ouvFerm) {
  digitalWrite(m_pinRelais, LOW);
  if (ouvFerm) !ouvFerm; else ouvFerm;
  return ouvFerm;
}

//-----test de la broche securite haute-----
unsigned int ServoMoteur::testSecuriteHaute(unsigned int compteRoueCodeuse, unsigned int finDeCourseOuverture) {
  if (!digitalRead(m_pinSecuriteHaute)) {
    delay(200); // attente fin de l'arrêt complet du servo
    return finDeCourseOuverture;
  } else {
    return compteRoueCodeuse;
  }
}

//-----accesseur - getter-----
int ServoMoteur::get_m_pulse() {
  return m_pulse;
}

//-----mutateur - setter-----
int ServoMoteur::set_m_pulse(int pulse) {
  m_pulse = pulse;
}

