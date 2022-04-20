#ifndef TRACK_MANAGER_DIAG
#define TRACK_MANAGER_DIAG

#include <QDialog>
#include <IntermediateDataStructure.h>
#include <vector>
#include <QString>

QT_BEGIN_NAMESPACE
namespace Ui { class TrackManager; }
QT_END_NAMESPACE

class TrackManager : public QDialog {
  Q_OBJECT

public:
    TrackManager(QWidget *parent, std::vector<TrackDefs> tracks);

private:
    Ui::TrackManager *ui;
    std::vector<QString> labelTypeNames;

    void genTypeNameLocale();
};


#endif
