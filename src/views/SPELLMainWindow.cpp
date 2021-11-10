#include "SPELLMainWindow.h"
#include "./ui_SPELLMainWindow.h"
#include <iostream>
#include <QFileDialog>
#include <QMessageBox>
#include <QDir>
#include <QGraphicsScene>
#include <QListWidget>
#include <vector>
#include <QMouseEvent>
#include <cmath>
#include "../control/Control.h"

SPELLMainWindow::SPELLMainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::SPELLMainWindow)
{   
    controller.initialize();
    ui->setupUi(this);
	
    //scene = new QGraphicsScene();
    ui->waveForm->setScene(&scene);
    
  
    
    //scene.addLine(-250,0,250,0,QPen(Qt::black));
    
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
    controller.start_sample = 0;
    controller.display_samples = 10000;
    
    // Query the data structure for the correct audio data to render,
    // then render the wave form of said audio.
    renderWaveForm(controller.getAudioData(controller.file_index));
    
}

void SPELLMainWindow::renderWaveForm(std::vector<double> data){
    
    scene.clear();

    int width = ui->waveForm->mapToScene(ui->waveForm->viewport()->geometry()).boundingRect().width();
    int height = ui->waveForm->mapToScene(ui->waveForm->viewport()->geometry()).boundingRect().height();
    int num_samples_display = controller.display_samples;
    int start_sample = controller.start_sample;
    int end_sample = num_samples_display + start_sample;
    int sample_per_pixel = num_samples_display/width;
    
    double max_val = 0.0000001;
    
    
    std::vector<double> vals;
    vals.push_back(0.0);
    for(int pixel_x=0;pixel_x<width;pixel_x++){
        double val = 0.0;
        for(int i=0;i<sample_per_pixel;i++){
            int sample_index = i+start_sample+(pixel_x*sample_per_pixel);
            double sample = 0.0;
            if(sample_index >= 0 && sample_index < data.size()) {          
                sample = data[sample_index];
                
            }

            
            val += (double)sample;
            
        }
        //val = std::sqrt(val);
        val /= sample_per_pixel;
        
        if(val > max_val){
            max_val = val;
        }else if (-val > max_val){
            max_val = -val;
        }
        
        
        val *= (height/2) * 0.9;
        vals.push_back(val);

        
    }
    
    for(int i=5;i<vals.size();i++){
        scene.addLine(i-1,(int)(vals[i-1]*(1/max_val)),i,(int)(vals[i]*(1/max_val)),QPen(Qt::black));
    }


       
}

void SPELLMainWindow::wheelEvent(QWheelEvent *event){
    QPoint numPixels = event->angleDelta();
    //std::cout << "X: " << numPixels.x() << "Y: " << numPixels.y() << "\n";
    if(numPixels.y() > 0){
        controller.display_samples = (int)(controller.display_samples * (1.0-ZOOM_SPEED));
    }else if(numPixels.y() < 0){
        controller.display_samples = (int)(controller.display_samples * (1.0+ZOOM_SPEED));
    }
    
    if(controller.file_index > -1){
        if(controller.getAudioData(controller.file_index).size() < (controller.display_samples+controller.start_sample)){
            controller.display_samples = controller.getAudioData(controller.file_index).size() - controller.start_sample;
        }
        renderWaveForm(controller.getAudioData(controller.file_index));
    }
}
