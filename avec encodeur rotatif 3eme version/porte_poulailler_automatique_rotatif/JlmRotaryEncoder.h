/*********************************

   classe JlmRotaryEncoder KY040

   pour arduino uno et nano

   12 2016

 *********************************/

#ifndef JlmRotaryEncoder_h
#define JlmRotaryEncoder_h

#include <Arduino.h>

class JlmRotaryEncoder
{
  public:

    JlmRotaryEncoder(byte pinA, byte pinB, int finDeCourseFermeture, int finDeCourseOuverture, int compteRoueCodeuse) ; // constructeur avec broches pour A et B

    ~JlmRotaryEncoder(); // destructeur

    void init(); // initialisation des branchements

    void compteurRoueCodeuse(bool sens);//compteur roue codeuse
    byte testCompteurRoueCodeuse (byte tolerance);//test du compteur roue codeuse pour affichage ouvert ou ferme
    int reglageFinDeCourse (bool ouvFerm, byte touche); //reglage de la fin de course
    
    int get_m_compteRoueCodeuse();
    void set_m_compteRoueCodeuse(int compteRoueCodeuse);
    int get_m_finDeCourseFermeture();
    void set_m_finDeCourseFermeture(int finDeCourseFermeture);
    int get_m_finDeCourseOuverture();
    void set_m_finDeCourseOuverture(int finDeCourseOuverture);
    volatile boolean get_m_interruptRoueCodeuse();
    void set_m_interruptRoueCodeuse ( volatile boolean interruptRoueCodeuse);

    
  protected:

    byte m_encoderPinA; //  pin A
    byte m_encoderPinB; //  pin B

    volatile boolean m_interruptRoueCodeuse;    // gestion de l'anti-rebonds
    const int m_finDeCourseMax; // valeur maxi de la fin de course 500
    int m_compteRoueCodeuse;  // un compteur de position au départ
    int m_finDeCourseFermeture; // initialisation de la valeur de la fin de course fermeture au depart
    int m_finDeCourseOuverture; // initialisation de la valeur de la fin de course ouverture au depart

};

#endif
