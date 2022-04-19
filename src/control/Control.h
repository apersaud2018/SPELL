#ifndef CONTROL_H
#define CONTROL_H
#include <string>
#include <vector>
#include <iostream>
#include <IntermediateDataStructure.h>
#include <QPixmap>
#include <QImage>
#include <QRgb>
#include <QObject>
#include <fftw3.h>
#include <QStringListModel>

#define FFT_LEN 512

// FFT_LEN / 2
#define FFT_HALF 256

#define FFT_HOP 256

// FFT_LEN / 2 + 1
#define FFT_BINS 257


class Control : public QObject {
  Q_OBJECT

    public:
        int file_index = -1;
        double left_position = 0;
        double zoom = 1;
        int start_sample = 0;
        int end_sample = 0;
        int data_size = 0;
        std::vector<QRgb> colormap;
        std::vector<QImage> spectrograms;
        double cursor_pos = 0.0;
        LabelTrack *phonemeTrack;
        QStringListModel *audio_files;

        Control();

        bool addAudioFile(std::string path);
        std::vector<double> *getAudioData(int index);
        void generateSpectrogram(int index);
        IntermediateDataStructure data;
        void setPosition(double percent);
        void setZoom(double percent);
        int getStartSample();
        int getEndSample();
        void setCursorPosition(double pos);
        void createPhonemeTrack();
        void updateLabels();
        void updateActiveIndex();
        void updateLabelText(std::string str);
    public slots:
      void setFileIndex(int index);
      void exportMonoLabels();
      void runML();

    signals:
      void fileIndexChanged(int newValue);
      void viewChanged();
      void updatedCursorPosition();
      void changedLabels();
      void changedActiveIndex();
      void setLabelText(std::string str);
      void triggerExportMonoLabels();
      void triggerML();
    private:
        fftw_plan plan;
        double *fft_frame;
        fftw_complex *fft_out;
        int cmap_count;

};


#endif
