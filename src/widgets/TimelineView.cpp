#include "TimelineView.h"
#include <QTimer>

TimelineView::TimelineView(QWidget *parent, Control *new_controller) :
QGraphicsView(parent), controller(new_controller)
{
  setScene(&scene);
  connect(controller, Control::fileIndexChanged, this, audioChanged);
  connect(controller, Control::viewChanged, this,  updateView);
  connect(controller, Control::updatedCursorPosition, this,  updatedCursor);
  connect(controller, Control::changedLabels, this,  renderLabels);
  connect(controller, Control::changedActiveIndex, this,  updateActiveIndex);

  /* Initialize audio data to prevent crash when adding tracks after selecting audio*/
  index = controller->file_index;
  audioChanged(index);

  cursorPen.setColor(QColor(0xFF, 0, 0, 0x90));
  cursorPen.setWidth(2);
  labelPen.setColor(QColor(0x0, 0x0, 0, 0xFF));
  labelPen.setWidth(2);
  labelBrush.setStyle(Qt::SolidPattern);
  labelBrush.setColor(QColor(0x23, 0x7d, 0x56, 0xFF));
  movingLabelBrush.setStyle(Qt::SolidPattern);
  movingLabelBrush.setColor(QColor(0x23, 0x7d, 0x56, 0xFF));
  inactiveLabelBrush.setStyle(Qt::BDiagPattern);
  inactiveLabelBrush.setColor(QColor(0x23, 0x7d, 0x56, 0xFF));
  nameTagPen.setColor(QColor(30, 30, 30, 0xFF));
  nameTagBrush.setColor(QColor(30, 30, 30, 0x40));
  nameTagBrush.setStyle(Qt::SolidPattern);

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
    if(movingLabel && track != nullptr){
        std::vector<TextTrackEntry> labels = track->getTextLabels();
        int start_sample = controller->getStartSample();
        int end_sample = controller->getEndSample();
        // find the label that corresponds to the correct position
        for(int i=0;i<displayElements.size();i++){
            int xpos = (int)((((labels[i].time*44100) - start_sample)/(end_sample-start_sample))*width);
            int xposNext = 0;
            if (i == displayElements.size()-1){
                xposNext = width;
            }else{
                xposNext =  (int)((((labels[i+1].time*44100) - start_sample)/(end_sample-start_sample))*width);
            }

            if(xpos < p.x() && xposNext >= p.x()){
                // Special behaviour when hovering over the previous label
                if(i == selectedLabelIndex-1){
                    if(selectedLabelIndex == displayElements.size()-1){
                        xposNext = width;
                    }else{
                        xposNext = (int)((((labels[selectedLabelIndex+1].time*44100) - start_sample)/(end_sample-start_sample))*width);
                    }
                }
                labelInMotion->setRect(p.x(), -height/2, xposNext-p.x(), height/2);
                break;
            }
        }
    }
}

