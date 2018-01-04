#include "sqlquery.h"

SQLquery::SQLquery(const QString & query,
                   const QString & name,
                   const QString & param,
                   bool master,
                   bool active)
   : m_Query(query),
     m_Name(name),
     m_mParameter(param),
     m_Result(nullptr),
     m_nResult(nullptr),
     m_Master(master),
     m_isActive(active){
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

QSqlQuery SQLquery::getResultQuery(){
   return *m_nResult;
}
QString SQLquery::getnResult(){
   QString tmp = m_nResult->lastQuery();
   tmp.replace(QRegExp("[\\n\\t\\r]"), " ");
   return tmp;
}
QString SQLquery::getName(){
   return m_Name;
}
QString SQLquery::getParam(){
   return m_mParameter;
}
QString SQLquery::getFinal(){
   return m_finalString;
}
bool SQLquery::getIsMaster(){
   return m_Master;
}
bool SQLquery::getActive(){
   return m_isActive;
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
void SQLquery::setFinal(const QString & finalString){
   m_finalString = finalString;
}
void SQLquery::setActive(bool active){
   m_isActive = active;
}
//Debug function
void SQLquery::printValue(){
   qDebug() << "Name:" << m_Name;
   qDebug() << "Query:" << m_Query;
   qDebug() << "FinalQuery:" << m_finalString;
   qDebug() << "MasterBool:" << m_Master;
}
//generates query result
void SQLquery::generateQuery(const QSqlDatabase & db){
   m_Result = new QSqlQuery(db);
   m_nResult = new QSqlQuery(db);
   if(m_Master){
      //      qDebug() << "Execute Classic"; //debug
      m_Result->setForwardOnly(true);
      m_nResult->setForwardOnly(true);
      m_Result->prepare(m_Query);
      m_nResult->prepare(m_Query);
   }else{
      //      qDebug() << "Execute Final"; //debug
      m_Result->setForwardOnly(true);
      m_nResult->setForwardOnly(true);
      m_Result->prepare(m_finalString);
      m_nResult->prepare(m_Query);
   }
}
//executes set query and checks for validity of SQL syntax
void SQLquery::executeQuery(){
   m_Result->exec();
   m_nResult->exec();
   if(!m_Result->isActive()){
      QMessageBox::critical(0, QObject::tr("SQL error"), m_Result->lastError().text());
      m_Result->clear();
   }
}

QStringList SQLquery::queryList(){
   QStringList queryData;
   queryData.append(m_Name);
   queryData.append(m_mParameter);
   queryData.append(m_nResult->lastQuery());
   return queryData;
}
//Binds parameter to value
void SQLquery::bindParameter(const QString & parameter, const QString & value){
   m_Result->bindValue(parameter, value);
   m_nResult->bindValue(parameter, value);
}
//Gets the number of result rows
qint32 SQLquery::getQueryResultRows(){
   return m_Result->size();
}
