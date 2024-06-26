#include <Arduino.h>
#include "scheduler.hpp"
#include "filesystem.hpp"
#include "memory.hpp"
#include "instructions.hpp"


#define MAX_TASKS       10


static struct task_s schedulerTable[MAX_TASKS];
static uint16_t process_counter = 1;


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
    // TODO: Create function in filesystem.cpp to check if file exist
    if (strcmp(file, "") == 0) {
        Serial.println(F("[error] File doesn't exist!"));
        return;
    }

    for (uint8_t i =0; i < MAX_TASKS; i++) {
        if (schedulerTable[i].state == TERMINATED && schedulerTable[i].p_id == 0) {
            schedulerTable[i].p_id = process_counter;
            memcpy(schedulerTable[i].file, file, 12);
            schedulerTable[i].fp = getFileAddress(file);
            schedulerTable[i].ps = getFileSize(file);
            schedulerTable[i].state = RUNNING;
            schedulerTable[i].pc = 0;
            schedulerTable[i].sp = 0;
            byte* t = stackAlloc(32);
            schedulerTable[i].stack = t;
            memset(schedulerTable[i].stack, 0, 32);

            process_counter++;

            Serial.print(F("[info] Process: "));
            Serial.print(process_counter);
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
            Serial.println(F("[info] Process succesfully resumed!"));
            return;
        }
    }

    Serial.println(F("[error] Process not found in the scheduling table!"));
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
            Serial.println(F("[info] Process succesfully suspended!"));
            return;
        }
    }

    Serial.println(F("[error] Process not found in the scheduling table!"));
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
            memset(schedulerTable[i].file, 0, 12);

            schedulerTable[i].stack = stackFree(schedulerTable[i].stack, 32);

            process_counter--;
            Serial.println(F("[info] Process successfully terminated!"));
            return;
        }
    }

    Serial.println(F("[error] Process not found in the scheduling table!"));
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
        // if (schedulerTable[i].state == RUNNING) {
        Serial.print(F("process id: "));
        Serial.print(schedulerTable[i].p_id);
        Serial.print(F(" | name: "));
        Serial.print(schedulerTable[i].file);
        Serial.print(F(" | state: "));
        switch (schedulerTable[i].state)
        {
            case RUNNING:
                Serial.print(F("RUNNING"));
                break;
            case SUSPENDED:
                Serial.print(F("SUSPENDED"));
                break;
            case TERMINATED:
                Serial.print(F("TERMINATED"));
                break;
        }
        Serial.print(F(" | file address: "));
        Serial.print(schedulerTable[i].fp);
        Serial.print(F(" | stack address: "));
        Serial.println((uint16_t)schedulerTable[i].stack);
        // }
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
            char b[10];
            int r = readDataRegion(b, schedulerTable[i].fp+schedulerTable[i].pc);
            
            /*
            Serial.print(F("Program counter: "));
            Serial.println(schedulerTable[i].pc);
 
            Serial.print(F("Program size: "));
            Serial.println(schedulerTable[i].ps);

            Serial.print(F("Instruction buffer: "));
            Serial.println(b);
            */
            schedulerTable[i].pc += r;

            execute(atoi(b), &schedulerTable[i]);
        }
    }
}
