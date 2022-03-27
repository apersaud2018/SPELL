#ifndef SAMPLE_CONTAINER
#define SAMPLE_CONTAINER
#include <map>
#include "LabelTrack.h"
#include "AudioData.h"
#include "LabelType.h"
#include "IDSStatus.h"
#include "rapidjson/document.h"

using namespace rapidjson;

class Sample {

    public:
      Sample(std::string npath);
      //~Sample();
      LabelTrack *getLabelTrack(std::string name);
      bool makeLabelTrack(std::string name, LabelType type);
      bool removeLabelTrack(std::string name);

      Value save(Document::AllocatorType& allocator);
      IDSStatus load(Value& trackDefs);

      std::vector<double> *getAudioData();
      AudioData getAudio();

    private:
      std::map<std::string, LabelTrack*> tracks;
      AudioData audio;
      std::string path;


};
#endif
