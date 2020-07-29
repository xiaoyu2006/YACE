// Copyright (c) 2020 Yi

// A complete instruction set in Chip8.

#ifndef _INSTRUCTION_SET_H_
#define _INSTRUCTION_SET_H_

#define MAX_ARGUMENTS 3

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "cpu.h"
#include "interface.h"

// Arguments for a instruction
typedef struct InstArg
{
    // Binary mask for the argument
    uint16_t mask;
    // Shift value to get the masked arg
    uint16_t shift;
} InstArg;

#define STATIC_INST_ARG_INIT(in_mask, in_shift) \
    { .mask = (in_mask), .shift = (in_shift) }

// A instruction
typedef struct Instruction
{
    // Binary mask for determing instruction
    uint16_t mask;
    // The correct pattern after masking
    uint16_t pattern;
    // Arguments
    uint8_t  args;
    InstArg arg[MAX_ARGUMENTS];
} Instruction;

#ifdef __cplusplus
extern "C" {
#endif

extern Instruction instructions[];

#ifdef __cplusplus
}
#endif

#endif
