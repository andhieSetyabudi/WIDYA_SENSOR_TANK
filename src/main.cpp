
#include <Arduino.h>

#include <EEPROM.h>
#include <SPI.h>

#include "bsp.h"
#include "com.h"

void setup() {
  pinMode(LED_BUILTIN,OUTPUT);
  Serial.begin(9600);

// initializing basic board support package
  if(BSP::initialize()==DEV_OK)
    Serial.println(F("load info success!!"));
  else
    Serial.println(F("load info error!!"));
  BSP::print_device_info();

// initializing communication package
  com::initialize();

}

void loop() {
  static uint64_t mils = millis();
  static bool state=false;
  
  BSP::loop();
  if( (millis() - mils) >= 1000)
  {
    com::loop();
    Serial.println("hallo");
    state = !state;
    digitalWrite(LED_BUILTIN, state); 
    mils = millis(); 
  }
}