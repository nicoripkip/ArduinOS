#include "instructions.hpp"
#include "instructionset.hpp"
#include "memory.hpp"
// #include "filesystem.cpp"
#include "scheduler.hpp"
#include <EEPROM.h>


#define SPATIAL_CHARACTER 32


static char var1[10];
static char var2[10];
static byte *s_address;
static memtype_e s_type;
static int result1;
static int result2;


/**
 * Function to execute an instruction in the os
 * 
 * @param instruction
*/
uint8_t execute(byte instruction, struct task_s *task)
{
    uint8_t x = 0;
    uint8_t y = 0;
    uint8_t r = 0;
    switch (instruction)
    {
        case CHARR:
            break;
        case INTT:
            x = readDataRegion(var1, task->fp+task->pc);
            task->pc+=x+1;
            y = readDataRegion(var2, task->fp+task->pc);
            // Serial.print(F("Stack pointer: "));
            // Serial.println(task->sp);
            result1 = atoi(var1);
            result2 = atoi(var2);
            s_type = INT;
            // Serial.print(F("Stack address: "));
            // Serial.println((uint16_t)s_address);
            // showStack(task->stack);
            s_address = pushInt(task->stack, task->sp, result2, result1);
            r = y;
            break;
        case STRINGG:
            break;
        case FLOATT:
            break;
        case SET:
            r = readDataRegion(var1, task->fp+task->pc);

            switch (s_type) 
            {
                case INT:
                    memAlloc(task->p_id, var1, 2, INT, s_address);
                    break;
                case CHAR:
                    memAlloc(task->p_id, var1, 1, CHAR, s_address);
                    break;
                case FLOAT:
                    memAlloc(task->p_id, var1, 4, FLOAT, s_address);
                    break;
                case STRING:
                    memAlloc(task->p_id, var1, 4, STRING, s_address);
                    break;
            }
            break;
        case GET:
            r = readDataRegion(var1, task->fp+task->pc);
            // Serial.print(F("Variable type: "));
            // Serial.println(s_type);

            struct memtable_s *var = memRead(task->p_id, var1);

            switch (var->type) 
            {
                case INT:
                    pushInt(task->stack, task->sp, atoi(var->value), atoi(var->value) <= 255 ? 0 : 1);
                    // showStack(task->stack);
                    // Serial.print(F("Get: "));
                    // Serial.println(result);
                    break;
                case CHAR:
                    pushChar(task->stack, task->sp, var->value[0], 0);
                    break;
                case FLOAT:

                    break;
                case STRING:

                    break;
            }

            memFree(task->p_id, var1);
            break;
        case INCREMENT:
            int varI = popInt(task->stack, task->sp);
            varI++;
            pushInt(task->stack, task->sp, varI, varI <= 255 ? 0 : 1);
            showStack(task->stack);
            break;
        case DECREMENT:
            result2--;
            break;
        case PLUS:

            break;
        case MINUS:
            break;
        case TIMES:
            break;
        case DIVIDEDBY:
            break;
        case MODULUS:
            break;
        case UNARYMINUS:
            break;
        case EQUALS:
            break;
        case NOTEQUALS:
            break;
        case LESSTHAN:
            break;
        case LESSTHANOREQUALS:
            break;
        case GREATERTHAN:
            break;
        case GREATERTHANOREQUALS:
            break;
        case LOGICALAND:
            break;
        case LOGICALOR:
            break;
        case LOGICALXOR:
            break;
        case LOGICALNOT:
            break;
        case BITWISEAND:
            break;
        case BITWISEOR:
            break;
        case BITWISEXOR:
            break;
        case BITWISENOT:
            break;
        case TOCHAR:
            break;
        case TOINT:
            break;
        case TOFLOAT:
            break;
        case ROUND:
            break;
        case FLOOR:
            break;
        case CEIL:
            break;
        case MIN:
            break;
        case MAX:
            break;
        case ABS:
            break;
        case CONSTRAIN:
            break;
        case MAP:
            break;
        case POW:
            break;
        case SQ:
            break;
        case SQRT:
            break;
        case DELAY:
            break;
        case DELAYUNTIL:
            break;
        case MILLIS:
            break;
        case PINMODE:
            break;
        case ANALOGREAD:
            break;
        case ANALOGWRITE:
            break;
        case DIGITALREAD:
            break;
        case DIGITALWRITE:
            break;
        case PRINT:
            Serial.print(result2);
            break;
        case PRINTLN:
            switch (s_type) 
            {
                case INT:
                    Serial.print(F("Incremente waarde: "));
                    Serial.println(varI);
                    int varI = popInt(task->stack, task->sp);
                    Serial.println(varI);
                    // showStack(task->stack);
                    // Serial.print(F("Get: "));
                    // Serial.println(result);
                    break;
                case CHAR:
                    char varC = popChar(task->stack, task->sp);
                    Serial.println(varC);
                    break;
                case FLOAT:
                    float varF = popFloat(task->stack, task->sp);
                    break;
                case STRING:
                    break;
            }
            break;

            break;
        case OPEN:
           break;
        case CLOSE:
            break;
        case WRITE:
            break;
        case READINT:
            break;
        case READCHAR:
            break;
        case READFLOAT:
            break;
        case READSTRING:
            break;
        case IF:
            break;
        case ELSE:
            break;
        case ENDIF:
            break;
        case WHILE:
            break;
        case ENDWHILE:
            break;
        case LOOP:
            break;
        case ENDLOOP:
            break;
        case STOP:
            Serial.println(F("[info]\tExecute stop"));
            memFree(task->p_id, task->file);
            suspendTask(task->p_id);
            return 0;
        case FORK:
            break;
        case WAITUNTILDONE:
            break;
        default:
            Serial.print(F("[error]\tOS cant understand instruction: "));
            Serial.println(instruction);
            return 0;
    }

    return r+1;
}