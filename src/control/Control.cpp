#include "Control.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <IntermediateDataStructure.h>
#include "AudioData.h"
#include <QImage>
#include <complex>
#include <thread>
#include <fftw3.h>
#include <math.h>
#include <QModelIndex>
#include <QtConcurrent/QtConcurrent>

Control::Control(){
    data.initialize("TestProject", "");
    std::fstream colormap_file("../colormaps/magma.cmap", std::ios_base::in);
    int a,b,c;
    cmap_count = 0;
    while(colormap_file >> a >> b >> c){
        colormap.push_back(qRgb(a, b, c));
        cmap_count++;
    }

    std::cout << "Colors in map: " << cmap_count << "\n";

    fft_frame = (double*) fftw_malloc(sizeof(double)*FFT_LEN);
    fft_out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * FFT_LEN);
    plan = fftw_plan_dft_r2c_1d(FFT_LEN, fft_frame, fft_out, FFTW_MEASURE);

    audio_files = new QStringListModel(this);
}

bool Control::addAudioFile(std::string path){
    bool success = data.addAudioFile(path);;

    if(success){
      if(audio_files->insertRow(audio_files->rowCount())) {
        QModelIndex index = audio_files->index(audio_files->rowCount() - 1, 0);
        audio_files->setData(index, QString::fromStdString(path));
      }

      std::vector<double> *audio = data.getAudioData(data.samples.size()-1);
      int nframes = audio->size() / FFT_HOP;

      QImage image(QSize(nframes,FFT_HALF) ,QImage::Format_RGB32);
      image.fill(QColor("purple"));
      spectrograms.push_back(image); // May need to change to insert

      QFuture<void> ft = QtConcurrent::run(&Control::generateSpectrogram, this, spectrograms.size()-1);
      //controller->computeSpectrogram(controller->spectrograms.size());
    }

    return success;
}

std::vector<double> *Control::getAudioData(int index){
    return data.getAudioData(index);
}
void Control::setCursorPosition(double pos){
    if(pos > 1.0){
        cursor_pos = 1.0;
    }else{
        cursor_pos = pos;
    }
    emit updatedCursorPosition();
}
void Control::setPosition(double percent) {
  if (percent < 0) {
    left_position = 0;
  }
  else if (percent + zoom > 1) {
    left_position = 1 - zoom;
  }
  else {
    left_position = percent;
  }

  emit viewChanged();
}

void Control::setZoom(double percent) {
  double diff = zoom - percent;
  if (percent < 0.001) {
    zoom = 0.001;
    setPosition(left_position + diff/2);
  }
  else if (percent > 1) {
    zoom = 1;
    setPosition(0);
  }
  else if (percent + left_position > 1) {
    zoom = percent;
    setPosition(1 - percent);
  }
  else {
    zoom = percent;
    setPosition(left_position + diff/2);
  }

  //std::cout << "Z " << zoom << " X " << left_position << "\n";
}

int Control::getStartSample() {
  return data_size * left_position;
}

int Control::getEndSample() {
  int pos = data_size * left_position + data_size * zoom;
  if (pos >= data_size) {
    return data_size - 1;
  }
  return pos;
}


void Control::setFileIndex(int index) {
  file_index = index;
  start_sample = 0;
  left_position = 0;
  zoom = 1;
  data_size = getAudioData(index)->size();
  end_sample = data_size;
  emit fileIndexChanged(index);
}

