/* HorlogeDS3232.h
  définitions de la classe HorlogeDS3232
  RTC DS3232 avec temperature et circuit mémoire
*/

#include "HorlogeDS3232.h"

HorlogeDS3232::HorlogeDS3232() : DS3232RTC(), m_deviceAddress(0x57), m_rtcINT(5), m_debug(false),
  m_alarm1Hour(0), m_alarm1Minute(0), m_alarm1Second(0), m_alarm2Hour(0), m_alarm2Minute(0)
{

}

/* sucharge du constructeur avec le nombre de lignes du menu */
HorlogeDS3232::HorlogeDS3232 ( const byte adresseMemoireI2C, const byte rtcINT, const boolean debug) : DS3232RTC(),
  m_deviceAddress(adresseMemoireI2C), m_rtcINT(rtcINT), m_debug(debug),
  m_alarm1Hour(0), m_alarm1Minute(0), m_alarm1Second(0), m_alarm2Hour(0), m_alarm2Minute(0)
{

}

HorlogeDS3232::~HorlogeDS3232() {

}

//-----initialisation-----
void HorlogeDS3232::init() {
  //Wire.begin();
  //RTC.writeRTC(0x0E,0x06); // registre control rtc
  RTC.writeRTC(0x0F, 0x00); // registre status rtc
  lectureHoraireALARM1();
  lectureHoraireALARM2();
}

//-----test IT RTC-----
void HorlogeDS3232::testInterruptRTC (volatile bool &interruptRTC) {
  if (!digitalRead(m_rtcINT)) { //pin 5 interruption RTC
    interruptRTC = true;
  }
}

//-----routine decToBcd : Convert normal decimal numbers to binary coded decimal-----
byte HorlogeDS3232::decToBcd(byte val) {
  return ( (val / 10 * 16) + (val % 10) );
}

//-----routine bcdToDec : Convert binary coded decimal to normal decimal numbers-----
byte HorlogeDS3232::bcdToDec(byte val) {
  return ( (val / 16 * 10) + (val % 16) );
}

/* eeprom at24c32 */
//-----ecriture dans l'eeprom at24c32 de la carte rtc------
void HorlogeDS3232::i2c_eeprom_write_byte(  unsigned int eeaddress, byte data ) {
  int rdata = data;
  Wire.beginTransmission(m_deviceAddress);   // adresse 0x57 pour l'i2c de l'eeprom de la carte rtc
  Wire.write((int)(eeaddress >> 8)); // MSB
  Wire.write((int)(eeaddress & 0xFF)); // LSB
  Wire.write(rdata);
  Wire.endTransmission();
}

//-----lecture de l'eeprom at24c32 de la carte rtc------
byte HorlogeDS3232::i2c_eeprom_read_byte(  unsigned int eeaddress ) {
  byte rdata = 0xFF;
  Wire.beginTransmission(m_deviceAddress); // adresse 0x57 pour l'i2c de l'eeprom de la carte rtc
  Wire.write((int)(eeaddress >> 8)); // MSB
  Wire.write((int)(eeaddress & 0xFF)); // LSB
  Wire.endTransmission();
  Wire.requestFrom(m_deviceAddress, 1);
  if (Wire.available()) rdata = Wire.read();
  return rdata;
}

//------lecture registre et conversion-----
byte HorlogeDS3232::lectureRegistreEtConversion (byte adresse, byte operationAND) {
  return bcdToDec(RTC.readRTC(adresse) & operationAND);
}

//-----reglaged date time-----
byte HorlogeDS3232::reglageHeure(const byte touche, byte tmDateTime, const byte type) {
  byte haut(0), bas(0);
  switch (type) { // test de type date time
    case 1: // jour de la semaine
      haut = 7; bas = 1;
      break;
    case 2: // jour
      haut = 31; bas = 1;
      break;
    case 3 : // mois
      haut = 12; bas = 1;
      break;
    case 4: // annee
      haut = 130; bas = 30;
      break;
    case 5: // heure
      haut = 24; bas = 0;
      break;
    case 6: // minute
      haut = 59; bas = 0;
      break;
    case 7: // secondes
      haut = 59; bas = 0;
      break;
  }
  if (touche == 2) {
    if (tmDateTime < haut) tmDateTime++;  else tmDateTime = bas;
  }
  if (touche == 3 ) {
    if (tmDateTime > bas) tmDateTime--; else tmDateTime = haut;
  }
  return (tmDateTime);
}

//-----lecture horaire ALARM1-----
void HorlogeDS3232::lectureHoraireALARM1() {
  m_alarm1Hour =  lectureRegistreEtConversion(ALM1_HOURS & 0x3f); // alarme 1 hours
  m_alarm1Minute = lectureRegistreEtConversion(ALM1_MINUTES); // alarme 1 minutes
  m_alarm1Second =  lectureRegistreEtConversion(ALM1_SECONDS & 0x7f); // alarme 1 seconds
}

