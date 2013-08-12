#include "inputdialogue.h"
#include "ui_inputdialogue.h"

InputDialogue::InputDialogue(QString setWhat, Management *m, QWidget *parent) :
    what(setWhat),
    management(m),
    QWidget(parent),
    ui(new Ui::InputDialogue)
{
    ui->setupUi(this);
    ui->labelSetWhat->setText("Set "+setWhat);

    connect(ui->cancelButton,SIGNAL(clicked()),this,SLOT(cancelClicked()));
    connect(ui->okButton,SIGNAL(clicked()),this,SLOT(okClicked()));
}

InputDialogue::~InputDialogue()
{
    delete ui;
}

void InputDialogue::okClicked(){
    if(what == "port"){
        bool parsed = false;
        //try parse port
        try{
            parsed = testInput(1200,65000);
        }catch(ErrorClass ec){
            showErrorMessage(ec.message);
            return;
        }

        if(!parsed){
            QString message = "Incorrect input for port. Port must be between 1200 and 65000";
            showErrorMessage(message);
        }
        management->setPort(ui->lineEdit->text().toInt());
        this->close();
    }
}

void InputDialogue::cancelClicked(){
    this->close();
}

bool InputDialogue::testInput(int lowerRange, int upperRange) {
    //firstly Test whether it is a correct input...
    bool ok;
    int value = ui->lineEdit->text().toInt(&ok,10);
    if(!ok){
        //means that it is not an integer
        throw ErrorClass("The input value is not numeric");
    }

    if(value > upperRange || value < lowerRange){
        return false;
    }
    else{
        return true;
    }

}

void InputDialogue::showErrorMessage(QString error){
    QMessageBox *msb = new QMessageBox();
    QString showIt = "Error: "+error;
    msb->setText(showIt);
    msb->show();
}
