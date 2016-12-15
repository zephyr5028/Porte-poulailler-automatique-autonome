/* choix pour l'ouverture et la fermeture :
     - heure
     - lumiere
*/

//------affichage du choix de l'ouverture et la fermeture------
void affiChoixOuvFerm() {
  if ( TboitierOuvert) { // si le boitier est ouvert
    mydisp.print(F(" Ouv:"));
    if (ouve == 1) {
      mydisp.print(F("Hre"));
    } else {
      mydisp.print(F("Lum"));
    }
    mydisp.print(F(" "));
    mydisp.print(F("Fer:"));
    if (ferm == 0) {
      mydisp.print(F("Lum"));
    } else {
      mydisp.print(F("Hre"));
    }
    mydisp.drawStr(decalage, 1, "");
  } else  {
    if (DEBUG) {
      Serial.print(F("Ouv : "));
      if (ouve == 1) {
        Serial.print(F("Hre"));
      } else {
        Serial.print(F("Lum "));
      }
      Serial.print(F("   Fer : "));
      if (ferm == 0) {
        Serial.println(F("Lum "));
      } else {
        Serial.println(F("Hre"));
      }
    }
  }
}

//-----routine choix ouverture fermeture-----
void choixOuvFerm () {
  if (touche == 4 and relache == true and incrementation == menuChoix) {
    relache = false;
    if (decalage < 15 ) { // boucle de reglage
      decalage = decalage + 7;   // incrementation decalage
      reglage = true; // reglages
    }
    if (decalage > 14 ) { // fin de la ligne d'affichage si >14
      decalage = 0;
      reglage = false;
    }
    mydisp.drawStr(decalage, 1, ""); // curseur position : decalage, ligne 1
  }
  if ((touche == 2 or touche == 3) and incrementation == menuChoix and relache == true and reglage == true ) { // si appui sur les touches 2 ou 3 pour reglage des valeurs
    relache = false;
    if (decalage == 7) {
      if (ouve == 1) {
        ouve = 0; // ouverture 0 donc lumière
        RTC.alarmInterrupt(alarm_1, false);     //disable Alarm1
      } else {
        ouve = 1; // ouverture 1 donc heure
        RTC.alarmInterrupt(alarm_1, true); // activation alarme 1 ouverture
      }
      i2c_eeprom_write_byte(0x57, 0x14, ouve); // écriture du type d'ouverture @14 de l'eeprom de la carte rtc (i2c @ 0x57)
      mydisp.drawStr(0, 1, "");
      affiChoixOuvFerm();
      mydisp.drawStr(decalage, 1, ""); // curseur position : decalage, ligne 1
    }
    if (decalage == 14) {
      if (ferm == 0) {
        ferm = 1; // fermeture 1 donc heure
        RTC.alarmInterrupt(alarm_2, true);// activation alarme 2 fermeture
      } else {
        ferm = 0; // fermeture 0 donc lumière
        RTC.alarmInterrupt(alarm_2, false);     //disable Alarm2
      }
      i2c_eeprom_write_byte(0x57, 0x15, ferm); // écriture du type de fermeture @15 de l'eeprom de la carte rtc (i2c @ 0x57)
      mydisp.drawStr(0, 1, "");
      affiChoixOuvFerm();
      mydisp.drawStr(decalage, 1, ""); // curseur position : decalage, ligne 1
    }
  }
}
