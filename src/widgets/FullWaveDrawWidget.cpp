#include "FullWaveDrawWidget.h"

FullWaveDrawWidget::FullWaveDrawWidget(QWidget *parent, Control *new_controller) :
QGraphicsView(parent), controller(new_controller)
{
  setScene(&scene);
  connect(controller, Control::fileIndexChanged, this, audioChanged);
  connect(controller, Control::viewChanged, this,  updateView);

  wavePen.setColor(Qt::blue);
  wavePen.setWidth(2);

  QColor brush_color(Qt::red);
  brush_color.setAlpha(50);
  viewBoxBrush.setColor(brush_color);
  viewBoxBrush.setStyle(Qt::SolidPattern);

  viewBoxPen.setColor(Qt::red);

}

FullWaveDrawWidget::~FullWaveDrawWidget() {
  scene.clear();
}

void FullWaveDrawWidget::audioChanged(int index) {
  data = controller->getAudioData(index);
  scene.clear();
  renderWave();
  makeBox();
}

void FullWaveDrawWidget::updateView() {
  updateBox();
}

void FullWaveDrawWidget::renderWave() {

  if (data == nullptr) {
    return;
  }

  int width = mapToScene(viewport()->geometry()).boundingRect().width();
  int height = mapToScene(viewport()->geometry()).boundingRect().height();

  scene.setSceneRect(0,-height/2,width,height);


  int start_sample = 0 ;
  int end_sample = data->size() - 1;
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

      min_sample *= (height/2) * 0.9;
      max_sample *= (height/2) * 0.9;

      max_vals.push_back(max_sample);
      min_vals.push_back(min_sample);


  }

  for(int i=5;i<max_vals.size();i++){
      scene.addLine(i,(int)(min_vals[i]*(1/max_val)),i,(int)(max_vals[i]*(1/max_val)),wavePen);
  }
}

void FullWaveDrawWidget::makeBox() {
  float width = mapToScene(viewport()->geometry()).boundingRect().width();
  float height = mapToScene(viewport()->geometry()).boundingRect().height();

  float relative_left = float(controller->getStartSample()) / data->size();
  float relative_right = float(controller->getEndSample()) / data->size();

  float x = relative_left * width;
  float y = -height/2.0;
  float rw = (relative_right - relative_left) * width;

  viewBox = scene.addRect(x, y, rw, height, viewBoxPen, viewBoxBrush);

}


void FullWaveDrawWidget::updateBox() {
  float width = mapToScene(viewport()->geometry()).boundingRect().width();
  float height = mapToScene(viewport()->geometry()).boundingRect().height();

  float relative_left = float(controller->getStartSample()) / data->size();
  float relative_right = float(controller->getEndSample()) / data->size();

  float x = relative_left * width;
  float y = -height/2.0;
  float rw = (relative_right - relative_left) * width;

  viewBox->setRect(x, y, rw, height );

}
