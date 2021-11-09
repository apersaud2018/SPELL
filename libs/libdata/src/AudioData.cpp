#include "AudioData.h"
#include <iostream>
#include <string>
#include <fstream>


void AudioData::readData(std::string path){
    header_t header;
    FILE *input_file;
    int header_size = sizeof(header);
    
    input_file = std::fopen(path.c_str(), "r");
    // read in header_size number of bytes (44) once.
    std::fread(&header, header_size, 1, input_file);
    
    AudioData::sample_rate = header.sample_rate;
    
    std::cout << "Sample Rate: " << AudioData::sample_rate << "Hz\n";
    
    int num_samples = header.data_size;
    int sample_size = header.sample_size_bits;
    std::cout  << num_samples << " Samples\n";
    std::cout  << sample_size << " Bits per Sample\n";
    

    
    for(int i=0;i<num_samples;i++){
        int sample = 0;
        std::fread(&sample, sample_size/8, 1, input_file);
        double noramlized_sample = (double)sample / (double)(1 << (sample_size-1));
        noramlized_sample += 1.0;
        data.push_back(noramlized_sample);
    }
    
    
}