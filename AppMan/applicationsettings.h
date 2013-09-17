#ifndef APPLICATIONSETTINGS_H
#define APPLICATIONSETTINGS_H

#include <QWidget>

namespace Ui {
    class ApplicationSettings;
}

class ApplicationSettings : public QWidget
{
    Q_OBJECT

public:
    explicit ApplicationSettings(QWidget *parent = 0);
    ~ApplicationSettings();

private:
    Ui::ApplicationSettings *ui;
};

#endif // APPLICATIONSETTINGS_H
