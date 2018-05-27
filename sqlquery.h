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
      explicit SQLquery(const QString & queryName,
                        const QString & masterName,
                        const QString & queryText,
                        const bool & isMaster,
                        const bool & isActive);
      ~SQLquery();
      //Getter for query member

      bool getIsMaster() const;
      void setIsMaster(bool isMaster);

      bool getIsActive() const;
      void setIsActive(bool isActive);

      QString & getMasterQuery();
      void setMasterQuery(const QString & masterQuery);

      QString & getOriginalQuery();
      void setOriginalQuery(const QString & originalQuery);

      QString getMasterQueryName() const;
      void setMasterQueryName(const QString & masterQueryName);

      QString getName() const;
      void setName(const QString & Name);

      void printQueryData() const;

   private:
      QString m_Name;
      //master query name if exists
      QString m_masterQueryName;
      //use this if no master
      QString m_originalQuery;
      //use this if master
      QString m_masterQuery;
      //indicates states
      //if is active
      bool m_isActive;
      //if is master
      bool m_isMaster;
};

#endif // SQLQUERY_H
