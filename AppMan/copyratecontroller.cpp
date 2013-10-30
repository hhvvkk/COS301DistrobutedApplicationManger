#include "copyratecontroller.h"
#include "copierphysical.h"

CopyRateController &CopyRateController::instance(){
    static CopyRateController theInstance;
    return theInstance;
}

CopyRateController::CopyRateController(QObject *parent) :
    QObject(parent)
{
    //50 ms time interval
    transferTimer.setInterval(50);
    connect(&transferTimer, SIGNAL(timeout()),this, SLOT(signalTransferGO()));
    counter = 0;
    loadUploadSpeed();
}


void CopyRateController::loadUploadSpeed(){

    QSettings setting("settings.ini", QSettings::IniFormat);
    //grouping the settings
    setting.beginGroup("Network");

    //default settings values
    QVariant defaultUploadRate;
    defaultUploadRate.setValue(25600);

    //send in the default values in case it does not exist...
    QString loadedUploadRate = setting.value("uploadRate(Bytes)", defaultUploadRate).toString();

    setting.endGroup();

    bool validUprate;
    maxUploadBytes = loadedUploadRate.toInt(&validUprate);

    if(!validUprate){
        maxUploadBytes = defaultUploadRate.toInt();
    }
}

void CopyRateController::signalTransferGO(){
    emit transferCopierGoAhead(next(), maxUploadBytes);
}

CopierPhysical *CopyRateController::next(){
    lock.lock();
    CopierPhysical *phy = transferBox.at(counter);
    //next one
    counter++;

    if(counter == transferBox.size())
        counter = 0;//start over

    lock.unlock();
    return phy;
}

void CopyRateController::addCopier(CopierPhysical *newCopier){
    lock.lock();
    if(!transferTimer.isActive())
        transferTimer.start(50);
    transferBox.append(newCopier);
    lock.unlock();
}

void CopyRateController::removeCopier(CopierPhysical *toRemove){
    lock.lock();

    //just adjust the counter accordingly
    if(transferBox.indexOf(toRemove) < counter )
        counter--;

    transferBox.removeOne(toRemove);

    //this means the counter is most probably at the end of the file
    if(counter == transferBox.size()){
        counter = 0;
    }

    //this means the transferBox is empty and the timer can stop
    if(transferBox.isEmpty()){
        transferTimer.stop();
    }

    lock.unlock();
}


void CopyRateController::setUploadRate(int bytesPer50MS){
    lock.lock();
    maxUploadBytes = bytesPer50MS;
    lock.unlock();
}
