#ifndef LIBDATA
#define LIBDATA
#include <string>
#include <vector>
#include "../src/Label.h"
#include "../src/AudioData.h"
#include "../src/TextLabelTrack.h"
#include "../src/WordLabelTrack.h"

class IntermediateDataStructure{

    std::string projectName;
    std::string projectPath;
    std::vector<std::string> audioFiles;
    std::vector<Label> labels;
    std::vector<AudioData> audioData;
    public:
        void initialize(std::string projectName, std::string projectPath);
        bool addAudioFile(std::string path);
        std::vector<double> *getAudioData(int index);
        AudioData getAudio(int index);
    private:
        bool isValidAudioFile(std::string path);
        std::vector<std::string> validFileExtensions = {"wav"};
};


#endif
