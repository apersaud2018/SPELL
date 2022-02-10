#ifndef FULLWAVESRAWWIDGET_H
#define FULLWAVESRAWWIDGET_H

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsRectItem>
#include <QGraphicsLineItem>
#include "control/Control.h"
#include <vector>

QT_BEGIN_NAMESPACE
namespace Ui { class FullWaveDrawWidget; }
QT_END_NAMESPACE

class FullWaveDrawWidget : public QGraphicsView
{
    Q_OBJECT
public:
    FullWaveDrawWidget(QWidget *parent, Control *new_controller);
    ~FullWaveDrawWidget();
    void resizeEvent(QResizeEvent *event);

public slots:
  void audioChanged(int index);
  void updateView();

private:
  QGraphicsScene scene;
  std::vector<double> *data = nullptr;
  Control *controller;

  void renderWave();
  QPen wavePen;

  void makeBox();
  void updateBox();
  QGraphicsRectItem *viewBox;
  QGraphicsLineItem *cursor;
  QPen viewBoxPen;
  QBrush viewBoxBrush;

};
#endif // MAINWINDOW_H