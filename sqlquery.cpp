#include <utility>

#include "sqlquery.h"
#include "log.h"

SQLquery::SQLquery(const QString & query,
                   QString  name,
                   QString  param,
                   bool master,
                   bool active)
   : m_Query(query),
     m_Name(std::move(name)),
     m_mParameter(std::move(param)),
     m_masterfinalString(query),
     m_finalString(query),
     m_Result(nullptr),
     m_Master(master),
     m_isActive(active){
}
SQLquery::~SQLquery(){
//   Crashes app
//   if(m_Result){
//      delete m_Result;
//   }
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
   tmp.replace(QRegExp(R"([\n\t\r])"), " ");
   return tmp;
}
QString SQLquery::getName(){
   return m_Name;
}
QString & SQLquery::getParam(){
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
   return m_QueryResultRows;
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

void SQLquery::setIsMaster(bool master){
   m_Master = master;
}
//Debug function
void SQLquery::printValue(){
   qInfo(logInfo()) << "Name:" << m_Name;
   qInfo(logInfo()) << "Query:" << m_Query;
   qInfo(logInfo()) << "FinalQuery:" << m_finalString;
   qInfo(logInfo()) << "Master Query:" << m_masterQueryString;
}
//generates query result
void SQLquery::generateQuery(const QSqlDatabase & db){
   m_Result = new QSqlQuery(db);
   if(m_Master){
      //      qDebug() << "Execute Classic"; //debug
      m_Result->prepare(m_finalString);
   }else{
      //      qDebug() << "Execute Final"; //debug
      m_Result->prepare(m_masterfinalString);
   }
}
//executes set query and checks for validity of SQL syntax
void SQLquery::executeQuery(){
   m_Result->exec();
   if(!m_Result->isActive()){
      qWarning(logWarning()) << "SQL error" + m_Result->lastError().text();
      QMessageBox::warning(nullptr, QObject::tr("SQL error."), m_Result->lastError().text());
      m_Result->clear();
   }
   m_QueryResultRows = m_Result->size();
}

void SQLquery::forceExecuteQuery(){
   m_Result->exec();
}

void SQLquery::clearQueries(){
   m_Result->clear();
}

void SQLquery::finishQuery(){
   m_Result->finish();
}

QStringList SQLquery::queryList(){
   QStringList queryData;
   queryData.append(m_Name);
   queryData.append(m_mParameter);
   queryData.append(m_finalString);
   return queryData;
}

QString SQLquery::getMasterQueryString() const{
   return m_masterQueryString;
}

void SQLquery::setMasterQueryString(const QString & masterQueryString){
   m_masterQueryString = masterQueryString;
}

QString SQLquery::getCustomParametersString() const{
   return m_CustomParametersString;
}

void SQLquery::setCustomParametersString(const QString & CustomParametersString){
   m_CustomParametersString = CustomParametersString;
}

QSqlQuery * SQLquery::getCustomParameters() const{
   return m_CustomParameters;
}

void SQLquery::setCustomParameters(QSqlQuery * CustomParameters){
   m_CustomParameters = CustomParameters;
}

QSqlQuery * SQLquery::getParametrizedQuery() const{
   return m_ParametrizedQuery;
}

void SQLquery::setParametrizedQuery(QSqlQuery * ParametrizedQuery){
   m_ParametrizedQuery = ParametrizedQuery;
}

QString SQLquery::getParametrizedQueryString() const{
   return m_parametrizedQueryString;
}

void SQLquery::setParametrizedQueryString(const QString & parametrizedQueryString){
   m_parametrizedQueryString = parametrizedQueryString;
}

void SQLquery::setQueryResultRows(const qint32 & QueryResultRows){
   m_QueryResultRows = QueryResultRows;
}
//Binds parameter to value
void SQLquery::bindParameter(const QString & parameter, const QString & value){
   m_finalString.replace(QString(parameter), QString(value));
   m_masterfinalString.replace(QString(parameter), QString(value));
}

void SQLquery::bindCustomParameter(const QString & parameter, const QString & value){
   m_parametrizedQueryString.replace(QString(parameter), QString(value));
}
