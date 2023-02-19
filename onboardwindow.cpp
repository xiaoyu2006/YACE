#include "onboardwindow.h"
#include "./ui_onboardwindow.h"

#include "chip8ui.h"

OnboardWindow::OnboardWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::OnboardWindow)
{
    ui->setupUi(this);
    selectedFilePath = "";
}

OnboardWindow::~OnboardWindow()
{
    delete ui;
}

Q_NORETURN void OnboardWindow::on_cancelButton_released()
{
    exit(0);
}

void OnboardWindow::on_selectFileButton_released()
{
    QString selectedROMFilePath = QFileDialog::getOpenFileName(nullptr, tr("Select a Chip8 ROM file"));
    if (selectedROMFilePath == "") { return; }
    selectedFilePath = selectedROMFilePath;
    ui->selectFileButton->setText(selectedFilePath);
}

void OnboardWindow::on_okButton_released()
{
    if (selectedFilePath == "" || !fileExists(selectedFilePath) ) {
        QMessageBox::warning(this, tr("Warning"), tr("Chip8 ROM not selected or file does not exist."));
    } else {
        Chip8UI* chip8Emu = new Chip8UI(this);
        std::string filenameStdStr = selectedFilePath.toStdString();
        chip8Emu->readChip8ROM(filenameStdStr.c_str());
        chip8Emu->show();
    }
}
