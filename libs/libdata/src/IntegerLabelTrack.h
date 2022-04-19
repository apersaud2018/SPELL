#ifndef INTEGER_LABEL_TRACK
#define INTEGER_LABEL_TRACK
#include "LabelTrack.h"
#include "rapidjson/document.h"

using namespace rapidjson;

class IntegerLabelTrack : public LabelTrack{

    public:
      IntegerLabelTrack(std::string nname, bool atleast_one);
      ~IntegerLabelTrack();
      std::string get(int index);
      int getInt(int index);
      bool remove(int index);
      bool insert(double time);
      bool insert(double time, std::string str);
      bool insertInt(double time, int val);
      bool set(int index, std::string str);
      bool setInt(int index, int val);
      std::vector<TextTrackEntry> getTextLabels();
      std::string getRegex();
      Value save(Document::AllocatorType& allocator);

    private:
      bool validateInput(std::string str);
      IDSStatus load(Value& labelDefs);
};
#endif
