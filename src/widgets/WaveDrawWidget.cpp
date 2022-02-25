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
}

WaveDrawWidget::~WaveDrawWidget() {
  scene.clear();
}

void WaveDrawWidget::audioChanged(int index) {
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
    }
}
void WaveDrawWidget::mousePressEvent(QMouseEvent *event){
    int start_sample = controller->getStartSample() ;
    int end_sample = controller->getEndSample();
    int sample = (int)((end_sample-start_sample)*controller->cursor_pos) + start_sample;

    // add new phoneme
    controller->phonemeTrack->insert(sample/44100.0, "b");
    std::cout << "Created new Phoneme\n";

    controller->updateLabels();
}
void WaveDrawWidget::renderWave() {

  if (data == nullptr) {
    return;
  }

  scene.clear();

  width = mapToScene(viewport()->geometry()).boundingRect().width();
  height = mapToScene(viewport()->geometry()).boundingRect().height();

  scene.setSceneRect(0,-height/2,width,height);


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
}
void WaveDrawWidget::makeCursor(){

    width = mapToScene(viewport()->geometry()).boundingRect().width();
    height = mapToScene(viewport()->geometry()).boundingRect().height();
    int cursor_pos = (int)(controller->cursor_pos * width);
    cursor = scene.addLine(cursor_pos,-(height/2),cursor_pos,+(height/2),cursorPen);
} 