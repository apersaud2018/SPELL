#include "TrackManager.h"
#include "./ui_TrackManager.h"
#include <QStringList>
#include <QTableWidgetItem>

TrackManager::TrackManager(QWidget *parent, std::vector<TrackDefs> tracks) :
QDialog(parent), ui(new Ui::TrackManager) {
  ui->setupUi(this);
  ui->trackTable->setHorizontalHeaderLabels(QStringList() << tr("Name") << tr("Type") << tr("Keep one"));

  genTypeNameLocale();

  ui->trackTable->setRowCount(tracks.size());

  for (size_t i = 0; i < tracks.size(); i++) {
    QTableWidgetItem *name = new QTableWidgetItem(QString::fromStdString(tracks[i].name));
    QTableWidgetItem *type = new QTableWidgetItem(labelTypeNames[tracks[i].type]);
    QTableWidgetItem *atleast_one = new QTableWidgetItem(tracks[i].atleast_one ? tr("Yes") : tr("No"));
    ui->trackTable->setItem(i, 0, name);
    ui->trackTable->setItem(i, 1, type);
    ui->trackTable->setItem(i, 2, atleast_one);
  }

}


void TrackManager::genTypeNameLocale() {
  labelTypeNames.push_back(tr("Word"));
  labelTypeNames.push_back(tr("Text"));
  labelTypeNames.push_back(tr("Integer"));
  labelTypeNames.push_back(tr("Real"));
  labelTypeNames.push_back(tr("Auto Number"));
  labelTypeNames.push_back(tr("Note"));
}
