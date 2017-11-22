#include "database.h"
//Constructor
Database::Database(QObject *parent)
   : QObject(parent){
   m_DBConnect.setInfo("QMYSQL",
                       "localhost",
                       3306,
                       "budil",
                       "root",
                       "Blizazrd5");
}
//Creates connection to DB
bool Database::createConnection(){
   if(m_DB.isOpen()){
      return true;
   }
   if(m_DBConnect.connectDB(m_DB)){
      return true;
   }else{
      m_DB.close();
      return false;
   }
}
//Getter
DatabaseConnector& Database::getDatabaseConnector(){
   return m_DBConnect;
}
//Getter
QSqlDatabase& Database::getDatabase(){
   return m_DB;
}
