/* reglage de la lumiere du matin */

//------affichage de la lumiere du matin------
void affiLumMatin() {
  if (boitierOuvert) {
    mydisp.print(F("   Lum: "));
    mydisp.print(lumMatin);
    if (lumMatin == 0) {
      mydisp.print(F("       "));
    } else {
      mydisp.print(F("      "));
    }
    mydisp.drawStr(decalage, 1, "");
  }
}

//-----reglage du choix de la lumiere du matin-------
void choixLumMatin() {
  if (boitierOuvert) {
    if (touche == 4 and relache == true and incrementation == menuLumiereMatin) {
      relache = false;
      if (decalage < 15 ) { // boucle de reglage
        decalage = decalage + 8;   // incrementation decalage
        reglage = true; // reglages
      }
      if (decalage > 14 ) { // fin de la ligne d'affichage si >14
        decalage = 0;
        reglage = false;
      }
      mydisp.drawStr(decalage, 1, ""); // curseur position : decalage, ligne 1
    }
    if ((touche == 2 or touche == 3) and incrementation == menuLumiereMatin and relache == true and reglage == true ) { // si appui sur les touches 2 ou 3 pour reglage des valeurs
      relache = false;
      if (decalage == 8) {
        if (touche == 2 ) {
          if (lumMatin < 1020) {
            lumMatin = lumMatin + 10; //incrementation de la lumiere du matin
          } else {
            lumMatin = 0;
          }
        }
        if (touche == 3 ) {
          if (lumMatin > 0) {
            lumMatin = lumMatin - 10; //decrementation de la lumiere du matin
          } else {
            lumMatin = 1020;
          }
        }
        byte val1 = lumMatin & 0xFF; // ou    byte val1= lowByte(sensorValue);// pf
        byte val2 = (lumMatin >> 8) & 0xFF; // ou  //byte val1= highByte(sensorValue); // Pf
        i2c_eeprom_write_byte(0x57, 0x16, val1); // écriture de la valeur du reglage de la lumiere du matin low @16  de l'eeprom de la carte rtc (i2c @ 0x57)
        delay(10);
        i2c_eeprom_write_byte(0x57, 0x17, val2); // écriture de la valeur du reglage de la lumiere du matin high @17 de l'eeprom de la carte rtc (i2c @ 0x57)
        delay(10);
        mydisp.drawStr(0, 1, "");
        affiLumMatin();
        mydisp.drawStr(decalage, 1, ""); // curseur position : decalage, ligne 1
      }
    }
  }
}
