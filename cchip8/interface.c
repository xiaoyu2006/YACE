// Copyright (c) 2020 Yi

#include <stdlib.h>
#include <string.h>
#include "interface.h"

/*
* Function: makeInterface
* --------------------
*  Create a new Interface.
*
*  returns: pointer to the interface
*/
Interface* makeInterface()
{
    Interface* ret = (Interface*) malloc(sizeof(Interface));
    clearDisplay(ret);
    ret->soundEnabled = false;
    ret->keys = 0;
    ret->keyPressed = KEY_NOT_PRESSED;
    return ret;
}

void clearDisplay(Interface* interface)
{
    memset(interface->screen, 0, sizeof(interface->screen));
}

void setKey(Interface* interface, uint8_t keyIndex)
{
    const uint16_t keyMask = 1 << keyIndex;

    interface->keys |= keyMask;
    interface->keyPressed = keyIndex;
}

void resetKey(Interface* interface, uint8_t keyIndex)
{
    const uint16_t keyMask = 1 << keyIndex;

    interface->keys &= ~keyMask;
    interface->keyPressed = KEY_NOT_PRESSED;
}

int16_t waitKey(Interface* interface)
{
    return interface->keyPressed;
}

uint16_t getKeys(Interface* interface)
{
    return interface->keys;
}

bool drawPixel(Interface* interface, uint8_t x, uint8_t y, bool value)
{
    uint16_t linearPos = y * DISPLAY_WIDTH + x;
    bool collision = interface->screen[linearPos] & value;
    interface->screen[linearPos] ^= value;
    return collision;
}

void enableSound(Interface* interface)
{
    interface->soundEnabled = true;
}

void disableSound(Interface* interface)
{
    interface->soundEnabled = false;
}
