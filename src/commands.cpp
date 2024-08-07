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


/**
 * Function to read the input from the serial monitor
 * 
*/
void readInput()
{
    if (Serial.available() != 0) {
        char input = Serial.read();
        Serial.print(input);

        if (input == ' ') {
            bp++;

            if (bp > 4) {
                Serial.println(F("[error]\tToo many arguments supported by the buffer!"));
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
                    Serial.println(F("[error]\tCommand does not exist!"));
                }
            }

            clearBuffer();
            bp = 0;
        } else if ((int)input == 127) { 
            Serial.println(F("Backspace pressed!"));
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
 * Command to show information of the device
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
    Serial.println(F("-------- FILES --------"));
    shuffle();
    allFilesOnFAT();
    Serial.print(F("[info] Total files in filesystem: "));
    Serial.println(totalFilesInFAT());
}


void freespace()
{
    uint16_t t = showFreeSpace();
    Serial.print(F("[info] Available space: "));
    Serial.print(t);
    Serial.println(F("/1024"));
}


/**
 * Command to store a file in the FAT
 * 
*/
void store()
{
    if (strcmp(command_buffer[1], "") == 0 || strcmp(command_buffer[2], "") == 0) {
        Serial.println(F("[error] Cannot save file!"));
        return;
    }

    Serial.flush();

    Serial.println(F("[info] Type in the file contents: "));

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
        Serial.println(F("[error] RETRIEVE requires a parameter!"));
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
        Serial.println(F("[error] ERASE requires a parameter!"));
        return;
    }

    Serial.println(F("[info] start erasing!"));
    if (strcmp(command_buffer[1], "all") == 0) {

        eraseAll();
        Serial.println(F("[info] All files on FAT erased!"));
    } else {
        eraseFATEntry(command_buffer[1]);
        Serial.print(F("[info] "));
        Serial.print(command_buffer[1]);
        Serial.println(F(" erased succesfully!"));
    }
}


/**
 * Function start a new task
 * 
*/
void run()
{
    if (strcmp(command_buffer[1], "") == 0) {
        Serial.println(F("[error] RUN requires a parameter!"));
        return;
    }

    Serial.println(F("[info] Try to start a new task!"));
    addTask(command_buffer[1]);
}


/**
 * Command to list all the processes in the system
 * 
*/
void list()
{
    Serial.println(F("-------- Processes: --------"));
    runningTasks();
}


/**
 * Command to suspend a process from running
 * 
*/
void suspend()
{
    if (strcmp(command_buffer[1], "") == 0) {
        Serial.println(F("[error] SUSPEND requires a parameter!"));
        return;
    }

    suspendTask(atoi(command_buffer[1]));
    Serial.println(F("[info] Succesfully suspended task!"));
}


/**
 * Command to resume a task from suspension
 * 
*/
void resume()
{
    if (strcmp(command_buffer[1], "") == 0) {
        Serial.println(F("[error] RESUME requires a parameter!"));
        return;
    }

    resumeTask(atoi(command_buffer[1]));
    Serial.println(F("[info] Succesfully resumed task!"));
}


/**
 * Command to terminate a process
 * 
*/
void kill()
{
    if (strcmp(command_buffer[1], "") == 0) {
        Serial.println(F("[error] KILL requires a parameter!"));
        return;
    }

    removeTask(atoi(command_buffer[1]));
}


/**
 * Command to show the entire memory table
 * 
*/
void memshow()
{
    showMemTable();
    showStck();
}