//-----lecture horaire ALARM2-----
void HorlogeDS3232::lectureHoraireALARM2() {
  m_alarm2Hour =  lectureRegistreEtConversion(ALM2_HOURS & 0x3f); // alarme 2 hours
  m_alarm2Minute = lectureRegistreEtConversion(ALM2_MINUTES); // alarme 2 minutes
}

//-----reglage de l'alarme-----
void HorlogeDS3232::reglageAlarme( const byte touche, const byte alarme, const byte type) {
  if (alarme == 1) {
    switch (type) { // test de type date time
      case 5: // heure
        m_alarm1Hour = reglageHeure(touche, m_alarm1Hour, type);
        break;
      case 6: // minute
        m_alarm1Minute = reglageHeure(touche, m_alarm1Minute, type);
        break;
      case 7: // secondes
        m_alarm1Second = reglageHeure(touche, m_alarm1Second, type);
        break;
    }
    RTC.setAlarm(ALM1_MATCH_HOURS, m_alarm1Second, m_alarm1Minute, m_alarm1Hour, 0); // écriture alarm 1
  }
  if (alarme == 2) {
    switch (type) { // test de type date time
      case 5: // heure
        m_alarm2Hour = reglageHeure(touche, m_alarm2Hour, type);
        break;
      case 6: // minute
        m_alarm2Minute = reglageHeure(touche, m_alarm2Minute, type);
        break;
    }
    RTC.setAlarm(ALM2_MATCH_HOURS, m_alarm2Minute, m_alarm2Hour, 0);  // écriture de l'heure alarme 2
  }
}

//-----sauvegarde dans l'eeprom I2C le choix de la lumiere ou de la valeur de fin de course-----
void HorlogeDS3232::sauvEepromChoix ( unsigned int valeurChoix, const bool matinSoirOuvFerm, const bool lumiereFinDeCourse) {
  byte val1 = valeurChoix & 0xFF; // pf
  byte val2 = (valeurChoix >> 8) & 0xFF; //  PF
  byte adresseMemoire(0);
  if (lumiereFinDeCourse) {
    if (matinSoirOuvFerm) adresseMemoire = 0x16; else adresseMemoire = 0x18; // début adresses memoire de sauvegarde
  } else {
    if (matinSoirOuvFerm) adresseMemoire = 0x22; else adresseMemoire = 0x20; // début adresses memoire de sauvegarde
  }
  i2c_eeprom_write_byte( adresseMemoire, val1); // écriture de la valeur du reglage de la lumiere ( low )  dans l'eeprom de la carte rtc (i2c @ 0x57)
  delay(10);
  i2c_eeprom_write_byte( adresseMemoire + 1, val2); // écriture de la valeur du reglage de la lumiere ( high )dans l'eeprom de la carte rtc (i2c @ 0x57)
  delay(10);
}

//------valeur de la temperature en fonction du type------
float HorlogeDS3232::calculTemperature (const bool typeTemperature) {
  int t = RTC.temperature();
  float celsius = t / 4.0;
  if (typeTemperature) {
    return celsius;
  } else {
    float fahrenheit = celsius * 9.0 / 5.0 + 32.0;
    return fahrenheit;
  }
}

//-----mise à jour du choix du type d'ouverture / fermeture-----
bool HorlogeDS3232::choixTypeOuvertureFermeture(bool choixOuvertureFermeture, const byte alarme) {
  byte adresse(0);
  if (alarme == 1)  adresse = 0x14; else adresse = 0x15; // 0x14 alarm1 / 0x15 alarm2
  if (choixOuvertureFermeture)  choixOuvertureFermeture = false; else choixOuvertureFermeture = true; // o lumiere, 1 horloge
  RTC.alarmInterrupt(alarme, choixOuvertureFermeture); // alarme : activation 1 / desactivation 0 
  i2c_eeprom_write_byte( adresse, choixOuvertureFermeture); // écriture du type d'ouverture/ fermeture @14 ou 15 de l'eeprom de la carte rtc (i2c @ 0x57)
  return choixOuvertureFermeture;
}

//-----accesseur - getter-----
byte HorlogeDS3232::get_m_alarm1Hour() {
  return m_alarm1Hour;
}

//-----accesseur - getter-----
byte HorlogeDS3232::get_m_alarm1Minute() {
  return m_alarm1Minute;
}

//-----accesseur - getter-----
byte HorlogeDS3232::get_m_alarm1Second() {
  return m_alarm1Second;
}

//-----accesseur - getter-----
byte HorlogeDS3232::get_m_alarm2Hour() {
  return m_alarm2Hour;
}

//-----accesseur - getter-----
byte HorlogeDS3232::get_m_alarm2Minute() {
  return m_alarm2Minute;
}

