#include "sysInfo.h"
//http://nadeausoftware.com/articles/2012/09/c_c_tip_how_get_physical_memory_size_system
sysInfo::sysInfo(){
#ifdef WIN32
    status.dwLength = sizeof(status);
    GlobalMemoryStatusEx(&status);
    GetSystemInfo(&siSysInfo);
    getDiskDetails();
    memoryLoad = QString::number(status.dwMemoryLoad/1.0);
    physMem = QString::number(status.ullTotalPhys/1024.0/1024.0,'f', 2);
    freePhysMem = QString::number(status.ullAvailPhys/1024.0/1024.0,'f', 2);
    procNum = QString::number(siSysInfo.dwNumberOfProcessors);
#else
    //Linux way to follow
#endif
}

sysInfo::~sysInfo(){

}

QStringList sysInfo::getAllSystemMemoryInfo()
{
    QStringList strings;
    strings.append("Percent of memory in use: " + memoryLoad + "%");
    strings.append("Total MB of physical memory: " + physMem + " MB");
    strings.append("Free MB of physical memory: " + freePhysMem + " MB");
    strings.append("Number of processors: " + procNum);
    strings.append("Total number of gigabytes on disk: " + diskSize + " GB");
    strings.append("Total number of free gigabytes on disk: " + freeOnDisk + " GB");
    return strings;
}

void sysInfo::getDiskDetails(){
#ifdef WIN32
    /*
     *http://www.tenouk.com/cpluscodesnippet/getdiskfreespacex.html
     */
    LPCWSTR pszDrive = NULL;
    BOOL test;
    __int64 lpFreeBytesAvailable, lpTotalNumberOfBytes, lpTotalNumberOfFreeBytes;
    test = GetDiskFreeSpaceEx(pszDrive,(PULARGE_INTEGER)&lpFreeBytesAvailable,(PULARGE_INTEGER)&lpTotalNumberOfBytes,(PULARGE_INTEGER)&lpTotalNumberOfFreeBytes);
    diskSize = QString::number(lpTotalNumberOfBytes/1024.0/1024.0/1024.0,'f', 2);
    freeOnDisk = QString::number(lpTotalNumberOfFreeBytes/1024.0/1024.0/1024.0,'f', 2);
#else
    //Linux way to follow
#endif
}

/*  To call this class and get all returns
 *  sysInfo s;
 *  QStringList mappy = s.getAllSystemMemoryInfo();
 *  for(int i = 0; i < mappy.size(); i++){
 *      qDebug()<<mappy.at(i);
 *  }
 */
