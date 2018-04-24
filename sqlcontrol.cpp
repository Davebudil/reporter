#include "sqlcontrol.h"

//Contructor
SQLControl::SQLControl(QObject *parent)
   : QObject(parent),
     m_queryModel(nullptr),
     m_Result(nullptr){
}
//Destructor
SQLControl::~SQLControl(){
   delete m_queryModel;
   delete m_Result;
}
//Setter
void SQLControl::setQueryModel(const QString & name){
   if(m_queryModel){
      delete m_queryModel;
   }
   if(m_Result){
      delete m_Result;
   }

   m_queryModel = new QSqlQueryModel;
   //not sure why, but the display table seems to be bugged, this is workaround
   if(m_Storage.getQueries()[name]->getIsMaster()){
      m_queryModel->setQuery(m_Storage.getQueries()[name]->getFinal(), m_DB.getDatabase());
   }else{
      m_queryModel->setQuery(m_Storage.getQueries()[name]->getResult());
   }

   m_Result = new QSortFilterProxyModel(this);
   m_Result->setDynamicSortFilter(true);
   m_Result->setSourceModel(m_queryModel);
}
//Getters
QString SQLControl::getPassword(){
   return m_DB.getDatabaseConnector().getPassword();
}
QSqlQueryModel * SQLControl::getModel(){
   return m_queryModel;
}

SQLStorage & SQLControl::getStorage(){
   return m_Storage;
}

Database & SQLControl::getDatabase(){
   return m_DB;
}
//Loads query data to stringlist
QStringList SQLControl::loadList(){
   QStringList tmpQueries;
   for(auto it: m_Storage.getQueries()){
      tmpQueries.append(it->getQuery());
      tmpQueries.append(it->getName());
      tmpQueries.append(it->getParam());
      tmpQueries.append(QString::number(it->getActive()));
   }
   return tmpQueries;
}
//adds time parameters to string
void SQLControl::setTimeParameters(const QDate &from, const QDate &to, const QString &queryName){
   m_Storage.getQueries()[queryName]->bindParameter(":TIMEFROM",from.toString());
   m_Storage.getQueries()[queryName]->bindParameter(":TIMETO", to.toString());
}

QSortFilterProxyModel * SQLControl::getResult(){
   return m_Result;
}

void SQLControl::setResult(QSortFilterProxyModel * Result){
   m_Result = Result;
}
