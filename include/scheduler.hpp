#pragma once


#include <Arduino.h>


#ifndef __TINBES03__ARDUINOS__SCHEDULER__
#define __TINBES03__ARDUINOS__SCHEDULER__


enum taskState_e 
{
    RUNNING,
    SUSPENDED,
    TERMINATED
};


struct task_s 
{
    uint32_t p_id;
    taskState_e state;
    char *file;
    uint16_t fp;
    uint16_t pc;
    uint16_t sp;
    byte *stack;
};


void initScheduler();
void addTask(char *file);
void suspendTask(uint16_t pid);
void removeTask(uint16_t pid);

void runningTasks();


#endif