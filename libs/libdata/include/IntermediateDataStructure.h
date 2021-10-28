#ifndef LIBDATA
#define LIBDATA
#include <string>
#include <vector>

class IntermediateDataStructure{

    std::string projectName;
    std::string projectPath;
    std::vector<std::string> audioFiles;
    public:
        void initialize(std::string projectName, std::string projectPath);
        void addAudioFile(std::string path);

    
    
};


#endif