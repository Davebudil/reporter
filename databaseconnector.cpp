#include "databaseconnector.h"
#include <QtSql>
//Constructor
DatabaseConnector::DatabaseConnector(QObject *parent)
   : QObject(parent){
}
//Connects to database
bool DatabaseConnector::connectDB(QSqlDatabase & db){
   if(!db.open()){
      QSqlDatabase::removeDatabase(db.connectionName());
   }
   db = QSqlDatabase::addDatabase(m_dbType);
   db.setHostName(m_hostName);
   db.setPort(m_port);
   db.setDatabaseName(m_dbName);
   db.setUserName(m_usrName);
   db.setPassword(m_usrPasswrd);
   return db.open();
}
//Sets info used during connection
void DatabaseConnector::setInfo(const QString &dbType,
                                const QString &host,
                                const int port,
                                const QString &dbName,
                                const QString &usrName,
                                const QString &usrPass){
   m_dbType = dbType;
   m_hostName = host;
   m_port = port;
   m_dbName = dbName;
   m_usrName = usrName;
   m_usrPasswrd = usrPass;
}
//Getter for password
QString DatabaseConnector::getPassword(){
   return m_usrPasswrd;
}
