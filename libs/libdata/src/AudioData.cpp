#include "AudioData.h"
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <complex>


void AudioData::readData(std::string path){
    header_t header;
    std::FILE *input_file;
    int header_size = sizeof(header);
    
    input_file = std::fopen(path.c_str(), "rb");

    if(input_file == NULL){
        std::cout << "WHY IS THIS NULL!\n";
    }
    
    // read in header_size number of bytes (44) once.
    
    int bytesRead1 = std::fread(&header, 1, sizeof(header), input_file);
    AudioData::sample_rate = header.sample_rate;
    
    std::cout << "Sample Rate: " << header.sample_rate << "Hz\n";
    
    int num_bytes = header.data_size;
    int sample_size = header.sample_size_bits;
    std::cout  << num_bytes << " Bytes\n";
    std::cout  << sample_size << " Bits per Sample\n";
    std::cout  << header.byte_rate << "\n";

    //char byte_data[num_bytes];
    //std::fread(byte_data, sizeof(byte_data), 1, input_file);
    short sample = 0;
    int num_samples =0;
    while(std::fread(&sample, 1, sample_size/8, input_file)){

        num_samples++;
        //(int)byte_data[i];
        
        //perror("fread");
        //a = byte_data[i];
        //std::cout << bytesRead << " E\n";
        double noramlized_sample = (double)sample / (double)32767;//(1 << (sample_size));
        //noramlized_sample += -1.0;
        //std::cout << sample << " \n";
        data.push_back(noramlized_sample);
        //std::cout << noramlized_sample << "\n"; 
        //data.push_back(sample);

        
        
    }
    std::cout << "Read in " << num_samples << " Samples\n";
    std::fclose(input_file);
}

std::vector<std::complex<double>> AudioData::fft(std::vector<std::complex<double>> input_data){
    int n = input_data.size(); 
    std::vector<std::complex<double>> output_data(n, std::complex<double>(0.0,0.0));
    if(n == 1){
        output_data[0] = input_data[0];
    }else{
        std::vector<std::complex<double>> even_data;
        std::vector<std::complex<double>> odd_data;
        for(int i=0;i<n;i++){
            if(i%2==0){
                even_data.push_back(input_data[i]);
            }else{
                odd_data.push_back(input_data[i]);
            }
        }
        std::vector<std::complex<double>> left_half = fft(even_data);
        std::vector<std::complex<double>> right_half = fft(odd_data);
        
        for(int k=0;k<n/2;k++){
            std::complex<double> temp = left_half[k];
            output_data[k] = temp + std::exp((-2*(3.14159265358979323846)*k*std::complex<double>(0.0,1.0))/(double)n)*right_half[k];
            output_data[k+(n/2)] = temp - std::exp((-2*(3.14159265358979323846)*k*std::complex<double>(0.0,1.0))/(double)n)*right_half[k];
            
        }
        
    }
    return output_data;
}