#pragma once


#include <Arduino.h>


#ifndef __TINBES03__ARDUINOS__COMMANDS__
#define __TINBES03__ARDUINOS__COMMANDS__


#define MAX_COMMANDS    12
#define BUFFER_SIZE     12


/**
 * 
 **/
struct s_command
{
    char command[BUFFER_SIZE];
    void (*com_func)();
};


void readInput();


void help();
void print(String *arg, int argCount);
void listmemtable(String *arg, int argCount);
void listphysmem(String *arg, int argCount);
void listfreemem(String *arg, int argCount);
void neofetch();


void add_instruction();
void delete_instruction();
void update_instruction();

// Commands for the filesystem
void files();
void store();
void retrieve();
void erase();
void freespace();

// Commands for the processes
void run();
void list();
void suspend();
void resume();
void kill();


static struct s_command command_table[MAX_COMMANDS] = {
    {"help", &help},
    // {"print", &print},
    // {"listmemtable", &listmemtable},
    // {"listphysmem", &listphysmem},
    // {"listfreemem", &listfreemem},
    {"neofetch",    &neofetch},
    {"FILES",       &files},
    {"STORE",       &store},
    {"RETRIEVE",    &retrieve},
    {"ERASE",       &erase},
    {"FREESPACE",   &freespace},
    {"RUN",         &run},
    {"LIST",        &list},
    {"SUSPEND",     &suspend},
    {"RESUME",      &resume},
    {"KILL",        &kill}
};

#endif