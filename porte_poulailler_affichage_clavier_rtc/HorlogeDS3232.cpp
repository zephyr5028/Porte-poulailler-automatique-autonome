/* HorlogeDS3232.h
  définitions de la classe HorlogeDS3232
  RTC DS3232 avec temperature et circuit mémoire
*/

#include "HorlogeDS3232.h"

HorlogeDS3232::HorlogeDS3232() : DS3232RTC(), m_deviceAddress(0x57), m_debug(false)
{

}
/* sucharge du constructeur avec le nombre de lignes du menu */
HorlogeDS3232::HorlogeDS3232 ( const int adresseMemoireI2C, const boolean debug) : DS3232RTC(),
  m_deviceAddress(adresseMemoireI2C), m_debug(debug)
{

}

HorlogeDS3232::~HorlogeDS3232() {

}

//-----initialisation-----
void HorlogeDS3232::init() {
  //Wire.begin();
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

