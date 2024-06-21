#pragma once


#ifndef __TINBES03__ARDUINOS__MEMORY__
#define __TINBES03__ARDUINOS__MEMORY__


/**
 * Enum for the datatypes of the variables
 * 
*/
enum memtype_e
{
    VOID,
    FLOAT,
    INT,
    STRING,
    CHAR
};


/**
 * Enum for the state of the variables
 * 
*/
enum memstate_e
{
    FREE,
    OCCUPIED
};


/**
 * Struct of memory chuch. Every data is a byte in a memory chunk
 * 
*/
struct memtable_s 
{
    uint8_t         pid;
    char            name[5];
    memtype_e       type;
    memstate_e      state;
    unsigned char   *address;
};


void initMemory();

byte *pushInt(byte *address, uint8_t &sp, int x, int y);
byte *pushChar(byte *address, uint8_t &sp, char x, int y);
byte *pushFloat(byte *address, uint8_t &sp, float x);
byte *pushString(byte *address, uint8_t &sp, char *x, int z);

int popInt(byte *address, uint8_t &sp);
char popChar(byte *address, uint8_t &sp);
float popFloat(byte *address, uint8_t &sp);
char *popString(byte *address, uint8_t &sp);


void memAlloc(uint8_t pid, char *name, size_t size, memtype_e type, byte *address);
memtable_s *memRead(uint8_t pid, char *name);
void memFree(uint8_t pid, char *name);
byte *stackAlloc(size_t size);
uint8_t readDataRegion(char *buffer, uint16_t address);

void showStack(byte *address);
void showMemTable();


#endif
