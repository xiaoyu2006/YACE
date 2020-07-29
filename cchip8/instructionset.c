// Copyright (c) 2020 Yi

#include "instructionset.h"

Instruction instructions[] = {
    { // CLS
        .mask = 0xffff, .pattern = 0x00e0, .args = 0
    },
    { // RET
        .mask = 0xffff, .pattern = 0x00ee, .args = 0
    },
    { // JP(ADDR)
        .mask = 0xf000, .pattern = 0x1000, .args = 1,
        .arg[0] = STATIC_INST_ARG_INIT(0x0fff, 0)
    },
    { // CALL(ADDR)
        .mask = 0xf000, .pattern = 0x2000, .args = 1,
        .arg[0] = STATIC_INST_ARG_INIT(0x0fff, 0)
    },
    { // SE(VX, NN)
        .mask = 0xf000, .pattern = 0x3000, .args = 2,
        .arg[0] = STATIC_INST_ARG_INIT(0x0f00, 8),
        .arg[1] = STATIC_INST_ARG_INIT(0x00ff, 0)
    },
    { // SNE(VX, NN)
        .mask = 0xf000, .pattern = 0x4000, .args = 2,
        .arg[0] = STATIC_INST_ARG_INIT(0x0f00, 8),
        .arg[1] = STATIC_INST_ARG_INIT(0x00ff, 0)
    },
    { // SE(VX, VY)
        .mask = 0xf00f, .pattern = 0x5000, .args = 2,
        .arg[0] = STATIC_INST_ARG_INIT(0x0f00, 8),
        .arg[1] = STATIC_INST_ARG_INIT(0x00f0, 4)
    },
    { // LD(VX, NN)
        .mask = 0xf000, .pattern = 0x6000, .args = 2,
        .arg[0] = STATIC_INST_ARG_INIT(0x0f00, 8),
        .arg[1] = STATIC_INST_ARG_INIT(0x00ff, 0)
    },
    { // ADD(VX, NN)
        .mask = 0xf000, .pattern = 0x7000, .args = 2,
        .arg[0] = STATIC_INST_ARG_INIT(0x0f00, 8),
        .arg[1] = STATIC_INST_ARG_INIT(0x00ff, 0)
    },
    { // LD(VX, VY)
        .mask = 0xf00f, .pattern = 0x8000, .args = 2,
        .arg[0] = STATIC_INST_ARG_INIT(0x0f00, 8),
        .arg[1] = STATIC_INST_ARG_INIT(0x00ff, 0)
    },
    { // OR(VX, VY)
        .mask = 0xf00f, .pattern = 0x8001, .args = 2,
        .arg[0] = STATIC_INST_ARG_INIT(0x0f00, 8),
        .arg[1] = STATIC_INST_ARG_INIT(0x00f0, 4)
    },
    { // AND(VX, VY)
        .mask = 0xf00f, .pattern = 0x8002, .args = 2,
        .arg[0] = STATIC_INST_ARG_INIT(0x0f00, 8),
        .arg[1] = STATIC_INST_ARG_INIT(0x00f0, 4)
    },
    { // XOR(VX, VY)
        .mask = 0xf00f, .pattern = 0x8003, .args = 2,
        .arg[0] = STATIC_INST_ARG_INIT(0x0f00, 8),
        .arg[1] = STATIC_INST_ARG_INIT(0x00f0, 4)
    },
    { // AND(VX, VY)
        .mask = 0xf00f, .pattern = 0x8004, .args = 2,
        .arg[0] = STATIC_INST_ARG_INIT(0x0f00, 8),
        .arg[1] = STATIC_INST_ARG_INIT(0x00f0, 4)
    },
    { // SUB(VX, VY)
        .mask = 0xf00f, .pattern = 0x8005, .args = 2,
        .arg[0] = STATIC_INST_ARG_INIT(0x0f00, 8),
        .arg[1] = STATIC_INST_ARG_INIT(0x00f0, 4)
    },
    { // SHR(VX, VY)
        .mask = 0xf00f, .pattern = 0x8006, .args = 2,
        .arg[0] = STATIC_INST_ARG_INIT(0x0f00, 8),
        .arg[1] = STATIC_INST_ARG_INIT(0x00f0, 4)
    },
    { // SUBN(VX, VY)
        .mask = 0xf00f, .pattern = 0x8007, .args = 2,
        .arg[0] = STATIC_INST_ARG_INIT(0x0f00, 8),
        .arg[1] = STATIC_INST_ARG_INIT(0x00f0, 4)
    },
    { // SHL(VX, VY)
        .mask = 0xf00f, .pattern = 0x800e, .args = 2,
        .arg[0] = STATIC_INST_ARG_INIT(0x0f00, 8),
        .arg[1] = STATIC_INST_ARG_INIT(0x00f0, 4)
    },
    { // SNE(VX, VY)
        .mask = 0xf00f, .pattern = 0x9000, .args = 2,
        .arg[0] = STATIC_INST_ARG_INIT(0x0f00, 8),
        .arg[1] = STATIC_INST_ARG_INIT(0x00f0, 4)
    },
    { // LD(I, ADDR)
        .mask = 0xf000, .pattern = 0xa000, .args = 2,
        .arg[0] = STATIC_INST_ARG_INIT(0x0000, 0),
        .arg[1] = STATIC_INST_ARG_INIT(0x0fff, 0)
    },
    { // JP(V0, ADDR)
        .mask = 0xf000, .pattern = 0xb000, .args = 2,
        .arg[0] = STATIC_INST_ARG_INIT(0x0000, 0),
        .arg[1] = STATIC_INST_ARG_INIT(0x0fff, 0)
    },
    { // RND(VX, NN)
        .mask = 0xf000, .pattern = 0xc000, .args = 2,
        .arg[0] = STATIC_INST_ARG_INIT(0x0f00, 8),
        .arg[1] = STATIC_INST_ARG_INIT(0x00ff, 0)
    },
    { // DRW(VX, VY, N)
        .mask = 0xf000, .pattern = 0xd000, .args = 3,
        .arg[0] = STATIC_INST_ARG_INIT(0x0f00, 8),
        .arg[1] = STATIC_INST_ARG_INIT(0x00f0, 4),
        .arg[2] = STATIC_INST_ARG_INIT(0x000f, 0)
    },
    { // SKP_VX(VX, NN)
        .mask = 0xf0ff, .pattern = 0xe09e, .args = 1,
        .arg[0] = STATIC_INST_ARG_INIT(0x0f00, 8)
    },
    { // SKNP_VX(VX, NN)
        .mask = 0xf0ff, .pattern = 0xe0a1, .args = 1,
        .arg[0] = STATIC_INST_ARG_INIT(0x0f00, 8)
    },
    { // LD(VX, DT)
        .mask = 0xf00f, .pattern = 0xf007, .args = 2,
        .arg[0] = STATIC_INST_ARG_INIT(0x0f00, 8),
        .arg[1] = STATIC_INST_ARG_INIT(0x0000, 0)
    },
    { // LD(VX, N)
        .mask = 0xf00f, .pattern = 0xf00a, .args = 2,
        .arg[0] = STATIC_INST_ARG_INIT(0x0f00, 8),
        .arg[1] = STATIC_INST_ARG_INIT(0x0000, 0)
    },
    { // LD(DT, VX)
        .mask = 0xf0ff, .pattern = 0xf015, .args = 2,
        .arg[0] = STATIC_INST_ARG_INIT(0x0000, 0),
        .arg[1] = STATIC_INST_ARG_INIT(0x0f00, 8)
    },
    { // LD(ST, VX)
        .mask = 0xf0ff, .pattern = 0xf018, .args = 2,
        .arg[0] = STATIC_INST_ARG_INIT(0x0000, 0),
        .arg[1] = STATIC_INST_ARG_INIT(0x0f00, 8)
    },
    { // ADD(I, VX)
        .mask = 0xf0ff, .pattern = 0xf01e, .args = 2,
        .arg[0] = STATIC_INST_ARG_INIT(0x0000, 0),
        .arg[1] = STATIC_INST_ARG_INIT(0x0f00, 8)
    },
    { // LD(F, VX)
        .mask = 0xf0ff, .pattern = 0xf029, .args = 2,
        .arg[0] = STATIC_INST_ARG_INIT(0x0000, 0),
        .arg[1] = STATIC_INST_ARG_INIT(0x0f00, 8)
    },
    { // LD(F, VX)
        .mask = 0xf0ff, .pattern = 0xf033, .args = 2,
        .arg[0] = STATIC_INST_ARG_INIT(0x0000, 0),
        .arg[1] = STATIC_INST_ARG_INIT(0x0f00, 8)
    },
    { // LD(I, VX)
        .mask = 0xf0ff, .pattern = 0xf055, .args = 2,
        .arg[0] = STATIC_INST_ARG_INIT(0x0000, 0),
        .arg[1] = STATIC_INST_ARG_INIT(0x0f00, 8)
    },
    { // LD(VX, I)
        .mask = 0xf0ff, .pattern = 0xf065, .args = 2,
        .arg[0] = STATIC_INST_ARG_INIT(0x0f00, 8),
        .arg[1] = STATIC_INST_ARG_INIT(0x0000, 0)
    },
    { // Pure data: Illegial
        .mask = 0x0000, .pattern = 0x0000, .args = 1,
        .arg[0] = STATIC_INST_ARG_INIT(0xffff, 0)
    }
};
