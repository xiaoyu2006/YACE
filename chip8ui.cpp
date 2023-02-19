#include "chip8ui.h"
#include "ui_chip8ui.h"

#include "keymap.h"
#include <QMessageBox>
#include <QDialog>
#include <QString>

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
        switch(error) {
        case ROMFileNotFound:
            showErrorAndExit(tr("ROM file missing"));
            break;
        case ROMFileTooLarge:
            showErrorAndExit(tr("ROM file too large"));
            break;
        case StackUnderflow:
            showErrorAndExit(tr("Stack underflow"));
            break;
        case StackOverFlow:
            showErrorAndExit(tr("Stack overflow"));
            break;
        case MemoryOutOfBounds:
            showErrorAndExit(tr("Memory out of bounds"));
            break;
        case InvalidDigit:
            showErrorAndExit(tr("Invalid digit"));
            break;
        case UnknownInstruction:
            showErrorAndExit(tr("Unknown instruction"));
            break;

        case UpdateScreen:
            update();
            break;

        case OK:
            break;
        }
    } else {
        event->ignore();
    }
}

void Chip8UI::showErrorAndExit(QString message)
{
    QString t(message + "\nDT: %1, I: %2, PC: %3, SP: %4, ST: %5");
    QMessageBox::critical(this, "Error", t.arg(
                              QString::number(cpu->DT),
                              QString::number(cpu->I),
                              QString::number(cpu->PC),
                              QString::number(cpu->SP),
                              QString::number(cpu->ST)));
    killTimer(this->updateTimer);
    this->close();
}
