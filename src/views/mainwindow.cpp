#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <iostream>
#include <QFileDialog>
#include <QMessageBox>
#include <QDir>
#include "Control.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    controller.initialize();
    ui->setupUi(this);

    connect(ui->ImportAudioFiles, &QPushButton::clicked, this, loadAudioFile);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::loadAudioFile()
{
    QString file_path = QFileDialog::getOpenFileName(this,"Open Audio File", "C:/test");
    std::cout << file_path.toStdString() << "\n";
}

