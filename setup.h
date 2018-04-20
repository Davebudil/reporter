#ifndef SETUP_H
#define SETUP_H

#include <QObject>
#include <QSettings>
#include <QDataStream>
#include <QFile>
#include <QStringList>
#include <QDebug>
#include <QDir>
#include <QList>
#include <QSettings>
#include <QDate>
#include <QApplication>
/*Class used to load and edit ini file
 * and to serialize and deserialize queries data
 * */

struct SPreferences{
   QString databaseType;
   QString host;
   qint32 port;
   QString databaseName;
   QString userName;
   QString userPassword;
   QString hotKey;
   qint32 timerInterval;
   qint32 customInterval;
   QString generatedByUser;
};

class Setup : public QObject{
      Q_OBJECT
   public:
      //Constructor
      explicit Setup(QObject *parent = nullptr);
      //Destructor
      ~Setup() override;
      //.ini file
      void loadIni();
      void saveIni();
      //serializes
      bool serializeQueries(const QStringList & queries);
      bool serializeParameters(const QStringList & parameters, const QVector<qint32> & count);
      bool serializeSchedule(const QList<QStringList> & serializeData, const QStringList & scheduleName);
      bool serializeGlobal(const QStringList & global);
      //deserializes
      bool deserializeQueries(QStringList & queries);
      bool deserializeParameters(QStringList & parameters, QVector<qint32> & count);
      bool deserializeSchedule(QList<QStringList> & deserializeData,
                               QStringList & scheduleName,
                               QMap<QString, QVector<QStringList>> & parameters);
      bool deserializeGlobal(QStringList & global);
      //cleans up after applicaton
      bool cleanUp();
      //setter
      void setFilePath(const QString & path);
      //getter
      QString getFilePath() const;
      SPreferences getSettings();

   signals:

   public slots:

   private:
      void loadSettings();
      void saveSettings();
      bool m_serializeQueries(const QStringList & queries);
      bool m_deserializeQueries(QStringList & queries);
      bool m_serializeParameters(const QStringList & parameters, const QVector<qint32> & count);
      bool m_deserializeParameters(QStringList & parameters, QVector<qint32> & count);
      QString m_filePath;
      SPreferences m_Settings;
};

#endif // SETUP_H
