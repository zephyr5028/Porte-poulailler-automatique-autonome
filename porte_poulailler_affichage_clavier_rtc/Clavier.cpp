/* Clavier.cpp
  définitions de la classe Clavier
*/

#include "Arduino.h"
#include "Clavier.h"

Clavier::Clavier() : m_oldKey (-1), m_MenuManuel(14), m_NumKeys(6),
  m_AdcKeyVal( {
  50, 200, 400, 600, 800
})
{

}
/* sucharge du constructeur avec le nombre de lignes du menu */
Clavier::Clavier(byte nbMenu) : m_oldKey (-1), m_MenuManuel(nbMenu), m_NumKeys(6),
  m_AdcKeyVal( {
  50, 200, 400, 600, 800
})
{

}

Clavier::~Clavier() {

}

//------routine get_key : Convert ADC value to key number------
int Clavier::get_key(unsigned int input) {
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

//-----routine read_key : lecture du clavier-----
int Clavier::read_key(int sensor) {
  int  adc_key_in = analogRead(sensor); // read the value from the sensor A1
  byte key = get_key(adc_key_in); // convert into key press
  if (key != m_oldKey) { // if keypress is detected
    delay(100); // wait for debounce time
    adc_key_in = analogRead(sensor); // read the value from the sensor
    key = get_key(adc_key_in); // convert into key press
    if (key != m_oldKey)  {
      return  key;
    }
  }
  return m_oldKey;
}

//-----position du menu pour l'affichage - touches 2 et 3 -----
byte Clavier::positionMenu(byte positionMenu, byte toucheClavier) {
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
  return positionMenu;
}

