#ifndef SQLCONTROL_H
#define SQLCONTROL_H

#include <QObject>
#include <QtSql>
#include <QTableView>
#include "database.h"
#include "sqlstorage.h"
#include <QSortFilterProxyModel>
#include <QSharedPointer>
#include <QtConcurrent>
#include <QMap>
#include <QDateTime>
#include <QFuture>

class SQLControl : public QObject{
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
      void setQueryModel(const QString & name,
                         const QMap<QString, QString> & parameters,
                         const QDateTime & from,
                         const QDateTime & to);
      void startQueryModelThread(const QString & name,
                                 const QMap<QString, QString> & parameters,
                                 const QDateTime & from,
                                 const QDateTime & to);

      QSharedPointer<QSortFilterProxyModel> getResult() const;

      QSharedPointer<QSqlQueryModel> getQueryModel() const;

      QFuture<void> getFuture() const;
      void setFuture(const QFuture<void> & Future);

   public slots:
      //      void modelChanged();

   signals:
      void modelChanged();
      void modelFailed(QString errorName);

   private:
      Database m_DB;
      QFuture<void> m_Future;
      SQLStorage m_Storage;
      QSharedPointer<QSqlQueryModel> m_queryModel;
      QSharedPointer<QSortFilterProxyModel> m_Result;
};

#endif // SQLCONTROL_H