void Control::generateSpectrogram(int index) {
  std::vector<double> *audio = data.getAudioData(index);
  int nframes = audio->size() / FFT_HOP;
  double *spec = (double*) fftw_malloc(sizeof(double)*FFT_BINS*nframes);
  double *powr = (double*) fftw_malloc(sizeof(double)*nframes);

  double mxpow = 0.01;
  double mnpow = 0;

  double mn = 100;
  double mx = -100;
  // Frame loop
  double accume;
  for(int i = 0; i < nframes; ++i){
    // Copy data
    accume = 0;
    int center = i * FFT_HOP;
    for(int delta = -FFT_HALF; delta < FFT_HALF; ++delta) {
      int dc = delta + center;
      if(dc < 0 || dc > audio->size()){
        fft_frame[delta+FFT_HALF] = 0;
      }
      else {
        fft_frame[delta+FFT_HALF] = audio->data()[dc];
        // accumulate to calc power
        accume += abs(audio->data()[dc]);
      }
    }

    powr[i] = accume / FFT_LEN;
    if (powr[i] > mxpow){
      mxpow = powr[i];
    }
    if (powr[i] < mnpow) {
      mnpow = powr[i];
    }

    // Execute and transform fft into power spectrum
    fftw_execute(plan);
    for (int bin = 0; bin < FFT_BINS; ++bin) {
      int bstart = FFT_BINS*i;
      spec[bstart+bin] = 20*log(sqrt(fft_out[bin][0]*fft_out[bin][0] + fft_out[bin][1]*fft_out[bin][1]));

      // Track min and max for scaling
      if (!(isinf(spec[bstart+bin]) || isnan(spec[bstart+bin]))){
        if (spec[bstart+bin] > mx) {
          mx = spec[bstart+bin];
        }
        if (spec[bstart+bin] < mn) {
          mn = spec[bstart+bin];
        }
      }
    }
  } // End fft loop

  mx -= mn;
  unsigned char tmp = 0;
  for (int y = 0; y < FFT_HALF; y++){
    for (int x = 0; x < nframes; x++) {
      double global_scale = sqrt((powr[x] - mnpow)/mxpow);
      if (global_scale < 0.2) {
        global_scale = 0.2;
      }
      if (global_scale > 0.8) {
        global_scale = 1;
      }
      double val = spec[x*FFT_BINS + y] * global_scale;
      if (isinf(val)) {
        if (val > 0) {
          tmp = cmap_count/2-1;
        }
        else {
          tmp = 0;
        }

      }
      else {
        if (isnan(val)) {
          tmp = 0;
        }
        else {
          tmp = (int)(pow((val - mn)/mx, 2) * (cmap_count/2-1));
        }
      }

      spectrograms[index].setPixel(x, FFT_HALF-y-1, colormap[tmp*2]);
    }
  }

  fftw_free(spec);
  fftw_free(powr);

  emit spectrogramUpdate();

}


void Control::genAllSpectrogtams() {
  for (int i = 0; i < data.paths.size(); ++i) {
    QFuture<void> ft = QtConcurrent::run(&Control::generateSpectrogram, this, i);
  }
}

void Control::createPhonemeTrack(){
    data.addTrack("Phonemes", WORD, true);

    phonemeTrack = data.getLabelTrack(0, "Phonemes");
}

void Control::updateLabels(){
    emit changedLabels();
}

void Control::updateActiveIndex(){
    emit changedActiveIndex();
}

void Control::updateLabelText(std::string str){
    emit setLabelText(str);
}

void Control::exportMonoLabels(){
    emit triggerExportMonoLabels();
}

void Control::runML(){
    emit triggerML();
}

std::vector<TrackDefs> Control::getTracks() {
  return data.tracks;
}

std::vector<TrackDefs> Control::getTrackPresets() {
  std::vector<TrackDefs> presets;

  TrackDefs phonemes = {"Phonemes", WORD, true};
  presets.push_back(phonemes);

  TrackDefs syllables = {"Syllables", AUTO_NUMBER, true};
  presets.push_back(syllables);

  TrackDefs phrases = {"Phrases", AUTO_NUMBER, true};
  presets.push_back(phrases);

  return presets;
}

// Project IO
std::string Control::getProjectPath() {
  return data.projectPath;
}
bool Control::save() {
  data.save();
  return true;
}

bool Control::saveAs(std::string path) {
  data.projectPath = path;
  return save();
}

bool Control::load(std::string path) {
  IDSStatus status = data.load(path);

  if (status & IDS_SUCCESS) {
    QStringList qsl;

    for (int i = 0; i < data.paths.size(); ++i) {
      std::vector<double> *audio = data.getAudioData(i);
      int nframes = audio->size() / FFT_HOP;

      QImage image(QSize(nframes,FFT_HALF) ,QImage::Format_RGB32);
      image.fill(QColor("purple"));
      spectrograms.push_back(image); // May need to change to insert

      qsl << QString::fromStdString(data.paths[i]);
    }
    audio_files->setStringList(qsl);

    return true;
  }

  return false;
}
// END Project IO
