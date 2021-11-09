#include <IntermediateDataStructure.h>
#include <iostream>
#include <vector>
#include <Label.h>
#include <fstream>
#include <algorithm>

void IntermediateDataStructure::initialize(std::string projectName, std::string projectPath){
    this->projectName = projectName;
    this->projectPath = projectPath;
    std::cout << "Initializing Project: " << projectName << " at " << projectPath << std::endl;
}

bool IntermediateDataStructure::addAudioFile(std::string path){
    bool validPath = isValidAudioFile(path);
    if(validPath){
        audioFiles.push_back(path);
        std::cout << "Added " << path << " to " << projectName << std::endl;
        return true;
    }else{
        std::cout << "ERROR: Failed to add " << path << " to " << projectName << " (Invalid Path)" << std::endl;
        return false;
    }
}

bool IntermediateDataStructure::isValidAudioFile(std::string path){
    std::ifstream file;
    file.open(path);
    bool exists = false;
    if(file){
        exists = true;
    }
    if(exists){
        std::string fileExtension = path.substr(path.find_last_of(".")+1);
        bool isValid = std::find(validFileExtensions.begin(), validFileExtensions.end(), fileExtension) != validFileExtensions.end();
        return isValid;
    }
    return false;
    
}

void helloDATA() {
	std::cout << "Hello DATA!" << std::endl;
}