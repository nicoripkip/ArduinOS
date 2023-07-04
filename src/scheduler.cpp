#include <Arduino.h>
#include "scheduler.hpp"
#include "filesystem.hpp"
#include "memory.hpp"


#define MAX_TASKS       25


static struct task_s schedulerTable[MAX_TASKS];
static uint16_t sp = 0;


/**
 * Function to setup the scheduler
 * 
*/
void initScheduler()
{
    for (uint8_t i = 0; i < MAX_TASKS; i++) {
        schedulerTable[i].p_id  = 0;
        schedulerTable[i].state = TERMINATED;
        schedulerTable[i].pc    = 0;
    }
}


/**
 * Function to add a task to the scheduler
 *
 * @param name
*/
void addTask(char *file)
{
    for (uint8_t i =0; i < MAX_TASKS; i++) {
        if (schedulerTable[i].state == TERMINATED && schedulerTable[i].p_id == 0) {
            schedulerTable[i].p_id = sp;
            memcpy(schedulerTable[i].file, file, 12);
            schedulerTable[i].fp = getFileAddress(file);
            schedulerTable[i].state = RUNNING;
            schedulerTable[i].pc = 0;
            schedulerTable[i].sp = 0;
            schedulerTable[i].stack = stackAlloc(30);

            sp++;

            Serial.print(F("[info]\tProcess: "));
            Serial.print(sp);
            Serial.println(F(" succesfully created!"));

            return;
        }
    }
}


/**
 * Function to resume a task in the scheduler
 * 
 * @param pid
*/
void resumeTask(uint8_t pid)
{
    for (uint8_t i = 0; i < MAX_TASKS; i++) {
        if (schedulerTable[i].p_id == pid) {
            schedulerTable[i].state = RUNNING;
            Serial.println(F("[info]\tProcess succesfully resumed!"));
            return;
        }
    }

    Serial.println(F("[error]\tProcess not found in the scheduling table!"));
}


/**
 * Function to suspend a task in 
 * 
 * @param pid
*/
void suspendTask(uint8_t pid)
{
    for (uint8_t i = 0; i < MAX_TASKS; i++) {
        if (schedulerTable[i].p_id == pid) {
            schedulerTable[i].state = SUSPENDED;
            Serial.println(F("[info]\tProcess succesfully suspended!"));
            return;
        }
    }

    Serial.println(F("[error]\tProcess not found in the scheduling table!"));
}


/**
 * Function to remove a task from the scheduler
 * 
 * @param pid
*/
void removeTask(uint8_t pid)
{
    for (uint8_t i = 0; i < MAX_TASKS; i++) {
        if (schedulerTable[i].p_id == pid) {
            schedulerTable[i].p_id = 0;
            schedulerTable[i].state = TERMINATED;
            // memset(schedulerTable[i].file, 0, 12);

            for (uint16_t j = 0; j < 10; j++) {
                schedulerTable[i].stack[j] = 0;
            }

            schedulerTable[i].stack = nullptr;

            sp--;
            Serial.println(F("[info]\tProcess successfully terminated!"));
            return;
        }
    }

    Serial.println(F("[error]\tProcess not found in the scheduling table!"));
}


/**
 * Function to display all running tasks
 * 
 * 
*/
void runningTasks()
{
    for (uint16_t i = 0; i < MAX_TASKS; i++)
    {  
        if (schedulerTable[i].state == RUNNING) {
            Serial.print(F("process id: "));
            Serial.print(schedulerTable[i].p_id);
            Serial.print(F(" name:  "));
            Serial.print(schedulerTable[i].file);
            Serial.print(F(" state:  "));
            Serial.print(schedulerTable[i].state);
            Serial.print(F(" file address: "));
            Serial.print(schedulerTable[i].fp);
            Serial.print(F(" stack pointer: "));
            Serial.println((uint8_t)schedulerTable[i].stack);
        }
    }
}


/**
 * 
 * 
*/
void runTasks()
{
    for (uint8_t i = 0; i < MAX_TASKS; i++) {
        if (schedulerTable[i].state == RUNNING) {
            schedulerTable[i].fp++;
        }
    }
}