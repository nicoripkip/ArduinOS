#include <Arduino.h>
#include "commands.hpp"
#include "memory.hpp"
#include "filesystem.hpp"
#include "scheduler.hpp"


#define MONITOR_SPEED 115200


int num = 9;


void setup() 
{
  Serial.begin(MONITOR_SPEED);

  initFileSystem();
  initScheduler();
}


void loop() 
{
  // put your main code here, to run repeatedly
  readInput();
  runTasks();

  Serial.flush();
}
