#include <QString>
#include <QtTest>
#include <QMap>
#include <QMapIterator>

#include "build.h"
#include "xmlreader.h"
#include "xmlwriter.h"
#include "management.h"

class AppManClient_UnitTestsTest : public QObject
{
    Q_OBJECT
    
public:
    AppManClient_UnitTestsTest();
    
private Q_SLOTS:
    void tst_setBuild_ID();
    void tst_setBuild_NAME();
    void tst_setBuild_DESC();
    void tst_setBuild_DIR();
    void tst_setBuildID();
    void tst_setBuildName();
    void tst_setBuildDescription();
    void tst_setBuildDirectory();
    void tst_receiveBuild();
    void tst_createXMLFile();
    void tst_managementAddBuild();
    void tst_managementGetBuildByID();
    void tst_managementGetBuilds();
    //void tst_parseXML();
};

AppManClient_UnitTestsTest::AppManClient_UnitTestsTest()
{
}

void AppManClient_UnitTestsTest::tst_setBuild_ID(){
    Build b;
    b.setBuild(1,"test","test","test");
    QVERIFY(b.getBuildID()==1);
}

void AppManClient_UnitTestsTest::tst_setBuild_NAME(){
    Build b;
    b.setBuild(1,"test","test","test");
    QVERIFY(b.getBuildName() == "test");
}

void AppManClient_UnitTestsTest::tst_setBuild_DESC(){
    Build b;
    b.setBuild(1,"test","test","test");
    QVERIFY(b.getBuildDescription()=="test");
}

void AppManClient_UnitTestsTest::tst_setBuild_DIR(){
    Build b;
    b.setBuild(1,"test","test","test");
    QVERIFY(b.getBuildDirectory() =="test");
}

void AppManClient_UnitTestsTest::tst_setBuildID(){
    Build b;
    b.setBuildID(2);
    QVERIFY(b.getBuildID()==2);
}

void AppManClient_UnitTestsTest::tst_setBuildName(){
    Build b;
    b.setBuildName("test");
    QVERIFY(b.getBuildName() == "test");
}

void AppManClient_UnitTestsTest::tst_setBuildDescription(){
    Build b;
    b.setBuildDescription("test");
    QVERIFY(b.getBuildDescription()=="test");
}

void AppManClient_UnitTestsTest::tst_setBuildDirectory(){
    Build b;
    b.setBuildDirectory("test");
    QVERIFY(b.getBuildDirectory() =="test");
}

void AppManClient_UnitTestsTest::tst_managementAddBuild(){
    Management m;
    Build b(1,"Test","Test","Test");
    m.addBuild(b);
    QVERIFY(m.getBuildCount() == 1);
}

void AppManClient_UnitTestsTest::tst_managementGetBuildByID(){
    Build b(1,"Test","Test","Test");
    Build c(2,"Test2","Test2","Test2");
    Management m;
    m.addBuild(b);
    m.addBuild(c);
    Build d = m.getBuildByID(1);
    QVERIFY(d.getBuildID() == 1);
}

void AppManClient_UnitTestsTest::tst_managementGetBuilds(){
    Build b(1,"Test","Test","Test");
    Build c(2,"Test2","Test2","Test2");
    Management m;
    m.addBuild(b);
    m.addBuild(c);
    Build * bs = m.getAllBuilds();
    QVERIFY(bs[0].getBuildID() == 1);
    QVERIFY(bs[1].getBuildID() == 2);
}


void AppManClient_UnitTestsTest::tst_receiveBuild(){
    xmlWriter * xml = new xmlWriter();
    xml->receiveBuild("1","1","1","1");
    QVERIFY(xml->getCurNum() =="1");
    QVERIFY(xml->getCurName() == "1");
    QVERIFY(xml->getCurDesc() == "1");
    QVERIFY(xml->getCurDir() == "1");
}

void AppManClient_UnitTestsTest::tst_createXMLFile(){
    xmlWriter * xml = new xmlWriter();
    xml->receiveBuild("1","1","1","1");
    xml->CreateXMLFile();
    QFile * file = new QFile("builds.xml");
    bool test;
    if(file->exists()){
        test = true;
    }
    else{
        test = false;
    }
    QVERIFY(test == TRUE);
}
/*
void AppManClient_UnitTestsTest::tst_parseXML(){
    xmlWriter * xml = new xmlWriter();
    xml->receiveBuild("1","1","1","1");
    xml->CreateXMLFile();
    xmlReader * xRead = new xmlReader();
    xRead->parseXML();
    QMap <QString,QString> buildNumber = xRead->getBuildNumber();
    QMap <QString,QString> buildName = xRead->getBuildName();
    QMap <QString,QString> buildDescription = xRead->getBuildDescription();
    QMap <QString,QString> buildDirectory = xRead->getBuildDirectory();
    QMapIterator<QString, QString> i(buildNumber);
    QMapIterator<QString, QString> j(buildName);
    QMapIterator<QString, QString> k(buildDescription);
    QMapIterator<QString, QString> l(buildDirectory);
    QVERIFY(i.value() == "1");
    QVERIFY(j.value() == "1");
    QVERIFY(k.value() == "1");
    QVERIFY(l.value() == "1");
}
*/
QTEST_APPLESS_MAIN(AppManClient_UnitTestsTest)

#include "tst_Client.moc"
