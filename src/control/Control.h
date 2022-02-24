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

class Control : public QObject {
  Q_OBJECT

    public:
        Control();
        bool addAudioFile(std::string path);
        std::vector<double> *getAudioData(int index);
        std::vector<QImage> spectrograms;
        void computeSpectrogram(int index);
        IntermediateDataStructure data;
        void setPosition(double percent);
        void setZoom(double percent);
        int getStartSample();
        int getEndSample();
        int file_index = -1;
        double left_position = 0;
        double zoom = 1;
        int start_sample = 0;
        int end_sample = 0;
        int data_size = 0;
        std::vector<QRgb> colormap;
        void setCursorPosition(double pos);
        double cursor_pos = 0.0;
        void createPhonemeTrack();
        LabelTrack *phonemeTrack;

    public slots:
      void setFileIndex(int index);

    signals:
      void fileIndexChanged(int newValue);
      void viewChanged();
      void updatedCursorPosition();

    private:
        void computeWindow(int num, int window_size, int stride, AudioData audio, QImage *img);
        void computeAllWindows(int num_windows, int window_size, int stride,int index, int step, int phase);

};


#endif
