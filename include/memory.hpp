#pragma once


#ifndef __TINBES03__ARDUINOS__MEMORY__
#define __TINBES03__ARDUINOS__MEMORY__


enum memtype_e
{
    VOID,
    FLOAT,
    INT,
    STRING,
    CHAR
};


/**
 * 
 * 
*/
enum memstate_e
{
    OCCUPIED,
    FREE
};


/**
 * Struct of memory chuch. Every data is a byte in a memory chunk
 * 
*/
struct memtable_s 
{
    byte            *address;
    char            *name;
    memtype_e       type;
    memstate_e      state;
    uint8_t         p_id;
};


byte *pushInt(int x);
byte *pushChar(char x);
byte *pushFloat(float x);
byte *pushString(char *x);

int popInt();


void memAlloc(uint8_t pid, char *name, size_t size, memtype_e type, uint16_t address);
void memFree(uint8_t pid, char *name);
void memWrite(uint16_t address, byte value);
byte memRead(uint16_t address);
byte *stackAlloc(size_t size);
uint8_t readDataRegion(char *buffer, uint16_t address);

void showStack(byte *address);



#endif