#include "sqlstorage.h"
#include "log.h"
//Constructor
SQLStorage::SQLStorage(QObject *parent)
   : QObject(parent){
}

SQLStorage::~SQLStorage(){
   qDeleteAll(m_Queries);
   qDeleteAll(m_Parameters);
   m_Queries.clear();
   m_Parameters.clear();
}
//Getters
QMap<QString, SQLquery*> & SQLStorage::getQueries(){
   return m_Queries;
}

QMap<qint32, SQLParameter*> & SQLStorage::getParameters(){
   return m_Parameters;
}

QSqlQuery SQLStorage::getResultQuery(){
   return m_Query;
}

bool SQLStorage::addQuery(const QString & query,
                          const QString & name,
                          const QString & param,
                          bool active,
                          bool display,
                          bool mode){
   SQLquery * tmp = new SQLquery(query, name, param, (param.isEmpty()), active);
   if(m_Queries.contains(name)){
      if(display){
         QMessageBox::warning(0,QObject::tr("New Query Error"), "Query with this name already exists.");
         return false;
      }
      return false;
   }
   if(((!param.isEmpty()) && (!m_Queries.contains(param))) && mode){
      QMessageBox::warning(0,QObject::tr("New Query Error"), "No query with specified master param name exists.");
      return false;
   }
   m_Queries[name] = tmp;
   if(m_Queries[name]->getIsMaster() && mode){
      masterQuery(name, tmp->getParam());
   }
   return true;
}

bool SQLStorage::masterQuery(const QString & detail, const QString & master){
   if(m_Queries[detail]->getIsMaster()){
      return false;
   }else{
      QString detailQuery;
      QString masterQuery;
      QString tmp;
      QString tempDetail;
      qint32 index;
      detailQuery = m_Queries[detail]->getFinal();
      masterQuery = m_Queries[master]->getFinal();
      index = detailQuery.lastIndexOf("WHERE", -1, Qt::CaseInsensitive) -1;
      tempDetail = "JOIN\n";
      tempDetail += '(' + QString(masterQuery);
      tempDetail += ") AS T2\n";
      detailQuery.insert(index + 1, tempDetail);
      index = detailQuery.lastIndexOf("AS T2\nWHERE", -1, Qt::CaseInsensitive) + 11;
      tmp = detailQuery.mid(index);
      tmp.replace(":","T2.");
      detailQuery.remove(index, 111);
      detailQuery.insert(index, tmp);
      if((index = masterQuery.lastIndexOf("\nORDER BY ")) != -1){
         tmp = masterQuery.mid(index + 1);
         detailQuery += '\n';
         detailQuery.append(tmp);
      }
      m_Queries[detail]->setMasterFinal(detailQuery);
      return true;
   }
}
void SQLStorage::setParameter(const QString & parameter, const QString & value, const QString & queryName){
   m_Queries[queryName]->bindParameter(parameter, value);
}

void SQLStorage::printQueries(){
   for(auto & it : m_Queries){
      it->printValue();
   }
}

void SQLStorage::printQueryText(){
   for(auto & it : m_Queries){
      qDebug() << it->getFinal();
   }
}

void SQLStorage::printParams(){
   for(auto & it : m_Parameters){
      it->printParams();
   }
}
void SQLStorage::generateQuery(const QString & name, const QSqlDatabase & db){
   m_Queries[name]->generateQuery(db);
}
void SQLStorage::executeQuery(const QString &name){
   m_Queries[name]->executeQuery();
}

bool SQLStorage::addParam(const QStringList & param, const qint32 & count, const qint32 & id){
   SQLParameter * tmp = new SQLParameter(param, count);
   m_Parameters[id] = tmp;
   //first version, in future possibly return false based on errors
   return true;
}
