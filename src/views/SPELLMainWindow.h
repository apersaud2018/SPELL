#ifndef SPELLMAINWINDOW_H
#define SPELLMAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include "../control/Control.h"

QT_BEGIN_NAMESPACE
namespace Ui { class SPELLMainWindow; }
QT_END_NAMESPACE

class SPELLMainWindow : public QMainWindow
{
    Q_OBJECT
    const double ZOOM_SPEED = 0.10; 
public:
    SPELLMainWindow(QWidget *parent = nullptr);
    ~SPELLMainWindow();
    QGraphicsScene scene;
    Control controller;
    void addFileToList(QString path);
    void wheelEvent(QWheelEvent *event);

public slots:
	void playSelection();
    void addAudioFile();
    void newFileSelected();

private:
    Ui::SPELLMainWindow *ui;
    void renderWaveForm(std::vector<double> data);
};
#endif // MAINWINDOW_H
