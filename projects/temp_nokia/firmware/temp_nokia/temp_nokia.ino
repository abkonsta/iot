#include <OneWire.h>
#include <SPI.h>
#include <stdlib.h>

#include "LCD_Functions.h"

int DS18S20_Pin = 3; // DS18S20 Signal pin

// Temperature chip i/o
OneWire ds(DS18S20_Pin);
char buffer[32];
char temp[10];

// sampling interval timer
unsigned long sampleTimer;

// trend timer (long, around 1 minute?)
unsigned long trendTimer;

float tempNow, tempJustBefore, tempLongAgo;

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
	trendTimer = sampleTimer = millis();
	tempNow = tempJustBefore = tempLongAgo = 0.0F;
	hasTempHist = false;
}

void loop() {
	
	long currentTime = millis();

	// if enough time has passed, get the temperature and update the LCD
	if(currentTime - sampleTimer > 1500UL)
	{
		tempJustBefore = tempNow;
		tempNow = getTemp();
	
		if(!hasTempHist)
		{
			tempJustBefore = tempNow;
			tempLongAgo = tempNow;
			hasTempHist = true;
		}
		
	  if(tempJustBefore > tempNow)
			sprintf(trend, "falling");
	  else if(tempJustBefore < tempNow)
			sprintf(trend, "rising");
	  else if(tempLongAgo > tempNow)
			sprintf(trend, "falling");
	  else if(tempLongAgo < tempNow)
			sprintf(trend, "rising");
		else
			sprintf(trend, "steady");	
	
		clearDisplay(WHITE);
    setStr("Temperature:", 0, 0, BLACK);
		dtostrf(tempNow, 3, 1, temp);
  	sprintf(buffer, "%s (%s)", temp, trend);
    setStr(buffer, 0, 11, BLACK);

    setStr("Uptime: ", 0, 22, BLACK);
		sprintf(buffer, "%d sec", millis() / 1000);
    setStr(buffer, 0, 33, BLACK);
   
    updateDisplay();
    Serial.println(tempNow);

		sampleTimer = currentTime;
	}

	// if 1 minute passed, update the trend temp
	if(currentTime - trendTimer > 60000UL)
	{
		tempLongAgo = tempNow;
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
