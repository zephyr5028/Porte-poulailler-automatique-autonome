/* ServoMoteur.cpp
  définitions de la classe ServoMoteur
  gestion du servo moteur au travers de la bibliothèque ServoTimer2
*/

#include "ServoMoteur.h"

//constructeur avec debug si nécessaire
ServoMoteur::ServoMoteur( byte pinCde, byte pinRelais, boolean debug) :  m_pinCde(pinCde), m_pinRelais(pinRelais),  m_debug(debug)
{
  pinMode(m_pinRelais, OUTPUT); // relais servo's pin  an OUTPUT
  digitalWrite(m_pinRelais, LOW); // mise hors tension du relais du servo
  attach(m_pinCde); // use digital pin for commande du servo
}

ServoMoteur::~ServoMoteur()
{
}

