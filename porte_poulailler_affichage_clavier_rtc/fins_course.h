/* fins de course haut et bas */

//------affichage fin de course Haut-----
void affiFinDeCourseHaut() {
  if ( boitierOuvert) { // si le boitier est ouvert
    mydisp.print(F("   Fer : "));
    mydisp.print(finDeCourseFermeture);
    if (finDeCourseFermeture < 10) {
      mydisp.print(F("      "));
    } else {
      mydisp.print(F("     "));
    }
    mydisp.drawStr(decalage, 1, "");
  }
  if (DEBUG) {
    Serial.print(F("Fin course fermeture = ")); Serial.println(finDeCourseFermeture);
  }
  if (RADIO) {
    char chaine1[VW_MAX_MESSAGE_LEN - 1] = "";
    char finDeCourseFermeture_temp[5];
    sprintf(finDeCourseFermeture_temp, "%i", finDeCourseFermeture);
    strcat(chaine1, finDeCourseFermeture_temp);
    rad.envoiMessage(chaine1);// on envoie le message
  }
}

//------affichage fin de course Ouverture-------
void affiFinDeCourseOuverture() {
  if ( boitierOuvert) { // si le boitier est ouvert
    mydisp.print(F("   Ouv : "));
    mydisp.print(finDeCourseOuverture);
    if (finDeCourseOuverture < 10) {
      mydisp.print(F("      "));
    } else {
      mydisp.print(F("     "));
    }
    mydisp.drawStr(decalage, 1, "");
  }
  if (DEBUG) {
    Serial.print(F("Fin course ouverture = ")); Serial.println(finDeCourseOuverture);
  }
  if (RADIO) {
    char chaine1[VW_MAX_MESSAGE_LEN - 1] = "";
    char finDeCourseOuverture_temp[5];
    sprintf(finDeCourseOuverture_temp, "%i", finDeCourseOuverture);
    strcat(chaine1, finDeCourseOuverture_temp);
    rad.envoiMessage(chaine1);// on envoie le message
  }
}

//------reglage fin de course Fermeture------
void regFinDeCourseFermeture() {
  if (boitierOuvert) {
    if (touche == 4 and relache == true and incrementation == menuFinDeCourseFermeture) {
      relache = false;
      if (decalage < 15 ) { // boucle de reglage
        decalage += 8;   // incrementation decalage
        reglage = true; // reglages
      }
      if (decalage > 12 ) { // fin de la ligne d'affichage si >12
        decalage = 0;
        reglage = false;
      }
      mydisp.drawStr(decalage, 1, ""); // curseur position : decalage, ligne 1
    }
    if ((touche == 2 or touche == 3) and incrementation == menuFinDeCourseFermeture and relache == true and reglage == true ) { // si appui sur les touches 2 ou 3 pour reglage des valeurs
      relache = false;
      if (decalage == 8) {
        if (touche == 2 ) {
          if (finDeCourseFermeture < 500) {
            finDeCourseFermeture++; //incrementation de la fin de course haut
          } else {
            finDeCourseFermeture = 0;
          }
        }
        if (touche == 3 ) {
          if (finDeCourseFermeture > 0) {
            finDeCourseFermeture--; //decrementation de la fin de course haut
          } else {
            finDeCourseFermeture = 500;
          }
        }
        byte val1 = finDeCourseFermeture & 0xFF; // ou    byte val1= lowByte(sensorValue); // pf
        byte val2 = (finDeCourseFermeture >> 8) & 0xFF; // ou  byte val1= highByte(sensorValue); // Pf
        i2c_eeprom_write_byte(0x57, 0x20, val1); // écriture de la valeur du reglage de la fin de course haut low @20  de l'eeprom de la carte rtc (i2c @ 0x57)
        delay(10);
        i2c_eeprom_write_byte(0x57, 0x21, val2); // écriture de la valeur du reglage de la fin de course haut  high @21 de l'eeprom de la carte rtc (i2c @ 0x57)
        delay(10);
        mydisp.drawStr(0, 1, "");
        affiFinDeCourseHaut();
        mydisp.drawStr(decalage, 1, ""); // curseur position : decalage, ligne 1
      }
    }
  }
}

//-----regalge fin de course ouverture------
void regFinDeCourseOuverture() {
  if (boitierOuvert) {
    if (touche == 4 and relache == true and incrementation == menuFinDeCourseOuverture) {
      relache = false;
      if (decalage < 15 ) { // boucle de reglage
        decalage +=  8;   // incrementation decalage
        reglage = true; // reglages
      }
      if (decalage > 12 ) { // fin de la ligne d'affichage si >12
        decalage = 0;
        reglage = false;
      }
      mydisp.drawStr(decalage, 1, ""); // curseur position : decalage, ligne 1
    }
    if ((touche == 2 or touche == 3) and incrementation == menuFinDeCourseOuverture and relache == true and reglage == true ) { // si appui sur les touches 2 ou 3 pour reglage des valeurs
      relache = false;
      if (decalage == 8) {
        if (touche == 2 ) {
          if (finDeCourseOuverture < 500) {
            finDeCourseOuverture++; //incrementation de la fin de course bas
          } else {
            finDeCourseOuverture = 0;
          }
        }
        if (touche == 3 ) {
          if (finDeCourseOuverture > 0) {
            finDeCourseOuverture--; //decrementation de la fin de course bas
          } else {
            finDeCourseOuverture = 500;
          }
        }
        byte val1 = finDeCourseOuverture & 0xFF; // ou    byte val1= lowByte(sensorValue); // pf
        byte val2 = (finDeCourseOuverture >> 8) & 0xFF; // ou  byte val1= highByte(sensorValue); // Pf
        i2c_eeprom_write_byte(0x57, 0x22, val1); // écriture de la valeur du reglage de la fin de course bas low @22  de l'eeprom de la carte rtc (i2c @ 0x57)
        delay(10);
        i2c_eeprom_write_byte(0x57, 0x23, val2); // écriture de la valeur du reglage de la fin de course bas  high @23 de l'eeprom de la carte rtc (i2c @ 0x57)
        delay(10);
        mydisp.drawStr(0, 1, "");
        affiFinDeCourseOuverture();
        mydisp.drawStr(decalage, 1, ""); // curseur position : decalage, ligne 1
      }
    }
  }
}
