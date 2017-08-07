/** Clavier.h
  prototype de la classe Clavier
*/

#ifndef Clavier_h
#define Clavier_h

#include "Bouton.h"
#include "Arduino.h"

class Clavier : public  Bouton
{
  public:

    Clavier(); // constructeur
    Clavier( byte nbMenu, const byte pinBp, const byte pinBoitier, const int debounce = 350, const boolean debug = false ); // surcharge du constructeur
    ~Clavier(); // destructeur

    int get_key( int &input); // Convert ADC value to key number
    int read_key(const int &sensor); // read_key
    void positionMenu(byte &positionMenu, const byte &toucheClavier); // position du menu pour l'affichage - touches 2 et 3
    bool deplacementDansMenu(const int &touche, bool &relache, const bool &reglage);    //deplacement dans le menu
    void relacheTouche(const int &touche, bool &relache);  //relache touche
    bool testTouche5(const int &touche, bool &relache) ;//test touche 5


  protected:

    const boolean m_debug ; // pour affichage console si nécessaire
    const byte m_oldKey;  // -1
    const byte m_NumKeys;  // nombre de touches +1
    byte m_MenuManuel; // nombre de lignes du menu
    int m_AdcKeyVal[5]; // Analogique digital Keyboard Module


};

#endif

