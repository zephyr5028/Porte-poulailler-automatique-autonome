/* reglage de la date */

//----routine reglage jour semaine, jour, mois, annee-----
void reglageDate () {
  if (touche == 4 and relache == true and incrementation == menuDate) {
    relache = false;
    if (decalage < 14 ) { // boucle de reglage
      decalage = decalage + 3;   // incrementation decalage
      reglage = true; // reglages
    }
    if (decalage > 14 ) { // fin de la ligne d'affichage si >14
      decalage = 0;
      reglage = false;
    }
    mydisp.drawStr(decalage, 1, ""); // curseur position : decalage, ligne 1
  }
  if ((touche == 2 or touche == 3) and incrementation == menuDate and relache == true and reglage == true) { // si appui sur les touches 2 ou 3 pour reglage des valeurs
    relache = false;
    if (decalage == 3) {
      if (touche == 2) {
        if (tm.Wday < 7) {
          mydisp.drawStr(decalage - 2, 1, ""); // curseur position decalage-2, ligne 1
          tm.Wday++; // incrementation du jour de la semaine
        } else {
          mydisp.drawStr(decalage - 2, 1, ""); // curseur position decalage-2, ligne 1
          tm.Wday = 1; // dimanche
        }
      }
      if (touche == 3) {
        if (tm.Wday > 1) {
          mydisp.drawStr(decalage - 2, 1, ""); // curseur position decalage-2, ligne 1
          tm.Wday--; // decrementation du jour de la semaine
        } else {
          mydisp.drawStr(decalage - 2, 1, ""); // curseur position decalage-2, ligne 1
          tm.Wday = 7;
        }
      }
      ecritureDateTime();
    }
    if (decalage == 6) {
      if (touche == 2) {
        if (tm.Day < 31) {
          tm.Day++; // incrementation des jours
        } else {
          tm.Day = 1;
        }
      }
      if (touche == 3 ) {
        mydisp.drawStr(decalage - 1, 1, ""); // curseur position decalage-2, ligne 1
        if (tm.Day > 1) {
          tm.Day--; // decrementation du jour
        } else {
          tm.Day = 31;
        }
      }
      ecritureDateTime();
    }
    if (decalage == 9) {
      if (touche == 2) {
        if (tm.Month < 12) {
          tm.Month++; // incrementation des mois
        } else {
          tm.Month = 1;
        }
      }
      if (touche == 3) {
        if (tm.Month > 1) {
          tm.Month--; // decrementation des mois
        } else {
          tm.Month = 12;
        }
      }
      ecritureDateTime();
    }
    if (decalage == 12 ) {
      if (touche == 2 ) {
        if (tm.Year < 9) {
          mydisp.drawStr(decalage + 1, 1, "0"); // curseur position decalage-2, ligne 1
        } else {
          mydisp.drawStr(decalage + 1, 1, ""); // curseur position decalage-2, ligne 1
        }
        tm.Year++; // incrementation de l'année
      }
      if (touche == 3 ) {
        if (tm.Year < 11) {
          mydisp.drawStr(decalage + 1, 1, "0"); // curseur position decalage-2, ligne 1
        } else {
          mydisp.drawStr(decalage + 1, 1, ""); // curseur position decalage-2, ligne 1
        }
        if (tm.Year > 0) {
          tm.Year--; // decrementation de l'année si > 2000
        }
      }
      ecritureDateTime();
    }
  }
}

