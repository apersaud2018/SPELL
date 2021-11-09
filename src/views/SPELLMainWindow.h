#ifndef SPELLMAINWINDOW_H
#define SPELLMAINWINDOW_H

#include <QMainWindow>
#include "../control/Control.h"

QT_BEGIN_NAMESPACE
namespace Ui { class SPELLMainWindow; }
QT_END_NAMESPACE

class SPELLMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    SPELLMainWindow(QWidget *parent = nullptr);
    ~SPELLMainWindow();
    
    Control controller;

public slots:
	void playSelection();

private:
    Ui::SPELLMainWindow *ui;
};
#endif // MAINWINDOW_H
