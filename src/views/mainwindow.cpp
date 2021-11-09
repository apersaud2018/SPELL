#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <iostream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->ImportAudioFiles, &QPushButton::clicked, this, loadAudioFile);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::loadAudioFile()
{
    std::cout << "YEEEEEET!" << "\n";
}

