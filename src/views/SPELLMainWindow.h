#ifndef SPELLMAINWINDOW_H
#define SPELLMAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QPixmap>
#include "control/Control.h"
#include "widgets/WaveDrawWidget.h"
#include "widgets/FullWaveDrawWidget.h"
#include "widgets/SpectrogramViewWidget.h"
#include "widgets/TimelineView.h"

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
    Control *controller;
    void addFileToList(QString path);
    void wheelEvent(QWheelEvent *event);
    void addTrack();

public slots:
	void playSelection();
    void addAudioFile();
    void newFileSelected();
    void zoomIn();
    void zoomOut();
    void changeZoom();
    void addNewTrack();
    void toggleWaveView();
    void toggleSpectrogramView();
    void swapView(bool checked);
    void labelEntered();

private:
    WaveDrawWidget *waveDraw;
    FullWaveDrawWidget *fullWaveDraw;
    SpectrogramViewWidget *spectrogramDraw;
    std::vector<TimelineView*> timelineViews;
    Ui::SPELLMainWindow *ui;
};
#endif // MAINWINDOW_H
