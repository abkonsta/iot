#include <OneWire.h>
#include <SPI.h>
#include <stdlib.h>

#include "LCD_Functions.h"

int DS18S20_Pin = 3; // DS18S20 Signal pin

// Temperature chip i/o
OneWire ds(DS18S20_Pin);
char buffer[10];
int sample;
unsigned long timer;
float temp, lastTemp1, lastTemp2;
char trend[10];
bool hasTempHist;
    
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  
  lcdBegin();       // This will setup our pins, and initialize the LCD
  updateDisplay();  // with displayMap untouched, SFE logo
  setContrast(50);  // Good values range from 40-60
  delay(500);       // had to update to 50, was too faint
  
  clearDisplay(WHITE);
  updateDisplay();
	
	// initialize globals
	timer = millis();
	sample = 1;
	lastTemp1 = -100.0F;
	lastTemp2 = -100.0F;
	temp = 0.0F;
	hasTempHist = false;
}

void loop() {
	
	if(millis() - timer > 1500UL)
	{
		clearDisplay(WHITE);

		// if we don't have any temperature history, 
		// do not print the trend
		hasTempHist = (lastTemp1 != -100.0F && lastTemp2 != -100.0F);
    
		// shift temps
		lastTemp2 = lastTemp1;
		lastTemp1 = temp;
		temp = getTemp();
	
	  if(!hasTempHist)
				strcpy(trend, "");
		else if(lastTemp2 == lastTemp1 && lastTemp1 == temp)
				strcpy(trend, "(steady)");
		else if(lastTemp2 > lastTemp1 && lastTemp1 > temp)
				strcpy(trend, "(falling)");
		else if(lastTemp2 < lastTemp1 && lastTemp1 < temp)
				strcpy(trend, "(rising)");
			
    setStr("Temperature:", 0, 0, BLACK);
    dtostrf(temp, 3, 1, buffer);
    setStr(buffer, 0, 11, BLACK);
    setStr(trend, 30, 11, BLACK);

    setStr("Sample: ", 0, 22, BLACK);
    itoa(sample, buffer, 10);
    setStr(buffer, 0, 33, BLACK);
   
    updateDisplay();
    Serial.println(temp);

		timer = millis();
    sample++;
	}
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
