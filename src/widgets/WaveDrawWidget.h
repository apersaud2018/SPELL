#ifndef WAVESRAWWIDGET_H
#define WAVESRAWWIDGET_H

#include <QGraphicsScene>
#include <QGraphicsView>
#include "control/Control.h"
#include <vector>

QT_BEGIN_NAMESPACE
namespace Ui { class WaveDrawWidget; }
QT_END_NAMESPACE

class WaveDrawWidget : public QGraphicsView
{
    Q_OBJECT
public:
    WaveDrawWidget(QWidget *parent, Control *new_controller);
    ~WaveDrawWidget();

public slots:
  void audioChanged(int index);
  void autoScaleChanged(int value);
  void updateView();

private:
  QGraphicsScene scene;
  int auto_scale = 0;
  std::vector<double> *data = nullptr;
  Control *controller;
  void renderWave();
  QPen wavePen;

};
#endif // MAINWINDOW_H
