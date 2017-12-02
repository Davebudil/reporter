#ifndef SQLQUERY_H
#define SQLQUERY_H

#include <QDebug>
#include <QtSql>
#include <QMessageBox>
/*Class SQLquery
 *used to store and manage query
*/

class SQLquery{
   public:
      //constuctor
      explicit SQLquery(const QString & query, const QString & name, const QString & param);
      ~SQLquery();
      //Getter for query member
      QString getQuery();
      QSqlQuery getResult();
      QString getName();
      QString getParam();
      //Setters for members
      void setQuery(const QString & query);
      void setName(const QString & name);
      void setParam(const QString & param);
      //print for debuggin
      void printValue();
      //Generate
      void generateQuery(const QSqlDatabase & db);
      //binds time Parameters
      void bindParameter(const QString & parameter, const QString & value);
      //function that executes the query
      void executeQuery();

   private:
      //query
      QString m_Query;
      QString m_Name;
      QString m_mParameter;
      QSqlQuery * m_Result;
      bool m_Active;
};

#endif // SQLQUERY_H