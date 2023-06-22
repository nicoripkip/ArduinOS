#include "commands.hpp"
#include <Arduino.h>
#include "memory.hpp"
#include "filesystem.hpp"
#include "scheduler.hpp"


#define MONITOR_SPEED 115220


static char command_buffer[4][BUFFER_SIZE];
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

            if (bp > 4) {
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
            strncat(command_buffer[bp], &input, BUFFER_SIZE);
        }

        // Serial.print(command_buffer[bp]);
    }
}


/**
 * Help command
 * 
*/
void help()
{    
    char *t = "";

    Serial.println("--- All available commands: ---");
    for (uint16_t i = 0; i < MAX_COMMANDS; i++) {
        if (strcmp(command_table[i].command, "")) {
            Serial.print(" -- ");
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
    // showMemTable();
}


void listphysmem(String *arg, int argCount)
{
    // if (argCount > 1) {
    //     for (int i = 1; i < argCount; i++) {
    //         if (arg[i] == "-h") {
    //             Serial.print("[info]\tValue: ");
    //             Serial.println(memRead(arg[++i].toInt()));
    //         } 
    //     }
    // } else {
    //     showPhysMem();
    // }
}


void listfreemem(String *arg, int argCount)
{
    // showFreeTable();
}


void neofetch()
{
    Serial.println("\t\tOS: arduinOS");
    Serial.println("\t\tHost: niko");
    Serial.println("\t\tProcessor: Atmega2560");
    Serial.println("\t\tArchitecture: Atmel AVR RISC");
}


///
/// Filesystem operations commands
///
void files()
{
    Serial.print("[info]\tTotal files in filesystem: ");
    Serial.println(totalFilesInFAT());
    Serial.println("-------- FILES --------");
    allFilesOnFAT();
}


void freespace()
{
    showFT();
}


void store()
{

    int t = atoi(command_buffer[2]);

    Serial.println(t);

    if (strcmp(command_buffer[1], "") == 0 || strcmp(command_buffer[2], "") == 0 ||strcmp(command_buffer[3], "") == 0) {
        Serial.println("[error]\tCannot save file!");
        return;
    }

    writeFATEntry(command_buffer[1], atoi(command_buffer[2]), command_buffer[3]);
}


/**
 * Command to retrieve a file from the filesystem
 * 
*/
void retrieve()
{
    // jemoeder();

    if (strcmp(command_buffer[1], "") == 0) {
        Serial.println("[error]\tRETRIEVE requires a parameter!");
        return;
    }

    char * result = retrieveFATEntry(command_buffer[1]);
    delete result;
}


/**
 * Command to erase files from the filesystem
 * 
*/
void erase()
{
    if (strcmp(command_buffer[1], "") == 0) {
        Serial.println("[error]\tERASE requires a parameter!");
        return;
    }

    Serial.println("[info]\tStart erasing!");
    if (strcmp(command_buffer[1], "all") == 0) {

        eraseAll();
        Serial.println("[info]\tAll files on FAT erased!");
    } else {
        eraseFATEntry(command_buffer[1]);
    }
}


void run()
{
    if (strcmp(command_buffer[1], "") == 0) {
        Serial.println("[error]\tRUN requires a parameter!");
        return;
    }
}


void list()
{

}


void suspend()
{
    if (strcmp(command_buffer[1], "") == 0) {
        Serial.println("[error]\tSUSPEND requires a parameter!");
        return;
    }
}


void resume()
{
    if (strcmp(command_buffer[1], "") == 0) {
        Serial.println("[error]\tRESUME requires a parameter!");
        return;
    }
}


void kill()
{
    if (strcmp(command_buffer[1], "") == 0) {
        Serial.println("[error]\tKILL requires a parameter!");
        return;
    }
}