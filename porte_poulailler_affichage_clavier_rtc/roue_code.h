/* roue codeuse */

//----montee , descente manuelle, position de la trappe-----
void monteeDescenteManuelle() {
  if (touche == 1 and incrementation == menuManuel ) {
    compteRoueCodeuse = finDeCourseB;
    pulse = pulseDescenteReduit;
  }  else if (touche == 4 and incrementation == menuManuel) {
    compteRoueCodeuse = finDeCourseH;
    pulse = pulseMonteeReduit;
  }
}

//-----compteur roue codeuse-----
void compteurRoueCodeuse() {
  // debounce
 // if (interruptRoueCodeuse) delay (2);  // attendre
  interruptRoueCodeuse = true; //activation de l'anti-rebond
  bool cpt = digitalRead(roueCodeuse);
  // Confirmation du changement
  if (cpt != positionRoueCodeuse) {
    positionRoueCodeuse = !positionRoueCodeuse;
    if (pulse >= pulseStop) {
      compteRoueCodeuse++;
    } else {
      compteRoueCodeuse--;
    }
    interruptRoueCodeuse = false; //libération de l'anti-rebond
  }
}



