#ifndef INPUTDIALOGUE_H
#define INPUTDIALOGUE_H

#include <QWidget>
#include "management.h"

namespace Ui {
    class InputDialogue;
}

class InputDialogue : public QWidget
{
    Q_OBJECT

public:
    explicit InputDialogue(QString setWhat, Management *m, QWidget *parent = 0);
    ~InputDialogue();
    bool testInput(int lowerRange, int upperRange);
    void showErrorMessage(QString mess);

private:

    class ErrorClass{
    public:
        ErrorClass(QString errorMessage){
            message = errorMessage;
        }

        QString message;
    };

private slots:
    void okClicked();
    void cancelClicked();

private:
    Ui::InputDialogue *ui;
    Management *management;
    QString what;

};

#endif // INPUTDIALOGUE_H
