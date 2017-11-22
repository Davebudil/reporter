#ifndef DATABASECONNECTOR_H
#define DATABASECONNECTOR_H

#include <QObject>
#include <QSqlDatabase>
#include <QSql>
#include <QSqlError>
#include <QMessageBox>

class DatabaseConnector : public QObject{
      Q_OBJECT
   public:
      //Constructor
      explicit DatabaseConnector(QObject *parent = nullptr);
      //Connects to DB
      bool connectDB(QSqlDatabase & db);
      //Sets info
      void setInfo(const QString & dbType,
                   const QString & host,
                   const int port,
                   const QString & dbName,
                   const QString & usrName,
                   const QString & usrPass);
      //getter
      QString getPassword();
   signals:

   public slots:

   private:
      QString m_dbType;
      QString m_hostName;
      int m_port;
      QString m_dbName;
      QString m_usrName;
      QString m_usrPasswrd;
};

#endif // DATABASECONNECTOR_H
