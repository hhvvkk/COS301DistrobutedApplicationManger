#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    //create the application
    QApplication a(argc, argv);

    QFile stylesheetFile(":/styles/stylesheet/AppManStyle.qss");
    if(stylesheetFile.open(QFile::ReadOnly)) {
       QString StyleSheet = QLatin1String(stylesheetFile.readAll());
       //set the stylesheet for application
       a.setStyleSheet(StyleSheet);
    }

    //disable quit when all windows are closed
    a.setQuitOnLastWindowClosed(false);

    MainWindow w;
    //connect the application such that when the quit button on the tray is clicked, the app closes
    a.connect(&w, SIGNAL(quitApplication()), &a, SLOT(quit()));

    //show the application
    w.show();

    //enter an event loop and return
    return a.exec();
}
