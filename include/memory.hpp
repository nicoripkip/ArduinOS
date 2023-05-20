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

};


/**
 * Struct of memory chuch. Every data is a byte in a memory chunk
*/
struct memchunk_s 
{
    unsigned long addr;
    memstate_e state;
};


void memInit();
void stack_alloc();
void stack_free();

void showMemTable();

#endif