#include "SPELLMainWindow.h"
#include "./ui_SPELLMainWindow.h"
#include "TrackManager.h"
#include <iostream>
#include <QFileDialog>
#include <QMessageBox>
#include <QListWidget>
#include <vector>
#include <QCheckBox>
#include <QAbstractItemView>
#include "control/Control.h"
#include <QFile>

SPELLMainWindow::SPELLMainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::SPELLMainWindow)
{
    controller = new Control();

    //controller->initialize();
    ui->setupUi(this);
    // enable tooltips
    this->setAttribute(Qt::WA_AlwaysShowToolTips,true);

    ui->fileList->setModel(controller->audio_files);


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
	connect(ui->runMLButton, QPushButton::clicked, controller, Control::runML);
  connect(ui->fileList, QAbstractItemView::clicked, this, fileSelected);
	connect(ui->autoScale, QCheckBox::stateChanged, waveDraw, waveDraw->autoScaleChanged);
  connect(ui->labelInput, QLineEdit::returnPressed, this, labelEntered);

  // Menu bar
  //File
  connect(ui->actionSave, QAction::triggered, this, saveFile);
  connect(ui->actionSave_As, QAction::triggered, this, saveFileAs);
  connect(ui->actionopen, QAction::triggered, this, loadFile);
  //View
  connect(ui->actionToggle_Wave_View, QAction::changed, this, toggleWaveView);
  connect(ui->actionSpectrogram_View, QAction::changed, this, toggleSpectrogramView);
  connect(ui->actionSwap_View, QAction::triggered, this, swapView);
  //Tools
  connect(ui->actionLabel_Track_Manager, QAction::triggered, this, showTrackManager);

  connect(ui->zoomSlider, QSlider::valueChanged, this, changeZoom);



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
    if (controller->createPhonemeTrack()) {
      TimelineView *timelineView = new TimelineView(ui->scrollVWidget, controller, "Phonemes");
      timelineView->setFixedHeight(50);
      timelineView->setMouseTracking(true);
      ui->scrollVLayout->insertWidget(timelineViews.size(), timelineView);
      //timelineView->track = controller->phonemeTrack;
      timelineViews.push_back(timelineView);
    }
}

void SPELLMainWindow::addAudioFile() {
    QString file_path = QFileDialog::getOpenFileName(this,"Open Audio File", "", "Audio (*.wav)");
    bool success = controller->addAudioFile(file_path.toStdString());
    if(success){
        addNewTrack(); // Temp track for testing
    }
}

void SPELLMainWindow::saveFile() {
  std::string path = controller->getProjectPath();
  if (path.length() == 0 || !QFile::exists(QString::fromStdString(path))) {
    saveFileAs();
  }

  controller->save();
}

void SPELLMainWindow::saveFileAs() {
  QString file_path = QFileDialog::getSaveFileName(this,"Save project", "", "Spell Project (*.spell.json)");
  controller->saveAs(file_path.toStdString());
}

void SPELLMainWindow::loadFile() {
  QString file_path = QFileDialog::getOpenFileName(this,"Open project", "", "Spell Project (*.spell.json)");
  controller->load(file_path.toStdString());

  TimelineView *timelineView = new TimelineView(ui->scrollVWidget, controller, "Phonemes");
  timelineView->setFixedHeight(50);
  timelineView->setMouseTracking(true);
  ui->scrollVLayout->insertWidget(timelineViews.size(), timelineView);
  //timelineView->track = controller->phonemeTrack;
  timelineViews.push_back(timelineView);
}

void SPELLMainWindow::showTrackManager() {
  std::cout << "SHOW" << "\n";
  TrackManager *tm = new TrackManager(this, controller->getTracks());
  tm->exec();
}

void SPELLMainWindow::addNewTrack() {
    addTrack();
	std::cout << "Created New Track" << "\n";
}

void SPELLMainWindow::playSelection() {

	std::cout << "Play" << "\n";
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


void SPELLMainWindow::fileSelected(const QModelIndex &index) {
  //QString current_path = ui->fileList->currentItem()->text();
  std::cout << "Selected " << controller->audio_files->data(index).toString().toStdString() << "\n";
  controller->setFileIndex(index.row());
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
