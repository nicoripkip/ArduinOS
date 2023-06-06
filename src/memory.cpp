#include <Arduino.h>
#include "memory.hpp"


// This represents the data components of the chunk blocks
// the data component of the chunk block is 8 bit,
// so the max memory allocated by the os is 128 blocks or
// 128 * 8 bits or 128 bytes
#define MAX_MEMORY_SIZE         25
#define PHYS_INT_MEM_START      0x0100// 0X0000
#define PHYS_INT_MEM_END        0x08FF// 0X21FF
#define PHYS_EXT_MEM_START      0x2200 // 0x0000 - startvalue internal sram
#define PHYS_EXT_MEM_END        0xA1FF // 0x21FF - endvalue internal sram

#define SYS_INT_MEM_START       0x0316
#define SYS_INT_MEM_END         0x0396
#define RESERVED_MEM_RANGE      128

static struct memtable_s memtable[MAX_MEMORY_SIZE];
static long freetable[30][2];
static uint32_t fp = 0;
static uint32_t tp = 0;
static size_t totalMemorySpace = 0;
static long addr = 0;


/**
 * Setup everything to get the mmu started
 * 
*/
void memInit()
{
    for (short i = 0; i < MAX_MEMORY_SIZE; i++) {
        memtable[i].virtaddr    = 0;
        memtable[i].state       = FREE;
        memtable[i].type        = VOID;
        memtable[i].length      = 0; 
    }

    allocFree();
}


/**
 * Function to read data from a given address from the sram
 * 
 * @param address
 * @return int
*/
int memRead(uint64_t address)
{
    return *(uint64_t *)address;
}


/**
 * Function to write a given number to a given address on the sram
 * 
 * @param address
 * @param value
*/
void memWrite(uint64_t address, int value)
{
    *(volatile uint16_t *)address = value;
}


/**
 * Function to check if a memoryblock is reserved
 * 
 * @param address
 * @return bool
*/
bool memOccupied(long address)
{
    for (short i = 0; i < 25; i++) {
        if (memtable[i].state == OCCUPIED && 
            address >= memtable[i].virtaddr && 
            address <= memtable[i].virtaddr+memtable[i].length
        ) {
            return true;
        }
    }

    return false;
}


/**
 * Function to construct the free memory table
 * 
*/
void allocFree()
{
    int i = SYS_INT_MEM_START;

    while (i < SYS_INT_MEM_END) {
        if (memRead(i) == 0 && !memOccupied(i)) {
            int c = 0;
            
            freetable[fp][0] = i;
            while (memRead(i+c) == 0) {
                c++;
            }

            i+=c;
            freetable[fp][1] = c+1;

            fp++;
        }

        i++;
    }
}


/**
 * Function to allocate a given size in memory
 * 
 * @param size
*/
void memAlloc(size_t size)
{
    if ((size + totalMemorySpace) > RESERVED_MEM_RANGE) {
        return;
    }

    totalMemorySpace+=size;

    addr = 0;
    for (short i = 0; i < 30; i++) {
        if (size == freetable[i][1]) { 
            addr = freetable[i][0];

            freetable[i][0] = 0;
            freetable[i][1] = 0;

            break;
        } else if (size > freetable[i][1]) {
            addr = freetable[i][0];

            Serial.println(freetable[i][0]);

            freetable[i][0] = freetable[i][0]+size;
            freetable[i][1] = freetable[i][1]-size;

            break;
        }
    }

    if (addr == 0) {
        return;
    }

    Serial.print("[mem]\tGevonden adres: ");
    Serial.print(addr);
    Serial.print(" met grootte: ");
    Serial.println(size);

    memtable[tp].virtaddr = addr;
    memtable[tp].state = OCCUPIED;
    memtable[tp].length = size;
    tp++;
}


/**
 * Function to show the memory table
 * 
*/
void showMemTable()
{
    Serial.println("address\t\t|\tstate\t\t|\ttype\t\t|\tsize");

    for (int i = 0; i < MAX_MEMORY_SIZE; i++) {
        Serial.print(memtable[i].virtaddr, HEX);
        Serial.print("\t\t|\t");
        if (memtable[i].state == 0) {
            Serial.print("OCCUPIED");
        } else {
            Serial.print("FREE\t");
        }
        Serial.print("\t|\t");
        Serial.print(memtable[i].type);
        Serial.print("\t\t|\t");
        Serial.println(memtable[i].length);
    }
}


void showPhysMem()
{
    // for (long i = PHYS_MEM_START+10; i < PHYS_MEM_START+30; i++) {
    //     memWrite(i, 20);
    // }

    long i;
    for (i = PHYS_INT_MEM_START; i < PHYS_INT_MEM_START+1700; i++) {
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
        Serial.print(freetable[i][0], HEX);
        Serial.print( "\t\t| size after: ");
        Serial.println(freetable[i][1]);
    }
}