#include "commands.hpp"
#include <Arduino.h>
#include "memory.hpp"


#define MONITOR_SPEED 115220


static char command_buffer[3][BUFFER_SIZE];
static size_t bp = 0;


/**
 * Function to clear the command buffer
 * 
*/
void clearBuffer()
{
    memset(command_buffer, 0, sizeof(command_buffer));
}


/**
 * Function to read the input from the serial monitor
 * 
*/
void readInput()
{
    if (Serial.available() != 0) {
        const char input = Serial.read();
        Serial.print(input);

        if (strcmp(&input, " ") == 0) {
            bp++;

            if (bp > 2) {
                Serial.println("[error]\tToo many arguments supported by the buffer!");
                bp = 0;
                clearBuffer();
                return;
            }
        } else if (strcmp(&input, "\r") == 0)  {
            for (uint16_t i = 0; i < MAX_COMMANDS; i++) {
                if (strcmp(command_buffer[0], command_table[i].command) == 0) {
                    command_table[i].com_func();
                    break;
                }

                if (i == MAX_COMMANDS-1) {
                    Serial.println("[error]\tCommand does not exist!");
                }
            }

            clearBuffer();
            bp = 0;
        } else {
            strcat(command_buffer[bp], &input);
        }

        // Serial.print(command_buffer[bp]);
    }
}


/**
 * Function to send output back to the serial monitor
*/
void sendOutput() 
{

}


void help()
{
    Serial.println("--- All available commands: ---");
}


/**
 * Function to print some random text
 * 
 * @param arg
 * @param argCount
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


/**
 * Function to print the contents of the memory table
 * 
 * @param arg
 * @param argCount
*/
void listmemtable(String *arg, int argCount)
{
    showMemTable();
}


void listphysmem(String *arg, int argCount)
{
    if (argCount > 1) {
        for (int i = 1; i < argCount; i++) {
            if (arg[i] == "-h") {
                Serial.print("[info]\tValue: ");
                Serial.println(memRead(arg[++i].toInt()));
            } 
        }
    } else {
        showPhysMem();
    }
}


void listfreemem(String *arg, int argCount)
{
    showFreeTable();
}


void neofetch()
{
    Serial.println("\t\tOS: arduinOS");
    Serial.println("\t\tHost: niko");
    Serial.println("\t\tProcessor: Atmega2560");
    Serial.println("\t\tArchitecture: Atmel AVR RISC");
}