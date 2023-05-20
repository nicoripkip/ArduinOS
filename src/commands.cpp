#include "commands.hpp"
#include <Arduino.h>
#include "memory.hpp"


#define MONITOR_SPEED 115220

static struct s_command *command_table[MAX_COMMANDS];
static String input;
static bool booted = false;


/**
 * Function to parse the args to the command function
 * 
 * @param commandName
 * @param argArray
*/
void parseInput(String &commandName, String *argArray, int argCount)
{
    int i;
    for (i = 0; i < MAX_COMMANDS; i++) {
        commandName.trim();
        if (commands[i].command == commandName) {
            commands[i].com_func(argArray, argCount);
            return;
        } 
    }

    Serial.println("[error]\tCommand does not exists!");
}


/**
 * Function to split a string by a delimiter to an array
 * 
 * @param input
 * @param delimiter
 * @param argArray
 * @return int
*/
int splitInput(String &input, String delimiter, String *argArray)
{
    int currentIndex = 0;
    int lastIndex = 0;
    int ap = 0;

    while (currentIndex < (input.length()-1)) {
        currentIndex = input.indexOf(delimiter, lastIndex);

        if (currentIndex == -1) {
            currentIndex = input.length();
            argArray[ap] = input.substring(lastIndex, currentIndex);
            break;
        } else {
            argArray[ap++] = input.substring(lastIndex, currentIndex);
            lastIndex = ++currentIndex;
        }
    }

    return ap+1;
}


/**
 * Function to read the input from the serial monitor
*/
void readInput()
{
    if (booted == false) {
        Serial.print("[atmega2560]~: ");
    }

    booted = true;

    if (Serial.available() != 0) {
        char n = Serial.read();
        Serial.print(n);
        input+=n;

        if (input.endsWith("\n") || input.endsWith("\r")) {
            Serial.println();
            for (int i = 0; i < 1; i++) {
                Serial.print("[info]\tCommando: ");
                Serial.println(input);

                String argArray[10] = {};
                int argCount = splitInput(input, " ", argArray);
                parseInput(argArray[0], argArray, argCount);
            }

            input="";
            Serial.print("[atmega2560]~: ");
        } 
    }
}


/**
 * Function to send output back to the serial monitor
*/
void sendOutput() 
{

}


/**
 * Function to print some random text
*/
void print(String *arg, int argCount)
{
    int i;

    for (i = 0; i < argCount; i++) {
        if (arg[i] == "-m") {
            Serial.print("[info]\t");
            Serial.println(arg[++i]);
        }
    }
}


void listmem(String *arg, int argCount)
{
    showMemTable();
}