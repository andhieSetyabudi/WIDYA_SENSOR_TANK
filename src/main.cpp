
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
  Serial.println("hallo");
  delay(1000);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(1000);
  digitalWrite(LED_BUILTIN, LOW);
  // put your main code here, to run repeatedly:
}