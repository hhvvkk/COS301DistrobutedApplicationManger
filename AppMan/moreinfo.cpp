#include "moreinfo.h"
#include "ui_moreinfo.h"

moreInfo::moreInfo(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::moreInfo)
{
    ui->setupUi(this);
    QString detStats = "";

    detStats = "Data Transmitted,2.39695MB#Data Received,5.375KB#Packets Transmitted,44368#Packets Received,58#Errors Transmitting,0#Errors Receiving,0#Number of Processors,4#Operating System,Microsoft Windows [Version 6.1.7600]#Drive Label,C:,Total Capacity,450.662GB,Used Space,81.9109GB#Drive Label,E:,Total Capacity,465.759GB,Used Space,227.792GB#Total RAM,3.85533GB#Used RAM,2.07654GB#% RAM in use,46#% CPU in use,26#Amount of Processes active,112#0,System Idle Process#4,System#348,smss.exe#608,csrss.exe#780,wininit.exe#800,csrss.exe#840,services.exe#860,lsass.exe#868,lsm.exe#972,svchost.exe#236,winlogon.exe#388,nvvsvc.exe#616,svchost.exe#688,svchost.exe#516,svchost.exe#420,svchost.exe#1088,svchost.exe#1168,svchost.exe#1340,spoolsv.exe#1368,svchost.exe#1408,NvXDSync.exe#1420,nvvsvc.exe#1512,httpd.exe#1664,taskhost.exe#1744,AdminService.exe#1784,dsiwmis.exe#1832,dwm.exe#1852,ePowerSvc.exe#1876,GREGsvc.exe#1904,McSvHost.exe#1948,explorer.exe#1120,mfevtps.exe#528,NOBuAgent.exe#2052,IScheduleSvc.exe#2132,pg_ctl.exe#2208,rundll32.exe#2216,rundll32.exe#2224,rundll32.exe#2276,UpdaterService.exe#2324,WLIDSVC.EXE#2356,mcshield.exe#2364,postgres.exe#2380,conhost.exe#2496,mfefire.exe#2664,WLIDSVCM.EXE#2924,postgres.exe#2956,httpd.exe#2632,postgres.exe#2672,postgres.exe#2844,postgres.exe#2168,postgres.exe#2596,postgres.exe#3100,taskeng.exe#3156,clear.fiAgent.exe#3328,RAVCpl64.exe#3436,nvtray.exe#3448,OSPPSVC.EXE#3508,svchost.exe#3696,RAVBg64.exe#4072,DMREngine.exe#4080,igfxtray.exe#4088,hkcmd.exe#2608,igfxpers.exe#3228,SynTPEnh.exe#3344,BtvStack.exe#3468,AthBtTray.exe#3692,ePowerTray.exe#3712,SynTPHelper.exe#1824,igfxsrvc.exe#4168,ApacheMonitor.exe#4268,IAStorIcon.exe#4276,mcagent.exe#4284,SuiteTray.exe#4300,PmmUpdate.exe#4328,BackupManagerTray.exe#4344,nusb3mon.exe#4412,igfxext.exe#4864,unsecapp.exe#4932,WmiPrvSE.exe#5008,EgisUpdate.exe#5024,LManager.exe#5040,ePowerEvent.exe#5076,SearchIndexer.exe#5112,MMDx64Fx.exe#3660,LMworker.exe#1448,clear.fiMovieService.exe#1796,jusched.exe#4644,RIMBBLaunchAgent.exe#4836,BbDevMgr.exe#4292,svchost.exe#6056,Rim.Desktop.AutoUpdate.ex#1560,PresentationFontCache.exe#5712,IAStorDataMgrSvc.exe#3300,LMS.exe#4880,daemonu.exe#6764,wmpnetwk.exe#7008,UNS.exe#6516,svchost.exe#6252,wuauclt.exe#1844,vlc.exe#6264,Steam.exe#5176,qtcreator.exe#5580,audiodg.exe#1152,notepad++.exe#1056,AppMan.exe#6172,SearchProtocolHost.exe#2876,SearchFilterHost.exe#6844,WmiPrvSE.exe#6604,AppManClient.exe#4340,cmd.exe#5172,conhost.exe#1288,tasklist.exe" ;

    QStringList noHash = detStats.split("#");
    QStringList * allInfo = parseDetailed(noHash);
    setNetw(allInfo[0]);
    setCPU(allInfo[1]);
    setRAM(allInfo[2]);
    setHDD(allInfo[3]);
    setProcesses(allInfo[4]);
}

moreInfo::~moreInfo()
{
    delete ui;

}

