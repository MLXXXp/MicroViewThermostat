// MicroView temperature display and thermostat using the
// DS18S20 or DS18B20 1-Wire digital thermometer IC

// Based on code from http://bildr.org/2011/07/ds18b20-arduino/

// Original license:
/*
Copyright (c) 2010 bildr community

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

// NOTE: The font used to display the temperature doesn't include a
//      minus sign. Only positive temperatures will display properly
//      unless a different font is used.

#include <MicroView.h>
#include <OneWire.h>

// Pin states for heaters
#define HEAT_ON LOW
#define HEAT_OFF HIGH

// Comment out the following line for Celsius 
#define FAHRENHEIT_SCALE

// Turn on heat when the temperature goes below this value
const float HeatOnTemp = 77;

// Turn off heat when the temperature goes above this value
const float HeatOffTemp = 79;

// Alert if the temperature goes below this value
const float AlertLowTemp = 75;

// Alert if the temperature goes above this value
const float AlertHighTemp = 81;

// Pin assignments
const int DS18S20_Pin = 2; // DS18S20 Signal pin
const int Heater1Pin = 3;  // Heater 1 control
const int Heater2Pin = 5;  // Heater 2 control


boolean heatOn = false; // heat is on when true

OneWire ds(DS18S20_Pin); // Temperature chip i/o

void setup() {
  uView.begin();

  // Set heater pins to outputs and turn them off
  digitalWrite(Heater1Pin, HEAT_OFF);
  pinMode(Heater1Pin, OUTPUT);
  digitalWrite(Heater2Pin, HEAT_OFF);
  pinMode(Heater2Pin, OUTPUT);
}

void loop() {
  float temperature;

  boolean screenFlash = false; // flash the display if set to true

  uView.clear(PAGE);

  temperature = getTemp();

  // A value of -1000 indicates an error.
  // Turn off the heaters and abort the loop.
  if (temperature == -1000) {
    setHeat(HEAT_OFF);
    heatOn = false;
    return;
  }

#ifdef FAHRENHEIT_SCALE
  temperature = temperature * 9/5 + 32;
#endif

  // Display the temperature
  uView.setFontType(2); // 10 X 16 "seven segment" numeric font
  uView.setCursor(8, 13);
  uView.print(temperature, 1); // display with one decimal place

  // Set heaters on or off if required
  if (temperature < HeatOnTemp) {
    heatOn = true;
  }
  else if (temperature > HeatOffTemp) {
    heatOn = false;
  }

  heatControl(heatOn);

  // Set an alert if temperature goes above or below the thresholds
  if (temperature > AlertHighTemp) {
    heatAlert(HIGH);
    screenFlash = true;
  }
  else if (temperature < AlertLowTemp) {
    heatAlert(LOW);
    screenFlash = true;
  }
    
  uView.display(); // Update the screen

  if (screenFlash) { // if necessary, invert the display to cause flashing
    uView.invert(true);
    delay(700);
    uView.invert(false);
    delay (1300);
  }
  else {
    delay(2000);
  }
}

// Set the heat on or off as required and display the current state
void heatControl(boolean on) {

  uView.setFontType(0);
  uView.setCursor(9, 38);
  uView.print("Heat ");

  if (on) {
    uView.print("ON");
    setHeat(HEAT_ON);
  }
  else {
    uView.print("OFF");
    setHeat(HEAT_OFF);
  }
}

// Display a heat alert message
void heatAlert(uint8_t state) {
  uView.setFontType(0);

  if (state == HIGH) {
    uView.setCursor(6, 2);
    uView.print("TOO HIGH!");
  }
  else {
    uView.setCursor(9, 2);
    uView.print("TOO LOW!");
  }
}

// Set the heaters on or off
void setHeat(uint8_t pinState) {
  digitalWrite(Heater1Pin, pinState);
  digitalWrite(Heater2Pin, pinState);
}

// Display a message for a while and then clear the display for
// as short time so a repeated message will flash
void displayMessage(const char *message) {
  uView.setFontType(0); // 5 X 7 font
  uView.setCursor(0,7);
  uView.print(message);
  uView.display();
  delay(2000);
  uView.clear(PAGE);
  uView.display();
  delay(200);
}

// Returns the temperature from one DS18S20 in DEG Celsius
float getTemp() {
  byte data[12];
  byte addr[8];

  if ( !ds.search(addr)) {
    //no more sensors on chain, reset search
    ds.reset_search();
    displayMessage("No sensors");
    return -1000;
  }

  if ( OneWire::crc8( addr, 7) != addr[7]) {
    displayMessage("CRC is\nnot valid!");
    return -1000;
  }

  if ( addr[0] != 0x10 && addr[0] != 0x28) {
    displayMessage("Device notrecognized");
    return -1000;
  }

  ds.reset();
  ds.select(addr);
  ds.write(0x44,1); // start conversion, with parasite power on at the end

  delay(750); // Wait for temperature conversion to complete

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

