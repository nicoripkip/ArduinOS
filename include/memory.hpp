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
    uint16_t        *address;
    char *          name;
    memtype_e       type;
    memstate_e      state;
    uint8_t         p_id;
};


uint16_t pushInt(int x);
uint16_t pushChar(char x);
uint16_t pushFloat(float x);
uint16_t pushString(char *x);

int popInt();


void memAlloc(uint8_t pid, char *name, size_t size, memtype_e type, uint16_t address);
void memFree(uint8_t pid, char *name);
void memWrite(uint16_t address, byte value);
byte memRead(uint16_t address);
byte *stackAlloc(size_t size);



#endif