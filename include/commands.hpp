#pragma once


#include <Arduino.h>


#ifndef __TINBES03__ARDUINOS__COMMANDS__
#define __TINBES03__ARDUINOS__COMMANDS__


#define MAX_COMMANDS 10


/**
 * 
 **/
struct s_command
{
    String command;
    void (*com_func)(String*, int);
};


void readInput();
void sendOutput();


void print(String *arg, int argCount);
void listmemtable(String *arg, int argCount);
void listphysmem(String *arg, int argCount);
void listfreemem(String *arg, int argCount);
void neofetch();


void add_instruction();
void delete_instruction();
void update_instruction();

static struct s_command commands[MAX_COMMANDS] = {
    {"print", &print},
    {"listmemtable", &listmemtable},
    {"listphysmem", &listphysmem},
    {"listfreemem", &listfreemem},
    {"neofetch", &neofetch}
};

#endif