#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

#include "management.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void connectClick();
    void disconnectClick();

private:
    Ui::MainWindow *ui;
    Management *management;

};

#endif // MAINWINDOW_H
