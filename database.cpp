#include "database.h"
#include "log.h"
//Constructor
Database::Database(){
//   m_DBConnect.setInfo("QMYSQL",
//                       "localhost",
//                       3306,
//                       "budil",
//                       "root",
//                       "Blizazrd5");
}
//Creates connection to DB

bool Database::createConnection(){
   if(m_DB.isOpen()){
      return true;
   }else if(m_DBConnect.connectDB(m_DB, "Default")){
      return true;
   }else{
      m_DB.close();
      return false;
   }
}
//Getter
DatabaseConnector & Database::getDatabaseConnector(){
   return m_DBConnect;
}
//Getter
QSqlDatabase & Database::getDatabase(){
   return m_DB;
}

QSqlDatabase Database::m_createDatabaseConnection(){
   QSqlDatabase tmp;
   qint32 randNumber = qrand();

   m_DBConnect.connectDB(tmp, QString::number(randNumber));
   return tmp;
}
