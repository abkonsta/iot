#include <OneWire.h>
#include <SPI.h>
#include <stdlib.h>

#include "LCD_Functions.h"

int DS18S20_Pin = 3; //DS18S20 Signal pin on digital 2

//Temperature chip i/o
OneWire ds(DS18S20_Pin);  // on digital pin 2
char buffer[10];
int sample = 1;
    
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  
  lcdBegin(); // This will setup our pins, and initialize the LCD
  updateDisplay(); // with displayMap untouched, SFE logo
  setContrast(50); // Good values range from 40-60
                    // had to update to 50, was too faint
  delay(1000);   
  clearDisplay(WHITE);
  updateDisplay();
  
}

void loop() {
    clearDisplay(WHITE);

    float temperature = getTemp();

    
    setStr("Temperature:", 0, 0, BLACK);
    dtostrf(temperature, 3, 1, buffer);
    setStr(buffer, 0, 11, BLACK);

    setStr("Sample: ", 0, 22, BLACK);
    itoa(sample, buffer, 10);
    setStr(buffer, 0, 33, BLACK);
    
   
    updateDisplay();
    Serial.println(temperature);
    
    delay(1000); //just here to slow down the output so it is easier to read
    sample++;
}

float getTemp(){
  //returns the temperature from one DS18S20 in DEG Celsius

  byte data[12];
  byte addr[8];

  if ( !ds.search(addr)) {
      //no more sensors on chain, reset search
      ds.reset_search();
      return -1000;
  }

  if ( OneWire::crc8( addr, 7) != addr[7]) {
      Serial.println("CRC is not valid!");
      return -1000;
  }

  if ( addr[0] != 0x10 && addr[0] != 0x28) {
      Serial.print("Device is not recognized");
      return -1000;
  }

  ds.reset();
  ds.select(addr);
  ds.write(0x44,1); // start conversion, with parasite power on at the end

  byte present = ds.reset();
  ds.select(addr);    
  ds.write(0xBE); // Read Scratchpad

  
  for (int i = 0; i < 9; i++) { // we need 9 bytes
    data[i] = ds.read();
  }
  
  ds.reset_search();
  
  byte MSB = data[1];
  byte LSB = data[0];

  float tempRead = ((MSB << 8) | LSB); //using two's compliment
  float TemperatureSum = tempRead / 16;
  
  return TemperatureSum;
  
}
