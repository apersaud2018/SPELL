#include "WaveDrawWidget.h"

WaveDrawWidget::WaveDrawWidget(QWidget *parent, Control *new_controller) :
QGraphicsView(parent), controller(new_controller)
{
  setScene(&scene);
  connect(controller, Control::fileIndexChanged, this, audioChanged);
  connect(controller, Control::viewChanged, this,  updateView);
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

void WaveDrawWidget::renderWave() {

  if (data == nullptr) {
    return;
  }

  scene.clear();

  int width = mapToScene(viewport()->geometry()).boundingRect().width();
  int height = mapToScene(viewport()->geometry()).boundingRect().height();

  scene.setSceneRect(0,-height/2,width,height);


  int start_sample = controller->getStartSample() ;
  int end_sample = controller->getEndSample();
  int num_samples_display = end_sample-start_sample;
  double sample_per_pixel = num_samples_display*1.0/width;
  //std::cout << sample_per_pixel << "\n";
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
      //vals.push_back(val);
      max_vals.push_back(max_sample);
      min_vals.push_back(min_sample);


  }
  QPen pen;
  QColor color(Qt::blue);
  color.setAlpha(100);
  pen.setColor(color);
  pen.setWidth(2);

  if(!auto_scale){
      max_val = 1;
  }

  for(int i=5;i<max_vals.size();i++){
  //scene.addLine(i-1,(int)(max_vals[i-1]*(1/max_val)),i,(int)(max_vals[i]*(1/max_val)),QPen(Qt::black));
  //scene.addLine(i-1,(int)(min_vals[i-1]*(1/max_val)),i,(int)(min_vals[i]*(1/max_val)),QPen(Qt::black));
      scene.addLine(i,(int)(min_vals[i]*(1/max_val)),i,(int)(max_vals[i]*(1/max_val)),pen);
  }
}
