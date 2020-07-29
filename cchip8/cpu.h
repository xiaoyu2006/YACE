// Copyright (c) 2020 Yi

// CPU for Chip8.

#ifndef _CPU_H_
#define _CPU_H_

#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>
#include <stdio.h>

#include "instructionset.h"
#include "interface.h"

#define MEMORY_SIZE 4096
#define REGISTERS_NUM 16
#define STACK_SIZE 16
#define FONT_SET_SIZE 80
#define MEM_START 0x200

// Basic CPU of Chip8
typedef struct CPU
{
    // Memory
    uint8_t  memory[MEMORY_SIZE];
    // Registers
    uint8_t  registers[REGISTERS_NUM];
    // Stack
    uint16_t stack[STACK_SIZE];
    // Timers
    //  Sound timer
    uint8_t  ST;
    //  Delay timer
    uint8_t  DT;
    // Index register
    uint16_t I;
    // Stack pointer
    int8_t   SP;
    // Program counter
    uint16_t PC;
} CPU;

typedef struct DecodedInst
{
    uint16_t pattern;
    uint16_t args[MAX_ARGUMENTS];
} DecodedInst;

#ifdef __cplusplus
extern "C" {
#endif

extern uint8_t fontSet[FONT_SET_SIZE];

extern CPU* makeCPU(void);
extern void readROM(CPU* cpu, const char* filename);
extern void nextInstruction(CPU* cpu);
extern void skipInstruction(CPU* cpu);
extern void tick(CPU* cpu, Interface* interface);
extern void step(CPU* cpu, Interface* interface);
extern uint16_t fetch(CPU* cpu);
extern DecodedInst decode(uint16_t opcode);
extern void execute(CPU* cpu, Interface* interface, DecodedInst inst);

#ifdef __cplusplus
}
#endif

#endif
