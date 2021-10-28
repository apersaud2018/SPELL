#include <IntermediateDataStructure.h>
#include <iostream>

void IntermediateDataStructure::initialize(std::string projectName, std::string projectPath){
    this->projectName = projectName;
    this->projectPath = projectPath;
    std::cout << "Initializing Project: " << projectName << " at " << projectPath << std::endl;
}

void helloDATA() {
	std::cout << "Hello DATA!" << std::endl;
}