#ifndef MAINBUILDINFO_H
#define MAINBUILDINFO_H

#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QWidget>
#include <QDebug>

/**
 * @class BuildInfo
 * @brief The BuildInfo class will be used to display information related to selected builds
 */
class BuildInfo: public QTreeWidget{
    Q_OBJECT
public:
    BuildInfo(QWidget *parent = 0);
};

#endif // MAINBUILDINFO_H
