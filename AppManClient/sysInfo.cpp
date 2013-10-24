#include "sysInfo.h"
//http://nadeausoftware.com/articles/2012/09/c_c_tip_how_get_physical_memory_size_system
sysInfo::sysInfo(){

}

sysInfo::~sysInfo(){

}

void sysInfo::memPerc(){
#ifdef WIN32
    //use a Windows defined struct and its variables
    status.dwLength = sizeof(status);
    GlobalMemoryStatusEx(&status);
    memoryLoad = QString::number(status.dwMemoryLoad/1.0);
    physMem = QString::number(status.ullTotalPhys,'f', 2);
    freePhysMem = QString::number(status.ullAvailPhys,'f', 2);
#else
    //reads the data from /proc/meminfo and parses if for relevant data
    QFile inputFile("/proc/meminfo");
    QString memtotal, freemem;
    if (inputFile.open(QIODevice::ReadOnly | QIODevice::Text))
        {
        QTextStream in(&inputFile);
        QString line;
           while ( (line = in.readLine()) != NULL )
           {
              if(line.contains("MemTotal")){
                  line.replace(" ","#");
                  QString delimited = recrun(line);
                  delimited.remove(0,10);
                  delimited.remove(delimited.length()-3,3);
                  physMem = delimited;
              }
              else if(line.contains("MemFree")){
                  line.replace(" ","#");
                  QString delimited = recrun(line);
                  delimited.remove(0,9);
                  delimited.remove(delimited.length()-3,3);
                  freePhysMem = delimited;
              }
           }
           double totMem = physMem.toDouble();
           double usedMem = totMem - freePhysMem.toDouble();
           double perc = (usedMem/totMem)*100.0;
           memoryLoad = QString::number(perc);
        }
    else{
        qDebug()<<"error reading file";
    }
    inputFile.close();
#endif
//    qDebug()<<"Total Ram: "<<physMem;
//    qDebug()<<"Free Ram: "<<freePhysMem;
//    qDebug()<<"Percentage RAM in use:"<<memoryLoad;
}

void sysInfo::getDiskDetails(){
#ifdef WIN32
    //Loop through all possible drives and see if they exist with popHDDList(mydrive);
    QString mydrive;
    for(char ch = 'A'; ch<='Z'; ch++){
        mydrive = ch;
        mydrive += ":";
        popHDDList(mydrive);
    }
#else
    //uses popen to make a file pointer to a system call "df -B K" which displays all filesystems with capacity and load in KB
    FILE *pop = popen("df -B K","r");
    size_t bufsize = 1024 * sizeof(char);
    char* buffer = (char*)malloc( bufsize );
    QString prev = "";
    QStringList listy;
    int count =0;
    while(getline(&buffer,&bufsize ,pop)){
        QString line = buffer;
        //prev to avoid infinite loops
        if(line.compare(prev)==0){
            break;
        }
        else{
            //prev and line cant be the same above to avoid infinite loops
            prev = line;
            //string mainpulation
            line.replace("\n","#");
            line.replace(" ","#");
            count++;
            //skip the header
            if(count != 1){
                listy<<line;
            }
        }
    }
    for(int i = 0; i < listy.length(); i++){
        //strip #'s from listy[i]
        listy[i] = recrun(listy[i]);
        //tokenise listy[i]
        QStringList listyElements = listy[i].split("#");
        //add listy contents to relevant containers
        filesystems<<listyElements[0];
        QString cap = listyElements[1];
        cap.chop(1);
        double capa = cap.toDouble();
        capa = capa * 1024;
        cap = QString::number(capa);
        QString use = listyElements[2];
        use.chop(1);
        double used = use.toDouble();
        used = used * 1024;
        use = QString::number(used);
        capacitys<<cap;
        useds<<use;
    }

#endif
    for(int i = 0; i < filesystems.length(); i++){
//        qDebug()<<"FILESYSTEM:\t"<<filesystems[i]<<"\t"<<capacitys[i]<<"\t"<<useds[i];
    }
}

