#ifndef SQLCONTROL_H
#define SQLCONTROL_H

#include <QObject>
#include <QtSql>
#include <QTableView>
#include "database.h"
#include "sqlstorage.h"
#include <QSortFilterProxyModel>

class SQLControl : public QObject
{
      Q_OBJECT
   public:
      //Constructor
      explicit SQLControl(QObject *parent = nullptr);
      //Destructor
      ~SQLControl() override;
      //Getters
      QString getPassword();
      QSqlQueryModel * getModel();
      Database & getDatabase();
      SQLStorage & getStorage();
      //loads list of queries info
      QStringList loadList();
      //Sets model to display the query result in table
      void setQueryModel(const QString & name);
      void setTimeParameters(const QDate & from, const QDate & to, const QString & queryName);

      QSortFilterProxyModel * getQueryModel();

      QSortFilterProxyModel * getResult();
      void setResult(QSortFilterProxyModel * Result);

   private:
      Database m_DB;
      SQLStorage m_Storage;
      QSqlQueryModel * m_queryModel;
      QSortFilterProxyModel * m_Result;
};

#endif // SQLCONTROL_H
