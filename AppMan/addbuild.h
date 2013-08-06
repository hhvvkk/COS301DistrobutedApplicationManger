#ifndef ADDBUILD_H
#define ADDBUILD_H

#include <QWidget>
#include "management.h"
#include "Build.h"
#include <QString>
#include <QFileDialog>
#include <QMap>
#include "xmlWriter.h"

namespace Ui {
class AddBuild;
}

/**
 * @class AddBuild
 * @brief The AddBuild class will be used to display a dialogue to add a new build to the AppMan program
 */

class AddBuild : public QWidget
{
    Q_OBJECT
    
signals:
    /**
     * \fn void initiateAddBuild(Build b);
     * @brief initiateAddBuild will emit a signal to add the build
     */
    void initiateAddBuild(Build b);
public:
    explicit AddBuild(Management *man, QString directory = "", QWidget *parent = 0);
    ~AddBuild();
public:
    void addToXML(int num,QString name,QString descript,QString direc);
private slots:
    /**
     * \fn void cancelClick();
     * @brief cancelClick will close the form
     */
    void cancelClick();

    /**
     * \fn void okClick();
     * @brief okClick will add the build to the program
     */
    void okClick();

    /**
     * \fn void chooseClick();
     * @brief chooseClick will open a dialogue to choose the directory of the build
     */
    void chooseClick();

private:
    Ui::AddBuild *ui;
    Management *management;
    xmlWriter *theXMLWriter;
};

#endif // ADDBUILD_H
