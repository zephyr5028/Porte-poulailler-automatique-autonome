/** Bouton.h
  prototype de la Bouton
*/

#ifndef Bouton_h
#define Bouton_h

#include "Arduino.h"

class Bouton
{
  public:

    Bouton(); // constructeur
    Bouton( const byte pinBp, const byte pinBoitier, const int debounce = 50, boolean debug = false ); // surcharge du constructeur
    ~Bouton(); // destructeur

    bool testToucheBp();  //test touche Bp
    void testRelacheBp (volatile bool &interruptBp);//test relache Bp
    void testInterruptionBp (volatile bool &interruptBp); //test Bp suite interruption
    void testInterruptionBoitier (volatile bool &interruptOuvBoi); //test IT ouverture boitier
    bool  testBoitierOuvert(const volatile bool &interruptOuvBoi, const bool &boitierOuvert);//test boitier ouvert
    bool  testBoitierFerme(const volatile bool &interruptOuvBoi, const  bool &boitierOuvert); //test boitier ferme

  protected:

    boolean m_debug ; // pour affichage console si nécessaire
    const byte m_pinBp; // pin D9 bouton poussoir ouverture / fermeture
    const byte m_pinBoitier; //pin D6 interrupteur ouverture boitier
    const int m_debounce; // debounce latency in ms (50)
    unsigned int m_tempoDebounce; // variable pour le calcul de l'anti-rebond
    bool m_relacheBp; // relache du Bp
    bool m_debouncePret;// pour le relache du bp

};

#endif

