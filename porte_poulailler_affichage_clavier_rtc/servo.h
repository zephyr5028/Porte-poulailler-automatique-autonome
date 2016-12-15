/* servomoteur :
    -montée
    -descente
*/

//------mise sous tension du servo et montee de la porte-------
void servoOuverture() {
  if (!batterieFaible) { // si la batterie n'est pas faible
    servoAction = true; // servo en action
    ouverture = false;
    fermeture = true;
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
    fermeture = false;
    ouverture = true;
    digitalWrite(servoPin, HIGH); // mise soustension du servo
    pulse = pulseFermeture;
    monServo.write(pulse); // modification vitesse servo
    delay(100);
  }
}

//------sequence ouverture de la porte------
void ouverturePorte() {
  if ((compteRoueCodeuse <= finDeCourseOuverture + 20) and ouverture == false) {  // 132 + 20
    pulse = pulseOuvertureReduit;
    monServo.write(pulse);  // value should usually be 500 to 2500 (environ 1280 = stop)
  }
  if ((touche == 5 and relache == true and ouverture == false) or (!digitalRead(securiteHaute) and ouverture == false)) {
    relache = false;
    digitalWrite(servoPin, LOW); // mise hors tension du servo led verte éteinte
    ouverture = true;
    if (!digitalRead(securiteHaute)) {
      delay(200); // attente fin l'arrêt complet du servo
      compteRoueCodeuse = finDeCourseOuverture;
    }
    servoAction = false; // servo arret
  }
 /* if ( boitierOuvert) { // si le boitier est ouvert
    boucleTempo++;
    if (boucleTempo >= tempoLcd) {
      deroulementMenu (incrementation); // affichage du menu pour pulse
      boucleTempo = 0;
    }
  }*/
}

//-----sequence fermeture de la porte-----
void  fermeturePorte() {
  if ((compteRoueCodeuse >= finDeCourseFermeture - 20) and fermeture == false) { // 225 - 20
    pulse = pulseFermetureReduit;
    monServo.write(pulse);  // value should usually be 500 to 2500 (1280 = stop)
  }
  if ((compteRoueCodeuse >= finDeCourseFermeture and fermeture == false ) or (touche == 5 and relache == true and fermeture == false) or (!digitalRead(securiteHaute) and fermeture == false)) {
    relache = false;
    digitalWrite(servoPin, LOW); // mise hors tension du servo led verte éteinte
    fermeture = true;
    if (!digitalRead(securiteHaute)) {
      delay(200); // attente fin l'arrêt complet du servo
      compteRoueCodeuse = finDeCourseOuverture;
    }
    servoAction = false; // servo arret
  }
 /* if ( boitierOuvert) { // si le boitier est ouvert
    boucleTempo++;
    if (boucleTempo >= tempoLcd) {
      deroulementMenu (incrementation); // affichage du menu pour pulse
      boucleTempo = 0;
    }
  }*/
}

