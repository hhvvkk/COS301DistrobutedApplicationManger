#ifndef MANAGMENTTEST_H
#define MANAGMENTTEST_H

#include "AutoTest.h"
#include "AppMan/management.h"

class ManagementTest : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();

    //TESTS
    void addBuildTest();
    void addMachineTest();
    void removeMachineTest();

    void cleanupTestCase();
private:
    Management *management;

};

DECLARE_TEST(ManagementTest)

#endif // MANAGMENTTEST_H
