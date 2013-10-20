#include "managementtest.h"
#include "AppMan/protocolhandler.h"

void ManagementTest::initTestCase(){
    management = new Management();
}

void ManagementTest::addBuildTest(){
    int amountBuildsTotal = management->getBuildCount();


    Build newBuild2(1, "NAME", "DESCript", "DIR");
    management->addBuild(newBuild2);

    QCOMPARE((amountBuildsTotal+1), management->getBuildCount());

    int count = 0;
    for(int i = 2; i < 8; i++){
        count++;//count how many extra is added
        Build anotherBuild(i, "anotherBuild"+QString::number(i), "Descript", "DIR");
        management->addBuild(anotherBuild);
    }

    int totalShouldBe = count + 1;
    QVERIFY(management->getBuildCount() == totalShouldBe);

    //test adding a build with name that already exist
    int beforeAmount = management->getBuildCount();
    Build oneWhichExist(1, "NAME", "DESCript", "DIR");
    management->addBuild(oneWhichExist);

    //verify whether the build has been added maby twice
    QVERIFY(management->getBuildCount() == beforeAmount);

}

void ManagementTest::addMachineTest(){
    QString supposedIP = "192.168.1.110";
    int amountBefore = management->getMachineCount();
    ProtocolHandler *machinesProtocolhandler = new ProtocolHandler(management);
    management->addMachine(1, supposedIP, machinesProtocolhandler);

    //verify that something has been added
    QVERIFY((amountBefore+1) == management->getMachineCount());


    Machine *theProtocolsMachine = machinesProtocolhandler->getMachine();
    //compare to see the right values are set
    QCOMPARE(theProtocolsMachine->getMachineID(), 1);
    QString theSetIp = theProtocolsMachine->getMachineIP();
    QCOMPARE(theSetIp, supposedIP);
}

void ManagementTest::removeMachineTest(){
    if(management->getMachineCount() <= 0){
        return;
    }

//    int beforeRemove = management->getMachineCount();
//    //else there exist a machine to delete
//    Machine *machineToRemove = management->getMachineAt(0);


//    management->removeMachine(machineToRemove);
//    machineToRemove == 0;
//    //verify that it has been removed
//    QVERIFY(management->getMachineCount() == (beforeRemove-1));
}

void ManagementTest::cleanupTestCase(){
    delete management;
}
