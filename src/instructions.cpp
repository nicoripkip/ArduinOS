#include "instructions.hpp"
#include "instructionset.hpp"
#include "memory.hpp"
// #include "filesystem.cpp"
#include "scheduler.hpp"
#include <EEPROM.h>


#define SPATIAL_CHARACTER 32


static char buff[10];
static byte *s_address;
static memtype_e s_type;


/**
 * Function to execute an instruction in the os
 * 
 * @param instruction
*/
uint8_t execute(byte instruction, struct task_s *task)
{
    uint8_t r = 0;
    switch (instruction)
    {
        case CHARR:
            break;
        case INTT:
            r = readDataRegion(buff, task->fp+task->pc);
            Serial.print(F("Stack pointer: "));
            Serial.println(task->sp);
            s_address = pushInt(task->stack, task->sp, atoi(buff));
            s_type = INT;
            showStack(task->stack);
            break;
        case STRINGG:
            break;
        case FLOATT:
            break;
        case SET:
            r = readDataRegion(buff, task->fp+task->pc);
            // Serial.print(F("Process id: "));
            // Serial.println(task->p_id);
            memAlloc(task->p_id, buff, 2, s_type, s_address);
            break;
        case GET:
            r = readDataRegion(buff, task->fp+task->pc);
            memtype_e v_type = memSearch(task->p_id, buff);

            Serial.print(F("Variable type: "));
            Serial.println(v_type);

            switch (v_type) 
            {
                case INT:
                    int result = popInt(task->stack, task->sp);
                    break;
                case CHAR:

                    break;
                case FLOAT:

                    break;
                case STRING:

                    break;
            }
            break;
        case INCREMENT:
            break;
        case DECREMENT:
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
            r = readDataRegion(buff, task->fp+task->pc);
            Serial.print(buff);
            break;
        case PRINTLN:
            r = readDataRegion(buff, task->fp+task->pc);
            Serial.println(buff);
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
            Serial.println(F("Execute stop"));
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