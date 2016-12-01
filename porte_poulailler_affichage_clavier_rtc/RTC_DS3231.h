/* RTC_DS3231 */

#include <DS3232RTC.h>    //http://github.com/JChristensen/DS3232RTC
#include <Time.h>         //http://www.arduino.cc/playground/Code/Time  
#define DS3231_I2C_ADDRESS 0x68

tmElements_t tm; // declaration de tm pour la lecture des informations date et heure

//-----routine decToBcd : Convert normal decimal numbers to binary coded decimal-----
byte decToBcd(byte val) {
  return ( (val / 10 * 16) + (val % 10) );
}

//-----routine bcdToDec : Convert binary coded decimal to normal decimal numbers-----
byte bcdToDec(byte val) {
  return ( (val / 16 * 10) + (val % 16) );
}

//----routine lecture et ecriture date and time-----
void ecritureDateTime() {
  RTC.write(tm);
  if (incrementation == menuHeure) {
    mydisp.drawStr(0, 1, "   ");
    displayTime(); // affichage de l'heure
  }
  if (incrementation == menuDate) {
    mydisp.drawStr(0, 1, " ");
    displayDate(); // affichage de la date
  }
  mydisp.drawStr(decalage, 1, ""); // curseur position : decalage, ligne 1
}
