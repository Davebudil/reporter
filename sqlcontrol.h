#ifndef SQLCONTROL_H
#define SQLCONTROL_H

#include <QObject>
#include <QtSql>
#include <QTableView>
#include "database.h"
#include "sqlstorage.h"

class SQLControl : public QObject
{
      Q_OBJECT
   public:
      //Constructor
      explicit SQLControl(QObject *parent = nullptr);
      //Destructor
      ~SQLControl();
      //Getters
      QString getPassword();
      QSqlQueryModel * getModel();
      Database & getDatabase();
      SQLStorage & getStorage();
      //loads list of queries info
      QStringList loadList();
      //Sets model to display the query result in table
      void setQueryModel(const QString & name);

   signals:

   public slots:

   private:
      Database m_DB;
      SQLStorage m_Storage;
      QSqlQueryModel * m_queryModel;
};

#endif // SQLCONTROL_H