void sysInfo::netStat(){
#ifdef WIN32
    //Call cmd command netstat-e, save to file and parse for relevant data
    system("netstat -e > netstat.txt");
    QFile inputFile("netstat.txt");
    long tranPacket = 0;
    long reciPacket = 0;
    if (inputFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream in(&inputFile);
        QString read;
        bool OK = true;
        while ( OK)
        {
            read = in.readLine();
            if(read.contains("Bytes")){
                read.replace(" ","#");
                QString delimited = recrun(read);
                QStringList tokens = delimited.split("#");
                bytesReceived = tokens.at(1);
                bytesTransmitted = tokens.at(2);
            }
            else if(read.contains("Errors")){
                read.replace(" ","#");
                QString delimited = recrun(read);
                QStringList tokens = delimited.split("#");
                receiveErrors = tokens.at(1);
                transmitErrors = tokens.at(2);
            }
            else if(read.contains("Unicast packets")){
                read.replace(" ","#");
                QString delimited = recrun(read);
                QStringList tokens = delimited.split("#");
                reciPacket += tokens.at(2).toLong();
                tranPacket += tokens.at(3).toLong();
            }
            else if(read.contains("Non-unicast packets")){
                read.replace(" ","#");
                QString delimited = recrun(read);
                QStringList tokens = delimited.split("#");
                reciPacket += tokens.at(2).toLong();
                tranPacket += tokens.at(3).toLong();
            }
            if(in.atEnd()){
                OK=false;
            }
        }
        packetsReceived = QString::number(reciPacket);
        packetsTransmitted = QString::number(tranPacket);
    }
    else{
        qDebug()<<"error reading file";
    }
    //Remove the file
    QFile::remove("netstat.txt");
    inputFile.close();
#else
    //reads the data from /proc/net/dev and parses if for relevant data
    QFile inputFile("/proc/net/dev");
    if (inputFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream in(&inputFile);
        QString read;
        while ( ( read= in.readLine()) != NULL)
        {
            if(read.contains("eth0")){
                read.replace(" ","#");
                QString delimited = recrun(read);
                delimited.remove(0,7);
                QStringList tokens = delimited.split("#");
                bytesReceived = tokens.at(0);
                packetsReceived = tokens.at(1);
                receiveErrors = tokens.at(2);
                bytesTransmitted = tokens.at(8);
                packetsTransmitted = tokens.at(9);
                transmitErrors = tokens.at(10);
            }
        }
    }
    else{
        qDebug()<<"error reading file";
    }
    inputFile.close();
#endif
//    qDebug()<<"Bytes received: "<<bytesReceived;
//    qDebug()<<"Bytes transmitted: "<<bytesTransmitted;
//    qDebug()<<"Packets received: "<<packetsReceived;
//    qDebug()<<"Packets transmitted: "<<packetsTransmitted;
//    qDebug()<<"Receive error count: "<<receiveErrors;
//    qDebug()<<"transmit error count: "<<transmitErrors;

}

QString sysInfo::recrun(QString in){
    //loop through string recursively to strip unnecessary #'s
    int ip = in.length();
    for(int i = 0; i < ip; i++){
        if((in.at(i)=='#'&&in.at(i+1)=='#')){
            in.remove(i,1);
            return recrun(in);
        }
    }
    return in;
}

void sysInfo::osVersion(){
#ifdef WIN32
    //Call cmd command ver, save to file and parse for relevant data
    system("ver>ver.txt");
    QFile inputFile("ver.txt");
    if (inputFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream in(&inputFile);
        QString read;
        bool OK = true;
        while ( OK)
        {
            read = in.readLine();
            if(read.contains("Microsoft")){
                osVers = read;
            }
            if(in.atEnd()){
                OK=false;
            }
        }
    }
    else{
        qDebug()<<"error reading file";
    }
    //Remove the file
    QFile::remove("ver.txt");
    inputFile.close();
#else
    //reads the data from /proc/version and parses if for relevant data, cutting away excess info
    QFile inputFile("/proc/version");
    if (inputFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream in(&inputFile);
        QString read;
        while ( ( read= in.readLine()) != NULL){
            int posit;
            while((posit = read.indexOf("(")) != -1){
                read.chop(read.length()-posit+1);
            }
            osVers = read;
        }
    }
    else{
        qDebug()<<"error reading file";
    }
    inputFile.close();
#endif
//    qDebug()<<"OS Version: "<<osVers;
}

