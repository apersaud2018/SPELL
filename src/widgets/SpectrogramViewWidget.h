#ifndef SPECTROGRAMVIEW_H
#define SPECTROGRAMVIEW_H

#include <QGraphicsScene>
#include <QGraphicsView>
#include "control/Control.h"
#include <vector>

QT_BEGIN_NAMESPACE
namespace Ui { class SpectrogramViewWidget; }
QT_END_NAMESPACE

class SpectrogramViewWidget : public QGraphicsView
{
    Q_OBJECT
public:
    SpectrogramViewWidget(QWidget *parent, Control *new_controller);
    ~SpectrogramViewWidget();
    void resizeEvent(QResizeEvent *event);

public slots:
  void audioChanged(int index);
  void updateView();

private:
  Control *controller;
  float data_size;
  int index;
  QGraphicsScene scene;
  void updateSpectrogram();
};
#endif // MAINWINDOW_H
