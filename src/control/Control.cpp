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

Control::Control(){
    data.initialize("TestProject", "C:/test/project");
    std::fstream colormap_file("../colormaps/magma.cmap", std::ios_base::in);
    int a,b,c;
    while(colormap_file >> a >> b >> c){
        colormap.push_back(qRgb(a, b, c));
    }
}

bool Control::addAudioFile(std::string path){
    return data.addAudioFile(path);
}

std::vector<double> *Control::getAudioData(int index){
    return data.getAudioData(index);
}
void Control::setCursorPosition(double pos){
    if(pos > 1.0){
        cursor_pos = 1.0;
    }else{
        cursor_pos = pos;
    }
    emit updatedCursorPosition();
}
void Control::setPosition(double percent) {
  if (percent < 0) {
    left_position = 0;
  }
  else if (percent + zoom > 1) {
    left_position = 1 - zoom;
  }
  else {
    left_position = percent;
  }

  emit viewChanged();
}

void Control::setZoom(double percent) {
  double diff = zoom - percent;
  if (percent < 0.001) {
    zoom = 0.001;
    setPosition(left_position + diff/2);
  }
  else if (percent > 1) {
    zoom = 1;
    setPosition(0);
  }
  else if (percent + left_position > 1) {
    zoom = percent;
    setPosition(1 - percent);
  }
  else {
    zoom = percent;
    setPosition(left_position + diff/2);
  }

  //std::cout << "Z " << zoom << " X " << left_position << "\n";
}

int Control::getStartSample() {
  return data_size * left_position;
}

int Control::getEndSample() {
  int pos = data_size * left_position + data_size * zoom;
  if (pos >= data_size) {
    return data_size - 1;
  }
  return pos;
}


void Control::setFileIndex(int index) {
  file_index = index;
  start_sample = 0;
  left_position = 0;
  zoom = 1;
  data_size = getAudioData(index)->size();
  end_sample = data_size;
  emit fileIndexChanged(index);
}

void Control::computeSpectrogram(int index){
    AudioData audio = data.getAudio(index);
    int window_size = 512;
    int stride = 256;
    int num_windows = (audio.data.size()-window_size)/stride;

    QImage image(QSize(num_windows,window_size/2),QImage::Format_RGB32);
    image.fill(QColor("purple"));
    spectrograms.push_back(image); // May need to change to insert
    int num_threads = 1;
    for(int i = 0;i<num_threads;i++){
        std::thread compute_thread(&Control::computeAllWindows, this, num_windows, window_size, stride, index, num_threads, i);
        compute_thread.detach();
    }
    //compute_thread.join();
}

void Control::computeAllWindows(int num_windows, int window_size, int stride,int index, int step, int phase){
    AudioData audio = data.getAudio(index);
    for(int i=phase;i<num_windows;i+=step){
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
        double amplitude = std::abs(output_data[i])/window_size;
        amplitude = std::log(amplitude);
        int val = ((amplitude+20)/22)*colormap.size();
        if(val >= colormap.size()){
            val = colormap.size()-1;
        }else if(val <0){
            val = 0;
        }
        img->setPixel(window_index, 255-i, colormap[val]);
    }

}

void Control::createPhonemeTrack(){
    data.addTrack("Phonemes", Word);

    phonemeTrack = data.getLabelTrack(0, "Phonemes");
}

void Control::updateLabels(){
    emit changedLabels();
}
