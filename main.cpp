#include "onboardwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    OnboardWindow w;
    w.show();
    return a.exec();
}
