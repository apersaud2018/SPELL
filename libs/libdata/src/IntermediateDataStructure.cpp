#include <IntermediateDataStructure.h>
#include "AudioData.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include "rapidjson/document.h"
#include "rapidjson/prettywriter.h"
#include <rapidjson/ostreamwrapper.h>

using namespace rapidjson;

void IntermediateDataStructure::initialize(std::string projectName, std::string projectPath){
    this->projectName = projectName;
    this->projectPath = projectPath;
    std::cout << "Initializing Project: " << projectName << " at " << projectPath << std::endl;
}

bool IntermediateDataStructure::addAudioFile(std::string path){
    bool validPath = isValidAudioFile(path);
    if(validPath){
        std::cout << "Added " << path << " to " << projectName << std::endl;
        samples.push_back(Sample(path));
        return true;
    }else{
        std::cout << "ERROR: Failed to add " << path << " to " << projectName << " (Invalid Path)" << std::endl;
        return false;
    }
}

bool IntermediateDataStructure::isValidAudioFile(std::string path){
    std::ifstream file;
    file.open(path);
    bool exists = false;
    if(file){
        exists = true;
    }
    if(exists){
        std::string fileExtension = path.substr(path.find_last_of(".")+1);
        bool isValid = std::find(validFileExtensions.begin(), validFileExtensions.end(), fileExtension) != validFileExtensions.end();
        return isValid;
    }
    return false;

}

std::vector<double> *IntermediateDataStructure::getAudioData(int index){
    return samples[index].getAudioData();
}

AudioData IntermediateDataStructure::getAudio(int index){
    return samples[index].getAudio();
}

bool IntermediateDataStructure::addTrack(std::string name, LabelType type) {
  for (int i = 0; i < tracks.size(); ++i) {
    if (name == tracks[i].name) {
      return false;
    }
  }

  TrackDefs new_track = {name, type};
  tracks.push_back(new_track);

  return true;
}

bool IntermediateDataStructure::removeTrack(std::string name) {
  for (int i = 0; i < samples.size(); ++i) {
    samples[i].removeLabelTrack(name);
  }
  return true;
}

LabelTrack *IntermediateDataStructure::getLabelTrack(int index, std::string name) {
  LabelTrack *lt = samples[index].getLabelTrack(name);
  if (lt == nullptr) {
    for (int i = 0; i < tracks.size(); ++i) {
      if (name == tracks[i].name) {
        samples[index].makeLabelTrack(tracks[i].name, tracks[i].type);
        break;
      }
    }
    lt = samples[index].getLabelTrack(name);
  }
  return lt;
}

void IntermediateDataStructure::save() {

  // Init json doc
  Document doc;
  doc.SetObject();

  // Insert project name
  Value projName;
  projName.SetString(this->projectName.c_str(), this->projectName.length(), doc.GetAllocator());
  doc.AddMember("name", projName, doc.GetAllocator());

  // Add track defs
  Value trackAr(kArrayType);
  for (int i = 0; i < tracks.size(); ++i) {
    Value trackEntry(kObjectType);

    Value trackName;
    trackName.SetString(tracks[i].name.c_str(), tracks[i].name.length(), doc.GetAllocator());
    trackEntry.AddMember("name", trackName, doc.GetAllocator());

    trackEntry.AddMember("type", tracks[i].type, doc.GetAllocator());

    trackAr.PushBack(trackEntry, doc.GetAllocator());
  }
  doc.AddMember("tracks", trackAr, doc.GetAllocator());

  // Add samples
  Value sampleAr(kArrayType);
  for (int i = 0; i < samples.size(); ++i) {
    sampleAr.PushBack(samples[i].save(doc.GetAllocator()), doc.GetAllocator());
  }
  doc.AddMember("samples", sampleAr, doc.GetAllocator());

  // Save
  std::ofstream ofs(this->projectPath);
  OStreamWrapper osw(ofs);
  PrettyWriter<OStreamWrapper> writer(osw);
  doc.Accept(writer);

}

// pr->document.Parse<kParseFullPrecisionFlag>(json);

void helloDATA() {
	std::cout << "Hello DATA!" << std::endl;
}
