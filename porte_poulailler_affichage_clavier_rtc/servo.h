/* servomoteur :
    -montée
    -descente
*/

//------mise sous tension du servo et montee de la porte-------
void servoMontee() {
  if (!batterieFaible) { // si la batterie n'est pas faible
    servoAction = true; // servo en action
    montee = true;
    descente = false;
    digitalWrite(servoPin, HIGH); // mise soustension du servo
    pulse = pulseMontee;
    monServo.write(pulse); // modification vitesse servo
    delay(100);
  }
}

//-----mise sous tension du servo et descente de la porte-----
void  servoDescente() {
  if (!batterieFaible) { // si la batterie n'est pas faible
    servoAction = true; // servo en action
    descente = true;
    montee = false;
    digitalWrite(servoPin, HIGH); // mise soustension du servo
    pulse = pulseDescente;
    monServo.write(pulse); // modification vitesse servo
    delay(100);
  }
}

//------sequence de montee de la porte------
void monteePorte() {
  if ((compteRoueCodeuse >= finDeCourseH - 10) and montee == true) {
    if (sens) {
      pulse = pulseMonteeReduit;
    } else {
      pulse = pulseMonteeReduitSens;
    }
    monServo.write(pulse);  // value should usually be 500 to 2500 (environ 1280 = stop)
  }
  if ((compteRoueCodeuse >= finDeCourseH + 2 and montee == true and sens == false) or (touche == 5 and relache == true and montee == true) or (!digitalRead(securiteHaute) and montee == true)) {
    relache = false;
    digitalWrite(servoPin, LOW); // mise hors tension du servo led verte éteinte
    montee = false;
    if (!digitalRead(securiteHaute)) {
      delay(300); // attente fin l'arrêt complet du servo
      compteRoueCodeuse = finDeCourseH;
      //   sens = true; // sens de fonctionnement du servo pour éviter de bloquer le servo en cas de probleme
    }
    servoAction = false; // servo arret
  }
  if ( boitierOuvert) { // si le boitier est ouvert
    deroulementMenu (incrementation); // affichage du menu pour pulse
  }
}

//-----sequence de descente de la porte-----
void  descentePorte() {
  if ((compteRoueCodeuse <= finDeCourseB + 20) and descente == true) {
    if (sens) {
      pulse =  pulseDescenteReduitSens;
    } else {
      pulse = pulseDescenteReduit;
    }
    monServo.write(pulse);  // value should usually be 500 to 2500 (1280 = stop)
  }
  if ((compteRoueCodeuse <= finDeCourseB and descente == true and sens == true) or (touche == 5 and relache == true and descente == true) or (!digitalRead(securiteHaute) and descente == true)) {
    relache = false;
    digitalWrite(servoPin, LOW); // mise hors tension du servo led verte éteinte
    descente = false;
    if (!digitalRead(securiteHaute)) {
      delay(300); // attente fin l'arrêt complet du servo
      compteRoueCodeuse = finDeCourseB;
      //   sens = false ; // sens de fonctionnement pour éviter de bloquer le servo en cas de probleme
    }
    servoAction = false; // servo arret
  }
  if ( boitierOuvert) { // si le boitier est ouvert
    deroulementMenu (incrementation); // affichage du menu pour pulse
  }
}
