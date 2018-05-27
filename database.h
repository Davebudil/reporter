#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QSqlDatabase>
#include <QSql>
#include "databaseconnector.h"
/*Class that manages database
 * and connection to it
 * */
class Database{
   public:
      //constructor
      Database();
      //Creates connection to database
      bool createConnection();
      //getters
      DatabaseConnector & getDatabaseConnector();
      QSqlDatabase & getDatabase();
      QSqlDatabase m_createDatabaseConnection();

   private:
      QSqlDatabase m_DB;
      DatabaseConnector m_DBConnect;
};

#endif // DATABASE_H
