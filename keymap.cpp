#include "keymap.h"

using namespace Qt;

const char keyMap[KEYMAP_SIZE] = {
    Key_1, Key_2, Key_3, Key_4,
    Key_Q, Key_W, Key_E, Key_R,
    Key_A, Key_S, Key_D, Key_F,
    Key_Z, Key_X, Key_C, Key_V
};

uint8_t mapToIndex(int ch)
{
    for (uint8_t i = 0 ; i < KEYMAP_SIZE ; ++i) {
        if (keyMap[i] == ch) return i;
    }
    return KEYMAP_SIZE;
}

