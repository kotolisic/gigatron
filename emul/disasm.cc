#include "gigatron.h"
#include <string.h>

char* Gigatron::disasm(uint16_t address) {

    strcpy(disasm_row,    "");
    strcpy(disasm_opcode, "");
    strcpy(disasm_op1,    "");
    strcpy(disasm_op2,    "");

    uint16_t ir   = rom[address];
    uint8_t  op   = (ir >> 13) & 0x0007;
    uint8_t  mode = (ir >> 10) & 0x0007;
    uint8_t  bus  = (ir >> 8) & 0x0003;
    uint8_t  d    = (ir >> 0) & 0x00ff;

    // printf("%x %x %x %x\n", op, mode, bus, d);

    // Мнемоника
    if (op < 7) strcpy(disasm_opcode, s_opcodes[op]);
    else        strcpy(disasm_opcode, s_branches[mode]);

    // ALU
    if (op < 6) {

        // Операнд dst
        switch (mode) {

            case 0:
            case 1:
            case 2:
            case 3: sprintf(disasm_op1, "ac"); break;
            case 4: sprintf(disasm_op1, "x"); break;
            case 5: sprintf(disasm_op1, "y"); break;
            case 6:
            case 7: sprintf(disasm_op1, "out"); break;
        }

        // Операнд src
        switch (bus) {

            case 0: sprintf(disasm_op2, "$%02X", d); break;
            case 1: switch (mode) {

                case 0:
                case 4:
                case 5:
                case 6: sprintf(disasm_op2, "[$%02X]", d); break;
                case 1: sprintf(disasm_op2, "[x]"); break;
                case 2: sprintf(disasm_op2, "[y,$%02X]", d); break;
                case 3: sprintf(disasm_op2, "[y,x]"); break;
                case 7: sprintf(disasm_op2, "[y,x++]"); break;
            }
            break;

            case 2: sprintf(disasm_op2, "ac"); break;
            case 3: sprintf(disasm_op2, "inreg"); break;
        }

        sprintf(disasm_row, "%s %s,%s", disasm_opcode, disasm_op1, disasm_op2);
    }
    // STORE
    else if (op == 6) {

        if (bus == 1) sprintf(disasm_op1, "ctrl");
        else switch (mode) {

            case 0:
            case 6: sprintf(disasm_op1, "[$%02X]", d); break;
            case 1: sprintf(disasm_op1, "[x]"); break;
            case 2: sprintf(disasm_op1, "[y,$%02X]", d); break;
            case 3: sprintf(disasm_op1, "[y,x]"); break;
            case 4: sprintf(disasm_op1, "[$%02X],x", d); break;
            case 5: sprintf(disasm_op1, "[$%02X],y", d); break;
            case 7: sprintf(disasm_op1, "[y,x++]"); break;
        }

        // Операнд src
        switch (bus) {

            case 0:
            case 1: sprintf(disasm_op2, "$%02X", d); break;
            case 2: sprintf(disasm_op2, "ac"); break;
            case 3: sprintf(disasm_op2, "inreg"); break;
        }

        sprintf(disasm_row, "%s %s,%s", disasm_opcode, disasm_op2, disasm_op1);
    }
    // BRANCH
    else if (op == 7) {

        switch (bus) {

            case 0: sprintf(disasm_op1, "$%02X", d); break;
            case 1: sprintf(disasm_op1, "[$%02X]", d); break;
            case 2: sprintf(disasm_op1, "ac"); break;
            case 3: sprintf(disasm_op1, "inreg"); break;

        }
        sprintf(disasm_row, "%s %s%s", disasm_opcode, mode == 0 ? "y," : "", disasm_op1);

    }

    return disasm_row;
}
