/* reglage heure fermeture */

//-----reglage de l'heure de fermeture------
void reglageHeureFermeture() {
  if (boitierOuvert) {
    if (touche == 4 and relache == true and incrementation == menuFermeture) {
      relache = false;
      if (decalage < 10 ) { // boucle de reglage
        decalage = decalage + 4;   // incrementation decalage
        reglage = true; // reglages
      }
      if (decalage > 10 ) { // fin de la ligne d'affichage si >10
        decalage = 0;
        reglage = false;
      }
      mydisp.drawStr(decalage, 1, ""); // curseur position : decalage, ligne 1
    }
    if ((touche == 2 or touche == 3) and incrementation == menuFermeture and relache == true and reglage == true ) { // si appui sur les touches 2 ou 3 pour reglage des valeurs
      relache = false;
      byte alarm2Hour =  bcdToDec(RTC.readRTC(0x0C) & 0x3f); // alarme 2 hours
      byte alarm2Minute = bcdToDec(RTC.readRTC(0x0B)); // alarme 2 minutes
      if (decalage == 4) {
        if (touche == 2 ) {
          if (alarm2Hour < 24) {
            alarm2Hour++; // incrementation de l heure
          } else {
            alarm2Hour = 0;
          }
        }
        if (touche == 3 ) {
          if (alarm2Hour > 0) {
            alarm2Hour--; // decrementation de l heure
          } else {
            alarm2Hour = 24;
          }
        }
        RTC.setAlarm(ALM2_MATCH_HOURS, alarm2Minute, alarm2Hour, 0); // écriture de l'heure alarme 2
        mydisp.drawStr(0, 1, "   ");
        closeTime(); // affichage de l'heure d'ouverture
        mydisp.drawStr(decalage, 1, ""); // curseur position : decalage, ligne 1
      }
      if (decalage == 8) {
        if (touche == 2) {
          if (alarm2Minute < 59) {
            alarm2Minute++; // incrementation des minutes
          } else {
            alarm2Minute = 0;
          }
        }
        if (touche == 3 ) {
          if (alarm2Minute > 0) {
            alarm2Minute--; // decrementation de l'heure
          } else {
            alarm2Minute = 59;
          }
        }
        RTC.setAlarm(ALM2_MATCH_HOURS, alarm2Minute, alarm2Hour, 0);  // écriture de l'heure alarme 2
        mydisp.drawStr(0, 1, "   ");
        closeTime(); // affichage de l'heure d'ouverture
        mydisp.drawStr(decalage, 1, ""); // curseur position : decalage, ligne 1
      }
    }
  }
}

