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
class sysInfo{
private:
    //THE MINIMAL STATS
    /**
     * @fn void cpuUsage();
     * @brief cpuUsage sets the CPU usage % variable
     */
    void cpuUsage();
    /**
     * @fn void memPerc();
     * @brief memPerc reads memory (ram) and calculates percentage used
     */
    void memPerc();
    /**
     * @fn void netStat();
     * @brief netStat sets the bytes transmitted and received variables
     * @bug Packets received stays 0 in windows
     */
    void netStat();

    //THE DETAILED STATS
    /**
     * @fn void osVersion();
     * @brief osVersion sets the Operating System version variable
     */
    void osVersion();
    /**
    * @fn void cpuStats();
    * @brief cpuStats gets more statistics from the CPU
    */
    void cpuStats();
    /**
     * @fn void getDiskDetails();
     * @brief getDiskDetails initializes diskSize and freeOnDisk
     */
    void getDiskDetails();
    /**
     * @fn void listProcesses();
     * @brief listProcesses populates the processes QStringList
     */
    void listProcesses();

    //INTERNAL EXTRA FUNCTIONS
    /**
     * @fn void popHDDList(QString driveLabel);
     * @brief popHDDList populates the list of filesystems on windows
     * @param driveLabel the label of the hdd to check
     */
    void popHDDList(QString driveLabel);
     /**
     * @fn QString getSizeUnit(QString inval);
     * @brief getSizeUnit analyses the input and returns it in a simpler unit (B/KB/MB/GB)
     * @param inval the string passed through
     * @return the new string with the measurement unit (B/KB/MB/GB)
     */
    QString getSizeUnit(QString inval);
    /**
     * @fn QString recrun(QString in);
     * @brief recrun is a recursive function to run
     * @param in the QString input
     * @return the QString containing the stripped line
     */
    QString recrun(QString in);
    //VARIABLES
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
    QString _cpuCount;
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
     * @var bytesTransmitted
     * @brief bytesTransmitted is the amount of bytes transmitted to be returned as a QString
     */
    QString bytesTransmitted;
    /**
     * @var bytesReceived
     * @brief bytesReceived is the amount of bytes received to be returned as a QString
     */
    QString bytesReceived;
    /**
     * @var packetsReceived
     * @brief packetsReceived is the amount of packets that have been received
     */
    QString packetsReceived;
    /**
     * @var packetsTransmitted
     * @brief packetsTransmitted is the amount of packets that have been transmitted
     */
    QString packetsTransmitted;
    /**
     * @var receiveErrors
     * @brief receiveErrors the amount of errors encountered while receiving data
     */
    QString receiveErrors;
    /**
     * @var transmitErrors
     * @brief transmitErrors the amount of errors encountered while transmitting data
     */
    QString transmitErrors;
    /**
     * @var filesystems
     * @brief filesystems the QStringList containing filesystem names
     */
    QStringList filesystems;
    /**
     * @var capacitys
     * @brief capacitys the QStringList containing filesystem capacities
     */
    QStringList capacitys;
    /**
     * @var useds
     * @brief useds the QStringList containing filesystem used space
     */
    QStringList useds;
    /**
     * @var osVers
     * @brief osVers the version of the Operating System
     */
    QString osVers;
    /**
     * @var cpuPerc
     * @brief cpuPerc the load of the cpu
     */
    QString cpuPerc;
    /**
     * @var procIDs
     * @brief procIDs the QStringList containing process ID's
     */
    QStringList procIDs;
    /**
     * @var procNames
     * @brief procNames the QStringList containing process names
     */
    QStringList procNames;
    /**
     * @var procCount
     * @brief procCount the amount of processes
     */
    QString procCount;
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

    //FUNCTIONS TO BE CALLED
    /**
     * @fn QString getMinimalStats();
     * @brief getMinimalStats returns the stats for the main GUI
     * @return a QString delimited with # between types and , between name/value pairs
     */
    QString getMinimalStats();
    /**
     * @fn QString getDetailedStats();
     * @brief getDetailedStats returns the stats for the dialog requesting more info
     * @return a QString delimited with # between types and , between name/value pairs
     */
    QString getDetailedStats();
};

#endif // SYSINFO_H
