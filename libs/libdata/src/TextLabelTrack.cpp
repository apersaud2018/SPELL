#include "LabelType.h"
#include "TextLabelTrack.h"

TextLabelTrack::TextLabelTrack(std::string nname, bool atleast_one) : LabelTrack(nname, TEXT, atleast_one, true) {
  if (atleast_one) {
    insert(0, "");
  }
}

TextLabelTrack::~TextLabelTrack() {
  while (labels.size() > 0) {
    remove(0);
  }
}

std::string TextLabelTrack::get(int index) {
  if (index < 0 || index >= labels.size()) {
    return nullptr;
  }

  return *(static_cast<std::string*>(labels[index]->data));
}

bool TextLabelTrack::remove(int index) {
  if (index < 0 || index >= labels.size()) {
    return false;
  }

  if (atleast_one && index == 0) {
    return false;
  }

  delete static_cast<std::string*>(labels[index]->data);
  delete labels[index];
  labels.erase(labels.begin() + index);

  return true;

}

bool TextLabelTrack::insert(double time, std::string str) {
  //Text allows any input, so no need to validate, always return true;

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

bool TextLabelTrack::set(int index, std::string str) {
  if (index < 0 || index >= labels.size()) {
    return false;
  }

  if (labels[index]->data != nullptr) {
    delete static_cast<std::string*>(labels[index]->data);
  }

  std::string *copystr = new std::string(str);
  labels[index]->data = static_cast<void*>(copystr);

  return true;
}



std::vector<TextTrackEntry> TextLabelTrack::getTextLabels() {
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

std::string TextLabelTrack::getRegex() {
  return ".*";
}

Value TextLabelTrack::save(Document::AllocatorType& allocator) {
  Value trackObj(kObjectType);
  trackObj.AddMember("type", type, allocator);
  trackObj.AddMember("atleast_one", atleast_one, allocator);

  Value labelsAr(kArrayType);
  for (int i = 0; i < labels.size(); ++i) {
    Value labelEntry(kObjectType);

    labelEntry.AddMember("pos", labels[i]->time, allocator);

    Value labelValue;
    labelValue.SetString(get(i).c_str(), get(i).length(), allocator);
    labelEntry.AddMember("value", labelValue, allocator);

    labelsAr.PushBack(labelEntry, allocator);
  }
  trackObj.AddMember("labels", labelsAr, allocator);

  return trackObj;
}

IDSStatus TextLabelTrack::load(Value& labelDefs) {
  IDSStatus status = 0;
  for (int i = 0; i < labelDefs.Size(); ++i) {
    if (labelDefs[i].IsObject() &&  labelDefs[i].HasMember("pos")  &&  labelDefs[i].HasMember("value")
        && labelDefs[i]["pos"].IsDouble() && labelDefs[i]["value"].IsString()) {
          if (atleast_one && abs(labelDefs[i]["pos"].GetDouble()) < 0.00001) {
            set(0, labelDefs[i]["value"].GetString());
          }
          else {
            insert(labelDefs[i]["pos"].GetDouble(), labelDefs[i]["value"].GetString());
          }
    }
    else {
      status = status | IDS_SAMPLETRACKINVALIDLABEL;
    }
  }

  return status;
}
