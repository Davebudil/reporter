#include "sqlcontrol.h"
#include "log.h"

//Contructor
SQLControl::SQLControl(QObject *parent)
   : QObject(parent){
}
//Destructor
SQLControl::~SQLControl(){}
//Setter
void SQLControl::setQueryModel(const QString & name,
                               const QMap<QString, QString> & parameters,
                               const QDateTime & from,
                               const QDateTime & to){
   m_queryModel = QSharedPointer<QSqlQueryModel>::create();
   QSqlQuery displayResult(m_DB.m_createDatabaseConnection());
   //TODO maybe look into masterQuery???.

   //limited to 10000 query results, improves performance drastically
   if(parameters.isEmpty()){
      displayResult.prepare(QString(m_Storage.getQueries()[name]->getOriginalQuery()/* + " LIMIT 10000"*/) );
   }else{
      QString parametrizedQuery;
      parametrizedQuery = m_Storage.getQueries()[name]->getOriginalQuery();

      for(auto & it : parameters){
         parametrizedQuery.replace(parameters.key(it), it);
      }

      parametrizedQuery.replace("#TIMEFROM", "'#TIMEFROM'");
      parametrizedQuery.replace("#TIMETO", "'#TIMETO'");

      parametrizedQuery.replace("#TIMEFROM", from.toString("yyyy-MM-dd hh:mm:ss"));
      parametrizedQuery.replace("#TIMETO", to.toString("yyyy-MM-dd hh:mm:ss"));

      qInfo(logInfo()) << "Parametrized Query: " + parametrizedQuery;

      displayResult.prepare(parametrizedQuery);
   }

   displayResult.exec();
   if(!displayResult.isActive()){
      emit modelFailed(displayResult.lastError().text());
      return;
   }else{
      m_queryModel->setQuery(displayResult);
      m_queryModel->blockSignals(true);
      emit modelChanged();
      return;
   }
}

void SQLControl::startQueryModelThread(const QString & name,
                                       const QMap<QString, QString> & parameters,
                                       const QDateTime & from,
                                       const QDateTime & to){
   if(m_Future.isFinished()){
      m_Future = QtConcurrent::run(this, &SQLControl::setQueryModel, name, parameters, from, to);
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

QSharedPointer<QSqlQueryModel> SQLControl::getQueryModel() const{
   return m_queryModel;
}

QFuture<void> SQLControl::getFuture() const{
   return m_Future;
}

void SQLControl::setFuture(const QFuture<void> & Future){
   m_Future = Future;
}
