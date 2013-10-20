#include "slavetest.h"
#include "AppMan/Build.h"

void SlaveTest::initTestCase(){
    slave = new Slave(1, "192.168.1.110");
}

void SlaveTest::addBuildTest(){
    int supposedID = 0;
    QString supposedName = "newName";
    QString supposedDescription = "Descrept";
    QString supposedDirectory = "/aRootDirHere";
    Build newBuild(supposedID, supposedName, supposedDescription, supposedDirectory);

    slave->addBuild(newBuild);

    //verify one build has been added";
    QVERIFY(slave->getBuildCount() == 1);
    for(int i = 1; i <= 14; i++){
        QString newSupposedBuildName = "newNAME"+QString::number(i);
        QString newsupposedDescription = "newDescr"+QString::number(i);
        QString newsupposedDirectory = "newDIR"+QString::number(i);
        Build anotherBuild(i, newSupposedBuildName, newsupposedDescription, newsupposedDirectory);

        slave->addBuild(anotherBuild);
    }
    //verify there are 15 builds at this stage
    QVERIFY(slave->getBuildCount() == 15);


    //verify that it cannot add another build with same id
    Build newSameBuild(supposedID, supposedName, supposedDescription, supposedDirectory);
    slave->addBuild(newSameBuild);
    QVERIFY(slave->getBuildCount() == 15);

}


void SlaveTest::deleteBuildTest(){
    //this stage there exist builds with ids, 1-15
    int validID = 112;
    int invalidID = -111;

    int buildCountBefore = slave->getBuildCount();
    //add the build that will be deleted
    slave->addBuild(Build(validID, "","",""));

    //verify the build was added
    QVERIFY((buildCountBefore+1)==slave->getBuildCount());

    //try delete invalid build and test if something was deleted
    slave->deleteBuild(invalidID);
    QVERIFY(buildCountBefore != slave->getBuildCount());

    //delete build and test that it was deleted
    slave->deleteBuild(validID);
    QVERIFY(buildCountBefore == slave->getBuildCount());

}

void SlaveTest::cleanupTestCase(){
    delete slave;
}
