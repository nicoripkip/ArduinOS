#include "instructions.hpp"
#include "instructionset.hpp"
#include "memory.hpp"
#include "scheduler.hpp"


#define SPATIAL_CHARACTER 32


static char pbuffer[10];       // value buffer 1      // value buffer 2
static byte *s_address;
static memtype_e s_type;

// Define value registers to hold retrieved data
static uint8_t r1;
static uint8_t r2;
static uint8_t r3;
static uint8_t r4;


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

    Serial.print(F("Instruction run: "));
    Serial.println(instruction);

    switch (instruction)
    {
        case CHARR:
            task->pc+=1;

            readDataRegion(pbuffer, task->fp+task->pc);
            task->pc+=2;
      
            pushChar(task->stack, task->sp, pbuffer[0]);
            break;
        case INTT:
            task->pc+=1;

            readDataRegion(pbuffer, task->fp+task->pc);
            r1 = atoi(pbuffer);
            task->pc+=2;

            readDataRegion(pbuffer, task->fp+task->pc);
            r2 = atoi(pbuffer);
            task->pc+=2;

            pushInt(task->stack, task->sp, r2, r1);    
            showStack(task->stack);
            break;
        case STRINGG:
            task->pc+=1;
            char tmp[10];
            memset(tmp, '\0', 10);

            while ((int)pbuffer[0] != 48) {
                readDataRegion(pbuffer, task->fp+task->pc);
                task->pc+=2;

                if ((int)pbuffer[0] != 48) {
                    strncat(tmp, &pbuffer[0], 1);
                }
            }

            pushString(task->stack, task->sp, tmp);
            showStack(task->stack);
            break;
        case FLOATT:
            task->pc+=1;
            task->pc+=readDataRegion(pbuffer, task->fp+task->pc);
            r1 = atoi(pbuffer);
            task->pc++;

            task->pc+=readDataRegion(pbuffer, task->fp+task->pc);
            r2 = atoi(pbuffer);
            task->pc++;

            task->pc+=readDataRegion(pbuffer, task->fp+task->pc);
            r3 = atoi(pbuffer);
            task->pc++;

            task->pc+=readDataRegion(pbuffer, task->fp+task->pc);
            r4 = atoi(pbuffer);
            task->pc++;

            pushFloat(task->stack, task->sp, r1, r2, r3, r4);
            showStack(task->stack);
            break;
        case SET:
            task->pc+=2;
            readDataRegion(pbuffer, task->fp+task->pc);
            task->pc+=2;

            switch (s_type) 
            {
                case INT:
                    int v = popInt(task->stack, task->sp);
                    memAlloc(task->p_id, pbuffer, 2, INT, s_address);
                    break;
                case CHAR:
                    memAlloc(task->p_id, pbuffer, 1, CHAR, s_address);
                    break;
                case FLOAT:
                    memAlloc(task->p_id, pbuffer, 4, FLOAT, s_address);
                    break;
                case STRING:
                    memAlloc(task->p_id, pbuffer, 4, STRING, s_address);
                    break;
            }
            break;
        case GET:
            r = readDataRegion(pbuffer, task->fp+task->pc);
            // Serial.print(F("Variable type: "));
            // Serial.println(s_type);
            
            /*
            struct memtable_s *var = memRead(task->p_id, var1);

            switch (var->type) 
            {
                case INT:
                    // pushInt(task->stack, task->sp, atoi(var->value), atoi(var->value) <= 255 ? 0 : 1);
                    // showStack(task->stack);
                    // Serial.print(F("Get: "));
                    // Serial.println(result);
                    break;
                case CHAR:
                    // pushChar(task->stack, task->sp, var->value[0], 0);
                    break;
                case FLOAT:

                    break;
                case STRING:

                    break;
            }

            memFree(task->p_id, var1);
            */
            break;
        case INCREMENT:
            // int varI = popInt(task->stack, task->sp);
            // varI++;
            // pushInt(task->stack, task->sp, varI, varI <= 255 ? 0 : 1);
            showStack(task->stack);
            break;
        case DECREMENT:
            r2--;
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
            Serial.print(r2);
            break;
        case PRINTLN:
            s_type = popByte(task->stack, task->sp);

            Serial.print(F("Datatype: "));
            Serial.println(s_type);

            switch (s_type) 
            {
                case INT:
                    // Serial.println(varI);
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

            task->pc+=1;

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
            // Serial.println(F("[info] Execute stop"));
            // memFree(task->p_id, task->file);
            suspendTask(task->p_id);
            r = 0;
            break;
        case FORK:
            break;
        case WAITUNTILDONE:
            break;
        default:
            Serial.print(F("[error]\tOS cant understand instruction: "));
            Serial.println(instruction);
            r = 0;
    }

    return r+1;
}
