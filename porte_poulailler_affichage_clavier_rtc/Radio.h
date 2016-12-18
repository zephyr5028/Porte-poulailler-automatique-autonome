/* Radior.h
  prototype de la classe Radio
  radio 433MHz
*/

#ifndef DEF_RADIO
#define DEF_RADIO

class Radio
{
  public:

    Radio(byte taille, boolean radio = false , boolean debug = false );// constructeur avec debug, radio
    ~Radio(); // destructeur

    void envoiMessage(char chaine1[]);//routine envoi message radio
    void messageRadio(char chaine1[]);  // message Radio
    void chaineVide();//chaine radio fin de ligne avant transmission

  private:

    boolean m_debug ; // pour affichage console si nécessaire
    boolean m_radio; // pour envoi radio si necessaire
    int const m_taille; // taille du tableau
    char m_chaine[]; // tableau de la chaine avant envoi

};

#endif

