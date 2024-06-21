#include "filesystem.hpp"
#include <EEPROM.h>


#define MAX_FAT_SIZE        10
#define BEGIN_CONTENT_SPACE 280
#define END_CONTENT_SPACE   1024


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

    // make sure every empty pointers stays zero
    for (uint8_t i = 0; i < MAX_FAT_SIZE; i++) {
        if (FAT[i].contents == 65535) {
            FAT[i].contents = 0;
        }
    }
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
        Serial.println(F("[error] No filename given!"));
        return;
    } 

    if (size == 0) {
        Serial.println(F("[error] No data length given!"));
        return;
    }

    if (totalFiles+1 > MAX_FAT_SIZE) {
        Serial.println(F("[error] Too many files on FAT!"));
        return;
    }

    // Check if file name already exists on filesystem
    for (uint8_t i = 0; i < MAX_FAT_SIZE; i++) {
        if (strcmp(file, FAT[i].filename) == 0) {
            Serial.println(F("[error] File already exists on disk!"));
            return;
        }
    }

    Serial.print(F("Text to save on disk: "));
    Serial.println(contents);

    uint8_t i;
    for (i = 0; i < MAX_FAT_SIZE-1; i++) {

        // Check if there is a free place in the FAT table 
        if (FAT[i+1].contents == 0) {

            // Check if there is enough space on disk
            if (FAT[i].contents + FAT[i].size + size + 1 < END_CONTENT_SPACE) {
                memcpy(FAT[i+1].filename, file, 12);
                FAT[i+1].contents = BEGIN_CONTENT_SPACE + FAT[i].contents + FAT[i].size + 1;
                FAT[i+1].size = size;
                
                uint8_t k = 0;
                for (uint16_t j = FAT[i+1].contents; j < FAT[i+1].contents + size; j++) {
                    EEPROM.write(j, contents[k]);
                    k++;

                    Serial.print("FAT address: ");
                    Serial.print(j);
                    Serial.print(" | Value: ");
                    Serial.println(contents[k]);
                }
            } else {
                Serial.println(F("[error] Not enough space on disk!"));
                return;
            }

            break;
        }
    }

    EEPROM.write(0, (totalFiles++));
    EEPROM.put(2, FAT);
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
        Serial.println(F("[error] No filename given!"));
        return nullptr;
    }

    // Reset file buffer
    memset(buffer, 0, 30);

    for (uint8_t i = 0; i < MAX_FAT_SIZE; i++) {
        if (strcmp(file, FAT[i].filename) == 0) {

            // Read file contents from eeprom
            for (uint16_t j = FAT[i].contents; j < FAT[i].contents + FAT[i].size; j++) {
                char b = EEPROM.read(j); 
                strncat(buffer, &b, 1); 
            }
            
            return buffer;
        }
    }


    Serial.println(F("[error] File does not exist on filesystem!"));
    return nullptr;
}


/**
 * Function to show the freespace on the eeprom
 * 
*/
void showFT()
{
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
        Serial.println(F("[error] No filename given!"));
        return;
    }

    for (uint8_t i = 0; i < MAX_FAT_SIZE; i++) {
        if (strcmp(file, FAT[i].filename) == 0) {
            memcpy(FAT[i].filename, '\0', 12);
            FAT[i].contents = 0;
            FAT[i].size = 0;

            break;
        }
    }

    EEPROM.write(0, (--totalFiles));
    EEPROM.put(2, FAT);

    Serial.println(F("[error] File erased from FAT!"));
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
        // if (strcmp(FAT[i].filename, "")) {
            Serial.print(F(" -- "));
            Serial.print(FAT[i].filename); 
            Serial.print(F(" and: "));
            Serial.println(FAT[i].contents);
        // }
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



uint8_t readDataRegion(char *buffer, uint16_t address)
{
    memset(buffer, '\0', 10);

    int counter = 0;

    while ((int)EEPROM.read(address+counter) != 32) {
        char c = (char)EEPROM.read(address);
        strncat(buffer, c, 1);
        counter++;
    }   

    return counter;
}
