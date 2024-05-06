#include <Arduino.h>
#include "memory.hpp"
#include "instructionset.hpp"


#define STACK_ADDRESS_START     0
#define STACK_ADDRESS_END       256
#define MEMORY_TABLE_SIZE       25
#define ZERO_TERMINATOR         0


static memtable_s memoryTable[MEMORY_TABLE_SIZE];
static byte stack[STACK_ADDRESS_END];


/**
 * Function to setup memory
 * 
*/
void initMemory()
{
    for (uint16_t i = 0; i < STACK_ADDRESS_END; i++) {
        stack[i] = 254;
    }
}


/**
 * Function to push a byte onto the stack
 * 
 * @param address
 * @param sp
 * @param b
 * @return byte
*/
byte *pushByte(byte *address, uint8_t &sp, byte b)
{
    // Serial.println(sp);
    address[sp++] = b;
    return address;
}


/**
 * Function to pop a byte from the stack
 * 
 * @param address
 * @param sp
 * @return byte
*/
byte popByte(byte *address, uint8_t &sp)
{
    return address[--sp];
}


/**
 * Function to push an int variable to the stack
 * 
 * @param x
 * @param y
 * @return uint16_t
*/
byte *pushInt(byte *address, uint8_t &sp, int x, int y)
{
    if (2 + sp > 32) {
        Serial.println(F("[error]\tNo space available in memory!"));
        return;
    }

    // Serial.println(sp);

    byte *a = pushByte(address, sp, INT);
    pushByte(address, sp, x);
    pushByte(address, sp, y);

    return a;
}


/**
 * Function to push a char on the stack
 * 
 * @param x
 * @return uint16_t
*/
byte *pushChar(byte *address, uint8_t &sp, char x, int y)
{
    if (2 + sp > 32) {
        Serial.println(F("[error]\tNo space available in memory!"));
        return;
    }

    pushByte(address, sp, CHAR);
    pushByte(address, sp, x);
}


/**
 * Function to push a float on the stack
 * 
 * @param x
 * @return uint16_t
*/
byte *pushFloat(byte *address, uint8_t &sp, float x)
{

}


/**
 * Function to push a string on the stack
 * 
 * @param x
 * @return uint16_t
*/
byte *pushString(byte *address, uint8_t &sp, char *x)
{
    uint16_t l = strlen(x);

    if (3 + l + sp > 32) {
        Serial.println(F("[error]\tNo space available in memory!"));
        return;
    }

    pushByte(address, sp, STRING);
    pushByte(address, sp, l);
    for (uint8_t i = 0; i < l; i++) {
        pushByte(address, sp, x[i]);
    }
    pushByte(address, sp, ZERO_TERMINATOR);
}


/**
 * Function to pop an integer from the stack
 * 
 * @param address
*/
int popInt(byte *address, uint8_t &sp)
{
    byte r = popByte(address, sp);
    popByte(address, sp);

    return (int)r;
}


/**
 * Function to pop a char from the stack
 * 
 * @param address
 * @param sp
*/
char popChar(byte *address, uint8_t &sp)
{
    byte r = popByte(address, sp);
    popByte(address, sp);

    return (char)r;
}


float popFloat(byte *address, uint8_t &sp)
{

}


char *popString(byte *address, uint8_t &sp)
{

}


/**
 * Function to allocate the variable in memory
 * 
 * @param pid
 * @param name
 * @param data
 * @param size
 * @param type
*/
void memAlloc(uint8_t pidd, char *name, size_t size, memtype_e type, byte *address)
{
    // Serial.print(F("Process id in ma: "));
    // Serial.println(pidd);

    for (uint8_t i = 0; i < MEMORY_TABLE_SIZE; i++) {
        if (memoryTable[i].state == FREE) {
            memoryTable[i].pid = pidd;
            // memoryTable[i].coolerepid = pidd;
            memcpy(memoryTable[i].name, name, 12);
            memoryTable[i].type = type;
            memoryTable[i].state = OCCUPIED;
            memoryTable[i].address = address;
            return;
        }
    }
}


/**
 * Function to find the value from the memory table
 * 
 * @param pid
 * @param name
*/
memtable_s *memRead(uint8_t pid, char *name)
{
    for (uint8_t i = 0; i < MEMORY_TABLE_SIZE; i++) {
        if (memoryTable[i].pid == pid && strcmp(memoryTable[i].name, name) == 0) {
            return &memoryTable[i];
        }
    }

    Serial.println(F("[error]\tVariable doesn't exist"));
}


/**
 * Function to get the type of the variable
 * 
*/
memtype_e typeSearch(uint8_t pid, char *name)
{
    for (uint8_t i = 0; i < MEMORY_TABLE_SIZE; i++) {
        if (memoryTable[i].pid == pid && strcmp(memoryTable[i].name, name) == 0) {
            return memoryTable[i].type;
        }
    }
}


/**
 * Function to get the type of the variable
 * 
*/
byte *addressSearch(uint8_t pid, char *name)
{
    for (uint8_t i = 0; i < MEMORY_TABLE_SIZE; i++) {
        if (memoryTable[i].pid == pid && strcmp(memoryTable[i].name, name) == 0) {
            return memoryTable[i].address;
        }
    }
}


/**
 * Function to free memory
 * 
 * @param pid
 * @param name
*/
void memFree(uint8_t pid, char *name)
{
    for (uint8_t i = 0; i < MEMORY_TABLE_SIZE; i++) {
        if (memoryTable[i].pid == pid) {
            memoryTable[i].pid = 0;
            memcpy(memoryTable[i].name, "", 0);
            memoryTable[i].type = VOID;
            memoryTable[i].state = FREE;
            memoryTable[i].address = nullptr;
        }
    }
}


/**
 * Function to reserve a bit of memory on the public stack
 * 
 * @param size
 * @return byte
*/
byte *stackAlloc(size_t size)
{    
    for (uint16_t i = STACK_ADDRESS_START; i < STACK_ADDRESS_END; i++) {
        if (stack[i] == 254) {
            uint8_t j;
            for (j = i; i < i + size; i++) if (stack[j] != 254) break;

            if (j >= i+size) {
                if (j+1 < STACK_ADDRESS_END) {
                    byte *t = &stack[i];
                    Serial.println((uint16_t)t);
                    return t;
                } else {
                    Serial.println(F("[error]\tProcess can't allocate stack"));
                    return nullptr;
                }
            }
        }
    }
}


/**
 * Function to show the contents of the stack
 * 
 * @param address
*/
void showStack(byte *address)
{
    for (uint8_t i = 0; i < 32; i++) {
        Serial.print(F("Address: "));
        Serial.print(i);
        Serial.print(F(" | data: "));
        Serial.println(address[i]);
    }
}


/**
 * Function to show the contents of the memory table
 * 
*/
void showMemTable()
{
    for (uint8_t i = 0; i < MEMORY_TABLE_SIZE; i++) {
        Serial.print(F("Process id: "));
        Serial.print(memoryTable[i].pid);
        Serial.print(F(" | variable name: "));
        Serial.print(memoryTable[i].name);
        Serial.print(F(" | data type: "));
        Serial.print(memoryTable[i].type);
        Serial.print(F(" | address: "));
        Serial.println((uint16_t)memoryTable[i].address);
    }
}
