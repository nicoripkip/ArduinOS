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
    FLOAT,
    INT,
    STRING,
    CHAR
};


/**
 * Struct of memory chuch. Every data is a byte in a memory chunk
*/
struct memtable_s 
{
    long            virtaddr;
    memstate_e      state;
    memtype_e       type;
    size_t          length;
    uint32_t        p_id;
};


void pushInt(int x, int y);
void pushChar(char x);
void pushFloat(float x);
void pushString(char *x);


void addToMemTable();
void removeFromMemTable();



#endif