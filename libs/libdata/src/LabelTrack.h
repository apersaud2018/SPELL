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
      LabelTrack(std::string nname, LabelType ltype, bool one, bool entry) {
        name = nname;
        type = ltype;
        atleast_one = one;
        entry = allow_entry;
      };
      std::string name;
      LabelType type;
      bool atleast_one;
      bool allow_entry;

      virtual std::string get(int index) = 0;
      virtual bool remove(int index) = 0;
      virtual bool insert(double time) = 0;
      virtual bool insert(double time, std::string str) = 0;
      virtual bool set(int index, std::string str) = 0;

      // Generic enough to be put here
      virtual bool move(int index, double time) {
        int new_index = index;
        TrackEntry *temp = nullptr;

        if (index < 0 || index >= labels.size()) {
          return false;
        }

        if (atleast_one && index == 0) {
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

      virtual bool insertEntry(TrackEntry* entry) {
        if (labels.size() == 0 || entry->time >= labels[labels.size()-1]->time ) {
          labels.push_back(entry);
        }
        else if (entry->time <= labels[0]->time) {
          labels.insert(labels.begin(), entry);
        }
        else {
          for (int i = 0 ; i < labels.size()-1 ; ++i) {
            if (labels[i]->time <= entry->time && entry->time <= labels[i+1]->time) {
              labels.insert(labels.begin() + i + 1, entry);
              break;
            }
          }
        }
        return true;
      }

};


#endif
