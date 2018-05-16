#ifndef EXPORTSQLQUERY_H
#define EXPORTSQLQUERY_H

#include <QObject>
#include <QSharedDataPointer>

class ExportSQLQueryData;

class ExportSQLQuery : public QObject
{
      Q_OBJECT
   public:
      explicit ExportSQLQuery(QObject *parent = nullptr,
                              const QString & queryName = "",
                              const QString & masterName = "",
                              const QString & queryText = "",
                              const bool & isMaster = false,
                              const bool & isActive = false);
      ExportSQLQuery(const ExportSQLQuery &);
      ExportSQLQuery &operator=(const ExportSQLQuery &);
      ~ExportSQLQuery();

      QString getName() const;
      void setName(const QString & Name);

      QString getMasterQueryName() const;
      void setMasterQueryName(const QString & masterQueryName);

      QString getOriginalQuery() const;
      void setOriginalQuery(const QString & originalQuery);

      QString getMasterQuery() const;
      void setMasterQuery(const QString & masterQuery);

      bool getIsActive() const;
      void setIsActive(bool isActive);

      bool getIsMaster() const;
      void setIsMaster(bool isMaster);

   signals:

   public slots:

   private:
      //date
      QSharedDataPointer<ExportSQLQueryData> data;
      //name
      QString m_Name;
      //master query name if exists
      QString m_masterQueryName;
      //use this if no master
      QString m_originalQuery;
      //use this if master
      QString m_masterQuery;
      //parametrized final string, test if its needed, uses original or master depending on state
      QString m_parametrizedQuery;

      //indicates states
      //if is active
      bool m_isActive;
      //if is master
      bool m_isMaster;
};

#endif // EXPORTSQLQUERY_H
