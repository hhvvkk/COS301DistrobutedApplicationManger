#include "viewbuilds.h"
#include "ui_viewbuilds.h"

viewBuilds::viewBuilds(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::viewBuilds)
{
    ui->setupUi(this);
    ui->treeWidget->headerItem()->setHidden(true);
    ui->treeWidget->setColumnCount(2);
    ui->treeWidget->hideColumn(1);
    this->showBuilds();

}

viewBuilds::~viewBuilds()
{
    delete ui;
    delete manage;
}

void viewBuilds::on_pushButton_clicked()
{
    this->close();
}

void viewBuilds::getReader(xmlReader xRead){
    xReader = xRead;
}

void viewBuilds::getAppReader(appXMLReader xreader){
    xAppRead = xreader;
}

void viewBuilds::showBuilds(){
    QTreeWidgetItem *boola;
    QTreeWidgetItem *boola1;
    QTreeWidgetItem *boola2;
    //QTreeWidgetItem *boola3;
    xReader.parseXML();
    QString msg;
    if(xReader.getBuildDescription().size() == 1){
        msg = "There is " + QString::number(xReader.getBuildDescription().size()) + " build on this PC";
    }
    else
    {
        msg = "There are " + QString::number(xReader.getBuildDescription().size()) + " builds on this PC";
    }
    ui->msg_label->setText(msg);

    QMap<QString,QString> buildsID = xReader.getBuildUniqueID();
    QMap<QString,QString> buildsName = xReader.getBuildName();
    QMap<QString,QString> buildsDesc = xReader.getBuildDescription();
    QMap<QString,QString> buildsDir = xReader.getBuildDirectory();

    QMapIterator<QString, QString> i(buildsID);
    QMapIterator<QString, QString> j(buildsName);
    QMapIterator<QString, QString> k(buildsDesc);
    QMapIterator<QString, QString> l(buildsDir);

    while (i.hasNext() && j.hasNext() && k.hasNext() && l.hasNext())
    {
        i.next(); j.next(); k.next(); l.next();
        boola = new QTreeWidgetItem();
        boola1 = new QTreeWidgetItem();
        boola2 = new QTreeWidgetItem();
        boola->setText(1,i.value());
        boola->setText(0,"Build Name: "+j.value());
        boola1->setText(0,"Build Description: "+k.value());
        boola1->setText(1,i.value());
        boola2->setText(0,"Build Directory: "+l.value());
        boola2->setText(1,i.value());
        boola->addChild(boola1);
        boola->addChild(boola2);
        ui->treeWidget->addTopLevelItem(boola);
    }

    xAppRead.parseXML();
    QMap<QString,QString> appName = xAppRead.getAppNames();
    QMap<QString,QString> appFile = xAppRead.getAppFiles();
    QMapIterator<QString, QString> m(appName);
    QMapIterator<QString, QString> n(appFile);

    while (m.hasNext() && n.hasNext())
    {
        m.next(); n.next();
        boola = new QTreeWidgetItem();
        boola1 = new QTreeWidgetItem();
        boola2 = new QTreeWidgetItem();
        boola->setText(0,"Application");
        boola1->setText(0,"App Name: "+m.value());
        boola1->setToolTip(0,m.value());
        boola2->setText(0,"App Executable: "+n.value());
        boola2->setToolTip(0,n.value());
        boola1->addChild(boola2);
        boola->addChild(boola1);
        ui->treeWidget->addTopLevelItem(boola);
    }

}

void viewBuilds::on_treeWidget_clicked(const QModelIndex &index)
{
    QString selBuild = index.sibling(index.row(),1).data().toString();
    int num = selBuild.toInt();

}

void viewBuilds::getManager(Management * man){
    manage = man;
}