void sysInfo::cpuUsage(){
#ifdef WIN32
    //Call cmd command wmic cpu get loadpercentage, save to file and parse for relevant data
    system("wmic cpu get loadpercentage > cpu.txt");
    QFile inputFile("cpu.txt");
    if (inputFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream in(&inputFile);
        QString line;
        while ( (line = in.readLine()) != NULL )
        {
            if(!line.contains("LoadPercentage")){
                line.remove(" ");
                cpuPerc = line;
            }
        }
    }
    else{
        qDebug()<<"error reading file";
    }
    //Remove the file
    QFile::remove("cpu.txt");
    inputFile.close();
#else
    //reads the data from /proc/stat and parses if for relevant data
    QFile inputFile("/proc/stat");
    if (inputFile.open(QIODevice::ReadOnly | QIODevice::Text))
        {
           QTextStream in(&inputFile);
        QString read;
           while ( ( read= in.readLine()) != NULL)
           {
           if(read.contains("cpu")){
               break;
           }
           }
       read.replace(" ","#");
       read = recrun(read);
       QStringList cpuToke = read.split("#");
       int  user,nice,system,idle,iowait,irq,softirq;
       user = cpuToke[1].toInt();
       nice = cpuToke[2].toInt();
       system = cpuToke[3].toInt();
       idle = cpuToke[4].toInt();
       iowait = cpuToke[5].toInt();
       irq = cpuToke[6].toInt();
       softirq = cpuToke[7].toInt();
       float total = user + nice + system + idle + iowait + irq + softirq;
       float used = total - idle;
       float perc = used/total*100;
       cpuPerc = QString::number(perc);
        }
    else{
        qDebug()<<"error reading file";
    }
    inputFile.close();
#endif
//     qDebug()<<"CPU usage percentage: "<<cpuPerc;
}

void sysInfo::cpuStats(){
#ifdef WIN32
    //Use windows struct and access variable
    GetSystemInfo(&siSysInfo);
    _cpuCount = QString::number(siSysInfo.dwNumberOfProcessors);
#else
    int cpuCount = 0;
    //reads the data from/proc/cpuinfo and counts occurences for a cpu count
    QFile inputFile("/proc/cpuinfo");
    if (inputFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream in(&inputFile);
        QString read;
        bool OK = true;
        while ( OK)
        {
            read = in.readLine();
            if(read.contains("processor")){
                cpuCount++;
            }
            if(in.atEnd()){
                OK=false;
            }
        }
        _cpuCount = QString::number(cpuCount);
    }
    else{
        qDebug()<<"error reading file";
    }
#endif
//   qDebug()<<"Number of processors: "<<_cpuCount;
}

void sysInfo::listProcesses(){
#ifdef WIN32
    //call cmd function tasklist and save to file, parse file for relevant data
    system("tasklist>tasklist.txt");
    int count = 0;
    QFile inputFile("tasklist.txt");
    if (inputFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream in(&inputFile);
        QString read;
        bool OK = true;
        while ( OK)
        {
            read = in.readLine();
            count++;
            if(in.atEnd()){
                OK=false;
            }
            //First 3 lines are headers
            if((count > 3) && (!read.compare("")==0)){
                read.replace(" ","#");
                QString stripped = recrun(read);
                QStringList tokens = stripped.split("#");
                if(tokens.size()>5){
                    QString PID = tokens.at(tokens.size()-5);
                    QString taskNm;
                    //if the process name is a single word
                    if(tokens.size()-6 == 0){
                        taskNm = tokens.at(tokens.size()-6);
                    }
                    else{
                        //if the process name is ,multiple words
                        for(int i =0; i <= tokens.size()-6; i++){
                            if(i == tokens.size()-6){
                                taskNm += tokens.at(i);
                            }
                            else{
                                taskNm += tokens.at(i) +" ";
                            }
                        }
                    }
                    procIDs<<PID;
                    procNames<<taskNm;
                }
            }
        }
    }
    else{
        qDebug()<<"error reading file";
    }
#else
    //uses popen to make a file pointer to a system call "ps -e" which displays all processes
    FILE *pop = popen("ps -e","r");
    size_t bufsize = 1024 * sizeof(char);
    char* buffer = (char*)malloc( bufsize );
    QString prev = "";
    QStringList listy;
    int count =0;
    while(getline(&buffer,&bufsize ,pop)){
        QString line = buffer;
        //to avoid infinte loop
        if(line.compare(prev)==0){
            break;
        }
        else{
            count++;
            //to avoid infinite loops prev and line must never be the same above
            prev = line;
            //string manipulation
            line.replace("\n","#");
            line.replace(" ","#");
            line = recrun(line);
            listy = line.split("#");
            //the first line is a header
            if((count !=1)&&(!listy[4].isEmpty())){
                procIDs<<listy[1];
                procNames<<listy[4];
            }
        }
    }
#endif
    procCount = QString::number(procIDs.length());
    for(int i = 0; i < procIDs.length(); i++){
//		qDebug()<<procIDs[i]<<"\t"<<procNames[i];
    }
//	qDebug()<<"There are: "<<procCount<<" processes running";
}

