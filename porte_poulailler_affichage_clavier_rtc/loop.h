/* loop */

void loop() {
  // le boitier est ouvert
  if ( boitierOuvert) {
    lectureClavier(); // lecture du clavier
    // affichage du menu lorsque temps est >....
    temporisationAffichage() ; // temporisation pour l'affichage
    // reglages
    reglageTime(); // reglages de l'heure, minute, seconde si touche fleche droite
    reglageDate(); // reglage de la date si touche fleche droite
    reglageHeureOuverture();// reglage de l'heure d'ouverture
    reglageHeureFermeture();// reglage de l'heure de fermeture
    choixOuvFerm (); // choix de l'ouverture et la fermeture
    choixLumMatin();//  reglage de la lumiere du matin
    choixLumSoir();//  reglage de la lumiere du soir
    regFinDeCourseHaut(); // reglage fin de course Haut
    regFinDeCourseBas(); // reglage fin de course bas
    eclairageAfficheur(); // retro eclairage de l'afficheur
    monteeDescenteManuelle();// initialisation butée roue codeuse
    compteurWatchdogLumiere = 0; //raz du compteur watchdog lumiere pour ne pas prendre en compte une ombre
    if (TESTSERVO) {
      testServo(); // reglage du servo plus test de la roue codeuse et du servo, à l'aide de la console
    }
  }
  // le boitier est fermé et le servo n'est pas en action
  if (!boitierOuvert and  !servoAction) {
    ouvFermLum() ;  // ouverture/fermeture par test de la lumière
  }
  compteurRoueCodeuse(); //compteur roue codeuse
  batterieCdesFaible(); // batterie cdes < 4 volt
  monteePorte(); // sequence de montee de la porte
  descentePorte();// sequence de descente de la porte
  routineTestFermetureBoitier(); // test fermeture boitier
  routineInterruptionBp(); // routine interruption Bp
  routineInterrruptionAlarme2() ; // routine alarme 2
  routineInterruptionAlarme1(); // routine alarme 1
  routineInterruptionBoitierOuvert(); // routine interruption boitier ouvert
  routineGestionWatchdog(); // routine de gestion du watchdog
}
