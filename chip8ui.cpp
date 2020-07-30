#include "chip8ui.h"
#include "ui_chip8ui.h"

Chip8UI::Chip8UI(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Chip8UI)
{
    ui->setupUi(this);

    cpu = makeCPU();
    interface = makeInterface();

    ui->display->setInterface(interface);
}

Chip8UI::~Chip8UI()
{
    delete ui;
}

void Chip8UI::readChip8ROM(const char* fileName)
{
    readROM(cpu, fileName);

    updateTimer = startTimer(UPDATE_MILLISEC);

    chip8Timer = 0;
}

void Chip8UI::keyPressEvent(QKeyEvent *event)
{
    const uint8_t keyIndex = mapToIndex(event->key());
    if ( keyIndex == KEYMAP_SIZE ) {
        event->ignore();
        return;
    }
    setKey(interface, keyIndex);
}

void Chip8UI::keyReleaseEvent(QKeyEvent *event)
{
    const uint8_t keyIndex = mapToIndex(event->key());
    if ( keyIndex == KEYMAP_SIZE ) {
        event->ignore();
        return;
    }
    resetKey(interface, keyIndex);
}

void Chip8UI::timerEvent(QTimerEvent *event)
{
    if ( event->timerId() == updateTimer ) {
        chip8Timer++;
        if (chip8Timer % 5 == 0) {
            tick(cpu, interface);
            chip8Timer = 0;
        }
        CChip8Errors error = step(cpu, interface);
        if (error == UpdateScreen) update();
        // TODO: Error handling.
    } else {
        event->ignore();
    }
}
