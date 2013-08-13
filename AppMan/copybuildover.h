#ifndef COPYBUILDOVER_H
#define COPYBUILDOVER_H

#include <QWidget>
#include <QDebug>
#include <QCompleter>
#include <QMessageBox>

#include "management.h"

namespace Ui {
class CopyBuildOver;
}

class CopyBuildOver : public QWidget
{
    Q_OBJECT
    
public:
    /**
     * @brief CopyBuildOver
     * @param suggestName suggestions for build names that are taken from the main form
     * @param suggestIP suggestions of machine IP addresses that are connected
     * @param buildName
     * @param parent
     */
    explicit CopyBuildOver(Management *man, QStringList suggestName, QStringList suggestIP, QString buildName = "", QWidget *parent = 0);
    ~CopyBuildOver();
    
private slots:
    /**
     * \fn void cancelClick();
     * @brief When the user clicks cancel
     */
    void cancelClick();

    /**
     * \fn void okClick();
     * @brief When the user clicks ok
     */
    void okClick();


    /**
     * \fn void buildMoreInformationClick();
     * @brief When the user clicks More information next to build
     */
    void buildMoreInformationClick();

    void showError(QString error);

private:
    Ui::CopyBuildOver *ui;

    Management *management;

    QCompleter *nameSuggestor;
    QCompleter *ipSuggestor;
};

#endif // COPYBUILDOVER_H
