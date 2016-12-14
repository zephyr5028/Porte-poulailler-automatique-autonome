/* setup */

void setup() {
  Wire.begin();
  Serial.begin(9600);
  /*----------for text LCD adapter and graphic LCD adapter ------------*/
  mydisp.begin();
  mydisp.setPrintPos(0, 0); // position 0 ligne 0 pour débuter le clear screen
  mydisp.clearScreen(); // CLear screen
  mydisp.backLightOff(); // retro eclairage off
  mydisp.enableCursor(); // enable cursor
  for (uint8_t j = 0; j < 1; j++)  {  //making "Hello" string moving
    for (uint8_t i = 0; i < 9; i++)  {  //move string to right
      mydisp.setPrintPos(i, 0); // ligne 0
      mydisp.print(F(" Bonjour"));
      delay(200); //delay
    }
    for (uint8_t i = 0; i < 9; i++) {  //move string to left
      mydisp.setPrintPos(8 - i, 0);
      mydisp.print(F("Bonjour "));
      delay(200);
    }
  }
  incrementation = menuManuel; // pour affichage menu
  deroulementMenu (incrementation); // affichage du menu
  ouve = i2c_eeprom_read_byte(0x57, 0x14); // lecture du type d'ouverture @14  de l'eeprom de la carte rtc (i2c @ 0x57)
  delay(10);
  ferm = i2c_eeprom_read_byte(0x57, 0x15); // lecture du type de fermeture @15   de l'eeprom de la carte rtc (i2c @ 0x57)
  delay(10);
  byte val1 = i2c_eeprom_read_byte(0x57, 0x16); // lecture pf lumière du matin (byte)
  delay(10);
  byte val2 = i2c_eeprom_read_byte(0x57, 0x17); // lecture Pf lumiere du matin (byte)
  delay(10);
  lumMatin = (val2 << 8) + val1;  // mots 2 byte vers mot int lumMatin
  val1 = i2c_eeprom_read_byte(0x57, 0x18); // lecture pf lumière du soir (byte)
  delay(10);
  val2 = i2c_eeprom_read_byte(0x57, 0x19); // lecture Pf lumiere du soir (byte)
  delay(10);
  lumSoir = (val2 << 8) + val1;  // mots 2 byte vers mot int lumMatin
  val1 = i2c_eeprom_read_byte(0x57, 0x20); // lecture pf fin de course haut (byte)
  delay(10);
  val2 = i2c_eeprom_read_byte(0x57, 0x21); // lecture Pf fin de course haut (byte)
  delay(10);
  finDeCourseH = (val2 << 8) + val1;  // mots 2 byte vers mot int finDeCourseH
  val1 = i2c_eeprom_read_byte(0x57, 0x22); // lecture pf fin de course bas (byte)
  delay(10);
  val2 = i2c_eeprom_read_byte(0x57, 0x23); // lecture Pf fin de course bas (byte)
  delay(10);
  finDeCourseB = (val2 << 8) + val1;  // mots 2 byte vers mot int finDeCourseB
  // roue codeuse
  pinMode(roueCodeuse, INPUT); // make the roueCodeuse's pin 7 an input
  // servo
  pinMode(servoPin, OUTPUT); // relais servo's pin 4 an OUTPUT
  digitalWrite(servoPin, LOW); // mise hors tension du relais du servo
  monServo.attach(servoCde); // use digital pin D8 for commande du servo
  // on démarre à une valeur censée être la moitié de l'excursion totale de l'angle réalisé par le servomoteur
  monServo.write(pulse);  // value should usually be 750 to 2200 (environ 1500 = stop)
  attachInterrupt(1, myInterruptINT1, FALLING); // validation de l'interruption sur int1 (d3)
  attachInterrupt(0, myInterruptINT0, CHANGE); // validation de l'interruption sur int0 (d2)
  //RTC.writeRTC(0x0E,0x06); // registre control rtc
  RTC.writeRTC(0x0F, 0x00); // registre status rtc
  tempoDebounce = debounce; // initialisation de la temporisation pour le debounce d'un Bp
  //Optimisation de la consommation
  //power_adc_disable(); // Convertisseur Analog / Digital pour les entrées analogiques
  power_spi_disable();
  //power_twi_disable();
  //Si pas besoin de communiquer par l'usb
  //power_usart0_disable();
  //Extinction des timers, attention timer0 utilisé par millis ou delay
  //power_timer0_disable();
  //power_timer1_disable();
  //power_timer2_disable();
  setup_watchdog(9); // maxi de 8 secondes
  vw_set_tx_pin(pinEmRadio); // broche d10 emetteur
  vw_setup(600); // initialisation de la bibliothèque avec la vitesse (vitesse_bps)
  servoOuverture(); // initialisation des paramétres par la mise sous tension du servo pour la montee de la porte (fonction du sens de rotation du servo)
}
