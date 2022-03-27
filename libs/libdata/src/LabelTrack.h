#ifndef LABEL_TRACK_H
#define LABEL_TRACK_H
#include <string>
#include <vector>
#include "LabelType.h"
#include "IDSStatus.h"
#include "rapidjson/document.h"

using namespace rapidjson;

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
      LabelTrack(std::string nname, LabelType ltype) {
        name = nname;
        type = ltype;
      };
      std::string name;
      LabelType type;
      virtual std::string get(int index) = 0;
      virtual bool remove(int index) = 0;
      virtual bool insert(double time, std::string str) = 0;
      virtual bool set(int index, std::string str) = 0;

      // Generic enough to be put here
      virtual bool move(int index, double time) {
        int new_index = index;
        TrackEntry *temp = nullptr;

        if (index < 0 || index >= labels.size()) {
          return false;
        }

        temp = labels[index];

        if (time >= labels[labels.size()-1]->time) {
          new_index = labels.size()-1;
        }
        else if (time <= labels[0]->time) {
          new_index = 0;
        }
        else {
          for (int i = 0 ; i < labels.size()-1 ; ++i) {
            if (labels[i]->time <= time && time <= labels[i+1]->time) {
              if (i >= index) {
                new_index = i;
              }
              else {
                new_index = i+1;
              }

              break;
            }
          }
        }

        // Set time AFTER index is found or else it can map to itself
        labels[index]->time = time;

        // If new_index == index, do nothing
        if (new_index > index) {
          for (int i = index; i < new_index; ++i){
            labels[i] = labels[i+1];
          }
          labels[new_index] = temp;
        }
        else if (new_index < index) {
          for (int i = index; i > new_index; --i){
            labels[i] = labels[i-1];
          }
          labels[new_index] = temp;
        }


        return true;
      };

      virtual std::vector<TextTrackEntry> getTextLabels() = 0;
      // https://www.softwaretestinghelp.com/regex-in-cpp/
      virtual std::string getRegex() = 0;
      virtual Value save(Document::AllocatorType& allocator) = 0;
      virtual IDSStatus load(Value& labelDefs) = 0;


    protected:
      std::vector<TrackEntry*> labels;

};


#endif
