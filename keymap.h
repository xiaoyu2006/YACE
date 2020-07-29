#ifndef KEYMAP_H
#define KEYMAP_H

#include <cinttypes>
#include <Qt>

#define KEYMAP_SIZE 16

extern const char keyMap[KEYMAP_SIZE];
extern uint8_t mapToIndex(int ch);

#endif // KEYMAP_H
