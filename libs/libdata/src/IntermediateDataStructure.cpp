#include <IntermediateDataStructure.h>
#include "AudioData.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>

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

void helloDATA() {
	std::cout << "Hello DATA!" << std::endl;
}
