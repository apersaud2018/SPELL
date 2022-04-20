#include "TrackManager.h"
#include "./ui_TrackManager.h"

TrackManager::TrackManager(QWidget *parent) :
QDialog(parent), ui(new Ui::TrackManager) {
  ui->setupUi(this);
}
