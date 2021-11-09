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
    
    std::cout << AudioData::sample_rate << "\n";
}