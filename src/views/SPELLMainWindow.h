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
    void mouseMoveEvent(QMouseEvent *event);

public slots:
	void playSelection();
    void addAudioFile();
    void newFileSelected();
    void zoomIn();
    void zoomOut();

private:
    WaveDrawWidget *waveDraw;
    FullWaveDrawWidget *fullWaveDraw;
    SpectrogramViewWidget *spectrogramDraw;
    Ui::SPELLMainWindow *ui;
};
#endif // MAINWINDOW_H
