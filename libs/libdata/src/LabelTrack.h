#ifndef LABEL_TRACK_H
#define LABEL_TRACK_H
#include <string>
#include <vector>

struct TrackEntry {
  double time;
  void *data;
};

struct TextTrackEntry {
  double time;
  std::string data;
};

class LabelTrack {

    public:
      LabelTrack(std::string nname) {
        name = nname;
      };
      std::string name;
      virtual std::string get(int index) = 0;
      virtual bool remove(int index) = 0;
      virtual bool insert(double time, std::string str) = 0;
      virtual bool set(int index, std::string str) = 0;
      virtual bool move(int index, double time) = 0;
      virtual std::vector<TextTrackEntry> getTextLabels() = 0;
      // https://www.softwaretestinghelp.com/regex-in-cpp/
      virtual std::string getRegex() = 0;


    protected:
      std::vector<TrackEntry*> labels;

};


#endif
