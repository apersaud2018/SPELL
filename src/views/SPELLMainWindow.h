#ifndef SPELLMAINWINDOW_H
#define SPELLMAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class SPELLMainWindow; }
QT_END_NAMESPACE

class SPELLMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    SPELLMainWindow(QWidget *parent = nullptr);
    ~SPELLMainWindow();

public slots:
	void playSelection();

private:
    Ui::SPELLMainWindow *ui;
};
#endif // MAINWINDOW_H
