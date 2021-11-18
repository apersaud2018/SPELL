#ifndef CONTROL_H
#define CONTROL_H
#include <string>
#include <vector>
#include <iostream>
#include <IntermediateDataStructure.h>

class Control{


    public:
        void initialize();
        bool addAudioFile(std::string path);
        std::vector<double> getAudioData(int index);
        void test_fft();
        IntermediateDataStructure data;
        int file_index = -1;
        int start_sample = 0;
        int end_sample = 0;
};


#endif