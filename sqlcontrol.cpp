#include "sqlcontrol.h"

//Contructor
SQLControl::SQLControl(QObject *parent)
   : QObject(parent){
}
//Destructor
SQLControl::~SQLControl(){
   delete m_queryModel;
}

//Setter
void SQLControl::setQueryModel(const QString & name){
   m_queryModel = new QSqlQueryModel;
   m_queryModel->setQuery(m_Storage.getQueries()[name]->getResult());
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

QStringList SQLControl::loadList(){
   QStringList tmpQueries;
   for(auto it: m_Storage.getQueries()){
      tmpQueries.append(it->getQuery());
      tmpQueries.append(it->getName());
      tmpQueries.append(it->getParam());
   }
   return tmpQueries;
}