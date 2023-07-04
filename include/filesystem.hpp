#pragma once


#include <Arduino.h>


#ifndef __TINBES03__ARDUINOS__FILESYSTEM__
#define __TINBES03__ARDUINOS__FILESYSTEM__


struct Entry_s
{
    char filename[12];
    char contents[60];
    size_t size;
};


void initFileSystem();
void writeFATEntry(char *file, size_t size, char *contents);
char *retrieveFATEntry(char *file);
uint32_t totalFilesInFAT();
void allFilesOnFAT();
uint16_t getFileAddress(char *file);
void showFT();
void eraseFATEntry(char *file);
void eraseAll();


#endif