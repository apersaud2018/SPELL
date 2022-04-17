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
    // enable tooltips
    this->setAttribute(Qt::WA_AlwaysShowToolTips,true);


    waveDraw = new WaveDrawWidget(ui->tlLayoutWidget, controller);
    waveDraw->setFixedHeight(150);
    waveDraw->setMouseTracking(true);
    fullWaveDraw = new FullWaveDrawWidget(ui->scrollVWidget, controller);
    fullWaveDraw->setFixedHeight(50);
    fullWaveDraw->setMouseTracking(true);
    spectrogramDraw = new SpectrogramViewWidget(ui->tlLayoutWidget, controller);
    spectrogramDraw->setFixedHeight(256);

    // noteViewWidget = new noteViewWidget(ui->scrollVWidget, controller);
    // noteViewWidget->setVisible(false);


    ui->tlLayout->insertWidget(1, waveDraw);
    ui->tlLayout->insertWidget(2, spectrogramDraw);
    ui->rightVLayout->insertWidget(1, fullWaveDraw);
    // ui->rightVLayout->insertWidget(3, noteViewWidget);




	connect(ui->addAudioFile, QPushButton::clicked, this, addAudioFile);
	connect(ui->InsertNewTrack, QPushButton::clicked, this, addNewTrack);
	connect(ui->playButton, QPushButton::clicked, this, playSelection);
	connect(ui->exportMonoButton, QPushButton::clicked, controller, Control::exportMonoLabels);
	connect(ui->fileList, QListWidget::currentItemChanged, this, newFileSelected);
	connect(ui->autoScale, QCheckBox::stateChanged, waveDraw, waveDraw->autoScaleChanged);
  connect(ui->actionToggle_Wave_View, QAction::changed, this, toggleWaveView);
  connect(ui->actionSpectrogram_View, QAction::changed, this, toggleSpectrogramView);
  connect(ui->actionSwap_View, QAction::triggered, this, swapView);
  connect(ui->labelInput, QLineEdit::returnPressed, this, labelEntered);


  connect(ui->zoomSlider, QSlider::valueChanged, this, labelEntered);



}

SPELLMainWindow::~SPELLMainWindow()
{
    delete ui;
}

void SPELLMainWindow::labelEntered(){
    std::string str = ui->labelInput->text().toStdString();
    ui->labelInput->setText("");
    controller->updateLabelText(str);
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
        addFileToList(file_path);
        addNewTrack(); // Temp track for testing
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

void SPELLMainWindow::toggleWaveView() {
  waveDraw->setVisible(ui->actionToggle_Wave_View->isChecked());
  ui->autoScale->setVisible(ui->actionToggle_Wave_View->isChecked());
}

void SPELLMainWindow::toggleSpectrogramView() {
  spectrogramDraw->setVisible(ui->actionSpectrogram_View->isChecked());
}

void SPELLMainWindow::swapView(bool checked) {
  ui->tlLayoutWidget->setVisible(!ui->tlLayoutWidget->isVisible());
  //noteViewWidget->setVisible(!ui->tlLayoutWidget->isVisible());
}

// Save Load
