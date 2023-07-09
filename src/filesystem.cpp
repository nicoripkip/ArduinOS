#include "filesystem.hpp"
#include <EEPROM.h>


#define MAX_FAT_SIZE        15
#define BEGIN_CONTENT_SPACE 280


static uint8_t  totalFiles = 0;
static uint16_t availableSpace = 0;
static Entry_s FAT[MAX_FAT_SIZE];
static char buffer[30];


/**
 * Function to init the filesystem
 * 
*/
void initFileSystem()
{
    totalFiles = EEPROM.read(0);
    availableSpace = EEPROM.read(1);
    EEPROM.get(2, FAT);
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
        Serial.println(F("[error]\tNo filename given!"));
        return;
    } 

    if (size == 0) {
        Serial.println(F("[error]\rNo data length given!"));
        return;
    }

    for (uint8_t i = 0; i < MAX_FAT_SIZE; i++) {
        if (strcmp(file, FAT[i].filename) == 0) {
            Serial.println(F("[error]\tFile already exists on disk!"));
            return;
        }
    }

    Serial.print(F("Grootte eeprom: "));
    Serial.println(contents);

    uint16_t i;
    for (i = BEGIN_CONTENT_SPACE; i < EEPROM.length(); i++) {
        if (EEPROM.read(i) == 255) {
            uint16_t j;
            for (j = i; j < EEPROM.length(); j++) {
                if (EEPROM.read(j) != 255 || j > size) {
                    break;
                }
            }

            if (j >= size) {
                break;
            }
        }
    }

    if (i+size+1 < EEPROM.length()) {
        memcpy(FAT[totalFiles].filename, file, 12);
        FAT[totalFiles].contents = i;
        FAT[totalFiles].size = size;

        uint16_t j;
        for (j = i; j < i + size; j++) {
            Serial.println(contents[j-i]);
            EEPROM.write(j, (int)contents[j-i]);
        }
        EEPROM.write(j+1, 32);

        EEPROM.put(2, FAT);
        EEPROM.write(0, ++totalFiles);
        EEPROM.write(1, (availableSpace+size));

        Serial.println(F("[info]\tFile saved on disk!"));
    } else {
        Serial.println(F("[error]\tNot enough space on disk!"));
    }
}


/**
 * Function to retrieve a file from the FAT and read its contents
 * 
 * @param file
 * @returns char
*/
char *retrieveFATEntry(char *file)
{
    if (strcmp(file, "") == 0) {
        Serial.println(F("[error]\tNo filename given!"));
        return nullptr;
    }

    memset(buffer, 0, 30);

    uint8_t i;
    for (i = 0; i < MAX_FAT_SIZE; i++) {
        if (strcmp(file, FAT[i].filename) == 0) {
            Serial.println(F("[info]\tFound file on FAT"));
            Serial.println(FAT[i].contents);

            for (uint16_t j = FAT[i].contents; j < FAT[i].contents + FAT[i].size; j++) {
                char b = EEPROM.read(j);
                strncat(buffer, &b, 1);
            }
            
            return buffer;
        }
    }


    Serial.println(F("[error]\tFile does not exist on filesystem!"));
    return nullptr;
}


/**
 * Function to show the freespace on the eeprom
 * 
*/
void showFT()
{
    Serial.flush();
    Serial.println(F("---------------------------------------------------------"));

    for (uint16_t i = BEGIN_CONTENT_SPACE; i < EEPROM.length(); i++) {
        Serial.print(F("Address: "));
        Serial.print(i);
        Serial.print(F(" width value: "));
        Serial.println(EEPROM[i]);
    }
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
        Serial.println(F("[error]\tNo filename given!"));
        return;
    }

    uint8_t i;
    for (i = 0; i < MAX_FAT_SIZE; i++) {
        if (strcmp(file, FAT[i].filename) == 0) {
            Serial.println(F("[info]\tFound file on FAT"));
            Serial.println(FAT[i].contents);

            uint16_t j;
            for (j = FAT[i].contents; j < FAT[i].contents + FAT[i].size; j++) {
                EEPROM.write(j, 255);
            }
            EEPROM.write(j+1, 255);

            break;
        }
    }

    memcpy(FAT[i].filename, "", 1);
    FAT[i].size = 0;
    FAT[i].contents = 0;

    EEPROM.write(0, (--totalFiles));
    EEPROM.put(2, FAT);

    Serial.println(F("[error]\tFile does not exist on filesystem!"));
    return;
}


/**
 * Function to erase all the data on the FAT
 * 
*/
void eraseAll()
{
    for (uint8_t i = 0; i < MAX_FAT_SIZE; i++) {
        memcpy(FAT[i].filename, "", 1);
        FAT[i].size = 0;
        FAT[i].contents = 0;
    }

    for (uint16_t i = 1; i < EEPROM.length(); i++) {
        EEPROM.write(i, 255);
    }

    EEPROM.write(0, (totalFiles = 0));
    EEPROM.write(1, (availableSpace = 0));
    EEPROM.put(2, FAT);
}


/**
 * Function to list all files on the FAT
 * 
 * 
*/
void allFilesOnFAT()
{
    for (uint8_t i = 0; i < MAX_FAT_SIZE; i++) {
        if (strcmp(FAT[i].filename, "")) {
            Serial.print(F(" -- "));
            Serial.println(FAT[i].filename);
        }
    }
}


/**
 * Function to retrieve the file address
 * 
 * @param file
 * @return uint16_t
*/
uint16_t getFileAddress(char *file)
{
    for (uint8_t i = 0; i < MAX_FAT_SIZE; i++) {
        if (strcmp(FAT[i].filename, file) == 0) {
            return FAT[i].contents;
        }
    }

    return 0;
}


/**
 * Function to retrieve the file size
 * 
 * @param file
 * @return size_t
*/
size_t getFileSize(char *file)
{
    for (uint8_t i = 0; i < MAX_FAT_SIZE; i++) {
        if (strcmp(FAT[i].filename, file) == 0) {
            return FAT[i].size;
        }
    }

    return 0;
}


/**
 * Show the freespace on the FAT
 * 
 * @return uint16_t
*/
uint16_t showFreeSpace()
{
    uint16_t c = 0;

    for (uint16_t i = 0; i < MAX_FAT_SIZE; i++) {
        if (FAT[i].size != 0) {
            c+= FAT[i].size;
        }
    }

    return EEPROM.length() - 280 - c;
}