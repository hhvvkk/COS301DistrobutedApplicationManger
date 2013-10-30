#ifndef COPYRATECONTROLLER_H
#define COPYRATECONTROLLER_H

#include <QObject>
#include <QTimer>
#include <QMutex>
#include <QSettings>

class CopierPhysical;


/**
  * @class CopyRateController
  * @brief A rate controller class which slows the physical copy of the files down in order to ensure it is copied over the network
  */
class CopyRateController : public QObject
{
    Q_OBJECT
signals:
    /**
      * \fn void transferCopierGoAhead(CopierPhysical* cpPhy, int max);
      * @brief A signal which is emitted each time a copierPhysical has the go ahead to transfer next few bytes
      *
      */
    void transferCopierGoAhead(CopierPhysical* cpPhy, int max);
public:
    /**
      * \fn CopyRateController &instance();
      * @brief A function to get the one instance of this class
      * @return Returns the only instance of this class
      */
    static CopyRateController &instance();

    /**
      * \fn void addCopier(CopierPhysical *newCopier);
      * @brief A function to add a copier in order to copy files across the network
      * @param newCopier The new copier to be added to the transfer box
      */
    void addCopier(CopierPhysical *newCopier);


    /**
      * \fn void removeCopier(CopierPhysical *toRemove);
      * @brief A function to remove a copier from the transferbox
      * @param toRemove The  copier to be deleted to the transfer box
      */
    void removeCopier(CopierPhysical *toRemove);

    /**
      * \fn void setUploadRate(int bytesPer50MS);
      * @brief Sets the upload rate of the controller to parameter passed through
      * @param bytesPer50MS The amount of bytes which will be sent in 50 mili seconds
      */
    void setUploadRate(int bytesPer50MS);
protected:
    explicit CopyRateController(QObject *parent = 0);

private slots:
    /**
      * \fn void signalTransferGO();
      * @brief Function connected such that it will cause a transfer piec with one of the Physical copiers in the transfer box
      */
    void signalTransferGO();
private:
    /**
      * \fn  void loadUploadSpeed();
      * @brief Function which uploads the upload speed
      */
    void loadUploadSpeed();


    /**
      * \fn CopierPhysical *next();
      * @brief A function to return the next CopierPhysical who will be allowed to transfer a file across the network
      * @return Returns the next CopierPhysical which will transfer a piece of the file
      */
    CopierPhysical *next();


private:
    QTimer transferTimer;

    QMutex lock;

    QList<CopierPhysical*> transferBox;

    int counter;

    //the maximum upload rate to slaves combined
    int maxUploadBytes;

};

#endif // COPYRATECONTROLLER_H
