/** Lumiere.h
  prototype de la classe Lumiere
  ldr
*/

#ifndef Lumiere_h
#define Lumiere_h

#include "PowerTools.h"
#include "Arduino.h"

class Lumiere  :  public PowerTools
{

  public:

    // constructeur avec debug
    Lumiere( const byte lumierePin, unsigned int lumMatin, unsigned int lumSoir,  const byte heureFenetreSoir = 17 ,
             const int R2 = 10000, const int maxCAD = 1023, const byte tempsLum = 2,  boolean debug = false );
    //const int R2 = 10000, const float  Vref = 5.0, const int maxCAD = 1023, const byte tempsLum = 2,  boolean debug = false );
    ~Lumiere(); // destructeur

    unsigned int  reglageLumiere (bool matinSoir, byte touche);// reglage de la lumiere du matin ou du soir
    int luminositeCAD(); //lecture de la luminosite CAD
    unsigned int tensionLuminosite(); // convertion CAD  vers LDR
    void testLuminosite();//test luminosite et mise à jour du compteur watchdog lumiere
    void fenetreNonDeclenchement(byte horaire, byte mois);//fenetre de non declenchement et mise à jour du compteur watchdog lumiere
    //non Declenchement Position Servo et mise à jour du compteur watchdog lumiere
    void nonDeclenchementPositionServo (volatile unsigned int compteRoueCodeuse, unsigned int finDeCourseFermeture, unsigned int finDeCourseOuverture);
    byte declenchementServoLuminosite();//declenchement servo par la luminosite
    bool get_m_ouverture();
    void set_m_ouverture(bool ouverture);
    bool get_m_fermeture();
    void set_m_fermeture(bool fermeture);
    unsigned int get_m_lumMatin();
    void set_m_lumMatin(unsigned int lumMatin);
    unsigned int get_m_lumSoir();
    void set_m_lumSoir(unsigned int lumSoir);
    unsigned int get_m_compteurWatchdogLumiere();
    void set_m_compteurWatchdogLumiere(unsigned int compteurWatchdogLumiere);


  protected:

    unsigned int  m_lumMatin; /// valeur de la lumière du matin
    unsigned int m_lumSoir; // valeur de la lumiere du soir
    bool m_ouverture ; // ouverture  (lumiere ou heure) 1 donc heure
    bool m_fermeture ; //  fermeture (lumiere ou heure) 0 donc lumiere
    unsigned int m_compteurWatchdogLumiere; // compteur pour eviter un declenchement intempestif
    const unsigned int m_lumiereMax; // valeur maxi de la lumiere 1020
    const byte m_incrementation; // pas de 10
    const byte m_lumierePin; //analog pin - tension luminosite
    const int m_R2 ; // resistance R2 du pont
    // const float  m_Vref; // tension de reference
    const int m_maxCAD; // maximum du convertisseur analogique numérique 1023
    const byte m_heureFenetreSoir; // horaire de la fenetre de non declenchement lumiere si utilisation horaire : 17h
    const byte m_tempsLum; // 2 boucles pour valider l'ouverture / fermeture avec la lumière (compteur watchdog)
    const boolean m_debug ; // pour affichage console si nécessaire

};

#endif

