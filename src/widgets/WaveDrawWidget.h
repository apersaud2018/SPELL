#ifndef WAVESRAWWIDGET_H
#define WAVESRAWWIDGET_H

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsLineItem>
#include <QMouseEvent>
#include <QGraphicsSceneContextMenuEvent>
#include <QContextMenuEvent>
#include <QMenu>
#include <QToolTip>
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

    int index = -1;

public slots:
  void audioChanged(int aindex);
  void autoScaleChanged(int value);
  void updateView();
  void resizeEvent(QResizeEvent *event);
  void updatedCursor();
  void showContextMenu();
private:
  QGraphicsScene scene;
  int auto_scale = 0;
  std::vector<double> *data = nullptr;
  Control *controller;
  void renderWave();
  void renderTimeTicks();
  void makeCursor();
  void renderNameTag();
  QPen wavePen;
  QPen cursorPen;
  QPen tickPen;
  QPen nameTagPen;
  QBrush nameTagBrush;

  void mouseMoveEvent(QMouseEvent *event);
  int width = 1;
  int height = 1;
  void mousePressEvent(QMouseEvent *event);
  QGraphicsLineItem *cursor;

};
#endif // MAINWINDOW_H
