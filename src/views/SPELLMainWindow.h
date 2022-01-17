#ifndef SPELLMAINWINDOW_H
#define SPELLMAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QPixmap>
#include "control/Control.h"
#include "widgets/WaveDrawWidget.h"
#include "widgets/FullWaveDrawWidget.h"

QT_BEGIN_NAMESPACE
namespace Ui { class SPELLMainWindow; }
QT_END_NAMESPACE

class SPELLMainWindow : public QMainWindow
{
    Q_OBJECT
    const double ZOOM_SPEED = 0.10;
    const double SCROLL_SPEED = 0.05;

public:
    SPELLMainWindow(QWidget *parent = nullptr);
    ~SPELLMainWindow();
    QGraphicsScene scene;
    QGraphicsScene fullWaveScene;
    QGraphicsView *spectrogram;
    QGraphicsScene specScene;
    Control *controller;
    void addFileToList(QString path);
    void wheelEvent(QWheelEvent *event);
    void resizeEvent(QResizeEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

public slots:
	void playSelection();
    void addAudioFile();
    void newFileSelected();
    void autoScrollChanged(int);
    void updateUI();
    void zoomIn();
    void zoomOut();

private:
    WaveDrawWidget *waveDraw;
    FullWaveDrawWidget *fullWaveDraw;
    Ui::SPELLMainWindow *ui;
    void renderFullWaveForm(std::vector<double> data);
    void renderSpectrogram(QImage img, int sample_len);
};
#endif // MAINWINDOW_H
