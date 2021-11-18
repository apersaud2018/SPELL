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
#include <QPen>
#include <QColor>
#include <QCheckBox>
#include <QPolygonF>
#include <QTimer>
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
    ui->fullWaveForm->setScene(&fullWaveScene);
    ui->spectrogram->setScene(&specScene);

    QTimer *timer = new QTimer(this);
    connect(timer, QTimer::timeout, this, updateUI);
    timer->start(1000);

    //ui->scrollAreaWidgetContents_2->viewport()->setMouseTracking(true);
    
    //scene.addLine(-250,0,250,0,QPen(Qt::black));
    
	connect(ui->addAudioFile, QPushButton::clicked, this, addAudioFile);
	connect(ui->playButton, QPushButton::clicked, this, playSelection);
	connect(ui->fileList, QListWidget::currentItemChanged, this, newFileSelected);
	connect(ui->autoScale, QCheckBox::stateChanged, this, autoScrollChanged);
}

SPELLMainWindow::~SPELLMainWindow()
{
    delete ui;
}

// 

void SPELLMainWindow::addAudioFile() {
    QString file_path = QFileDialog::getOpenFileName(this,"Open Audio File", "C:/test/audio");
    bool success = controller.addAudioFile(file_path.toStdString());
    if(success){       
        controller.computeSpectrogram(controller.spectrograms.size());
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

void SPELLMainWindow::autoScrollChanged(int val){
    SPELLMainWindow::auto_scale = val;
    if(controller.file_index > -1){
        renderWaveForm(controller.getAudioData(controller.file_index));
        renderFullWaveForm(controller.getAudioData(controller.file_index));
    }
}

void SPELLMainWindow::newFileSelected() {
    
    QString current_path = ui->fileList->currentItem()->text();
    std::cout << "Selected " << current_path.toStdString() << "\n";
    controller.file_index = ui->fileList->currentRow();
    controller.start_sample = 0;
    controller.end_sample = 10000;
    
    // Query the data structure for the correct audio data to render,
    // then render the wave form of said audio.
    renderWaveForm(controller.getAudioData(controller.file_index));
    renderFullWaveForm(controller.getAudioData(controller.file_index));
    renderSpectrogram(controller.spectrograms[controller.file_index]);
    
}

void SPELLMainWindow::renderWaveForm(std::vector<double> data){
    
    scene.clear();

    int width = ui->waveForm->mapToScene(ui->waveForm->viewport()->geometry()).boundingRect().width();
    int height = ui->waveForm->mapToScene(ui->waveForm->viewport()->geometry()).boundingRect().height();
    
    scene.setSceneRect(0,-height/2,width,height);
    
   
    int start_sample = controller.start_sample;
    int end_sample = controller.end_sample;
    int num_samples_display = end_sample-start_sample;
    double sample_per_pixel = num_samples_display*1.0/width;
    //std::cout << sample_per_pixel << "\n";
    double max_val = 0.0000001;
    
    
    std::vector<double> max_vals;
    std::vector<double> min_vals;
    max_vals.push_back(0.0);
    min_vals.push_back(0.0);
    for(int pixel_x=0;pixel_x<width;pixel_x++){
        double val = 0.0;
        double max_sample = 0.0;
        double min_sample = 0.0;
        for(int i=0;i<(int)sample_per_pixel;i++){
            int sample_index = (int)(i+start_sample+(pixel_x*sample_per_pixel));
            double sample = 0.0;
            if(sample_index >= 0 && sample_index < data.size()) {          
                sample = data[sample_index];
                
            }
            
            if(sample > max_sample){
                max_sample = (double)sample;
            }
            
            if(sample < min_sample){
                min_sample = (double)sample;
            }
            
            //val += (double)sample;
            
        }
        //val = std::sqrt(val);
        //val /= sample_per_pixel;
        if(pixel_x > 4){
            if(max_sample > max_val){
                max_val = max_sample;
            }else if (-min_sample > max_val){
                max_val = -min_sample;
            }
        }
        
        min_sample *= (height/2) * 0.8;
        max_sample *= (height/2) * 0.8;
        //vals.push_back(val);
        max_vals.push_back(max_sample);
        min_vals.push_back(min_sample);

        
    }
    QPen pen;
    QColor color(Qt::blue);
    color.setAlpha(100);
    pen.setColor(color);
    pen.setWidth(2);
    
    if(!SPELLMainWindow::auto_scale){
        max_val = 1;
    }
    
    for(int i=5;i<max_vals.size();i++){
    //scene.addLine(i-1,(int)(max_vals[i-1]*(1/max_val)),i,(int)(max_vals[i]*(1/max_val)),QPen(Qt::black));
    //scene.addLine(i-1,(int)(min_vals[i-1]*(1/max_val)),i,(int)(min_vals[i]*(1/max_val)),QPen(Qt::black));
        scene.addLine(i,(int)(min_vals[i]*(1/max_val)),i,(int)(max_vals[i]*(1/max_val)),pen);
    }


       
}


void SPELLMainWindow::renderFullWaveForm(std::vector<double> data){
    
    fullWaveScene.clear();

    int width = ui->fullWaveForm->mapToScene(ui->fullWaveForm->viewport()->geometry()).boundingRect().width();
    int height = ui->fullWaveForm->mapToScene(ui->fullWaveForm->viewport()->geometry()).boundingRect().height();
    
    fullWaveScene.setSceneRect(0,-height/2,width,height);
    
    int num_samples_display = data.size();
    int start_sample = 0;
    int end_sample = controller.end_sample;
    double sample_per_pixel = num_samples_display*1.0/width;
    //std::cout << sample_per_pixel << "\n";
    double max_val = 0.0000001;
    
    
    std::vector<double> max_vals;
    std::vector<double> min_vals;
    max_vals.push_back(0.0);
    min_vals.push_back(0.0);
    for(int pixel_x=0;pixel_x<width;pixel_x++){
        double val = 0.0;
        double max_sample = 0.0;
        double min_sample = 0.0;
        for(int i=0;i<(int)sample_per_pixel;i++){
            int sample_index = (int)(i+start_sample+(pixel_x*sample_per_pixel));
            double sample = 0.0;
            if(sample_index >= 0 && sample_index < data.size()) {          
                sample = data[sample_index];
                
            }
            
            if(sample > max_sample){
                max_sample = (double)sample;
            }
            
            if(sample < min_sample){
                min_sample = (double)sample;
            }
            
            //val += (double)sample;
            
        }
        //val = std::sqrt(val);
        //val /= sample_per_pixel;
        if(pixel_x > 4){
            if(max_sample > max_val){
                max_val = max_sample;
            }else if (-min_sample > max_val){
                max_val = -min_sample;
            }
        }
        
        min_sample *= (height/2) * 0.8;
        max_sample *= (height/2) * 0.8;
        //vals.push_back(val);
        max_vals.push_back(max_sample);
        min_vals.push_back(min_sample);

        
    }
    QPen pen;
    QColor color(Qt::blue);
    color.setAlpha(100);
    pen.setColor(color);
    pen.setWidth(2);
    
    if(!SPELLMainWindow::auto_scale){
        max_val = 1;
    }
    
    for(int i=5;i<max_vals.size();i++){
    //scene.addLine(i-1,(int)(max_vals[i-1]*(1/max_val)),i,(int)(max_vals[i]*(1/max_val)),QPen(Qt::black));
    //scene.addLine(i-1,(int)(min_vals[i-1]*(1/max_val)),i,(int)(min_vals[i]*(1/max_val)),QPen(Qt::black));
        fullWaveScene.addLine(i,(int)(min_vals[i]*(1/max_val)),i,(int)(max_vals[i]*(1/max_val)),pen);
    }
    
    int start_x = width * controller.start_sample / data.size();
    int end_x = width * (controller.end_sample) / data.size();
    
    QPolygonF poly;
    poly << QPointF(start_x, -height/2) << QPointF(start_x, +height/2-1) << QPointF(end_x, +height/2-1)<< QPointF(end_x, -height/2);
    QBrush brush;
    QColor brush_color(Qt::red);
    brush_color.setAlpha(50);
    brush.setColor(brush_color);
    brush.setStyle(Qt::SolidPattern);
    
    QPen pen2(Qt::red);
    fullWaveScene.addPolygon(poly, pen2, brush);



       
}

void SPELLMainWindow::renderSpectrogram(QImage img){
    
    specScene.clear();

    int width = ui->spectrogram->mapToScene(ui->spectrogram->viewport()->geometry()).boundingRect().width();
    int height = ui->spectrogram->mapToScene(ui->spectrogram->viewport()->geometry()).boundingRect().height();
    
    specScene.setSceneRect(0,0,width,height);
    
    specScene.addPixmap(QPixmap::fromImage(img).scaled(width, height, Qt::IgnoreAspectRatio, Qt::FastTransformation));
        
    
}

void SPELLMainWindow::wheelEvent(QWheelEvent *event){
    QPoint numPixels = event->angleDelta();
    //std::cout << "X: " << numPixels.x() << "Y: " << numPixels.y() << "\n";
    if(numPixels.y() > 0){
        controller.end_sample = (int)((controller.end_sample - controller.start_sample) * (1.0-ZOOM_SPEED)) + controller.start_sample;
    }else if(numPixels.y() < 0){
        controller.end_sample = (int)((controller.end_sample - controller.start_sample) * (1.0+ZOOM_SPEED)) + controller.start_sample;
    }
    
    if(numPixels.x() > 0){
        int delta = (int)((controller.end_sample- controller.start_sample)*SCROLL_SPEED);
        controller.start_sample += delta;
        controller.end_sample += delta;
    }else if(numPixels.x() < 0){
        int delta = (int)((controller.end_sample- controller.start_sample)*SCROLL_SPEED);
        controller.start_sample -= delta;
        controller.end_sample -= delta;
    }
    if(controller.start_sample < 0){
        controller.start_sample = 0;
    }
    
    
    
    if(controller.file_index > -1){
        int sample_len = controller.getAudioData(controller.file_index).size();
        if(sample_len < (controller.end_sample)){
            controller.end_sample = sample_len;
        }
        
        
        renderWaveForm(controller.getAudioData(controller.file_index));
        renderFullWaveForm(controller.getAudioData(controller.file_index));
        renderSpectrogram(controller.spectrograms[controller.file_index]);
    }
    

    
}

void SPELLMainWindow::mouseMoveEvent(QMouseEvent *event){
    // Handle click and drag of waveform window
    QPoint point = ui->fullWaveForm->mapFromGlobal(QCursor::pos());
    QPointF relative_point= ui->fullWaveForm->mapToScene(point);
    int width = ui->fullWaveForm->mapToScene(ui->fullWaveForm->viewport()->geometry()).boundingRect().width();
    int height = ui->fullWaveForm->mapToScene(ui->fullWaveForm->viewport()->geometry()).boundingRect().height();
    int mx = relative_point.x() + width/2;
    int my = relative_point.y();
    //if(mx > 0 && mx < width && my > 0 && my < height){
        //std::cout << mx << ", ";
        //std::cout << my << "\n";
    //}
    
}

void SPELLMainWindow::updateUI(){
    if(controller.file_index > -1){
        renderSpectrogram(controller.spectrograms[controller.file_index]);
    }
}


void SPELLMainWindow::resizeEvent(QResizeEvent *event){
	if(controller.file_index > -1){
        renderWaveForm(controller.getAudioData(controller.file_index));
        renderFullWaveForm(controller.getAudioData(controller.file_index));
        renderSpectrogram(controller.spectrograms[controller.file_index]);
    }
}


