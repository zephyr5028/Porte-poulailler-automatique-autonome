/* Bouton.cpp
  définitions de la classe Bouton
*/

#include "Bouton.h"

Bouton::Bouton() :  m_pinBp(9), m_pinBoitier(6), m_debounce(350), m_relacheBp(true)
{

}
/* sucharge du constructeur avec le nombre de lignes du menu */
Bouton::Bouton( const byte pinBp, const byte pinBoitier, const int debounce, const boolean debug) : m_debug(debug), m_pinBp(pinBp),
m_pinBoitier(pinBoitier), m_relacheBp(true), m_debounce(debounce)
{

}

Bouton::~Bouton() {

}

//-----test touche Bp-----
bool Bouton::testToucheBp(unsigned long  &tempoDebounce) {
  if (((millis() - tempoDebounce) > m_debounce)  and  m_relacheBp == true and !digitalRead(m_pinBp) ) {
    m_relacheBp = false;
    return true;
  } else {
    return false;
  }
}

//-----test relache Bp-----
void Bouton::testRelacheBp (volatile bool &interruptBp) {
  if (m_relacheBp == false and digitalRead(m_pinBp) ) {
    interruptBp = false; // autorisation de la prise en compte de l'IT
    m_relacheBp = true;
  }
}

//-----test IT Bp-----
void Bouton::testInterruptionBp (volatile bool &interruptBp, unsigned long  &tempoDebounce) {
  if (!digitalRead(m_pinBp) and !interruptBp) { // entree 9 pour interruption BP
    interruptBp = true;
    tempoDebounce = millis();
  }
}

//-----test IT ouverture boitier-----
void Bouton::testInterruptionBoitier (volatile bool &interruptOuvBoi) {
  if (!digitalRead(m_pinBoitier) and !interruptOuvBoi) { // entree 9 pour interruption BP
    interruptOuvBoi = true;
  }
}

//-----test boitier ouvert------
bool  Bouton::testBoitierOuvert(const volatile bool &interruptOuvBoi, const bool &boitierOuvert) {
  if ( interruptOuvBoi and !digitalRead(m_pinBoitier) and !boitierOuvert) {    //  interruption ouverture boitier
    return true;
  } else {
    return false;
  }
}

//-----test boitier ferme------
bool  Bouton::testBoitierFerme(const volatile bool &interruptOuvBoi, const bool &boitierOuvert) {
  if (digitalRead(m_pinBoitier) and boitierOuvert) { //  fermeture boitier
    return true;
  } else {
    return false;
  }
}

