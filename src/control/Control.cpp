#include "Control.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <IntermediateDataStructure.h>

void Control::initialize(){
    data.initialize("TestProject", "C:/test/project");
}

bool Control::addAudioFile(std::string path){
    return data.addAudioFile(path);
}

std::vector<double> Control::getAudioData(int index){
    return data.getAudioData(index);
}