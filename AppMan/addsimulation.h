#ifndef ADDSIMULATION_H
#define ADDSIMULATION_H

#include <QDialog>
#include <QVector>

#include "Build.h"
#include "Machine.h"
#include "management.h"

namespace Ui {
class AddSimulation;
}

class AddSimulation : public QDialog
{
    Q_OBJECT
    
signals:
    void initiateAddSimulation(Simulation * sim);
public:
    explicit AddSimulation(QWidget *parent = 0, Management * man = new Management());
    ~AddSimulation();
    
private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    void setup();
    Ui::AddSimulation *ui;
    Build * allBuilds;
    Management * management;
    QVector<Machine*> allMachines;

    void hideStuff();
};

#endif // ADDSIMULATION_H
