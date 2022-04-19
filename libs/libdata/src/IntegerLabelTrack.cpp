#include "IntegerLabelTrack.h"
#include "LabelType.h"
#include <regex>

IntegerLabelTrack::IntegerLabelTrack(std::string nname, bool atleast_one) : LabelTrack(nname, INTEGER, atleast_one, true) {
  if (atleast_one) {
    insertInt(0, 0);
  }
}

IntegerLabelTrack::~IntegerLabelTrack() {
  while (labels.size() > 0) {
    remove(0);
  }
}

std::string IntegerLabelTrack::get(int index) {
  if (index < 0 || index >= labels.size()) {
    return nullptr;
  }

  return *(static_cast<std::string*>(labels[index]->data));
}

int IntegerLabelTrack::getInt(int index) {
  if (index < 0 || index >= labels.size()) {
    return 0;
  }

  return *static_cast<int*>(labels[index]->data);
}

bool IntegerLabelTrack::remove(int index) {
  if (index < 0 || index >= labels.size()) {
    return false;
  }

  if (atleast_one && index == 0) {
    return false;
  }

  delete static_cast<int*>(labels[index]->data);
  delete labels[index];
  labels.erase(labels.begin() + index);

  return true;

}

bool IntegerLabelTrack::insert(double time, std::string str) {
  //Allow digits only
  if (str.length() > 0 && !validateInput(str)) {
    return false;
  }

  int val = 0;
  if (str.length() > 0) {
    val = std::stoi(str);
  }

  insertInt(time, val);

  return true;
}

bool IntegerLabelTrack::insertInt(double time, int val) {

  TrackEntry *new_data = new TrackEntry();
  new_data->time = time;
  new_data->data = static_cast<void*>(new int(val));

  if (labels.size() == 0 || time >= labels[labels.size()-1]->time ) {
    labels.push_back(new_data);
  }
  else if (time <= labels[0]->time) {
    labels.insert(labels.begin(), new_data);
  }
  else {
    for (int i = 0 ; i < labels.size()-1 ; ++i) {
      if (labels[i]->time <= time && time <= labels[i+1]->time) {
        labels.insert(labels.begin() + i + 1, new_data);
        break;
      }
    }
  }

  return true;
}



bool IntegerLabelTrack::set(int index, std::string str) {
  if (index < 0 || index >= labels.size()) {
    return false;
  }

  if (str.length() > 0 && !validateInput(str)) {
    return false;
  }

  int val = 0;
  if (str.length() > 0) {
    val = std::stoi(str);
  }

  setInt(index, val);

  return true;
}

bool IntegerLabelTrack::setInt(int index, int val) {
  if (index < 0 || index >= labels.size()) {
    return false;
  }

  if (labels[index]->data == nullptr) {
    labels[index]->data = new int(val);
  }
  else {
    *(int*)(labels[index]->data) = val;
  }

  return true;
}

std::vector<TextTrackEntry> IntegerLabelTrack::getTextLabels() {
  TextTrackEntry temp;
  std::vector<TextTrackEntry> tlabels;
  tlabels.reserve(labels.size());

  for (int i = 0; i < labels.size(); ++i) {
    temp.time = labels[i]->time;
    temp.data = std::to_string(getInt(i));
    tlabels.push_back(temp);
  }

  return tlabels;
}

std::string IntegerLabelTrack::getRegex() {
  return "-?[0-9]+";
}

bool IntegerLabelTrack::validateInput(std::string str) {
  std::regex allInts(getRegex());
  return std::regex_match(str, allInts);
}

Value IntegerLabelTrack::save(Document::AllocatorType& allocator) {
  Value trackObj(kObjectType);
  trackObj.AddMember("type", type, allocator);
  trackObj.AddMember("atleast_one", atleast_one, allocator);

  Value labelsAr(kArrayType);
  for (int i = 0; i < labels.size(); ++i) {
    Value labelEntry(kObjectType);

    labelEntry.AddMember("pos", labels[i]->time, allocator);

    labelEntry.AddMember("value", getInt(i), allocator);

    labelsAr.PushBack(labelEntry, allocator);
  }
  trackObj.AddMember("labels", labelsAr, allocator);

  return trackObj;
}

IDSStatus IntegerLabelTrack::load(Value& labelDefs) {
  IDSStatus status = 0;
  for (int i = 0; i < labelDefs.Size(); ++i) {
    if (labelDefs[i].IsObject() &&  labelDefs[i].HasMember("pos")  &&  labelDefs[i].HasMember("value")
        && labelDefs[i]["pos"].IsDouble() && labelDefs[i]["value"].IsInt()) {
          if (atleast_one && abs(labelDefs[i]["pos"].GetDouble()) < 0.00001) {
            setInt(0, labelDefs[i]["value"].GetInt());
          }
          else {
            insertInt(labelDefs[i]["pos"].GetDouble(), labelDefs[i]["value"].GetInt());
          }
    }
    else {
      status = status | IDS_SAMPLETRACKINVALIDLABEL;
    }
  }
  return status;
}
