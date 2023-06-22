#pragma once


#include <Arduino.h>


#ifndef __TINBES03__ARDUINOS__FILESYSTEM__
#define __TINBES03__ARDUINOS__FILESYSTEM__


void initFileSystem();
void writeFATEntry(char *file, size_t size, char *data);
int readFATEntry(uint32_t address);
char *retrieveFATEntry(char *file);
uint32_t totalFilesInFAT();
void allFilesOnFAT();

void showFT();
void jemoeder();
void eraseFATEntry(char *file);
void eraseAll();


#endif