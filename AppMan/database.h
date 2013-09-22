#ifndef DATABASE_H
#define DATABASE_H

#include <QMutex>
#include <QtSql>
#include <QString>

/**
* @class Database
* @brief Singleton class structure that handles all Database access (using sqlite)
*/
class Database
{
public:
    /**
    * \fn static Database* instance();
    * @brief returns a static pointer to the database object, if one does not exist it creates one.
    */
    static Database* instance() {
        static QMutex mutex;
        if (!m_instance) {
            mutex.lock();

            if (!m_instance)
                m_instance = new Database();

            mutex.unlock();
        }

        return m_instance;
    }
    /**
    * \fn static void drop();
    * @brief drops and deletes the current pointer handle to the database
    */
    static void drop() {
        static QMutex mutex;
        mutex.lock();
        delete m_instance;
        m_instance = 0;
        mutex.unlock();
    }

    /**
    * \fn void create_table_example();
    * @brief creating a table with sqlite in QT example
    */
    void create_table_example();
    /**
    * \fn void insert_query_example();
    * @brief inserting data with sqlite in QT example
    */
    void insert_query_example();
    /**
    * \fn QString select_name_where_id_example(int id);
    * @brief selecting data with sqlite in QT example
    * @param id that has to match with expected name
    */
    QString select_name_where_id_example(int id);

    //add all additional/needed queries here and implement in database.cpp

private:
    /**
    * \fn Database();
    * @brief opens the database or creates it if it does not exist
    */
    Database();
    /**
    * @var m_instance
    * @brief static pointer to the database object
    */
    static Database* m_instance;
};

#endif // DATABASE_H
