#ifndef CHIP8UI_H
#define CHIP8UI_H

#include <QMainWindow>
#include <QEvent>
#include <QKeyEvent>
#include <QTimer>
#include <QTimerEvent>

#include "keymap.h"

#include "cchip8/cpu.h"
#include "cchip8/interface.h"

#define UPDATE_MILLISEC 2

namespace Ui {
class Chip8UI;
}

class Chip8UI : public QMainWindow
{
    Q_OBJECT

public:
    explicit Chip8UI(QWidget *parent = nullptr);
    ~Chip8UI();

    void readChip8ROM(const char* fileName);

protected:
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    void timerEvent(QTimerEvent *event);

private:
    Ui::Chip8UI *ui;

    CPU* cpu;
    Interface* interface;
    int chip8Timer;

    uint8_t updateTimer;
};

#endif // CHIP8UI_H
