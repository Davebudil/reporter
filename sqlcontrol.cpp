#include "sqlcontrol.h"
#include "log.h"

//Contructor
SQLControl::SQLControl(QObject *parent)
   : QObject(parent){
}
//Destructor
SQLControl::~SQLControl(){}
//Setter
void SQLControl::setQueryModel(const QString & name){
   m_queryModel = QSharedPointer<QSqlQueryModel>::create();
   QSqlQuery displayResult(m_DB.getDatabase());
//   if(m_Storage.getQueries()[name]->getIsMaster()){
//      //FIX MASTER QUERY and delete the generating below
//   }else{
////      displayResult.prepare(name);
////      displayResult.setForwardOnly(true);
////      displayResult.exec();
//   }

   //limited to 10000 query results, improves performance drastically
   displayResult.prepare(QString(m_Storage.getQueries()[name]->getOriginalQuery()/* + " LIMIT 10000"*/) );
   displayResult.exec();
   if(!displayResult.isActive()){
      QMessageBox::critical(nullptr, "Query Error", displayResult.lastError().text());
   }
   //not sure why, but the display table seems to be bugged, this is a workaround
//   if(m_Storage.getQueries()[name]->getIsMaster()){
//      m_queryModel->setQuery(m_Storage.getQueries()[name]->getFinal(), m_DB.getDatabase());
//   }else{
//      m_queryModel->setQuery(m_Storage.getQueries()[name]->getResult());
//   }

//   QObject::connect(m_Result.data(), SIGNAL(&QAbstractProxyModel::dataChanged()), this, SLOT(&SQLControl::modelChanged()));
   m_queryModel->setQuery(displayResult);
   m_queryModel->blockSignals(true);
//   m_Result = QSharedPointer<QSortFilterProxyModel>::create();
//   m_Result.data()->setDynamicSortFilter(true);
//   m_Result->setSourceModel(m_queryModel.data());
   emit modelChanged();
   return;
}

void SQLControl::startQueryModelThread(const QString & name){
   if(m_Future.isFinished()){
      m_Future = QtConcurrent::run(this, &SQLControl::setQueryModel, name);
   }
}
//Getters
QString SQLControl::getPassword(){
   return m_DB.getDatabaseConnector().getPassword();
}
QSharedPointer<QSqlQueryModel> SQLControl::getModel() const{
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
      tmpQueries.append(it->getOriginalQuery());
      tmpQueries.append(it->getName());
      tmpQueries.append(it->getMasterQueryName());
      tmpQueries.append(QString::number(it->getIsActive()));
   }
   return tmpQueries;
}
//adds time parameters to string
//void SQLControl::setTimeParameters(const QDate &from, const QDate &to, const QString &queryName){
//   m_Storage.getQueries()[queryName]->bindParameter(":TIMEFROM",from.toString());
//   m_Storage.getQueries()[queryName]->bindParameter(":TIMETO", to.toString());
//}

QSharedPointer<QSortFilterProxyModel> SQLControl::getResult() const{
   return m_Result;
}

QSharedPointer<QSqlQueryModel> SQLControl::getQueryModel() const
{
   return m_queryModel;
}
