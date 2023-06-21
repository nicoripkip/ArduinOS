#include "filesystem.hpp"
#include <EEPROM.h>


#define EEPROM_ADDRESS_START    0
#define EEPROM_ADDRESS_END      1023


static uint32_t totalFiles = 0;
static uint32_t freeTable[30][2] = {0};


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
            sa=(i+1);

            t = 0;
            while (readFATEntry(1) != 255 && t < 1023) t++;
            i = t-1;
            
            freeTable[fp][0] = sa;
            freeTable[fp][1] = 1023 - t;

            fp++; 
        }
    }

    if (freeTable[fp-1][0] != sa) {
        freeTable[fp][0] = sa;
        freeTable[fp][1] = i - t;
    }
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

    t = 0;
    for (i = EEPROM_ADDRESS_START; i < EEPROM_ADDRESS_END; i++) {
        if (readFATEntry(i) != 255) {
            sa=(i+1);

            t = 0;
            while (readFATEntry(t) != 255 && t < 1023) t++;
            i = t-1;
            
            freeTable[fp][0] = sa;
            freeTable[fp][1] = 1023 - t;

            fp++; 
        }
    }

    if (freeTable[fp-1][0] != sa) {
        freeTable[fp][0] = sa;
        freeTable[fp][1] = i - t;
    }
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

            EEPROM.write(sa, 1);
            for (uint16_t i = 0; i < strlen(file); i++) {
                EEPROM.write(sa+=(i+1), file[i]);
            }
            j+=strlen(file)+1;
            EEPROM.write(sa+j, 2);
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
    char *name = new char[12];
    char *contents = new char[12];

    for (uint16_t i = EEPROM_ADDRESS_START; i < EEPROM_ADDRESS_END; i++) {
        if (readFATEntry(i) == 1) {
            Serial.println("[info]\tFound a file");

            uint16_t t = 0;
            while (readFATEntry(i+1+t) != 2 && t < 12) {
                if (readFATEntry(i+1+t) > 29 && readFATEntry(i+1+t) < 173) {
                    name[t] = readFATEntry(i+1+t);
                }

                t++;
            }

            Serial.println("It gets here");

            uint16_t p = 0;
            for (uint16_t j; j < strlen(file); j++) {
                Serial.print("[info]\t");
                Serial.print(file[j]);
                Serial.print(" == ");
                Serial.println(name[j]);

                if (file[j] == name[j]) {
                    p++;
                }
            }

            if (p == strlen(file) && name[p+1] == 0) {
                Serial.println("[info]\tFound the correct file!");
                return name;
            }

            i += 1;
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
