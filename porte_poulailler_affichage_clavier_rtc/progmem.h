/* progmem  mémoire flash */

/*------Bibliothèque Flash pour mise en mémoire flash  F()--------*/
#include <Flash.h>

#include <avr/pgmspace.h> // non nécessaire maintenant

const char listeDayWeek[] PROGMEM = "DimLunMarMerJeuVenSam"; // day of week en mémoire flash
const char affichageMenu[] PROGMEM = "      Date      .      Heure     . Heure Ouverture. Heure Fermeture.  Temperature   .     Lumiere    .  Lumiere matin .  Lumiere soir  . Choix Ouv/Ferm . Fin de course H. Fin de course B. Volt bat Cdes  . Volt bat servo . Commande Manuel.";
const char affichageRadio[] PROGMEM = "Batterie faible !!!!.Temp = .Time : .Lum : .Fin course H = .Fin course B = .Ten Cdes = . Ten Servo = .Porte ouv !.Porte ferm !.";

