#pragma once


#include <Arduino.h>


#ifndef __TINBES03__ARDUINOS__COMMANDS__
#define __TINBES03__ARDUINOS__COMMANDS__


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


void add_instruction();
void delete_instruction();
void update_instruction();

static struct s_command commands[] = {
    {"print", &print}
};

#endif