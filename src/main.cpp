#include <Arduino.h>
#include "commands.hpp"
#include "memory.hpp"


#define MONITOR_SPEED 115200


int num = 9;


void setup() 
{
  Serial.begin(MONITOR_SPEED);
  memInit();
}


void loop() 
{
  // put your main code here, to run repeatedly:
  readInput();
}
