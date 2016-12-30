/* Codeur.cpp
  définitions de la classe Codeur
  Codeur optqie de 20 pas
*/

#include "Codeur.h"

//constructeur avec debug
Codeur::Codeur( const byte roueCodeusePin, unsigned int finDeCourseFermeture, unsigned int finDeCourseOuverture, unsigned int compteRoueCodeuse, const boolean debug) :
  m_roueCodeusePin(roueCodeusePin), m_finDeCourseFermeture(finDeCourseFermeture), m_finDeCourseOuverture(finDeCourseOuverture),
  m_compteRoueCodeuse(compteRoueCodeuse), m_debug(debug), m_finDeCourseMax(500)
{
}

Codeur::~Codeur()
{
}


//-----test du compteur roue codeuse pour affichage ouvert ou ferme-----
byte Codeur::testCompteurRoueCodeuse (byte tolerance) {
  byte resultat;
  if ( m_compteRoueCodeuse > (m_finDeCourseFermeture - 5) and m_compteRoueCodeuse < (m_finDeCourseFermeture + 5)) resultat = 1; else resultat = 0; // creneau porte fermée
  if ( m_compteRoueCodeuse > (m_finDeCourseOuverture - 5) and m_compteRoueCodeuse < (m_finDeCourseOuverture + 5)) resultat = 2; else resultat = 0;  // creneau porte ouverte
  return resultat;
}

//-----reglage de la fin de course-----
unsigned int  Codeur::reglageFinDeCourse (bool ouvFerm, byte touche) {
  if (touche == 2 or touche == 3) {
    unsigned int finDeCourse;
    if (ouvFerm) finDeCourse = m_finDeCourseOuverture; else finDeCourse = m_finDeCourseFermeture;
    if (touche == 2) {
      if (finDeCourse < m_finDeCourseMax) {
        finDeCourse++ ; //incrementation
      } else {
        finDeCourse = 0;
      }
    } else {
      if (finDeCourse > 0) {
        finDeCourse-- ; //decrementation
      } else {
        finDeCourse =  m_finDeCourseMax;
      }
    }
    if (ouvFerm) m_finDeCourseOuverture = finDeCourse; else m_finDeCourseFermeture = finDeCourse;
    return finDeCourse;
  }
  if ( ouvFerm) return m_finDeCourseOuverture; else return m_finDeCourseFermeture;
}

//-----accesseur - getter-----
unsigned int Codeur::get_m_compteRoueCodeuse() {
  return m_compteRoueCodeuse;
}
//-----mutateur - setter-----
void Codeur::set_m_compteRoueCodeuse(unsigned int compteRoueCodeuse) {
  m_compteRoueCodeuse = compteRoueCodeuse;
}

//-----accesseur - getter-----
unsigned int Codeur::get_m_finDeCourseFermeture() {
  return m_finDeCourseFermeture;
}
//-----mutateur - setter-----
void Codeur::set_m_finDeCourseFermeture(unsigned int finDeCourseFermeture) {
  m_finDeCourseFermeture = finDeCourseFermeture;
}

//-----accesseur - getter-----
unsigned int Codeur::get_m_finDeCourseOuverture() {
  return m_finDeCourseOuverture;
}
//-----mutateur - setter-----
void Codeur::set_m_finDeCourseOuverture(unsigned int finDeCourseOuverture) {
  m_finDeCourseOuverture = finDeCourseOuverture;
}
