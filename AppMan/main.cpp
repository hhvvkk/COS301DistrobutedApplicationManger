#include "mainform.h"
#include <QApplication>
#include <QObject>

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(images);
    QApplication a(argc, argv);
    a.setQuitOnLastWindowClosed(false);
    MainForm w;
    a.connect(&w, SIGNAL(quitApplication()), &a, SLOT(quit()));
    w.show();
    return a.exec();
}
