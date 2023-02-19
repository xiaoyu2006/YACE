#ifndef ONBOARDWINDOW_H
#define ONBOARDWINDOW_H

#include <QMainWindow>
#include <QDialog>
#include <QMessageBox>
#include <QFileDialog>
#include <QFileInfo>

QT_BEGIN_NAMESPACE
namespace Ui { class OnboardWindow; }
QT_END_NAMESPACE

#define fileExists(path) (QFileInfo::exists((path)) && QFileInfo((path)).isFile())

class OnboardWindow : public QMainWindow
{
    Q_OBJECT

public:
    OnboardWindow(QWidget *parent = nullptr);
    ~OnboardWindow();

private slots:
    Q_NORETURN void on_cancelButton_released();

    void on_selectFileButton_released();

    void on_okButton_released();

private:
    Ui::OnboardWindow *ui;
    QString selectedFilePath;
};
#endif // ONBOARDWINDOW_H
