#include "TimelineView.h"
#include <QTimer>

TimelineView::TimelineView(QWidget *parent, Control *new_controller) :
QGraphicsView(parent), controller(new_controller)
{
  setScene(&scene);
  connect(controller, Control::fileIndexChanged, this, audioChanged);
  connect(controller, Control::viewChanged, this,  updateView);
  connect(controller, Control::updatedCursorPosition, this,  updatedCursor);

  /* Initialize audio data to prevent crash when adding tracks after selecting audio*/
  index = controller->file_index;
  audioChanged(index);

  cursorPen.setColor(QColor(0xFF, 0, 0, 0x90));
  cursorPen.setWidth(2);

}

TimelineView::~TimelineView() {
  scene.clear();
}

void TimelineView::resizeEvent(QResizeEvent *event) {
  updateTimeline();
}

void TimelineView::audioChanged(int nindex) {
  data_size = controller->data_size;
  index = nindex;
  data = controller->getAudioData(index);
  updateTimeline();
}

void TimelineView::updateView() {
  updateTimeline();
}

// Handle mouse movement
void TimelineView::mouseMoveEvent(QMouseEvent *event){
    const QPointF p  =event->pos();
    // check if a file is selected
    if(controller->file_index > -1){
        controller->setCursorPosition((p.x()*1.0)/width);
    }
}

void TimelineView::mousePressEvent(QMouseEvent *event) {
  const QPointF p  =event->pos();
  width = mapToScene(viewport()->geometry()).boundingRect().width();
  height = mapToScene(viewport()->geometry()).boundingRect().height();

  if (event->button() == Qt::RightButton) {
    scene.addLine(p.x(),-(height/2),p.x(),+(height/2),cursorPen);
  }
}

void TimelineView::updateTimeline() {
    if (data == nullptr) {
        return;
    }

    scene.clear();
    width = mapToScene(viewport()->geometry()).boundingRect().width();
    height = mapToScene(viewport()->geometry()).boundingRect().height();

    scene.setSceneRect(0,-height/2,width,height);
    makeCursor();
}
void TimelineView::updatedCursor() {

    width = mapToScene(viewport()->geometry()).boundingRect().width();
    height = mapToScene(viewport()->geometry()).boundingRect().height();
    int cursor_pos = (int)(controller->cursor_pos * width);
    cursor->setLine(cursor_pos,-(height/2),cursor_pos,+(height/2));
}

void TimelineView::makeCursor(){

    width = mapToScene(viewport()->geometry()).boundingRect().width();
    height = mapToScene(viewport()->geometry()).boundingRect().height();
    int cursor_pos = (int)(controller->cursor_pos * width);
    cursor = scene.addLine(cursor_pos,-(height/2),cursor_pos,+(height/2),cursorPen);
}
