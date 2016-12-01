/* Test du servo avec la console arduino

    a = -1
    q = -1

    z = +10
    s = -10

    e = arret du servo (relais)
    d = marche du servo (relais)

    r = montée
    f = descente

*/
// reglage du servo plus test de la roue codeuse et du servo, à l'aide de la console
void testServo() {
  //des données sur la liaison série : (lorsque l'on appuie sur a, q, z, s, e, d )
  if (Serial.available())    {
    char commande = Serial.read(); //on lit
    //on modifie la consigne si c'est un caractère qui nous intéresse
    if (commande == 'a') {
      pulse += 1;  //ajout de 1µs au temps HAUT
      Serial.println(monServo.read());
      monServo.write(pulse); // modification vitesse servo
      Serial.println(monServo.read());
    }
    else if (commande == 'q') {
      pulse -= 1;  //retrait de 1µs au temps HAUT
      Serial.println(monServo.read());
      monServo.write(pulse); // modification vitesse servo
      Serial.println(monServo.read());
    }
    else if (commande == 'z') {
      pulse += 10;  //ajout de 10µs au temps HAUT
      Serial.println(monServo.read());
      monServo.write(pulse); // modification vitesse servo
      Serial.println(monServo.read());
    }
    else if (commande == 's') {
      pulse -= 10;  //retrait de 10µs au temps HAUT
      Serial.println(monServo.read());
      monServo.write(pulse); // modification vitesse servo
      Serial.println(monServo.read());
    }
    else if (commande == 'e') {
      digitalWrite(servoPin, LOW); // mise hors tension du servo
    }
    else if (commande ==  'd') {
      digitalWrite(servoPin, HIGH); // mise soustension du servo
    }
    else if (commande == 'r' ) {
      //servoMontee(); // mise sous tension du servo et montee de la porte
      servoAction = true; // servo en action
      montee = true;
      descente = false;
      digitalWrite(servoPin, HIGH); // mise soustension du servo
      delay(50);
      pulse = pulseMontee;
      Serial.println(monServo.read());
      monServo.write(pulse); // modification vitesse servo
      Serial.println(monServo.read());
    }
    else if (commande == 'f') {
      //servoDescente(); // mise sous tension du servo et descente de la porte
      servoAction = true; // servo en action
      descente = true;
      montee = false;
      digitalWrite(servoPin, HIGH); // mise soustension du servo
      delay(50);
      pulse = pulseDescente;
      Serial.println(monServo.read());
      monServo.write(pulse); // modification vitesse servo
      Serial.println(monServo.read());
    }
    //et on fait un retour sur la console
    Serial.print("Etat de l'impulsion du servo = ");
    Serial.print(pulse);
    Serial.println(" ms");
  }
}

