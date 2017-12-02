#ifndef SQLSTORAGE_H
#define SQLSTORAGE_H

#include <QObject>
#include <QtSql>
#include <sqlquery.h>
#include <sqlparameter.h>
/* SQLStorage class
 * used to manage storage of queries
 * and parameters
*/
class SQLStorage : public QObject{
      Q_OBJECT
   public:
      //Constructor
      explicit SQLStorage(QObject *parent = nullptr);
      ~SQLStorage();
      //Used to get list of queries member
      QMap<QString, SQLquery*> & getQueries();
      //used to get list of parameters member
      QMap<qint32, SQLParameter*>  & getParameters();
      //getter
      QSqlQuery getResultQuery();
      bool addQuery(const QString & query, const QString & name, const QString & param, bool display);
      bool addParam(const QStringList & param, const qint32 & count, const qint32 & id);
      void printQueries();
      void printParams();
      void generateQuery(const QString & name, const QSqlDatabase & db);
      void executeQuery(const QString & name);
   signals:

   public slots:

   private:
      //list used to store queries
      QMap<QString, SQLquery*> m_Queries;
      //param used to store parameters
      QMap<qint32, SQLParameter*> m_Parameters;
      //query used during SQL
      QSqlQuery m_Query;
};

#endif // SQLSTORAGE_H
