#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setQuitOnLastWindowClosed(false);
    MainWindow w;
    a.connect(&w, SIGNAL(quitApplication()), &a, SLOT(quit()));
    w.show();
    return a.exec();
}
