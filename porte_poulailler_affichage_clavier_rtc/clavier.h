/* clavier */

//-----lecture clavier------
void  lectureClavier() {
    touche = clav.read_key(sensorClavier); // read key sensor = A1     
  if (touche == oldkey ) { // si touche == -1, donc touche relache=true
    relache = true;
  }
  if ((touche == 2 or touche == 3) and relache and !reglage) { // si appui sur les touches 2 ou 3 et la touche relache=true et mode reglage
    relache = false;
    incrementation = clav.positionMenu(incrementation, touche); // position du menu pour l'affichage
    deroulementMenu (incrementation); // affichage du menu
  }
}

