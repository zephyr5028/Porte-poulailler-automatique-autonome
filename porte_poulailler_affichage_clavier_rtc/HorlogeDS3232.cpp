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

/*----------------------------------------------------------------------*
 * Reads the current time from the RTC and returns it in a tmElements_t *
 * structure. Returns the I2C status (zero if successful).              *
 *----------------------------------------------------------------------*/
 /*
byte DS3232RTC::read(tmElements_t &tm)
{
    i2cBeginTransmission(RTC_ADDR);
    i2cWrite((uint8_t)RTC_SECONDS);
    if ( byte e = i2cEndTransmission() ) return e;
    //request 7 bytes (secs, min, hr, dow, date, mth, yr)
    i2cRequestFrom(RTC_ADDR, tmNbrFields);
    tm.Second = bcd2dec(i2cRead() & ~_BV(DS1307_CH));   
    tm.Minute = bcd2dec(i2cRead());
    tm.Hour = bcd2dec(i2cRead() & ~_BV(HR1224));    //assumes 24hr clock
    tm.Wday = i2cRead();
    tm.Day = bcd2dec(i2cRead());
    tm.Month = bcd2dec(i2cRead() & ~_BV(CENTURY));  //don't use the Century bit
    tm.Year = y2kYearToTm(bcd2dec(i2cRead()));
    return 0;
}
*/

/*----------------------------------------------------------------------*
 * Sets the RTC's time from a tmElements_t structure and clears the     *
 * oscillator stop flag (OSF) in the Control/Status register.           *
 * Returns the I2C status (zero if successful).                         *
 *----------------------------------------------------------------------*/
 /*
byte DS3232RTC::write(tmElements_t &tm)
{
    i2cBeginTransmission(RTC_ADDR);
    i2cWrite((uint8_t)RTC_SECONDS);
    i2cWrite(dec2bcd(tm.Second));
    i2cWrite(dec2bcd(tm.Minute));
    i2cWrite(dec2bcd(tm.Hour));         //sets 24 hour format (Bit 6 == 0)
    i2cWrite(tm.Wday);
    i2cWrite(dec2bcd(tm.Day));
    i2cWrite(dec2bcd(tm.Month));
    i2cWrite(dec2bcd(tmYearToY2k(tm.Year))); 
    byte ret = i2cEndTransmission();
    uint8_t s = readRTC(RTC_STATUS);        //read the status register
    writeRTC( RTC_STATUS, s & ~_BV(OSF) );  //clear the Oscillator Stop Flag
    return ret;
}
*/

/*
//-----reglage de la lumiere du matin ou du soir-----
unsigned int  Lumiere::reglageLumiere (bool matinSoir, byte touche) {
  if (touche == 2 or touche == 3) {
    unsigned int lumiere;
    if (matinSoir) lumiere = m_lumMatin; else lumiere = m_lumSoir;
    if (touche == 2) {
      if (lumiere < m_lumiereMax) {
        lumiere += m_incrementation; //incrementation de la lumiere
      } else {
        lumiere = 0;
      }
    } else {
      if (lumiere > 0) {
        lumiere -= m_incrementation; //decrementation de la lumiere
      } else {
        lumiere =  m_lumiereMax;
      }
    }
    if (matinSoir) m_lumMatin = lumiere; else m_lumSoir = lumiere;
    return lumiere;
  }
  if (matinSoir) return m_lumMatin ; else return m_lumSoir ;
}*/


