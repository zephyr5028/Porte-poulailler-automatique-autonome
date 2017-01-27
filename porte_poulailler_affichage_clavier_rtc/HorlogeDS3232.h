/* HorlogeDS3232.h
  prototype de la classe HorlogeDS3232
  RTC DS3232 avec temperature et circuit mémoire
*/

#ifndef HorlogeDS3232_h
#define HorlogeDS3232_h

#include <Wire.h>
#include <DS3232RTC.h>    //http://github.com/JChristensen/DS3232RTC
#include <Time.h>         //http://www.arduino.cc/playground/Code/Time  
#define DS3231_I2C_ADDRESS 0x68
#include "Arduino.h"

class HorlogeDS3232 : public DS3232RTC
{

  public:

    // constructeur
    HorlogeDS3232();
    HorlogeDS3232(const int adresseMemoireI2C = 0x57, const boolean debug = false ); // constructeur avec debug
    ~HorlogeDS3232(); // destructeur

    void init();// initialisation
    byte decToBcd(byte val); //routine decToBcd : Convert normal decimal numbers to binary coded decimal
    byte bcdToDec(byte val);//routine bcdToDec : Convert binary coded decimal to normal decimal numbers
    void i2c_eeprom_write_byte(  unsigned int eeaddress, byte data );//ecriture dans l'eeprom at24c32 de la carte rtc
    byte i2c_eeprom_read_byte(  unsigned int eeaddress );//lecture de l'eeprom at24c32 de la carte rtc
    byte lectureRegistreEtConversion (byte adresse, byte operationAND = 0xFF);//lecture registre et conversion
    /*
     *   static byte read(tmElements_t &tm);
        byte write(tmElements_t &tm);
     */


  protected:

    const boolean m_debug ; // pour affichage console si nécessaire
    const int m_deviceAddress ; // adresse du boitier mémoire 24C32 I2C = 0x57


};

#endif

