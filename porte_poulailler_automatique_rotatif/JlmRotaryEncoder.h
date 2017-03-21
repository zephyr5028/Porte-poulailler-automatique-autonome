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

    JlmRotaryEncoder(byte dt, byte clk, byte sw = 0) ; // constructeur avec broches pour dt , clk et sw

    ~JlmRotaryEncoder(); // destructeur

    void init(); // initialisation des branchements

    int encoderA(); //  changement d'état de A
    int encoderB(); // changement d'etat de B
    void switchClear(); // clear compteur avec le switch

    void compteurRoueCodeuse(bool ouvFerm);//compteur roue codeuse
    byte testCompteurRoueCodeuse (byte tolerance);//test du compteur roue codeuse pour affichage ouvert ou ferme
    unsigned int reglageFinDeCourse (bool ouvFerm, byte touche); //reglage de la fin de course
    unsigned int get_m_compteRoueCodeuse();
    void set_m_compteRoueCodeuse(unsigned int compteRoueCodeuse);
    unsigned int get_m_finDeCourseFermeture();
    void set_m_finDeCourseFermeture(unsigned int finDeCourseFermeture);
    unsigned int get_m_finDeCourseOuverture();
    void set_m_finDeCourseOuverture(unsigned int finDeCourseOuverture);

  protected:



    byte m_encoderPinA; // right (DT) pin A
    byte m_encoderPinB; // left (CLK) pin B
    byte m_switchButton; // switch (SW) pin sw
    boolean m_unSwitch; // un seul appui à la fois sur sw

    volatile unsigned int m_encoderPos;  // un compteur de position
    boolean m_rotating;    // gestion de l'anti-rebonds

    // variable pour les routines de changement d'état
    boolean m_A_set;
    boolean m_B_set;
    boolean m_A_change;
    boolean m_B_change;

    volatile boolean m_interruptRoueCodeuse;    // gestion de l'anti-rebonds
    const unsigned int m_finDeCourseMax; // valeur maxi de la fin de course 500
    volatile unsigned int m_compteRoueCodeuse;  // un compteur de position 150 au départ
    unsigned int m_finDeCourseFermeture; // initialisation de la valeur de la fin de course fermeture 250 au depart
    unsigned int m_finDeCourseOuverture; // initialisation de la valeur de la fin de course ouverture 150 au depart

};

#endif
