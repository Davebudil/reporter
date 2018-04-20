#ifndef SQLSTORAGE_H
#define SQLSTORAGE_H

#include <QObject>
#include <QtSql>
#include <sqlquery.h>
/* SQLStorage class
 * used to manage storage of queries
 * and parameters
*/
class SQLStorage : public QObject{
      Q_OBJECT
   public:
      //Constructor
      explicit SQLStorage(QObject *parent = nullptr);
      ~SQLStorage() override;
      //Used to get list of queries member
      QMap<QString, SQLquery*> & getQueries();
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
   signals:

   public slots:

   private:
      //list used to store queries
      QMap<QString, SQLquery*> m_Queries;
      //query used during SQL
      QSqlQuery m_Query;
};

#endif // SQLSTORAGE_H
