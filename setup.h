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
      //loads .ini settings file
      bool loadIni();
      //serializes
      bool serializeQueries(const QStringList & queries);
      bool serializeParameters(const QStringList & parameters, const QVector<qint32> & count);
      //deserializes
      bool deserializeQueries(QStringList & queries);
      bool deserializeParameters(QStringList & parameters, QVector<qint32> & count);
      //cleans up after applicaton
      bool cleanUp();
      //setter
      void setFilePath(const QString & path);
      //getter
      QString getFilePath();

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
