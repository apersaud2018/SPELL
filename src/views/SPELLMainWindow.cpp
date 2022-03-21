#include "SPELLMainWindow.h"
#include "./ui_SPELLMainWindow.h"
#include <iostream>
#include <QFileDialog>
#include <QMessageBox>
#include <QDir>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QSplitter>
#include <QListWidget>
#include <vector>
#include <QMouseEvent>
#include <QPen>
#include <QColor>
#include <QCheckBox>
#include <QPolygonF>
#include <cmath>
#include "control/Control.h"

SPELLMainWindow::SPELLMainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::SPELLMainWindow)
{
    controller = new Control();

    //controller->initialize();
    ui->setupUi(this);

    //QSplitter *splitter = new QSplitter(ui->scrollVWidget);
    //splitter->setOrientation(Qt::Vertical);
    waveDraw = new WaveDrawWidget(ui->scrollVWidget, controller);
    waveDraw->setFixedHeight(150);
    waveDraw->setMouseTracking(true);
    fullWaveDraw = new FullWaveDrawWidget(ui->scrollVWidget, controller);
    fullWaveDraw->setFixedHeight(50);
    fullWaveDraw->setMouseTracking(true);
    spectrogramDraw = new SpectrogramViewWidget(ui->scrollVWidget, controller);
    spectrogramDraw->setFixedHeight(150);
    

    ui->rightVLayout->insertWidget(2, spectrogramDraw);
    ui->rightVLayout->insertWidget(2, waveDraw);
    ui->rightVLayout->insertWidget(2, fullWaveDraw);
    
    
    
    //ui->scrollVLayout->insertWidget(0, waveDraw);
    //ui->scrollVLayout->insertWidget(1, spectrogramDraw);


	connect(ui->addAudioFile, QPushButton::clicked, this, addAudioFile);
	connect(ui->InsertNewTrack, QPushButton::clicked, this, addNewTrack);
	connect(ui->playButton, QPushButton::clicked, this, playSelection);
	connect(ui->fileList, QListWidget::currentItemChanged, this, newFileSelected);
	connect(ui->autoScale, QCheckBox::stateChanged, waveDraw, waveDraw->autoScaleChanged);

    //connect(ui->zoomInButton, QPushButton::clicked, this, zoomIn);
    //connect(ui->zoomOutButton, QPushButton::clicked, this, zoomOut);
    connect(ui->zoomSlider, QSlider::valueChanged, this, changeZoom);

}

SPELLMainWindow::~SPELLMainWindow()
{
    delete ui;
}

void SPELLMainWindow::addTrack(){
    controller->createPhonemeTrack();
    TimelineView *timelineView = new TimelineView(ui->scrollVWidget, controller);
    timelineView->setFixedHeight(50);
    timelineView->setMouseTracking(true);
    ui->scrollVLayout->insertWidget(timelineViews.size(), timelineView);
    timelineView->track = controller->phonemeTrack;
    timelineViews.push_back(timelineView);
}

//

void SPELLMainWindow::addAudioFile() {
    QString file_path = QFileDialog::getOpenFileName(this,"Open Audio File", "C:/test/audio");
    bool success = controller->addAudioFile(file_path.toStdString());
    if(success){
        controller->computeSpectrogram(controller->spectrograms.size());
        addFileToList(file_path);
        if(controller->spectrograms.size() == 1){
            addTrack();
        }

    }
}

void SPELLMainWindow::addNewTrack() {
    addTrack();
	std::cout << "Created New Track" << "\n";
}

void SPELLMainWindow::playSelection() {

	std::cout << "Play" << "\n";
}

void SPELLMainWindow::addFileToList(QString path)
{
    ui->fileList->addItem(path);
}

void SPELLMainWindow::zoomOut() {
  controller->setZoom(controller->zoom + ZOOM_SPEED);

}

void SPELLMainWindow::zoomIn() {
  controller->setZoom(controller->zoom - ZOOM_SPEED);
}

void SPELLMainWindow::changeZoom() {
  int val = 100 - (ui->zoomSlider->value());
  controller->setZoom(val/100.0);
}

void SPELLMainWindow::newFileSelected() {
    QString current_path = ui->fileList->currentItem()->text();
    std::cout << "Selected " << current_path.toStdString() << "\n";
    controller->setFileIndex(ui->fileList->currentRow());
}


void SPELLMainWindow::wheelEvent(QWheelEvent *event){
    QPoint numPixels = event->angleDelta();

    if (numPixels.x() > 0) {
      controller->setPosition(controller->left_position + SCROLL_SPEED * controller->zoom);
    }
    else if (numPixels.x() < 0) {
      controller->setPosition(controller->left_position - SCROLL_SPEED * controller->zoom);
    }

}


