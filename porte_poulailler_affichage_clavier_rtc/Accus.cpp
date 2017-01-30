/* Accus.cpp
  définitions de la classe Accus
  bloc accus NIMH
*/

#include "Accus.h"

//constructeur avec debug
Accus::Accus(  const byte accusPin, const float tensionMiniAccus, const float rapportConvertion, const boolean debug) :
  m_accusPin(accusPin), m_tensionMiniAccus(tensionMiniAccus), m_rapportConvertion(rapportConvertion), m_debug(debug), m_maxCAD(1023)
{
}

Accus::~Accus()
{
}

//-----batterie cdes < 4 volt-----
bool Accus::accusFaible() {
  bool batterieFaible;
  int convertMiniTension = (m_maxCAD * m_tensionMiniAccus) / m_rapportConvertion; // convertion tension mini en pas du CAD
  int valAccus = analogRead(m_accusPin); // read the input on analog pin  tension batterie
  if (valAccus < convertMiniTension) { // si la batterie est faible < 4,8v (654)
    return batterieFaible = true;
  } else {
    return batterieFaible = false;
  }
}

//------- lecture tension batterie CAD-----
int Accus::tensionAccusCAD() {
  int valAccus = analogRead(m_accusPin); //read the input on analog pin tension batterie
  return valAccus;
}

//------- convertion CAD  vers tension batterie -----
float Accus::tensionAccus(int valAccus) {
  // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 6V)
  float voltage = valAccus * (m_rapportConvertion / m_maxCAD);
  return voltage;
}

//-----lecture et convertion vers tension batterie en float-----
float Accus::tensionAccusCADversFloat() {
  float volt = tensionAccus(tensionAccusCAD());
  return volt;
}


