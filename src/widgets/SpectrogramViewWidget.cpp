#include "SpectrogramViewWidget.h"
#include <QTimer>

SpectrogramViewWidget::SpectrogramViewWidget(QWidget *parent, Control *new_controller) :
QGraphicsView(parent), controller(new_controller)
{
  setScene(&scene);
  connect(controller, Control::fileIndexChanged, this, audioChanged);
  connect(controller, Control::viewChanged, this,  updateView);

  index = -1;

  QTimer *timer = new QTimer(this);
  connect(timer, QTimer::timeout, this, updateView);
  timer->start(1000);
}

SpectrogramViewWidget::~SpectrogramViewWidget() {
  scene.clear();
}

void SpectrogramViewWidget::resizeEvent(QResizeEvent *event) {
  updateSpectrogram();
}

void SpectrogramViewWidget::audioChanged(int nindex) {
  data_size = controller->data_size;
  index = nindex;
  updateSpectrogram();
}

void SpectrogramViewWidget::updateView() {
  updateSpectrogram();
}

void SpectrogramViewWidget::updateSpectrogram(){
    scene.clear();
    if (index != -1) {
      int width = mapToScene(viewport()->geometry()).boundingRect().width();
      int height = mapToScene(viewport()->geometry()).boundingRect().height();

      scene.setSceneRect(0,0,width,height);
      QPixmap pixmap = QPixmap::fromImage(controller->spectrograms[index]);//.scaled(width, height, Qt::IgnoreAspectRatio, Qt::FastTransformation);
      int img_width = pixmap.width();
      int img_height = pixmap.height();
      double len = controller->getEndSample() - controller->getStartSample();
      double size = len/data_size;
      double start = controller->getStartSample()/data_size;
      QRect crop(int(img_width*start), 0, int(img_width*size), img_height);
      scene.addPixmap(pixmap.copy(crop).scaled(width, height, Qt::IgnoreAspectRatio, Qt::FastTransformation));
    }
}
