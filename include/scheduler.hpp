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
    uint8_t p_id;
    taskState_e state;
    char file[12];
    size_t ps;
    uint16_t fp;
    uint16_t pc;
    uint8_t sp;
    byte *stack;
};


void initScheduler();
void addTask(char *file);
void resumeTask(uint8_t pid);
void suspendTask(uint8_t pid);
void removeTask(uint8_t pid);

void runningTasks();
void runTasks();

#endif