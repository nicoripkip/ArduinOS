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
    FREE,
    OCCUPIED
};


/**
 * Struct of memory chuch. Every data is a byte in a memory chunk
 * 
*/
struct memtable_s 
{
    byte            *address;
    char            name[5];
    memtype_e       type;
    memstate_e      state;
    uint8_t         p_id;
};


void initMemory();

byte *pushInt(byte *address, uint8_t &sp, int x);
byte *pushChar(byte *address, uint8_t &sp, char x);
byte *pushFloat(byte *address, uint8_t &sp, float x);
byte *pushString(byte *address, uint8_t &sp, char *x);

int popInt();
char popChar();
float popFloat();
char *popString();


void memAlloc(uint8_t pid, char *name, size_t size, memtype_e type, byte *address);
void memFree(uint8_t pid, char *name);
byte *stackAlloc(size_t size);
uint8_t readDataRegion(char *buffer, uint16_t address);

void showStack(byte *address);
void showMemTable();



#endif