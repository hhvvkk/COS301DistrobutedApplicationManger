#ifndef ABOUTVERSION_H
#define ABOUTVERSION_H

#include <QWidget>

namespace Ui {
class AboutVersion;
}

class AboutVersion : public QWidget
{
    Q_OBJECT
    
public:
    explicit AboutVersion(QWidget *parent = 0);
    ~AboutVersion();
    
private:
    Ui::AboutVersion *ui;
};

#endif // ABOUTVERSION_H
