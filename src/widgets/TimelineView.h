#ifndef TIMELINEVIEW_H
#define TIMELINEVIEW_H

#include <QGraphicsScene>
#include <QGraphicsView>
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

public slots:
  void audioChanged(int index);
  void updateView();

private:
  Control *controller;
  float data_size;
  int index;
  QGraphicsScene scene;
  void updateTimeline();
  int width = 1;
  int height = 1;
};
#endif // MAINWINDOW_H
