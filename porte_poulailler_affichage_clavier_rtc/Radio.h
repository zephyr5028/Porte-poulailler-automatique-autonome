/* Radior.h
  prototype de la classe Radio
  radio 433MHz
*/

#ifndef DEF_RADIO
#define DEF_RADIO

class Radio
{
  public:

    // constructeur avec la taille du tableau, debug, radio et le type de temperature
    Radio(byte taille, boolean radio = false , boolean debug = false );
    ~Radio(); // destructeur

    void envoiMessage(char chaine1[]);//routine envoi message radio
    void messageRadio(char chaine1[]);  // message Radio
    void chaineVide();//chaine radio fin de ligne avant transmission
    //envoi message float avec test de l'ouverture du boitier plus texte
    void envoiFloat(float valeur , boolean boitierOuvert = true, char texte[] = "");
    // envoi du message unsigned int avec test de l'ouverture du boitier
    void envoiUnsignedInt(unsigned int valeur, boolean boitierOuvert = true, char texte[] = "");

  private:

    boolean m_debug ; // pour affichage console si nécessaire
    boolean m_radio; // pour envoi radio si necessaire
    int const m_taille; // taille du tableau
    char m_chaine[]; // tableau de la chaine avant envoi

};

#endif


/* ajouter la variable boitier ouvert/ferme pour éviter de remplir le tableau sans envoi du message*/
