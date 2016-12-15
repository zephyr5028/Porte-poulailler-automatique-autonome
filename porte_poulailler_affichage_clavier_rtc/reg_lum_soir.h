/* reglage de la lumier du soir  */

//------affichage de la lumiere du soir-------
void affiLumSoir() {
  if (boitierOuvert) {
    mydisp.print(F("   Lum: "));
    mydisp.print(lumSoir);
    if (lumSoir == 0) {
      mydisp.print(F("       "));
    } else {
      mydisp.print(F("      "));
    }
    mydisp.drawStr(decalage, 1, "");
  }
}

//------reglage du choix de la lumiere du soir--------
void choixLumSoir() {
  if (boitierOuvert) {
    if (touche == 4 and relache == true and incrementation == menuLumiereSoir) {
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
    if ((touche == 2 or touche == 3) and incrementation == menuLumiereSoir and relache == true and reglage == true ) { // si appui sur les touches 2 ou 3 pour reglage des valeurs
      relache = false;
      if (decalage == 8) {
        if (touche == 2 ) {
          if (lumSoir < 1020) {
            lumSoir = lumSoir + 10; //incrementation de la lumiere du soir
          } else {
            lumSoir = 0;
          }
        }
        if (touche == 3 ) {
          if (lumSoir > 0) {
            lumSoir = lumSoir - 10; //decrementation de la lumiere du soir
          } else {
            lumSoir = 1020;
          }
        }
        byte val1 = lumSoir & 0xFF; // ou    byte val1= lowByte(sensorValue); // pf
        byte val2 = (lumSoir >> 8) & 0xFF; // ou  byte val1= highByte(sensorValue); // Pf
        i2c_eeprom_write_byte(0x57, 0x18, val1); // écriture de la valeur du reglage de la lumiere du soir low @18  de l'eeprom de la carte rtc (i2c @ 0x57)
        delay(10);
        i2c_eeprom_write_byte(0x57, 0x19, val2); // écriture de la valeur du reglage de la lumiere du soir high @19 de l'eeprom de la carte rtc (i2c @ 0x57)
        delay(10);
        mydisp.drawStr(0, 1, "");
        affiLumSoir();
        mydisp.drawStr(decalage, 1, ""); // curseur position : decalage, ligne 1
      }
    }
  }
}
