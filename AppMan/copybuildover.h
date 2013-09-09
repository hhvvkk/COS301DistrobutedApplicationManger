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

    /**
     * @fn void showError(QString errorMessage);
     * @brief showError shows an error message
     * @param errorMessage the message to show
     * @param info The type of message
     */
    void showMessage(QString errorMessage, QString info);

signals:
    /**
     * @fn void copyBuildOver(int machineID, QString buildName);
     * @brief signal to copy the build from master to slave machine
     * @param machineID The unique machine Id to which the build will be copied
     * @param buildName The name of the build to copy over
     */
    void copyBuildOver(int machineID, QString buildName);

private:
    /**
     * @fn bool validateInput();
     * @brief A function to validate the user input
     * @return Returns true if the user entered a correct input
     */
    bool validateInput();

private:
    Ui::CopyBuildOver *ui;

    /**
      * @var management The management class to use checking functions on
      */
    Management *management;

    /**
      * @var nameSuggestor A list of name suggestions
      */
    QCompleter *nameSuggestor;

    /**
      * @var ipSuggestor A list of ip suggestions
      */
    QCompleter *ipSuggestor;
};

#endif // COPYBUILDOVER_H
