#include "database.h"

//Sample Usage:

//Database::instance()->create_table_example();
//Database::instance()->insert_query_example();
//QString s = Database::instance()->select_name_where_id(10);
//Database::drop();

Database* Database::m_instance = 0;

Database::Database()
{
    QSqlDatabase db;
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("AppManDB.db3");
    db.open();
}

void Database::create_table_example() {
    QSqlQuery q;
    q.prepare("CREATE TABLE t1(id int,name text);");
    q.exec();
}

void Database::insert_query_example() {
    QSqlQuery q;
    q.prepare("INSERT INTO t1 (id,name) VALUES (10,'Joe');");
    q.exec();
}

QString Database::select_name_where_id_example(int id) {
    QSqlQuery q;
    QString s = "SELECT name FROM t1 WHERE id=" + QString::number(id) + ";";
    q.prepare(s);
    q.exec();
    q.next();
    return q.value(0).toString();
}

