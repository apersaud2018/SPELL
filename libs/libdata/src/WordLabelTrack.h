#ifndef WORD_LABEL_TRACK
#define WORD_LABEL_TRACK
#include "LabelTrack.h"

class WordLabelTrack : public LabelTrack{

    public:
      WordLabelTrack(std::string nname);
      ~WordLabelTrack();
      std::string get(int index);
      bool remove(int index);
      bool insert(double time, std::string str);
      bool set(int index, std::string str);
      bool move(int index, double time);
      std::vector<TextTrackEntry> getTextLabels();
      std::string getRegex();

    private:
      bool validateInput(std::string str);

};
#endif
