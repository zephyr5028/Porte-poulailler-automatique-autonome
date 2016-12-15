/* affichages */

//-----routine display Date-----
void displayDate() {
  RTC.read(tm); // lecture date et heure
  // affichage du jour de la semaine
  byte j = ((tm.Wday - 1) * 3);
  for (byte i = j; i < j + 3; i++) {
    char dayWeek = pgm_read_byte(listeDayWeek + i);
    mydisp.print(dayWeek);
  }
  mydisp.print(F(" "));
  if (tm.Day < 10) {
    mydisp.print(F("0"));  // si < 10
  }
  mydisp.print(tm.Day, DEC); // print jour
  mydisp.print(F(" "));
  if (tm.Month < 10) {
    mydisp.print(F("0"));  // si < 10
  }
  mydisp.print(tm.Month, DEC); // print mois
  mydisp.print(F(" 20"));
  if (tm.Year < 10) {
    mydisp.print(F("0"));  // si < 10
  }
  mydisp.print(tm.Year - 30, DEC); // print année depuis 1970
  mydisp.print(F(" "));
  mydisp.drawStr(decalage, 1, ""); // curseur position 0 ligne 1
}

//-----routine display Time-----
void displayTime () {
  if ( TboitierOuvert) { // si le boitier est ouvert
    RTC.read(tm); // lecture date et heure
    if (tm.Hour < 10) {
      mydisp.print(F("0"));  // si < 10
    }
    mydisp.print(tm.Hour, DEC); // print heure
    mydisp.print(F("h "));
    if (tm.Minute < 10) {
      mydisp.print(F("0"));  // si < 10
    }
    mydisp.print(tm.Minute, DEC); // print minute
    mydisp.print(F("m "));
    if (tm.Second < 10) {
      mydisp.print(F("0"));  // si < 10
    }
    mydisp.print(tm.Second, DEC); // print seconde
    mydisp.print(F("s  "));
    mydisp.drawStr(decalage, 1, ""); // curseur position 0 ligne 1
  } else {
    if (DEBUG or RADIO) {
      byte timeHour =  bcdToDec(RTC.readRTC(0x02) & 0x3f); // heure
      byte timeMinute = bcdToDec(RTC.readRTC(0x01)); // minutes
      if (DEBUG) {
        Serial.print(F("Time : "));
        Serial.print(timeHour, DEC);
        Serial.print(F("h "));
        Serial.print(timeMinute, DEC);
        Serial.println(F("m"));
      }
      if (RADIO) {
        char chaine1[VW_MAX_MESSAGE_LEN - 1] = "";
        char hour_temp[3];
        char minute_temp[3];
        sprintf(hour_temp, "%i",  timeHour);
        strcat(chaine1, hour_temp);
        strcat(chaine1, "h");
        sprintf(minute_temp, "%i",  timeMinute);
        strcat(chaine1, minute_temp);
        strcat(chaine1, "m");
        rad.envoiMessage(chaine1);// on envoie le message
      }
    }
  }
}

//----routine door open time-----
void openTime() {
  byte val;
  val = bcdToDec(RTC.readRTC(0x09) & 0x3f); // alarme 1 hours
  mydisp.drawStr(0, 1, "   ");
  if (val < 10) {
    mydisp.print(F("0"));  // si < 10
  }
  mydisp.print(val, DEC);
  mydisp.print(F("h "));
  val = bcdToDec(RTC.readRTC(0x08)); // alarme 1 minutes
  if (val < 10) {
    mydisp.print(F("0"));  // si < 10
  }
  mydisp.print(val, DEC);
  mydisp.print(F("m "));
  val = bcdToDec(RTC.readRTC(0x07) & 0x7f); // alarme 1 seconds
  if (val < 10) {
    mydisp.print(F("0"));  // si < 10
  }
  mydisp.print(val, DEC);
  mydisp.print(F("s   "));
  mydisp.drawStr(decalage, 1, ""); // curseur position 0 ligne 1
}

//-----routine door close time-----
void closeTime() {
  //Set Alarm2
  byte val;
  val = bcdToDec(RTC.readRTC(0x0C) & 0x3f); // alarme 2 hours
  mydisp.drawStr(0, 1, "   ");
  if (val < 10) {
    mydisp.print(F("0"));  // si < 10
  }
  mydisp.print(val, DEC);
  mydisp.print(F("h "));
  val = bcdToDec(RTC.readRTC(0x0B)); //alarme 2 minutes
  if (val < 10) {
    mydisp.print(F("0"));  // si < 10
  }
  mydisp.print(val, DEC);
  mydisp.print(F("m      "));
  mydisp.drawStr(decalage, 1, ""); // curseur position 0 ligne 1
}

//------affichage pulse et comptage roue codeuse------
void affiPulsePlusCptRoue() {
  if ( TboitierOuvert) { // si le boitier est ouvert
    mydisp.print(F("P: "));
    mydisp.print(pulse);
    mydisp.print(F("  R: "));
    mydisp.print(compteRoueCodeuse);
    mydisp.print(F(" "));
    mydisp.drawStr(0, 0, "");
  } else {
    if (DEBUG) {
      Serial.print(F("Pulse servo = "));
      Serial.println(pulse);
      Serial.print(F("Cpt roue codeuse = "));
      Serial.println(compteRoueCodeuse);
      if ( compteRoueCodeuse > (finDeCourseFermeture - 5) and compteRoueCodeuse < (finDeCourseFermeture + 5)) {
        Serial.println(F("Porte fermee"));
      }
      if ( compteRoueCodeuse > (finDeCourseOuverture - 5) and compteRoueCodeuse < (finDeCourseOuverture + 5)) {
        Serial.println(F("Porte ouverte"));
      }
    }
    if (RADIO) {
      if ( compteRoueCodeuse > (finDeCourseFermeture - 5) and compteRoueCodeuse < (finDeCourseFermeture + 5)) {
        char chaine1[VW_MAX_MESSAGE_LEN - 1] = "";
        strcat(chaine1, "fer");
        rad.envoiMessage(chaine1);// on envoie le message
      }
      if ( compteRoueCodeuse > (finDeCourseOuverture - 5) and compteRoueCodeuse < (finDeCourseOuverture + 5)) {
        char chaine1[VW_MAX_MESSAGE_LEN - 1] = "";
        strcat(chaine1, "ouv");
        rad.envoiMessage(chaine1);// on envoie le message
      }
      char chaine1[VW_MAX_MESSAGE_LEN - 1] = "";
      char compteRoueCodeuse_temp[5];
      sprintf(compteRoueCodeuse_temp, "%i", compteRoueCodeuse);
      strcat(chaine1, compteRoueCodeuse_temp);
      rad.envoiMessage(chaine1);// on envoie le message
    }
  }
}

