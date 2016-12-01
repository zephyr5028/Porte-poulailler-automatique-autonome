/* I2C */

/*----------NOTE----------
  new version of lib will save you compiled code size for the sample
  The size for 2 version(@Arduino 1.0.1):
      UART  I2C    SPI
  OLD   8998  8988   9132
  NEW   6966  7566   6354
  ------------------------*/
  
/*------all available are:_Digole_Serial_UART_, _Digole_Serial_I2C_ and _Digole_Serial_SPI_ ------*/

#define _Digole_Serial_I2C_  // To tell compiler compile the special communication only, 
#include <DigoleSerial.h> // bibliotheque afficheur serie

//------UART setup----
#if defined(_Digole_Serial_UART_)
DigoleSerialDisp mydisp(&Serial, 9600); // UART:Arduino UNO: Pin 1(TX)on arduino to RX on module
#endif

//------I2C setup----
#if defined(_Digole_Serial_I2C_)
#include <Wire.h>
DigoleSerialDisp mydisp(&Wire, '\x27'); // I2C:Arduino UNO: SDA (data line) is on analog input pin 4, and SCL (clock line) is on analog input pin 5 on UNO and Duemilanove
#endif

//------SPI setup----
#if defined(_Digole_Serial_SPI_)
DigoleSerialDisp mydisp(8, 9, 10); // SPI:Pin 8:data, 9:clock, 10:SS, you can assign 255 to SS, and hard ground SS pin on module
#endif
