#ifndef CONTROL_H
#define CONTROL_H
#include <string>
#include <vector>
#include <iostream>
#include <IntermediateDataStructure.h>
#include <QPixmap>
#include <QImage>

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
        
    private:
        void computeWindow(int num, std::vector<double> data, QImage *img);
};


#endif