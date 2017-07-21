/** Lumiere.cpp
  définitions de la classe Lumiere
  LDR
*/

#include "Lumiere.h"

//constructeur avec debug
Lumiere::Lumiere( const byte lumierePin, unsigned int lumMatin, unsigned int lumSoir, const byte heureFenetreSoir, const float rapportConvertion, const byte tempsLum, const boolean debug) :
  m_lumierePin(lumierePin), m_lumMatin(lumMatin), m_lumSoir(lumSoir),
  m_rapportConvertion(rapportConvertion), m_debug(debug), m_ouverture(1), m_fermeture(0),
  m_lumiereMax(1020), m_incrementation(10), m_maxCAD(1023), m_compteurWatchdogLumiere(0),
  m_heureFenetreSoir(heureFenetreSoir), m_tempsLum(tempsLum)
{
}

Lumiere::~Lumiere()
{
}

///-----reglage de la lumiere du matin ou du soir-----
unsigned int  Lumiere::reglageLumiere (bool matinSoir, byte touche) {
  if (touche == 2 or touche == 3) {
    unsigned int lumiere;
    if (matinSoir) lumiere = m_lumMatin; else lumiere = m_lumSoir;
    if (touche == 2) {
      if (lumiere < m_lumiereMax) {
        lumiere += m_incrementation; //incrementation de la lumiere
      } else {
        lumiere = 0;
      }
    } else {
      if (lumiere > 0) {
        lumiere -= m_incrementation; //decrementation de la lumiere
      } else {
        lumiere =  m_lumiereMax;
      }
    }
    if (matinSoir) m_lumMatin = lumiere; else m_lumSoir = lumiere;
    return lumiere;
  }
  if (matinSoir) return m_lumMatin ; else return m_lumSoir ;
}

///------- lecture luminosite CAD-----
int Lumiere::luminositeCAD() {
  int valLumiere = analogRead(m_lumierePin); //read the input on analog pin tension batterie
  return valLumiere;
}

///------- convertion CAD  vers tension luminosite -----
float Lumiere::tensionLuminosite(int valLumiere) {
  // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V)
  float voltage = valLumiere * (m_rapportConvertion / m_maxCAD);
  return voltage;
}

///-----lecture et convertion vers tension luminosite en float-----
float Lumiere::tensionLuminositeCADversFloat() {
  float tension = tensionLuminosite(luminositeCAD());
  return tension;
}

///-----test luminosite et mise à jour du compteur watchdog lumiere-----
void Lumiere::testLuminosite() {
  int sensorValue = luminositeCAD();
  if (sensorValue >  m_lumSoir and sensorValue < m_lumMatin) {
    m_compteurWatchdogLumiere = 0;//raz du compteur watchdog lumiere pour ne pas prendre en compte une ombre
  }
}

///-----fenetre de non declenchement et mise à jour du compteur watchdog lumiere------
// fenetre de non declenchement avec la lumiere si utilisation horaire, en fonction du mois (jour 17h00 nuit  9h00 jour , 21 décembre)
void Lumiere::fenetreNonDeclenchement(byte horaire, byte mois) {
  byte fenetre =  m_heureFenetreSoir;
  if (mois == 3 or mois == 10 or mois == 4 or mois == 9) {
    fenetre = m_heureFenetreSoir + 1;
  } else if (mois == 5 or mois == 8 ) {
    fenetre = m_heureFenetreSoir + 2;
  } else if (mois == 6 or mois == 7) {
    fenetre = m_heureFenetreSoir + 3;
  }
  if ((m_ouverture and horaire < fenetre) or (m_fermeture and horaire > fenetre)) {
    m_compteurWatchdogLumiere = 0; //raz du compteur watchdog lumiere pour ne pas prendre en compte une ombre ou un eclairage
  }
}

///-----non Declenchement Position Servo et mise à jour du compteur watchdog lumiere-----
void Lumiere::nonDeclenchementPositionServo (volatile unsigned int compteRoueCodeuse, unsigned int finDeCourseFermeture, unsigned int finDeCourseOuverture) {
  int sensorValue = luminositeCAD();
  if (((compteRoueCodeuse <= (finDeCourseOuverture + 2)) and !m_ouverture and (sensorValue >= m_lumMatin))
      or (( compteRoueCodeuse >= (finDeCourseOuverture + finDeCourseFermeture - 2)) and !m_fermeture and (sensorValue <= m_lumSoir))) {
    m_compteurWatchdogLumiere = 0; //raz du compteur watchdog lumiere pour ne pas prendre en compte une ombre
  }
}

///-----declenchement servo par la luminosite-----
byte Lumiere::declenchementServoLuminosite() {
  if (m_compteurWatchdogLumiere >= m_tempsLum) {
    int sensorValue = luminositeCAD();
    if ((sensorValue >= m_lumMatin) and !m_ouverture ) {
      m_compteurWatchdogLumiere = 0; //raz du compteur watchdog lumiere pour ne pas prendre en compte une ombre
      return 1; // ok + ouverture
    }
    if ((sensorValue <= m_lumSoir) and !m_fermeture ) {
      m_compteurWatchdogLumiere = 0; //raz du compteur watchdog lumiere pour ne pas prendre en compte une ombre
   /////   m_buzzer = 2; // réarmement du compteur buzzer
      return 2; // ok + fermeture
    }
  }
  return 0;
}

//-----accesseur - getter-----
bool Lumiere::get_m_ouverture() {
  return m_ouverture;
}
//-----mutateur - setter-----
void Lumiere::set_m_ouverture(bool ouverture) {
  m_ouverture = ouverture;
}

///-----accesseur - getter-----
bool Lumiere::get_m_fermeture() {
  return m_fermeture;
}
//-----mutateur - setter-----
void Lumiere::set_m_fermeture(bool fermeture) {
  m_fermeture = fermeture;
}

//-----accesseur - getter-----
unsigned int Lumiere::get_m_lumMatin() {
  return m_lumMatin;
}
//-----mutateur - setter-----
void Lumiere::set_m_lumMatin(unsigned int lumMatin) {
  m_lumMatin = lumMatin;
}

//-----accesseur - getter-----
unsigned int Lumiere::get_m_lumSoir() {
  return m_lumSoir;
}
//-----mutateur - setter-----
void Lumiere::set_m_lumSoir(unsigned int lumSoir) {
  m_lumSoir = lumSoir;
}

//-----accesseur - getter-----
unsigned int Lumiere::get_m_compteurWatchdogLumiere() {
  return m_compteurWatchdogLumiere;
}
//-----mutateur - setter-----
void Lumiere::set_m_compteurWatchdogLumiere(unsigned int compteurWatchdogLumiere) {
  m_compteurWatchdogLumiere = compteurWatchdogLumiere;
}

