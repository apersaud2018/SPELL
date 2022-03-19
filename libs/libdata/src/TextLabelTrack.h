#ifndef TEXT_LABEL_TRACK
#define TEXT_LABEL_TRACK
#include "LabelTrack.h"
#include "rapidjson/document.h"

using namespace rapidjson;

class TextLabelTrack : public LabelTrack{

    public:
      TextLabelTrack(std::string nname);
      ~TextLabelTrack();
      std::string get(int index);
      bool remove(int index);
      bool insert(double time, std::string str);
      bool set(int index, std::string str);
      std::vector<TextTrackEntry> getTextLabels();
      std::string getRegex();
      Value save(Document::AllocatorType& allocator);

};
#endif
