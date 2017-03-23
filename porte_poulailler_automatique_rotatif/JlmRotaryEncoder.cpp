#include "JlmRotaryEncoder.h"

// surcharge constructeur avec timer - broches dt et clk ainsi que sw si besoin
JlmRotaryEncoder::JlmRotaryEncoder(byte dt, byte clk, byte sw) : m_encoderPinA(dt), m_encoderPinB(clk), m_switchButton(sw),
  m_A_set(false) , m_B_set(false), m_A_change(false), m_B_change(false), m_rotating(false), m_encoderPos(0),  m_unSwitch(true),
  m_finDeCourseFermeture(40), m_finDeCourseOuverture(100), m_compteRoueCodeuse(120),  m_finDeCourseMax(500),  m_interruptRoueCodeuse(false)
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
  pinMode(m_encoderPinA, INPUT); //
  pinMode(m_encoderPinB, INPUT); //
  if (m_switchButton != 0)   pinMode(m_switchButton, INPUT_PULLUP); // utilisation du pullup
}

// changement d'état de A
int JlmRotaryEncoder::encoderA()
{
  // debounce
  if ( m_rotating ) delay (3);  // attendre un petit peut
  m_rotating = true; //activation de l'anti-rebond
  // Confirmation du changement
  if ( digitalRead(m_encoderPinA) != m_A_set )  {
    m_A_set = !m_A_set;
    if (m_B_change) {
      m_encoderPos += 1;
      m_B_change = false;
    } else
      m_A_change = true;
    m_rotating = false;  //libération de l'anti-rebond
  }
  return m_encoderPos;
}

// changement d'etat de B
int JlmRotaryEncoder::encoderB()
{
  if ( m_rotating ) delay (3);
  m_rotating = true;
  if ( digitalRead(m_encoderPinB) != m_B_set )  {
    m_B_set = !m_B_set;
    if (m_A_change) {
      m_compteRoueCodeuse -= 1;
      m_encoderPos -= 1;
      m_A_change = false;
    } else
      m_B_change = true;
    m_rotating = false;
  }
  return m_encoderPos;
}

// clear le compteur avec le switch
void JlmRotaryEncoder::switchClear()
{
  //debounce et un seul appui à la fois
  if (digitalRead(m_switchButton) == LOW )  delay(20); else m_unSwitch = true;
  if (digitalRead(m_switchButton) == LOW  and m_unSwitch) {
    m_encoderPos = 0;
    m_A_change = false;
    m_B_change = false;
    m_unSwitch = false;
  }
}



///-----compteur roue codeuse-----
// code gray et comparaison avec l'état des pins n-1
void JlmRotaryEncoder::compteurRoueCodeuse() {
  bool pinA = digitalRead(m_encoderPinA);
  bool pinB = digitalRead(m_encoderPinB);
  if ((!pinA && pinB && m_A_change && m_B_change) or
      ( pinA && !pinB && !m_A_change && !m_B_change)) {
    m_compteRoueCodeuse++;
  }
  else if ((!pinB && pinA && m_A_change && m_B_change ) or
           (pinB && !pinA && !m_A_change && !m_B_change)) {
    m_compteRoueCodeuse--;
  }
  m_A_change = pinA;// n-1
  m_B_change = pinB;// n-1
}

/// mise à jour de A et B change
void JlmRotaryEncoder::writeRotaryDtClk() {
  m_A_change = digitalRead(m_encoderPinA);
  m_B_change = digitalRead(m_encoderPinB);
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
  if ( m_compteRoueCodeuse > (m_finDeCourseFermeture - 5) and m_compteRoueCodeuse < (m_finDeCourseFermeture + 5)) {
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
bool JlmRotaryEncoder::get_m_A_change() {
  return m_A_change;
}
//-----mutateur - setter-----
void JlmRotaryEncoder::set_m_A_change(bool A_change) {
  m_A_change = A_change;
}

//-----accesseur - getter-----
bool JlmRotaryEncoder::get_m_B_change() {
  return m_B_change;
}
//-----mutateur - setter-----
void JlmRotaryEncoder::set_m_B_change(bool B_change) {
  m_B_change = B_change;
}