QStringList * moreInfo::parseDetailed(QStringList input){
    QStringList * parsed = new QStringList[5];
    for(int i = 0; i < input.length(); i++){
        if(i < 6){
            parsed[0]<<input.at(i);
        }
        else if(i < 8){
            parsed[1]<<input.at(i);
        }
        else{
            if(input.at(i).contains("Drive Label")){
                parsed[3]<<input.at(i);
            }
            else if(input.at(i).contains("Amount")){
                parsed[1]<<input.at(i);
            }
            else if(input.at(i).contains("RAM")){
                parsed[2]<<input.at(i);
            }
            else if(input.at(i).contains("CPU")){
                parsed[2]<<input.at(i);
            }
            else{
                parsed[4]<<input.at(i);
            }
        }
    }
    return parsed;
}

void moreInfo::setRAM(QStringList data){
    QString * strings = new QString[data.length()];
    for(int i = 0; i < data.length(); i++){
        strings[i] = data.at(i);
    }
    QTreeWidget * widg = new QTreeWidget();
    widg->setColumnCount(2);
    QStringList headers;
    headers.append("Property");
    headers.append("Value");
    widg->setHeaderLabels(headers);
    QTreeWidgetItem *item;
    QStringList stringyListy;
    for(int i = 0; i < data.length(); i++){
        stringyListy = strings[i].split(",");
        item = new QTreeWidgetItem();
        item->setText(0,stringyListy.at(0));
        item->setText(1,stringyListy.at(1));
        widg->addTopLevelItem(item);
    }
    ui->vlRAM->addWidget(widg);
}

void moreInfo::setCPU(QStringList data){
    QString * strings = new QString[data.length()];
    for(int i = 0; i < data.length(); i++){
        strings[i] = data.at(i);
    }
    QTreeWidget * widg = new QTreeWidget();
    widg->setColumnCount(2);
    QStringList headers;
    headers.append("Property");
    headers.append("Value");
    widg->setHeaderLabels(headers);
    QTreeWidgetItem *item;
    QStringList stringyListy;
    for(int i = 0; i < data.length(); i++){
        stringyListy = strings[i].split(",");
        item = new QTreeWidgetItem();
        item->setText(0,stringyListy.at(0));
        item->setText(1,stringyListy.at(1));
        widg->addTopLevelItem(item);
    }
    ui->vlCPU->addWidget(widg);
}

void moreInfo::setHDD(QStringList data){
    QString * strings = new QString[data.length()];
    for(int i = 0; i < data.length(); i++){
        strings[i] = data.at(i);
    }
    QTreeWidget * widg = new QTreeWidget();
    widg->setColumnCount(3);
    QStringList headers;
    headers.append("Drive Label");
    headers.append("Total Capacity");
    headers.append("Used Space");
    widg->setHeaderLabels(headers);
    QTreeWidgetItem *item;
    QStringList stringyListy;
    for(int i = 0; i < data.length(); i++){
        stringyListy = strings[i].split(",");
        item = new QTreeWidgetItem();
        item->setText(0,stringyListy.at(1));
        item->setText(1,stringyListy.at(3));
        item->setText(2,stringyListy.at(5));
        widg->addTopLevelItem(item);
    }
    ui->vlHDD->addWidget(widg);
}

void moreInfo::setNetw(QStringList data){
    QString * strings = new QString[data.length()];
    for(int i = 0; i < data.length(); i++){
        strings[i] = data.at(i);
    }
    QTreeWidget * widg = new QTreeWidget();
    widg->setColumnCount(2);
    QStringList headers;
    headers.append("Property");
    headers.append("Value");
    widg->setHeaderLabels(headers);
    QTreeWidgetItem *item;
    QStringList stringyListy;
    for(int i = 0; i < data.length(); i++){
        stringyListy = strings[i].split(",");
        item = new QTreeWidgetItem();
        item->setText(0,stringyListy.at(0));
        item->setText(1,stringyListy.at(1));
        widg->addTopLevelItem(item);
    }
    ui->vlNetStat->addWidget(widg);
}

void moreInfo::setProcesses(QStringList data){
    QString * strings = new QString[data.length()];
    for(int i = 0; i < data.length(); i++){
        strings[i] = data.at(i);
    }
    QTreeWidget * widg = new QTreeWidget();
    widg->setColumnCount(2);
    QStringList headers;
    headers.append("Process ID");
    headers.append("Process Name");
    widg->setHeaderLabels(headers);
    QTreeWidgetItem *item;
    QStringList stringyListy;
    for(int i = 0; i < data.length(); i++){
        stringyListy = strings[i].split(",");
        item = new QTreeWidgetItem();
        item->setText(0,stringyListy.at(0));
        item->setText(1,stringyListy.at(1));
        widg->addTopLevelItem(item);
    }
    ui->vlProcesses->addWidget(widg);
}

void moreInfo::on_pushButton_clicked()
{
    this->close();
}
