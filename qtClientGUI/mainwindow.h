#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "dialog.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void goToNext(QString ip);
private slots:
    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    Dialog *myDialog;

};

#endif // MAINWINDOW_H