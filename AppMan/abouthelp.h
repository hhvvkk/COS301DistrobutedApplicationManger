/**
* @file abouthelp.h
* @brief This is the file containing the abouthelp widget
* @author 101Solutions
* @version 2.0
*/

#ifndef ABOUTHELP_H
#define ABOUTHELP_H

#include <QWidget>

namespace Ui {
class AboutHelp;
}
/**
 * @class AboutHelp
 * @brief The AboutHelp class will be used to display useful help tutorials for
 */
class AboutHelp : public QWidget
{
    Q_OBJECT
    
public:
    /**
     * \fn AboutHelp(QWidget *parent = 0);
     * @brief The default constructor to create the AboutHelp Widget
     * @param parent Parent of this class which will destroy it
     */
    explicit AboutHelp(QWidget *parent = 0);

    /**
      * \fn ~AboutHelp();
      * @brief The destructor
      */
    ~AboutHelp();
    
private:
    /**
     * @var ui
     * @brief The ui that is displayed
     */
    Ui::AboutHelp *ui;
};

#endif // ABOUTHELP_H
