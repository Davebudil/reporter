#include "sqlstorage.h"
//Constructor
SQLStorage::SQLStorage(QObject *parent)
   : QObject(parent){

}

QMap<QString, SQLquery*> SQLStorage::getQueries(){
   return m_Queries;
}

QList<SQLParameter> SQLStorage::getParameters(){
   return m_Parameters;
}

bool SQLStorage::m_loadQueries(){
   return true;
}
bool SQLStorage::m_loadParameters(){
   return true;
}
QSqlQuery SQLStorage::getResultQuery(){
   return m_Query;
}

bool SQLStorage::addQuery(const QString & query, const QString & name, const QString & param, bool display){
   SQLquery * tmp = new SQLquery(query,name,param);
   if(m_Queries.contains(name)){
      if(display){
         QMessageBox::warning(0,"New Query Error", "Query with this name already exists.");
         return false;
      }
      return false;
   }
   if((param != "0") && (!m_Queries.contains(param))){
      QMessageBox::warning(0,"New Query Error", "No query with specified master param name exists.");
      return false;
   }
   m_Queries[name] = tmp;
   return true;
}

void SQLStorage::printQueries(){
   for(auto & it : m_Queries){
      it->printValue();
   }
}

void SQLStorage::generateQuery(const QString & name){
   m_Queries[name]->generateQuery();
}
