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
        IntermediateDataStructure data;
};


#endif