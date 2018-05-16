#include "sqlstorage.h"
#include "log.h"
//Constructor
SQLStorage::SQLStorage(QObject *parent)
   : QObject(parent){
}

SQLStorage::~SQLStorage(){
   qDeleteAll(m_Queries);
   m_Queries.clear();
}
//Getters
QMap<QString, SQLquery*> & SQLStorage::getQueries(){
   return m_Queries;
}
QSqlQuery SQLStorage::getResultQuery(){
   return m_Query;
}

QQueue<SQLquery> SQLStorage::getQueueQueries(){
   QQueue<SQLquery> tmp;
   for(auto & it : m_Queries){
      tmp.append(*it);
   }
   return tmp;
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
         QMessageBox::warning(nullptr, QObject::tr("New Query Error"), "Query with this name already exists.");
         return false;
      }
      return false;
   }
   if(((!param.isEmpty()) && (!m_Queries.contains(param))) && mode){
      QMessageBox::warning(nullptr, QObject::tr("New Query Error"), "No query with specified master param name exists.");
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

void SQLStorage::fixMaster(){
   for(auto & it : m_Queries){
      for(auto & it2 : m_Queries){
         if(it != it2){
            if(!it->getParam().isEmpty()){
               it->setMasterQueryString(m_Queries[it->getParam()]->getFinal());
            }else{
               it->setMasterQueryString("");
            }
         }
      }
   }
}

QMap<QString, QSharedPointer<ExportSQLQueryData> > SQLStorage::getExportQueries() const{
   return m_ExportQueries;
}


void SQLStorage::generateQuery(const QString & name, const QSqlDatabase & db){
   m_Queries[name]->generateQuery(db);
}
void SQLStorage::executeQuery(const QString &name){
   m_Queries[name]->executeQuery();
}
