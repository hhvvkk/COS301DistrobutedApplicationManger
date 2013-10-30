#include "applicationsettings.h"
#include "ui_applicationsettings.h"
#include <QDebug>


ApplicationSettings::ApplicationSettings(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ApplicationSettings)
{
    ui->setupUi(this);

    speedTypes<<"Bytes/sec"<<"KiloBytes/sec"<<"MegaBytes/sec";
    ui->cmbxSpeedType->addItems(speedTypes);

    loadUploadSpeed();
}

ApplicationSettings::~ApplicationSettings()
{
    delete ui;
}

void ApplicationSettings::on_cmbxSpeedType_currentIndexChanged(QString changedValue){
    Unit previouslySelected = currentlySelected;

    if(!changedValue.compare("Bytes/sec")){
        currentlySelected = BYTES;
    }else if(!changedValue.compare("KiloBytes/sec")){
        currentlySelected = KILOBYTES;
    }else if(!changedValue.compare("MegaBytes/sec")){
        currentlySelected = MEGABYTES;
    }

    int currentValue = ui->spinBoxUploadRate->value();

    int convertedValue = convert(currentValue, previouslySelected, currentlySelected);

    if(convertedValue <= 0){
        ui->spinBoxUploadRate->setValue(1);
    }else{
        ui->spinBoxUploadRate->setValue(convertedValue);
    }

}

void ApplicationSettings::loadUploadSpeed(){
    QSettings setting("settings.ini",QSettings::IniFormat);
    //grouping the settings
    setting.beginGroup("Network");

    //default settings values
    QVariant defaultUploadRate;
    defaultUploadRate.setValue(25600);

    //send in the default values in case it does not exist...
    QString loadedUploadRate = setting.value("uploadRate(Bytes)", defaultUploadRate).toString();

    setting.endGroup();

    bool validUprate;
    int uploadRateBeforeConvert = loadedUploadRate.toInt(&validUprate);

    if(!validUprate){
        uploadRateBeforeConvert = defaultUploadRate.toInt();
    }else if(uploadRateBeforeConvert <= 0){
        uploadRateBeforeConvert = defaultUploadRate.toInt();
    }

    //convert to seconds(20 times per second it will upload)
    int uploadRateAfter = uploadRateBeforeConvert*20;

    ui->spinBoxUploadRate->setValue(uploadRateAfter);

    currentlySelected = BYTES;

}

int ApplicationSettings::convert(int currentValue, Unit from, Unit to){

    //convert from anything to bytes.
    int returnValue = convertToBytes(currentValue, from);

    //then convert back to unit to
    if(to == BYTES){
        return returnValue;
    }
    else if(to == KILOBYTES){
        returnValue = returnValue/1000;
    }
    else if(to == MEGABYTES){
        returnValue = returnValue/1000/1024;
    }

    return returnValue;

    //reason for above((Less code & complexity))
}

int ApplicationSettings::convertToBytes(int currentValue, Unit from){
    if(from == BYTES){
        //do nothing
    }
    else if(from == KILOBYTES){
        currentValue = 1000*currentValue;
    }
    else if(from == MEGABYTES){
        currentValue = 1024*1000*currentValue;
    }
    return currentValue;
}

void ApplicationSettings::on_PushButtonDone_pressed(){
    //-currentlySelected

    //setting upconvertToBytesloadRate
    int uploadRateToSet = convertToBytes(ui->spinBoxUploadRate->value(), currentlySelected);
    CopyRateController &rc = CopyRateController::instance();


    /*----get it into legible form(i.e. every 50ms a part will be sent across network)*/
    int convertedRate = uploadRateToSet/20;

    rc.setUploadRate(convertedRate);

    //set it in the settings
    QSettings setting("settings.ini",QSettings::IniFormat);
    //grouping the settings
    setting.beginGroup("Network");

    QVariant theValue = convertedRate;
    setting.setValue("uploadRate(Bytes)", theValue);

    setting.endGroup();
    /*----endSetting uploadRate*/

    this->close();
}
