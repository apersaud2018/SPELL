#ifndef TEXT_LABEL_TRACK
#define TEXT_LABEL_TRACK
#include "LabelTrack.h"

class TextLabelTrack : public LabelTrack{

    public:
      TextLabelTrack(std::string nname);
      ~TextLabelTrack();
      std::string get(int index);
      bool remove(int index);
      bool insert(double time, std::string str);
      bool set(int index, std::string str);
      bool move(int index, double time);
      std::vector<TextTrackEntry> getTextLabels();
      std::string getRegex();

};
#endif
