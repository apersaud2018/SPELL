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
    case WORD:
      tracks[name] = new WordLabelTrack(name);
      return true;
      break;
    case TEXT:
      tracks[name] = new TextLabelTrack(name);
      return true;
      break;
    case INTEGER:
      return false;
      break;
    case REAL:
      return false;
      break;
    case AUTO_NUMBER:
      return false;
      break;
    case NOTE:
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

Value Sample::save(Document::AllocatorType& allocator) {
  Value sampleObj(kObjectType);

  Value samplePath;
  samplePath.SetString(path.c_str(), path.length(), allocator);
  sampleObj.AddMember("path", samplePath, allocator);

  Value tracksObj(kObjectType);
  for (auto iter = tracks.begin(); iter != tracks.end(); ++iter) {
    std::string tname = iter->first;
    LabelTrack *lt = iter->second;
    Value tval = lt->save(allocator);
    tracksObj.AddMember(Value(tname.c_str(), allocator).Move(), tval, allocator);
  }

  sampleObj.AddMember("tracks", tracksObj, allocator);

  return sampleObj;
}

std::vector<double> *Sample::getAudioData() {
  return &(audio.data);
}

AudioData Sample::getAudio() {
  return audio;
}
