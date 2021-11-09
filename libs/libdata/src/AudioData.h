#ifndef LIBDATA_AUDIO_H
#define LIBDATA_AUDIO_H

#include <iostream>
#include <string>
#include <fstream>
#include <vector>

class AudioData{

    typedef struct HEADER{
        char RIFF_header[4];
        unsigned long RIFF_chunk_size;
        char RIFF_format[4];
        
        char fmt_header[4];
        unsigned long fmt_chunk_size;
        unsigned short fmt_format;
        unsigned short channels;
        unsigned long sample_rate;
        unsigned long byte_rate;
        unsigned short block_align;
        unsigned short sample_size_bits;
        char chunk_id[4];
        unsigned long data_size;
        
        
    }header_t;
    
    public:
        void readData(std::string path);
        unsigned long data_size;
        unsigned long sample_rate;
        std::vector<double> data;
        

};


#endif
