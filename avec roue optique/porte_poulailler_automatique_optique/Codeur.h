/** Codeur.h
  prototype de la classe Codeur
  codeur optique de 20 pas
*/

#ifndef Codeur_h
#define Codeur_h

#include "Arduino.h"

class Codeur
{

  public:

    // constructeur avec debug
    Codeur( const byte roueCodeusePin, unsigned int finDeCourseFermeture = 250, unsigned int finDeCourseOuverture = 150,
            unsigned int compteRoueCodeuse = 150, boolean debug = false );
    ~Codeur(); // destructeur

    void init();// initialisation
    byte testCompteurRoueCodeuse (byte tolerance);//test du compteur roue codeuse pour affichage ouvert ou ferme
    unsigned int reglageFinDeCourse (bool ouvFerm, byte touche); //reglage de la fin de course
    void compteurRoueCodeuse(bool ouvFerm);//compteur roue codeuse
    unsigned int get_m_compteRoueCodeuse();
    void set_m_compteRoueCodeuse(unsigned int compteRoueCodeuse);
    unsigned int get_m_finDeCourseFermeture();
    void set_m_finDeCourseFermeture(unsigned int finDeCourseFermeture);
    unsigned int get_m_finDeCourseOuverture();
    void set_m_finDeCourseOuverture(unsigned int finDeCourseOuverture);

  protected:

    const byte m_roueCodeusePin;//digital pin D7 pour entrée roue codeuse
    const unsigned int m_finDeCourseMax; // valeur maxi de la fin de course 500
    const boolean m_debug ; // pour affichage console si nécessaire
    volatile unsigned int m_compteRoueCodeuse;  // un compteur de position 150 au départ
    volatile boolean m_interruptRoueCodeuse;    // gestion de l'anti-rebonds
    volatile boolean m_positionRoueCodeuse;// gestion de l'anti-rebonds
    unsigned int m_finDeCourseFermeture; // initialisation de la valeur de la fin de course fermeture 250 au depart
    unsigned int m_finDeCourseOuverture; // initialisation de la valeur de la fin de course ouverture 150 au depart

};

#endif

