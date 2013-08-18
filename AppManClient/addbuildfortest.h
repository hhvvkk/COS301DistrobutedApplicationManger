/**
 * @file addbuildfortest.h
 * @brief a dummy ui to insert builds to test
 */
#ifndef ADDBUILDFORTEST_H
#define ADDBUILDFORTEST_H

#include <QDialog>
#include "Build.h"

namespace Ui {
class addBuildForTest;
}
/**
 * @class addBuildForTest
 * @brief The addBuildForTest class
 */
class addBuildForTest : public QDialog
{
    Q_OBJECT

signals:
    /**
     * @fn void initiateAddBuild(Build b);
     * @param b the Build to add
     * @brief initiateAddBuild will emit a signal to add the build
     */
    void initiateAddBuild(Build b);
public:
    /**
     * @fn explicit addBuildForTest(QWidget *parent = 0);
     * @brief addBuildForTest is the constructor
     * @param parent
     */
    explicit addBuildForTest(QWidget *parent = 0);
    /**
     * @fn ~addBuildForTest();
     * @brief the destructor
     */
    ~addBuildForTest();
    
private slots:
    /**
     * @fn void on_pushButton_clicked();
     * @brief on_pushButton_clicked the onclick button function
     */
    void on_pushButton_clicked();

private:
    /**
     * @var Ui::addBuildForTest *ui;
     * @brief ui the User Interface
     */
    Ui::addBuildForTest *ui;
};

#endif // ADDBUILDFORTEST_H
