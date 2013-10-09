#include "database.h"
#include "management.h"

//Sample Usage:
//
//Database::instance()->insert_query_example();
//QString s = Database::instance()->select_name_where_id(10);
//Database::drop();

Database* Database::m_instance = 0;

Database::Database()
{
    QSqlDatabase db;
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("AppManDB.db3");
    QFile file("AppManDB.db3");
    if (!file.exists()) {
        db.open();
        create_table_machine();
        create_table_machine_vitality();
    } else {
        db.open();
    }
}

bool Database::checkUniqueID(int Id){
    QSqlQuery q;
    QString s = "SELECT machineId FROM machine WHERE machineId='" + QString::number(Id) + "';";
    q.prepare(s);
    q.exec();
    q.next();
    if (q.value(0).toInt() == 0) {
        return false;
    } else {
        return true;
    }
}

void Database::create_table_machine() {
    QSqlQuery q;
    q.prepare("CREATE TABLE machine(machineId int PRIMARY KEY,IPaddr text);");
    q.exec();
}

void Database::create_table_machine_vitality() {
    QSqlQuery q;
    q.prepare("CREATE TABLE vitality(machineId int PRIMARY KEY, CPUusage int, RAMused int, RAMavail int, FOREIGN KEY(machineId) REFERENCES machine(machineId));");
    q.exec();
}
void Database::insert_machine(int Id, QString IPaddr) {
    QSqlQuery q;
    q.prepare("INSERT INTO machine (machineId,IPaddr) VALUES (" + QString::number(Id) + ",'" + IPaddr +  "');");
    q.exec();
}

void Database::insert_vitality(int machineId, int CPUusage, int RAMused, int RAMavail) {
    QSqlQuery q;
    q.prepare("INSERT INTO vitality (machineId,CPUusage,RAMused,RAMavail) VALUES (" + QString::number(machineId) + "," + QString::number(CPUusage) +  "," + QString::number(RAMused) + "," + QString::number(RAMavail) + ");");
    q.exec();
}

QString Database:: select_machineId_by_IPaddr(QString IPaddr) {
    QSqlQuery q;
    QString s = "SELECT machineId FROM machine WHERE IPaddr='" + IPaddr + "';";
    q.prepare(s);
    q.exec();
    q.next();
    return q.value(0).toString();
}

