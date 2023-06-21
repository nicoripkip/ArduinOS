#include <Arduino.h>
#include "memory.hpp"


#define STACK_ADDRESS_START     0
#define STACK_ADDRESS_END       256
#define MEMORY_TABLE_SIZE       25
#define ZERO_TERMINATOR         0


static memtype_e memoryTable[MEMORY_TABLE_SIZE];
static byte stack[STACK_ADDRESS_END];
static uint32_t sp = 0;


void pushByte(byte b)
{
    stack[sp++] = b;
}


byte popByte()
{
    return stack[--sp];
}


void pushInt(int x, int y)
{
    if (2 + x > STACK_ADDRESS_END) {
        Serial.println("[error]\tNo space available in memory!");
        return;
    }

    pushByte();
    pushByte(x);
}


void pushChar(char x)
{

}


void pushFloat(float x)
{

}


void pushString(char *x)
{

}