/* servomoteur :
    -montée
    -descente
*/

//------mise sous tension du servo et montee de la porte-------
void servoOuverture() {
  if (!batterieFaible) { // si la batterie n'est pas faible
    servoAction = true; // servo en action
    montee = false;
    descente = true;
    digitalWrite(servoPin, HIGH); // mise soustension du servo
    pulse = pulseOuverture;
    monServo.write(pulse); // modification vitesse servo
    delay(100);
  }
}

//-----mise sous tension du servo et descente de la porte-----
void  servoFermeture() {
  if (!batterieFaible) { // si la batterie n'est pas faible
    servoAction = true; // servo en action
    descente = false;
    montee = true;
    digitalWrite(servoPin, HIGH); // mise soustension du servo
    pulse = pulseFermeture;
    monServo.write(pulse); // modification vitesse servo
    delay(100);
  }
}

//------sequence montee de la porte------
void ouverturePorte() {
  if ((compteRoueCodeuse <= finDeCourseB + 20) and montee == false) {  // 132 + 20
    pulse = pulseOuvertureReduit;
    monServo.write(pulse);  // value should usually be 500 to 2500 (environ 1280 = stop)
  }
  if ((touche == 5 and relache == true and montee == false) or (!digitalRead(securiteHaute) and montee == false)) {
    relache = false;
    digitalWrite(servoPin, LOW); // mise hors tension du servo led verte éteinte
    montee = true;
    if (!digitalRead(securiteHaute)) {
      delay(200); // attente fin l'arrêt complet du servo
      compteRoueCodeuse = finDeCourseB;
    }
    servoAction = false; // servo arret
  }
  if ( boitierOuvert) { // si le boitier est ouvert
    deroulementMenu (incrementation); // affichage du menu pour pulse
  }
}

//-----sequence descente de la porte-----
void  fermeturePorte() {
  if ((compteRoueCodeuse >= finDeCourseH - 20) and descente == false) { // 225 - 20
    pulse = pulseFermetureReduit;
    monServo.write(pulse);  // value should usually be 500 to 2500 (1280 = stop)
  }
  if ((compteRoueCodeuse >= finDeCourseH and descente == false ) or (touche == 5 and relache == true and descente == false) or (!digitalRead(securiteHaute) and descente == false)) {
    relache = false;
    digitalWrite(servoPin, LOW); // mise hors tension du servo led verte éteinte
    descente = true;
    if (!digitalRead(securiteHaute)) {
      delay(200); // attente fin l'arrêt complet du servo
      compteRoueCodeuse = finDeCourseB;
    }
    servoAction = false; // servo arret
  }
  if ( boitierOuvert) { // si le boitier est ouvert
    deroulementMenu (incrementation); // affichage du menu pour pulse
  }
}

