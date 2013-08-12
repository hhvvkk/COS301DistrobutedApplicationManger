#ifndef VIEWBUILDS_H
#define VIEWBUILDS_H

#include <QDialog>
#include <QTreeWidgetItem>
#include "xmlReader.h"
#include "management.h"

namespace Ui {
class viewBuilds;
}

class viewBuilds : public QDialog
{
    Q_OBJECT
    
public:
    explicit viewBuilds(QWidget *parent = 0);
    ~viewBuilds();
    void getReader(xmlReader xRead);
    void showBuilds();
    void getManager(Management * man);
    
private slots:
    void on_pushButton_clicked();

    void on_treeWidget_clicked(const QModelIndex &index);

private:
    Ui::viewBuilds *ui;
    xmlReader xReader;
    Management * manage;
};

#endif // VIEWBUILDS_H
