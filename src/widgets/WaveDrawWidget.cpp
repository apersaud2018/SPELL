#include "WaveDrawWidget.h"

WaveDrawWidget::WaveDrawWidget(QWidget *parent, Control *new_controller) :
QGraphicsView(parent), controller(new_controller)
{
  setScene(&scene);
  connect(controller, Control::fileIndexChanged, this, audioChanged);
  connect(controller, Control::viewChanged, this,  updateView);
  connect(controller, Control::updatedCursorPosition, this,  updatedCursor);

  wavePen.setColor(Qt::blue);
  wavePen.setWidth(2);
  cursorPen.setColor(QColor(0xFF, 0, 0, 0x90));
  cursorPen.setWidth(2);
  tickPen.setColor(Qt::white);
  nameTagPen.setColor(QColor(30, 30, 30, 0xFF));
  nameTagBrush.setColor(QColor(30, 30, 30, 0x40));
  nameTagBrush.setStyle(Qt::SolidPattern);
  this->setContextMenuPolicy(Qt::CustomContextMenu);
  connect(this, customContextMenuRequested, this, showContextMenu);


}

WaveDrawWidget::~WaveDrawWidget() {
  scene.clear();
}

void WaveDrawWidget::audioChanged(int aindex) {
  index = aindex;
  if(data == nullptr){
      controller->phonemeTrack->insert(0.0, "b");
  }
  data = controller->getAudioData(index);
  renderWave();
}

void WaveDrawWidget::autoScaleChanged(int value) {
  auto_scale = value;
  renderWave();
}

void WaveDrawWidget::updateView() {
  renderWave();
}

void WaveDrawWidget::updatedCursor() {

    width = mapToScene(viewport()->geometry()).boundingRect().width();
    height = mapToScene(viewport()->geometry()).boundingRect().height();
    int cursor_pos = (int)(controller->cursor_pos * width);
    cursor->setLine(cursor_pos,-(height/2),cursor_pos,+(height/2));

}

void WaveDrawWidget::resizeEvent(QResizeEvent *event) {
  renderWave();
}

