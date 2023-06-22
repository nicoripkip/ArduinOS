#include <Arduino.h>
#include "scheduler.hpp"
#include "filesystem.hpp"


#define MAX_TASKS       25


static struct task_s schedulerTable[MAX_TASKS];
static uint16_t sp = 0;


/**
 * Function to setup the scheduler
 * 
*/
void initScheduler()
{
    for (uint16_t i = 0; i < MAX_TASKS; i++) {
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
    for (uint16_t i =0; i < MAX_TASKS; i++) {
        if (schedulerTable[i].state == TERMINATED && schedulerTable[i].p_id == 0) {
            schedulerTable[i].p_id = sp;
            schedulerTable[i].file = file;
            schedulerTable[i].fp = getFileAddress(file);
            schedulerTable[i].state = RUNNING;
            schedulerTable[i].pc = 0;
            schedulerTable[i].sp = 0;
            schedulerTable[i].stack = nullptr;

            sp++;

            Serial.print("[info]\tProcess: ");
            Serial.print(sp);
            Serial.println(" succesfully created!");

            return;
        }
    }
}


/**
 * Function to resume a task in the scheduler
 * 
 * @param pid
*/
void resumeTask(uint16_t pid)
{
    for (uint16_t i = 0; i < MAX_TASKS; i++) {
        if (schedulerTable[i].p_id == pid) {
            schedulerTable[i].state = RUNNING;
            Serial.println("[info]\tProcess succesfully resumed!");
            return;
        }
    }

    Serial.println("[error]\tProcess not found in the scheduling table!");
}


/**
 * Function to suspend a task in 
 * 
 * @param pid
*/
void suspendTask(uint16_t pid)
{
    for (uint16_t i = 0; i < MAX_TASKS; i++) {
        if (schedulerTable[i].p_id == pid) {
            schedulerTable[i].state = SUSPENDED;
            Serial.println("[info]\tProcess succesfully suspended!");
            return;
        }
    }

    Serial.println("[error]\tProcess not found in the scheduling table!");
}


/**
 * Function to remove a task from the scheduler
 * 
 * @param pid
*/
void removeTask(uint16_t pid)
{
    for (uint16_t i = 0; i < MAX_TASKS; i++) {
        if (schedulerTable[i].p_id == pid) {
            schedulerTable[i].p_id = 0;
            schedulerTable[i].state = TERMINATED;
            schedulerTable[i].file = nullptr;

            for (uint16_t j = 0; j < 10; j++) {
                schedulerTable[i].stack[j] = 0;
            }

            schedulerTable[i].stack = nullptr;

            sp--;
            Serial.println("[info]\tProcess successfully terminated!");
        }
    }

    Serial.println("[error]\tProcess not found in the scheduling table!");
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
            Serial.print("process id: ");
            Serial.print(schedulerTable[i].p_id);
            Serial.print(" name:  ");
            Serial.print(schedulerTable[i].file);
            Serial.print(" state:  ");
            Serial.println(schedulerTable[i].state);
        }
    }
}