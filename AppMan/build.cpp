#include "build.h"

Build::Build()
{
    buildName = "";
    buildDescription = "";
}

Build::~Build(){

}

Build::Build(QString bn, QString bd){
    buildName = bn;
    buildDescription = bd;
}



