/* reglage heure ouverture */

//-----routine de reglage de l'heure d'ouverture-----
void reglageHeureOuverture() {
  if (touche == 4 and relache == true and incrementation == menuOuverture) {
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
  // Set Alarm1
  if ((touche == 2 or touche == 3) and incrementation == menuOuverture and relache == true and reglage == true ) { // si appui sur les touches 2 ou 3 pour reglage des valeurs
    relache = false;
    byte alarm1Hour =  bcdToDec(RTC.readRTC(0x09) & 0x3f); // alarme 1 hours
    byte alarm1Minute = bcdToDec(RTC.readRTC(0x08)); // alarme 1 minutes
    byte alarm1Second =  bcdToDec(RTC.readRTC(0x07) & 0x7f); // alarme 1 seconds
    if (decalage == 4) {
      if (touche == 2 ) {
        if (alarm1Hour < 24) {
          alarm1Hour++; // incrementation de l heure
        } else {
          alarm1Hour = 0;
        }
      }
      if (touche == 3 ) {
        if (alarm1Hour > 0) {
          alarm1Hour--; // decrementation de l heure
        } else {
          alarm1Hour = 24;
        }
      }
      RTC.setAlarm(ALM1_MATCH_HOURS, alarm1Second, alarm1Minute, alarm1Hour, 0); // écriture alarm 1
      mydisp.drawStr(0, 1, "   ");
      openTime(); // affichage de l'heure d'ouverture
      mydisp.drawStr(decalage, 1, ""); // curseur position : decalage, ligne 1
    }
    if (decalage == 8) {
      // byte alarm1Minute = bcdToDec(RTC.readRTC(0x08)); // alarme 1 minutes
      if (touche == 2) {
        if (alarm1Minute < 59) {
          alarm1Minute++; // incrementation des minutes
        } else {
          alarm1Minute = 0;
        }
      }
      if (touche == 3 ) {
        if (alarm1Minute > 0) {
          alarm1Minute--; // decrementation de l heure
        } else {
          alarm1Minute = 59;
        }
      }
      RTC.setAlarm(ALM1_MATCH_HOURS, alarm1Second, alarm1Minute, alarm1Hour, 0); // écriture alarm 1
      mydisp.drawStr(0, 1, "   ");
      openTime(); // affichage de l'heure d'ouverture
      mydisp.drawStr(decalage, 1, ""); // curseur position : decalage, ligne 1
    }
    if (decalage == 12) {
      if (touche == 2 ) {
        if (alarm1Second < 59) {
          alarm1Second++; // incrementation des secondes
        } else {
          alarm1Second = 0;
        }
      }
      if (touche == 3) {
        if ( alarm1Second > 0) {
          alarm1Second--; // decrementation des secondes
        } else {
          alarm1Second = 59;
        }
      }
      RTC.setAlarm(ALM1_MATCH_HOURS, alarm1Second, alarm1Minute, alarm1Hour, 0); // écriture alarm 1
      mydisp.drawStr(0, 1, "   ");
      openTime(); // affichage de l'heure d'ouverture
      mydisp.drawStr(decalage, 1, ""); // curseur position : decalage, ligne 1
    }
  }
}

