/**
* @file aboutversion.h
* @brief This is the file containing the aboutversion widget
* @author 101Solutions
* @version 2.0
*/

#ifndef ABOUTVERSION_H
#define ABOUTVERSION_H

#include <QWidget>

namespace Ui {
class AboutVersion;
}
/**
 * @class AboutVersion The widget class that will contain help and steps for new users
 * @brief The AboutVersion class
 */
class AboutVersion : public QWidget
{
    Q_OBJECT
    
public:
    /**
     * \fn explicit AboutVersion(QWidget *parent = 0);
     * @brief AboutVersion The default constructor to create the AboutVersion Widget
     * @param parent the parent of the widget
     */
    explicit AboutVersion(QWidget *parent = 0);
    /**
      * \fn ~AboutVersion();
      * @brief The destructor
      */
    ~AboutVersion();
    
private:
    /**
     * @var ui
     * @brief the user interface that is displayed
     */
    Ui::AboutVersion *ui;
};

#endif // ABOUTVERSION_H
