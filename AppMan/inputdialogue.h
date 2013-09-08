/**
* @file inputdialogue.h
* @brief This is the file containing the InputDialogue widget
* @author 101Solutions
* @version 2.0
*/

#ifndef INPUTDIALOGUE_H
#define INPUTDIALOGUE_H

#include "management.h"

#include <QMessageBox>
#include <QDebug>
#include <QWidget>
#include <QSettings>

namespace Ui {
    class InputDialogue;
}
/**
 * @class InputDialogue
 * @brief The InputDialogue class will be displayed each time it is required for a user to set a value
 */
class InputDialogue : public QWidget
{
    Q_OBJECT
public:
    /**
     * @brief InputDialogue
     * @param setWhat The item that will be set. Example = port
     * @param m The Management class that is passed by reference to call functions on it to set values
     * @param parent The parent of the widget
     */
    explicit InputDialogue(QString setWhat, Management *m, QWidget *parent = 0);

    /**
      * \fn ~InputDialogue();
      * @brief The destructor
      */
    ~InputDialogue();


    /**
     * \fn bool testInput(int lowerRange, int upperRange);
     * @brief testInput will test the input value inside the ui->lineEdit and state whether it is correct
     * @param lowerRange the minimum value for the input
     * @param upperRange the maximum value fot the input
     * @return returns a boolean to say whether the value entered is correct
     */
    bool testInput(int lowerRange, int upperRange);


    /**
     * @fn void showError(QString errorMessage);
     * @brief showError shows an error message
     * @param errorMessage the message to show
     * @param info The type of message
     */
    void showErrorMessage(QString errorMessage, QString info);

private:

    /**
     * @class ErrorClass
     * @brief The ErrorClass class will be used to throw an error if something wrong has happened. Will be caught then
     */
    class ErrorClass{
    public:
        ErrorClass(QString errorMessage){
            message = errorMessage;
        }
        /**
         * @var message
         * @brief message will be used to set the message shown to the user
         */
        QString message;
    };

private slots:
    /**
     * \fn okClicked();
     * @brief okClicked When the user clicks ok button
     */
    void okClicked();

    /**
     * \fn cancelClicked();
     * @brief okClicked When the user clicks cancel button
     */
    void cancelClicked();

private:
    /**
     * @var ui
     * @brief The ui for widget
     */
    Ui::InputDialogue *ui;
    /**
     * @var management
     * @brief The Management Class passed by reference
     */
    Management *management;
    /**
     * @var what
     * @brief The item that will be changed via the management class
     */
    QString what;

};

#endif // INPUTDIALOGUE_H
