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
    QPixmap pixmap(100, 100);
    pixmap.fill(QColor("purple"));
    spectrograms.push_back(pixmap); // May need to change to insert
    //pixmap.fill(QColor("red"));
}