#include "mainform.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(images);
    QApplication a(argc, argv);
    MainForm w;
    w.show();
    return a.exec();
}
