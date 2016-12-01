/* reglahe time */

//-----routine reglage heure , minute , seconde-----
void reglageTime () {
  if (touche == 4 and relache == true and incrementation == menuHeure) {
    relache = false;
    if (decalage < 14 ) { // boucle de reglage
      decalage = decalage + 4;   // incrementation decalage
      reglage = true; // reglages
    }
    if (decalage > 14 ) { // fin de la ligne d'affichage si >14
      decalage = 0;
      reglage = false;
    }
    mydisp.drawStr(decalage, 1, ""); // curseur position : decalage, ligne 1
  }
  if ((touche == 2 or touche == 3) and incrementation == menuHeure and relache == true and reglage == true ) { // si appui sur les touches 2 ou 3 pour reglage des valeurs
    relache = false;
    if (decalage == 4) {
      if (touche == 2 ) {
        if (tm.Hour < 24) {
          tm.Hour++; // incrementation de l heure
        } else {
          tm.Hour = 0;
        }
      }
      if (touche == 3 ) {
        if (tm.Hour > 0) {
          tm.Hour--; // decrementation de l heure
        } else {
          tm.Hour = 24;
        }
      }
      ecritureDateTime(); // routine écriture date and time
    }
    if (decalage == 8) {
      if (touche == 2) {
        if (tm.Minute < 59) {
          tm.Minute++; // incrementation des minutes
        } else {
          tm.Minute = 0;
        }
      }
      if (touche == 3 ) {
        if (tm.Minute > 0) {
          tm.Minute--; // decrementation de l heure
        } else {
          tm.Minute = 59;
        }
      }
      ecritureDateTime();
    }
    if (decalage == 12) {
      if (touche == 2 ) {
        if (tm.Second < 59) {
          tm.Second++; // incrementation des secondes
        } else {
          tm.Second = 0;
        }
      }
      if (touche == 3) {
        if (tm.Second > 0) {
          tm.Second--; // decrementation des secondes
        } else {
          tm.Second = 59;
        }
      }
      ecritureDateTime();
    }
  }
}
