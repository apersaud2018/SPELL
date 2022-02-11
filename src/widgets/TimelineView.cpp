#include "TimelineView.h"
#include <QTimer>

TimelineView::TimelineView(QWidget *parent, Control *new_controller) :
QGraphicsView(parent), controller(new_controller)
{
  setScene(&scene);
  connect(controller, Control::fileIndexChanged, this, audioChanged);
  connect(controller, Control::viewChanged, this,  updateView);

  index = -1;

 
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
  updateTimeline();
}

void TimelineView::updateView() {
  updateTimeline();
}

void TimelineView::updateTimeline() {


    scene.clear();

    width = mapToScene(viewport()->geometry()).boundingRect().width();
    height = mapToScene(viewport()->geometry()).boundingRect().height();

}


