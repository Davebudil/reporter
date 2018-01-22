#include "sqlquery.h"
#include "log.h"

SQLquery::SQLquery(const QString & query,
                   const QString & name,
                   const QString & param,
                   bool master,
                   bool active)
   : m_Query(query),
     m_Name(name),
     m_mParameter(param),
     m_masterfinalString(query),
     m_finalString(query),
     m_Result(nullptr),
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
QString SQLquery::getResultString(){
   QString tmp = m_finalString;
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
QString SQLquery::getMasterFinal(){
   return m_masterfinalString;
}
bool SQLquery::getIsMaster(){
   return m_Master;
}
bool SQLquery::getActive(){
   return m_isActive;
}
//Gets the number of result rows
qint32 SQLquery::getQueryResultRows(){
   return m_Result->size();
}
//Setter for query member
void SQLquery::setQuery(const QString & query){
   m_finalString = query;
   m_Query = query;
}
void SQLquery::setName(const QString &name){
   m_Name = name;
}
void SQLquery::setParam(const QString & param){
   m_mParameter = param;
}
void SQLquery::setMasterFinal(const QString & finalString){
   m_masterfinalString = finalString;
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
   if(m_Master){
      //      qDebug() << "Execute Classic"; //debug
      m_Result->setForwardOnly(true);
      m_Result->prepare(m_finalString);
   }else{
      //      qDebug() << "Execute Final"; //debug
      m_Result->setForwardOnly(true);
      m_Result->prepare(m_masterfinalString);
   }
}
//executes set query and checks for validity of SQL syntax
void SQLquery::executeQuery(){
   m_Result->exec();
   if(!m_Result->isActive()){
      qWarning(logWarning()) << "SQL error" + m_Result->lastError().text();
      QMessageBox::warning(0, QObject::tr("SQL error."), m_Result->lastError().text());
      m_Result->clear();
   }
}

QStringList SQLquery::queryList(){
   QStringList queryData;
   queryData.append(m_Name);
   queryData.append(m_mParameter);
   queryData.append(m_finalString);
   return queryData;
}
//Binds parameter to value
void SQLquery::bindParameter(const QString & parameter, const QString & value){
   m_finalString.replace(QString(parameter), QString(value));
}
