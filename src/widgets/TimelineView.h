#ifndef TIMELINEVIEW_H
#define TIMELINEVIEW_H

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsLineItem>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QPen>
#include <QFileDialog>
#include "control/Control.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stdio.h>

QT_BEGIN_NAMESPACE
namespace Ui { class TimelineView; }
QT_END_NAMESPACE

class TimelineView : public QGraphicsView
{
    Q_OBJECT
public:
    TimelineView(QWidget *parent, Control *new_controller, std::string ntrack_name);
    ~TimelineView();
    void resizeEvent(QResizeEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    LabelTrack *track = nullptr;

    std::string track_name;
public slots:
  void audioChanged(int index);
  void updateView();
  void renderLabels();
  void updateActiveIndex();
  void updateLabelText(std::string str);
  void exportMonoLabels();
  void runML();
private:
  std::vector<double> *data = nullptr;
  Control *controller;
  float data_size;
  int index;
  QGraphicsScene scene;
  QPen cursorPen;
  QPen labelPen;
  QBrush labelBrush;
  QBrush movingLabelBrush;
  QBrush inactiveLabelBrush;
  QPen nameTagPen;
  QBrush nameTagBrush;
  void renderNameTag();
  void updateTimeline();
  void keyPressEvent(QKeyEvent* event);
  int width = 1;
  int height = 1;
  int activeLabelIndex = -1;
  int oldActiveLabelIndex = -1;
  QGraphicsLineItem *cursor;
  void updatedCursor();
  void makeCursor();
  std::vector<QGraphicsRectItem *> displayElements;
  void printLabels(std::vector<TextTrackEntry> label);
  int selectedLabelIndex = -1;
  bool movingLabel = false;
  QGraphicsRectItem *labelInMotion;


};
#endif // MAINWINDOW_H
