#include "instructions.hpp"
#include "instructionset.hpp"
#include "memory.hpp"
#include "scheduler.hpp"


#define SPATIAL_CHARACTER 32


static char pbuffer[10];       // value buffer 1      // value buffer 2
static char strbuffer[10];
static byte *s_address;
static memtype_e s_type;
static uint8_t s;
static struct memtable_s *var;

// Define value registers to hold retrieved data
static uint8_t r1;
static uint8_t r2; static uint8_t r3;
static uint8_t r4;


static int i1;
static int i2;
static float f1;
static float f2;


/**
 * Function to execute an instruction in the os
 * 
 * @param instruction
*/
uint8_t execute(byte instruction, struct task_s *task)
{
    uint8_t r = 0;

    // Serial.print(F("Instruction run: "));
    // Serial.println(instruction);

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

            // showStack(task->stack);
            break;
        case STRINGG:
            task->pc+=1;
            memset(strbuffer, '\0', 10);

            while ((int)pbuffer[0] != 48) {
                readDataRegion(pbuffer, task->fp+task->pc);
                task->pc+=2;

                if ((int)pbuffer[0] != 48) {
                    strncat(strbuffer, &pbuffer[0], 1);
                }
            }

            pushString(task->stack, task->sp, strbuffer);
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
            break;
        case SET:
            task->pc+=1;
            readDataRegion(pbuffer, task->fp+task->pc);
            task->pc+=2;

            s_type = popByte(task->stack, task->sp);

            if (s_type == INT) {
                bool t = memCheck(task->p_id, pbuffer);
                if (t) {
                    var = memRead(task->p_id, pbuffer);
                    s_address = var->address;
                } else {
                    s_address = stackAlloc(2);
                }

                int v = popInt(task->stack, task->sp);
                s = 0;

                pushByte(s_address, s, v % 256);
                pushByte(s_address, s, v / 256);
                
                if (t == false) {
                    memAlloc(task->p_id, pbuffer, 2, INT, s_address);
                }
            } else if (s_type == CHAR) {
                bool t = memCheck(task->p_id, pbuffer);
                if (t) {
                    var = memRead(task->p_id, pbuffer);
                    s_address = var->address;
                } else {
                    s_address = stackAlloc(1);
                }

                char c = popChar(task->stack, task->sp);
                s = 0;

                pushByte(s_address, s, c);
                
                if (t == false) {
                    memAlloc(task->p_id, pbuffer, 1, CHAR, s_address);
                }
            } else if (s_type == FLOAT) {
                    bool t = memCheck(task->p_id, pbuffer);
                    if (t) {
                        var = memRead(task->p_id, pbuffer);
                        s_address = var->address;
                    } else {
                        s_address = stackAlloc(4);
                    }

                    f1 = popFloat(task->stack, task->sp);
                
                    uint32_t tt = *((uint32_t*)&f1);
                    s = 0;
                    
                    pushByte(s_address, s, (byte)(tt & 0xFF));
                    pushByte(s_address, s, (byte)((tt >> 8) & 0xFF));
                    pushByte(s_address, s, (byte)((tt >> 16) & 0xFF));
                    pushByte(s_address, s, (byte)((tt >> 24) & 0xFF));
                    
                    if (t == false) {
                        memAlloc(task->p_id, pbuffer, 4, FLOAT, s_address);
                    }
            } else if (s_type == STRING) {
                    memset(strbuffer, '\0', 10);
                    r1 = popString(task->stack, task->sp, strbuffer);
                    var = memRead(task->p_id, pbuffer);

                    bool t = memCheck(task->p_id, pbuffer);
                    if (t) {
                        stackFree(var->address, var->size);
                    }                    
                    s_address = stackAlloc(r1);

                    s = 0;
                    
                    for (uint8_t i = 0; i < strlen(strbuffer); i++) {
                        pushByte(s_address, s, strbuffer[i]);
                    }
                    
                    if (t == false) {
                        memAlloc(task->p_id, pbuffer, s, STRING, s_address);
                    } else {
                        var->address = s_address;
                        var->size = s;
                    }
            }
            // showStack(task->stack);
            break;
        case GET:

            task->pc+=1;
            r = readDataRegion(pbuffer, task->fp+task->pc);
            task->pc+=2;
            // Serial.print(F("Variable type: "));
            // Serial.println(s_type);
            var = memRead(task->p_id, pbuffer);
            s = var->size;

            if (var->type == INT) {
                i1 = popInt(var->address, s);
                pushInt(task->stack, task->sp, i1 % 256, i1 / 256);
            } else if (var->type == CHAR) {
                char cc = popChar(var->address, s);
                pushChar(task->stack, task->sp, cc);
            } else if (var->type == FLOAT) {
                f1 = popFloat(var->address, s);
                uint32_t ttt = *((uint32_t*)&f1);
                pushFloat(task->stack, task->sp, 
                        (byte)((ttt >> 24) & 0xFF),
                        (byte)((ttt >> 16) & 0xFF),
                        (byte)((ttt >> 8) & 0xFF),
                        (byte)(ttt & 0xFF)
                );
            } else if (var->type == STRING) {
                memset(strbuffer, '\0', 10);

                for (uint8_t i = 0; i < var->size; i++) {
                    char c = (char)popByte(var->address, s);
                    strncat(strbuffer, &c, 1);
                }

                pushString(task->stack, task->sp, strbuffer);
            }

            //showStack(task->stack);
            
            // stackFree(var->address, var->size);
            // memFree(task->p_id, pbuffer);

            break;
        case INCREMENT:
            s_type = popByte(task->stack, task->sp);

            if (s_type == INT) {
                i1 = popInt(task->stack, task->sp);
                i1++;
                pushInt(task->stack, task->sp, i1 % 256, i1 / 256);
            } else if (s_type == CHAR) {
                r1 = popChar(task->stack, task->sp);
                r1++;
                pushChar(task->stack, task->sp, (char)r1);
            } else if (s_type == FLOAT) {
                f1 = popFloat(task->stack, task->sp);
                f1+=1;
                uint32_t ttt = *((uint32_t*)&f1);
                pushFloat(task->stack, task->sp, 
                        (byte)((ttt >> 24) & 0xFF),
                        (byte)((ttt >> 16) & 0xFF),
                        (byte)((ttt >> 8) & 0xFF),
                        (byte)(ttt & 0xFF)
                );
            }
            
            task->pc+=1;
            break;
        case DECREMENT:
            s_type = popByte(task->stack, task->sp);

            if (s_type == INT) {
                i1 = popInt(task->stack, task->sp);
                i1--;
                pushInt(task->stack, task->sp, i1 % 256, i1 / 256);
            } else if (s_type == CHAR) {
                r1 = popChar(task->stack, task->sp);
                r1--;
                pushChar(task->stack, task->sp, (char)r1);
            } else if (s_type == FLOAT) {
                f1 = popFloat(task->stack, task->sp);
                f1-=1;
                uint32_t ttt = *((uint32_t*)&f1);
                pushFloat(task->stack, task->sp, 
                        (byte)((ttt >> 24) & 0xFF),
                        (byte)((ttt >> 16) & 0xFF),
                        (byte)((ttt >> 8) & 0xFF),
                        (byte)(ttt & 0xFF)
                );
            }
            
            task->pc+=1;
            break;
        case PLUS:    
            s_type = popByte(task->stack, task->sp);
            if (s_type == INT) {
                i1 = popInt(task->stack, task->sp);
            } else if (s_type == FLOAT) {
                f1 = popFloat(task->stack, task->sp);
                uint32_t ttt = *((uint32_t*)&f1);
                pushFloat(task->stack, task->sp, 
                        (byte)((ttt >> 24) & 0xFF),
                        (byte)((ttt >> 16) & 0xFF),
                        (byte)((ttt >> 8) & 0xFF),
                        (byte)(ttt & 0xFF)
                );
            }

            s_type = popByte(task->stack, task->sp);
            if (s_type == INT) {
                i2 = popInt(task->stack, task->sp);
            } else if (s_type == FLOAT) {
                f2 = popFloat(task->stack, task->sp);
            }   
            
            if (s_type == INT) {
                pushInt(task->stack, task->sp, (i1 + i2) % 256, (i1 + i2) / 256);
            } else if (s_type == FLOAT) {
                
            }

            task->pc+=1;

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
            s_type = popByte(task->stack, task->sp);

            if (s_type == INT) {
                i1 = popInt(task->stack, task->sp);
            }

            s_type = popByte(task->stack, task->sp);

            if (s_type == INT) {
                i2 = popInt(task->stack, task->sp);
            }

            if ((i1 - i2) == 0) {
                pushInt(task->stack, task->sp, 0, 0);
            } else {
                pushInt(task->stack, task->sp, 1, 0);
            }            

            task->pc+=1;

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
            
            if (s_type == INT) {
                i1 = popInt(task->stack, task->sp);
                Serial.println(i1);
            } else if (s_type == CHAR) {
                r1 = popChar(task->stack, task->sp);
                Serial.println((char)r1);
            } else if (s_type == FLOAT) {
                f1 = popFloat(task->stack, task->sp);
                Serial.println(f1);
            } else if (s_type == STRING) {
                memset(strbuffer, '\0', 10);
                r1 = popString(task->stack, task->sp, strbuffer);
                    
                strcpy(strbuffer, strrev(strbuffer));

                Serial.println(strbuffer);
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
            popByte(task->stack, task->sp);
            i1 = popInt(task->stack, task->sp);
            r1 = (uint8_t)i1;

            r4 = task->pc-1;

            task->pc++;
            task->pc+=readDataRegion(pbuffer, task->fp+task->pc);
            r2 = atoi(pbuffer);
            task->pc++;

            task->pc+=readDataRegion(pbuffer, task->fp+task->pc);
            r3 = atoi(pbuffer);

            if (r1 == 0) {
                task->pc+=r4;               
            } else {
                pushByte(task->stack, task->sp, (r3+r4));
                task->pc+=1;
            }
            break;
        case ENDWHILE:
            task->pc--;
            r1 = popByte(task->stack, task->sp);
            task->pc-=(r1-5);
            break;
        case LOOP:
            break;
        case ENDLOOP:
            break;
        case STOP:
            // stackFree(task->stack, 32);
            // removeTask(task->p_id);
            suspendTask(task->p_id);
            r = 0;
            break;
        case FORK:
            break;
        case WAITUNTILDONE:
            break;
        default:
            // Serial.print(F("[error] OS cant understand instruction: "));
            // Serial.println(instruction);
            r = 0;
    }

    return r+1;
}
