#ifndef SQLCONTROL_H
#define SQLCONTROL_H

#include <QObject>
#include <QtSql>
#include <QTableView>
#include "database.h"
#include "sqlstorage.h"
#include <QSortFilterProxyModel>
#include <QSharedPointer>

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
      QSharedPointer<QSqlQueryModel> getModel() const;
      Database & getDatabase();
      SQLStorage & getStorage();
      //loads list of queries info
      QStringList loadList();
      //Sets model to display the query result in table
      void setQueryModel(const QString & name);

      QSharedPointer<QSortFilterProxyModel> getQueryModel() const;
      QSharedPointer<QSortFilterProxyModel> getResult() const;

   private:
      Database m_DB;
      SQLStorage m_Storage;
      QSharedPointer<QSqlQueryModel> m_queryModel;
      QSharedPointer<QSortFilterProxyModel> m_Result;
};

#endif // SQLCONTROL_H
