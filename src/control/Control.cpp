#include "Control.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <IntermediateDataStructure.h>
#include "AudioData.h"
#include <QImage>
#include <complex>
#include <thread>

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
    int stride = 64;
    int num_windows = (audio.data.size()-window_size)/stride;
    
    QImage image(QSize(num_windows,256),QImage::Format_RGB32);
    image.fill(QColor("purple"));
    spectrograms.push_back(image); // May need to change to insert
    
    std::thread compute_thread(&Control::computeAllWindows, this, num_windows, window_size, stride, index);
    compute_thread.detach();
    //compute_thread.join();
}

void Control::computeAllWindows(int num_windows, int window_size, int stride,int index){
    AudioData audio = data.getAudio(index);
    for(int i=0;i<num_windows;i++){
        computeWindow(i, window_size, stride, audio, &spectrograms[index]);
    }
}

void Control::computeWindow(int window_index, int window_size, int stride, AudioData audio, QImage *img){
    
    int first_sample = stride*window_index;
    int last_sample = first_sample + window_size;
   
    std::vector<std::complex<double>> input_data;
   
    for(int i=first_sample;i<last_sample;i++){
        input_data.push_back(std::complex<double>(audio.data[i],0.0));
    }
    std::vector<std::complex<double>> output_data = audio.fft(input_data);
    for(int i=0;i<output_data.size()/2;i++){
        int val = std::abs(output_data[i])*255;
        img->setPixel(window_index, i, qRgb(val, val, val));
    }
    
}