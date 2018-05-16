#include "exportsqlquery.h"

class ExportSQLQueryData : public QSharedData{
   public:
};


ExportSQLQuery::ExportSQLQuery(QObject * parent,
                               const QString & queryName,
                               const QString & masterName,
                               const QString & queryText,
                               const bool & isMaster,
                               const bool & isActive)
                               : QObject(parent),
                                 data(new ExportSQLQueryData),
                                 m_Name(queryName),
                                 m_masterQueryName(masterName),
                                 m_originalQuery(queryText),
                                 m_isActive(isActive),
                                 m_isMaster(isMaster){

}

ExportSQLQuery::ExportSQLQuery(const ExportSQLQuery &rhs) : data(rhs.data){

}

ExportSQLQuery &ExportSQLQuery::operator=(const ExportSQLQuery &rhs){
   if (this != &rhs){
      data.operator=(rhs.data);
   }
   return *this;
}

ExportSQLQuery::~ExportSQLQuery(){

}

QString ExportSQLQuery::getName() const
{
   return m_Name;
}

void ExportSQLQuery::setName(const QString & Name)
{
   m_Name = Name;
}

QString ExportSQLQuery::getMasterQueryName() const
{
   return m_masterQueryName;
}

void ExportSQLQuery::setMasterQueryName(const QString & masterQueryName)
{
   m_masterQueryName = masterQueryName;
}

QString ExportSQLQuery::getOriginalQuery() const
{
   return m_originalQuery;
}

void ExportSQLQuery::setOriginalQuery(const QString & originalQuery)
{
   m_originalQuery = originalQuery;
}

QString ExportSQLQuery::getMasterQuery() const
{
   return m_masterQuery;
}

void ExportSQLQuery::setMasterQuery(const QString & masterQuery)
{
   m_masterQuery = masterQuery;
}

bool ExportSQLQuery::getIsActive() const
{
   return m_isActive;
}

void ExportSQLQuery::setIsActive(bool isActive)
{
   m_isActive = isActive;
}

bool ExportSQLQuery::getIsMaster() const
{
   return m_isMaster;
}

void ExportSQLQuery::setIsMaster(bool isMaster)
{
   m_isMaster = isMaster;
}
