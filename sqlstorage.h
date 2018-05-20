#ifndef SQLSTORAGE_H
#define SQLSTORAGE_H

#include <QObject>
#include <QtSql>
#include <QSharedPointer>
#include "sqlquery.h"
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
      QMap<QString, QSharedPointer<SQLquery> > getQueries() const;
      //used to get list of parameters member
      //getter
      QQueue<SQLquery> getQueueQueries();
      bool addQuery(const QString & query, const QString & name, const QString & param, bool active, bool display, bool mode);
      void printQueries();
      void printQueryText();

   signals:

   public slots:

   private:
      //list used to store queries
      QMap<QString, QSharedPointer<SQLquery>> m_Queries;
};

#endif // SQLSTORAGE_H
