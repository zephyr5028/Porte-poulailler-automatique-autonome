/* lumiere
  -affichage
*/

//-----routine lecture et affichage de la lumière-----
void lumiere() {
  int lumValue = analogRead(A0); // read the input on analog pin 0
  // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V)
  float voltage = lumValue * (5.0 / 1023.0);
  if ( boitierOuvert) { // si le boitier est ouvert
    // print out the value you read:
    mydisp.print(F(" "));
    mydisp.print(lumValue);
    mydisp.print(F(" = "));
    mydisp.print(voltage);
    mydisp.print(F("V    "));
  } else {
    if (DEBUG) {
      Serial.print(F("lum : "));
      Serial.println(lumValue);
    }
    if (RADIO) {
      //char chaine1[VW_MAX_MESSAGE_LEN - 1] = "Lum : ";
      char chaine1[12] = "Lum : ";
      char lumValue_temp[5];
      sprintf(lumValue_temp, "%i",  lumValue);
      strcat(chaine1, lumValue_temp);
      envoiMessage(chaine1);// on envoie le message
    }
  }
}

//-----ouverture/fermeture par test de la lumière----
void ouvFermLum() {
  byte  valHeure = bcdToDec(RTC.readRTC(0x02) & 0x3f); // lecture de l'heure
  sensorValue = analogRead(A0); // read the input on analog pin 0 : lumière
  if (sensorValue > lumMatin and sensorValue < lumSoir) {
    compteurWatchdogLumiere = 0; //raz du compteur watchdog lumiere pour ne pas prendre en compte une ombre
  }
  if ((ouve == 1 and valHeure < 16) or (ferm == 1 and valHeure > 16)) { // pour ne pas declencher la fermeture avant 16h00 et l'ouverture après 16h00 si utilisation de l'heure
    compteurWatchdogLumiere = 0; //raz du compteur watchdog lumiere pour ne pas prendre en compte une ombre
  }
  if ((compteRoueCodeuse <= (finDeCourseB + 2) and sens) or (compteRoueCodeuse >= (finDeCourseH - 2) and !sens )) {
    compteurWatchdogLumiere = 0; // pour ne pas relancer l'action une fois effectuée.
  }
  if ((sensorValue <= lumMatin) and (ouve == 0) and (compteurWatchdogLumiere >= tempsLum)) {
    compteurWatchdogLumiere = 0; //raz du compteur watchdog lumiere pour ne pas prendre en compte une ombre
    if (sens) {
      servoMontee(); // mise sous tension du servo et montee de la porte
    } else {
      servoDescente(); // mise sous tension du servo et descente de la porte
    }
  }
  if ((sensorValue >= lumSoir) and (ferm == 0) and (compteurWatchdogLumiere >= tempsLum)) {
    compteurWatchdogLumiere = 0; //raz du compteur watchdog lumiere pour ne pas prendre en compte une ombre
    if (sens) {
      servoDescente(); // mise sous tension du servo et descente de la porte
    } else {
      servoMontee(); // mise sous tension du servo et montee de la porte
    }

  }
}


