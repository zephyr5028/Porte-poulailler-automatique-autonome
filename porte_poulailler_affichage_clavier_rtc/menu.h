/* menu */

//-----routine affichage menus------
void deroulementMenu (byte increment) {
 // if (boitierOuvert) {
    byte j = ((increment - 1) * (colonnes + 1)); // tous les 16 caractères
    mydisp.drawStr(0, 0, ""); // position du curseur en 0,0
    for (byte i = j; i < j + colonnes; i++) { // boucle pour afficher 16 caractères sur le lcd
      char temp = pgm_read_byte(affichageMenu + i); // utilisation du texte présent en mèmoire flash
      mydisp.print(temp);
    }
    switch (increment) { // test de la valeur de incrementation pour affichage des parametres
      case 1: // Date
        mydisp.drawStr(0, 1, " ");
        displayDate(); // affichage de la date
        break;
      case 2: // heure
        mydisp.drawStr(0, 1, "   ");
        displayTime(); // affichage de l'heure
        break;
      case 3: // heure ouverture
        mydisp.drawStr(0, 1, "   ");
        openTime();  // affichage de l'heure d'ouverture de la porte
        break;
      case 4: // heure fermeture
        mydisp.drawStr(0, 1, "   ");
        closeTime();  // affichage de l'heure de fermeture de la porte
        break;
      case 5: // temperature
        mydisp.drawStr(0, 1, "    ");
        read_temp(true); // read temperature celsius=true
        break;
      case 6: //lumiere
        mydisp.drawStr(0, 1, "");
        lumiere(); // lecture et affichage de la lumiere
        break;
      case 7: // lumiere matin
        mydisp.drawStr(0, 1, "");
        affiLumMatin(); // affichage de la lumiere du matin
        break;
      case 8: // lumiere soir
        mydisp.drawStr(0, 1, "");
        affiLumSoir(); // affichage de la lumiere du soir
        break;
      case 9: // choix Ouverture / Fermeture
        mydisp.drawStr(0, 1, "");
        affiChoixOuvFerm(); // choix
        break;
      case 10:  // fin de course Fermeture
        mydisp.drawStr(0, 1, "");
        affiFinDeCourseHaut(); // fin de course Haut
        break;
      case 11:  // fin de course ouverture
        mydisp.drawStr(0, 1, "");
        affiFinDeCourseOuverture();  // fin de course ouverture
        break;
      case 12:  // tension batterie commandes
        mydisp.drawStr(0, 1, "");
        affiTensionBatCdes(); //
        break;
      case 13:  // tension batterie servo
        mydisp.drawStr(0, 1, "");
        affiTensionBatServo(); //
        break;
      case 14:  // commande manuelle
        mydisp.drawStr(0, 1, "");
        affiPulsePlusCptRoue(); // affichage pulse et comptage roue codeuse
        break;
    }
 // }
}
