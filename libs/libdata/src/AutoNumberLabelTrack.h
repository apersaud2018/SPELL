#ifndef AUTO_NUMBER_LABEL_TRACK
#define AUTO_NUMBER_LABEL_TRACK
#include "LabelTrack.h"
#include "rapidjson/document.h"

using namespace rapidjson;

class AutoNumberLabelTrack : public LabelTrack{

    public:
      AutoNumberLabelTrack(std::string nname, bool atleast_one);
      ~AutoNumberLabelTrack();
      std::string get(int index);
      int getInt(int index);
      bool remove(int index);
      bool insert(double time);
      bool insert(double time, std::string str);
      bool set(int index, std::string str);
      std::vector<TextTrackEntry> getTextLabels();
      std::string getRegex();
      Value save(Document::AllocatorType& allocator);

    private:
      bool validateInput(std::string str);
      IDSStatus load(Value& labelDefs);
};
#endif