void sysInfo::popHDDList(QString driveLabel){
#ifdef WIN32
    //make the passed driveLabel from getDiskDetails() a wstring
    std::wstring wstr = driveLabel.toStdWString();
    //turn the wstring into a Long Pointer to Constant Wide String(LPCWSTR)
    LPCWSTR pszDrive = wstr.c_str();
    //create a bool value that tests if the drivelabel is correct
    bool test;
    __int64 lpFreeBytesAvailable, lpTotalNumberOfBytes, lpTotalNumberOfFreeBytes;
    //use windows GetDiskFreeSpaceEx, returns true to test if pszDrive is a logical drive, assigns values to _int64's declared above
    test = GetDiskFreeSpaceEx(pszDrive,(PULARGE_INTEGER)&lpFreeBytesAvailable,(PULARGE_INTEGER)&lpTotalNumberOfBytes,(PULARGE_INTEGER)&lpTotalNumberOfFreeBytes);
    //if driveLabel is a drive add this drive and it's details to the QStringList
    if(test){
        diskSize = QString::number(lpTotalNumberOfBytes,'f', 2);
        freeOnDisk = QString::number(lpTotalNumberOfFreeBytes,'f', 2);
        double cap = diskSize.toDouble();
        double free = freeOnDisk.toDouble();
        double used = cap - free;
        filesystems<<driveLabel;
        capacitys<<diskSize;
        useds<<QString::number(used);
    }
#endif
}

QString sysInfo::getMinimalStats(){
    netStat();
    cpuUsage();
    memPerc();
    bytesTransmitted = getSizeUnit(bytesTransmitted);
    bytesReceived = getSizeUnit(bytesReceived);
    //Sende the CPU usage %, the RAM usage %, the amount of transmitted data in the applicable unit, the amount of received data in the applicable unit
    QString stats = cpuPerc + "%" + "#" + memoryLoad + "%" + "#"+ bytesTransmitted + "#" + bytesReceived;
    return stats;
}

QString sysInfo::getDetailedStats(){
    cpuUsage();
    memPerc();
    netStat();
    cpuStats();
    getDiskDetails();
    osVersion();
    listProcesses();
    QString stats = "";
    bytesTransmitted = getSizeUnit(bytesTransmitted);
    bytesReceived = getSizeUnit(bytesReceived);
    //Add the netstat info to the stats string
    stats = stats + "Data Transmitted," + bytesTransmitted + "#Data Received," + bytesReceived + "#Packets Transmitted," + packetsTransmitted + "#Packets Received," + packetsReceived + "#Errors Transmitting," + transmitErrors + "#Errors Receiving," + receiveErrors;
    //Add the cpuStats to the stats string
    stats = stats + "#Number of Processors," + _cpuCount;
    //Add the OS Version
    stats = stats + "#Operating System," + osVers;
    //Add the various logical drive details
    for(int i = 0; i < filesystems.length(); i++){
        QString cap = capacitys.at(i);
        cap.chop(cap.length() - cap.indexOf("."));
        cap = getSizeUnit(cap);
        QString used = useds.at(i);
        used = getSizeUnit(used);
        stats = stats + "#Drive Label," + filesystems.at(i) + ",Total Capacity," + cap+ ",Used Space," + used;
    }
    //Add RAM and CPU
    physMem.chop(physMem.length()- physMem.indexOf("."));
    physMem = getSizeUnit(physMem);
    freePhysMem.chop(freePhysMem.length() - freePhysMem.indexOf("."));
    freePhysMem = getSizeUnit(freePhysMem);
    stats = stats + "#Total RAM,"+physMem+"#Used RAM,"+freePhysMem+"#% RAM in use,"+memoryLoad+"#% CPU in use,"+cpuPerc;
    //Add the process/task related info
    stats = stats + "#Amount of Processes active," + procCount;
    for(int j = 0; j < procIDs.length(); j++){
        stats = stats + "#" + procIDs.at(j) + "," + procNames.at(j);
    }
    filesystems.clear();
    capacitys.clear();
    useds.clear();
    procIDs.clear();
    procNames.clear();
    procCount.clear();
    return stats;
}

QString sysInfo::getSizeUnit(QString inval){
    double invalDoub;
    if(inval.length() > 3 && inval.length() < 7){
        invalDoub = inval.toDouble()/1024.0;
        inval = QString::number(invalDoub) + "KB";
    }
    else if(inval.length() > 6 && inval.length() < 10){
        invalDoub = inval.toDouble()/1024.0/1024.0;
        inval = QString::number(invalDoub) + "MB";
    }
    else if(inval.length() > 9 && inval.length() < 13){
        invalDoub = inval.toDouble()/1024.0/1024.0/1024.0;
        inval = QString::number(invalDoub) + "GB";
    }
    else{
        inval += "B";
    }
    return inval;
}
