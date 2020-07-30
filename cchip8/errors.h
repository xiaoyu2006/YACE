// Copyright (c) 2020 Yi

#ifndef _ERRORS_H_
#define _ERRORS_H_

typedef enum CChip8Errors {
    OK, // No errors
    ROMFileNotFound, // ROM can't be read.
    ROMFileTooLarge, // ROM is too large.
    UpdateScreen, // Screen should be updated.
    StackUnderflow, // Stack underflow.
    StackOverFlow, // Stack overflow.
    MemoryOutOfBounds, // Memory out of bounds.
    InvalidDigit, // Invalid digit.
    UnknownInstruction,// Unknown instruction.
} CChip8Errors;

#endif
