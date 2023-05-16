#include "commands.hpp"
#include <Arduino.h>

#define MONITOR_SPEED 115220
#define MAX_COMMANDS 10

static struct s_command *command_table[MAX_COMMANDS];


/**
 * 
*/
char *read()
{

    return "";
}

void send() 
{

}

void print()
{
    Serial.println("[test]\ttest");
}