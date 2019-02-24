#pragma once

#ifndef ASMLANG_H
#define ASMLANG_H

static const char ASMBEGIN[] =
".MODEL  SMALL, C\n"
"EXTRN   printf:NEAR\n\n";

static const char STACKSG[] =
"STACKSG SEGMENT STACK \'S\'\n"
"        DW      128 DUP(?)\n"
"STACKSG ENDS\n\n";

static const char DATA_BEGIN[] =
"DATA    SEGMENT WORD PUBLIC \'WORD\'\n"
"PRN_STR DB \'Set %s = %d\', 13, 10, 0\n";

static const char DATA_END[] =
"DATA    ENDS\n\n"
"DGROUP  GROUP DATA\n\n";

static const char DATA_HEAD[] =
"D_";

static const char DATA_NAME[] =
"N_";

static const char DATA_IDDB[] =
"     DB";

static const char DATA_ITEM[] =
"     DW      0\n";

static const char CODE_BEGIN[] =
"CODE    SEGMENT WORD PUBLIC \'CODE\'\n"
"MAIN    PROC    NEAR\n"
"        ASSUME  CS:CODE, DS:DGROUP, SS:STACKSG\n";

static const char CODE_END[] =
"        MOV     AX, 4C00H\n"
"        INT     21H\n"
"MAIN    ENDP\n"
"CODE    ENDS\n\n"
"        END     MAIN\n";

static const char WHILE_BEGIN[] =
"WB_";

static const char WHILE_END[] =
"WE_";

#endif
