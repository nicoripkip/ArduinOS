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


enum memstate_e
{
    OCCUPIED,
    FREE
};


/**
 * Struct of memory chuch. Every data is a byte in a memory chunk
*/
struct memtable_s 
{
    long            virtaddr;
    char *          name;
    memtype_e       type;
    memstate_e      state;
    uint32_t        p_id;
};


void pushInt(int x);
void pushChar(char x);
void pushFloat(float x);
void pushString(char *x);

int popInt();


void memAlloc(uint16_t pid, char *name, void *data, size_t size, memtype_e type);
void memFree(uint16_t pid, char *name);



#endif