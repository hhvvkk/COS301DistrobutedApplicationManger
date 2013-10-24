#ifndef ADDAPP_H
#define ADDAPP_H

#include <QDialog>
#include <QFileDialog>

#include "appxmlwriter.h"
#include "management.h"

namespace Ui {
class AddApp;
}

class AddApp : public QDialog
{
    Q_OBJECT

public:
    explicit AddApp(QWidget *parent = 0, Management * man = 0);
    ~AddApp();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::AddApp *ui;
    Management *manage;
};

#endif // ADDAPP_H
