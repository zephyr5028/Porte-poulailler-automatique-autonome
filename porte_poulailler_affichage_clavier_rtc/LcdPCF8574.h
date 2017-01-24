/* LcdPCF8574.h
  prototype de la classe LCD circuit PCF8574 I2C
  afficheur lcd 2*16 caractères  avec liaison serie i2c LiquidCrystal_I2C
*/

#ifndef LcdPCF8574_h
#define LcdPCF8574_h

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "Arduino.h"

class LcdPCF8574  : public LiquidCrystal_I2C
{
  public:

    // constructeur  avec debug
    // I2C:Arduino UNO: SDA (data line) is on analog input pin 4, and SCL (clock line) is on analog input pin 5 on UNO and Duemilanove
    LcdPCF8574 ( char , const byte taille = 16, const byte nbLigne = 2,  boolean debug = false);

    ~LcdPCF8574(); // destructeur



  protected:

    const boolean m_debug ; // pour affichage console si nécessaire
    //  const byte m_taille; // taille du tableau - 16 caractères
    byte m_decalage; // position du curseur
    //  const byte m_nbLigne; // nombre de lignes

};
#endif


