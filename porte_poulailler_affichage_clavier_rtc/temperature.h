/* temperature */

//-----routine lecture température sur ds3231 rtc type celsius=true ,fahrenheit=false------
void read_temp(boolean type) {
  int t = RTC.temperature();
  float celsius = t / 4.0;
  float fahrenheit = celsius * 9.0 / 5.0 + 32.0;
  if ( boitierOuvert) { // si le boitier est ouvert
    if (type) {
      mydisp.print(celsius); // affichage celsius
      mydisp.print(F(" C     "));
    } else {
      mydisp.print(fahrenheit); // affichage fahrenheit
      mydisp.print(F(" F     "));
    }
  } else {
    if (DEBUG) {
      Serial.print(F("Temp = "));
      if (type) {
        Serial.print(celsius); Serial.println(F(" °C"));// affichage celsius
      } else {
        Serial.print(fahrenheit); Serial.println(F(" F"));// affichage fahrenheit
      }
    }
    if (RADIO) {
      // N.B. La constante VW_MAX_MESSAGE_LEN est fournie par la lib VirtualWire
      //char chaine1[VW_MAX_MESSAGE_LEN - 1] = "Temp = ";
      char chaine1[16] = "Temp = ";
      char celsius_temp[6];
      dtostrf(celsius, 2, 2, celsius_temp);
      strcat(chaine1, celsius_temp);
      strcat(chaine1, "°C");
      envoiMessage(chaine1);// on envoie le message
    }
  }
}
