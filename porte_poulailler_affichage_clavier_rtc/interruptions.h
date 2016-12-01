/* interruptions
   -routine interruption D2 INT0
   -routine interruption D3 INT1
  -routine interruption Bp
  -routine alarme 2
  -routine alarme 1
  -routine interruption boitier ouvert
  -test fermeture boitier
*/

//-----routine interruption D2 INT0------
void myInterruptINT0() {
  if (!interruptRoueCodeuse and servoAction) {
    interruptRoueCodeuse = true; // compteur roue codeuse
    //compteurRoueCodeuse();
  }
}

//-----routine interruption D3 INT1-----
void myInterruptINT1() {
  if (!digitalRead(rtcINT)) { // entree 10 pour interruption RTC
    interruptRTC = true;
  }
  if (!digitalRead(BpOF) and !interruptBp) { // entree 9 pour interruption BP
    interruptBp = true;
    tempoDebounce = millis();
  }
  if (!digitalRead(interOuvBoitier) and !interruptOuvBoi) { // entree 6 pour interruption ouverture boitier
    interruptOuvBoi = true;
  }
}

//-----routine interruption Bp-----
void routineInterruptionBp() {
  if (interruptBp) { // test de l'appui sur bouton bp
    // If the switch changed, due to noise or pressing:
    if (((millis() - tempoDebounce) > debounce)  and  relacheBpOF == true and !digitalRead(BpOF) ) {
      relacheBpOF = false;
      if (pulse >= pulseStop) {
        servoDescente(); // mise sous tension du servo et descente de la porte
      } else {
        servoMontee(); // mise sous tension du servo et montee de la porte
      }
      tempoDebounce = millis() + 1000; // attente relache du bouton
    }
    if (((millis() - tempoDebounce) > debounce)  and  relacheBpOF == false and digitalRead(BpOF) ) {
      interruptBp = false; // autorisation de la prise en compte de l'IT
      relacheBpOF = true;
    }
  }
}

//-----routine alarme 2-----
void  routineInterrruptionAlarme2() {
  if ( RTC.alarm(alarm_2) and interruptRTC ) {    // has Alarm2 (fermeture) triggered?  alarme rtc
    if (!sens) {
      servoMontee(); // mise sous tension du servo et montee de la porte
    } else {
      servoDescente(); // mise sous tension du servo et descente de la porte
    }
    interruptRTC = false; // autorisation de la prise en compte de l'IT
  }
}

//-----routine alarme 1-----
void  routineInterruptionAlarme1() {
  if ( RTC.alarm(alarm_1) and interruptRTC ) {    // has Alarm1 (ouverture) triggered?  alarme rtc
    if (!sens) {
      servoDescente(); // mise sous tension du servo et descente de la porte
    } else {
      servoMontee(); // mise sous tension du servo et montee de la porte
    }
    interruptRTC = false; // autorisation de la prise en compte de l'IT
  }
}

//-----routine interruption boitier ouvert------
void  routineInterruptionBoitierOuvert() {
  if ( interruptOuvBoi and  (boitierOuvert == false)) {    //  interruption ouverture boitier
    if (!digitalRead(interOuvBoitier) ) { // ouverture boitier
      boitierOuvert = true; // boitier ouvert
    }
  }
}

//-----test fermeture boitier-----
void  routineTestFermetureBoitier() {
  if (digitalRead(interOuvBoitier) and  (boitierOuvert == true)) { //  fermeture boitier
    if (!retroEclairage)  {
      mydisp.backLightOff(); // retro eclairage off
      retroEclairage = true;
    }
    boitierOuvert = false; // boitier ferme
    interruptOuvBoi = false; // autorisation de la prise en compte de l'IT
  }
}
