#ifndef MOREINFO_H
#define MOREINFO_H

#include <QMainWindow>
#include <QTreeWidget>
#include <QStringList>
#include <QDebug>
#include <QTimer>
#include "Machine.h"

namespace Ui {
class moreInfo;
}

class moreInfo : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit moreInfo(Machine* m, QWidget *parent = 0);
    ~moreInfo();
    
private slots:
    void reshow();
    void closeEvent(QCloseEvent *event);

private:
    Ui::moreInfo *ui;
    QStringList * parseDetailed(QStringList input);
    void setProcesses(QStringList data);
    void setHDD(QStringList data);
    void setCPU(QStringList data);
    void setRAM(QStringList data);
    void setNetw(QStringList data);


    Machine* machine;

    QTimer* update;
};

#endif // MOREINFO_H
