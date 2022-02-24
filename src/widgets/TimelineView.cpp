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

  brush.setStyle(Qt::SolidPattern);


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
void TimelineView::printLabels(std::vector<TextTrackEntry> label) {
  std::cout << "Printout:\n";
  for (int i = 0; i < label.size(); ++i) {
    std::cout << "Time: " << label[i].time << "\t\tPhoneme: " << label[i].data << "\n";
  }
  std::cout << "\n";
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

    if(track != nullptr){
        //printLabels(track->getTextLabels());
        std::vector<TextTrackEntry> labels = track->getTextLabels();
        if(displayElements.size() != labels.size()){
            for(int i=0;i<displayElements.size();i++){
                scene.removeItem(displayElements[i]);
            }
            displayElements = std::vector<QGraphicsRectItem *>();
            int start_sample = controller->getStartSample() ;
            int end_sample = controller->getEndSample();
            for(int i=0;i<labels.size();i++){
            int xpos = (int)((((labels[i].time*44100) - start_sample)/(end_sample-start_sample))*width);
                displayElements.push_back(scene.addRect(xpos, -height/2, 10, height, cursorPen, brush));
            }
        }
    }

}

void TimelineView::makeCursor(){

    width = mapToScene(viewport()->geometry()).boundingRect().width();
    height = mapToScene(viewport()->geometry()).boundingRect().height();
    int cursor_pos = (int)(controller->cursor_pos * width);
    cursor = scene.addLine(cursor_pos,-(height/2),cursor_pos,+(height/2),cursorPen);


}
