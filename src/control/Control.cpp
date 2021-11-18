#include "Control.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <IntermediateDataStructure.h>
#include "AudioData.h"
#include <QImage>

void Control::initialize(){
    data.initialize("TestProject", "C:/test/project");
}

bool Control::addAudioFile(std::string path){
    return data.addAudioFile(path);
}

std::vector<double> Control::getAudioData(int index){
    return data.getAudioData(index);
}

void Control::computeSpectrogram(int index){
    AudioData audio = data.getAudio(index);
    int window_size = 512;
    int stride = 16;
    int num_windows = (audio.data.size()-window_size)/stride;
    
    QImage image(QSize(num_windows,512),QImage::Format_RGB32);
    image.fill(QColor("purple"));
    spectrograms.push_back(image); // May need to change to insert
    computeWindow(0, audio.data, &spectrograms[index]);
}

void Control::computeWindow(int num, std::vector<double> data, QImage *img){
    img->fill(QColor("blue"));
    
    
    
}