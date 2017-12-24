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
      explicit SQLquery(const QString & query,
                        const QString & name,
                        const QString & param,
                        bool master,
                        bool active);
      ~SQLquery();
      //Getter for query member
      QString getQuery();
      QSqlQuery getResult();
      QString getnResult();
      QString getName();
      QString getParam();
      QString getFinal();
      qint32 getQueryResultRows();
      bool getIsMaster();
      bool getActive();
      //Setters for members
      void setQuery(const QString & query);
      void setName(const QString & name);
      void setParam(const QString & param);
      void setFinal(const QString & finalString);
      void setActive(bool active);
      //print for debugging
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
      QString m_finalString;
      QSqlQuery * m_Result;
      QSqlQuery * m_nResult;
      bool m_Master;
      bool m_isActive;
};

#endif // SQLQUERY_H
