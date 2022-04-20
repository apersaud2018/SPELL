#ifndef TRACK_MANAGER_DIAG
#define TRACK_MANAGER_DIAG

#include <QDialog>

QT_BEGIN_NAMESPACE
namespace Ui { class TrackManager; }
QT_END_NAMESPACE

class TrackManager : public QDialog {
  Q_OBJECT

public:
    TrackManager(QWidget *parent = nullptr);

private:
    Ui::TrackManager *ui;
};


#endif
