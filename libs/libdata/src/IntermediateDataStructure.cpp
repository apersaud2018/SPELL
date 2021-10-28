#include <IntermediateDataStructure.h>
#include <iostream>
#include <vector>


void IntermediateDataStructure::initialize(std::string projectName, std::string projectPath){
    this->projectName = projectName;
    this->projectPath = projectPath;
    std::cout << "Initializing Project: " << projectName << " at " << projectPath << std::endl;
}

void IntermediateDataStructure::addAudioFile(std::string path){
    audioFiles.push_back(path);
    std::cout << "Added " << path << " to " << projectName << std::endl;
}


void helloDATA() {
	std::cout << "Hello DATA!" << std::endl;
}