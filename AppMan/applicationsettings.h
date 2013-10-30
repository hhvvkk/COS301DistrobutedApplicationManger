#ifndef APPLICATIONSETTINGS_H
#define APPLICATIONSETTINGS_H

#include <QWidget>
#include <QSettings>
#include "copyratecontroller.h"

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

    enum Unit{BYTES, KILOBYTES, MEGABYTES};


    Unit currentlySelected;

    /**
      * \fn void loadUploadSpeed();
      * @brief Loads the upload speed from the settings file
      */
    void loadUploadSpeed();


    /**
      * \fn int convert(int currentValue, Unit from, Unit to);
      * @brief Converts the current value into the correct selected to value
      * @param currentValue The current value that will be changed and converted
      * @param from The unit from which it will be converted
      * @param to The unit to which it will be converted
      */
    int convert(int currentValue, Unit from, Unit to);

    /**
      * \fn int convertToBytes(int currentValue, Unit from);
      * @brief Converts the value sent through into bytes
      * @param currentValue The current value that will be changed and converted
      * @param from The unit from which it will be converted
      */
    int convertToBytes(int currentValue, Unit from);

private:

    Ui::ApplicationSettings *ui;

    //the speed types(bytes, kbytes, mbytes)
    QStringList speedTypes;

private slots:
    void on_PushButtonDone_pressed();
    void on_cmbxSpeedType_currentIndexChanged(QString);
};

#endif // APPLICATIONSETTINGS_H
