#ifndef DATABASECONNECTOR_H
#define DATABASECONNECTOR_H

#include <QSqlDatabase>
#include <QObject>
#include <QSql>
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
                   const qint32 port,
                   const QString & dbName,
                   const QString & usrName,
                   const QString & usrPass);
      //getter
      QString getPassword() const;
   signals:

   public slots:

   private:
      qint32 m_port;
      QString m_dbType;
      QString m_hostName;
      QString m_dbName;
      QString m_usrName;
      QString m_usrPasswrd;
};

#endif // DATABASECONNECTOR_H
