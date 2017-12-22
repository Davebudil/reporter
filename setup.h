#ifndef SETUP_H
#define SETUP_H

#include <QObject>
#include <QSettings>
#include <QDataStream>
#include <QFile>
#include <QStringList>
#include <QDebug>
/*Class used to load and edit ini file
 * and to serialize and deserialize queries data
 * */
class Setup : public QObject{
      Q_OBJECT
   public:
      //Constructor
      explicit Setup(QObject *parent = nullptr);
      //Destructor
      ~Setup();
      //.ini file
      bool loadIni();
      bool saveIni();
      //serializes
      bool serializeQueries(const QStringList & queries);
      bool serializeParameters(const QStringList & parameters, const QVector<qint32> & count);
      bool serializeSchedule(const QStringList & shift,
                             const QStringList & day,
                             const QStringList & weekly,
                             const QStringList & monthly);
      bool serializeGlobal(const QStringList & global);
      //deserializes
      bool deserializeQueries(QStringList & queries);
      bool deserializeParameters(QStringList & parameters, QVector<qint32> & count);
      bool deserializeSchedule(QStringList & shift,
                               QStringList & day,
                               QStringList & weekly,
                               QStringList & monthly);
      bool deserializeGlobal(QStringList & global);
      //cleans up after applicaton
      bool cleanUp();
      //setter
      void setFilePath(const QString & path);
      //getter
      QString getFilePath() const;

   signals:

   public slots:

   private:
      bool m_serializeQueries(const QStringList & queries);
      bool m_deserializeQueries(QStringList & queries);
      bool m_serializeParameters(const QStringList & parameters, const QVector<qint32> & count);
      bool m_deserializeParameters(QStringList & parameters, QVector<qint32> & count);
      QString m_filePath;

};

#endif // SETUP_H
