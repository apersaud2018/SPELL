#include "SPELLMainWindow.h"
#include "./ui_SPELLMainWindow.h"
#include <iostream>
#include <QFileDialog>
#include <QMessageBox>
#include <QDir>
#include <QGraphicsScene>
#include <QListWidget>
#include <vector>
#include "../control/Control.h"

SPELLMainWindow::SPELLMainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::SPELLMainWindow)
{   
    controller.initialize();
    ui->setupUi(this);
	
    //scene = new QGraphicsScene();
    ui->waveForm->setScene(&scene);
    
    std::cout << scene.width() << "\n";
    
    scene.addLine(-250,0,250,0,QPen(Qt::black));
    
	connect(ui->addAudioFile, QPushButton::clicked, this, addAudioFile);
	connect(ui->playButton, QPushButton::clicked, this, playSelection);
	connect(ui->fileList, QListWidget::currentItemChanged, this, newFileSelected);
}

SPELLMainWindow::~SPELLMainWindow()
{
    delete ui;
}

// 

void SPELLMainWindow::addAudioFile() {
    QString file_path = QFileDialog::getOpenFileName(this,"Open Audio File", "C:/test");
    bool success = controller.addAudioFile(file_path.toStdString());
    if(success){
        addFileToList(file_path);
    }
}

void SPELLMainWindow::playSelection() {
	
	std::cout << "Play" << "\n";
}

void SPELLMainWindow::addFileToList(QString path)
{
    ui->fileList->addItem(path);
}

void SPELLMainWindow::newFileSelected() {
    
    QString current_path = ui->fileList->currentItem()->text();
    std::cout << "Selected " << current_path.toStdString() << "\n";
    controller.file_index = ui->fileList->currentRow();
    renderWaveForm(controller.getAudioData(controller.file_index));
    
}

void SPELLMainWindow::renderWaveForm(std::vector<double> data){
    ui->waveForm->setScene(&scene);
    std::cout << scene.width() << "\n";
    std::cout << scene.height() << "\n";
    
    scene.addLine(-250,0,250,0,QPen(Qt::black));
    
}


