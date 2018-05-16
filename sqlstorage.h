#ifndef SQLSTORAGE_H
#define SQLSTORAGE_H

#include <QObject>
#include <QtSql>
#include <QSharedPointer>
#include "sqlquery.h"
#include "exportsqlquery.h"
/* SQLStorage class
 * used to manage storage of queries
 * and parameters
*/
//PREPSAT SQLQUERY, asi oddelit testovani a provadeni query
class SQLStorage : public QObject{
      Q_OBJECT
   public:
      //Constructor
      explicit SQLStorage(QObject *parent = nullptr);
      ~SQLStorage() override;
      //Used to get list of queries member
      QMap<QString, SQLquery*> & getQueries();
      QMap<QString, QSharedPointer<ExportSQLQueryData> > getExportQueries() const;
      //used to get list of parameters member
      //getter
      QSqlQuery getResultQuery();
      QQueue<SQLquery> getQueueQueries();
      bool addQuery(const QString & query, const QString & name, const QString & param, bool active, bool display, bool mode);
      bool masterQuery(const QString & detail, const QString & master);
      void setParameter(const QString & parameter, const QString & value, const QString & queryName);
      void printQueries();
      void generateQuery(const QString & name, const QSqlDatabase & db);
      void executeQuery(const QString & name);
      void printQueryText();
      void fixMaster();

   signals:

   public slots:

   private:
      //list used to store queries
      QMap<QString, SQLquery*> m_Queries;
      //      QMap<QString, QSharedPointer<SQLquery>> m_Queries;
      QMap<QString, QSharedPointer<ExportSQLQueryData>> m_ExportQueries;
      //query used during SQL
      QSqlQuery m_Query;
};

#endif // SQLSTORAGE_H
