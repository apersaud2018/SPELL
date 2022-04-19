#include "Sample.h"
#include "LabelType.h"
#include "TextLabelTrack.h"
#include "WordLabelTrack.h"
#include "IntegerLabelTrack.h"


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

bool Sample::makeLabelTrack(std::string name, LabelType type, bool atleast_one){
  auto it = tracks.find(name);
  if (it != tracks.end()) {
    return false;
  }

  switch (type) {
    case WORD:
      tracks[name] = new WordLabelTrack(name, atleast_one);
      return true;
      break;
    case TEXT:
      tracks[name] = new TextLabelTrack(name, atleast_one);
      return true;
      break;
    case INTEGER:
      tracks[name] = new IntegerLabelTrack(name, atleast_one);
      return true;
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

IDSStatus Sample::load(Value& trackDefs) {
  IDSStatus status = 0;

  for (Value::MemberIterator itr = trackDefs.MemberBegin(); itr != trackDefs.MemberEnd(); ++itr) {
    if (itr->value.HasMember("type") && itr->value["type"].IsUint()
        && itr->value.HasMember("atleast_one") && itr->value["atleast_one"].IsBool()
        && makeLabelTrack(itr->name.GetString(), itr->value["type"].GetUint(), itr->value["atleast_one"].GetBool())) {
      if (itr->value.HasMember("labels") && itr->value["labels"].IsArray()) {
        LabelTrack *ntrack = getLabelTrack(itr->name.GetString());
        IDSStatus temp = ntrack->load(itr->value["labels"]);
        status = status | temp;

        if (temp & IDS_SAMPLETRACKINVALIDLABEL) {
          std::cout << "Skipped 1 or more labels on track " << itr->name.GetString() << " in sample " << path << std::endl;
        }
      }
      else {
        std::cout << "Track " << itr->name.GetString() << " has missing or invalid labels on sample " << path << std::endl;
        status = status | IDS_SAMPLETRACKNOLABELS;
      }
    }
    else {
      std::cout << "Track " << itr->name.GetString() << " has invalid type on sample " << path << std::endl;
      status = status | IDS_SAMPLETRACKINVALIDTYPE;
    }

  }

  return status;
}

std::vector<double> *Sample::getAudioData() {
  return &(audio.data);
}

AudioData Sample::getAudio() {
  return audio;
}
