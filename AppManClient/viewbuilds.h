/**
 *@file viewbuilds.h
 *@brief the file with the code to view builds on the client
 */

#ifndef VIEWBUILDS_H
#define VIEWBUILDS_H

#include <QDialog>
#include <QTreeWidgetItem>
#include "xmlReader.h"
#include "management.h"

namespace Ui {
class viewBuilds;
}
/**
 * @class viewBuilds
 * @brief The viewBuilds class
 */
class viewBuilds : public QDialog
{
    Q_OBJECT
    
public:
    /**
     * @fn explicit viewBuilds(QWidget *parent = 0);
     * @brief viewBuilds the Default constructor
     * @param parent the QWidget object to be set as the parent
     */
    explicit viewBuilds(QWidget *parent = 0);
    /**
     * @fn ~viewBuilds();
     * @brief the default destructor
     */
    ~viewBuilds();
    /**
     * @fn void getReader(xmlReader xRead);
     * @brief getReader sets up the xmlReader to use
     * @param xRead the xmlReader object
     */
    void getReader(xmlReader xRead);
    /**
     * @fn void showBuilds();
     * @brief showBuilds displays the builds
     */
    void showBuilds();
    /**
     * @fn void getManager(Management * man);
     * @brief getManager gets the manager object from the mainform
     * @param man the manager to be set
     */
    void getManager(Management * man);
    
private slots:
    void on_pushButton_clicked();

    void on_treeWidget_clicked(const QModelIndex &index);

private:
    /**
     * @var ui
     * @brief ui the viewBuilds user interface
     */
    Ui::viewBuilds *ui;
    /**
     * @var xReader
     * @brief xReader the xmlReader instance
     */
    xmlReader xReader;
    /**
     * @var manage
     * @brief manage the pointer to a manager instance
     */
    Management * manage;
};

#endif // VIEWBUILDS_H
