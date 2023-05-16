#include <Arduino.h>
#include "commands.hpp"


#define MONITOR_SPEED 115200


int num = 9;
String t = "";


void setup() 
{
  Serial.begin(MONITOR_SPEED);
}


void loop() 
{
  // put your main code here, to run repeatedly:
  while (Serial.available() != 0) {
    char n = Serial.read();
    t+=n;

    if (t.endsWith(" ")) {
      Serial.println(t);

      for (int i = 0; i < 1; i++) {
        t.trim();
        if (commands[i].command == t) {
          commands[i].com_func();
          break;
        } else {
          Serial.println("[error]\tCommand not found!");
        }
      }

      t="";
    } 
  }
}
