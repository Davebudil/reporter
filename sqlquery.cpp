#include "sqlquery.h"

SQLquery::SQLquery(const QString & query,
                   const QString & name,
                   const QString & param)
   : m_Query(query),
     m_Name(name),
     m_mParameter(param){

}
SQLquery::~SQLquery(){
   delete m_Result;
}
//Getter for query member
QString SQLquery::getQuery(){
   return m_Query;
}
QSqlQuery SQLquery::getResult(){
   return *m_Result;
}
QString SQLquery::getName(){
   return m_Name;
}
QString SQLquery::getParam(){
   return m_mParameter;
}
//Setter for query member
void SQLquery::setQuery(const QString & query){
   m_Query = query;
}
void SQLquery::setName(const QString &name){
   m_Name = name;
}
void SQLquery::setParam(const QString & param){
   m_mParameter = param;
}
//Debug function
void SQLquery::printValue(){
   qDebug() << "Name:" << m_Name;
   qDebug() << "Query:" << m_Query;
}
//generates query result
bool SQLquery::generateQuery(const QSqlDatabase & db){
   m_Result = new QSqlQuery(db);
   m_Result->exec(m_Query);
   if(!m_Result->isActive()){
      QMessageBox::critical(0, QObject::tr("SQL error"), m_Result->lastError().text());
      m_Result->clear();
      return false;
   }else{
      return true;
   }


}
