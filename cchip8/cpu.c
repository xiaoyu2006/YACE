// Copyright (c) 2020 Yi

#include "cpu.h"

uint8_t fontSet[FONT_SET_SIZE] = {
    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
    0x20, 0x60, 0x20, 0x20, 0x70, // 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
    0x90, 0x90, 0xF0, 0x10, 0x10, // 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
    0xF0, 0x10, 0x20, 0x40, 0x40, // 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90, // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
    0xF0, 0x80, 0x80, 0x80, 0xF0, // C
    0xE0, 0x90, 0x90, 0x90, 0xE0, // D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
    0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

/*
* Function: makeCPU
* --------------------
*  Create a Chip-8 CPU.
*
*  returns: pointer of the CPU
*/
CPU* makeCPU()
{
    srand(time(NULL));

    CPU* cpu = (CPU*) malloc(sizeof(CPU));
    memset(cpu->memory, 0, sizeof(cpu->memory));
    memset(cpu->registers, 0, sizeof(cpu->registers));
    cpu->ST = cpu->DT = 0;
    cpu->I = 0;
    cpu->SP = -1;
    cpu->PC = MEM_START;
    // 0 to 80 for font set.
    for (uint16_t i = 0; i < FONT_SET_SIZE; ++i) {
        cpu->memory[i] = fontSet[i];
    }
    return cpu;
}

void nextInstruction(CPU* cpu)
{
    cpu->PC += 2;
}

void skipInstruction(CPU* cpu)
{
    cpu->PC += 4;
}

/*
* Function: readRom
* --------------------
*  Read ROM into the memory.
*
*  cpu: current cpu.
*  filename: ROM to read
*/
CChip8Errors readROM(CPU* cpu, const char* filename)
{
    FILE* in = fopen(filename, "rb");
    if ( in == NULL ) {
        perror("ROM file not exists.");
        return ROMFileNotFound;
    }
    uint16_t i = MEM_START;
    int charInt = fgetc(in);
    while ( charInt != EOF ) {
        cpu->memory[i++] = (uint8_t) charInt;
        if ( i >= MEMORY_SIZE ) {
            perror("Invaild ROM file: too large.");
            return ROMFileTooLarge;
        }
        charInt = fgetc(in);
    }
    return OK;
}

void tick(CPU* cpu, Interface* interface)
{
    if (cpu->DT > 0) {
        cpu->DT--;
    }

    if (cpu->ST > 0) {
        cpu->ST--;
    } else {
        disableSound(interface);
    }
}

/*
* Function: step
* --------------------
*  Next step
*
*  cpu: current cpu
*  interface: current Interface
*/
CChip8Errors step(CPU* cpu, Interface* interface)
{
    uint16_t opcode = fetch(cpu);
    DecodedInst inst = decode(opcode);
    return execute(cpu, interface, inst);
}

/*
* Function: fetch
* --------------------
*  Fetch an operation code from memory.
*
*  cpu: current cpu
*
*  returns: operation code
*/
uint16_t fetch(CPU* cpu)
{
    uint16_t op = (cpu->memory[cpu->PC] << 8) | (cpu->memory[cpu->PC+1] << 0);
    return op;
}

/*
* Function: decode
* --------------------
*  Disassemble the current operation code
*
*  opcode: operation code.
*
*  returns: decided instruction
*/
DecodedInst decode(uint16_t opcode)
{
    DecodedInst ret;
    Instruction found;
    for (uint8_t currInst = 0 ; ; ++currInst) {
        uint16_t masked = instructions[currInst].mask & opcode;
        if (masked == instructions[currInst].pattern) {
            found = instructions[currInst];
            ret.pattern = masked;
            break;
        }
    }
    for (uint8_t currArg = 0 ; currArg < found.args ; ++currArg) {
        InstArg realArg = found.arg[currArg];
        ret.args[currArg] = (opcode & realArg.mask) >> realArg.shift;
    }
    return ret;
}

/*
* Function: execute
* --------------------
*  Execute an instruction.
*
*  cpu: current cpu
*  interface: current Interface
*  inst: Decoded instruction
*/
CChip8Errors execute(CPU* cpu, Interface* interface, DecodedInst inst)
{
    // TODO: Debug
    switch (inst.pattern) {

    case 0x00e0: // CLS
    {
        clearDisplay(interface);
        nextInstruction(cpu);
        return UpdateScreen;
    }

    case 0x00ee: // RET
    {
        if (cpu->SP == -1) {
            perror("Stack underflow.");
            return StackUnderflow;
        }
        cpu->PC = cpu->stack[cpu->SP--];
        break;
    }

    case 0x1000: // JP(ADDR)
    {
        cpu->PC = inst.args[0];
        break;
    }

    case 0x2000: // CALL(ADDR)
    {
        if (cpu->SP == 15) {
            perror("Stack overflow.");
            return StackOverFlow;
        }
        cpu->stack[++cpu->SP] = cpu->PC + 2;
        cpu->PC = inst.args[0];
        break;
    }

    case 0x3000: // SE(VX, NN)
    {
        if ( cpu->registers[inst.args[0]] == inst.args[1] ) {
            skipInstruction(cpu);
        } else {
            nextInstruction(cpu);
        }
        break;
    }

    case 0x4000: // SNE(VX, NN)
    {
        if ( cpu->registers[inst.args[0]] != inst.args[1] )
            skipInstruction(cpu);
        else
            nextInstruction(cpu);
        break;
    }

    case 0x5000: // SE(VX, VY)
    {
        if ( cpu->registers[inst.args[0]] == cpu->registers[inst.args[1]] ) {
            skipInstruction(cpu);
        } else {
            nextInstruction(cpu);
        }
        break;
    }

    case 0x6000: // LD(VX, NN)
    {
        cpu->registers[inst.args[0]] = inst.args[1];
        nextInstruction(cpu);
        break;
    }

    case 0x7000: // ADD(VX, NN)
    {
        cpu->registers[inst.args[0]] += inst.args[1];
        nextInstruction(cpu);
        break;
    }

    case 0x8000: // LD(VX, VY)
    {
        cpu->registers[inst.args[0]] = cpu->registers[inst.args[1]];
        nextInstruction(cpu);
        break;
    }

    case 0x8001: // OR(VX, VY)
    {
        cpu->registers[inst.args[0]] |= cpu->registers[inst.args[1]];
        nextInstruction(cpu);
        break;
    }

    case 0x8002: // AND(VX, VY)
    {
        cpu->registers[inst.args[0]] &= cpu->registers[inst.args[1]];
        nextInstruction(cpu);
        break;
    }

    case 0x8003: // XOR(VX, VY)
    {
        cpu->registers[inst.args[0]] ^= cpu->registers[inst.args[1]];
        nextInstruction(cpu);
        break;
    }

    case 0x8004: // ADD(VX, VY)
    {
        cpu->registers[0xf] = ( cpu->registers[inst.args[0]] + cpu->registers[inst.args[1]] > 0xff ) ? 1 : 0;
        cpu->registers[inst.args[0]] += cpu->registers[inst.args[1]];
        nextInstruction(cpu);
        break;
    }

    case 0x8005: // SUB(VX, VY)
    {
        cpu->registers[0xf] = (cpu->registers[inst.args[0]] > cpu->registers[inst.args[1]]) ? 1 : 0;
        cpu->registers[inst.args[0]] -= cpu->registers[inst.args[1]];
        nextInstruction(cpu);
        break;
    }

    case 0x8006: // SHR(VX, VY)
    {
        cpu->registers[0xf] = cpu->registers[inst.args[0]] & 1;
        cpu->registers[inst.args[0]] >>= 1;
        nextInstruction(cpu);
        break;
    }

    case 0x8007: // SUBN(VX, VY)
    {
        cpu->registers[0xf] = (cpu->registers[inst.args[1]] > cpu->registers[inst.args[0]]) ? 1 : 0;
        cpu->registers[inst.args[0]] = cpu->registers[inst.args[1]] - cpu->registers[inst.args[0]];
        nextInstruction(cpu);
        break;
    }

    case 0x800e: // SHL(VX, VY)
    {
        cpu->registers[0xf] = cpu->registers[inst.args[0]] >> 7;
        cpu->registers[inst.args[0]] <<= 1;
        nextInstruction(cpu);
        break;
    }

    case 0x9000: // SNE(VX, VY)
    {
         if (cpu->registers[inst.args[0]] != cpu->registers[inst.args[1]]) {
             skipInstruction(cpu);
         } else {
             nextInstruction(cpu);
         }
        break;
    }

    case 0xa000: // LD(I, ADDR)
    {
        cpu->I = inst.args[1];
        nextInstruction(cpu);
        break;
    }

    case 0xb000: // JP(V0, ADDR)
    {
        cpu->PC = cpu->registers[0] + inst.args[1];
        break;
    }

    case 0xc000: // RND(VX, NN)
    {
        cpu->registers[inst.args[0]] = ((rand() % 0xff) & inst.args[1]);
        nextInstruction(cpu);
        break;
    }

    case 0xd000: // DRW(VX, VY, N)
    {
        if ( cpu->I > (MEMORY_SIZE - 1 - inst.args[2]) ) {
            perror("Memory out of bounds");
            return MemoryOutOfBounds;
        } else {
            cpu->registers[0xf] = 0;
            for (int i = 0 ; i < inst.args[2] ; ++i) {
                const uint8_t line = cpu->memory[cpu->I + i];
                for (int pos = 0 ; pos < 8 ; ++pos) {
                    const bool value = ( line & (1 << (7 - pos)) ) ? 1 : 0;

                    const uint8_t x = (cpu->registers[inst.args[0]] + pos) % DISPLAY_WIDTH;
                    const uint8_t y = (cpu->registers[inst.args[1]] + i) % DISPLAY_HEIGHT;

                    if (drawPixel(interface, x, y, value)) {
                        cpu->registers[0xf] = 1;
                    }
                }
            }
            nextInstruction(cpu);
            return UpdateScreen;
        }
    }

    case 0xe09e: // SKP(VX)
    {
        if ( getKeys(interface) & (1 << cpu->registers[inst.args[0]]) ) {
            skipInstruction(cpu);
        } else {
            nextInstruction(cpu);
        }
        break;
    }

    case 0xe0a1: // SKNP(VX)
    {
        if ( getKeys(interface) & (1 << cpu->registers[inst.args[0]]) ) {
            nextInstruction(cpu);
        } else {
            skipInstruction(cpu);
        }
        break;
    }

    case 0xf007: // LD(VX, DT)
    {
        cpu->registers[inst.args[0]] = cpu->DT;
        nextInstruction(cpu);
        break;
    }

    case 0xf00a: // LD(VX, N)
    {
        const int16_t keyPressed = waitKey(interface);
        if ( keyPressed == KEY_NOT_PRESSED ) return OK;

        cpu->registers[inst.args[0]] = keyPressed;
        nextInstruction(cpu);
        break;
    }

    case 0xf015: // LD(DT, VX)
    {
        cpu->DT = cpu->registers[inst.args[1]];
        nextInstruction(cpu);
        break;
    }

    case 0xf018: // LD(ST, VX)
    {
        cpu->ST = cpu->registers[inst.args[1]];
        if (cpu->ST > 0) {
            enableSound(interface);
        }
        nextInstruction(cpu);
        break;
    }

    case 0xf01e: // ADD(I, VX)
    {
        cpu->I += cpu->registers[inst.args[1]];
        nextInstruction(cpu);
        break;
    }

    case 0xf029: // LD(F, VX)
    {
        if (cpu->registers[inst.args[1]] > 0xf) {
            perror("Invalid digit.");
            return InvalidDigit;
        } else {
            cpu->I = cpu->registers[inst.args[1]] * 5;
            nextInstruction(cpu);
        }
        break;
    }

    case 0xf033: // LD(B, VX)
    {
        const uint16_t I = cpu->I;

        if (I > (MEMORY_SIZE - 3)) {
            perror("Memory out of bounds.");
            return MemoryOutOfBounds;
        } else {
            uint8_t x = cpu->registers[inst.args[1]];
            const uint8_t a = x / 100;
            x -= a * 100;
            const uint8_t b = x / 10;
            x -= b * 10;
            const uint8_t c = x;

            cpu->memory[I] = a;
            cpu->memory[I + 1] = b;
            cpu->memory[I + 2] = c;

            nextInstruction(cpu);
        }
        break;
    }

    case 0xf055: // LD(I, VX)
    {
        const uint16_t I = cpu->I;
        if (I > (MEMORY_SIZE - 1 - inst.args[1])) {
            perror("Memory out of bounds.");
            return MemoryOutOfBounds;
        } else {
            for (uint16_t i = 0 ; i <= inst.args[1] ; ++i) {
                cpu->memory[I + i] = cpu->registers[i];
            }
            nextInstruction(cpu);
        }
        break;
    }

    case 0xf065: // LD(VX, I)
    {
        const uint16_t I = cpu->I;
        if (I > (MEMORY_SIZE - 1 - inst.args[0])) {
            perror("Memory out of bounds.");
            return MemoryOutOfBounds;
        } else {
            for (uint16_t i = 0 ; i <= inst.args[0]; ++i) {
                cpu->registers[i] = cpu->memory[I + i];
            }
            nextInstruction(cpu);
        }
        break;
    }

    default: // DW
    {
        perror("Unknown instruction.");
        return UnknownInstruction;
    }

    }

    return OK;
}
