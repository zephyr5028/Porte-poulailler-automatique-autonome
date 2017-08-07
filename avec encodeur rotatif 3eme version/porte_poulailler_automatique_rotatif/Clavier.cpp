/** Clavier.cpp
  définitions de la classe Clavier
*/

#include "Clavier.h"

Clavier::Clavier() : Bouton(  ), m_oldKey (-1), m_MenuManuel(14), m_NumKeys(6),
  m_AdcKeyVal( {
  50, 200, 400, 600, 800
})
{

}
/* sucharge du constructeur avec le nombre de lignes du menu */
Clavier::Clavier(byte nbMenu, const byte pinBp, const byte pinBoitier, const int debounce, const boolean debug) :
  Bouton(  pinBp, pinBoitier,  debounce,  debug ),
  m_oldKey (-1), m_MenuManuel(nbMenu), m_debug(debug), m_NumKeys(6), m_AdcKeyVal( {
  50, 220, 420, 620, 820
})
{

}

Clavier::~Clavier() {

}

///------routine get_key : Convert ADC value to key number------
int Clavier::get_key(int &input) {
  int k;
  for (k = 1; k < m_NumKeys; k++ ) {
    if (input < m_AdcKeyVal [k - 1]) {
      return k;
    }
  }
  if (k >= m_NumKeys) {
    k = -1; // No valid key pressed
    return k;
  }
}

///-----routine read_key : lecture du clavier-----
int Clavier::read_key(const int &sensor) {
  int  adc_key_in = analogRead(sensor); // read the value from the sensor A1
  byte key = get_key(adc_key_in); // convert into key press
  if (key != m_oldKey) { // if keypress is detected
    delay(5); // wait for debounce time 50
    adc_key_in = analogRead(sensor); // read the value from the sensor
    key = get_key(adc_key_in); // convert into key press
    if (key != m_oldKey)  {
      return  key;
    }
  }
  return m_oldKey;
}

///-----position du menu pour l'affichage - touches 2 et 3 -----
void Clavier::positionMenu(byte &positionMenu, const byte &toucheClavier) {
  if (toucheClavier == 3 ) {
    if (positionMenu < m_MenuManuel) {
      positionMenu++; // +1
    } else {
      positionMenu = 1;
    }
  }
  if (toucheClavier == 2 ) {
    if (positionMenu > 1) {
      positionMenu-- ; // -1
    } else {
      positionMenu = m_MenuManuel;
    }
  }
}

///-----relache touche-----
void Clavier::relacheTouche(const int &touche, bool &relache) {
  if (touche == m_oldKey ) { // si touche == -1, donc  relache=true
    relache = true;
  }
}

///-----deplacement dans le menu-----
bool Clavier::deplacementDansMenu(const &touche, bool &relache, const bool &reglage) {
  if ((touche == 2 or touche == 3) and relache and !reglage) { // si appui sur les touches 2 ou 3 , la touche relache et le mode reglage
    relache = false;
    return true;
  } else {
    return false;
  }
}

///-----test touche 5-----
bool Clavier::testTouche5(const &touche, bool &relache) {
  if (touche == 5 and relache == true ) { // retro eclairage si appuis sur la touche 5
    relache = false;
    return true;
  } else {
    return false;
  }
}

