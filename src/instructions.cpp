#include "instructions.hpp"
#include "instructionset.hpp"
#include "memory.hpp"
// #include "filesystem.cpp"
#include "scheduler.hpp"
#include <EEPROM.h>


#define SPATIAL_CHARACTER 32


static char buff[10];
static byte *s_address;


/**
 * 
 * 
 * @param instruction
*/
uint8_t execute(byte instruction, struct task_s *task)
{

    Serial.print("Instruction: ");
    Serial.println(instruction);

    uint8_t r = 0;
    switch (instruction)
    {
        case CHARR:
            break;
        case INTT:
            r = readDataRegion(buff, task->fp+task->pc);
            Serial.print("Data: ");
            Serial.println(buff);
            s_address = pushInt(atoi(buff));
            break;
        case STRINGG:
            break;
        case FLOATT:
            break;
        case SET:
            Serial.println("Yo man");
            r = readDataRegion(buff, task->fp+task->pc);
            Serial.print("Data: ");
            Serial.println(buff);
            // memAlloc(task->p_id, buff, 2, INT, task->stack);
            break;
        case GET:

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
        // case UNARYMINUS:
        //     break;
        // case EQUALS:
        //     break;
        // case NOTEQUALS:
        //     break;
        // case LESSTHAN:
        //     break;
        // case LESSTHANOREQUALS:
        //     break;
        // case GREATERTHAN:
        //     break;
        // case GREATERTHANOREQUALS:
        //     break;
        // case LOGICALAND:
        //     break;
        // case LOGICALOR:
        //     break;
        // case LOGICALXOR:
        //     break;
        // case LOGICALNOT:
        //     break;
        // case BITWISEAND:
        //     break;
        // case BITWISEOR:
        //     break;
        // case BITWISEXOR:
        //     break;
        // case BITWISENOT:
        //     break;
        // case TOCHAR:
        //     break;
        // case TOINT:
        //     break;
        // case TOFLOAT:
        //     break;
        // case ROUND:
        //     break;
        // case FLOOR:
        //     break;
        // case CEIL:
        //     break;
        // case MIN:
        //     break;
        // case MAX:
        //     break;
        // case ABS:
        //     break;
        // case CONSTRAIN:
        //     break;
        // case MAP:
        //     break;
        // case POW:
        //     break;
        // case SQ:
        //     break;
        // case SQRT:
        //     break;
        // case DELAY:
        //     break;
        // case DELAYUNTIL:
        //     break;
        // case MILLIS:
        //     break;
        // case PINMODE:
        //     break;
        // case ANALOGREAD:
        //     break;
        // case ANALOGWRITE:
        //     break;
        // case DIGITALREAD:
        //     break;
        // case DIGITALWRITE:
        //     break;
        // case PRINT:
        //     break;
        // case PRINTLN:
        //     break;
        // case OPEN:
        //    break;
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
            removeTask(task->p_id);
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

    free(task);
    return r+1;
}