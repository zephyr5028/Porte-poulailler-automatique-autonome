/* ServoMoteur.cpp
  définitions de la classe ServoMoteur
  gestion du servo moteur au travers de la bibliothèque ServoTimer2
*/

#include "ServoMoteur.h"

//constructeur avec debug si nécessaire
ServoMoteur::ServoMoteur( byte pinCde, byte pinRelais, const byte pinSecuriteHaute, const int pulseStop, const int pulseOuvFerm ,
                          const int pulseReduit, const boolean debug) :  m_pinCde(pinCde), m_pinRelais(pinRelais),
  m_pulseStop(pulseStop),  m_pulseOuvFerm(pulseOuvFerm), m_pinSecuriteHaute(pinSecuriteHaute),
  m_pulseReduit(pulseReduit), m_debug(debug), m_pulse(pulseStop), m_ouvFerm(true), m_servoAction(false)
{
}

ServoMoteur::~ServoMoteur()
{
}

//-----initialisation-----
void ServoMoteur::init () {
  pinMode(m_pinRelais, OUTPUT); // relais servo's pin  an OUTPUT
  digitalWrite(m_pinRelais, LOW); // mise hors tension du relais du servo
  ServoTimer2::attach(m_pinCde); // use digital pin for commande du servo
  // on démarre à une valeur censée être la moitié de l'excursion totale de l'angle réalisé par le servomoteur
  ServoTimer2::write(m_pulseStop);  // value should usually be 750 to 2200 (environ 1500 = stop)

}

//------mise sous tension du servo et ouverture fermeture de la porte-------
// value should usually be 500 to 2500 (1280 = stop)
void ServoMoteur::servoOuvFerm(boolean batterieFaible, bool reduit)
{
  if (!batterieFaible and  !m_servoAction) { // si la batterie n'est pas faible et le servo non en action
    ServoMoteur::relaisSousTension(); // relais sous tension
    // modification vitesse ouverture : 1500 - (140 ou 70)  / fermeture : 1500 + (140 ou 70)
    // if (reduit) m_pulse = m_pulseOuvFerm; else m_pulse = m_pulseReduit;
    //  if (m_ouvFerm) ServoTimer2:: write(m_pulse = m_pulseStop - m_pulse); else ServoTimer2:: write(m_pulse = m_pulseStop + m_pulse);
    modificationVitesse(reduit);
    delay(150);
    m_servoAction = true; // servo en action
  }
}

//------modificaton de la vitesse si le servo en action-------
// value should usually be 500 to 2500 (1280 = stop)
void ServoMoteur::servoVitesse( bool reduit)
{
  if (m_servoAction ) {
    // modification vitesse ouverture : 1500 - (140 ou 70)  / fermeture : 1500 + (140 ou 70)
    //if (reduit) m_pulse = m_pulseOuvFerm; else m_pulse = m_pulseReduit;
    //if (m_ouvFerm) ServoTimer2::write(m_pulse = m_pulseStop - m_pulse); else  ServoTimer2::write(m_pulse = m_pulseStop + m_pulse);
    modificationVitesse(reduit);
  }
}

//-----modification de la vitesse-----
void ServoMoteur::modificationVitesse( bool reduit) {
  // modification vitesse ouverture : 1500 - (140 ou 70)  / fermeture : 1500 + (140 ou 70)
  if (reduit) m_pulse = m_pulseOuvFerm; else m_pulse = m_pulseReduit;
  if (m_ouvFerm) ServoTimer2:: write(m_pulse = m_pulseStop - m_pulse); else ServoTimer2:: write(m_pulse = m_pulseStop + m_pulse);
}

//-----mise hors tension relais du servo-----
unsigned int ServoMoteur::servoHorsTension (unsigned int compteRoueCodeuse, unsigned int finDeCourseOuverture) {
  ServoMoteur::relaisHorsTension(); // relais hors tension
  if (!digitalRead(m_pinSecuriteHaute)) {
    delay(200); // attente fin de l'arrêt complet du servo
    m_servoAction = false; // servo arrêt
    return finDeCourseOuverture;
  } else {
    m_servoAction = false; // servo arrêt
    return compteRoueCodeuse;
  }
}

//-----relais hors tension-----
void ServoMoteur::relaisHorsTension () {
  digitalWrite(m_pinRelais, LOW);
  if (m_ouvFerm) !m_ouvFerm; else m_ouvFerm;
}

//-----relais sous tension-----
void ServoMoteur::relaisSousTension() {
  digitalWrite(m_pinRelais, HIGH);
}

//-----accesseur - getter-----
int ServoMoteur::get_m_pulse() {
  return m_pulse;
}

//-----mutateur - setter-----
void ServoMoteur::set_m_pulse(int pulse) {
  m_pulse = pulse;
}

//-----accesseur - getter-----
bool ServoMoteur::get_m_ouvFerm() {
  return m_ouvFerm;
}

//-----mutateur - setter-----
void ServoMoteur::set_m_ouvFerm(bool ouvFerm) {
  m_ouvFerm = ouvFerm;
}

//-----accesseur - getter-----
bool ServoMoteur::get_m_servoAction() {
  return m_servoAction;
}

//-----mutateur - setter-----
void ServoMoteur::set_m_servoAction(bool servoAction) {
  m_servoAction = servoAction;
}

