/* afficheur */

#define LCDCol 16
#define LCDRow 2

//-----retro eclairage de l'afficheur-----
void eclairageAfficheur() {
  if (touche == 5 and relache == true and fermeture == false and  ouverture == false) { // retro eclairage si appuis sur la touche 5
    relache = false;
    if (retroEclairage)  {
      mydisp.backLightOn(); // retro eclairage on
      retroEclairage = false;
    } else {
      mydisp.backLightOff(); // retro eclairage off
      retroEclairage = true;
    }
  }
}

//---- temporisation pour l'affichage, affichage du menu lorsque temps est >....
void temporisationAffichage() {
  if ( temps > 1000) {
    deroulementMenu (incrementation); // affichage du menu en fonction de incrementation
    temps = 0;
  } else {
    temps = temps + 1;
  }
}
