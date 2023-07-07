#pragma once


#include <Arduino.h>


#ifndef __TINBES03__ARDUINOS__FILESYSTEM__
#define __TINBES03__ARDUINOS__FILESYSTEM__


struct Entry_s
{
    char        filename[12];           // The name of the file stored in memory
    uint16_t    contents;               // The pointer to the file contents
    size_t      size;                   // The total size of the file
};


void initFileSystem();
void writeFATEntry(char *file, size_t size, char *contents);
char *retrieveFATEntry(char *file);
uint32_t totalFilesInFAT();
void allFilesOnFAT();
uint16_t getFileAddress(char *file);
size_t getFileSize(char *file);
void showFT();
void eraseFATEntry(char *file);
void eraseAll();
uint16_t showFreeSpace();


#endif