#include <Arduino.h>
#include "memory.hpp"


// This represents the data components of the chunk blocks
// the data component of the chunk block is 8 bit,
// so the max memory allocated by the os is 128 blocks or
// 128 * 8 bits or 128 bytes
#define MAX_MEMORY_SIZE         256
#define PHYS_INT_MEM_START      0X0000
#define PHYS_INT_MEM_END        0X21FF
#define PHYS_EXT_MEM_START      0x2200 // 0x0000 - startvalue internal sram
#define PHYS_EXT_MEM_END        0xA1FF // 0x21FF - endvalue internal sram

static struct memtable_s memtable[MAX_MEMORY_SIZE];
static struct memchunk_s freetable[50];
static uint32_t fp = 0;


void memInit()
{
    int i;

    for (i = 0; i < MAX_MEMORY_SIZE; i++) {
        struct memtable_s block;
        block.virtaddr = i;
        block.state = FREE;
        block.type  = VOID;

        memtable[i] = block;
    }

    for (i = PHYS_INT_MEM_START; i < PHYS_INT_MEM_START+100; i++) {
        if (memRead(i) == 0) {
            struct memchunk_s block;
            block.addr = i;
            block.value = memRead(i);
            fp++;
        }
    }
}


int memRead(uint64_t address)
{
    return *(uint64_t *)address;
}


void memWrite(uint64_t address, int value)
{
    *(volatile uint16_t *)address = value;
}


void showMemTable()
{
    Serial.println("address\t\t|\tstate\t\t|\ttype");

    for (int i = 0; i < MAX_MEMORY_SIZE; i++) {
        Serial.print(memtable[i].virtaddr);
        Serial.print("\t\t|\t");
        if (memtable[i].state == 0) {
            Serial.print("OCCUPIED");
        } else {
            Serial.print("FREE");
        }
        Serial.print("\t\t|\t");
        Serial.println(memtable[i].type);
    }
}


void showPhysMem()
{
    // for (long i = PHYS_MEM_START+10; i < PHYS_MEM_START+30; i++) {
    //     memWrite(i, 20);
    // }

    long i;
    for (i = PHYS_INT_MEM_START; i < PHYS_INT_MEM_START+100; i++) {
        Serial.print("[info]\tMem address: ");
        Serial.print(i, HEX);
        Serial.print(" with data: ");
        Serial.println(memRead(i));
    }
}


void showFreeTable()
{
    for (int i = 0; i < fp; i++) {
        Serial.print("[info]\tMem address: ");
        Serial.print(freetable[i].addr, HEX);
        Serial.print(" with data:  ");
        Serial.println(freetable[i].value);
    }
}