#ifndef MOREINFO_H
#define MOREINFO_H

#include <QMainWindow>
#include <QTreeWidget>
#include <QStringList>
#include <QDebug>

namespace Ui {
class moreInfo;
}

class moreInfo : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit moreInfo(QWidget *parent = 0);
    ~moreInfo();
    
private slots:
    void on_pushButton_clicked();

private:
    Ui::moreInfo *ui;
    QStringList * parseDetailed(QStringList input);
    void setProcesses(QStringList data);
    void setHDD(QStringList data);
    void setCPU(QStringList data);
    void setRAM(QStringList data);
    void setNetw(QStringList data);
};

#endif // MOREINFO_H
