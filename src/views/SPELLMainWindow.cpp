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
    fullWaveDraw = new FullWaveDrawWidget(ui->scrollVWidget, controller);
    fullWaveDraw->setFixedHeight(100);
    spectrogramDraw = new SpectrogramViewWidget(ui->scrollVWidget, controller);
    spectrogramDraw->setFixedHeight(150);
    // spectrogram = new QGraphicsView(ui->scrollVWidget);
    // spectrogram->setFixedHeight(400);
    //ui->scrollVLayout->insertWidget(0, splitter);
    ui->scrollVLayout->insertWidget(0, waveDraw);
    ui->rightVLayout->insertWidget(2, fullWaveDraw);
    ui->scrollVLayout->insertWidget(0, spectrogramDraw);


    //spectrogram->setScene(&specScene);



    //ui->scrollAreaWidgetContents_2->viewport()->setMouseTracking(true);

    //scene.addLine(-250,0,250,0,QPen(Qt::black));

	connect(ui->addAudioFile, QPushButton::clicked, this, addAudioFile);
	connect(ui->playButton, QPushButton::clicked, this, playSelection);
	connect(ui->fileList, QListWidget::currentItemChanged, this, newFileSelected);
	connect(ui->autoScale, QCheckBox::stateChanged, waveDraw, waveDraw->autoScaleChanged);

  connect(ui->zoomInButton, QPushButton::clicked, this, zoomIn);
  connect(ui->zoomOutButton, QPushButton::clicked, this, zoomOut);
}

SPELLMainWindow::~SPELLMainWindow()
{
    delete ui;
}

//

void SPELLMainWindow::addAudioFile() {
    QString file_path = QFileDialog::getOpenFileName(this,"Open Audio File", "C:/test/audio");
    bool success = controller->addAudioFile(file_path.toStdString());
    if(success){
        controller->computeSpectrogram(controller->spectrograms.size());
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

void SPELLMainWindow::zoomOut() {
  controller->setZoom(controller->zoom + ZOOM_SPEED);
}

void SPELLMainWindow::zoomIn() {
  controller->setZoom(controller->zoom - ZOOM_SPEED);
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

void SPELLMainWindow::mouseMoveEvent(QMouseEvent *event){


}




void SPELLMainWindow::resizeEvent(QResizeEvent *event){
	if(controller->file_index > -1){
        // renderWaveForm(controller->getAudioData(controller->file_index));
        // renderFullWaveForm(controller->getAudioData(controller->file_index));
        // renderSpectrogram(controller->spectrograms[controller->file_index],controller->data_size);
    }
}
