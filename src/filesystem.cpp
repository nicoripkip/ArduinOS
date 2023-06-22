#include "filesystem.hpp"
#include <EEPROM.h>


#define EEPROM_ADDRESS_START    0
#define EEPROM_ADDRESS_END      1023
#define START_FILE_NAME         253
#define START_FILE_CONTENTS     254


static uint32_t totalFiles = 0;
static uint32_t freeTable[30][2] = {0};
static char name[12];
static char contents[12];


/**
 * Functionto init the file system
 * 
*/
void initFileSystem()
{
    size_t fp = 0;
    uint32_t sa = 0;
    uint32_t i, t;

    totalFiles = EEPROM.read(0);
    if (EEPROM.read(0) == 255) {
        EEPROM.write(0, 0);
    }

    t = 0;
    for (i = EEPROM_ADDRESS_START; i < EEPROM_ADDRESS_END; i++) {
        if (readFATEntry(i) != 255) {
            sa += t+1;
            t = 0;
            if (readFATEntry(i-1) == 255) {
                freeTable[fp][0] = sa;
                freeTable[fp][1] = t;
                fp++;
            }
        }  else if (readFATEntry(i) == 255) {
            t++;
        } 
    } 

    freeTable[fp][0] = sa;
    freeTable[fp][1] = t;
}


/**
 * Function to refresh the freetable to update the fat entries
 * 
*/
void refreshFreeTable()
{
    size_t fp = 0;
    uint32_t sa = 0;
    uint32_t i, t;

    EEPROM.write(0, totalFiles);

    memset(freeTable, 0, sizeof(freeTable));

    t = 0;
    for (i = EEPROM_ADDRESS_START; i < EEPROM_ADDRESS_END; i++) {
       if (readFATEntry(i) != 255) {
            sa += t+1;
            t = 0;
            if (readFATEntry(i-1) == 255) {
                freeTable[fp][0] = sa;
                freeTable[fp][1] = t;
                fp++;
            }
        }  else if (readFATEntry(i) == 255) {
            t++;
        } 
    }

    freeTable[fp][0] = sa;
    freeTable[fp][1] = t;
}


/**
 * Function to write a file to the fat
 * 
 * @param file
 * @param size 
 * @param data
*/
void writeFATEntry(char *file, size_t size, char *data)
{
    if (strcmp(file, "") == 0) {
        Serial.println("[error]\tNo filename given!");
        return;
    }

    if (size == 0) {
        Serial.println("[error]\rNo data length given!");
        return;
    }

    unsigned int totalSize = strlen(file) + size + 2;

    for (uint32_t i = 0; i < 30; i++) {
        if (totalSize >= freeTable[i][1]) {
            uint32_t sa = freeTable[i][0];
            uint16_t j = 0;

            EEPROM.write(sa, START_FILE_NAME);
            for (uint16_t i = 0; i < strlen(file); i++) {
                EEPROM.write(sa+(i+1), file[i]);
            }
            j+=strlen(file)+1;
            EEPROM.write(sa+j, START_FILE_CONTENTS);
            j+=1;
            for (uint16_t i = 0; i < size; i++) {
                EEPROM.write(sa+(j+i), data[i]);
            }

            totalFiles++;
            refreshFreeTable();
            return;
        }
    } 

    Serial.println("[error]\tNo disk space available!");
}


/**
 * Function to read data from a specific fat entry
 * 
*/
int readFATEntry(uint32_t address)
{
    int t = EEPROM.read(address);
    return t;
}


void showFT()
{
    Serial.flush();
    Serial.println("---------------------------------------------------------");

    for (uint32_t i = 0; i < 30; i++) {
        if (freeTable[i][1]) {
            Serial.print("begin adres: ");
            Serial.print(freeTable[i][0]);
            Serial.print(" met vrije grootte: ");
            Serial.println(freeTable[i][1]);
        }
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
 * 
 * 
*/
char *retrieveFATEntry(char *file)
{
    if (strcmp(file, "") == 0) {
        Serial.println("[error]\tNo filename given!");
        return nullptr;
    }

    uint16_t np = 0;
    memset(name, 0, 12);
    for (uint16_t i = EEPROM_ADDRESS_START; i < EEPROM_ADDRESS_END; i++) {
        if (readFATEntry(i) == START_FILE_NAME) {
            Serial.println("[info]\tFound a file");

            uint16_t t = 1;
            while (readFATEntry(i+t) != START_FILE_CONTENTS) {
                if (readFATEntry(i+t) > 29 && readFATEntry(i+t) < 173) {
                    name[np++] = readFATEntry(i+t);
                }

                t++;
            }

            if (strcmp(name, file) == 0) {
                t+=1;
                np = 0;

                while (readFATEntry(i+t) != START_FILE_CONTENTS && readFATEntry(i+t) != 255) {
                    if (readFATEntry(i+t) > 29 && readFATEntry(i+t) < 173) {
                        contents[np++] = readFATEntry(i+t);
                    }

                    t++;
                }
        
                return contents;
            }

            memset(name, 0, 12);
            i += 1;
            np = 0;
        }
    }

    Serial.println("[error]\tFile does not exist on filesystem!");
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
    uint32_t i;

    for (i = EEPROM_ADDRESS_START+1; i < EEPROM_ADDRESS_END; i++) {
        EEPROM.write(i, 255);
    }

    totalFiles = 0;
    refreshFreeTable();
}


/**
 * Function to list all files on the FAT
 * 
 * 
*/
void allFilesOnFAT()
{
    uint16_t np = 0;

    memset(name, 0, 12);
    for (uint16_t i = EEPROM_ADDRESS_START; i < EEPROM_ADDRESS_END; i++) {
        if (readFATEntry(i) == START_FILE_NAME) {
            uint16_t t = 1;
            while (readFATEntry(i+t) != START_FILE_CONTENTS) {
                if (readFATEntry(i+t) > 29 && readFATEntry(i+t) < 173) 
                    name[np++] = readFATEntry(i+t);
                t++;
            }

            Serial.println(name);
            memset(name, 0, 12);
            np = 0;

            i += t;
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
    char *name = new char[12];
    uint16_t np = 0;

    for (uint16_t i = EEPROM_ADDRESS_START; i < EEPROM_ADDRESS_END; i++) {
        if (readFATEntry(i) == START_FILE_NAME) {
            uint16_t t = 1;
            while (readFATEntry(i+t) != START_FILE_CONTENTS) {
                if (readFATEntry(i+t) > 29 && readFATEntry(i+t) < 173) 
                    name[np++] = readFATEntry(i+t);
                t++;
            }

            Serial.println(name);

            if (strcmp(name, file) == 0) {
                return i + strlen(name) + 1;
            }

            name = "";
            np = 0;

            i += t;
        }
    }
}


void jemoeder() 
{
    uint32_t i;

    for (i = EEPROM_ADDRESS_START; i < EEPROM_ADDRESS_END; i++) {
        Serial.print("[info]\taddress: ");
        Serial.print(i);
        Serial.print(" width data: ");
        Serial.println(EEPROM.read(i));
    }
}
