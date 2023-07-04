#include "filesystem.hpp"
#include <EEPROM.h>


static uint8_t  totalFiles = 0;
static uint16_t availableSpace = 0;
static Entry_s _entry;


/**
 * Function to init the filesystem
 * 
*/
void initFileSystem()
{
    totalFiles = EEPROM.read(0);
    availableSpace = EEPROM.read(1);
}


/**
 * Function to write a file to the fat
 * 
 * @param file
 * @param size 
 * @param data
*/
void writeFATEntry(char *file, size_t size, char *contents)
{
    if (strcmp(file, "") == 0) {
        Serial.println("[error]\tNo filename given!");
        return;
    } 

    if (size == 0) {
        Serial.println("[error]\rNo data length given!");
        return;
    }

    struct Entry_s document;
    memcpy(document.filename, file, 12);
    memcpy(document.contents, contents, 60);
    document.size = size;

    availableSpace += sizeof(document);
    if (availableSpace > 1024) {
        Serial.println("[error]\tNo available free space!");
        return;
    }

    uint16_t address = 2;
    while (EEPROM.read(address) != 255 && address < 1024) address++;
    EEPROM.put(address, document);

    Serial.println("[info]\tFile saved on disk!");

    EEPROM.write(0, ++totalFiles);
    EEPROM.write(1, availableSpace);
}


/**
 * 
 * 
*/
char *retrieveFATEntry(char *file)
{
    if (strcmp(file, "") == 0) {
        Serial.println("[error]\tNo filename given!");
        return nullptr;
    }

    uint16_t i = 2;
    while (i < EEPROM.length()) { 
        EEPROM.get(i, _entry);

        if (strcmp(file, _entry.filename) == 0) {
            return _entry.contents;
        }

        i+=sizeof(_entry);
    }

    Serial.println("[error]\tFile does not exist on filesystem!");
    return nullptr;
}


void showFT()
{
    Serial.flush();
    Serial.println("---------------------------------------------------------");

    for (uint8_t i = 0; i < 160; i++) {
        Serial.print("Address: ");
        Serial.print(i);
        Serial.print(" width value: ");
        Serial.println(EEPROM[i]);
    }

    // for (uint8_t i = 0; i < 30; i++) {
    //     if (freeTable[i][1]) {
    //         Serial.print("begin adres: ");
    //         Serial.print(freeTable[i][0]);
    //         Serial.print(" met vrije grootte: ");
    //         Serial.println(freeTable[i][1]);
    //     }
    // }
}


/**
 * Function to read the total files in the file system
 * 
 * @return uint32_t
*/
uint32_t totalFilesInFAT()
{
    return EEPROM.read(0);
}


/**
 * Function to erase a specific file in the fat
 * 
 * @param file
*/
void eraseFATEntry(char *file)
{
    if (strcmp(file, "") == 0) {
        Serial.println("[error]\tNo filename given!");
        return;
    }

}


/**
 * 
 * 
*/
void eraseAll()
{
    for (uint16_t i = 1; i < EEPROM.length(); i++) {
        EEPROM.write(i, 255);
    }

    EEPROM.write(0, (totalFiles = 0));
    EEPROM.write(1, (availableSpace = 0));
}


/**
 * Function to list all files on the FAT
 * 
 * 
*/
void allFilesOnFAT()
{

}


/**
 * Function to retrieve the file address
 * 
 * @param file
 * @return uint16_t
*/
uint16_t getFileAddress(char *file)
{
    return 0;
}
