/* Clavier.h
  prototype de la classe Clavier
*/

#ifndef DEF_CLAVIER
#define DEF_CLAVIER

class Clavier
{
  public:

    Clavier(); // constructeur
    Clavier( byte nbMenu ); // surcharge du constructeur avec le nombre de lignes du menu
    ~Clavier(); // destructeur
    
    int get_key(unsigned int input); // Convert ADC value to key number
    int read_key(int sensor); // read_key
    byte positionMenu(byte positionMenu, byte toucheClavier); // position du menu pour l'affichage - touches 2 et 3


  private:

    byte m_oldKey;  // -1
    byte m_MenuManuel; // nombre de lignes du menu
    byte m_NumKeys;  // nombre de touches +1
    int m_AdcKeyVal[5]; // Analogique digital Keyboard Module

};

#endif
