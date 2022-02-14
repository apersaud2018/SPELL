#include "WordLabelTrack.h"
#include <regex>

WordLabelTrack::WordLabelTrack(std::string nname) : LabelTrack(nname) {}

WordLabelTrack::~WordLabelTrack() {
  while (labels.size() > 0) {
    remove(0);
  }
}

std::string WordLabelTrack::get(int index) {
  if (index < 0 || index >= labels.size()) {
    return nullptr;
  }

  return *(static_cast<std::string*>(labels[index]->data));
}

bool WordLabelTrack::remove(int index) {
  if (index < 0 || index >= labels.size()) {
    return false;
  }

  delete static_cast<std::string*>(labels[index]->data);
  delete labels[index];
  labels.erase(labels.begin() + index);

  return true;

}

bool WordLabelTrack::insert(double time, std::string str) {
  //Text allows any input, so no need to validate, always return true;
  if (!validateInput(str)) {
    return false;
  }


  TrackEntry *new_data = new TrackEntry();
  new_data->time = time;
  std::string *copystr = new std::string(str);
  new_data->data = static_cast<void*>(copystr);

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

bool WordLabelTrack::set(int index, std::string str) {
  if (index < 0 || index >= labels.size() || !validateInput(str)) {
    return false;
  }

  if (labels[index]->data != nullptr) {
    delete static_cast<std::string*>(labels[index]->data);
  }

  std::string *copystr = new std::string(str);
  labels[index]->data = static_cast<void*>(copystr);

  return true;
}

bool WordLabelTrack::move(int index, double time) {
  int new_index = index;
  TrackEntry *temp = nullptr;

  if (index < 0 || index >= labels.size()) {
    return false;
  }

  labels[index]->time = time;
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
        new_index = i+1;
        break;
      }
    }
  }

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
}

std::vector<TextTrackEntry> WordLabelTrack::getTextLabels() {
  TextTrackEntry temp;
  std::vector<TextTrackEntry> tlabels;
  tlabels.reserve(labels.size());

  for (int i = 0; i < labels.size(); ++i) {
    temp.time = labels[i]->time;
    temp.data = *(static_cast<std::string*>(labels[i]->data));
    tlabels.push_back(temp);
  }

  return tlabels;
}

std::string WordLabelTrack::getRegex() {
  return "[^\\s]+";
}

bool WordLabelTrack::validateInput(std::string str) {
  std::regex noSpace(getRegex());
  return std::regex_match(str, noSpace);
}
