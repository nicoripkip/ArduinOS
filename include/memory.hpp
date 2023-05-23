#pragma once


#ifndef __TINBES03__ARDUINOS__MEMORY__
#define __TINBES03__ARDUINOS__MEMORY__


enum memstate_e
{
    OCCUPIED    = 0,
    FREE        = 1
};


enum memtype_e
{
    VOID,
    CHAR,
    INT,
    SHORT,
    LONG,
    DOUBLE,
    FLOAT
};


/**
 * Struct of memory chuch. Every data is a byte in a memory chunk
*/
struct memtable_s 
{
    unsigned long   virtaddr;
    memstate_e      state;
    memtype_e       type;
    size_t          lenght;
    uint32_t        p_id;
};


struct memchunk_s
{
    long addr;
    int value;
};


void memInit();
void memAlloc(size_t size);
void memFree();

void allocFree();
void rallocFree();

int memRead(uint64_t address);
void memWrite(uint64_t address, int value);

void showMemTable();
void showPhysMem();
void showFreeTable();

#endif