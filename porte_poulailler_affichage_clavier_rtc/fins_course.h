/* fins de course haut et bas */

//------affichage fin de course Haut-----
void affiFinDeCourseHaut() {
  if ( boitierOuvert) { // si le boitier est ouvert
    mydisp.print(F("   H : "));
    mydisp.print(finDeCourseH);
    if (finDeCourseH < 10) {
      mydisp.print(F("        "));
    } else {
      mydisp.print(F("       "));
    }
    mydisp.drawStr(decalage, 1, "");
  } else {
    if (DEBUG) {
      Serial.print(F("Fin course H = ")); Serial.println(finDeCourseH);
    }
    if (RADIO) {
      //char chaine1[VW_MAX_MESSAGE_LEN - 1] = "F. course H = ";
      char chaine1[22] = "Fin course H = ";
      char finDeCourseH_temp[5];
      sprintf(finDeCourseH_temp, "%i", finDeCourseH);
      strcat(chaine1, finDeCourseH_temp);
      envoiMessage(chaine1);// on envoie le message
    }
  }
}

//------affichage fin de course Bas-------
void affiFinDeCourseBas() {
  if ( boitierOuvert) { // si le boitier est ouvert
    mydisp.print(F("   B : "));
    mydisp.print(finDeCourseB);
    if (finDeCourseB < 10) {
      mydisp.print(F("        "));
    } else {
      mydisp.print(F("       "));
    }
    mydisp.drawStr(decalage, 1, "");
  } else {
    if (DEBUG) {
      Serial.print(F("Fin course B = ")); Serial.println(finDeCourseB);
    }
    if (RADIO) {
      //char chaine1[VW_MAX_MESSAGE_LEN - 1] = "F. course B = ";
      char chaine1[22] = "Fin course B = ";
      char finDeCourseB_temp[5];
      sprintf(finDeCourseB_temp, "%i", finDeCourseB);
      strcat(chaine1, finDeCourseB_temp);
      envoiMessage(chaine1);// on envoie le message
    }
  }
}

//------reglage fin de course Haut------
void regFinDeCourseHaut() {
  if (touche == 4 and relache == true and incrementation == menuFinDeCourseH) {
    relache = false;
    if (decalage < 15 ) { // boucle de reglage
      decalage = decalage + 8;   // incrementation decalage
      reglage = true; // reglages
    }
    if (decalage > 12 ) { // fin de la ligne d'affichage si >12
      decalage = 0;
      reglage = false;
    }
    mydisp.drawStr(decalage, 1, ""); // curseur position : decalage, ligne 1
  }
  if ((touche == 2 or touche == 3) and incrementation == menuFinDeCourseH and relache == true and reglage == true ) { // si appui sur les touches 2 ou 3 pour reglage des valeurs
    relache = false;
    if (decalage == 8) {
      if (touche == 2 ) {
        if (finDeCourseH < 500) {
          finDeCourseH = finDeCourseH + 1; //incrementation de la fin de course haut
        } else {
          finDeCourseH = 0;
        }
      }
      if (touche == 3 ) {
        if (finDeCourseH > 0) {
          finDeCourseH = finDeCourseH - 1; //decrementation de la fin de course haut
        } else {
          finDeCourseH = 500;
        }
      }
      byte val1 = finDeCourseH & 0xFF; // ou    byte val1= lowByte(sensorValue); // pf
      byte val2 = (finDeCourseH >> 8) & 0xFF; // ou  byte val1= highByte(sensorValue); // Pf
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

//-----regalge fin de course Bas------
void regFinDeCourseBas() {
  if (touche == 4 and relache == true and incrementation == menuFinDeCourseB) {
    relache = false;
    if (decalage < 15 ) { // boucle de reglage
      decalage = decalage + 8;   // incrementation decalage
      reglage = true; // reglages
    }
    if (decalage > 12 ) { // fin de la ligne d'affichage si >12
      decalage = 0;
      reglage = false;
    }
    mydisp.drawStr(decalage, 1, ""); // curseur position : decalage, ligne 1
  }
  if ((touche == 2 or touche == 3) and incrementation == menuFinDeCourseB and relache == true and reglage == true ) { // si appui sur les touches 2 ou 3 pour reglage des valeurs
    relache = false;
    if (decalage == 8) {
      if (touche == 2 ) {
        if (finDeCourseB < 500) {
          finDeCourseB = finDeCourseB + 1; //incrementation de la fin de course bas
        } else {
          finDeCourseB = 0;
        }
      }
      if (touche == 3 ) {
        if (finDeCourseB > 0) {
          finDeCourseB = finDeCourseB - 1; //decrementation de la fin de course bas
        } else {
          finDeCourseB = 500;
        }
      }
      byte val1 = finDeCourseB & 0xFF; // ou    byte val1= lowByte(sensorValue); // pf
      byte val2 = (finDeCourseB >> 8) & 0xFF; // ou  byte val1= highByte(sensorValue); // Pf
      i2c_eeprom_write_byte(0x57, 0x22, val1); // écriture de la valeur du reglage de la fin de course bas low @22  de l'eeprom de la carte rtc (i2c @ 0x57)
      delay(10);
      i2c_eeprom_write_byte(0x57, 0x23, val2); // écriture de la valeur du reglage de la fin de course bas  high @23 de l'eeprom de la carte rtc (i2c @ 0x57)
      delay(10);
      mydisp.drawStr(0, 1, "");
      affiFinDeCourseBas();
      mydisp.drawStr(decalage, 1, ""); // curseur position : decalage, ligne 1
    }
  }
}
