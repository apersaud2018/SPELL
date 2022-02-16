#ifndef SAMPLE_CONTAINER
#define SAMPLE_CONTAINER
#include <map>
#include "LabelTrack.h"
#include "AudioData.h"
#include "LabelType.h"

class Sample {

    public:
      Sample(std::string npath);
      //~Sample();
      LabelTrack *getLabelTrack(std::string name);
      bool makeLabelTrack(std::string name, LabelType type);
      bool removeLabelTrack(std::string name);

      std::vector<double> *getAudioData();
      AudioData getAudio();

    private:
      std::map<std::string, LabelTrack*> tracks;
      AudioData audio;
      std::string path;


};
#endif
