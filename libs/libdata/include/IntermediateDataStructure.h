#ifndef LIBDATA
#define LIBDATA
#include <string>
#include <vector>
#include <Label.h>

class IntermediateDataStructure{

    std::string projectName;
    std::string projectPath;
    std::vector<std::string> audioFiles;
    std::vector<Label> labels;
    public:
        void initialize(std::string projectName, std::string projectPath);
        void addAudioFile(std::string path);

    
    
};


#endif