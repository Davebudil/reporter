#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QSqlDatabase>
#include <QSql>
#include "databaseconnector.h"
/*Class that manages database
 * and connection to it
 * */
class Database : public QObject
{
      Q_OBJECT
   public:
      //constructor
      explicit Database(QObject *parent = nullptr);
      //Creates connection to database
      bool createConnection();
      //getters
      DatabaseConnector & getDatabaseConnector();
      QSqlDatabase & getDatabase();
      QSqlDatabase m_createDatabaseConnection();

   signals:

   public slots:

   private:
      QSqlDatabase m_DB;
      DatabaseConnector m_DBConnect;
};

#endif // DATABASE_H
