/* batteries */

//-------affichage tension batterie commandes
void affiTensionBatCdes() {
  int valBatCdes = analogRead(A2); // read the input on analog pin A2 : tension batterie commandes
  // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 6V)
  float voltage = valBatCdes * (7.50 / 1023.0);
  // print out the value you read:
  if ( TboitierOuvert) { // si le boitier est ouvert
    mydisp.print(F(" "));
    mydisp.print(valBatCdes);
    mydisp.print(F(" = "));
    mydisp.print(voltage);
    mydisp.print(F("V    "));
    mydisp.drawStr(0, 0, "");
  } else {
    if (DEBUG) {
      Serial.print(F("Ten bat cdes = "));
      Serial.print(valBatCdes);
      Serial.print(F(" = "));
      Serial.print(voltage);
      Serial.println(F(" V"));
    }
    if (RADIO) {
      char chaine1[VW_MAX_MESSAGE_LEN - 1] = "";
      char voltage_temp[6];
      dtostrf(voltage, 1, 2, voltage_temp);
      strcat(chaine1, voltage_temp);
      strcat(chaine1, "V");
      rad.envoiMessage(chaine1);// on envoie le message
    }
  }
}

//-------affichage tension batterie servo-moteur
void affiTensionBatServo() {
  int valBatServo = analogRead(A3); //read the input on analog pin A3 : tension batterie servo moteur
  // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 6V)
  float voltage = valBatServo * (7.50 / 1023.0);
  // print out the value you read:
  if ( TboitierOuvert) { // si le boitier est ouvert
    mydisp.print(F(" "));
    mydisp.print(valBatServo); //print tension batterie servo moteur
    mydisp.print(F(" = "));
    mydisp.print(voltage);
    mydisp.print(F("V    "));
    mydisp.drawStr(0, 0, "");
  } else {
    if (DEBUG) {
      Serial.print(F("Ten bat servo = "));
      Serial.print(valBatServo);
      Serial.print(F(" = "));
      Serial.print(voltage);
      Serial.println(F(" V"));
    }
    if (RADIO) {
      char chaine1[VW_MAX_MESSAGE_LEN - 1] = "";
      char voltage_temp[6];
      dtostrf(voltage, 1, 2, voltage_temp);
      strcat(chaine1, voltage_temp);
      strcat(chaine1, "V");
      rad.envoiMessage(chaine1);// on envoie le message
    }
  }
}

//-----batterie cdes < 4 volt-----
void accusFaible() {
  int valBatCdes = analogRead(A2); // read the input on analog pin A2 : tension batterie commandes
  if (valBatCdes < 654) { // si la batterie est faible < 4,8v (654)
    batterieFaible = true;
  } else {
    batterieFaible = false;
  }
}
