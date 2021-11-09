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
    void addFileToList(QString path);

public slots:
	void playSelection();
    void addAudioFile();

private:
    Ui::SPELLMainWindow *ui;
};
#endif // MAINWINDOW_H
