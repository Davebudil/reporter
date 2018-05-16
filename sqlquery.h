#ifndef SQLQUERY_H
#define SQLQUERY_H

#include <QDebug>
#include <QtSql>
#include <QMessageBox>
/*Class SQLquery
 *used to store and manage query
*/
//PREPSAT, asi to staci mit jako uloziste a budu se zde starat jen praci se stringy,
//dosazovani budu resit jinde, executovani, generovani a ostatni funkcionalitu budu resit jinde

class SQLquery{
   public:
      //constuctor
      explicit SQLquery(const QString & query,
                        QString  name,
                        QString  param,
                        bool master,
                        bool active);
      ~SQLquery();
      //Getter for query member
      QString getQuery();
      QSqlQuery getResult();
      QString getResultString();
      QString getName();
      QString & getParam();
      QString getFinal();
      QString getMasterFinal();
      qint32 getQueryResultRows();
      bool getIsMaster();
      bool getActive();
      //Setters for members
      void setQuery(const QString & query);
      void setName(const QString & name);
      void setParam(const QString & param);
      void setMasterFinal(const QString & finalString);
      void setParamString(const QString & paramString);
      void setQueryResultRows(const qint32 & QueryResultRows);
      void setActive(bool active);
      void setIsMaster(bool master);
      //print for debugging
      void printValue();
      //Generate
      void generateQuery(const QSqlDatabase & db);
      //binds time Parameters
      void bindParameter(const QString & parameter, const QString & value);
      void bindCustomParameter(const QString & parameter, const QString & value);
      //function that executes the query
      void executeQuery();
      void executeParametrizedQuery();
      void forceExecuteQuery();
      void clearQueries();
      void finishQuery();
      void finishParemetrizedQuery();
      //
      QStringList queryList();
      QString getMasterQueryString() const;
      void setMasterQueryString(const QString & masterQueryString);

      QString getCustomParametersString() const;
      void setCustomParametersString(const QString & CustomParametersString);

      QSqlQuery * getCustomParameters() const;
      void setCustomParameters(QSqlQuery * CustomParameters);

      QSqlQuery * getParametrizedQuery() const;
      void setParametrizedQuery(QSqlQuery * ParametrizedQuery);

      QString getParametrizedQueryString() const;
      void setParametrizedQueryString(const QString & parametrizedQueryString);

   private:
      //original text
      QString m_Query;
      //names
      QString m_Name;
      //master name
      QString m_mParameter;
      //final edited query text with master
      QString m_masterfinalString;
      //final string with no master
      QString m_finalString;
      QString m_masterQueryString;
      QString m_CustomParametersString;
      QString m_parametrizedQueryString;
      //Final result
      QSqlQuery * m_Result;
      QSqlQuery * m_CustomParameters;
      QSqlQuery * m_ParametrizedQuery;
      //master/detail
      bool m_Master;
      //is Active
      bool m_isActive;
      qint32 m_QueryResultRows;
};

#endif // SQLQUERY_H
