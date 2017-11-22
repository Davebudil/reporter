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
      //Used to get list of queries member
      QMap<QString, SQLquery*> getQueries();
      //used to get list of parameters member
      QList<SQLParameter> getParameters();
      //getter
      QSqlQuery getResultQuery();
      bool addQuery(const QString & query, const QString & name, const QString & param, bool display);
      void printQueries();
      void generateQuery(const QString & name);

   signals:

   public slots:

   private:
      //used to load queries
      bool m_loadQueries();
      //used to load parameters
      bool m_loadParameters();
      //list used to store queries
      QMap<QString, SQLquery*> m_Queries;
      //param used to store parameters
      QList<SQLParameter> m_Parameters;
      //query used during SQL
      QSqlQuery m_Query;
};

#endif // SQLSTORAGE_H
