#include "AudioData.h"
#include <iostream>
#include <string>
#include <fstream>

void AudioData::readData(std::string path){
    header_t header;
    FILE *input_file;
    int header_size = sizeof(header);
    std::cout << header_size << "\n";
    
}