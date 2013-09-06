#ifndef MANAGMENTTEST_H
#define MANAGMENTTEST_H

#include "AutoTest.h"

class ManagementTest : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void aTest();
    void test2();
    void cleanupTestCase();
};

DECLARE_TEST(ManagementTest)

#endif // MANAGMENTTEST_H
