#include "AutoNumberLabelTrack.h"
#include "LabelType.h"
#include <regex>

AutoNumberLabelTrack::AutoNumberLabelTrack(std::string nname, bool atleast_one) : LabelTrack(nname, AUTO_NUMBER, atleast_one, false) {
  if (atleast_one) {
    insert(0);
  }
}

AutoNumberLabelTrack::~AutoNumberLabelTrack() {
  while (labels.size() > 0) {
    remove(0);
  }
}

std::string AutoNumberLabelTrack::get(int index) {
  if (index < 0 || index >= labels.size()) {
    return nullptr;
  }

  return std::to_string(index);
}

int AutoNumberLabelTrack::getInt(int index) {
  if (index < 0 || index >= labels.size()) {
    return -1;
  }

  return index;
}

bool AutoNumberLabelTrack::remove(int index) {
  if (index < 0 || index >= labels.size()) {
    return false;
  }

  if (atleast_one && index == 0) {
    return false;
  }

  delete labels[index];
  labels.erase(labels.begin() + index);

  return true;

}

bool AutoNumberLabelTrack::insert(double time) {
  return insert(time, "");
}

bool AutoNumberLabelTrack::insert(double time, std::string str) {
  //Ignore input
  TrackEntry *new_data = new TrackEntry();
  new_data->time = time;

  return insertEntry(new_data);
}


bool AutoNumberLabelTrack::set(int index, std::string str) {
  return false;
}

std::vector<TextTrackEntry> AutoNumberLabelTrack::getTextLabels() {
  TextTrackEntry temp;
  std::vector<TextTrackEntry> tlabels;
  tlabels.reserve(labels.size());

  for (int i = 0; i < labels.size(); ++i) {
    temp.time = labels[i]->time;
    temp.data = std::to_string(i);
    tlabels.push_back(temp);
  }

  return tlabels;
}

std::string AutoNumberLabelTrack::getRegex() {
  return "^$";
}

bool AutoNumberLabelTrack::validateInput(std::string str) {
  std::regex reg(getRegex());
  return std::regex_match(str, reg);
}

Value AutoNumberLabelTrack::save(Document::AllocatorType& allocator) {
  Value trackObj(kObjectType);
  trackObj.AddMember("type", type, allocator);
  trackObj.AddMember("atleast_one", atleast_one, allocator);

  Value labelsAr(kArrayType);
  for (int i = 0; i < labels.size(); ++i) {
    Value labelEntry(kObjectType);

    labelEntry.AddMember("pos", labels[i]->time, allocator);

    labelsAr.PushBack(labelEntry, allocator);
  }
  trackObj.AddMember("labels", labelsAr, allocator);

  return trackObj;
}

IDSStatus AutoNumberLabelTrack::load(Value& labelDefs) {
  IDSStatus status = 0;
  for (int i = 0; i < labelDefs.Size(); ++i) {
    if (labelDefs[i].IsObject() &&  labelDefs[i].HasMember("pos")
        && labelDefs[i]["pos"].IsDouble()) {
          if (atleast_one && !(abs(labelDefs[i]["pos"].GetDouble()) < 0.00001)) {
            insert(labelDefs[i]["pos"].GetDouble());
          }
    }
    else {
      status = status | IDS_SAMPLETRACKINVALIDLABEL;
    }
  }
  return status;
}
