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
  if (interruptRoueCodeuse) {
    if (pulse >= pulseStop) {
      compteRoueCodeuse++;
    } else {
      compteRoueCodeuse--;
    }
    interruptRoueCodeuse = false;
  }
}
