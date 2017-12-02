#include "sqlstorage.h"
//Constructor
SQLStorage::SQLStorage(QObject *parent)
   : QObject(parent){

}

SQLStorage::~SQLStorage(){
   //DELETE MAP CONTENT
}
//Getters
QMap<QString, SQLquery*> & SQLStorage::getQueries(){
   return m_Queries;
}

QMap<qint32, SQLParameter*> & SQLStorage::getParameters(){
   return m_Parameters;
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

void SQLStorage::printParams(){
   for(auto & it : m_Parameters){
      it->printParams();
   }
}

void SQLStorage::generateQuery(const QString & name, const QSqlDatabase & db){
   m_Queries[name]->generateQuery(db);
}
void SQLStorage::executeQuery(const QString &name){
   m_Queries[name]->executeQuery();
}

bool SQLStorage::addParam(const QStringList & param, const qint32 & count, const qint32 & id){
   SQLParameter * tmp = new SQLParameter(param, count);
   m_Parameters[id] = tmp;
   //first version, in future possibly return false based on errors
   return true;
}
