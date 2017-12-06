/**
    \file Accus.h
    \class Accus
    \brief Gestion des blocs accumulateurs NIMH de 4,8v
    \details Suivi de la tension et la valeur minimum
    \author Zephyr5028
    \date hiver 2017
  Prototype de la classe Accus
*/

#ifndef Accus_h
#define Accus_h

#include "PowerTools.h"
#include "Arduino.h"

class Accus  :  public PowerTools
{

  public:
    // constructeur avec debug
    //Accus( const byte accusPin, const float tensionMiniAccus = 4.8,  const int R1 = 4700,  const int R2 = 10000, const float  Vref = 5.0, const int maxCAD = 1023, boolean debug = false);
    Accus( const byte accusPin, const float tensionMiniAccus = 4.8,  const int R1 = 4700,  const int R2 = 10000, const int maxCAD = 1023, boolean debug = false);
    ~Accus(); // destructeur

    void init();// initialisation
    bool accusFaible();//batterie faible
    int lectureAccusCAD(); // lecture CAD
    float tensionAccus(); // convertion tension CAD tension de l'accu


  protected:
    const boolean m_debug ; // pour affichage console si nécessaire
    const byte m_accusPin; //analog pin - tension batterie
    const int m_maxCAD; // maximum du convertisseur analogique numérique
    const float m_tensionMiniAccus; // tension mini de l'accu
    const int m_R1 ; // resistance  R1 du pont
    const int m_R2 ; // resistance R2 du pont
    //   const float  m_Vref; // tension de reference
    int m_valMinCAD; // valeur minimale du CAD en fonction de la tension mini de l'accu

};

#endif

