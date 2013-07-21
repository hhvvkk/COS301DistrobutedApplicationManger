#include "mainform.h"
#include <QApplication>
//#include <QWindowsStyle>


int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(images);
    QApplication a(argc, argv);
    MainForm w;
   // w.setStyle(new QWindowsStyle);
    w.show();
    
    return a.exec();
}
