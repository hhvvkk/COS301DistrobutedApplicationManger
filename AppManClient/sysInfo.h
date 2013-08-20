/**
 * @file sysInfo.h
 * @brief the File with our system info collector for windows
 */

#ifndef SYSINFO_H
#define SYSINFO_H

#include <QMap>
#include <QString>
#ifdef WIN32
    #include <windows.h>
#endif
#include <QDebug>
#include <QStringList>
#include <QFile>
#include <QTextStream>

/**
 * @class sysInfo
 * @brief The sysInfo class
 */
////////////////////////NOTE: TO UNCOMMENT HIGHLIGHT ALL AND PRESS ( [Ctrl] + [/] )
class sysInfo{
private:
#ifdef WIN32
    /**
     * @var status
     * @brief status is a MEMORYSTATUSEX struct
     */
    MEMORYSTATUSEX status;
    /**
     * @var siSysInfo
     * @brief siSysInfo is a SYSTEM_INFO struct
     */
    SYSTEM_INFO siSysInfo;
#endif
    /**
     * @var memoryLoad
     * @brief memoryLoad is a QString for the % of memory (RAM) in use
     */
    QString memoryLoad;
    /**
     * @var physMem
     * @brief physMem is a QString for the GB representation of total memory (RAM)
     */
    QString physMem;
    /**
     * @var freePhysMem
     * @brief freePhysMem is a QString for the GB representation of free memory (RAM)
     */
    QString freePhysMem;
    /**
     * @var procNum
     * @brief procNum is a QString for the number of processors
     */
    QString procNum;
    /**
     * @var diskSize
     * @brief diskSize is a QString for the total size of the disk
     */
    QString diskSize;
    /**
     * @var freeOnDisk
     * @brief freeOnDisk is a QString for the free space on the disk
     */
    QString freeOnDisk;
    /**
     * @fn void getDiskDetails();
     * @brief getDiskDetails initializes diskSize and freeOnDisk
     */
    void getDiskDetails();
    /**
     * @var bytesTransmitted
     * @brief bytesTransmitted is the amount of bytes transmitted to be returned as a QString
     */
    QString bytesTransmitted;
    /**
     * @var bytesReceived
     * @brief bytesReceived is the amount of bytes received to be returned as a QString
     */
    QString bytesReceived;
public:
    /**
     * @fn sysInfo();
     * @brief sysInfo is the default constructor
     */
    sysInfo();
    /**
     * @fn ~sysInfo();
     * @brief ~sysInfo is the destructor
     */
    ~sysInfo();
    /**
     * @fn QStringList getAllSystemMemoryInfo();
     * @brief getAllSystemMemoryInfo returns a QStringList of all the values stored
     * @return QSringList
     */
    QStringList getAllSystemMemoryInfo();
    /**
     * @fn QString getMemoryLoad();
     * @brief accessor for memoryLoad
     * @return memoryLoad
     */
    QString getMemoryLoad() {return memoryLoad;}
    /**
     * @fn QString getPhysMem();
     * @brief accessor for physMem
     * @return physMem
     */
    QString getPhysMem() {return physMem;}
    /**
     * @fn QString getFreePhysMem();
     * @brief accessor for freePhysMem
     * @return freePhysMem
     */
    QString getFreePhysMem() {return freePhysMem;}
    /**
     * @fn QString getProcNum();
     * @brief accessor for procNum
     * @return procNum
     */
    QString getProcNum() {return procNum;}
    /**
     * @fn QString getDiskSize();
     * @brief accessor for diskSize
     * @return diskSize
     */
    QString getDiskSize() {return diskSize;}
    /**
     * @fn QString getFreeOnDisk();
     * @brief accessor for freeOnDisk
     * @return freeOnDisk
     */
    QString getFreeOnDisk() {return freeOnDisk;}

};

#endif // SYSINFO_H