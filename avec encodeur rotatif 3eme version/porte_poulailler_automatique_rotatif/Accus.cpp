/** Accus.cpp
  définitions de la classe Accus
  bloc accus NIMH
*/

#include "Accus.h"

//constructeur avec debug
Accus::Accus(  const byte accusPin, const float tensionMiniAccus, const int R1, const int R2, const int maxCAD, const boolean debug) :
  PowerTools(), m_accusPin(accusPin), m_tensionMiniAccus(tensionMiniAccus), m_R1(R1), m_R2(R2), m_maxCAD(maxCAD), m_debug(debug), m_valMinCAD(668)
{
}

Accus::~Accus()
{
}

///-----initialisation-----
void Accus::init () {
  //utilisation d'un pont de resistances : vout = vin * R2 / R1 + R2
  float vout = (m_tensionMiniAccus * m_R2) / (m_R1 + m_R2) ;
  m_valMinCAD = (vout * m_maxCAD) /  analogReadReference() ;
  //m_valMinCAD = (vout * m_maxCAD) / m_Vref ;
}

///------- lecture tension batterie CAD-----
int Accus::lectureAccusCAD() {
  int lectureAccus = analogRead(m_accusPin); //read the input on analog pin tension batterie
  return lectureAccus;
}

/**
   \brief teste si la tension est faible
   \details Lecture et conversion de la valeur mesurée sur un pont de résistances en parallèle de l'accu
   \param m_accusPin numéro de l'entrée analogique
   \return batterieFaible si < 4,8v
*/
bool Accus::accusFaible() {
  bool batterieFaible;
  if (lectureAccusCAD() < m_valMinCAD) { /// si la batterie est faible
    return batterieFaible = true;
  } else {
    return batterieFaible = false;
  }
}


///------- convertion CAD  vers tension batterie -----
float Accus::tensionAccus() {
  //utilisation d'un pont de resistances : vout = vin * R2 / R1 + R2
  float vout = (lectureAccusCAD() * analogReadReference()) / m_maxCAD;
  //float vout = (lectureAccusCAD() * m_Vref) / m_maxCAD;
  float vin = (vout * (m_R1 + m_R2)) / m_R2;
  // vin : tension de l'accu
  return vin;
}


