#include "sqlstorage.h"
#include "log.h"
//Constructor
SQLStorage::SQLStorage(QObject *parent)
   : QObject(parent){
}

SQLStorage::~SQLStorage(){}
//Getters
QQueue<SQLquery> SQLStorage::getQueueQueries(){
   QQueue<SQLquery> tmp;
   for(auto & it : m_Queries){
      //copy only active queries
      if(it->getIsActive()){
         tmp.append(*it);
      }
   }
   return tmp;
}

bool SQLStorage::addQuery(const QString & query,
                          const QString & name,
                          const QString & param,
                          bool active,
                          bool display,
                          bool mode){
   auto tmpQueryData = QSharedPointer<SQLquery>::create(name, param, query, param.isEmpty(), active);
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
   m_Queries[name] = tmpQueryData;
   return true;
}
//TODO: COPY THIS
//bool SQLStorage::masterQuery(const QString & detail, const QString & master){
//   if(m_Queries[detail]->getIsMaster()){
//      return false;
//   }else{
//      QString detailQuery;
//      QString masterQuery;
//      QString tmp;
//      QString tempDetail;
//      qint32 index;
//      detailQuery = m_Queries[detail]->getFinal();
//      masterQuery = m_Queries[master]->getFinal();
//      index = detailQuery.lastIndexOf("WHERE", -1, Qt::CaseInsensitive) -1;
//      tempDetail = "JOIN\n";
//      tempDetail += '(' + QString(masterQuery);
//      tempDetail += ") AS T2\n";
//      detailQuery.insert(index + 1, tempDetail);
//      index = detailQuery.lastIndexOf("AS T2\nWHERE", -1, Qt::CaseInsensitive) + 11;
//      tmp = detailQuery.mid(index);
//      tmp.replace(":","T2.");
//      detailQuery.remove(index, 111);
//      detailQuery.insert(index, tmp);
//      if((index = masterQuery.lastIndexOf("\nORDER BY ")) != -1){
//         tmp = masterQuery.mid(index + 1);
//         detailQuery += '\n';
//         detailQuery.append(tmp);
//      }
//      m_Queries[detail]->setMasterFinal(detailQuery);
//      return true;
//   }
//}
void SQLStorage::printQueries(){
   for(const auto & it : m_Queries){
      qInfo(logInfo()) << it->getName();
   }
}

QMap<QString, QSharedPointer<SQLquery>> & SQLStorage::getQueries(){
   return m_Queries;
}

