#ifndef ABOUTHELP_H
#define ABOUTHELP_H

#include <QWidget>

namespace Ui {
class AboutHelp;
}

class AboutHelp : public QWidget
{
    Q_OBJECT
    
public:
    explicit AboutHelp(QWidget *parent = 0);
    ~AboutHelp();
    
private:
    Ui::AboutHelp *ui;
};

#endif // ABOUTHELP_H
