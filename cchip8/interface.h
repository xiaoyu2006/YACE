// Copyright (c) 2020 Yi

// Interface for IO.

#ifndef _INTERFACE_H_
#define _INTERFACE_H_

#include <stdbool.h>
#include <inttypes.h>

#define KEYMAP_LEN 16

#define DISPLAY_WIDTH 64
#define DISPLAY_HEIGHT 32
#define TOT_CHAR_COUNT (DISPLAY_WIDTH * DISPLAY_HEIGHT)
#define KEY_NOT_PRESSED -1

typedef struct Interface {
    bool        screen[TOT_CHAR_COUNT];
    bool        soundEnabled;
    uint16_t    keys;
    int16_t     keyPressed;
} Interface;


#ifdef __cplusplus
extern "C" {
#endif

extern Interface* makeInterface();
extern void clearDisplay(Interface* interface);
extern void setKey(Interface* interface, uint8_t keyIndex);
extern void resetKey(Interface* interface, uint8_t keyIndex);
extern int16_t waitKey(Interface* interface);
extern uint16_t getKeys(Interface* interface);
extern bool drawPixel(Interface* interface, uint8_t wid, uint8_t hei, bool value);
extern void enableSound(Interface* interface);
extern void disableSound(Interface* interface);

#ifdef __cplusplus
}
#endif


#endif
