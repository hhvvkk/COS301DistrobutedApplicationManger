#include "buildtest.h"

void BuildTest::initTestCase(){
    build = new Build(1, "a", "b", "c");
}

void BuildTest::constructorTest(){
    int supposedID = 1;
    QString supposedName = "Name";
    QString supposedDescription = "Description";
    QString supposedDirectory = "Supposed/Directory";
    Build * b = new Build(supposedID, supposedName, supposedDescription, supposedDirectory);

    //test whether the values has been set correctly
    QCOMPARE(b->getBuildID(), supposedID);
    QCOMPARE(b->getBuildDescription(), supposedDescription);
    QCOMPARE(b->getBuildName(), supposedName);
    QCOMPARE(b->getBuildDirectory(), supposedDirectory);
}
//void setBuildID(int id);
///**
//* \fn void setBuildName(QString name);
//* @brief The function to mutate the buildName variable
//* @param name The name to be assigned to buildName
//*/
//void setBuildName(QString name);
///**
//* \fn void setBuildDescription(QString descript);
//* @brief The function to mutate the buildDescription variable
//* @param descript The description to be assigned tobuildDescription
//*/
//void setBuildDescription(QString descript);
///**
//* \fn void setBuildDirectory(QString direc);
//* @brief The function to mutate the buildDirectory variable
//* @param descript The description to be assigned to buildDirectory
//*/
//void setBuildDirectory(QString direc);
void BuildTest::setBuildIDTest(){

    //test first time
    build->setBuildID(20);
    QCOMPARE(build->getBuildID(), 20);


    //test it again
    build->setBuildID(25);
    QCOMPARE(build->getBuildID(), 25);
}

void BuildTest::setBuildNameTest(){

}

void BuildTest::setBuildDescriptionTest(){

}

void BuildTest::setBuildIsSameTest(){

}

void BuildTest::cleanupTestCase(){

}
