/** HorlogeDS3232.h
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
    HorlogeDS3232(const byte adresseMemoireI2C = 0x57, const byte m_rtcINT = 5, const boolean debug = false ); // constructeur avec debug
    ~HorlogeDS3232(); // destructeur

    void init();// initialisation
    bool testPresenceCarteRTC(); //verification de la presence de la carte RTC / memoire 24C32
    byte decToBcd(byte val); //routine decToBcd : Convert normal decimal numbers to binary coded decimal
    byte bcdToDec(byte val);//routine bcdToDec : Convert binary coded decimal to normal decimal numbers
    void i2c_eeprom_write_byte(  unsigned int eeaddress, byte data );//ecriture dans l'eeprom at24c32 de la carte rtc
    byte i2c_eeprom_read_byte(  unsigned int eeaddress );//lecture de l'eeprom at24c32 de la carte rtc
    byte lectureRegistreEtConversion (byte adresse, byte operationAND = 0xFF);//lecture registre et conversion
    byte reglageHeure(const byte touche, byte tmDateTime, const byte type);// reglage date time
    void testInterruptRTC (volatile bool &interruptRTC); //test IT RTC
    void reglageAlarme( const byte touche, const byte alarme, const byte type); //reglage de l'alarme
    // sauvegarde dans l'eeprom I2C le choix de la lumiere ou de la valeur de fin de course
    void sauvEepromChoix ( unsigned int valeurChoix, const bool matinSoirOuvFerm, const bool lumiereFinDeCourse);
    float calculTemperature (const bool typeTemperature);//valeur de la temperature en fonction du type
    //ajout manuel : bool choixTypeOuvertureFermeture(bool choixOuvertureFermeture, const byte alarme); // mise à jour du choix du type d'ouverture / fermeture
    byte choixTypeOuvertureFermeture(byte choixOuvertureFermeture, const byte alarme); // mise à jour du choix du type d'ouverture / fermeture
    byte get_m_alarm1Hour();//accesseur - getter
    byte get_m_alarm1Minute();//accesseur - getter
    byte get_m_alarm1Second();//accesseur - getter
    byte get_m_alarm2Hour();//accesseur - getter
    byte get_m_alarm2Minute();//accesseur - getter


  protected:


    void lectureHoraireALARM1();//lecture horaire ALARM1
    void lectureHoraireALARM2();//lecture horaire ALARM2

    const boolean m_debug ; // pour affichage console si nécessaire
    const int m_deviceAddress ; // adresse du boitier mémoire 24C32 I2C = 0x57
    const byte m_rtcINT ; // digital pin D5 as l'interruption du rtc ( alarme)
    byte m_alarm1Hour ; // alarme 1 hours
    byte m_alarm1Minute ; // alarme 1 minutes
    byte m_alarm1Second ; // alarme 1 seconds
    byte m_alarm2Hour ; // alarme 2 hours
    byte m_alarm2Minute ; // alarme 2 minutes
   

};

#endif

