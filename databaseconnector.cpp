#include "databaseconnector.h"
#include <QtSql>
//Constructor
DatabaseConnector::DatabaseConnector(){
}
//Connects to database
bool DatabaseConnector::connectDB(QSqlDatabase & db, const QString & name){
   if(!db.open()){
      QSqlDatabase::removeDatabase(db.connectionName());
   }
   db = QSqlDatabase::addDatabase(m_dbType, name);
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
                                const qint32 port,
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
QString DatabaseConnector::getPassword() const{
   return m_usrPasswrd;
}
