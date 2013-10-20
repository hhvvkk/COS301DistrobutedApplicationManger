#ifndef BUILDTEST_H
#define BUILDTEST_H

#include "AutoTest.h"
#include "AppMan/Build.h"

class BuildTest : public QObject
{
    Q_OBJECT
private slots:
    void initTestCase();

    //TESTS
    void constructorTest();
    void setBuildIDTest();
    void setBuildNameTest();
    void setBuildDescriptionTest();
    void setBuildIsSameTest();


    void cleanupTestCase();

private:
    Build *build;
};

DECLARE_TEST(BuildTest)

#endif // BUILDTEST_H
