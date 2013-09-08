#include "inputdialogue.h"
#include "ui_inputdialogue.h"

InputDialogue::InputDialogue(QString setWhat, Management *m, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::InputDialogue),
    management(m),
    what(setWhat)
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
            parsed = testInput(1024,65000);
        }catch(ErrorClass ec){
            showErrorMessage(ec.message, "error");
            return;
        }

        if(!parsed){
            QString message = "Incorrect input for port. Port must be between 1200 and 65000";
            showErrorMessage(message, "error");
            return;
        }

        QSettings setting("settings.ini",QSettings::IniFormat);
        //grouping the settings
        setting.beginGroup("Connection");

        setting.setValue("port", ui->lineEdit->text().toInt());

        setting.endGroup();
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

void InputDialogue::showErrorMessage(QString errorMessage, QString info){
    QMessageBox *msb = new QMessageBox();
    if(info.compare("error") == 0){
        msb->setIcon(QMessageBox::Critical);
    }
    if(info.compare("inform") == 0){
        msb->setIcon(QMessageBox::Information);
    }
    QPixmap pic(":/images/images/ALogo.png");
    //msb->setIconPixmap(pic.scaled(50,50,Qt::IgnoreAspectRatio,Qt::FastTransformation));
    msb->setWindowIcon(QIcon(pic));
    msb->setText(errorMessage);
    msb->show();
}
