#ifndef LIBDATA
#define LIBDATA
#include <string>
#include <vector>
#include "../src/AudioData.h"
#include "../src/Sample.h"
#include "../src/LabelType.h"
#include "../src/TextLabelTrack.h"
#include "../src/WordLabelTrack.h"

struct TrackDefs {
  std::string name;
  LabelType type;
};

class IntermediateDataStructure{
    public:
        void initialize(std::string projectName, std::string projectPath);
        bool addAudioFile(std::string path);
        std::vector<double> *getAudioData(int index);
        AudioData getAudio(int index);

        bool addTrack(std::string name, LabelType type);
        bool removeTrack(std::string name);
        LabelTrack *getLabelTrack(int index, std::string name);

        void save();

        std::string projectName;
        std::string projectPath;
        std::vector<Sample> samples;
        std::vector<TrackDefs> tracks;

    private:
        bool isValidAudioFile(std::string path);
        std::vector<std::string> validFileExtensions = {"wav"};
};


#endif
