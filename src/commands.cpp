#include "commands.hpp"
#include <Arduino.h>
#include "memory.hpp"
#include "filesystem.hpp"
#include "scheduler.hpp"


#define MONITOR_SPEED 115220


static char command_buffer[4][BUFFER_SIZE];
static char instructionBuffer[30];
static size_t bp = 0;


/**
 * Function to clear the command buffer
 * 
*/
void clearBuffer()
{
    memset(command_buffer, 0, sizeof(command_buffer));
}


bool isNullTerminated(const char* str) {
  return (str[strlen(str)] == '\0');
}


/**
 * Function to read the input from the serial monitor
 * 
*/
void readInput()
{
    Serial.flush();
    if (Serial.available() != 0) {
        char input = Serial.read();
        Serial.print(input);

        if (input == ' ') {
            bp++;

            if (bp > 4) {
                Serial.println("[error]\tToo many arguments supported by the buffer!");
                bp = 0;
                clearBuffer();
                return;
            }
        } else if (input == '\r')  {            
            for (uint8_t i = 0; i < MAX_COMMANDS; i++) {
                if (strncmp(command_buffer[0], command_table[i].command, 12) == 0) {
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
            strncat(command_buffer[bp], &input, 1);
        }
    }
}


/**
 * Help command
 * 
*/
void help()
{    
    char *t = "";

    Serial.println(F("--- All available commands: ---"));
    for (uint16_t i = 0; i < MAX_COMMANDS; i++) {
        if (strcmp(command_table[i].command, "")) {
            Serial.print(F(" -- "));
            Serial.println(command_table[i].command);
        }
    }
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
            Serial.print(F("[info]\t"));
            Serial.println(arg[++i]);
        }
    }
}


/**
 * 
 * 
*/
void neofetch()
{
    Serial.flush();
    Serial.println(F("\t\tOS: arduinOS"));
    Serial.println(F("\t\tHost: niko"));
    Serial.println(F("\t\tProcessor: Atmega2560"));
    Serial.println(F("\t\tArchitecture: Atmel AVR RISC"));
}


///
/// Filesystem operations commands
///
void files()
{
    Serial.flush();
    Serial.print(F("[info]\tTotal files in filesystem: "));
    Serial.println(totalFilesInFAT());
    Serial.println(F("-------- FILES --------"));
    allFilesOnFAT();
}


void freespace()
{
    showFT();
}


/**
 * Command to store a file in the FAT
 * 
*/
void store()
{
    if (strcmp(command_buffer[1], "") == 0 || strcmp(command_buffer[2], "") == 0) {
        Serial.println(F("[error]\tCannot save file!"));
        return;
    }

    Serial.flush();

    Serial.println(F("[info]\tType in the file contents: "));

    while (true) {
        if (Serial.available() != 0) {
            char b = Serial.read();
            Serial.print(b);

            if (b == '\r') {
                Serial.println(instructionBuffer);
                writeFATEntry(command_buffer[1], atoi(command_buffer[2]), instructionBuffer);
                memset(instructionBuffer, 0, sizeof(instructionBuffer));
                return;
            } else {
                strncat(instructionBuffer, &b, 1);
            }
        }
    }
}


/**
 * Command to retrieve a file from the filesystem
 * 
*/
void retrieve()
{
    Serial.flush();
    if (strcmp(command_buffer[1], "") == 0) {
        Serial.println(F("[error]\tRETRIEVE requires a parameter!"));
        return;
    }

    char * result = retrieveFATEntry(command_buffer[1]);
    Serial.println(F("-------- Contents: --------"));
    Serial.println(result);
    delete result;
}


/**
 * Command to erase files from the filesystem
 * 
*/
void erase()
{
    if (strcmp(command_buffer[1], "") == 0) {
        Serial.println(F("[error]\tERASE requires a parameter!"));
        return;
    }

    Serial.println(F("[info]\tStart erasing!"));
    if (strcmp(command_buffer[1], "all") == 0) {

        eraseAll();
        Serial.println(F("[info]\tAll files on FAT erased!"));
    } else {
        eraseFATEntry(command_buffer[1]);
        Serial.print(F("[info]\t"));
        Serial.print(command_buffer[1]);
        Serial.println(F(" erased succesfully!"));
    }
}


/**
 * Function
 * 
*/
void run()
{
    if (strcmp(command_buffer[1], "") == 0) {
        Serial.println(F("[error]\tRUN requires a parameter!"));
        return;
    }

    Serial.println(F("[info]\tTry to start a new task!"));
    addTask(command_buffer[1]);
}


void list()
{
    Serial.println(F("-------- Processes: --------"));
    runningTasks();
}


void suspend()
{
    if (strcmp(command_buffer[1], "") == 0) {
        Serial.println(F("[error]\tSUSPEND requires a parameter!"));
        return;
    }
}


void resume()
{
    if (strcmp(command_buffer[1], "") == 0) {
        Serial.println(F("[error]\tRESUME requires a parameter!"));
        return;
    }
}


void kill()
{
    if (strcmp(command_buffer[1], "") == 0) {
        Serial.println(F("[error]\tKILL requires a parameter!"));
        return;
    }

    removeTask(atoi(command_buffer[1]));
}