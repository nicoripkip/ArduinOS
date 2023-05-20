#include <Arduino.h>
#include "memory.hpp"


// This represents the data components of the chunk blocks
// the data component of the chunk block is 8 bit,
// so the max memory allocated by the os is 128 blocks or
// 128 * 8 bits or 128 bytes
#define MAX_MEMORY_SIZE 128 


static struct memchunk_s stack[MAX_MEMORY_SIZE];


void memInit()
{
    int i;

    for (i = 0; i < MAX_MEMORY_SIZE; i++) {
        struct memchunk_s block;
        block.addr = i;
        block.state = FREE;

        stack[i] = block;
    }
}


void showMemTable()
{
    Serial.println("address\t\t|\tstate");

    for (int i = 0; i < MAX_MEMORY_SIZE; i++) {
        Serial.print(stack[i].addr);
        Serial.print("\t\t|\t");
        if (stack[i].state == 0) {
            Serial.println("OCCUPIED");
        } else {
            Serial.println("FREE");
        }
        
    }
}