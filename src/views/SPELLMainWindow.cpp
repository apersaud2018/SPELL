#include "SPELLMainWindow.h"
#include "./ui_SPELLMainWindow.h"

#include <iostream>
#include "../control/Control.h"

SPELLMainWindow::SPELLMainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::SPELLMainWindow)
{   
    controller.initialize();
    ui->setupUi(this);
	
	connect(ui->playButton, QPushButton::clicked, this, playSelection);
}

SPELLMainWindow::~SPELLMainWindow()
{
    delete ui;
}

void SPELLMainWindow::playSelection() {
	
	std::cout << "Play" << "\n";
}
