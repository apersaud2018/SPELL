#ifndef CONTROL_H
#define CONTROL_H
#include <string>
#include <vector>
#include <iostream>
#include <IntermediateDataStructure.h>
#include <QPixmap>
#include <QImage>
#include <QRgb>

class Control{


    public:
        void initialize();
        bool addAudioFile(std::string path);
        std::vector<double> getAudioData(int index);
        std::vector<QImage> spectrograms;
        void computeSpectrogram(int index);
        IntermediateDataStructure data;
        int file_index = -1;
        int start_sample = 0;
        int end_sample = 0;
        std::vector<QRgb> colormap;

        
    private:
        void computeWindow(int num, int window_size, int stride, AudioData audio, QImage *img);
        void computeAllWindows(int num_windows, int window_size, int stride, int index);
        
};


#endif