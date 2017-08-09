#include "JlmRotaryEncoder.h"

// surcharge constructeur avec timer - broches dt et clk ainsi que sw si besoin
JlmRotaryEncoder::JlmRotaryEncoder(byte pinA, byte pinB,  int finDeCourseFermeture, int finDeCourseOuverture, int compteRoueCodeuse) : m_encoderPinA(pinA), m_encoderPinB(pinB),
  m_finDeCourseFermeture(finDeCourseFermeture), m_finDeCourseOuverture(finDeCourseOuverture), m_compteRoueCodeuse(compteRoueCodeuse),  m_finDeCourseMax(500),  m_interruptRoueCodeuse(false)
{
  JlmRotaryEncoder::init();// initialisation des branchements
}

// destrcuteur
JlmRotaryEncoder::~JlmRotaryEncoder()
{
}

// initialisation des branchements
void JlmRotaryEncoder::init()
{
  // pinMode(m_encoderPinA, INPUT); // interruption D2
  //  attachInterrupt(0, m_encoderPinA, RISING); // validation de l'interruption sur int0 (d2)
  pinMode(m_encoderPinB, INPUT); //
}

///-----compteur roue codeuse-----
// code gray avec l'it sur la broche d2 INT0
void JlmRotaryEncoder::compteurRoueCodeuse() {
  bool pinA = digitalRead(m_encoderPinA);
  bool pinB = digitalRead(m_encoderPinB);
  if (pinA) {
    if (!pinB) {
      m_compteRoueCodeuse--;
    } else {
      m_compteRoueCodeuse++;
    }
  }
  // pour la verification de la variable tempoEncodeur
  // Serial.print(pinA);
  // Serial.print(pinB);
  // Serial.println(" ");

}

///-----reglage de la fin de course-----
int  JlmRotaryEncoder::reglageFinDeCourse (bool ouvFerm, byte touche) {
  if (touche == 2 or touche == 3) {
    int finDeCourse;
    if (ouvFerm) finDeCourse = m_finDeCourseOuverture; else finDeCourse = m_finDeCourseFermeture;
    if (touche == 2) {
      if (finDeCourse < m_finDeCourseMax) {
        finDeCourse++ ; //incrementation
      } else {
        finDeCourse = 0;
      }
    } else {
      if (finDeCourse > 0) {
        finDeCourse-- ; //decrementation
      } else {
        finDeCourse =  m_finDeCourseMax;
      }
    }
    if (ouvFerm) m_finDeCourseOuverture = finDeCourse; else m_finDeCourseFermeture = finDeCourse;
    return finDeCourse;
  }
  if ( ouvFerm) return m_finDeCourseOuverture; else return m_finDeCourseFermeture;
}

///-----test du compteur roue codeuse pour affichage ouvert ou ferme-----
byte JlmRotaryEncoder::testCompteurRoueCodeuse (byte tolerance) {
  byte resultat;
  if ( m_compteRoueCodeuse > (m_finDeCourseOuverture + m_finDeCourseFermeture - 5) and m_compteRoueCodeuse < (m_finDeCourseOuverture + m_finDeCourseFermeture + 5)) {
    resultat = 1; // creneau porte fermée
  } else  if ( m_compteRoueCodeuse > (m_finDeCourseOuverture - 5) and m_compteRoueCodeuse < (m_finDeCourseOuverture + 5)) {
    resultat = 2;// creneau porte ouverte
  } else {
    resultat = 0;
  }
  return resultat;
}

//-----accesseur - getter-----
int JlmRotaryEncoder::get_m_compteRoueCodeuse() {
  return m_compteRoueCodeuse;
}
//-----mutateur - setter-----
void JlmRotaryEncoder::set_m_compteRoueCodeuse(int compteRoueCodeuse) {
  m_compteRoueCodeuse = compteRoueCodeuse;
}

//-----accesseur - getter-----
int JlmRotaryEncoder::get_m_finDeCourseFermeture() {
  return m_finDeCourseFermeture;
}
//-----mutateur - setter-----
void JlmRotaryEncoder::set_m_finDeCourseFermeture(int finDeCourseFermeture) {
  m_finDeCourseFermeture = finDeCourseFermeture;
}

//-----accesseur - getter-----
int JlmRotaryEncoder::get_m_finDeCourseOuverture() {
  return m_finDeCourseOuverture;
}
//-----mutateur - setter-----
void JlmRotaryEncoder::set_m_finDeCourseOuverture(int finDeCourseOuverture) {
  m_finDeCourseOuverture = finDeCourseOuverture;
}

//-----accesseur - getter-----
volatile boolean  JlmRotaryEncoder::get_m_interruptRoueCodeuse() {
  return m_interruptRoueCodeuse;
}
//-----mutateur - setter-----
void  JlmRotaryEncoder::set_m_interruptRoueCodeuse( volatile boolean interruptRoueCodeuse) {
  m_interruptRoueCodeuse = interruptRoueCodeuse;
}

