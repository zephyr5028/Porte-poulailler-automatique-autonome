/* LcdPCF8574.h
    /* LcdPCF8574.h
  définitions de la classe LCD circuit PCF8574 I2C
  afficheur lcd 2*16 caractères  avec liaison serie i2c LiquidCrystal_I2C
*/

#include "LcdPCF8574.h"

//constructeur avec debug
// I2C:Arduino UNO: SDA (data line) is on analog input pin 4, and SCL (clock line) is on analog input pin 5 on UNO and Duemilanove
//LiquidCrystal_I2C lcd(0x27, 16, 2);
LcdPCF8574::LcdPCF8574 ( char, const byte taille, const byte nbLigne, const boolean debug) : LiquidCrystal_I2C (0x27, taille, nbLigne), m_debug(debug), m_decalage(0)
{

}

LcdPCF8574::~LcdPCF8574()
{
}


