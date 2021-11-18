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

void Control::test_fft(){
    std::vector<std::complex<double>> test_data;
    
    test_data.push_back(std::complex<double>(1.0,0.0));
    test_data.push_back(std::complex<double>(5.0,0.0));
    test_data.push_back(std::complex<double>(2.0,0.0));
    test_data.push_back(std::complex<double>(7.0,0.0));
    AudioData test_obj;
    std::vector<std::complex<double>> output = test_obj.fft(test_data);
    std::cout << output[0] << output[1] << output[2] << output[3] << "\n";
}