void TimelineView::mousePressEvent(QMouseEvent *event) {
    const QPointF p  =event->pos();
    width = mapToScene(viewport()->geometry()).boundingRect().width();
    height = mapToScene(viewport()->geometry()).boundingRect().height();
    // Don't do anything if the top part of the timeline is left clicked
    if(event->button() == Qt::LeftButton && p.y() < height/2 && !movingLabel){
        //std::cout << p.y()<<"\n";
        // initiate move of label on left click
    }else if (event->button() == Qt::LeftButton && track != nullptr && !movingLabel) {
        std::vector<TextTrackEntry> labels = track->getTextLabels();
        int start_sample = controller->getStartSample();
        int end_sample = controller->getEndSample();
        // find the label that the user clicked on (if any)
        for(int i=0;i<displayElements.size();i++){
            int xpos = (int)((((labels[i].time*44100) - start_sample)/(end_sample-start_sample))*width);
            int xposNext = 0;
            if (i == displayElements.size() -1){
                xposNext = width;
            }else{
                xposNext =  (int)((((labels[i+1].time*44100) - start_sample)/(end_sample-start_sample))*width);
            }
            if(xpos < p.x() && xposNext >= p.x()){
                if(i%2==0){
                    movingLabelBrush.setColor(QColor(0x23, 0x7d, 0x56, 0xFF));
                }else{
                    movingLabelBrush.setColor(QColor(0x32, 0x7d, 0x23, 0xFF));
                }
                selectedLabelIndex = i;
                movingLabel = true;
                labelInMotion = scene.addRect(xpos, -height/2, xposNext-xpos, height/2, labelPen, movingLabelBrush);
                displayElements[i]->setBrush(inactiveLabelBrush);
                break;
            }
        }
    // complete move of label
    }else if (event->button() == Qt::LeftButton && track != nullptr && movingLabel) {
        std::vector<TextTrackEntry> labels = track->getTextLabels();
        int start_sample = controller->getStartSample();
        int end_sample = controller->getEndSample();
        double newTime = (((p.x()/width) * (end_sample-start_sample)) + start_sample) / 44100;
        std::cout << newTime << " " << labels[selectedLabelIndex].time << "\n";

        track->move(selectedLabelIndex, newTime);

        movingLabel = false;
        scene.removeItem(labelInMotion);
        //controller->updateLabels();
        updateView();
    // abort move of label
    }else if (event->button() == Qt::RightButton && track != nullptr && movingLabel) {
             movingLabel = false;
             scene.removeItem(labelInMotion);
             updateView();
    // delete label
    }else if (event->button() == Qt::MiddleButton && track != nullptr && !movingLabel) {
        std::vector<TextTrackEntry> labels = track->getTextLabels();
        int start_sample = controller->getStartSample();
        int end_sample = controller->getEndSample();
        // find the label that the user clicked on (if any)
        for(int i=0;i<displayElements.size();i++){
            int xpos = (int)((((labels[i].time*44100) - start_sample)/(end_sample-start_sample))*width);
            int xposNext = 0;
            if (i == displayElements.size() -1){
              xposNext = width;
            }else{
              xposNext =  (int)((((labels[i+1].time*44100) - start_sample)/(end_sample-start_sample))*width);
            }
            if(xpos < p.x() && xposNext >= p.x()){
              track->remove(i);
              break;
            }
        }
        renderLabels();
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
    displayElements = std::vector<QGraphicsRectItem *>();
    width = mapToScene(viewport()->geometry()).boundingRect().width();
    height = mapToScene(viewport()->geometry()).boundingRect().height();



    scene.setSceneRect(0,-height/2,width,height);
    makeCursor();
    renderLabels();
    renderNameTag();

}
void TimelineView::renderLabels(){

    if(track != nullptr){
        //printLabels(track != nullptrtrack->getTextLabels());
        std::vector<TextTrackEntry> labels = track->getTextLabels();
        if(displayElements.size() != labels.size() || (!movingLabel && selectedLabelIndex != -1) || oldActiveLabelIndex != activeLabelIndex){
            oldActiveLabelIndex = activeLabelIndex;
            //std::cout << "AHHH" << "\n";
            if(selectedLabelIndex != -1){
                selectedLabelIndex = -1;
                movingLabel = false;
                if(labelInMotion != nullptr){
                    scene.removeItem(labelInMotion);
                }
            }
            for(int i=0;i<displayElements.size();i++){
                scene.removeItem(displayElements[i]);
            }
            //scene.clear();
            displayElements = std::vector<QGraphicsRectItem *>();
            int start_sample = controller->getStartSample() ;
            int end_sample = controller->getEndSample();
            for(int i=0;i<labels.size()-1;i++){
                if(i%2==0){
                  labelBrush.setColor(QColor(0x23, 0x7d, 0x56, 0xFF));
                }else{
                  labelBrush.setColor(QColor(0x32, 0x7d, 0x23, 0xFF));
                }
                if(i == activeLabelIndex){
                   labelBrush.setColor(QColor(0xFF, 0x0, 0x0, 0xFF));
                }
                int xpos = (int)((((labels[i].time*44100) - start_sample)/(end_sample-start_sample))*width);
                int xposNext =  (int)((((labels[i+1].time*44100) - start_sample)/(end_sample-start_sample))*width);
                displayElements.push_back(scene.addRect(xpos, 0, xposNext-xpos, height/2 -1, labelPen, labelBrush));
                if(!movingLabel){
                    QGraphicsTextItem *tickText = scene.addText(QString::fromUtf8(labels[i].data));
                    tickText->setPos(xpos,0);
                    tickText->setDefaultTextColor(QColor(0xB0, 0xB0, 0xB0, 0xFF));
                }
            }
            if((labels.size()-1)%2==0){
              labelBrush.setColor(QColor(0x23, 0x7d, 0x56, 0xFF));
            }else{
              labelBrush.setColor(QColor(0x32, 0x7d, 0x23, 0xFF));
            }
            if(labels.size()-1 == activeLabelIndex){
               labelBrush.setColor(QColor(0xFF, 0x0, 0x0, 0xFF));
            }
            int xpos = (int)((((labels[labels.size()-1].time*44100) - start_sample)/(end_sample-start_sample))*width);
            displayElements.push_back(scene.addRect(xpos, 0, width-xpos, height/2 -1, labelPen, labelBrush));
        }

    }

    //updatedCursor();

}

void TimelineView::updatedCursor() {
    if(cursor == nullptr){
        makeCursor();
    }

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
void TimelineView::updateActiveIndex(){
    if(TimelineView::track != nullptr){
        std::vector<TextTrackEntry> labels = TimelineView::track->getTextLabels();
        TimelineView::activeLabelIndex = labels.size()-2;
        controller->updateLabels();
    }
}

void TimelineView::renderNameTag(){
    scene.addRect(0,+height/2 -15, 50, 15, nameTagPen, nameTagBrush);
    QGraphicsTextItem *tickText = scene.addText("Timeline");
    tickText->setPos(0,+height/2 -20);
    tickText->setDefaultTextColor(QColor(0xB0, 0xB0, 0xB0, 0xFF));
}

void TimelineView::keyPressEvent(QKeyEvent* event) {
    if(track == nullptr){
        event->ignore();
        return;
    }
    std::vector<TextTrackEntry> labels = track->getTextLabels();
    switch (event->key()) {
    case Qt::Key_Left:
        if(activeLabelIndex > 0){
            activeLabelIndex--;
        }else{
            activeLabelIndex = labels.size()-1;
        }
        controller->updateLabels();
        break;
    case Qt::Key_Right:
        activeLabelIndex = (activeLabelIndex+1)%labels.size();
        controller->updateLabels();
        break;
    default:
        event->ignore();
        break;
    }
}