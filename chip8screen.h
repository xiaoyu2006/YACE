#ifndef CHIP8SCREEN_H
#define CHIP8SCREEN_H

#include <QWidget>
#include <QPaintDevice>
#include <QPainter>
#include <QBrush>
#include <QPaintEvent>

#include <interface.h>

#define BLACK QColor(0, 0, 0)
#define GREEN QColor(0, 255, 0)

class Chip8Screen : public QWidget
{
    Q_OBJECT

public:
    explicit Chip8Screen(QWidget *parent = nullptr);
    void setInterface(Interface* interface);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    Interface* interface;
};

#endif // CHIP8SCREEN_H
