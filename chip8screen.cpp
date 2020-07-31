#include "chip8screen.h"

Chip8Screen::Chip8Screen(QWidget *parent) : QWidget(parent)
{
    return;
}

void Chip8Screen::setInterface(Interface* interface)
{
    this->interface = interface;
}

void Chip8Screen::paintEvent(QPaintEvent *event)
{
    const int windowWidth = this->width();
    const int windowHeight = this->height();
    const int widthStep = windowWidth / DISPLAY_WIDTH;
    const int heightStep = windowHeight / DISPLAY_HEIGHT;

    QPainter painter(this);

    painter.fillRect(0, 0, windowWidth, windowHeight, BLACK);

    for (int h = 0 ; h < DISPLAY_HEIGHT ; ++h) {
        for (int w = 0 ; w < DISPLAY_WIDTH ; ++w) {
            const int index = h * DISPLAY_WIDTH + w;
            if (interface->screen[index]) {
                const int x = w * widthStep;
                const int y = h * heightStep;
                painter.fillRect(x, y, widthStep, heightStep, GREEN);
            }
        }
    }

    event->accept();
}
