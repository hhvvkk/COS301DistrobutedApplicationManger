#include "protoupdatebuildinfo.h"

ProtoUpdateBuildInfo::ProtoUpdateBuildInfo(QObject *parent) :
    Protocol(parent)
{
}

ProtoUpdateBuildInfo::~ProtoUpdateBuildInfo(){

}

void ProtoUpdateBuildInfo::handle(QVariantMap jsonObject, Management *man, QTcpSocket *slaveSocket){

}
