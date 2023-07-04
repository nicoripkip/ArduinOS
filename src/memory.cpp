#include <Arduino.h>
#include "memory.hpp"
#include "instructionset.hpp"


#define STACK_ADDRESS_START     0
#define STACK_ADDRESS_END       256
#define MEMORY_TABLE_SIZE       25
#define ZERO_TERMINATOR         0


static memtable_s memoryTable[MEMORY_TABLE_SIZE];
static byte stack[STACK_ADDRESS_END] = { 255 };
// static uint8_t occupiedTable[10];
static uint8_t sp = 0;


void pushByte(byte b)
{
    stack[sp++] = b;
}


byte popByte()
{
    return stack[--sp];
}


/**
 * Function to push an int variable to the stack
 * 
 * @param x
 * @param y
*/
void pushInt(int x)
{
    if (2 + sp > STACK_ADDRESS_END) {
        Serial.println(F("[error]\tNo space available in memory!"));
        return;
    }

    pushByte(INT);
    pushByte(x);
}


/**
 * Function to push a char on the stack
 * 
 * @param x
*/
void pushChar(char x)
{
    if (2 + sp > STACK_ADDRESS_END) {
        Serial.println(F("[error]\tNo space available in memory!"));
        return;
    }

    pushByte(CHAR);
    pushByte(x);
}


void pushFloat(float x)
{

}


/**
 * Function to push a string on the stack
 * 
 * @param x
*/
void pushString(char *x)
{
    uint16_t l = strlen(x);

    if (3 + l + sp > STACK_ADDRESS_END) {
        Serial.println(F("[error]\tNo space available in memory!"));
        return;
    }

    pushByte(STRING);
    pushByte(l);
    for (uint8_t i = 0; i < l; i++) {
        pushByte(x[i]);
    }
    pushByte(ZERO_TERMINATOR);
}


void refreshStack()
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
void memAlloc(uint8_t pid, char *name, void *data, size_t size, memtype_e type)
{
    void *p;
    if (type == INT) {
        // (int)p = (int)data;
    } else if (type == CHAR) {

    } else if (type == FLOAT) {

    } else if (type == STRING) {

    }

    for (uint8_t i = 0; i < MEMORY_TABLE_SIZE; i++) {
        if (memoryTable[i].state == FREE) {
            memoryTable[i].p_id = pid;
            memoryTable[i].name = name;
            memoryTable[i].type = type;
            memoryTable[i].state = OCCUPIED;
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
        if (memoryTable[i].p_id == pid && strcmp(memoryTable[i].name, name) == 0 && memoryTable[i].state == OCCUPIED) {
            if (memoryTable[i].type == STRING) {

            } else if (memoryTable[i].type == FLOAT) {

            } else {
                stack[memoryTable[i].virtaddr] = 0;
                stack[memoryTable[i].virtaddr+1] = 0;
            }

            memoryTable[i].state = FREE;
        }
    }
}


/**
 * 
 * 
*/
byte *stackAlloc(size_t size)
{
    for (uint8_t i = STACK_ADDRESS_START; i < STACK_ADDRESS_END; i++) {
        if (stack[i] == 255) {
            uint8_t j;
            for (j = i; i < i + size; i++) if (stack[j] != 255) break;

            if (j >= i+size) {
                if (j+1 < STACK_ADDRESS_END) {
                    return &stack[i];
                } else {
                    Serial.println(F("[error]\tProcess can't allocate stack"));
                    return nullptr;
                }
            }
        }
    }
}