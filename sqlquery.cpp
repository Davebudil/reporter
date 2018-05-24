#include <utility>
#include "sqlquery.h"
#include "log.h"

SQLquery::SQLquery(const QString & queryName,
                   const QString & masterName,
                   const QString & queryText,
                   const bool & isMaster,
                   const bool & isActive)
   : m_Name(queryName),
     m_masterQueryName(masterName),
     m_originalQuery(queryText),
     m_isActive(isActive),
     m_isMaster(isMaster){

}

SQLquery::~SQLquery(){}

bool SQLquery::getIsMaster() const{
   return m_isMaster;
}

void SQLquery::setIsMaster(bool isMaster){
   m_isMaster = isMaster;
}

bool SQLquery::getIsActive() const{
   return m_isActive;
}

void SQLquery::setIsActive(bool isActive){
   m_isActive = isActive;
}

QString SQLquery::getMasterQuery() const{
   return m_masterQuery;
}

void SQLquery::setMasterQuery(const QString & masterQuery){
   m_masterQuery = masterQuery;
}

QString SQLquery::getOriginalQuery() const{
   return m_originalQuery;
}

void SQLquery::setOriginalQuery(const QString & originalQuery){
   m_originalQuery = originalQuery;
}

QString SQLquery::getMasterQueryName() const{
   return m_masterQueryName;
}

void SQLquery::setMasterQueryName(const QString & masterQueryName){
   m_masterQueryName = masterQueryName;
}

QString SQLquery::getName() const{
   return m_Name;
}

void SQLquery::setName(const QString & Name){
   m_Name = Name;
}

void SQLquery::printQueryData() const{
   qInfo(logInfo()) << "-----------------------------------";
   qInfo(logInfo()) << "Name: " + m_Name;
   qInfo(logInfo()) << "Master: " + QString(m_masterQueryName);
   qInfo(logInfo()) << "Master Query: " + QString(m_masterQuery);
   qInfo(logInfo()) << "-----------------------------------";
}
//Getter for query member
//QString SQLquery::getResultString(){
//   QString tmp = m_finalString;
//   tmp.replace(QRegExp(R"([\n\t\r])"), " ");
//   return tmp;
//}

