#include "Sample.h"
#include "LabelType.h"
#include "TextLabelTrack.h"
#include "WordLabelTrack.h"


Sample::Sample(std::string npath) {
  audio.readData(npath);
  path = std::string(npath);
}

// ~Sample() {
//   // TODO: go through cleanup procedure
// }

LabelTrack *Sample::getLabelTrack(std::string name){
  auto it = tracks.find(name);
  if (it != tracks.end()) {
    return it->second;
  }
  else {
    return nullptr;
  }
}

bool Sample::makeLabelTrack(std::string name, LabelType type){
  auto it = tracks.find(name);
  if (it != tracks.end()) {
    return false;
  }

  switch (type) {
    case Word:
      tracks[name] = new WordLabelTrack(name);
      return true;
      break;
    case Text:
      tracks[name] = new TextLabelTrack(name);
      return true;
      break;
    case Numerical:
      return false;
      break;
    case AutoNumber:
      return false;
      break;
    case Note:
      return false;
      break;
    default:
      return false;
  }
}


bool Sample::removeLabelTrack(std::string name) {
  auto it = tracks.find(name);
  if (it != tracks.end()) {
    tracks.erase(it);
    return true;
  }
  return false;
}

std::vector<double> *Sample::getAudioData() {
  return &(audio.data);
}

AudioData Sample::getAudio() {
  return audio;
}