// Handle mouse movement
void WaveDrawWidget::mouseMoveEvent(QMouseEvent *event){
    const QPointF p  =event->pos();
    // check if a file is selected
    if(controller->file_index > -1){
        controller->setCursorPosition((p.x()*1.0)/width);
        if(QToolTip::isVisible()){
            QToolTip::hideText();
        }
        int start_sample = controller->getStartSample() ;
        int end_sample = controller->getEndSample();
        int sample = (int)((end_sample-start_sample)*controller->cursor_pos) + start_sample;
        QToolTip::showText(this->mapToGlobal(QPoint(p.x(), p.y())), QString("%1 seconds").arg(sample/44100.0));

    }
}
void WaveDrawWidget::mousePressEvent(QMouseEvent *event){
    int start_sample = controller->getStartSample() ;
    int end_sample = controller->getEndSample();
    int sample = (int)((end_sample-start_sample)*controller->cursor_pos) + start_sample;

    // add new phoneme
    //controller->phonemeTrack->insert(sample/44100.0, "b");
    if (index >= 0) {
        controller->getLabelTrack(index, "Phonemes")->insert(sample/44100.0, "b");
    }
    std::cout << "Created new Phoneme\n";

    controller->updateLabels();
    controller->updateActiveIndex();
}
void WaveDrawWidget::showContextMenu(){
    std::cout << "Opening Context Menu\n";
    //QMenu *menu;
    //menu->addAction("b");
    //menu->exec(QPoint(10,10));
}
void WaveDrawWidget::renderTimeTicks() {
    double zoomLevel = controller->zoom;
    int sampleRate = 44100;
    int startSample = controller->getStartSample();
    int endSample = controller->getEndSample();
    // compute range of time in seconds that the window covers
    double timeRange = (endSample - startSample)/(1.0*sampleRate);
    double startTime = startSample/(1.0*sampleRate);
    int endTime = startTime + timeRange;
    enum tickModes {milliSeconds, centiSeconds, deciSeconds, seconds, decaSeconds};
    tickModes tickMode = decaSeconds;
    //std::cout << timeRange << "\n";
    if (timeRange > 20.0){
        tickMode = decaSeconds;
    }else if (timeRange > 2.0){
        tickMode = seconds;
    }else if(timeRange > 0.2){
        tickMode = deciSeconds;
    }else if(timeRange > 0.02){
        tickMode = centiSeconds;
    }else{
        tickMode = milliSeconds;
    }

    std::string tickLabel = "ms";
    int tickDivision = sampleRate;
    switch(tickMode){
        case decaSeconds:
            tickLabel = "0s";
            tickDivision = sampleRate*10;
            break;
        case seconds:
            tickLabel = "s";
            tickDivision = sampleRate;
            break;
        case deciSeconds:
            tickLabel = "cs";
            tickDivision = sampleRate/10;
            break;
        case centiSeconds:
            tickLabel = "0ms";
            tickDivision = sampleRate/100;
            break;
        case milliSeconds:
            tickLabel = "ms";
            tickDivision = sampleRate/1000;
            break;
        default:
            break;
    }
    int tickSample = (startSample / tickDivision) * tickDivision;
    while(tickSample < endSample){
        if (tickSample > startSample){
            int x = (int)(((tickSample-startSample) / (1.0*(endSample-startSample))) * width);
            scene.addLine(x, -height/2, x, -height/2 + 10, tickPen);
            int tickValue = tickSample/tickDivision;
            QGraphicsTextItem *tickText = scene.addText(QString::fromStdString(std::to_string(tickValue)+tickLabel));
            tickText->setPos(x,-height/2 - 5);
            tickText->setDefaultTextColor(Qt::white);
        }
        tickSample += tickDivision;
    }
    //std::cout << startSample<<"\n";

}
void WaveDrawWidget::renderWave() {

  if (data == nullptr) {
    return;
  }

  scene.clear();


  width = mapToScene(viewport()->geometry()).boundingRect().width();
  height = mapToScene(viewport()->geometry()).boundingRect().height();

  scene.setSceneRect(0,-height/2,width,height);
  renderTimeTicks();

  int start_sample = controller->getStartSample() ;
  int end_sample = controller->getEndSample();
  int num_samples_display = end_sample-start_sample;
  double sample_per_pixel = num_samples_display*1.0/width;
  double max_val = 0.0000001;


  std::vector<double> max_vals;
  std::vector<double> min_vals;
  max_vals.push_back(0.0);
  min_vals.push_back(0.0);
  for(int pixel_x=0;pixel_x<width;pixel_x++){
      double val = 0.0;
      double max_sample = 0.0;
      double min_sample = 0.0;
      int offset = start_sample+pixel_x*sample_per_pixel;
      for(int i=0;i<(int)sample_per_pixel;i++){
          int sample_index = i + offset;
          double sample = 0.0;
          if(sample_index >= 0 && sample_index < data->size()) {
              sample = (*data)[sample_index];
              if(sample > max_sample){
                  max_sample = sample;
              }
              else if(sample < min_sample) {
                  min_sample = sample;
              }
          }

      }

      if(pixel_x > 4){
          if(max_sample > max_val){
              max_val = max_sample;
          }else if (abs(min_sample) > max_val){
              max_val = abs(min_sample);
          }
      }

      min_sample *= (height/2) * 0.8;
      max_sample *= (height/2) * 0.8;
      max_vals.push_back(max_sample);
      min_vals.push_back(min_sample);


  }


  if(!auto_scale){
      max_val = 1;
  }

  for(int i=5;i<max_vals.size();i++){
      scene.addLine(i,(int)(min_vals[i]*(1/max_val)),i,(int)(max_vals[i]*(1/max_val)),wavePen);
  }
  makeCursor();
  // render name tag
  renderNameTag();

}
void WaveDrawWidget::makeCursor(){

    width = mapToScene(viewport()->geometry()).boundingRect().width();
    height = mapToScene(viewport()->geometry()).boundingRect().height();
    int cursor_pos = (int)(controller->cursor_pos * width);
    cursor = scene.addLine(cursor_pos,-(height/2),cursor_pos,+(height/2),cursorPen);
}

void WaveDrawWidget::renderNameTag(){
    scene.addRect(0,+height/2 -15, 60, 15, nameTagPen, nameTagBrush);
    QGraphicsTextItem *tickText = scene.addText("Waveform");
    tickText->setPos(0,+height/2 -20);
    tickText->setDefaultTextColor(QColor(0xB0, 0xB0, 0xB0, 0xFF));
}
