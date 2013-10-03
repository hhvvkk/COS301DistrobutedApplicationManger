#include "mainbuildinfo.h"

BuildInfo::BuildInfo(QWidget *parent)
    :QTreeWidget(parent){
    this->setEditTriggers(NoEditTriggers);
    QString labelHeader1 = "Property";
    QString labelHeader2 = "Value";
    QStringList labelHeaders;
    labelHeaders << labelHeader1<<labelHeader2;

    QTreeWidget::setColumnCount(2);

    QTreeWidget::setHeaderLabels(labelHeaders);

    QTreeWidgetItem *newItem;

    newItem = new QTreeWidgetItem();
    newItem->setText(0,"BuildID");
    this->addTopLevelItem(newItem);

    newItem = new QTreeWidgetItem();
    newItem->setText(0,"Name");
    this->addTopLevelItem(newItem);


    newItem = new QTreeWidgetItem();
    newItem->setText(0,"Directory");
    this->addTopLevelItem(newItem);


    newItem = new QTreeWidgetItem();
    newItem->setText(0,"Description");
    this->addTopLevelItem(newItem);

    //connect the slot that will enable editing of items
    //connect(this, SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)), this, SLOT(buildInfoDoubleClicked(QTreeWidgetItem*,int)));
}

//void BuildInfo::buildInfoDoubleClicked(QTreeWidgetItem* theDoubleClickedItem, int theColumn){
//    if(theColumn == 1){

//    }
//    else{
//        //it is not editable

//        qDebug()<<"No";
//    }
//}
