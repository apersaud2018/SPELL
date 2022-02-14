#ifndef TIMELINEVIEW_H
#define TIMELINEVIEW_H

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsLineItem>
#include <QMouseEvent>
#include <QPen>
#include "control/Control.h"
#include <vector>

QT_BEGIN_NAMESPACE
namespace Ui { class TimelineView; }
QT_END_NAMESPACE

class TimelineView : public QGraphicsView
{
    Q_OBJECT
public:
    TimelineView(QWidget *parent, Control *new_controller);
    ~TimelineView();
    void resizeEvent(QResizeEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
public slots:
  void audioChanged(int index);
  void updateView();

private:
  std::vector<double> *data = nullptr;
  Control *controller;
  float data_size;
  int index;
  QGraphicsScene scene;
  QPen cursorPen;
  void updateTimeline();
  int width = 1;
  int height = 1;
  QGraphicsLineItem *cursor;
  void updatedCursor();
  void makeCursor();

};
#endif // MAINWINDOW_H
